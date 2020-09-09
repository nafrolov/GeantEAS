#include "PrimaryGeneratorAction.hh"

#include<G4Event.hh>
#include<G4ParticleGun.hh>
#include<G4GeneralParticleSource.hh>

#include<G4ParticleTable.hh>
#include<G4ParticleDefinition.hh>
#include<globals.hh> 
#include "Globals.hh"

#include <G4SPSEneDistribution.hh>
#include <G4SPSPosDistribution.hh>
#include <G4SPSAngDistribution.hh>
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

using namespace std;

PrimaryGeneratorAction :: PrimaryGeneratorAction()
		:G4VUserPrimaryGeneratorAction(), 
		fParticleGun(0)
		
			     
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle;
    G4String particleName;


    G4double Energy = 100*GeV;

    particleName="proton";
    fParticleGun  = new G4GeneralParticleSource();

    particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);

    G4SPSEneDistribution* edist = fParticleGun->GetCurrentSource()->GetEneDist();
    edist->SetEnergyDisType("Mono");
    edist->SetMonoEnergy(Energy);

    G4SPSAngDistribution* adist = fParticleGun->GetCurrentSource()->GetAngDist();
    adist->SetAngDistType("beam1d");
    adist->SetParticleMomentumDirection(G4ThreeVector(0,0,1*m));

    G4SPSPosDistribution* pdist = fParticleGun->GetCurrentSource()->GetPosDist();
    pdist->SetPosDisType("Plane");
    pdist->SetPosDisShape("Circle");
    pdist->SetRadius(3*m);
    pdist->SetCentreCoords(G4ThreeVector(0.,0.,1.*m));
}

PrimaryGeneratorAction :: ~ PrimaryGeneratorAction()
{
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4int evNum = anEvent->GetEventID();
    //G4cout<<"new event"<<evNum<<G4endl;
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

