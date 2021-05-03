#!/bin/bash

ROCKSDB_PATH=./

rocksdb_clean () {
    echo "Cleaning RocksDB"
    pushd $ROCKSDB_PATH
    make clean -j
    popd
}

sudo apt-get install -y libgflags2.2 libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev

rocksdb_clean

echo "compiling rocksdb"
pushd $ROCKSDB_PATH
make clean
DEBUG_LEVEL=0 make db_bench -j$(nproc)
popd
