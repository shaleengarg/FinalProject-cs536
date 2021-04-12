#include <dirent.h>

#include <string>
#include <list>

#include "shim.h"

string get_debug_output_path(){
	char *debug_output = getenv("DEBUG_OUTPUT");
	if(debug_output != NULL){
		return string(debug_output);
	}
	return "";
}

string get_xml_export_path(){
  char *export_env = getenv("XML_EXPORT");
  if(export_env != NULL){
    return string(export_env);
  }
  return "";
}

string get_xml_export_directory(){
  string xml_path = get_xml_export_path();
  if(xml_path != ""){
    const size_t last_slash_idx = xml_path.rfind('/');
    if (string::npos != last_slash_idx){
          return xml_path.substr(0, last_slash_idx);
    }
    return "/";
  }
  return "";
}

string get_xml_export_name(){
  string xml_path = get_xml_export_path();
  if(xml_path != ""){
    const size_t last_slash_idx = xml_path.rfind('/');
    if (string::npos != last_slash_idx){
          return xml_path.substr(last_slash_idx + 1);	
    }
    return "";
  }
  return "";

}

string get_xml_export_prefix(){
  string xml_path = get_xml_export_path();
  if(xml_path != ""){
    const size_t last_slash_idx = xml_path.rfind('/');
    if (string::npos != last_slash_idx){
          return xml_path.substr(last_slash_idx + 1);	
    }
    return "";
  }
  return "";
}

string get_run_name(){
  char *run_name_env = getenv("RUN_NAME");
  if (run_name_env != NULL){
    return string(run_name_env);
  }
  return "";
}

string get_run_description(){
  char *run_desc_env = getenv("RUN_DESCRIPTION");
  if (run_desc_env != NULL){
	  return string(run_desc_env);
  }
  return "";
}

list<string> get_intermediate_file_paths(){
	string export_dir = get_xml_export_directory();
	string export_name = get_xml_export_name();
	string export_prefix = get_xml_export_prefix();
	
	list<string> ret;
	
	if(export_dir == ""){
		return ret;
	}
	
	DIR *dp = NULL;
	struct dirent *entry;
	string entryname;
	dp = opendir(export_dir.c_str());
	if (dp != NULL) {
		while ((entry = readdir(dp))){
			entryname.assign(entry->d_name);
			if(entryname.rfind(export_prefix) == 0 && entryname != export_name){	
				ret.push_back(export_dir + "/" + entryname);
			}
		}
	}else{
		printf("Could not open directory\n");
	}
	closedir(dp);
	
	return ret;
}


string get_target_gppid(){
  char *gppid_env = getenv("TARGET_GPPID");
  if (gppid_env != NULL){
    return string(gppid_env);
  }
  return "";
}

bool is_root_process(){
	char *gppid_env = getenv("TARGET_GPPID");

	if(gppid_env == NULL){
		// Cant aggregate
		printf("TARGET_GPPID not set, can not try and aggregate automatically\n");
		return false;
	}

	if(getgppid() == atoi(gppid_env)){
		return true;
	}else{
		return false;	
	}
	
}

