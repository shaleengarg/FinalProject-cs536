#!/bin/bash

# Check if this script is being sourced
if [ $_ != $0 ]; then 
	: #echo "Setting necessary environment variables..."
else
	echo "This is a subshell, you need to run this as source"
	exit 1
fi

# Check if executing in correct position
stat $PWD/scripts/setvars.sh &> /dev/null
if [ $? -ne 0 ]; then
	echo "Not executing setvars from correct source. Run from project root directory."
	return
fi

echo "Setting necessary environment variables..."

set -x

# Project Paths
export CASH_HOME=$PWD
export SRC_PATH=$CASH_HOME/src
export APP_PATH=$CASH_HOME/applications
export SCRIPTS_PATH=$CASH_HOME/scripts

# Library paths
export SHIM=$SRC_PATH/shim.so

# RocksDB variables
export ROCKSDB_PATH=$APP_PATH/rocksdb
export ROCKSDB_DBPATH=$CASH_HOME/rocksdata


# APPBENCH variables
export APPBENCH_PATH=$APP_PATH/appbench
export APPBENCH=$APPBENCH_PATH # need this for appbench repo scripts
export SHARED_DATA=$APPBENCH_PATH/shared_data
export SHARED_LIBS=$APPBENCH_PATH/shared_libs
export OUTPUTDIR=$APPBENCH_PATH/output
export GRAPHCHI_ROOT=$APPBENCH_PATH/graphchi/graphchi-cpp
export APPPREFIX=""

export PRELOADPATH="$CASH_HOME/src/shim.so"

set +x
