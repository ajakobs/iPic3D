#ifndef MIsolver_h
#define MIsolver_h
// forward declarations
class MImoments;
class Collective;
class Grid3DCU;
class VCtopology3D;
class EMfields3D;
class Kinetics;
class SpeciesMoms;
class Timing;
class OutputWrapperTXT;
class OutputWrapperFPP;
#include "arraysfwd.h"
//
#include "Setting.h"
#include "asserts.h"
#include "assert.h"
#include "SolverType.h"

namespace iPic3D
{
  // generic class to solve with the moment-implicit method
  class MIsolver
  {
  private:
    const Setting &setting;
    const Collective* col;
    const VCtopology3D* vct;
    const Grid3DCU* grid;
    SpeciesMoms   *speciesMoms;
    MImoments     *miMoments;
    EMfields3D    *EMf; // implicit field solver
    array4_double *fieldForPcls;
    Kinetics      *kinetics;
    Timing        *my_clock; // deprecated
    SolverType solver_type;
    //
    // output
    //
    OutputWrapperTXT *outputWrapperTXT;
    OutputWrapperFPP *outputWrapperFPP;
    //
    // convenience variables
    const int ns;
    const int nxn;
    const int nyn;
    const int nzn;
    const int nxc;
    const int nyc;
    const int nzc;
#ifdef NB_COMM
	MPI_Request pending_request;
        MPI_Request pending_request2;
#endif

  public: // accessors
    OutputWrapperFPP& fetch_outputWrapperFPP(){
      assert(outputWrapperFPP);
      return *outputWrapperFPP;
    }
  public:
    ~MIsolver();
    MIsolver(int argc, const char **argv);
    void run_Booster(MPI_Comm clustercomm);
    void run_Cluster();
    void run();
  protected:
    // virtual so inheriting application can override
    virtual void set_initial_conditions();
    void initialize_output();
    void initialize(MPI_Comm clustercomm);
    void init_kinetics_from_restart();
    void init_fields_from_restart();
    void init_from_restart();
    void initDipole();
    void initGEM();
    void initBATSRUS();
    // these should be moved into inheriting class
    void initOriginalGEM();
    void initDoublePeriodicHarrisWithGaussianHumpPerturbation();
    void initGEMDipoleLikeTailNoPert();
    void initGEMnoPert();
    void initRandomField();
    void initForceFree();
  protected:
    void accumulate_moments();
    double* compute_moments_Booster(MPI_Comm clustercomm);
    void compute_moments_Cluster();
    void advance_Efield_Cluster();
    void advance_Efield_Booster(MPI_Comm clustercomm);
    void move_particles();
    void advance_Bfield_Cluster();
    void advance_Bfield_Booster(MPI_Comm clustercomm);
    //
    // output methods
    //
    void WriteRestart(int cycle);
    void WriteConserved(int cycle);
    void WriteVelocityDistribution(int cycle);
    void WriteVirtualSatelliteTraces();
    void WriteFields(int cycle);
    void WriteParticles(int cycle);
    void WriteOutput(int cycle);
    void Finalize();

  private:
    void send_field_to_kinetic_solver(bool sender, MPI_Comm *clustercomm);
    void send_moments_to_field_solver(bool sender,MPI_Comm *clustercomm);
    void send_Bsmooth_to_kinetic_solver(bool sender, MPI_Comm *clustercomm);
    void set_fieldForPcls(MPI_Comm clustercomm);
    int FirstCycle();
    int FinalCycle();
    bool is_rank0();
    virtual bool I_am_kinetic_solver(){return true;}
    virtual bool I_am_field_solver(){return true;}

  protected: // accessors
    EMfields3D& fetch_EMfields(){return *EMf;}
    MImoments& fetch_miMoments(){return *miMoments;}
    const MImoments& get_miMoments()const{return *miMoments;}
    SpeciesMoms& fetch_speciesMoms(){return *speciesMoms;}
    const Kinetics& get_kinetics()const{return *kinetics;}
    const Collective& get_col()const{return setting.col();}
    //const Grid3DCU& get_grid()const{return setting.grid();}
    //const VCtopology3D& get_vct()const{return setting.vct();}
    const array4_double& get_fieldForPcls()const{return *fieldForPcls;}
  };
}

#endif // MIsolver_h
