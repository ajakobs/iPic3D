#!/bin/bash
module purge
module load cmake/2.8
module load intel/15.0
#module load impi/5.0.1
module load parastation/intel-mt-5.1.1

module use $IPIC_HOME/env/deep
module load ipic-no_offload-nohdf5
