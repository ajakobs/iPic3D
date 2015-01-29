
#include "MPIdata.h"
#include "iPic3D.h"
#include "Parameters.h"
#include "debug.h"
#include "TimeTasks.h"
#include <stdio.h>

using namespace iPic3D;

int main(int argc, const char **argv) {

 MPIdata::init(&argc, argv);
 Parameters::init_parameters();

//part which runs on Cluster with reverse offload

  MPI_Comm cluster; //The communicater for the offload to the cluster
  deep_booster_alloc(MPI_COMM_WORLD, 1, 4, &cluster);
  int newRank, clusterSize,size;
  MPI_Comm_rank(cluster,&newRank);
  MPI_Comm_size(cluster,&clusterSize);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  printf("Groesse des Communicator: %d\n",size);
  //serialising argc and argv
    
   char *buff = new char[100];
   int offset=0, argsize=0;
   memcpy(buff, argv[0],strlen(argv[0]));
   offset=strlen(argv[0]);
   memcpy((void*)(buff+offset), argv[1],strlen(argv[1]));
   argsize=strlen(argv[0])+strlen(argv[1]);
   int arglen[3] = {strlen(argv[0]),strlen(argv[1]),0};

   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);


#pragma omp task device(mpi) onto(cluster,rank) in(argc, buff[0;argsize],arglen[0;1]) copy_deps
  {
   //deserialize argc and argv
   char **argv=new char*[argc];
   argv[0] = new char[arglen[0]];
   argv[1] = new char[arglen[1]];
   memcpy(argv[0],buff,arglen[0]);
   memcpy(argv[1],buff+arglen[0],arglen[1]);
   printf("argc = %d, arglen = {%d , %d}, argv = {%s , %s}\n",argc,arglen[0],arglen[1],argv[0],argv[1]);

   MPIdata::init(&argc, NULL);
   Parameters::init_parameters();
   int parentSize, parentRank, test=5;
   MPI_Comm parent;
   MPI_Comm_get_parent(&parent);
   MPI_Comm_rank(parent, &parentRank);
   MPI_Comm_size(parent,&parentSize);
   test+=parentRank;  
   printf("Mein Rang im Communicator parent im Offload Bereich: %d\n",parentRank);
   MPI_Send(&test, 1, MPI_INT, parentRank, 77, parent);
   MIsolver::MIsolver solver(1,NULL);
   solver.run_Cluster();
  }


//part which runs on Booster

  {
    iPic3D::c_Solver solver(argc, argv);
    int tmp;
    MPI_Status stat;
    MPI_Recv(&tmp, 1, MPI_INT, newRank, 77, cluster, &stat);
    printf("Ich (Rang %d) bekomme den Wert %d gesendet\n",rank,tmp);  
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
