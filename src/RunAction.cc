#include "RunAction.hh"
#include "HistoManager.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo)
: G4UserRunAction(),
  fHistoManager(histo)  
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");  
  fAutoSeed=true;
  if (fAutoSeed)
  {
    // automatic (time-based) random seeds for each run
    G4cout << "*******************" << G4endl;
    G4cout << "*** AUTOSEED ON ***" << G4endl;
    G4cout << "*******************" << G4endl;
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());
    G4Random::setTheSeeds(seeds);
    //G4Random::showEngineStatus();
 }
 else
 {
   //G4Random::showEngineStatus();
 }
 if (fSaveRndm > 0)
 {
   std::ostringstream os;
   //os<<"beginOfRun_"<<G4Threading::G4GetThreadId()<<".rndm";
   G4Random::saveEngineStatus(os.str().c_str());
 }
  //histograms
  //
  fHistoManager->book(aRun); 
}


void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  if (fSaveRndm == 1) 
  {
    G4Random::showEngineStatus();
    std::ostringstream os;
    //os<<"endOfRun_"<<G4Threading::G4GetThreadId()<<".rndm";
    G4Random::saveEngineStatus(os.str().c_str());
  }  
  fHistoManager->save();   
}