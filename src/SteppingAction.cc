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

  //aStep->GetTotalEnergyDeposit()/keV
 
  if(volume->GetName() == "NuDetector" && name == "neutron")
  {
    //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
  	//std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep(aStep->GetTotalEnergyDeposit()/keV);
  }
}
