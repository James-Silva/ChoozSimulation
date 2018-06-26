#include "Materials.hh"

#include <globals.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4Material.hh>
#include <G4Element.hh>
#include <G4Isotope.hh>
#include <G4MaterialPropertiesTable.hh>
#include "G4NistManager.hh"

#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

Materials* Materials::materials = nullptr;

////////////////////////////////////////////////////////////////////////////////

Materials::Materials()
{
    G4MaterialPropertiesTable* MPT = nullptr;
    G4String symbol;
    G4double M, density, fractionmass, mol; //M = mass of a mole;
    G4int Z, A, nelements, natoms;

    // use G4-NIST materials data base
    G4NistManager* man = G4NistManager::Instance();
    G4Material* materialGalactic = man->FindOrBuildMaterial("G4_Galactic");
    G4Material* materialGe = man->FindOrBuildMaterial("G4_Ge");
    G4Material* materialSi = man->FindOrBuildMaterial("G4_Si");
    G4Material* materialAir = man->FindOrBuildMaterial("G4_AIR");
    G4Material* materialPoly = man->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material* materialConcrete = man->FindOrBuildMaterial("G4_CONCRETE");
    G4Material* materialWater = man->FindOrBuildMaterial("G4_WATER");
    G4Material* materialLead = man->FindOrBuildMaterial("G4_Pb");
    G4Material* materialOs = man->FindOrBuildMaterial("G4_Os");
    G4Material* materialAl = man->FindOrBuildMaterial("G4_Al");
    G4Material* materialW = man->FindOrBuildMaterial("G4_W");
    G4Material* materialCu = man->FindOrBuildMaterial("G4_Cu");
    G4Material* materialPb = man->FindOrBuildMaterial("G4_Pb");
    G4Material* materialZn = man->FindOrBuildMaterial("G4_Zn");
    G4Material* materialZr = man->FindOrBuildMaterial("G4_Zr");
    G4Material* materialGd = man->FindOrBuildMaterial("G4_Gd");
    G4Element* elH = man->FindOrBuildElement("H");
    G4Element* elO = man->FindOrBuildElement("O");
    G4Element* elSi = man->FindOrBuildElement("Si");
    G4Element* elMg = man->FindOrBuildElement("Mg");
    G4Element* elAl = man->FindOrBuildElement("Al");
    G4Element* elFe = man->FindOrBuildElement("Fe");
    G4Element* elK = man->FindOrBuildElement("K");
    G4Element* elB = man->FindOrBuildElement("B");
    G4Element* elC = man->FindOrBuildElement("C");

    /////////////////////////////////////////////////////////////
    //////////////// Polyethylene materials /////////////////////
    /////////////////////////////////////////////////////////////

    //Edit: Boron fraction was calculated by keeping constant H/C mass fraction ratio
    //Excluded any oxygen elements in borated polyethylene plastics [Matthieu - Jan 2018]

    G4Material* Pol = new G4Material("Pol",density= 0.96*g/cm3, nelements= 2);
    Pol->AddElement(elH, fractionmass= 14.86*perCent);
    Pol->AddElement(elC, fractionmass= 85.14*perCent);

    // Polyethylene with boron at 1.75% - Has borated polyethylene any oxygen elements? FIXME
    G4Material* PolBor1_75pc = new G4Material("PolBor1_75pc",density= 0.96*g/cm3, nelements= 3);
    PolBor1_75pc->AddElement(elH, fractionmass= 14.6*perCent);
    PolBor1_75pc->AddElement(elC, fractionmass= 83.65*perCent);
    PolBor1_75pc->AddElement(elB, fractionmass= 1.75 *perCent);

    // Polyethylene with boron at 3% - Has borated polyethylene any oxygen elements? FIXME
    G4Material* PolBor3pc = new G4Material("PolBor3pc",density= 0.96*g/cm3, nelements= 3);
    PolBor3pc->AddElement(elH, fractionmass= 14.424*perCent);
    PolBor3pc->AddElement(elC, fractionmass= 82.586*perCent);
    PolBor3pc->AddElement(elB, fractionmass= 3.00*perCent);

    // Polyethylene with boron at 3.5% - Has borated polyethylene any oxygen elements? FIXME
    G4Material* PolBor3_5pc = new G4Material("PolBor3_5pc",density= 0.96*g/cm3, nelements= 3);
    PolBor3_5pc->AddElement(elH, fractionmass= 14.340*perCent);
    PolBor3_5pc->AddElement(elC, fractionmass= 82.160*perCent);
    PolBor3_5pc->AddElement(elB, fractionmass= 3.50 *perCent);

    // Polyethylene with boron at 10% - Has borated polyethylene any oxygen elements? FIXME
    G4Material* PolBor10pc = new G4Material("PolBor10pc",density= 0.96*g/cm3, nelements= 3);
    PolBor10pc->AddElement(elH, fractionmass= 13.374*perCent);
    PolBor10pc->AddElement(elC, fractionmass= 76.626*perCent);
    PolBor10pc->AddElement(elB, fractionmass= 10.00 *perCent);

    /////////////////////////////////////////////////////////////

  	G4Material* materialScintillator = new G4Material("Scintillator",1.032*g/cm3,2);
  	materialScintillator->AddElement(elC,9);
  	materialScintillator->AddElement(elH,10);

  	G4Material* Chrysotile = new G4Material("Chrysotile",2.53*g/cm3,4);
  	Chrysotile->AddElement(elMg,3);
  	Chrysotile->AddElement(elSi,2);
  	Chrysotile->AddElement(elO,9);
  	Chrysotile->AddElement(elH,4);

  	G4Material* materialPoly_Borated = new G4Material("BoratedPoly",1.07*g/cm3,2);
  	materialPoly_Borated->AddMaterial(materialPoly,95.0*perCent);
  	materialPoly_Borated->AddElement(elB,5.0*perCent);

  	G4Material* materialQuartz = new G4Material("Quartz",2.81*g/cm3,2);
  	materialQuartz->AddElement(elSi,1);
  	materialQuartz->AddElement(elO,2);

  	G4Material* materialCorundum = new G4Material("Corundum",2.81*g/cm3,2);
  	materialCorundum->AddElement(elAl,2);
  	materialCorundum->AddElement(elO,3);

  	G4Material* materialIronOxide = new G4Material("IronOxide",2.81*g/cm3,2);
  	materialIronOxide->AddElement(elFe,1);
  	materialIronOxide->AddElement(elO,1);

  	G4Material* materialMgOxide = new G4Material("MgOxide",2.81*g/cm3,2);
  	materialMgOxide->AddElement(elMg,1);
  	materialMgOxide->AddElement(elO,1);

  	G4Material* materialKOxide = new G4Material("KOxide",2.81*g/cm3,2);
  	materialKOxide->AddElement(elK,2);
  	materialKOxide->AddElement(elO,1);

  	G4Material* materialChoozRock = new G4Material("ChoozRock",2.81*g/cm3,5);
  	materialChoozRock->AddMaterial(materialQuartz,58.0*perCent);
  	materialChoozRock->AddMaterial(materialCorundum,19.0*perCent);
  	materialChoozRock->AddMaterial(materialIronOxide,17.0*perCent);
  	materialChoozRock->AddMaterial(materialMgOxide,4.0*perCent);
  	materialChoozRock->AddMaterial(materialKOxide,2.0*perCent);

  	G4Material* mineraloil = new G4Material("MineralOil",0.86*g/cm3,2);
  	mineraloil->AddElement(elC,25);
  	mineraloil->AddElement(elH,43);

  	G4Material* materialSteel = new G4Material("Steel",7.9*g/cm3,2);
  	materialSteel->AddElement(elFe,98.0*perCent);
  	materialSteel->AddElement(elC,2.0*perCent);

  	G4Material* materialDodecane = new G4Material("Dodecane",0.75*g/cm3,2);
  	materialDodecane->AddElement(elC,12);
  	materialDodecane->AddElement(elH,24);

  	G4Material* materialPXE = new G4Material("PXE",0.988*g/cm3,2);
  	materialPXE->AddElement(elC,16);
  	materialPXE->AddElement(elH,18);

  	G4Material* materialPPO = new G4Material("PPO",1.06*g/cm3,3);
  	materialPPO->AddElement(elC,3);
  	materialPPO->AddElement(elH,6);
  	materialPPO->AddElement(elO,1);

  	G4Material* materialNuTarget = new G4Material("NuTargetLiquid",804.59*kg/m3,4);
  	materialNuTarget->AddMaterial(materialDodecane,74.5*perCent);
  	materialNuTarget->AddMaterial(materialPXE,24.6*perCent);
  	materialNuTarget->AddMaterial(materialPPO,0.8*perCent);
  	materialNuTarget->AddMaterial(materialGd,0.1*perCent);

  	G4Material* materialGammaCatcher = new G4Material("GammaCatcherLiquid",800.6*kg/m3,3);
  	materialGammaCatcher->AddMaterial(materialDodecane,74.94*perCent);
  	materialGammaCatcher->AddMaterial(materialPXE,24.68*perCent);
  	materialGammaCatcher->AddMaterial(materialPPO,0.38*perCent);
}
