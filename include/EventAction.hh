#ifndef EventAction_h
#define EventAction_h 1

#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class HistoManager;



class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  virtual void    BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);

  void accumulateEdep_Veto(G4double dEdep);
  void accumulateEdep_Nudetector(G4double dEdep);

private:
   RunAction*       fRunAct;
   HistoManager*    fHistoManager;

   G4double         fedep_veto;
   G4double         fedep_nudetector;
   G4double         fedep_nudetector_crosscheck;

   G4int            fPrintModulo;
   G4int            CrystalHCID;  
};



#endif
