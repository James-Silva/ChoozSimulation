#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "Shieldings.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"

using namespace std;

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
    :G4UImessenger(), fDetector(Det), layerThickness(0)
{
  fDetDir = new G4UIdirectory("/ricochetchoozsim/detector/");
  fDetDir->SetGuidance("detector control");
  //////////////////////////////////////////////////////////////////////////////
  fshieldDir = new G4UIdirectory("/ricochetchoozsim/shielding/");
  fshieldDir->SetGuidance("shielding control");
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
  // /ricochetchoozsim/shielding/setLayerLength 1. m        //Sets the layer length that the new layer is subtracted from
  // /ricochetchoozsim/shielding/setLayerThickness 50 mm   //Sets the thickness of the Layers added
  // /ricochetchoozsim/shielding/addLayerWithMaterial Pol  //Adds the layers with a specified material (1m->.95m)
  setLayerLengthCmd =
    new G4UIcmdWithADoubleAndUnit("/ricochetchoozsim/shielding/setLayerLength",
                                  this);
  setLayerLengthCmd->
    SetGuidance("Set the layer length before a layer is subtracted from it. Default 1m.");

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

  ////Commands to set the material from the outer rock to the inner crystal/////
  outerDetectorMaterialCmd =
    new G4UIcmdWithAString("/ricochetchoozsim/detector/setOuterDetectorMaterial",
                           this);
  outerDetectorMaterialCmd->
    SetGuidance("Set Outer Detector Material. Default Air.");

  ////////////////////////Construct the ADR/////////////////////////////////////
  condtructADRCmd =
    new G4UIcmdWithABool("/ricochetchoozsim/detector/constructADR", this);
  condtructADRCmd->SetGuidance("Construct ADR (true or false)");
  condtructADRCmd->SetDefaultValue("false");
}


DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fshieldDir;
  delete shieldingCmdPb;
  delete shieldingCmdPoly;
  delete setLayerThicknessCmd;
  delete addLayerWithMaterialCmd;
  delete outerDetectorMaterialCmd;
  delete condtructADRCmd;
  delete setLayerLengthCmd;
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
  else if (command == setLayerThicknessCmd) {
    this->layerThickness = setLayerThicknessCmd->GetNewDoubleValue(newValue);
  }
  else if (command == addLayerWithMaterialCmd) {
    if (layerThickness > 0) fDetector->AddLayer(newValue, layerThickness);
    else std::cerr<<"\n\nThickness not set. Layer not created.\n\n"<<std::endl;
  }
  else if (command == setLayerLengthCmd) {
    fDetector->SetLayerLength(setLayerThicknessCmd->GetNewDoubleValue(newValue));
  }
  else if (command == outerDetectorMaterialCmd) {
    fDetector->setOuterDetectorMaterial(newValue);
  }
  if(command == condtructADRCmd && newValue == "true")
  {
    fDetector->ConstructADR();
  }
}
