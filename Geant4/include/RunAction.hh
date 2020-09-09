#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "Globals.hh"
#include "G4Accumulable.hh"

class G4Run;
class EventAction;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
	
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEdep (G4double edep, G4int neuNum);

  private:
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4int> fNeuNum;
    G4int sum;

};

#endif

