#include "shim.h"
#include <string>

using namespace std;

#ifdef DEBUG
static FILE *monitor = NULL;
static string monitor_path = "";
#endif

void debug(const char *format, ...){
#ifdef DEBUG
	va_list arglist;
	va_start(arglist, format);
	if(monitor == NULL){
		// No nothing
		//printf("Output is null. Should not get here (PID=%d)", getpid());
	}else{
		vfprintf(monitor, format, arglist); // pring to monitor
		
		// don't fflush stderr, else it will seg fault
		if(monitor != stderr){
			real_fflush(monitor); // flush write to monitor
		}
	}
	va_end(arglist);
#endif
}

void debug_setup(void){
#ifdef DEBUG	
  	// Check if monitor available. Is so open to it and set it:
	char *monitor_env = getenv("DEBUG_OUTPUT");
	if(monitor_env == NULL){
		monitor_path = "";
	}else{
		monitor_path = string(monitor_env);
	}
	if(monitor_path == ""){
  		//printf("[CASH] Debug output path (DEBUG_OUTPUT) NOT found, setting debug output to STDERR\n");	
  		monitor = stderr;
		monitor_path = "stderr";
  	}else{
		//printf("[CASH] Debug output path (DEBUG_OUTPUT) found, setting debug output to %s\n ", monitor_path.c_str());
		monitor = real_fopen(monitor_path.c_str(), "w");
		//const char *message = explain_fopen(monitor_path.c_str(), "w");
  		//printf("[CASH] Done explain fopen\n");
		//printf("[CASH] Opening output (%s)\n", message);
		if(monitor == NULL){
			printf("Monitor is null, stderr is %p\n", stderr);
  			printf( "[CASH] (ERROR) Failed to open output\n");
  			printf("[CASH] Terminating Program...\n");
  			exit(1);
  		}
  	}
#endif
}

void debug_breakdown(void){
      //printf("Breaking down Debugging\n");	
#ifdef DEBUG
  	if(monitor != stderr && monitor != NULL){
  		//printf("Closing monitor %s\n", monitor_path.c_str());
		real_fclose(monitor);
		monitor_path = "";
		monitor = NULL;
	}
#endif
}


