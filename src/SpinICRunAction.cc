//
//
//
#include "SpinICRunAction.hh"
#include "SpinICRunMessenger.hh"
#include "SpinICAnalysis.hh"

#include "G4Run.hh"


SpinICRunAction::SpinICRunAction()
{
    runMessenger = new SpinICRunMessenger(this);
    
}

SpinICRunAction::~SpinICRunAction()
{
    delete runMessenger;
}

void SpinICRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
        
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Open an output file and create nTuple
    //
    if(fileName==""){fileName = "SpinICTest";}
    analysisManager->OpenFile(fileName);
    
    analysisManager->CreateNtuple("SpinIC", "Gammas produced in magnets");

    analysisManager->CreateNtupleDColumn("Energy");

    analysisManager->CreateNtupleDColumn("PosX");
    analysisManager->CreateNtupleDColumn("PosY");
    analysisManager->CreateNtupleDColumn("PosZ");
    
    analysisManager->CreateNtupleDColumn("Charge");
    analysisManager->CreateNtupleIColumn("PDGID");
    analysisManager->CreateNtupleIColumn("StepNum");
    analysisManager->CreateNtupleIColumn("Process");
    //analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleIColumn("Time");
    analysisManager->CreateNtupleIColumn("ICNumber");
    
    analysisManager->FinishNtuple();

}

void SpinICRunAction::EndOfRunAction(const G4Run* aRun)
{
    //Output filename
    G4cout << "Filename = " << fileName << G4endl;
    
    G4int nofEvents = aRun->GetNumberOfEvent();
    if ( nofEvents == 0 ) return;
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // save histograms
    //
    analysisManager->Write();
    analysisManager->CloseFile();
    
    //timer->Stop();
    //G4cerr << "Real Elapsed Time:  " << timer->GetRealElapsed() << G4endl;
    
    // complete cleanup
    //
    delete G4AnalysisManager::Instance();

}