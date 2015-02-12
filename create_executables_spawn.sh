cd build.xeon
cd build
./cmake-clean
### make version for host
module purge
module load ipic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.intel64
### make version for MIC
cd ../build.xeonphi
./cmake-clean
module purge
module load ipic-mic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.mic
mpiexec -np 4  ./iPic3D.mic parameters.inp

