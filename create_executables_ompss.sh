cd build
rm -rf *
### make version for host
module purge
module load ipic-mercurium-nohdf5
ipic cmake
make
mv iPic3D ../iPic3D.intel64
### make version for MIC
rm -rf *
module purge
module load ipic-mercurium-mic-nohdf5
ipic cmake
make
mv iPic3D iPic3D.mic
mv ../iPic3D.intel64 .
