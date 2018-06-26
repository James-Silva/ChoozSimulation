#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "Materials.hh"
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
#include "Shieldings.hh"

#include <cstdlib>
#include <cmath>

using namespace std;

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(), detectorMessenger(new DetectorMessenger(this))
{}

DetectorConstruction::~DetectorConstruction() { delete detectorMessenger; }

G4VPhysicalVolume*  DetectorConstruction::Construct() noexcept {
	Materials::CreateMaterials();
  InitializeWorld();
	ConstructPit(); //Rock Surrounding the Detector
	ConstructOuterDetectors(); //Air around the crystal
	//ConstructADR();
	DetectorComponents::ConstructGioveShielding(logicWorld);
  auto phystest = ConstructDetectors();

  return phystest;
}

void DetectorConstruction::InitializeWorld() noexcept {
  constexpr auto worldSize  = 20.*m;
  auto solidWorld = new G4Box("world", worldSize/2., worldSize/2., worldSize/2.);
  logicWorld = new G4LogicalVolume(solidWorld,
																	G4Material::GetMaterial("G4_Galactic"),
																	"world");
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  physicalWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "world", 0,
																		false, 0);
}

void DetectorConstruction::ConstructPit() noexcept {
	if(!physicalWorld)
	{
		G4cout << "ERROR in DetectorConstruction::ConstructPit" << G4endl;
		G4cout << "World volume does not exist!!" << G4endl;
		return;
	}
	const G4ThreeVector vec_offset(0*mm,0*mm,750*mm);
	const G4ThreeVector concrete_offset(0*mm,0*mm,100*mm);
	constexpr int zeroradius = 0.*cm;
	constexpr int startAngle = 0.*deg;
	constexpr int spanningAngleFull = 360.*deg;

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
	new G4PVPlacement(0, -vec_offset, PitLog, "PVRock", logicWorld, false, 0);
	auto PitJacketSolid = new G4SubtractionSolid("ChoozPit_ConcreteJacket",
																							 Jacket21Tube, Jacket2Tube, 0,
																							 concrete_offset);
	auto PitJacketLog = new G4LogicalVolume(PitJacketSolid,
																					G4Material::GetMaterial("G4_CONCRETE"),
																					"logicalConcrete");
	new G4PVPlacement(0, G4ThreeVector(0), PitJacketLog, "ConcreteJacket",
										logicWorld, false, 0);

	G4VisAttributes visRock(G4Colour(0.398,0.199,0.));
	visRock.SetForceWireframe(true);
	PitLog->SetVisAttributes(visRock);
	G4VisAttributes visJacket(G4Colour(0.625,0.625,0.625));
	visJacket.SetForceWireframe(true);
	PitJacketLog->SetVisAttributes(visJacket);

}

void DetectorConstruction::ConstructOuterDetectors() noexcept{
	constexpr int zeroradius = 0.*cm;
	constexpr int startAngle = 0.*deg;
	constexpr int spanningAngleFull = 360.*deg;
	// Offset include to have top of water shield line up with top of pit
	const G4ThreeVector vec_zero(0*mm,0*mm,50*mm);
	const G4ThreeVector vec_offset(0*mm,0*mm,+1000*mm);

	auto TempTube_inner = new G4Tubs("Temp_inner", zeroradius, 1000.0*mm,
																	 3200.0*mm,startAngle, spanningAngleFull);
	auto TempTube_outer = new G4Tubs("Temp_outer", zeroradius, 4250.0*mm,
																	 4200*mm,startAngle, spanningAngleFull);
	auto AirTubeSolid = new G4SubtractionSolid("AirShielding", TempTube_outer,
																						 TempTube_inner,0,vec_offset);
	auto AirTubeLog = new G4LogicalVolume(AirTubeSolid,
																				G4Material::GetMaterial("G4_AIR"),
																				"AirShielding");
	new G4PVPlacement(0,vec_zero, AirTubeLog, "AirShielding",logicWorld, false,0);

	G4VisAttributes visAirTube(G4Colour(0,0,1));
	visAirTube.SetForceWireframe(true);
	visAirTube.SetForceAuxEdgeVisible(true);
	AirTubeLog->SetVisAttributes(visAirTube);
}

