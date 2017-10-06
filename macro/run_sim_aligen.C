#if (!defined(__CINT__) && !defined(__CLING__)) || defined(__MAKECINT__)
  #include <iostream>

  #include "Rtypes.h"
  #include "TSystem.h"
  #include "TMath.h"
  #include "TString.h"
  #include "TStopwatch.h"
  #include "TGeoManager.h"

  #include "FairRunSim.h"
  #include "FairRuntimeDb.h"
  #include "FairPrimaryGenerator.h"
  #include "FairBoxGenerator.h"
  #include "FairParRootFileIo.h"
  #include "FairSystemInfo.h"

  #include "Field/MagneticField.h"

  #include "DetectorsPassive/Cave.h"
  #include "Generators/GeneratorFromFile.h"
  #include "TPCSimulation/Detector.h"
#endif

void run_sim_aligen(Int_t nEvents = 10, TString mcEngine = "TGeant3")
{
  TString dir = getenv("VMCWORKDIR");
  TString geom_dir = dir + "/Detectors/Geometry/";
  gSystem->Setenv("GEOMPATH",geom_dir.Data());


  TString tut_configdir = dir + "/Detectors/gconfig";
  gSystem->Setenv("CONFIG_DIR",tut_configdir.Data());

  // Output file name
  char fileout[100];
  sprintf(fileout, "AliceO2_%s.aligen.mc_%i_event.root", mcEngine.Data(), nEvents);
  TString outFile = fileout;

  // Parameter file name
  char filepar[100];
  sprintf(filepar, "AliceO2_%s.aligen.params_%i.root", mcEngine.Data(), nEvents);
  TString parFile = filepar;

  // In general, the following parts need not be touched

  // Debug option
  gDebug = 0;

  // Timer
  TStopwatch timer;
  timer.Start();


  // Create simulation run
  FairRunSim* run = new FairRunSim();
  // enable usage of the fair link mechanism
  run->SetUseFairLinks(kTRUE);

  run->SetName(mcEngine);      // Transport engine
  run->SetOutputFile(outFile); // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // Create media
  run->SetMaterials("media.geo"); // Materials

  // Create geometry
  o2::Passive::Cave* cave = new o2::Passive::Cave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  auto magField = std::make_unique<o2::field::MagneticField>("Maps","Maps", -1., -1., o2::field::MagFieldParam::k5kG);
  run->SetField(magField.get());

  // ===| Add TPC |============================================================
  o2::TPC::Detector* tpc = new o2::TPC::Detector("TPC", kTRUE);
  tpc->SetGeoFileName("TPCGeometry.root");
  run->AddModule(tpc);

  // Create AliGenHijing
  gSystem->Load("libHIJING");
  gSystem->Load("libTHijing");
  auto hij = new AliGenHijing(-1);
  hij->SetEnergyCMS(5000.);
  hij->SetImpactParameterRange(0., 20.);
  hij->SetReferenceFrame("CMS");
  hij->SetProjectile("A", 208, 82);
  hij->SetTarget("A", 208, 82);
  hij->SetSpectators(0);
  hij->KeepFullEvent();
  hij->SetJetQuenching(0);
  hij->SetShadowing(1);
  hij->SetDecaysOff(1);
  hij->SetSelectAll(0);
  hij->SetPtHardMin(2.3);
  hij->Init();

  // Create TGenerator interface
  auto gen = new o2::eventgen::GeneratorTGenerator();
  gen->setGenerator(hij->GetMC());
  
  // Create PrimaryGenerator
  auto primGen = new o2::eventgen::PrimaryGenerator();
  primGen->AddGenerator(gen);
  run->SetGenerator(primGen);

  // store track trajectories
  // run->SetStoreTraj(kTRUE);

  // Initialize simulation run
  run->Init();

  // Runtime database
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();

  // Start run
  run->Run(nEvents);
  delete run;
//  run->CreateGeometryFile("geofile_full.root");

  // Finish
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();

  // extract max memory usage
  FairSystemInfo sysinfo;

  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  std::cout << "Memory used " << sysinfo.GetMaxMemory() << "\n";
}
