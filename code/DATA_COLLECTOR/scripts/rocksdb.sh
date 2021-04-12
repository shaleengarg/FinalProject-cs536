#!/bin/bash

# RocksDB Script
#  - clone
#  - compile
#  - compile-debug
#  - clean
#  - remove
#
# Requires the follow enviroment vartiables to be set:
#  1. ROCKSDB_PATH


ROCKSDB_GIT_URL=https://github.com/facebook/rocksdb.git


rocksdb_debug () {
	echo "[ROCKSDB.sh] $1"
}


rocksdb_error () {
	echo "[ROCKSDB.sh] (ERROR) $1"
}


rocksdb_print_usage () {
	echo "Usage:"
	echo " - clone   (clones the RocksDB git repo)"
	echo " - compile (compiles RocksDB)"
	echo " - clean   (clean RocksDB)"
	echo " - remove  (removes RocksDB repo)"
}


rocksdb_check_env () {
	if [ -z "$ROCKSDB_PATH" ]; then
		rocksdb_error "ROCKSDB_PATH environment variable is undefined. Can not continue"
		exit 1
	fi
}


rocksdb_clone () {

	# Ensure gflags
	rocksdb_debug "Ensure gflags is installed"
	sudo apt-get install -y libgflags2.2 libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev


	# Check if rocksdb repo already exists
	ls $ROCKSDB_PATH &> /dev/null
	if [ $? -ne 0 ]; then
		rocksdb_debug "Going to clone RocksDB repo"
	else
		rocksdb_error "Seems like RockDB already has been cloned"
		exit 1
	fi

	# Clone the repo
	rocksdb_debug "Cloning RocksDB repo"
	git clone $ROCKSDB_GIT_URL $ROCKSDB_PATH
	if [ $? -eq 0 ]; then 
		rocksdb_debug "Sucessfully cloned RocksDB repo"
	else
		rocksdb_error "Failed to clone RocksDB repo"
		exit 1
	fi

}


rocksdb_compile () {

	# get number of cores for gcc
	#numcores=$(grep -c ^processor /proc/cpuinfo)

	# Move to rocksdb directory and compile
	pushd $ROCKSDB_PATH
	make clean
	DEBUG_LEVEL=0 make db_bench -j$(nproc)
	popd

}

rocksdb_compile_debug () {

	# get number of cores for gcc
	#numcores=$(grep -c ^processor /proc/cpuinfo)

	# Move to rocksdb directory and compile
	pushd $ROCKSDB_PATH
	make clean
	DEBUG_LEVEL=1 make db_bench -j$(nproc)
	popd

}

rocksdb_clean () {
	rocksdb_debug "Cleaning RocksDB"
	pushd $ROCKSDB_PATH
	make clean
	popd
}

rocksdb_remove () {
	rocksdb_debug "Removing RocksDB"
	rm -rf $ROCKSDB_PATH
}


rocksdb_check_env

if [ $# -eq 1 ]; then
	if [ $1 == "clone" ]; then
		rocksdb_clone
	elif [ $1 == "compile" ]; then
		rocksdb_compile
	elif [ $1 == "compile-debug" ]; then
		rocksdb_compile_debug
	elif [ $1 == "clean" ]; then
		rocksdb_clean
	elif [ $1 == "remove" ]; then
		rocksdb_remove
	else
		rocksdb_error "Unknown command"
		rocksdb_print_usage
	fi
else
	rocksdb_error "Incorrect # of arguments"
	rocksdb_print_usage
fi
