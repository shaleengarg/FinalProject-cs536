#include <dirent.h>

#include <fstream>
#include <string>
#include <set>

#include "shim.h"


// Setup functions (setup.c)
static void con() __attribute__((constructor));
static void dest() __attribute__((destructor));

void clean_up_previous_results(){
	string export_path = get_xml_export_path();
	printf("[CASH] Removing previous results %s (if exists)\n", export_path.c_str());
	remove(export_path.c_str());
}

void print_environment(){
#ifdef DEBUG
		string debug_output = get_debug_output_path();
		if(debug_output != ""){
			printf("[CASH] Debug output found. Setting Debug Output to %s\n", debug_output.c_str());	
		}else{
			printf("[CASH] Debug output NOT set. Setting Debug output to STDERR\n");		
		}
#endif
		string export_path = get_xml_export_path();
		if(export_path != ""){
			printf("[CASH] Exporting statistics results to %s\n", export_path.c_str());
		}else{
			printf("[CASH] No export path specified. Skipping statistics export.\n");
		}
		
		string run_name = get_run_name();
		if(run_name != ""){
			printf("[CASH] Run name set to %s\n", run_name.c_str());
		}else{
			printf("[CASH] No run name specified\n");
		}
		
		
		string run_description = get_run_description();
		if(run_description != ""){
			printf("[CASH] Run description set to %s\n", run_description.c_str());
		}else{
			printf("[CASH] No run description specified\n");
		}
		
		string target_gppid = get_target_gppid();
		if(target_gppid != "" && target_gppid == ""){
			printf("[CASH] Target GPPID not set. Will have to aggregate export files manually\n");
		}
			
}

void con(){
	if(is_root_process()){
		printf("[CASH] Cleaning up previous results\n");
		clean_up_previous_results();
		printf("[CASH] Cleaning up any previous ignore lists\n");
		delete_ignored_file_lists();
		print_environment();
		printf("[CASH] Application Starting. Initiating tracing...\n");
	}

	ignore_path(get_xml_export_directory() + "/" + get_xml_export_prefix());
	ignore_path(get_xml_export_path());
	ignore_path(get_debug_output_path());
	ignore_path("/dev");
	ignore_path("/proc");
	ignore_path("/sys");
	
	//printf("[CASH] Initiating tracing...\n");
	link_real_functions();
	debug_setup();
	//debug("[CASH] Process Starting. Initiating tracing...\n");

	// Start tracing right before running application
	start_tracing();
}


void dest(){
	
	// Stop tracing before handling exports
	stop_tracing();

	debug("[CASH] Proccess Ended. Tracing terminating...\n");
	debug_breakdown();
	//print_files();
	export_process_stats();
	if(is_root_process()){
		printf("[CASH] Application ending. Tracing terminating...\n");
		print_ignored_files();	
		aggregate_stats();
		delete_ignored_file_lists();
	}
}


