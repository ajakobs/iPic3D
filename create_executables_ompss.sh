if [ ! -d build ]; then mkdir build; fi
cd build
cp ../scripts/cmake-clean .
./cmake-clean
### make version for host
module purge
module load ipic-mercurium-nohdf5
ipic cmake
make
mv iPic3D ../iPic3D.intel64
### make version for MIC
./cmake-clean
module purge
module load ipic-mercurium-mic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.mic
mv ../iPic3D.intel64 .
