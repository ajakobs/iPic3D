COMP_HOST=1
COMP_MIC=0

if [ $COMP_HOST -eq 1 ]; then

### make version for Xeon
source $IPIC_HOME/env/marenostrum/set_environment_nooffload.sh
if  [ ! -d $IPIC_HOME/build.noOffload ]; then mkdir $IPIC_HOME/build.noOffload; fi
cd $IPIC_HOME/build.noOffload
cp $IPIC_HOME/scripts/cmake-clean .
make clean
./cmake-clean
### workaround, CMAKE doesn't set them correctly. Additionally, change link.txt_intel64
export IPIC_CMAKE_ARGS="-DCMAKE_CXX_FLAGS='-DOPENMP -DNB_COMM -openmp -fno-exceptions -O3 -fp-model fast=2 ' -DIPIC_HDF5_OUTPUT=OFF"
ipic cmake
cp $IPIC_HOME/env/marenostrum/noOffload_link.txt_intel64 CMakeFiles/iPic3D.dir/link.txt
make
cp ../inputfiles/benchmark.inp parameters.inp
cd - 
echo "... created executable for native offload version in build.noOffload"

fi


if [ $COMP_MIC -eq 1 ]; then

### make version for MIC
source $IPIC_HOME/env/marenostrum/set_environment_nooffload_mic.sh
if [ ! -d $IPIC_HOME/build.noOffload_mic ]; then mkdir $IPIC_HOME/build.noOffload_mic; fi
cd $IPIC_HOME/build.noOffload_mic
cp $IPIC_HOME/scripts/cmake-clean .
cp $IPIC_HOME/scripts/spawnfile .
./cmake-clean
### workaround, CMAKE doesn't set them correctly. Additionally, change link.txt_mic
export IPIC_CMAKE_ARGS="-DCMAKE_CXX_FLAGS='-DOPENMP -DNB_COMM -openmp -fno-exceptions -O3 -fp-model fast=2 -mmic -I/gpfs/apps/MN3/INTEL/impi/4.1.3.049/mic/include' -DIPIC_HDF5_OUTPUT=OFF"
ipic cmake
cp $IPIC_HOME/env/marenostrum/noOffload_link.txt_mic CMakeFiles/iPic3D.dir/link.txt
make 
cp ../inputfiles/benchmark.inp parameters.inp
cd -
echo "... created executable for native offload version in build.noOffload_mic "

fi 

