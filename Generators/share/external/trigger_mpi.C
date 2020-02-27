// MPI trigger
//
//   usage: o2sim --trigger external --extTrgFile trigger_mpi.C
// options:                          --extTrgFunc "trigger_mpi()"
//

/// \author R+Preghenella - February 2020

#include "Generators/Trigger.h"
#include "Pythia8/Pythia.h"

o2::eventgen::DeepTrigger
  trigger_mpi(int mpiMin = 5)
{
  return [mpiMin](void* interface, std::string name) -> bool {
    if (!name.compare("pythia8")) {
      auto py8 = reinterpret_cast<Pythia8::Pythia*>(interface);
      return py8->info.nMPI() >= mpiMin;
    }
    LOG(FATAL) << "Cannot define MPI for generator interface \'" << name << "\'";
    return false;
  };
}
