//Created 6/27/18 by James A. Silva

#include "Shieldings.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include <string>

namespace detectorcomponents {

LayerConstructor::LayerConstructor(): layerNum(1) {}

// Create two solid G4Boxes that will be subtracted from each other.
// The member variable boxLength is set in the middle of creating the solid.
void LayerConstructor::AddLayer(const std::string& material,
                                G4LogicalVolume* mother) {
  if (!boxLength || !cavityLength || !layerThickness) {
    std::cerr << "\n\nEither the cavityLength, layerThickness, or boxLength "
              << "was not correctly set using the messenger\n\n"<<std::endl;
    std::exit(-1);
  }
  std::string layerName = "Shielding Layer "+ std::to_string(layerNum);

  auto innerLayer = new G4Box("InLayer", boxLength/2, boxLength/2, boxLength/2);
  boxLength = boxLength + 2*layerThickness;  //The member boxLength is increased
  auto outerLayer = new G4Box("OutLayer", boxLength/2, boxLength/2, boxLength/2);
  auto solid = new G4SubtractionSolid(layerName, outerLayer, innerLayer);

  auto logic = new G4LogicalVolume(solid, G4Material::GetMaterial(material),
                                  layerName);
  new G4PVPlacement(0, {0,0,0}, logic, layerName, mother, false, 0, true);

  this->setLayerColour(logic, material);
  ++layerNum;
}

void LayerConstructor::AddULayer(const std::string& material,
                                G4LogicalVolume* mother) {
  if (!boxLength || !cavityLength || !layerThickness) {
    std::cerr << "\n\nEither the cavityLength, layerThickness, or boxLength "
              << "was not correctly set using the messenger\n\n"<<std::endl;
    std::exit(-1);
  }
  std::string layerName = "Shieling Layer "+ std::to_string(layerNum);

  auto cutOut = new G4Box("topBox", cavityLength/2, cavityLength/2, boxLength/2);
  auto innerLayer = new G4Box("InLayer", boxLength/2, boxLength/2, boxLength/2);
  boxLength = boxLength + 2*layerThickness;  //The member boxLength is increased
  auto outerLayer = new G4Box("OutLayer", boxLength/2, boxLength/2, boxLength/2);
  auto solid = new G4SubtractionSolid(layerName, outerLayer, innerLayer);
  auto uShapeSolid = new G4SubtractionSolid(layerName, solid, cutOut, 0,
                                            {0,0,layerThickness});
  auto logic = new G4LogicalVolume(uShapeSolid, G4Material::GetMaterial(material),
                                   layerName);
  new G4PVPlacement(0, {0,0,0}, logic, layerName, mother, false, 0, true);

  this->setLayerColour(logic, material);
  ++layerNum;
}

void LayerConstructor::setLayerColour(G4LogicalVolume* logic, const std::string& material) {
  if (material == "G4_Pb") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().Red()));
  } else if (material == "Pol") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().Green()));
  } else if (material == "PolBor3pc") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().White()));
  } else if (material == "PolBor10pc") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().Brown()));
  } else if (material == "G4_WATER") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().Blue()));
  } else if (material == "G4_AIR") {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour(1.,1.,0.,0.6))); //Yellow
  }  else {
    logic->SetVisAttributes(new G4VisAttributes(G4Colour().Magenta()));
  }
}

