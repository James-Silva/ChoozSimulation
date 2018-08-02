#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "Shieldings.hh"

class DetectorConstruction;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;
class G4UIcmdWithADoubleAndUnit;

// Defines UI commands that effect detector construction
class DetectorMessenger: public G4UImessenger
{
  public:
    // UI Commands are defined here
    DetectorMessenger(DetectorConstruction* );
    // Deletes the command pointers that were created
    virtual ~DetectorMessenger();
    // The methods entered commands invoke are decided here
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    // Used to hold and update information about the thickness, layer number,
    // cavity length in the layers being created
    detectorcomponents::LayerConstructor fShieldBuilder;
    // Used to access information about the detector used in shield creation
    DetectorConstruction* 		           fDetector;
    // Creates UI directory where certain commands are stored
    G4UIdirectory*                       fDetDir;
    // Creates UI directory where certain commands are stored
    G4UIdirectory*             	         fShieldDir;
    // Creates a lead shielding around the crystal. Dimensions are hard coded
    G4UIcmdWith3VectorAndUnit*           shieldingCmdPb;
    // Creates a poly shielding around the crystal. Dimensions are hard coded
    G4UIcmdWith3VectorAndUnit*           shieldingCmdPoly;
    // Changes the inner crystal. Simulation starts with Os and uses this command
    // to change it at run time
    G4UIcmdWithAString*                  setcrystalmaterial;
    // The outer detector is the area between the rock pit and the crystal.
    // The default is air, however this command changes it at run time.
    G4UIcmdWithAString*                  outerDetectorMaterialCmd;
    // Builds an adiabatic demagnetisation refrigerator with hard coded dimensions
    G4UIcmdWithABool*                    condtructADRCmd;
    // Adds a layer of given material. The thickness must be set before this
    // with setLayerThicknessCmd. Layers are built around a cavity of a size set
    // with the setCavityLengthCmd. Layers are automatically produced on top of
    // each other.
    G4UIcmdWithAString*                  addLayerWithMaterialCmd;
    // Sets the layer thickness that new layers are added with
    G4UIcmdWithADoubleAndUnit*           setLayerThicknessCmd;
    // Sets the length of the cavity that new layers are built around
    G4UIcmdWithADoubleAndUnit*           setCavityLengthCmd;
    // Adds a layer of given material with the top removed.
    // The thickness must be set before this with setLayerThicknessCmd.
    // Layers are built around a cavity of a size set with the
    //setCavityLengthCmd. Layers are automatically produced on top of each other.
    G4UIcmdWithAString*                  add_U_LayerWithMaterialCmd;
};


#endif // DetectorMessenger_h
