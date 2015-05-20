export IPIC_HOME=`pwd`
module use $IPIC_HOME/env/marenostrum

if [ ! -d $IPIC_HOME/build.ompss ]; then mkdir $IPIC_HOME/build.ompss; fi

COMP_HOST=0
COMP_MIC=1

if [ $COMP_HOST -eq 1 ]; then

### make version for host
source $IPIC_HOME/env/marenostrum/set_environment_x2x.sh
cd $IPIC_HOME/build.ompss
cp $IPIC_HOME/scripts/cmake-clean .
make clean
./cmake-clean
ipic cmake
make 
mv iPic3D iPic3D.intel64
cd -
echo "... created host executable for ompss offload version in build.ompss"

fi

if [ $COMP_MIC -eq 1 ]; then

### make version for MIC
source $IPIC_HOME/env/marenostrum/set_environment_x2m.sh
cd $IPIC_HOME/build.ompss
cp $IPIC_HOME/scripts/cmake-clean .
make clean
./cmake-clean
ipic cmake
make
mv iPic3D iPic3D.mic
cd -
echo "... created mic executable for ompss offload version in build.ompss"

fi

