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

	// The G4Box created in AddLayer subtracts its thickness from a default value
	// of 1m. That value is changed with SetLayerLength and this method is called
	// from the messenger class. It must be called before AddLayer to effect the
	// layer that is being added.
	void SetLayerLength(double length);

	// Original Fridge surrounding the crystal detector
	void ConstructADR() ;

private:
	// Creates the world. Fills the logicWorld and physicalWorld variables.
	void InitializeWorld() ;
	// Rock pit made of rock near the Chooz near site
	void ConstructPit() ;
	// Water or Air tube that surrounds the inner crystal detector
	void ConstructOuterDetectors() ;
	G4VPhysicalVolume*  ConstructDetectors() ;
	G4VPhysicalVolume*  ConstructSingleDetector() ;
	G4VPhysicalVolume*  ConstructNuDetector() ;

	DetectorMessenger* 									 detectorMessenger;
	G4LogicalVolume*										 logicWorld;
	G4VPhysicalVolume* 						 		   physicalWorld;
	G4Material* 											   outerDetectorMaterial;
	G4LogicalVolume*										 tubeLog;
	std::vector<G4LogicalVolume*>	  		 v_CrystalBoxesLog;
	detectorcomponents::LayerConstructor shieldBuilder;

public:
	// Changes the center crystal detector material to Os, Zn. or Zr
	void SetCrystalMaterial(G4String Material) ;
	void setOuterDetectorMaterial(const std::string& mat);
	G4LogicalVolume* GetWorldVolume() {return logicWorld;}
};


#endif // DetectorConstruction_h
