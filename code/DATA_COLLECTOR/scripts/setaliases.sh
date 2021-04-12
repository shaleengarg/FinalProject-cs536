#!/bin/bash

set -x

# alias to run with shim set
alias run="export TARGET_GPPID=$PPID; LD_PRELOAD=$SHIM"
alias xrun="$SCRIPTS_PATH/clear-cache.sh; export TARGET_GPPID=$PPID; LD_PRELOAD=$SHIM"

# set output environment variable
set_output (){
	set -x
	export DEBUG_OUTPUT=$(realpath $1)
	set +x
}
alias set-debug-output="set_output"
alias unset-debug-output="unset DEBUG_OUTPUT"

set_xml_output () {
	set -x
	export XML_EXPORT=$(realpath $1)
	set +x
}
alias set-xml-export="set_xml_output"
alias unset-xml-export="unset XML_EXPORT"

set_run_name () {
	set -x
	export RUN_NAME=$1 
	set +x
}
alias set-run-name="set_run_name"

set_run_description () {
	set -x 
	export RUN_DESCRIPTION=$1
	set +x
}
alias set-run-description="set_run_description"

alias xml-aggregate="$SCRIPTS_PATH/xml.sh aggregate"
alias xml-clean="$SCRIPTS_PATH/xml.sh clean"

# manually set shim on and off
alias shim-on="echo \"Turning Shim On\"; set -x; export LD_PRELOAD=$SHIM; set +x"
alias shim-off="echo \"Turning Shim Off\"; set -x; unset LD_PRELOAD; set +x"
alias shim-stat="echo \"Checking Shim Status\"; echo LD_PRELOAD=$LD_PRELOAD"

# Source shortcuts
alias compile="pushd $SRC_PATH; make; popd"
alias compile-debug="pushd $SRC_PATH; make USER_CFLAGS='-DDEBUG'; popd"
alias clean="pushd $SRC_PATH; make clean; popd"

# Experimental shortcuts
alias clear-cache='sudo $SCRIPTS_PATH/clear-cache.sh'
alias db_bench="$ROCKSDB_PATH/db_bench"
alias db_bench-clean="echo \"Removing $ROCKSDB_DBPATH\"; rm -rf $ROCKSDB_DBPATH"
alias db_bench-test='db_bench --db=$ROCKSDB_DBPATH --value_size=4096 --benchmarks=fillrandom,readrandom,fillseq,readseq --wal_dir=$ROCKSDB_DBPATH/WAL_LOG --sync=0 --key_size=100 --write_buffer_size=67108864 --use_existing_db=0 --threads=2 --num=100000'

#ROCKSDB aliases
alias rocksdb-clone='$SCRIPTS_PATH/rocksdb.sh clone'
alias rocksdb-compile='$SCRIPTS_PATH/rocksdb.sh compile'
alias rocksdb-compile-debug='$SCRIPTS_PATH/rocksdb.sh compile-debug'
alias rocksdb-clean='$SCRIPTS_PATH/rocksdb.sh clean'
alias rocksdb-remove='$SCRIPTS_PATH/rocksdb.sh remove'

#VTUNE aliases
alias vtune-install='$SCRIPTS_PATH/vtune.sh install'
alias vtune-uninstall='$SCRIPTS_PATH/vtune.sh uninstall'
alias vtune-run='sudo /opt/intel/vtune_amplifier/bin64/amplxe-gui'

#APPBENCH aliases
alias appbench-clone='$SCRIPTS_PATH/appbench.sh clone'
alias appbench-compile='$SCRIPTS_PATH/appbench.sh compile'
alias appbench-remove='$SCRIPTS_PATH/appbench.sh remove'
alias graphchi='$APPBENCH_PATH/graphchi/run.sh'
alias gtc='$APPBENCH_PATH/gtc-benchmark/run.sh'

set +x

