#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
// #include "g4root.hh"

double RunAction::diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
}

RunAction::RunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);

    // NTuple
    analysisManager->CreateNtuple("positron","store positron's information in target");
    analysisManager->CreateNtupleSColumn("partName");
    analysisManager->CreateNtupleIColumn("eventID"); 
    analysisManager->CreateNtupleIColumn("trackID"); 
    analysisManager->CreateNtupleIColumn("stepNumber");
    analysisManager->CreateNtupleIColumn("parentID"); 
    analysisManager->CreateNtupleDColumn("xPos");
    analysisManager->CreateNtupleDColumn("yPos");
    analysisManager->CreateNtupleDColumn("zPos");
    analysisManager->FinishNtuple(0);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    // get time
    time_t beginnow = time(0);
    beginTime = asctime(localtime(&beginnow));
    begin = clock();

    //
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("output.root");
}

void RunAction::EndOfRunAction(const G4Run*)
{
    // get time
    time_t endnow = time(0);
    endTime = asctime(localtime(&endnow));
    end = clock();

    int numberOfWorkerThreads = G4Threading::GetNumberOfRunningWorkerThreads();
    G4RunManager* runManager = G4RunManager::GetRunManager();
    int nofEvents = runManager->GetNumberOfEventsToBeProcessed();
    if(IsMaster())
    {
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                              ==> GEANT4 OUTPUT REPORT <==                            " << G4endl;
        G4cout << "  __________________________________ [ Job Title ] _________________________________  " << G4endl;
        G4cout << " |                                                                                  | " << G4endl;
        G4cout << " | Application for Bremmstrahlung process                                           | " << G4endl;
        G4cout << " | -------------------------------------------------------------------------------- | " << G4endl;
        G4cout << " | author: BUI Tien Hung                                                            | " << G4endl;
        G4cout << " | email : hungbt1908@gmail.com                                                     | " << G4endl;
        G4cout << " | adress: Institute for Nuclear Science and Technology (INST)                      | " << G4endl;
        G4cout << " |__________________________________________________________________________________| " << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                         Starting = " << beginTime                                      << G4endl;
        G4cout << "                         Ending   = " << endTime                                        << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << " # Number of worker threads        : " << numberOfWorkerThreads << " threads"           << G4endl;
        G4cout << " # Number of primary events        : " << nofEvents             << " events"            << G4endl;
        G4cout << " # Elapsed time                    : " << (double(diffclock(end, begin)/1000)) <<  " s" << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                                                                                      " << G4endl;
    }

    // save
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

