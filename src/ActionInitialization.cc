#include "ActionInitialization.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"


ActionInitialization::ActionInitialization(DetectorConstruction* detector)
 : G4VUserActionInitialization(),
   fDetector(detector)
{}


ActionInitialization::~ActionInitialization()
{}


void ActionInitialization::BuildForMaster() const
{
  // Histo manager
  HistoManager*  histo = new HistoManager();

  // Actions
  SetUserAction(new RunAction(histo));
}


void ActionInitialization::Build() const
{
  // Histo manager
  HistoManager*  histo = new HistoManager();

  // Actions
  SetUserAction(new PrimaryGeneratorAction(fDetector, histo));

  RunAction* runAction = new RunAction(histo);
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction(runAction, histo);
  SetUserAction(eventAction);

  SteppingAction* steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
