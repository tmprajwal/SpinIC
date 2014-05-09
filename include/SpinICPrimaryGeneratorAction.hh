#ifndef SpinICPrimaryGeneratorAction_h
#define SpinICPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4GeneralParticleSource;
class G4Event;

class SpinICPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    SpinICPrimaryGeneratorAction();
    ~SpinICPrimaryGeneratorAction();
    
public:
    void GeneratePrimaries(G4Event* anEvent);
    
private:
    
    G4GeneralParticleSource* particleGun;
    
};

#endif


