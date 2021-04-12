#!/bin/bash

# AppBench Script
#  - clone
#  - compile
#  - remove
#
# Requires the follow enviroment vartiables to be set:
#  1. APPBENCH


APPBENCH_GIT_URL=https://github.com/sudarsunkannan/appbench.git


appbench_debug () {
	echo "[APPBENCH.sh] $1"
}


appbench_error () {
	echo "[APPBENCH.sh] (ERROR) $1"
}


appbench_print_usage () {
	echo "Usage:"
	echo " - clone   (clones the AppBench git repo)"
	echo " - compile (compiles AppBench Benchmarks)"
	echo " - remove  (removes AppBench repo)"
}


appbench_check_env () {
	if [ -z "$APPBENCH_PATH" ]; then
		appbench_error "APPBENCH_PATH environment variable is undefined. Can not continue"
		exit 1
	fi
}


appbench_clone () {


	# Check if appbench repo already exists
	ls $APPBENCH_PATH &> /dev/null
	if [ $? -ne 0 ]; then
		appbench_debug "Going to clone AppBench repo"
	else
		appbench_error "Seems like AppBench already has been cloned"
		exit 1
	fi

	# Clone the repo
	appbench_debug "Cloning AppBench repo"
	git clone $APPBENCH_GIT_URL $APPBENCH_PATH
	if [ $? -eq 0 ]; then 
		appbench_debug "Sucessfully cloned AppBench repo"
	else
		appbench_error "Failed to clone AppBench repo"
		exit 1
	fi

}


appbench_compile () {

	# get number of cores for gcc
	#numcores=$(grep -c ^processor /proc/cpuinfo)

	# Move to appbench directory and compile
	pushd $APPBENCH_PATH
	
	appbench_debug "Setting up AppBench..."
	./setup.sh 
	appbench_debug "Finished Setting up AppBench"
	popd	

	pushd $APPBENCH_PATH
	appbench_debug "Compiling AppBench Benchmarks..."
	./compile_all.sh
	appbench_debug "Finished Compiling AppBench Benchmarks"
	popd

}


appbench_remove () {
	appbench_debug "Removing AppBench"
	rm -rf $APPBENCH_PATH
}


appbench_check_env

if [ $# -eq 1 ]; then
	if [ $1 == "clone" ]; then
		appbench_clone
	elif [ $1 == "compile" ]; then
		appbench_compile
	elif [ $1 == "remove" ]; then
		appbench_remove
	else
		appbench_error "Unknown command"
		appbench_print_usage
	fi
else
	appbench_error "Incorrect # of arguments"
	appbench_print_usage
fi
