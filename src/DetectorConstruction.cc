#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Element.hh"
#include "G4SDManager.hh"
#include "CrystalSensitiveDetector.hh"
#include "RicochetADR.hh"

#include <cstdlib>
#include <cmath>

using namespace std;

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 detectorMessenger(new DetectorMessenger(this)),
 outerDetectorMaterial(G4Material::GetMaterial("G4_AIR"))
{}

DetectorConstruction::~DetectorConstruction() { delete detectorMessenger; }

G4VPhysicalVolume*  DetectorConstruction::Construct()  {
  InitializeWorld();
	ConstructPit(); //Rock Surrounding the Detector
	ConstructOuterDetectors(); //Air around the crystal
  auto phystest = ConstructDetectors();

  return phystest;
}

void DetectorConstruction::InitializeWorld()  {
  constexpr auto worldSize  = 20.*m;
  auto solidWorld = new G4Box("world", worldSize/2., worldSize/2., worldSize/2.);
  logicWorld = new G4LogicalVolume(solidWorld,
																	G4Material::GetMaterial("G4_Galactic"),
																	"world");
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  physicalWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "world", 0,
																		false, 0, true);
}

void DetectorConstruction::ConstructPit()  {
	if(!physicalWorld)
	{
		G4cout << "ERROR in DetectorConstruction::ConstructPit" << G4endl;
		G4cout << "World volume does not exist!!" << G4endl;
		return;
	}
	const G4ThreeVector vec_offset(0*mm,0*mm,750*mm);
	const G4ThreeVector concrete_offset(0*mm,0*mm,100*mm);
	constexpr double zeroradius = 0.*cm;
	constexpr double startAngle = 0.*deg;
	constexpr double spanningAngleFull = 360.*deg;

  auto solidRock = new G4Box("Rock", 6.*m, 6.*m, 5.*m);
	auto PitTube = new G4Tubs("Pittube", zeroradius, 4350*mm, 8500*mm/2.0,
														startAngle, spanningAngleFull);
	auto Jacket2Tube = new G4Tubs("JacketShell21", zeroradius, 4250*mm,
																8400*mm/2.0,startAngle, spanningAngleFull);
	auto Jacket21Tube = new G4Tubs("JacketShell21", zeroradius, 4350*mm,
																 8500*mm/2.0,startAngle, spanningAngleFull);
	auto PitSolid = new G4SubtractionSolid("ChoozPit", solidRock, PitTube, 0,
																				 vec_offset);
	auto PitLog = new G4LogicalVolume(PitSolid,
																		G4Material::GetMaterial("ChoozRock"),
																		"logicalRock");
	new G4PVPlacement(0, -vec_offset, PitLog, "PVRock", logicWorld, false, 0, true);
	auto PitJacketSolid = new G4SubtractionSolid("ChoozPit_ConcreteJacket",
																							 Jacket21Tube, Jacket2Tube, 0,
																							 concrete_offset);
	auto PitJacketLog = new G4LogicalVolume(PitJacketSolid,
																					G4Material::GetMaterial("G4_CONCRETE"),
																					"logicalConcrete");
	new G4PVPlacement(0, G4ThreeVector(0), PitJacketLog, "ConcreteJacket",
										logicWorld, false, 0, true);

	G4VisAttributes visRock(G4Colour(0.398,0.199,0.));
	visRock.SetForceWireframe(true);
	PitLog->SetVisAttributes(visRock);
	G4VisAttributes visJacket(G4Colour(0.625,0.625,0.625));
	visJacket.SetForceWireframe(true);
	PitJacketLog->SetVisAttributes(visJacket);

}

void DetectorConstruction::ConstructOuterDetectors() {
	constexpr double zeroradius = 0.*cm;
	constexpr double startAngle = 0.*deg;
	constexpr double spanningAngleFull = 360.*deg;
	// Offset include to have top of water shield line up with top of pit
	const G4ThreeVector vec_zero(0*mm,0*mm,50*mm);
	const G4ThreeVector vec_offset(0*mm,0*mm,+1000*mm);

	auto TempTube_inner = new G4Tubs("Temp_inner", zeroradius, 1000.0*mm,
																	 3200.0*mm,startAngle, spanningAngleFull);
	auto TempTube_outer = new G4Tubs("Temp_outer", zeroradius, 4250.0*mm,
																	 4200*mm,startAngle, spanningAngleFull);
	auto tubeSolid = new G4SubtractionSolid("OuterDetector", TempTube_outer,
																					TempTube_inner,0,vec_offset);
	tubeLog = new G4LogicalVolume(tubeSolid,
																		 outerDetectorMaterial,
																		 "OuterDetector");
	new G4PVPlacement(0,vec_zero, tubeLog, "OuterDetector",logicWorld, false,0,true);

	G4VisAttributes visTube(G4Colour(0,0,1));
	visTube.SetForceWireframe(true);
	visTube.SetForceAuxEdgeVisible(true);
	tubeLog->SetVisAttributes(visTube);
}

void DetectorConstruction::AddLayer(const std::string& material, const double thickness) {
		shieldBuilder.AddG4Box(material, thickness, logicWorld);
}

void DetectorConstruction::ConstructADR()  {
	detectorcomponents::ConstructADR(logicWorld);
}

