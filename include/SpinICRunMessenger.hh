//

#ifndef SpinICRunMessenger_h
#define SpinICRunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class SpinICRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class SpinICRunMessenger: public G4UImessenger
{
public:
    
    SpinICRunMessenger(SpinICRunAction* );
    ~SpinICRunMessenger();
    
    void SetNewValue(G4UIcommand* ,G4String );
    
private:
    
    SpinICRunAction* 	 fRunAction;
    G4UIdirectory*           fDirectory;
    G4UIcmdWithAString*      fSetFileNameCmd;
    
};

#endif

