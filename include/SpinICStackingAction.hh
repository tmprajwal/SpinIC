
#ifndef SpinICStackingAction_h
#define SpinICStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

/// Stacking action class : manage the newly generated particles
///
/// One wishes do not track secondary neutrino.Therefore one kills it
/// immediately, before created particles will  put in a stack.

class SpinICStackingAction : public G4UserStackingAction
{
public:
    SpinICStackingAction();
    virtual ~SpinICStackingAction();
    
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
private:

};
#endif