G4VPhysicalVolume*  DetectorConstruction::ConstructDetectors()  {
	constexpr double DetectorSize = 5.2*cm; // Size needed for 1 Kg of detector material
	std::vector<std::string> CyrstalLabels = {"Crystal_1", "Crystal_2","Crystal_3",
																						"Crystal_4","Crystal_5"};
	std::vector<G4double> Cyrstalpos_x = {0.*cm, -6.*cm, 6.*cm, 6.*cm, -6.*cm};
	std::vector<G4double> Cyrstalpos_y = {0.*cm, 6.*cm, 6.*cm, -6.*cm, -6.*cm};
	std::vector<G4double> Cyrstalpos_z = {0.*cm, 0.*cm, 0.*cm, 0.*cm, 0.*cm};
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
	const G4ThreeVector vec_offset(0*mm,0*mm,0.5*cm+DetectorSize/2.0);
	G4VisAttributes visDetector(G4Colour(0.0,1.0,0.0));
	visDetector.SetForceWireframe(true);
	visDetector.SetForceAuxEdgeVisible(true);
	for(size_t jCrys = 0; jCrys < 1; ++jCrys)
	{
		auto solid = new G4Box(CyrstalLabels[jCrys],DetectorSize/2.0,DetectorSize/2.0,DetectorSize/2.0);
		v_CrystalBoxesLog.push_back(new G4LogicalVolume(solid, G4Material::GetMaterial("G4_Os"),CyrstalLabels[jCrys]));
		G4ThreeVector crys_pos(Cyrstalpos_x[jCrys],Cyrstalpos_y[jCrys],Cyrstalpos_z[jCrys]);
		new G4PVPlacement(0,crys_pos+vec_offset,v_CrystalBoxesLog[jCrys],CyrstalLabels[jCrys],logicWorld,false,0,true);

    v_CrystalBoxesLog[jCrys]->SetVisAttributes(visDetector);

    if (jCrys==0) {
			CrystalSensitiveDetector *Crystal1_SD = new CrystalSensitiveDetector("Crystal_1SD");
			G4SDManager::GetSDMpointer()->AddNewDetector(Crystal1_SD);
			v_CrystalBoxesLog[jCrys]->SetSensitiveDetector(Crystal1_SD);
		}
	}
	return physicalWorld;
}

G4VPhysicalVolume*  DetectorConstruction::ConstructSingleDetector()  {
	// arbitrarily assuming 4-inch iZIP here [AJA]
	// Taking out 4-inch iZIP for now [AFL - March 2016]
	constexpr double DetectorSize = 5.2*cm;
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
	auto crystalBox_single = new G4Box("Single Crystal", DetectorSize/2.0, DetectorSize/2.0,DetectorSize/2.0);
	auto crystalLog_single = new G4LogicalVolume(crystalBox_single,G4Material::GetMaterial("Zn"), "Single Crystal");
	new G4PVPlacement(0, vec_zero, crystalLog_single, "Single Crystal",logicWorld, false,0,true);
	G4VisAttributes visDetector(G4Colour(0.0,1.0,0.0));
	visDetector.SetForceWireframe(true);
	visDetector.SetForceAuxEdgeVisible(true);
	crystalLog_single->SetVisAttributes(visDetector);
	return physicalWorld;
}

G4VPhysicalVolume*  DetectorConstruction::ConstructNuDetector()  {
	constexpr double startAngle = 0.*deg;
	constexpr double spanningAngleFull = 360.*deg;
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);

	auto TargetDetectorBox = new G4Tubs("NuDetector",0*m,1150*mm,1221*mm,startAngle, spanningAngleFull);
	auto TargetDetectorLog = new G4LogicalVolume(TargetDetectorBox, G4Material::GetMaterial("NuTargetLiquid"),"NuDetector");
	new G4PVPlacement(0,vec_zero,TargetDetectorLog,"NuDetector",logicWorld,false,0,true);

  G4VisAttributes visDetector(G4Colour(0.0,1.0,0.0));
	TargetDetectorLog->SetVisAttributes(visDetector);
  visDetector.SetForceWireframe(true);
  visDetector.SetForceAuxEdgeVisible(true);

	auto NuDetector = new CrystalSensitiveDetector("NuDetector");
	G4SDManager::GetSDMpointer()->AddNewDetector(NuDetector);
	TargetDetectorLog->SetSensitiveDetector(NuDetector);

  return physicalWorld;
}

void DetectorConstruction::setOuterDetectorMaterial(const std::string& mat) {
  if (tubeLog) tubeLog->SetMaterial(G4Material::GetMaterial(mat));
}

void DetectorConstruction::SetCrystalMaterial(G4String Material)  {
  if (Material == "Os" || Material == "Zn" || Material == "Zr") {
    std::string crystalmaterial = Material;
    if (Material == "Os") {
      for (int i=0;i<1;i++){v_CrystalBoxesLog[i]->SetMaterial(G4Material::GetMaterial("G4_Os"));}
    }
    if (Material == "Zn") {
      for (int i=0;i<1;i++){v_CrystalBoxesLog[i]->SetMaterial(G4Material::GetMaterial("G4_Zn"));}
    }
    if (Material == "Zr") {
      for (int i=0;i<1;i++){v_CrystalBoxesLog[i]->SetMaterial(G4Material::GetMaterial("G4_Zr"));}
    }
  } else {
    std::cerr << "ERROR in SetCrystalMaterial source type must be Os or Zn setting to Os!" << std::endl;
    for (int i=0;i<1;i++) {
      v_CrystalBoxesLog[i]->SetMaterial(G4Material::GetMaterial("G4_Os"));
    }
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
