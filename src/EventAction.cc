#include "EventAction.hh"
#include "G4SDManager.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"


EventAction::EventAction(RunAction* run, HistoManager* histo)
:G4UserEventAction(),
 fRunAct(run),fHistoManager(histo), fedep_veto(0.),fedep_nudetector(0.),
 fPrintModulo(0)
{
 fPrintModulo = 1000;
}


EventAction::~EventAction()
{ }


void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if (evtNb%fPrintModulo == 0)
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;

  // reset event accumulators
  fedep_nudetector_crosscheck =0;
  fedep_veto =0;
  G4SDManager* SDmanager = G4SDManager::GetSDMpointer();
  CrystalHCID = SDmanager->GetCollectionID("CrystalHitsCollection");

}


void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  if(CrystalHCID > -1)
  {
    G4HCofThisEvent* thisHCE = anEvent->GetHCofThisEvent();
    CrystalHitsCollection* thisHC = 
        thisHCE ? (CrystalHitsCollection*)(thisHCE->GetHC(CrystalHCID)) : 0;
    if (thisHC)
    {
      // run the charge simulation
      fHistoManager->FillTree(anEvent, thisHC);
    }
  }
  fHistoManager->setEnergy_Nudetector(fedep_nudetector_crosscheck);
  fHistoManager->setEnergy_Veto(fedep_veto);
  std::cout << "Edep Veto: " << fedep_veto << std::endl;
  std::cout << "Edep setEnergy_Nudetector_crosscheck: " << fedep_nudetector_crosscheck << std::endl;
  fHistoManager->fill();
}


void EventAction::accumulateEdep_Veto(G4double dEdep)
{
  fedep_veto += dEdep;
}

void EventAction::accumulateEdep_Nudetector(G4double dEdep)
{
  fedep_nudetector_crosscheck += dEdep;
}
