//


#ifndef SpinICRunAction_h
#define SpinICRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class SpinICRunMessenger;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class SpinICRunAction : public G4UserRunAction
{
  public:
    SpinICRunAction();
   ~SpinICRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
    
    void SetFileName(G4String name);

    
  private:
    G4String fileName; 
    SpinICRunMessenger* runMessenger;
};

inline void SpinICRunAction::SetFileName(G4String name)
{
    fileName = name;
}


#endif

