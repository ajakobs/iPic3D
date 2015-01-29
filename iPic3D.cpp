
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
   
   MIsolver::MIsolver solver(1,NULL);
   solver.run_Cluster();
  }


//part which runs on Booster

  {
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
