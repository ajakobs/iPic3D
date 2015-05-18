# Resource requirements
export CN=1
export PPN_CN=4
export BN=1
export PPN_BN=4

export OMP_NUM_THREADS=$(( $PBS_NUM_PPN / $PPN_CN ))
export OFFL_OMP_NUM_THREADS=1
export NX_ARGS="--enable-yield --spins=1"
cd $IPIC_HOME/build.ompss
module load gcc
# Parastation + Torque does not like --hostfile option
mpiexec --envall -np $(( $CN * $PPN_CN )) ./iPic3D.intel64 parameters.small

