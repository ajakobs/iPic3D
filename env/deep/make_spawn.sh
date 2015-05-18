cp $IPIC_HOME/scripts/cmake-clean $IPIC_HOME/build.xeon 
cd $IPIC_HOME/build.xeon
./cmake-clean
### make version for host
source $IPIC_HOME/env/deep/set_environment_x2x_spawn.sh 
ipic cmake
make
mv $IPIC_HOME/build.xeon/iPic3D $IPIC_HOME/build.xeon/iPic3D.intel64
### make version for MIC
#./cmake-clean
#module purge
#module load ipic-mercurium-mic-nohdf5
#ipic cmake
#make
#mv iPic3D iPic3D.mic
cd $IPIC_HOME/env/deep
echo "... created executables for MPI_Comm_spawn  offload version in build.xeon"
