#APPPREFIX="numactl --membind=0"


rm -rf $PWD/files/
$PWD/clear_cache.sh
sudo sh -c "dmesg --clear"

export IOMODE=SYNC
export FILETYPE=UNIQUE
export IOMETHOD=POSIX
RECORD=4096 # bytes read at once
STRIDE=7 # set stride to $STRIDE * RECORD_SIZE
NPROC=16 ##Num MPI procs
FLUSH=1 ##flush writes

export LD_PRELOAD="/usr/lib/liblog.so"

TIMESPREFETCH=2 /usr/bin/time -v mpiexec -n $NPROC ./MADbench2_io 16384 10 1 8 64 1 1 $RECORD $STRIDE $FLUSH

export LD_PRELOAD=
