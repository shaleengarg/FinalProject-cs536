#include "shim.h"
#include <string>

using namespace std;

#define MAX_LENGTH_PROCESS_NAME 256
#define MAX_LENGTH_COMMAND 1024

void print_filename_fd(int fd){
	int MAXSIZE = 0xFFF;
	char proclnk[0xFFF];
	char filename[0xFFF];
	int fno = fd;
	ssize_t r;
	sprintf(proclnk, "/proc/self/fd/%d", fno);
	r = readlink(proclnk, filename, MAXSIZE);
	if (r < 0){
		debug("failed to readlink\n");
	}
	filename[r] = '\0';
	debug("%s", filename);

}

void print_filename_file(FILE *stream){
	print_filename_fd(fileno(stream));
}

void print_info_fd(int fd){
	int MAXSIZE = 0xFFF;
	char proclnk[0xFFF];
	char filename[0xFFF];
	int fno = fd;
	ssize_t r;
	sprintf(proclnk, "/proc/self/fd/%d", fno);
	r = readlink(proclnk, filename, MAXSIZE);
	if (r < 0){
		debug("failed to readlink\n");
	}
	filename[r] = '\0';
	debug("\t(fd=%d)\t(fp=--------------)\t%s\n", fd, filename);
}


void print_info_file(FILE *stream){
	print_info_fd(fileno(stream));
}

string filename_via_fd(int fd){
	int MAXSIZE = 0xFFF;
	char proclnk[0xFFF];
	char filename[0xFFF];
	ssize_t r;
	sprintf(proclnk, "/proc/self/fd/%d", fd);
	r = readlink(proclnk, filename, MAXSIZE);
	if (r < 0){
		debug("failed to readlink\n");
	}
	filename[r] = '\0';
	return string(filename);
}

string filename_via_file(FILE *stream){
	return filename_via_fd(fileno(stream));
}


string get_process_name(const pid_t pid) {
	char procfile[MAX_LENGTH_PROCESS_NAME];
	char name[MAX_LENGTH_PROCESS_NAME];
	
	sprintf(procfile, "/proc/%d/cmdline", pid);
	
	FILE* f = real_fopen(procfile, "r");
	if (f) {
		size_t size;
		size = real_fread(name, sizeof (char), sizeof (procfile), f);
		if (size > 0) {
			if ('\n' == name[size - 1])
				name[size - 1] = '\0';
		}
		real_fclose(f);												
	}
	
	string pname_string(name);
	return pname_string;
}


string get_process_command(const pid_t pid) {
	char procfile[MAX_LENGTH_COMMAND];
	char cmd[MAX_LENGTH_COMMAND];
	sprintf(procfile, "/proc/%d/cmdline", pid);
	FILE* f = real_fopen(procfile, "r");
	size_t size = 0;	
	if (f) {
		size = real_fread(cmd, sizeof (char), sizeof (procfile), f);
		if (size > 0) {
			if ('\n' == cmd[size - 1])
				cmd[size - 1] = '\0';

			// 1. replace null terminators with spaces
			// 2. replace any quotes with single quotes 
			//    as XML file export uses double quotes
			for(unsigned int x = 0; x < size; x++){
				if(cmd[x] == '\0'){cmd[x] = ' ';}
				if(cmd[x] == '\"'){cmd[x] = '\'';}
			}
			
			// maintain last null terminator 
			cmd[size - 1] = '\0';
		}
		real_fclose(f);			
	}

	if(size > 0){
		string cmdstring(cmd);
		return cmd;
	}
	return "";
}

pid_t getgppid(){
	char buf[128];
	
	pid_t ppid= getppid(); 

	pid_t gppid; 

	FILE *fp;

	sprintf(buf, "/proc/%d/stat", (int) ppid); 

	fp = fopen(buf, "r");
	if(fp == NULL){
		return -1;
	}
	
	fscanf(fp, "%*d %*s %*s %d", &gppid);
	fclose(fp);

	return gppid;
}


bool file_exists(const std::string& name) {
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

