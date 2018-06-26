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
using namespace std;

DetectorMessenger::DetectorMessenger( DetectorConstruction* Det)
: G4UImessenger(), fDetector(Det)
{
  fDetDir = new G4UIdirectory("/ricochetchoozsim/detector/");
  fDetDir->SetGuidance("detector control");
  fshieldDir = new G4UIdirectory("/ricochetchoozsim/shielding/");
  fshieldDir->SetGuidance("shielding control");
  shieldingCmdPb = new G4UIcmdWith3VectorAndUnit("/ricochetchoozsim/shielding/setPbshielding", this);
  shieldingCmdPb->SetGuidance("Determine inner and outer shielding (syntax: inner radius outer radius top thickness cm)");
  shieldingCmdPoly = new G4UIcmdWith3VectorAndUnit("/ricochetchoozsim/shielding/setPolyshielding", this);
  shieldingCmdPoly->SetGuidance("Determine inner and outer shielding (syntax: inner radius outer radius top thickness cm)");
  fTypeCmd_setcrystalmaterial = new G4UIcmdWithAString("/ricochetchoozsim/detector/setcrystalmaterial",this);
  fTypeCmd_setcrystalmaterial->SetGuidance("Set Crystal Material (Zn,Zr or Default = Os)");
  fTypeCmd_setcrystalmaterial->SetDefaultValue("Os");
}


DetectorMessenger::~DetectorMessenger()
{
  delete fDetDir;
  delete fshieldDir;
  delete shieldingCmdPb;
  delete shieldingCmdPoly;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == shieldingCmdPb)
  {
    DetectorComponents::ConstructPbSheilding(
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getX(),
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getY(),
        shieldingCmdPb->GetNew3VectorRawValue(newValue).getZ(),
        fDetector->GetWorldVolume());
  }
  if(command == shieldingCmdPoly)
  {
    DetectorComponents::ConstructPolySheilding(
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getX(),
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getY(),
        shieldingCmdPoly->GetNew3VectorRawValue(newValue).getZ(),
        fDetector->GetWorldVolume());
  }
	if(command == fTypeCmd_setcrystalmaterial)
  {
    fDetector->SetCrystalMaterial(newValue);
  }

}
