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
  G4ThreeVector vec_position(0*mm,0*mm,0*mm);
  // collect energy and track length step by step
  G4double d_stepenergy = aStep->GetTotalEnergyDeposit()/keV;
  //aStep->GetTotalEnergyDeposit()/keV
  if(volume->GetName() == "Crystal_1" && aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
  {
    vec_position = aStep->GetPreStepPoint()->GetPosition();
    fEventAction->storeentrypoint_crystal(vec_position);
    //std::cout << "Crystal Entry point: " << vec_position << std::endl;
  }
  if(volume->GetName() == "WaterShielding" && aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
  {
     //std::cout << "Leaving Water Shielding" << std::endl;
     vec_position = aStep->GetPreStepPoint()->GetPosition();
     //double temp = (vec_position.getX())*(vec_position.getX())+(vec_position.getY())*(vec_position.getY());
     //std::cout << "radius: " << sqrt(temp) << std::endl;
     fEventAction->storeleavepoint_watershield(vec_position);
  }
  if(volume->GetName() == "WaterShielding" && d_stepenergy > 0)
  {
    //std::cout << "Water Shielding Event " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
    //std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep_Veto(d_stepenergy);
  }
  if(volume->GetName() == "Borated_Poly_Shield" && d_stepenergy > 0)
  {
    //std::cout << "Poly Shielding event " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
    //std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep_poly(d_stepenergy);
  }
  if(volume->GetName() == "Crystal_1" && d_stepenergy > 0)
  {
    //std::cout << name << " Hit!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
    //std::cout << "Energy: " << aStep->GetTotalEnergyDeposit()/keV << std::endl;
    //std::cout << "in if statement" << std::endl;
    fEventAction->accumulateEdep_detector(d_stepenergy);
  }

}