void ConstructGioveShielding(G4LogicalVolume* mother) noexcept {
  constexpr double thickness = 50*mm;
	const double MuonVetoWidth = 1000*mm;
	const double MuonVetoHeight = 950*mm;

	std::cout << "   ***** Chose GIOVE-like shielding configuration *****\n";

	//////////////////////////////////////////////////////////////////////////////

	//Layer 1 - Polyethylene
	auto muonShielding = new G4Box("muonShielding",
      													 MuonVetoWidth/2.,
      													 MuonVetoWidth/2.,
      												   MuonVetoHeight/2.);
	auto muonShielding_lv = new G4LogicalVolume(muonShielding,
    																				  G4Material::GetMaterial("Pol"),
    																				  "muonShielding");
	new G4PVPlacement(0, {0, 0, 0}, muonShielding_lv, "muonShielding", mother,
  									false, 0, true);
	G4VisAttributes visMuShielding(G4Colour(1.,0.,0.));
	visMuShielding.SetForceWireframe(true);
	muonShielding_lv->SetVisAttributes(visMuShielding);

	//////////////////////////////////////////////////////////////////////////////

	//Layer 2 - Lead
	auto pbShielding1 = new G4Box("Pb_shielding1",
      													 MuonVetoWidth/2. - 1*thickness,
      													 MuonVetoWidth/2. - 1*thickness,
      													 MuonVetoWidth/2. - 1*thickness);
  auto pbShielding1_lv = new G4LogicalVolume(pbShielding1,
      																				G4Material::GetMaterial("G4_Pb"),
      																				"Pb_shielding1");
	new G4PVPlacement(0, {0,0,0}, pbShielding1_lv, "Pb_shielding1", muonShielding_lv,
  									false, 0, true);
	G4VisAttributes visPbShielding1(G4Colour(1.,0.,0.));
	visPbShielding1.SetForceWireframe(true);
	pbShielding1_lv->SetVisAttributes(visPbShielding1);

	//////////////////////////////////////////////////////////////////////////////

	//Layer 3 - 10% Borated Polyethylene
	auto pe_b_Shielding1 = new G4Box("PE_B_shielding1",
															 MuonVetoWidth/2. - 2*thickness,
															 MuonVetoWidth/2. - 2*thickness,
															 MuonVetoWidth/2. - 2*thickness);

  auto pe_b_Shielding1_lv = new G4LogicalVolume(pe_b_Shielding1,
																					 G4Material::GetMaterial("PolBor10pc"),
																					 "PE_B_shielding1");
	new G4PVPlacement(0, {0,0,0}, pe_b_Shielding1_lv, "PE_B_shielding1", pbShielding1_lv,
										false, 0, true);
	G4VisAttributes visPE_B_Shielding1(G4Colour(1.,0.,0.));
	visPE_B_Shielding1.SetForceWireframe(true);
	pe_b_Shielding1_lv->SetVisAttributes(visPE_B_Shielding1);

	//////////////////////////////////////////////////////////////////////////////

	//Layer 4 - Lead
	auto pbShielding2 = new G4Box("Pb_shielding2",
      													MuonVetoWidth/2. - 3*thickness,
      													MuonVetoWidth/2. - 3*thickness,
      													MuonVetoWidth/2. - 3*thickness);
  auto pbShielding2_lv = new G4LogicalVolume(pbShielding2,
      																			 G4Material::GetMaterial("G4_Pb"),
      																			 "Pb_shielding2");
	new G4PVPlacement(0, {0,0,0}, pbShielding2_lv, "Pb_shielding2", pe_b_Shielding1_lv,
										false, 0, true);
	G4VisAttributes visPbShielding2(G4Colour(1.,0.,0.));
	visPbShielding2.SetForceWireframe(true);
	pbShielding2_lv->SetVisAttributes(visPbShielding2);

	//////////////////////////////////////////////////////////////////////////////

	//Layer 5 - 3% Borated Polyethylene
	auto pe_b_Shielding2 = new G4Box("PE_B_shielding2",
															     MuonVetoWidth/2. - 4*thickness,
															     MuonVetoWidth/2. - 4*thickness,
															     MuonVetoWidth/2. - 4*thickness);

  auto pe_b_Shielding2_lv = new G4LogicalVolume(pe_b_Shielding2,
																					      G4Material::GetMaterial("PolBor3pc"),
																					      "PE_B_shielding2");
	new G4PVPlacement(0, {0,0,0}, pe_b_Shielding2_lv, "PE_B_shielding2",
										pbShielding2_lv, false, 0, true);
	G4VisAttributes visPE_B_Shielding2(G4Colour(1.,0.,0.));
	visPE_B_Shielding2.SetForceWireframe(true);
	pe_b_Shielding2_lv->SetVisAttributes(visPE_B_Shielding2);

	//////////////////////////////////////////////////////////////////////////////

	//Layer 6 - Lead
	auto pbShielding3 = new G4Box("Pb_shielding3",
													 MuonVetoWidth/2. - 6*thickness,
													 MuonVetoWidth/2. - 6*thickness,
													 MuonVetoWidth/2. - 6*thickness);
	auto pbShielding3_lv = new G4LogicalVolume(pbShielding3,
																				G4Material::GetMaterial("G4_Pb"),
																				"Pb_shielding3");
	new G4PVPlacement(0, {0,0,0}, pbShielding3_lv, "Pb_shielding3",
  									pe_b_Shielding2_lv, false, 0, true);
	G4VisAttributes visPbShielding3(G4Colour(1.,0.,0.));
	visPbShielding3.SetForceWireframe(true);
	pbShielding3_lv->SetVisAttributes(visPbShielding3);

	//////////////////////////////////////////////////////////////////////////////

	//Shield interior - Air
	auto shieldInterior = new G4Box("shieldInterior",
													 MuonVetoWidth/2. - 7*thickness,
													 MuonVetoWidth/2. - 7*thickness,
													 MuonVetoWidth/2. - 7*thickness);
	auto shieldInterior_lv = new G4LogicalVolume(shieldInterior,
																				G4Material::GetMaterial("G4_AIR"),
																				"shieldInterior");
	new G4PVPlacement(0, {0,0,0}, shieldInterior_lv, "shieldInterior",
  									pbShielding3_lv, false, 0, true);
	G4VisAttributes visshieldInterior(G4Colour(1.,0.,0.));
	visshieldInterior.SetForceWireframe(true);
	shieldInterior_lv->SetVisAttributes(visshieldInterior);

}

