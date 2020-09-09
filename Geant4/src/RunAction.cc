
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "SteppingAction.hh"
#include "Run.hh"
#include "G4MTRunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Globals.hh"
#include <fstream>
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fNeuNum(0),
  sum(0)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fNeuNum);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4MTRunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep  = fEdep.GetValue();
  G4int neuNum = fNeuNum.GetValue();

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4MTRunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;

  // Print
  //
  if (IsMaster()) {
    G4cout
    << G4endl
    << "--------------------End of Global Run-----------------------"
    << G4endl
    << " The run consists of " << nofEvents << " "<< runCondition
    << G4endl
    << " Cumulated Energy deposit per run, in scoring volume : "
    << edep << " MeV"
    << G4endl
    << " Cumulated neutron number per run, in scoring volume : "
    << neuNum
    << G4endl
    << "------------------------------------------------------------"
    << G4endl;
  }
  char name[100];
  sprintf(name, "captured_neutrons_%02dcm_from_center.txt", startpoint);
  std::ofstream file(name);
  file<<G4endl
     << "--------------------End of Global Run-----------------------"
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated Energy deposit per run, in scoring volume : "
     << edep << " MeV"
     << G4endl
     << " Cumulated neutron number per run, in scoring volume : "
     << neuNum
     << G4endl
     << "------------------------------------------------------------"
     << G4endl;
  file.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep, G4int neuNum)
{
  fEdep  += edep;
  fNeuNum += neuNum;
}
