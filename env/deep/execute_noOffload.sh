# Resource requirements
export CN=1
export PPN_CN=1

export OMP_NUM_THREADS=1 #$(( $PBS_NUM_PPN / $PPN_CN ))
cd $IPIC_HOME/build.no_offload

# Parastation + Torque does not like --hostfile option
mpiexec -np $(( $CN * $PPN_CN )) ./iPic3D.intel64 parameters.small >>test.benchmark2