void ConstructPolySheilding(const double innerR, const double outerR,
														const double topthickness, G4LogicalVolume* mother)
														noexcept {
	// detector shielding is arbitrarily assuemd to be two concentric cylinders of
	// polyethylene and lead; poly is 10cm thick and lead is 4cm thick (again arbitrarily)
	// Taking out this version of shielding while I work on the ATR geometry [AFL]
	// Lead Tube
	constexpr int zeroradius = 0.*cm;
	constexpr int startAngle = 0.*deg;
	constexpr int spanningAngleFull = 360.*deg;
	const int outerRadius_shield = 1.0*outerR*cm;
	const int outerRadius_shield1 = 1.0*innerR*cm;
  constexpr double heightADR = 34*cm;
	const auto heightADR1 = heightADR-2*(topthickness*cm);
  const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);

  auto shieldTube = new G4Tubs("Polyshield", zeroradius, outerRadius_shield, heightADR/2.0, startAngle, spanningAngleFull);
	auto shield1Tube = new G4Tubs("Polyshield", zeroradius, outerRadius_shield1, heightADR1/2.0,startAngle, spanningAngleFull);
	auto fullshieldTube = new G4SubtractionSolid("Borated_Poly_Shield",shieldTube,shield1Tube);
	auto fullshieldTubeLog = new G4LogicalVolume(fullshieldTube, G4Material::GetMaterial("PolBor3pc"), "Borated_Poly_Shield");
	new G4PVPlacement(0,vec_zero, fullshieldTubeLog, "Borated_Poly_Shield",mother, false, 0, true);

  G4VisAttributes visshieldTube(G4Colour(1.0,0.586,0.0));
	visshieldTube.SetForceWireframe(true);
	visshieldTube.SetForceAuxEdgeVisible(true);
	fullshieldTubeLog->SetVisAttributes(visshieldTube);
}

void ConstructPbSheilding(const double innerR, const double outerR,
													const double topthickness, G4LogicalVolume* mother)
													noexcept {
	// detector shielding is arbitrarily assuemd to be two concentric cylinders of
	// polyethylene and lead; poly is 10cm thick and lead is 4cm thick (again arbitrarily)
	// Taking out this version of shielding while I work on the ATR geometry [AFL]
	// Lead Tube
	constexpr int zeroradius = 0.*cm;
	constexpr int startAngle = 0.*deg;
	constexpr int spanningAngleFull = 360.*deg;
	const G4ThreeVector vec_zero(0*mm,0*mm,0*mm);
	const int outerRadius_shield = 1.0*outerR*cm;
	const int outerRadius_shield1 = 1.0*innerR*cm;
  constexpr double heightADR = 34*cm;
	const auto heightADR1 = heightADR-2*(topthickness*cm);
	auto shieldTube = new G4Tubs("Polyshield", zeroradius, outerRadius_shield, heightADR/2.0, startAngle, spanningAngleFull);
	auto shield1Tube = new G4Tubs("Polyshield", zeroradius, outerRadius_shield1, heightADR1/2.0,startAngle, spanningAngleFull);
	auto fullshieldTube = new G4SubtractionSolid("Shielding Shell (Pb)",shieldTube,shield1Tube);
	auto fullshieldTubeLog = new G4LogicalVolume(fullshieldTube, G4Material::GetMaterial("Lead"), "Shielding Shell (Pb)");
	new G4PVPlacement(0,vec_zero, fullshieldTubeLog, "Shielding shell (Pb)",mother, false, 0, true);

  G4VisAttributes visshieldTube(G4Colour(1.0,0.586,0.0));
	visshieldTube.SetForceWireframe(true);
	visshieldTube.SetForceAuxEdgeVisible(true);
	fullshieldTubeLog->SetVisAttributes(visshieldTube);
}

} // namespace detectorcomponents
