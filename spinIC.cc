//
// SpicIC
// Prajwal Mohanmurthy
//
//

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "SpinICDetectorConstruction.hh"
//#include "SpinICPhysicsList.hh"
#include "G4PhysListFactory.hh"
#include "SpinICPrimaryGeneratorAction.hh"
#include "SpinICRunAction.hh"
#include "SpinICStackingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
    
    //choose the Random engine
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    // Construct the default run manager
    G4RunManager * runManager = new G4RunManager;
    
    // set mandatory initialization classes

    // Detector
    SpinICDetectorConstruction* detector = new SpinICDetectorConstruction();
    runManager->SetUserInitialization(detector);
    
    // Physics list
    G4PhysListFactory factory;
    runManager->SetUserInitialization(                                      factory.GetReferencePhysList("FTFP_BERT_EMX"));
    factory.SetVerbose(1);
    
    // Run Action
    SpinICRunAction* runAct = new SpinICRunAction();
    runManager->SetUserAction(runAct);

    // Stacking Action
    SpinICStackingAction* stackingAction = new SpinICStackingAction();
    runManager->SetUserAction(stackingAction);
    
    // set user action classes
    runManager->SetUserAction(new SpinICPrimaryGeneratorAction());
    
    // Initialize G4 kernel
    //
    runManager->Initialize();
    
#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    if (argc!=1) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        // interactive mode : define UI session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
        UImanager->ApplyCommand("/control/execute init.mac");
#endif
        ui->SessionStart();
        delete ui;
#endif
    }
    
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;
    
    return 0;
}
