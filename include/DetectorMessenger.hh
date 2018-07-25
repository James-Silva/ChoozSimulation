#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    virtual ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction* 		  fDetector;
    G4UIdirectory*             	fDetDir;
    G4UIdirectory*             	fshieldDir;
    G4UIcmdWith3VectorAndUnit*  shieldingCmdPb;
    G4UIcmdWith3VectorAndUnit*  shieldingCmdPoly;
    G4UIcmdWithAString*         setcrystalmaterial;
    G4UIcmdWithAString*         outerDetectorMaterialCmd;
    G4UIcmdWithABool*           condtructADRCmd;
    double                      layerThickness;
    G4UIcmdWithADoubleAndUnit*  setLayerThicknessCmd;
    G4UIcmdWithADoubleAndUnit*  setLayerLengthCmd;
    G4UIcmdWithAString*         addLayerWithMaterialCmd;
};


#endif // DetectorMessenger_h
