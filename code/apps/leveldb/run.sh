#!/bin/bash
set -x
APPBASE=$CODEBASE/leveldb
APP=$APPBASE/db_bench
PARAM=$1
OUTPUT=$2

FlushDisk()
{
	sudo sh -c "echo 3 > /proc/sys/vm/drop_caches"
	sudo sh -c "sync"
	sudo sh -c "sync"
	sudo sh -c "echo 3 > /proc/sys/vm/drop_caches"
}

mkdir $APPBASE/DATA
export TEST_TMPDIR=$APPBASE/DATA
export TMP_TESTDIR=$APPBASE/DATA

RUN(){
export LD_PRELOAD=$SHARED_LIBS/construct/libmigration.so
$APPPREFIX $APP --num=500000 --value_size=4096
export LD_PRELOAD=""
}


cd $CODEBASE/leveldb
FlushDisk
RUN
set +x