// void DetectorConstruction::AddLayer(const std::string& material,
// 																	  const double thickness)
// {
//
// 	//Shieldings::AddLayer(material, thickness):
// 	//Add full Giove, Poly, and Lead
// 	//Move ADR out
// 	 //
// 	//  Shieldings shieldingBuilder;
// 	//  shieldingBuilder.AddLayer(material, thickness):
// }

void DetectorConstruction::ConstructADR() noexcept {
	// copper plate
	constexpr double outerRadius5 = 12.*cm;
	constexpr double hz5 = 1.*cm;
	constexpr int zeroradius = 0.*cm;
	constexpr int startAngle = 0.*deg;
	constexpr int startAngle90 = 90.*deg;
	constexpr int spanningAngleFull = 360.*deg;
	const G4ThreeVector vec_zero(0*mm, 0*mm, 0*mm);
	auto plateTube = new G4Tubs("Plate", zeroradius, outerRadius5, hz5/2.,
															startAngle90, spanningAngleFull);
	auto plateLog = new G4LogicalVolume(plateTube, G4Material::GetMaterial("G4_Cu"),
																			"Copper Plate");
	new G4PVPlacement(0, vec_zero, plateLog, "Copper Plate", logicWorld, false, 0);
	G4VisAttributes visplate(G4Colour(1.,0.,0.));
	visplate.SetForceWireframe(true);
	visplate.SetForceSolid(true);
	plateLog->SetVisAttributes(visplate);

	// inner Aluminum tube
	constexpr double outerRadius1 = 13.97*cm;
	constexpr double hz1 = 28.4*cm;
	constexpr double outerRadius11 = 12.94*cm;
	constexpr double hz11 = 27.4*cm;
	auto shell1Tube = new G4Tubs("Shell1", zeroradius, outerRadius1, hz1/2.0,
															 startAngle, spanningAngleFull);
	auto shell11Tube = new G4Tubs("Shell11", zeroradius, outerRadius11, hz11/2.0,
																startAngle, spanningAngleFull);
	auto shellinnerAlTube = new G4SubtractionSolid("Inner Shell", shell1Tube,
																								 shell11Tube);
	auto shellinnerAlTubeLog = new G4LogicalVolume(shellinnerAlTube,
																								 G4Material::GetMaterial("G4_Al"),
																								 "Inner Shell");
	new G4PVPlacement(0, vec_zero, shellinnerAlTubeLog, "Inner shell", logicWorld,
										false,0);
	G4VisAttributes visinnerAlTube(G4Colour(0.41,0.41,0.41));
	visinnerAlTube.SetForceWireframe(true);
	visinnerAlTube.SetForceAuxEdgeVisible(true);
	shellinnerAlTubeLog->SetVisAttributes(visinnerAlTube);

	// outer Aluminum tube/can
	constexpr double outerRadius2 = 16.94*cm;
	constexpr double hz2 = 29.4*cm;
	constexpr double outerRadius21 = 15.94*cm;
	constexpr double hz21 = 29.3*cm;
	auto shell2Tube = new G4Tubs("Shell2", zeroradius, outerRadius2, hz2/2.0,
															 startAngle, spanningAngleFull);
	auto shell21Tube = new G4Tubs("Shell21", zeroradius, outerRadius21, hz21/2.0,
																startAngle, spanningAngleFull);
	auto shellouterAlTube = new G4SubtractionSolid("Inner Shell", shell2Tube,
																								 shell21Tube);
	auto shellouterAlTubeLog = new G4LogicalVolume(shellouterAlTube,
																								 G4Material::GetMaterial("G4_Al"),
																								 "Outer Shell");
	new G4PVPlacement(0, vec_zero, shellouterAlTubeLog, "Outer shell", logicWorld,
	 									false, 0);
	G4VisAttributes visouterAlTube(G4Colour(0.41,0.41,0.41));
	visouterAlTube.SetForceWireframe(true);
	visouterAlTube.SetForceAuxEdgeVisible(true);
	shellouterAlTubeLog->SetVisAttributes(visouterAlTube);

	// Tungsten tube/can/sheild
	constexpr double outerRadius3 = 12.1*cm;
	constexpr double hz3 = 20.0*cm;
	constexpr double outerRadius31 = 12.0*cm;
	constexpr double hz31 = 19.9*cm;
	auto tungTube = new G4Tubs("Tungshell", zeroradius, outerRadius3, hz3/2.0,
														 startAngle, spanningAngleFull);
	auto tung1Tube = new G4Tubs("Tungshell", zeroradius, outerRadius31, hz31/2.0,
															startAngle, spanningAngleFull);
	auto fulltungTube = new G4SubtractionSolid("Tungsten Shell",tungTube,tung1Tube);
	auto fulltungTubeLog = new G4LogicalVolume(fulltungTube,
																						 G4Material::GetMaterial("G4_W"),
																						 "Tungsten Shell");
	new G4PVPlacement(0, vec_zero, fulltungTubeLog, "Tungsten shell", logicWorld,
										false, 0);
	G4VisAttributes vistungTube(G4Colour(0.18,0.31,0.31));
	vistungTube.SetForceWireframe(true);
	vistungTube.SetForceAuxEdgeVisible(true);
	fulltungTubeLog->SetVisAttributes(vistungTube);

	// Scintillator Tube
	constexpr double outerRadius4 = 17.5*cm;
	constexpr double hz4 = 31.4*cm;
	constexpr double outerRadius41 = 16.5*cm;
	constexpr double hz41 = 30.4*cm;
	auto scinTube = new G4Tubs("Scinshell", zeroradius, outerRadius4, hz4/2.0,
														 startAngle, spanningAngleFull);
	auto scin1Tube = new G4Tubs("Scinshell", zeroradius, outerRadius41, hz41/2.0,
															startAngle, spanningAngleFull);
	auto  fullscinTube = new G4SubtractionSolid("Scintillator Shell", scinTube,
																							scin1Tube);
	auto fullscinTubeLog = new G4LogicalVolume(fullscinTube,
		 																				 G4Material::GetMaterial("Scintillator"),
																						 "Scintillator Shell");
	new G4PVPlacement(0, vec_zero, fullscinTubeLog, "Scintillator shell",
										logicWorld, false, 0);
	G4VisAttributes visscinTube(G4Colour(0.0,0.0,1.0));
	visscinTube.SetForceWireframe(true);
	visscinTube.SetForceAuxEdgeVisible(true);
	fullscinTubeLog->SetVisAttributes(visscinTube);
}

