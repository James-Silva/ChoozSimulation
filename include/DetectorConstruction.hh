#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "Shieldings.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Cons;
class G4Tubs;
class G4Material;
class G4Element;
class DetectorMessenger;

// Necessary Geant4 class that inherits the G4VUserDetectorConstruction.
//
// Example:
//		// In the main of the program
//		G4RunManager runManager;
//		runManager.SetUserInitialization(new DetectorConstruction());
//
// Builds the world volume and the rock pit, concrete jacket, water/air pit,
// and the crystal detector. Shieldings around the detector can be created
// using macro commands with the messenger.
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	// Builds the world, rock pit, water and/or air inside rock pit,
	// and the crystal detector. Can be used as a place to build the
	// shieldings or ADR.
	virtual G4VPhysicalVolume* Construct() ;

private:
	// Creates the world. Fills the logicWorld and physicalWorld variables.
	void InitializeWorld();
	// Rock pit made of rock near the Chooz near site
	void ConstructPit();
	// Water or Air or Galactic tube that surrounds the inner crystal detector
	void ConstructOuterDetectors();
	// Produces an array of crystals that are sensitve detectors
	G4VPhysicalVolume*  ConstructDetectors();
	// Produces a single crystal that is a sensitve detector
	G4VPhysicalVolume*  ConstructSingleDetector();
	G4VPhysicalVolume*  ConstructNuDetector();

	DetectorMessenger* 									 detectorMessenger;
	G4LogicalVolume*										 logicWorld;
	G4VPhysicalVolume* 						 		   physicalWorld;
	G4LogicalVolume*										 outerLog;
	std::vector<G4LogicalVolume*>	  		 v_CrystalBoxesLog;

public:
	// Changes the center crystal detector material to Os, Zn. or Zr
	void SetCrystalMaterial(G4String Material);
	// Sets the logical volume of the outer detector (tubeLog)
	void setOuterDetectorMaterial(const std::string& mat) {
	  if (outerLog) outerLog->SetMaterial(G4Material::GetMaterial(mat));
		std::cout << outerLog->GetMaterial() << std::endl;
	}
	G4LogicalVolume* GetWorldVolume() {return logicWorld;}
};


#endif // DetectorConstruction_h
