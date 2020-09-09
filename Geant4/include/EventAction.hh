
#ifndef EventAction_h
#define EventAction_h 1
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Globals.hh"

class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    
    void AddEdep(G4double edep, G4int neuNum) {
        fEdep += edep;
        fNeuNum += neuNum;
    }

  private:
    RunAction* fRunAction;
    G4double     fEdep;
    G4int        fNeuNum;
};

#endif

    
