#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "RicochetADR.hh"

using namespace std;

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
    :G4UImessenger(), fDetector(Det)
{
  fDetDir = new G4UIdirectory("/ricochetchoozsim/detector/");
  fDetDir->SetGuidance("detector control");
  //////////////////////////////////////////////////////////////////////////////
  fShieldDir = new G4UIdirectory("/ricochetchoozsim/shielding/");
  fShieldDir->SetGuidance("shielding control");
  //////////////////////////////////////////////////////////////////////////////
  shieldingCmdPb =
    new G4UIcmdWith3VectorAndUnit("/ricochetchoozsim/shielding/setPbshielding",
                                  this);
  shieldingCmdPb->
    SetGuidance("Determine inner and outer shielding (syntax: inner radius outer radius top thickness cm)");
  //////////////////////////////////////////////////////////////////////////////
  shieldingCmdPoly =
    new G4UIcmdWith3VectorAndUnit("/ricochetchoozsim/shielding/setPolyshielding",
                                  this);
  shieldingCmdPoly->
    SetGuidance("Determine inner and outer shielding (syntax:inner radius outer radius top thickness cm)");
  //////////////////////////////////////////////////////////////////////////////
  setcrystalmaterial =
    new G4UIcmdWithAString("/ricochetchoozsim/detector/setcrystalmaterial",
                            this);
  setcrystalmaterial->SetGuidance("Set Crystal Material (Zn,Zr or Default = Os)");
  setcrystalmaterial->SetDefaultValue("Os");

  /////////////Commands to add layers areound the cyrstal///////////////////////
  /// Example ///
  // /ricochetchoozsim/shielding/setCavityLength 30. cm
  // /ricochetchoozsim/shielding/setLayerThickness 5 cm
  // /ricochetchoozsim/shielding/addLayerWithMaterial Pol
  // /ricochetchoozsim/shielding/addLayerWithMaterial G4_WATER
  // /ricochetchoozsim/shielding/add_U_LayerWithMaterial G4_Pb
  setCavityLengthCmd =
    new G4UIcmdWithADoubleAndUnit("/ricochetchoozsim/shielding/setCavityLength",
                                  this);
  setCavityLengthCmd->
    SetGuidance("Set the cavity length before a layers are added to it. Default 1m.");

  setLayerThicknessCmd =
    new G4UIcmdWithADoubleAndUnit("/ricochetchoozsim/shielding/setLayerThickness",
                                  this);
  setLayerThicknessCmd->
    SetGuidance("Set the layer thickness. Must be done before setting the material.");

  addLayerWithMaterialCmd =
    new G4UIcmdWithAString("/ricochetchoozsim/shielding/addLayerWithMaterial",
                           this);
  addLayerWithMaterialCmd->
    SetGuidance("Add a Layer centered around the origin of the world.");

  add_U_LayerWithMaterialCmd =
    new G4UIcmdWithAString("/ricochetchoozsim/shielding/add_U_LayerWithMaterial",
                           this);
  add_U_LayerWithMaterialCmd->
    SetGuidance("Add a Layer centered around the origin of the world.");

  ////////////////////////Construct the ADR/////////////////////////////////////
  condtructADRCmd =
    new G4UIcmdWithABool("/ricochetchoozsim/detector/constructADR", this);
  condtructADRCmd->SetGuidance("Construct ADR (true or false)");
  condtructADRCmd->SetDefaultValue("false");

  ////Commands to set the material from the outer rock to the inner crystal/////
  outerDetectorMaterialCmd =
    new G4UIcmdWithAString("/ricochetchoozsim/detector/setOuterDetectorMaterial",
                           this);
  outerDetectorMaterialCmd->
    SetGuidance("Set Outer Detector Material. Default Air.");
}

DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fShieldDir;
  delete shieldingCmdPb;
  delete shieldingCmdPoly;
  delete setLayerThicknessCmd;
  delete addLayerWithMaterialCmd;
  delete add_U_LayerWithMaterialCmd;
  delete outerDetectorMaterialCmd;
  delete condtructADRCmd;
  delete setCavityLengthCmd;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == shieldingCmdPb) {
    detectorcomponents::ConstructPbSheilding(
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getX(),
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getY(),
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getZ(),
        fDetector->GetWorldVolume());
  }
  else if (command == shieldingCmdPoly) {
    detectorcomponents::ConstructPolySheilding(
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getX(),
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getY(),
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getZ(),
        fDetector->GetWorldVolume());
  }
	else if (command == setcrystalmaterial) {
    fDetector->SetCrystalMaterial(newValue);
  }
  else if (command == setCavityLengthCmd) {
    fShieldBuilder.SetCavityLength(setCavityLengthCmd->GetNewDoubleValue(newValue));
  }
  else if (command == setLayerThicknessCmd) {
    fShieldBuilder.SetLayerThickness(setLayerThicknessCmd->GetNewDoubleValue(newValue));
  }
  else if (command == addLayerWithMaterialCmd) {
    fShieldBuilder.AddLayer(newValue, fDetector->GetWorldVolume());
  }
  else if (command == add_U_LayerWithMaterialCmd) {
    fShieldBuilder.AddULayer(newValue, fDetector->GetWorldVolume());
  }
  else if (command == outerDetectorMaterialCmd) {
    fDetector->setOuterDetectorMaterial(newValue);
  }
  else if(command == condtructADRCmd && newValue == "true") {
    detectorcomponents::ConstructADR(fDetector->GetWorldVolume());
  }
}
