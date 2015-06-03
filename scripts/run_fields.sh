#!/bin/sh
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/apps/intel/13/composer_xe_2013_sp1.1.106/compiler/lib/intel64
# Set number of OpenMP threads per spawned MPI proc
#export NX_ARGS="--smp-workers=2"
export OMP_NUM_THREADS=2
exec ../build.xeon/iPic3D.intel64 $1

