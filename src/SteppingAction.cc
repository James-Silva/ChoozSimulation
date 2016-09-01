#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"


SteppingAction::SteppingAction(EventAction* evt)
: G4UserSteppingAction(), fEventAction(evt)
{ }


SteppingAction::~SteppingAction()
{ }


void SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
  // get volume and particle name of the current step
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4String name = aStep->GetTrack()->GetDefinition()->GetParticleName();
  // collect energy and track length step by step
  G4double d_stepenergy = aStep->GetTotalEnergyDeposit()/keV;
  //aStep->GetTotalEnergyDeposit()/keV
 
  if(volume->GetName() == "Veto" && d_stepenergy > 0)
  {
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
    //std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep_Veto(d_stepenergy);
  }
  if(volume->GetName() == "Crystal_1" && d_stepenergy > 0)
  {
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
    //std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep_detector(d_stepenergy);
  }

}
