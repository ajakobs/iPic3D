COMP_HOST=1
COMP_MIC=1

if [ $COMP_HOST -eq 1 ]; then

### make version for Xeon
source $IPIC_HOME/env/marenostrum/set_environment_x2x_native.sh
if  [ ! -d $IPIC_HOME/build.xeon ]; then mkdir $IPIC_HOME/build.xeon; fi
cd $IPIC_HOME/build.xeon
cp $IPIC_HOME/scripts/cmake-clean .
./cmake-clean
### workaround, CMAKE doesn't set them correctly. Additionally, change ./CMakeFiles/iPiC3D.dir/link.txt
export IPIC_CMAKE_ARGS="-DCMAKE_CXX_FLAGS='-DOFFLOAD -DOPENMP -DNB_COMM -openmp -fno-exceptions -fp-model fast=2' -DIPIC_HDF5_OUTPUT=OFF"
ipic cmake
make
mv iPic3D iPic3D.intel64
cd - 
echo "... created executable for native offload version in build.xeon"

fi


if [ $COMP_MIC -eq 1 ]; then

### make version for MIC
source $IPIC_HOME/env/marenostrum/set_environment_x2m_native.sh
if [ ! -d $IPIC_HOME/build.xeonphi ]; then mkdir $IPIC_HOME/build.xeonphi; fi
cd $IPIC_HOME/build.xeonphi
cp $IPIC_HOME/scripts/cmake-clean .
cp $IPIC_HOME/scripts/run_fields.sh .
cp $IPIC_HOME/scripts/spawnfile .
./cmake-clean
### workaround, CMAKE doesn't set them correctly. Additionally, change ./CMakeFiles/iPiC3D.dir/link.txt
export IPIC_CMAKE_ARGS="-DCMAKE_CXX_FLAGS='-DOFFLOAD -DOPENMP -DNB_COMM -openmp -fno-exceptions -fp-model fast=2 -mmic' -DIPIC_HDF5_OUTPUT=OFF"
ipic cmake
make 
mv iPic3D iPic3D.mic
cd -
echo "... created executable for native offload version in build.xeonphi "

fi 

