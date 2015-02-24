if  [ ! -d build.xeon ]; then mkdir build.xeon; fi
cd build.xeon
cp ../scripts/cmake-clean .
./cmake-clean
### make version for host
module purge
module load ipic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.intel64
cd ..
### make version for MIC
if [ ! -d build.xeonphi ]; then mkdir build.xeonphi; fi
cd build.xeonphi
cp ../scripts/cmake-clean .
cp ../scripts/run_fields.sh .
cp ../scripts/spawnfile .
./cmake-clean
module purge
module load ipic-mic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.mic

cd ..
echo "... created executables for spawn offload version in build.xeon and build.xeonphi "
