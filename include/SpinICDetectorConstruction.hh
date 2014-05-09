#ifndef SpinICDetectorConstruction_h
#define SpinICDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4LogicalVolume;

class SpinICDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    SpinICDetectorConstruction();
    virtual ~SpinICDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
    void DefineMaterials();
    void DefineMagnets();
    void DefineIC();
    
    
private:

    
    // data members
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    //G4int   verbosity;
    
    // World Logical Volume
    G4LogicalVolume* flogicalWorld;
    
};
#endif
