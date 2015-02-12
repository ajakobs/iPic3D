#include "MPIdata.h"
#include "iPic3D.h"
#include "Parameters.h"
#include "debug.h"
#include "TimeTasks.h"
#include "arg_serializer.h"
#include <stdio.h>
#include <unistd.h>
using namespace iPic3D;

int main(int argc, const char **argv) {

 MPIdata::init(&argc, argv);
 Parameters::init_parameters();
  
  //MPI_Comm clustercomm; //The communicater for the offload to the cluster
  //deep_booster_alloc(MPI_COMM_WORLD, 1, 4, &clustercomm);

  /* Serialise argc and argv */
  //char *argv_ser = arg_serializer(argc, argv);
  //int argv_ser_len = strlen(argv_ser);
  
 // int newRank, clusterSize,size;
  //MPI_Comm_rank(clustercomm,&newRank);
  //MPI_Comm_size(clustercomm,&clusterSize);
  //MPI_Comm_size(MPI_COMM_WORLD,&size);
  //printf("Size of the cluster communicator: %d\n",clusterSize);
    
  //int rank;
  //MPI_Comm_rank(MPI_COMM_WORLD,&rank);


//#pragma omp task device(mpi) onto(clustercomm,rank) in(argv_ser[0;argv_ser_len+1]) copy_deps
  //{
   /* Deserialize argc and argv */
  // char **argv;
   //int argc = 0;
   //arg_deserializer(argv_ser, &argc, &argv);

   //MPIdata::init(&argc, (const char **)argv);
   //Parameters::init_parameters();
  // int parentSize, parentRank, test=5;
  // MPI_Comm parent;
  // MPI_Comm_get_parent(&parent);
  // MPI_Comm_rank(parent, &parentRank);
   //MPI_Comm_size(parent,&parentSize);     
 
   //MIsolver::MIsolver solver(argc,(const char **)argv);
   //solver.run_Cluster();
   //trying to call finalize_mpi here leads to the application hanging after "simulation ended succesfully"
   //MPIdata::instance().finalize_mpi();
  //}


//part which runs on Booster

  //{
    iPic3D::c_Solver solver(argc, argv);
    //solver.run_Booster(mpi->intercomm);
  //}

//#pragma omp taskwait 
//sleep(20);
  MPIdata::instance().finalize_mpi();
  //deep_booster_free(&clustercomm);
  return 0;
}

void c_Solver::set_initial_conditions()
{
  MIsolver::set_initial_conditions();
}
