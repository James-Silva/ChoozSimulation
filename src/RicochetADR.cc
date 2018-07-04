//Created 7/1/18 by James A. Silva

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

namespace detectorcomponents {

void ConstructADR(G4LogicalVolume* mother) {
	// copper plate
	constexpr double outerRadius5 = 12.*cm;
  constexpr double hz5 = 1.*cm;
	constexpr double zeroradius = 0.*cm;
	constexpr double startAngle = 0.*deg;
	constexpr double startAngle90 = 90.*deg;
	constexpr double spanningAngleFull = 360.*deg;
	const G4ThreeVector vec_zero(0*mm, 0*mm, 0*mm);
	auto plateTube = new G4Tubs("Plate", zeroradius, outerRadius5, hz5/2.,
															startAngle90, spanningAngleFull);
	auto plateLog = new G4LogicalVolume(plateTube, G4Material::GetMaterial("G4_Cu"),
																			"Copper Plate");
	new G4PVPlacement(0, vec_zero, plateLog, "Copper Plate", mother, false, 0, true);
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
	new G4PVPlacement(0, vec_zero, shellinnerAlTubeLog, "Inner shell", mother,
										false,0,true);
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
	new G4PVPlacement(0, vec_zero, shellouterAlTubeLog, "Outer shell", mother,
	 									false, 0, true);
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
	new G4PVPlacement(0, vec_zero, fulltungTubeLog, "Tungsten shell", mother,
										false, 0, true);
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
										mother, false, 0, true);
	G4VisAttributes visscinTube(G4Colour(0.0,0.0,1.0));
	visscinTube.SetForceWireframe(true);
	visscinTube.SetForceAuxEdgeVisible(true);
	fullscinTubeLog->SetVisAttributes(visscinTube);
}

}
