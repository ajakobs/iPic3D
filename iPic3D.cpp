
#include "MPIdata.h"
#include "iPic3D.h"
#include "Parameters.h"
#include "debug.h"
#include "TimeTasks.h"
#include "arg_serializer.h"
#include <stdio.h>

using namespace iPic3D;

int main(int argc, const char **argv) {

 MPIdata::init(&argc, argv);
 Parameters::init_parameters();

//part which runs on Cluster with reverse offload

  MPI_Comm cluster; //The communicater for the offload to the cluster
  deep_booster_alloc(MPI_COMM_WORLD, 1, 4, &cluster);

  /* Serialise argc and argv */
  char *argv_ser = arg_serializer(argc, argv);
  int argv_ser_len = strlen(argv_ser);
  
  int newRank, clusterSize,size;
  MPI_Comm_rank(cluster,&newRank);
  MPI_Comm_size(cluster,&clusterSize);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  //printf("Size of the cluster communicator: %d\n",clusterSize);
    
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);


#pragma omp task device(mpi) onto(cluster,rank) in(argv_ser[0;argv_ser_len+1]) copy_deps
  {
   /* Deserialize argc and argv */
   char **argv =  NULL;
   int argc = 0;
   arg_deserializer(argv_ser, &argc, &argv);

   /* Why NULL? */
   MPIdata::init(&argc, NULL);
   Parameters::init_parameters();
   int parentSize, parentRank, test=5;
   MPI_Comm parent;
   MPI_Comm_get_parent(&parent);
   MPI_Comm_rank(parent, &parentRank);
   MPI_Comm_size(parent,&parentSize);     
   //printf("My rank in the communicator parent: %d\n",parentRank);
   //MPI_Status stat;
   //MPI_Recv(&test, 1, MPI_INT, parentRank, 77, parent,&stat);
   // printf("I (rank %d) on Cluster (in offload) receive %d \n",rank,test);  
   MIsolver::MIsolver solver(1,NULL);
   solver.run_Cluster();
  }


//part which runs on Booster

  {
    //int tmp=10+rank;
    //MPI_Send(&tmp, 1, MPI_INT, newRank, 77, cluster);
    //printf("I (rank %d) on Booster sent %d \n",rank,tmp);  
    iPic3D::c_Solver solver(argc, argv);
    solver.run(argc,argv);
  }
  MPIdata::instance().finalize_mpi();

#pragma omp taskwait
  deep_booster_free(&cluster);
  return 0;
}

void c_Solver::set_initial_conditions()
{
  MIsolver::set_initial_conditions();
}
