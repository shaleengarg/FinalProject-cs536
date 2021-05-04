#!/bin/bash


NPROC=4

rm -rf btio.epio.out*


export LD_PRELOAD="/usr/lib/liblog.so"

mpirun -np $NPROC ./bin/bt.A.x.ep_io

export LD_PRELOAD=""
