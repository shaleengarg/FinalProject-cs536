#include "shim.h"
#include "XML.h"

#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <iostream>
#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <dirent.h>

#include "XML.h"

using namespace std;

#define TRACK // ENABLE TRACKING

//The thread name is a meaningful C language string, whose length is restricted to 16 characters, including the terminating null byte.
#define MAX_LENGTH_PTHREAD_NAME 16

typedef unordered_map<string, int> statmap;
typedef unordered_map<string, statmap> opmap;
typedef unordered_map<string, opmap> filemap;

pthread_mutex_t names_lock;
pthread_mutex_t maps_lock;
unordered_map<pid_t, string> threadnames = {};
unordered_map<pid_t, filemap*> threadmaps = {};
bool capture_stopped = true;

thread_local bool new_thread = true;
thread_local filemap* files = NULL;

void start_tracing(){
	capture_stopped = false;
}
void stop_tracing(){
	capture_stopped = true;
}

void inc_file_op_filename(string filename, string op, string stat, int value){
#ifdef TRACK
	
	// Check if we are capturing
	if(capture_stopped){
		return;
	}

	if(new_thread){
		pid_t tid = syscall(SYS_gettid);
		
		files = new filemap();

		char thread_name[MAX_LENGTH_PTHREAD_NAME] = {0};
		int get;

		get = pthread_getname_np(pthread_self(), thread_name, MAX_LENGTH_PTHREAD_NAME);		
	
		if(get != 0){
			printf("Error: Unable to get thread name");
		}

		// We should probably check here if tid exists already 
		// if tid does not match upwith thread_local semantics 

		// Check if already exists 
		pthread_mutex_lock(&names_lock);
		threadnames[tid] = thread_name;
		pthread_mutex_unlock(&names_lock);

		pthread_mutex_lock(&maps_lock);
		threadmaps[tid] = files;
		pthread_mutex_unlock(&maps_lock);
		
		new_thread = false;
	}
	
	// Update statistic
	(*files)[filename][op][stat] += value;

#endif
}

void inc_file_op_fd(int fd, string op, string stat, int value){
	inc_file_op_filename(filename_via_fd(fd), op, stat, value);
}

void inc_file_op_file(FILE *stream, string op, string stat, int value){
	inc_file_op_filename(filename_via_file(stream), op, stat, value);
}

void print_files(){
	//printf("Printing files\n");

	/*	
	map<pid_t, unordered_map<string, unordered_map<string, unordered_map<string, int>>>> threads_sorted(threads.begin(), threads.end());

	printf("PID: %d\n", getpid());

	for (auto thread : threads_sorted){
		printf("TID: %d (%s)\n", thread.first, threadnames[thread.first].c_str());
		//cout << thread.first << "\n";
		
		map<string, unordered_map<string, unordered_map<string, int>>> files_sorted(thread.second.begin(), thread.second.end());
		for(auto file : files_sorted){
			printf("    %s\n", file.first.c_str());
			
			map<string, unordered_map<string, int>> ops_sorted(file.second.begin(), file.second.end());

			for (auto operation : ops_sorted){
				printf("        %s\n", operation.first.c_str());

				map<string, int> stats_sorted(operation.second.begin(), operation.second.end());

				for(auto stat : stats_sorted){
					printf("            %s:%d\n", stat.first.c_str(), stat.second);
				}

			}
		}	
	}
	*/

}

void export_process_stats(){
	pid_t pid = getpid();
	string export_prefix = get_xml_export_prefix();
	string export_directory = get_xml_export_directory();
	string export_path = export_directory + "/" + export_prefix + "_pid-" + to_string(pid) + ".xml" ;
	string ignores_path  = export_directory + "/ignore-list_pid-" + to_string(pid) + ".txt"; 

	fstream ignore_file;
	ignore_file.open(ignores_path, fstream::trunc | fstream::out);

	if(export_prefix == ""){
		//printf("[STATS EXPORT] Export path (EXPORT_PATH) NOT found. Skipping...\n");
		return;
	}
	
	//printf("[STATS EXPORT] Export path found. Going to export stats to XML file %s\n", export_path.c_str());
	//printf("[STATS EXPORT] Export path set to %s\n", export_path.c_str());
	
	XMLWriter writer(export_path);

	writer.startElement("process");
	writer.writeAttribute("id", to_string(pid));
	writer.writeAttribute("name", get_process_name(pid));
	writer.writeAttribute("command", get_process_command(pid));

	map<pid_t, filemap*> threads_sorted(threadmaps.begin(), threadmaps.end());	
	writer.startElement("threads");
	for (auto thread : threads_sorted){
		writer.startElement("thread"); // start thread
		writer.writeAttribute("id", to_string(thread.first));
		writer.writeAttribute("name", threadnames[thread.first]);
		map<string, opmap> files_sorted(threadmaps[thread.first]->begin(), threadmaps[thread.first]->end());
		writer.startElement("files"); // start files
		for(auto file : files_sorted){

			// check if we should ignore file, if so add to ignore file and move onto next file
			if(should_we_ignore(file.first)){
				ignore_file << file.first + "\n";
				continue;
			}

			writer.startElement("file"); // start file
			writer.writeAttribute("name", file.first);
			map<string, statmap> ops_sorted(file.second.begin(), file.second.end());
			for (auto operation : ops_sorted){	
				writer.startElement("operation"); // start operation 
				writer.writeAttribute("name", operation.first);
				map<string, int> stats_sorted(operation.second.begin(), operation.second.end());
				for(auto stat : stats_sorted){
					writer.startElement("stat"); // start stat
					writer.writeAttribute("name", stat.first);
					writer.writeElementValue(to_string(stat.second));
					writer.endElementInline(); // end stat 
				}
				writer.endElement(); // end operation
			}
			writer.endElement(); // end file
		}
		writer.endElement(); // end files
		writer.endElement(); // end thread

		delete threadmaps[thread.first]; // delete allocated threadmap
		threadmaps[thread.first] = NULL;

	}
	writer.endElement(); // end threads
	writer.endElement(); // end process	
	writer.close();

	ignore_file.close(); // close ignore file
}

