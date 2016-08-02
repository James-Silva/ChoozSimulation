#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction*);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step*);

private:
  EventAction*          fEventAction;
};

#endif
