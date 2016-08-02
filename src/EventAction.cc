#include "EventAction.hh"

#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"


EventAction::EventAction(RunAction* run, HistoManager* histo)
:G4UserEventAction(),
 fRunAct(run),fHistoManager(histo), fedep(0.),
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
  fedep = 0.;

}


void EventAction::EndOfEventAction(const G4Event*)
{
  if(fedep > 0.)
  {
    fHistoManager->setEnergy(fedep);
  }
    fHistoManager->fill();
}


void EventAction::accumulateEdep(G4double dEdep)
{
  fedep += dEdep;
}
