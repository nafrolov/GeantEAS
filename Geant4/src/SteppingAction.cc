#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "Run.hh"
#include <G4StepPoint.hh>
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh" 

#include "G4UserRunAction.hh"
#include "G4SystemOfUnits.hh"
#include <G4Triton.hh>
#include "RunAction.hh"
#include "Randomize.hh"





SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{	
	const G4String processName = step->GetPostStepPoint()->
		GetProcessDefinedStep()->GetProcessName(); 
	const std::vector<const G4Track*>* secondaries =step->GetSecondaryInCurrentStep();

    if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Detector")
    {
        G4double time=step->GetTrack()->GetGlobalTime();
        if (time/microsecond <= 1.)
            fEventAction->AddEdep(step->GetTotalEnergyDeposit()/MeV, 0);
        else if (time/microsecond > 100 && time/ms < 20)
        {
            if (processName == "neutronInelastic")
            {
                if ((*secondaries->begin())->GetDefinition()->GetParticleName() == "alpha")
                {
                    step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
                    fEventAction->AddEdep(0, 1);
                }
            }
        }
    }
}