G4VPhysicalVolume*  DetectorConstruction::ConstructDetectors() noexcept {
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
		v_CrystalBoxes.push_back(new G4Box(CyrstalLabels[jCrys],DetectorSize/2.0,DetectorSize/2.0,DetectorSize/2.0));
		v_CrystalBoxesLog.push_back(new G4LogicalVolume(v_CrystalBoxes[jCrys], G4Material::GetMaterial("G4_Os"),CyrstalLabels[jCrys]));
		G4ThreeVector crys_pos(Cyrstalpos_x[jCrys],Cyrstalpos_y[jCrys],Cyrstalpos_z[jCrys]);
		v_CrystalBoxesPhys.push_back(new G4PVPlacement(0,crys_pos+vec_offset,v_CrystalBoxesLog[jCrys],CyrstalLabels[jCrys],logicWorld,false,0));
		v_CrystalBoxesLog[jCrys]->SetVisAttributes(visDetector);
		if (jCrys==0)
		{
			CrystalSensitiveDetector *Crystal1_SD = new CrystalSensitiveDetector("Crystal_1SD");
			G4SDManager::GetSDMpointer()->AddNewDetector(Crystal1_SD);
			v_CrystalBoxesLog[jCrys]->SetSensitiveDetector(Crystal1_SD);
		}

	}
	return physicalWorld;
}

