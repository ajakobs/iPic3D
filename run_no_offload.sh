if [ ! -d build.no_offload ]; then mkdir build.no_offload; fi
cd build.no_offload
rm CMakeCache.txt
module purge
module load ipic-no_offload-nohdf5
ipic cmake
make
export PBS_NODEFILE=../env/miclogin/no_offload_hosts
export MPIEXEC_UNIVERSE_SIZE=4
mpiexec -np 4 ./iPic3D parameters.inp
