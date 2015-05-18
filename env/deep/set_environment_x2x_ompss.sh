#!/bin/bash

module purge
module load cmake/2.8
module load intel/15.0
module load parastation/intel-mt-5.1.1
#module load impi/5.0.1
module load gcc
module use /homec/deep/deep59/privatemodules
module load offload/ompss
module use $IPIC_HOME/env/deep
module load ipic-mercurium-nohdf5
