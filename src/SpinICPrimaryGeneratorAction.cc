#include "SpinICPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

SpinICPrimaryGeneratorAction::SpinICPrimaryGeneratorAction()
{
    particleGun = new G4GeneralParticleSource();
}

SpinICPrimaryGeneratorAction::~SpinICPrimaryGeneratorAction()
{
    delete particleGun;
}

void SpinICPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    particleGun->GeneratePrimaryVertex(anEvent);
}