set<string> get_ignored_files(){
	string export_dir = get_xml_export_directory();
	string ignores_prefix = "ignore-list";

	set<string> ignored_set;

	if(export_dir == ""){
		//printf("Failed to read export directory for igored files\n");
		return ignored_set;
	}

	fstream infile; 
	DIR *dp = NULL;
	struct dirent *entry;
	string entryname;
	string line;
	string fullpath;
	dp = opendir(export_dir.c_str());
	if(dp != NULL){
		while ((entry = readdir(dp))){
			entryname.assign(entry->d_name);
			if(entryname.rfind(ignores_prefix) == 0){
				fullpath = export_dir + "/" + entryname;
				infile.open(fullpath, fstream::in);
				while(getline(infile, line)){
					ignored_set.insert(line);
				}
				infile.close();
			}
		}
	}else{
		printf("Could not open directory\n");
	}
	closedir(dp);

	return ignored_set;
}

void print_ignored_files(){
	set<string> ignored_set = get_ignored_files();

	if(ignored_set.empty()){
		return;
	}

	if(ignored_set.size() > 20){
		printf("[CASH] Large number of files were ignored (%lu files). Check XML export for the whole list of files.\n", ignored_set.size());
		return;
	}

	// Print them out 
	for(auto filename: ignored_set){
		printf("[CASH] Ignored file %s\n", filename.c_str());
	}
	printf("[CASH] Total # files ignored: %lu\n", ignored_set.size());	

}

void delete_ignored_file_lists(){
	string export_dir = get_xml_export_directory();
	string ignores_prefix = "ignore-list";

	set<string> ignored_set;

	if(export_dir == ""){
		//printf("Failed to read export directory for igored files\n");
		return;
	}

	fstream infile; 
	DIR *dp = NULL;
	struct dirent *entry;
	string entryname;
	string line;
	string fullpath;
	dp = opendir(export_dir.c_str());
	if(dp != NULL){
		while ((entry = readdir(dp))){
			entryname.assign(entry->d_name);
			if(entryname.rfind(ignores_prefix) == 0){
				fullpath = export_dir + "/" + entryname;
				remove(fullpath.c_str());
			}
		}
	}else{
		printf("Could not open directory\n");
	}
	closedir(dp);
}

void aggregate_stats(){
	string export_path = get_xml_export_path();
	string export_dir = get_xml_export_directory();
	string export_prefix = get_xml_export_prefix();

	// Check if export exists
	if(export_path == ""){
		printf("[CASH] Export path (EXPORT_PATH) NOT found. Skipping XML Export.\n");
		return;
	}

	printf("[CASH] Exporting statistics to %s\n", export_path.c_str());

	//printf("Aggregating files\n");
	//printf("Export path %s\n", export_path.c_str());
	//printf("Export directory is %s\n", export_dir.c_str());
	//printf("Export prefix is %s\n", export_prefix.c_str());

	XMLWriter output_file(export_path);

	output_file.startElement("run");
	output_file.writeAttribute("name", get_run_name());
	output_file.writeAttribute("description", get_run_description());	
	output_file.startElement("processes");
	
	for(string path: get_intermediate_file_paths()){
		//printf("Aggregating %s\n", path.c_str());
		output_file.addXMLFile(path, 2);
		if (remove(path.c_str()) != 0)
			printf("Unable to delete the file %s\n", path.c_str());
	}

	output_file.endElement(); // end processes
	
	set<string> ignored_files = get_ignored_files();
	
	// add ignored files
	output_file.startElement("ignored_files");
	for(auto file : ignored_files){
		output_file.startElement("ignored_file");
		output_file.writeAttribute("name", file);
		output_file.endElementInline();
	}
	output_file.endElement();
	
	
	output_file.endElement(); // end run
	output_file.close(); // close 

	printf("[CASH] Sucessfully exported statistics.\n");
}
