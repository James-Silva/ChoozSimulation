#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"


class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Cons;
class G4Tubs;
class G4Material;
class G4Element;
class DetectorMessenger;
class Materials;
// class Shieldings;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct() noexcept;
	void ConstructADR() noexcept;
	void ConstructPit() noexcept;
	void ConstructOuterDetectors() noexcept;
	void InitializeWorld() noexcept;
	void ConstructPolySheilding(const double innerR, const double outerR,
														  const double topthickness) noexcept;
	void ConstructPbSheilding(const double innerR, const double outerR,
													  const double topthickness) noexcept;
	void SetCrystalMaterial(G4String Material) noexcept;
	void AddGioveShielding() noexcept;

private:
	DetectorMessenger* 							detectorMessenger;
	G4LogicalVolume*								logicWorld;
	G4VPhysicalVolume* 						  physicalWorld;
	std::vector<G4Box*>	            v_CrystalBoxes;
	std::vector<G4LogicalVolume*>	  v_CrystalBoxesLog;
	std::vector<G4VPhysicalVolume*>	v_CrystalBoxesPhys;

private:
	G4VPhysicalVolume*  ConstructDetectors() noexcept;
	G4VPhysicalVolume*  ConstructSingleDetector() noexcept;
	G4VPhysicalVolume*  ConstructNuDetector() noexcept;
	void AddConcreteFloor() noexcept;
	void AddCoffinWalls() noexcept;
	void AddConcreteWalls() noexcept;
};


#endif // DetectorConstruction_h