G4VPhysicalVolume*  DetectorConstruction::ConstructSingleDetector() noexcept {
	// arbitrarily assuming 4-inch iZIP here [AJA]
	// Taking out 4-inch iZIP for now [AFL - March 2016]
	constexpr double DetectorSize = 5.2*cm;
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
	auto crystalBox_single = new G4Box("Single Crystal", DetectorSize/2.0, DetectorSize/2.0,DetectorSize/2.0);
	auto crystalLog_single = new G4LogicalVolume(crystalBox_single,G4Material::GetMaterial("Zn"), "Single Crystal");
	new G4PVPlacement(0, vec_zero, crystalLog_single, "Single Crystal",logicWorld, false,0);
	G4VisAttributes visDetector(G4Colour(0.0,1.0,0.0));
	visDetector.SetForceWireframe(true);
	visDetector.SetForceAuxEdgeVisible(true);
	crystalLog_single->SetVisAttributes(visDetector);
	return physicalWorld;
}

void DetectorConstruction::AddConcreteFloor() noexcept {
  if(!physicalWorld)
  {
    G4cout << "ERROR in DetectorConstruction::AddConcreteFloor" << G4endl;
    G4cout << "World volume does not exist!!" << G4endl;
    return;
  }
  constexpr double floorWidthX = 2210*mm;
  constexpr double floorWidthZ = 3810*mm;
  constexpr double floorThickness = 0.5*m;
  constexpr double heightADRaboveground = 100*cm;
  constexpr double heightADR = 34*cm;
  const G4ThreeVector vec_roomoffset = {-750*mm,-1525*mm,-(floorThickness/2.)-1.25*heightADR};
  const G4ThreeVector vec_heightaboveground(0.,0.,-heightADRaboveground);

  auto solidFloor = new G4Box("solidFloor", floorWidthX, floorWidthZ,floorThickness);
  auto logFloor = new G4LogicalVolume(solidFloor, G4Material::GetMaterial("G4_CONCRETE"), "logicalFloor");
  new G4PVPlacement(0, vec_roomoffset+vec_heightaboveground, logFloor, "PVFloor", logicWorld, false, 0);
  G4VisAttributes visFloor(G4Colour(1.,0.,0.));
  visFloor.SetForceWireframe(true);
  logFloor->SetVisAttributes(visFloor);
}

void DetectorConstruction::AddCoffinWalls() noexcept {
  constexpr double d_widthz = 5.839/2.0; // in meters
  constexpr double d_widthx = 8.277/2.0;
  constexpr double d_wallheight = 5.639/2.0;
  constexpr double d_wallthickness = .1;
  constexpr double floorThickness = 0.5*m;
  constexpr double heightADRaboveground = 100*cm;
  constexpr double heightADR = 34*cm;
  const G4ThreeVector vec_roomoffset = {-750*mm,-1525*mm,-(floorThickness/2.)-1.25*heightADR};
  const G4ThreeVector posWalls(0.,0.,0.);
  const G4ThreeVector posfloor(0.,0.,0.);
  const G4ThreeVector vec_heightaboveground(0.,0.,-heightADRaboveground);
  const G4ThreeVector voidTranslation(0.0, -floorThickness, 0.0);
  const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
  G4RotationMatrix* nullRotation = new G4RotationMatrix();

  auto solidWallsOuterBox = new G4Box("solidWallsOuterBox", d_widthx*m, d_wallheight*m, d_widthz*m);
  auto coffinfloor = new G4Box("coffinfloor", d_widthx*m, floorThickness, d_widthz*m);
  auto solidWallsInnerVoid = new G4Box("solidWallsInnerVoid", d_widthx*m-2.0*d_wallthickness*m,d_wallheight*m-2.0*d_wallthickness*m, d_widthz*m-2.0*d_wallthickness*m);
  auto solidWalls = new G4SubtractionSolid("solidWalls", solidWallsOuterBox, solidWallsInnerVoid, nullRotation, voidTranslation);
  auto logWalls = new G4LogicalVolume(solidWalls, G4Material::GetMaterial("G4_CONCRETE"), "logicalWalls");
  auto logFloor = new G4LogicalVolume(coffinfloor, G4Material::GetMaterial("G4_CONCRETE"), "coffinfloor");
  new G4PVPlacement(0, posWalls+vec_roomoffset, logWalls, "PVWalls", logicWorld, false, 0);
  new G4PVPlacement(0, posfloor+vec_roomoffset, logFloor, "PVfloor", logicWorld, false, 0);

  G4VisAttributes viscoffin(G4Colour(1.,0.,0.));
  viscoffin.SetForceWireframe(true);
  logWalls->SetVisAttributes(viscoffin);
  G4VisAttributes visfloor(G4Colour(1.,0.,0.));
  visfloor.SetForceWireframe(false);
  logFloor->SetVisAttributes(visfloor);
}

