#!/bin/bash
module purge
module use /home/bsc15/bsc15685/privatemodules
module load ipic-mercurium-native-nohdf5

module load cmake
module load gcc/4.7.2
module load intel/14.0.2
module load impi/5.0.1.035
module load offload/ompss/15.03

