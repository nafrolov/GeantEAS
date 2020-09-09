#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_HP.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "PhysicsList.hh"

int waterfraction = 0;
int startpoint = 0;

int main(int argc, char **argv)
{
  if (argc < 3)
      return 0;
  waterfraction = atoi(argv[2]);
  //G4UIExecutive* ui = new G4UIExecutive(argc, argv);

  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(108798);

  G4MTRunManager* runManager = new G4MTRunManager;
 // G4VModularPhysicsList* physicsList = new MyPhysicsList();
  runManager->SetVerboseLevel(0);
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new MyPhysicsList());

  runManager->SetUserInitialization(new ActionInitialization);

  runManager->SetNumberOfThreads(16);
  runManager->Initialize();
  //G4UImanager* UI = G4UImanager::GetUIpointer();



  //UI->ApplyCommand("/control/execute run.mac");
  runManager->BeamOn(atoi(argv[1]));
  //ui->SessionStart();

  //delete ui;
  delete runManager;
  return 0;
}

