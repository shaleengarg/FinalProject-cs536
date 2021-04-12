#include "shim.h"

#include <unordered_set>

#define MAXFDS 1024 
#define MAXPATH 1024

unordered_set<string> ignored_names;

static char ignore_fd_bitmap[MAXFDS/8] = {0};


void ignore_path(string path){
	// compare against not absolute path as sometimes there are files with sys not /sys
	if(path.find("/") == 0){
		ignored_names.insert(path.substr(1));
		return;
	}	
	ignored_names.insert(path);

}

// Should be used when opening a file that is under a certain namepsace 
// (ex. /dev/* or /proc/*)
// returns 1 if yes we should ignore or 0 if not
// should only be called upon open
int should_we_ignore(const char *path){
	// should check before
	// than add fd to ignore
	return should_we_ignore(string(path));
}

int should_we_ignore(string path){

	int index;	
	for(auto ignorepath: ignored_names){
		//printf("Checking %s against %s\n", path.c_str(), ignorepath.c_str());
		//if(path.rfind(ignorepath) == 0){
		index = path.find(ignorepath);

		if(index == 0 || index == 1){
			//printf("Ignoring %s\n", path.c_str());
			return 1;
		}
	}

	return 0;
}

// Check if we ignored this fd
// returns 1 if we did, else 0
int ignored_fd(int fd){

	if ((ignore_fd_bitmap[fd/8] >> (fd % 8)) & 0x1){
		debug("[IGNORE] Ignoring %s\n", filename_via_fd(fd).c_str());
		debug("\n");
	}

	return (ignore_fd_bitmap[fd/8] >> (fd % 8)) & 0x1;
}

int ignored_file(FILE *stream){
	return ignored_fd(fileno(stream));
}

int ignored_filename(const char *path){
	// ignore /dev/* /proc/* and /sys/*	
		
	string pstring(path);

	for(auto path: ignored_names){
		if(path.rfind(pstring) == 0){
			debug("[IGNORE] Ignoring %s\n", path);
			return 1;
		}
	}
	
	return 0;
}

// Ignore this fd
void ignore_fd(int fd){

	debug("[IGNORE] Setting file ");
	print_filename_fd(fd);
	debug(" to be ignored\n");

	// set fd bit
	ignore_fd_bitmap[fd/8] |= 0x1 << (fd % 8);

	// fetch the name of file using fd
	//char * filename = fd_get_filename(fd);	

	// add file name to list
	//add_filename(filename);	
}

void ignore_file(FILE *stream){
	// call ignore_fd with FILE fd
	ignore_fd(fileno(stream));
}

// Unignore a partifular file descriptor
// Should be called upon close

// Assumption: has been checked to be ignored prior to being called
void unignore_fd(int fd){	
	debug("[IGNORE] Unignoring file ");
	print_filename_fd(fd);
	debug("\n");
		
	// unset fd bit
	ignore_fd_bitmap[fd/8] &= ~(0x1 << (fd % 8));
}

void unignore_file(FILE *stream){
	// call unignore_fd_ with FILE fd
	unignore_fd(fileno(stream));
}


