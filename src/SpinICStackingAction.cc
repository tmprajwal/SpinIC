#include "SpinICStackingAction.hh"
#include "SpinICAnalysis.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"
#include "Randomize.hh"
#include "G4Pow.hh"
#include <time.h>


SpinICStackingAction::SpinICStackingAction()
{
    G4cout << "********** Stacking Action Created *************" << G4endl;
}

SpinICStackingAction::~SpinICStackingAction()
{
    
}

G4ClassificationOfNewTrack SpinICStackingAction::ClassifyNewTrack(const G4Track * track)
{
    G4int PDG = track->GetParticleDefinition()->GetPDGEncoding();
    G4int parentID = track->GetParentID();
    G4int trackID = track->GetTrackID();
    G4int stepNum = track->GetCurrentStepNumber();

    G4int icnum = 3; //(1: Left IC, 2: right IC)
    G4int MagnetNumber = 0;
    G4ThreeVector Pos = track->GetPosition();

    G4double genergy = track->GetKineticEnergy()/MeV;
    G4double chrg = track->GetParticleDefinition()->GetPDGCharge();   
    G4double prodtm = track->GetGlobalTime();
    G4int prodstm = time(NULL);

    if (PDG == 22) {
        if (Pos.z()/m > -5.2 && Pos.z()/m < -4.9) {MagnetNumber = 1;}
        else if (Pos.z()/m > -6.85 && Pos.z()/m < -5.55) {MagnetNumber = 2;}
        else if (Pos.z()/m > -8.5 && Pos.z()/m < -8.2) {MagnetNumber = 3;}
        else {MagnetNumber = 5;}
    }

    // Get the creator process of the track that deposits energy
    const G4VProcess* edepCreatorProcess = track->GetCreatorProcess();
    G4ProcessType creatorProcessType = fNotDefined;
    G4int creatorProcessSubType = 0;
    if(edepCreatorProcess)
    {
        //edepCreatorProcess->DumpInfo();
        creatorProcessType = edepCreatorProcess->GetProcessType();
        creatorProcessSubType = edepCreatorProcess->GetProcessSubType();
    }
    G4int cpTypeCode = (G4int)creatorProcessType*10000 + creatorProcessSubType;

#if 0
    G4double phasymmetry = (4.543E-7) + (0.0000572046*genergy) + (-0.0000306751*std::pow(genergy,2)) + (0.0000138074*std::pow(genergy,3)) + (-3.37522E-6*std::pow(genergy,4)) + (3.29347E-7*std::pow(genergy,5));
    G4double asymmetry = 1. - phasymmetry;
    //G4double asymmetry = 1.; 
    
    //up-down asymmetry
    if (MagnetNumber==1 || MagnetNumber==3)
    {
        if (track->GetMomentumDirection().y() < 0. &&
        G4UniformRand() < asymmetry) {return fKill;}
    }
    else if (MagnetNumber==2)
    {
        if (track->GetMomentumDirection().y() > 0. &&
        G4UniformRand() < asymmetry) {return fKill;}
    }
    //Energy Spread asym
    if (G4UniformRand() > phasymmetry) {return fKill;}*/
#endif
    
    // Determine which IC based on xyz-position
      
    if (((Pos.z()/m >= 4.4) && (Pos.z()/m <= 6.6)) && ((Pos.y()/m >=-0.225) && (Pos.y()/m <= 0.225)))
    {
		 //icnum = 4;
         if ((Pos.x()/m >= -0.36) && (Pos.x()/m <= 0.0)){ icnum = 1; }
         else if ((Pos.x()/m <= 0.36) && (Pos.x()/m >= 0.0)){ icnum = 2; }	
    }
       
    if(PDG!=22 && (icnum==1 || icnum==2) /*genergy > 0.465 && parentID<=1000*/)
    {	
    // Get AnalysisManager to fill ntuples/histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        
    analysisManager->FillNtupleDColumn(0, genergy);
        
    analysisManager->FillNtupleDColumn(1, Pos.x()/m);
    analysisManager->FillNtupleDColumn(2, Pos.y()/m);
    analysisManager->FillNtupleDColumn(3, Pos.z()/m);
      
    analysisManager->FillNtupleDColumn(4, chrg);
    analysisManager->FillNtupleIColumn(5, PDG);
    analysisManager->FillNtupleIColumn(6, parentID);
    analysisManager->FillNtupleIColumn(7, cpTypeCode);
    //analysisManager->FillNtupleDColumn(8, prodtm);
    analysisManager->FillNtupleIColumn(8, MagnetNumber);
    analysisManager->FillNtupleIColumn(9, trackID);
        
    analysisManager->AddNtupleRow();
    }
    return fUrgent;
}