void DetectorConstruction::AddConcreteWalls() noexcept {
  if(!physicalWorld)
  {
    G4cout << "ERROR in DetectorConstruction::AddConcreteWalls" << G4endl;
    G4cout << "World volume does not exist!!" << G4endl;
    return;
  }

  // always build the floor by default when the walls are built
  //if(!physFloor) { AddConcreteFloor(); }

  // position vector of the walls
  constexpr double wallsHeight = 2.5*m;
  constexpr double floorWidthX = 2210*mm;
  constexpr double floorWidthZ = 3810*mm;
  constexpr double floorThickness = 0.5*m;
  constexpr double heightADRaboveground = 100*cm;
  constexpr double heightADR = 34*cm;
  double offset = wallsHeight/2.0-heightADRaboveground-0.5*heightADR;
  const G4ThreeVector vec_roomoffset = {-750*mm,-1525*mm,-(floorThickness/2.)-1.25*heightADR};
  G4ThreeVector posWalls(0.,0.0,offset);
  G4ThreeVector posCeiling(0.,0.0,floorThickness/2.0+1.38*wallsHeight-heightADRaboveground-0.5*heightADR);
  G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
  G4ThreeVector vec_wallA(-2561*mm,2057*mm,offset);
  G4ThreeVector vec_wallB(+209.7*mm,2057*mm,offset);
  G4ThreeVector vec_wallC(+551*mm,1372*mm,offset);
  G4ThreeVector vec_wallD(+1236*mm,1371*mm,offset);
  G4ThreeVector vec_wallE(+1236*mm,-2210*mm,offset);
  G4ThreeVector vec_testbox(-1649*mm,-1400*mm,offset-wallsHeight/2.0+38*cm);
  // transformation variables to help build the walls using a subtraction solid
  G4ThreeVector voidTranslation(0.0, 0.0,-floorThickness);

  auto inbetweenwall = new G4Box("Inbetweenwall",381*mm,229*mm,wallsHeight/2.0);
  auto inbetweenwall_logvolume = new G4LogicalVolume(inbetweenwall,G4Material::GetMaterial("G4_CONCRETE"),"inbetweenwall_logvolume");
  auto inbetweenwall_B = new G4Box("Inbetweenwall_B",1123*mm,229*mm,wallsHeight/2.0);
  auto inbetweenwall_logvolume_B = new G4LogicalVolume(inbetweenwall_B,G4Material::GetMaterial("G4_CONCRETE"),"inbetweenwall_logvolume_B");
  auto inbetweenwall_C = new G4Box("Inbetweenwall_C",457*mm,457*mm,wallsHeight/2.0);
  auto inbetweenwall_logvolume_C = new G4LogicalVolume(inbetweenwall_C,G4Material::GetMaterial("G4_CONCRETE"),"inbetweenwall_logvolume_C");
  auto inbetweenwall_D = new G4Box("Inbetweenwall_D",229*mm,457*mm,wallsHeight/2.0);
  auto inbetweenwall_logvolume_D = new G4LogicalVolume(inbetweenwall_D,G4Material::GetMaterial("G4_CONCRETE"),"inbetweenwall_logvolume_D");
  auto inbetweenwall_E = new G4Box("Inbetweenwall_E",267*mm,3125*mm,wallsHeight/2.0);
  auto inbetweenwall_logvolume_E = new G4LogicalVolume(inbetweenwall_E,G4Material::GetMaterial("Chrysotile"),"inbetweenwall_logvolume_E");
  auto solidceiling = new G4Box("solidFloor", floorWidthX, floorWidthZ,floorThickness);
  auto logceiling = new G4LogicalVolume(solidceiling, G4Material::GetMaterial("G4_CONCRETE"), "logicalCeiling");

  new G4PVPlacement(0, posCeiling+vec_roomoffset, logceiling,"PVCeiling", logicWorld, false, 0);
  new G4PVPlacement(0,vec_wallA,inbetweenwall_logvolume,"PVinbetweenWall",logicWorld,false,0);
  new G4PVPlacement(0,vec_wallB,inbetweenwall_logvolume_B,"PVinbetweenWall",logicWorld,false,0);
  new G4PVPlacement(0,vec_wallC,inbetweenwall_logvolume_C,"PVinbetweenWall",logicWorld,false,0);
  new G4PVPlacement(0,vec_wallD,inbetweenwall_logvolume_D,"PVinbetweenWall",logicWorld,false,0);
  new G4PVPlacement(0,vec_wallE,inbetweenwall_logvolume_E,"PVinbetweenWall",logicWorld,false,0);

  G4VisAttributes visbetweenwall(G4Colour(0.,1.,0.));
  visbetweenwall.SetForceWireframe(true);
  inbetweenwall_logvolume->SetVisAttributes(visbetweenwall);
  G4VisAttributes visbetweenwall_B(G4Colour(0.,1.,0.));
  visbetweenwall_B.SetForceWireframe(true);
  inbetweenwall_logvolume_B->SetVisAttributes(visbetweenwall_B);
  G4VisAttributes visbetweenwall_C(G4Colour(0.,1.,0.));
  visbetweenwall_C.SetForceWireframe(true);
  inbetweenwall_logvolume_C->SetVisAttributes(visbetweenwall_C);
  G4VisAttributes visbetweenwall_D(G4Colour(0.,1.,0.));
  visbetweenwall_D.SetForceWireframe(true);
  inbetweenwall_logvolume_D->SetVisAttributes(visbetweenwall_D);
  G4VisAttributes visbetweenwall_E(G4Colour(0.,0.,1.));
  visbetweenwall_E.SetForceWireframe(true);
  inbetweenwall_logvolume_E->SetVisAttributes(visbetweenwall_E);
  G4VisAttributes visceiling(G4Colour(1.,0.,0.));
  visceiling.SetForceWireframe(true);
  logceiling->SetVisAttributes(visceiling);
}

G4VPhysicalVolume*  DetectorConstruction::ConstructNuDetector() noexcept {
	constexpr double startAngle = 0.*deg;
	constexpr double spanningAngleFull = 360.*deg;
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);

	auto TargetDetectorBox = new G4Tubs("NuDetector",0*m,1150*mm,1221*mm,startAngle, spanningAngleFull);
	auto TargetDetectorLog = new G4LogicalVolume(TargetDetectorBox, G4Material::GetMaterial("NuTargetLiquid"),"NuDetector");
	new G4PVPlacement(0,vec_zero,TargetDetectorLog,"NuDetector",logicWorld,false,0);

  G4VisAttributes visDetector(G4Colour(0.0,1.0,0.0));
	TargetDetectorLog->SetVisAttributes(visDetector);
  visDetector.SetForceWireframe(true);
  visDetector.SetForceAuxEdgeVisible(true);

	auto NuDetector = new CrystalSensitiveDetector("NuDetector");
	G4SDManager::GetSDMpointer()->AddNewDetector(NuDetector);
	TargetDetectorLog->SetSensitiveDetector(NuDetector);

  return physicalWorld;
}

void DetectorConstruction::SetCrystalMaterial(G4String Material) noexcept {
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
