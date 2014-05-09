//

#include "SpinICRunMessenger.hh"
#include "SpinICRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


SpinICRunMessenger::SpinICRunMessenger(SpinICRunAction* runAction)
: G4UImessenger(),
fRunAction(runAction),
fDirectory(0),
fSetFileNameCmd(0)
{
    fDirectory = new G4UIdirectory("/SpinIC/run/");
    fDirectory->SetGuidance("run control");
    
    fSetFileNameCmd
    = new G4UIcmdWithAString("/SpinIC/run/setFileName",this);
    fSetFileNameCmd->SetGuidance("Set the analysis file name (default: SpinICTest");
    fSetFileNameCmd->SetParameterName("fileName",false);
    fSetFileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SpinICRunMessenger::~SpinICRunMessenger()
{
    delete fSetFileNameCmd;
    delete fDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SpinICRunMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if ( command == fSetFileNameCmd ) {
        fRunAction->SetFileName(newValue);
    }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

   
