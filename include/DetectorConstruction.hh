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
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	// Builds the world, rock pit, water and/or air inside rock pit,
	// and the crystal detector. Can be used as a place to build the
	// shieldings or ADR.
	virtual G4VPhysicalVolume* Construct() ;

	// Adds a G4Box of a given material and thickness around the origin.
	// Can be called multiple times to create multiple layers without
	// specifing positions of each one.
	// Example: // Creates two 50mm thick Pb G4Boxes less than a predefined max
	//          // boxLength defined in the Shieldings class.
	//		detectorcomponents::LayerConstructor shieldBuilder; // In header
	//		AddLayer("G4_Pb",50*mm,"G4Box");
	//		AddLayer("G4_Pb",50*mm,"G4Box");
	void AddLayer(const std::string& material, double thickness);

private:
	// Creates the world. Fills the logicWorld and physicalWorld variables.
	void InitializeWorld() ;
	// Rock pit made of rock near the Chooz near site
	void ConstructPit() ;
	// Water or Air tube that surrounds the inner crystal detector
	void ConstructOuterDetectors() ;
	// Original Fridge surrounding the crystal detector
	void ConstructADR() ;
	G4VPhysicalVolume*  ConstructDetectors() ;
	G4VPhysicalVolume*  ConstructSingleDetector() ;
	G4VPhysicalVolume*  ConstructNuDetector() ;

	DetectorMessenger* 							detectorMessenger;
	G4LogicalVolume*								logicWorld;
	G4VPhysicalVolume* 						  physicalWorld;
	G4Material* 										outerDetectorMaterial;
	std::vector<G4Box*>	            v_CrystalBoxes;
	std::vector<G4LogicalVolume*>	  v_CrystalBoxesLog;
	std::vector<G4VPhysicalVolume*>	v_CrystalBoxesPhys;
	detectorcomponents::LayerConstructor shieldBuilder;

public:
	// Changes the center crystal detector material to Os, Zn. or Zr
	void SetCrystalMaterial(G4String Material) ;
	void setOuterDetectorMaterial(const std::string& mat) {
		outerDetectorMaterial = G4Material::GetMaterial(mat);
	}
	G4LogicalVolume* GetWorldVolume() {return logicWorld;}
};


#endif // DetectorConstruction_h
