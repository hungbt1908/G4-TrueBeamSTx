#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
// #include "g4root.hh"
#include "G4EventManager.hh"

SteppingAction::SteppingAction()
    : G4UserSteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto analysisManager = G4AnalysisManager::Instance();


    if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "physTarget")
    {
        G4Track* track = dynamic_cast<G4Track*>(step->GetTrack());
        G4ParticleDefinition* particleDef = track->GetDefinition();
        G4String projname = particleDef->GetParticleName();

        if(projname == "e-")
        {
            G4StepPoint* prePoint = dynamic_cast<G4StepPoint*>(step->GetPreStepPoint());
            G4StepPoint* postPoint = dynamic_cast<G4StepPoint*>(step->GetPostStepPoint());
            const G4String &procName = postPoint->GetProcessDefinedStep()->GetProcessName();

            if(procName == "eBrem")
            {
                G4int eventID = (G4EventManager::GetEventManager())->GetConstCurrentEvent()->GetEventID(); // get eventID
                G4int trackID = step->GetTrack()->GetTrackID(); // get track ID
                G4int stepNumber = track->GetCurrentStepNumber();
                G4int parentID = track->GetParentID();

                G4double xPos = postPoint->GetPosition().getX();
                G4double yPos = postPoint->GetPosition().getY();
                G4double zPos = postPoint->GetPosition().getZ();

                analysisManager->FillNtupleSColumn(0, 0, projname);
                analysisManager->FillNtupleIColumn(0, 1, eventID);
                analysisManager->FillNtupleIColumn(0, 2, trackID);
                analysisManager->FillNtupleIColumn(0, 3, stepNumber);
                analysisManager->FillNtupleIColumn(0, 4, parentID);
                analysisManager->FillNtupleDColumn(0, 5, xPos);
                analysisManager->FillNtupleDColumn(0, 6, yPos);
                analysisManager->FillNtupleDColumn(0, 7, zPos);
                analysisManager->AddNtupleRow(0);
            }
        }
    }
}