#include "MPIdata.h"
#include "iPic3D.h"
#include "Parameters.h"
#include "debug.h"
#include "TimeTasks.h"
#include "arg_serializer.h"
#include "SolverType.h"
#include <stdio.h>
#include <unistd.h>
#include <omp.h>
#include <sys/time.h>

using namespace iPic3D;
//struct timeval begin, end;
int main(int argc, const char **argv) {
  //gettimeofday(&begin,0);
  MPIdata::init(&argc, argv);

  Parameters::init_parameters();

#if defined(OFFLOAD) || defined(OMPSS_OFFLOAD)
  MPI_Comm clustercomm; //The communicater for the offload to the cluster
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
#ifdef OMPSS_OFFLOAD
  deep_booster_alloc(MPI_COMM_WORLD, 1, size, &clustercomm);

  /* Serialise argc and argv */
  char *argv_ser = arg_serializer(argc, argv);
  int argv_ser_len = strlen(argv_ser);
  
  int newRank, clusterSize;
  MPI_Comm_rank(clustercomm,&newRank);
  MPI_Comm_size(clustercomm,&clusterSize);
  printf("Size of the cluster communicator: %d\n",clusterSize);
    

#pragma omp task device(mpi) onto(clustercomm,rank) in(argv_ser[0;argv_ser_len+1]) copy_deps
  {
   char hostname[255];
   gethostname(hostname,255);
   printf("OmpSs Version. Fields solver on %s\n",hostname);
   /* Deserialize argc and argv */
   char **argv;
   int argc = 0;
   arg_deserializer(argv_ser, &argc, &argv);
   //printf("argv: %s %s\n",argv[0],argv[1]);

    MPIdata::init(&argc, (const char **)argv);
    Parameters::init_parameters();
    int parentSize, parentRank;
    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);
    MPI_Comm_rank(parent, &parentRank);
    MPI_Comm_size(parent,&parentSize);

    //timeTasks.set_output("output_cluster.out");
    MIsolver::MIsolver solver(argc,(const char **)argv);
    solver.run_Cluster();
    //trying to call finalize_mpi here leads to the application hanging after "simulation ended succesfully"
    //MPIdata::instance().finalize_mpi();
  }

//part which runs on Booster
  {
    //timeTasks.set_output("output_booster.out");
    char hostname[255];
    gethostname(hostname,255);
    printf("OmpSs Version. Particle solver on %s\n",hostname);
    iPic3D::c_Solver solver(argc, argv);
    solver.run_Booster(clustercomm);
  }

//#pragma omp taskwait 
  deep_booster_free(&clustercomm);
  MPIdata::instance().finalize_mpi();

#else // End of OMPSS_OFFLOAD
  {
    /* Set type of solver */
    char **params=(char **)argv;
    SolverType solver_type;
    char hostname[255];
    MPI_Comm_get_parent(&clustercomm);
    solver_type = (MPI_COMM_NULL == clustercomm) ? PARTICLES : FIELDS;
    gethostname(hostname,255);
    if (solver_type == PARTICLES) {
      //set output from stdout to file
      timeTasks.set_output("output_booster.out");
      //solver has to be created here to be able to set different output files beforehand
      iPic3D::c_Solver solver(argc, argv);
      printf("Native Version. Particle solver: process %d on %s\n",MPIdata::get_rank(),hostname);
      MPI_Info info;
      MPI_Info_create(&info);
	/* We should improve this */
      MPI_Info_set(info, "hostfile", "spawnfile");
      MPI_Comm_spawn("run_fields.sh", &params[1], size, info, 0, MPI_COMM_WORLD, &clustercomm, MPI_ERRCODES_IGNORE); 
      MPI_Info_free(&info);
      solver.run_Booster(clustercomm);
    }
    else {
      //set output from stdout to file
      timeTasks.set_output("output_cluster.out");
      //solver has to be created here to be able to set different output files beforehand
      iPic3D::c_Solver solver(argc, argv);
      printf("Native Version. Fields solver: process %d on %s\n",MPIdata::get_rank(),hostname);
      solver.run_Cluster();
    }
  }
  MPIdata::instance().finalize_mpi();
#endif // End of OFFLOAD
#else
  {
    char hostname[255];
    gethostname(hostname,255);
    printf("No Offload Version. Application on %s\n",hostname);
    #ifdef OPENMP
    printf("OpenMP is used!\n");
    #else
    printf("OpenMP is not used!\n");
    #endif
    timeTasks.set_output("output_noOffload.out");
    iPic3D::c_Solver solver(argc, argv);
    solver.run();
  }
  MPIdata::instance().finalize_mpi();
#endif
  //gettimeofday(&end,0);
  //double time=(1000000*(end.tv_sec - begin.tv_sec)+(end.tv_usec - begin.tv_usec))*0.000001;
  //printf("Application runtime: %f\n",time);
  //MPIdata::instance().finalize_mpi();
  return 0;
}

void c_Solver::set_initial_conditions()
{
  MIsolver::set_initial_conditions();
}
