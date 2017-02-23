// PrimaryGeneratorAction.cc

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "TMath.h"
#include "G4UnitsTable.hh"
#include "GPSPrimaryGeneratorAction.hh"
#include <stdlib.h>

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC, HistoManager* histo)
: G4VUserPrimaryGeneratorAction(),
  fMessenger(0),
  fParticleGun(0),
  fDetector(DC),
  fHistoManager(histo)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fMessenger = new PrimaryGeneratorMessenger(this);

  // default particle kinematic
  //setNewSource(sourceType);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fMessenger;
  delete fParticleGun;
}
void PrimaryGeneratorAction::SetGenerator(G4String generatorType)
{
  if(generatorType == "GPS")
  {  
    sourceType = "GPS";
    particleSource = new GPSPrimaryGeneratorAction();
  }  
}  


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Set position differently depending on source type
  G4double randomenergy;
  if(sourceType == "gamma")
  {  
    setGammaPosition();
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }    
  else if(sourceType == "neutron")
  {  
    setNeutronMomentum();
    setNeutronPosition();
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }  
  else if(sourceType == "neutronpoint")
  { 
    setNeutronMomentum();
    setPointNeutronPosition();
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  else if(sourceType == "gammapoint")
  {  
    setPointGammaPosition();
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  else if(sourceType == "neutronspectrum" && logaxis == true)
  {  
    randomenergy = TMath::Power(10,h_Spectrum.GetRandom());
    //cout << "Random Energy (Log) selected: " << randomenergy << endl;
    buildNeutronSource(randomenergy);
    setNeutronMomentum();
    setNeutronPosition();  
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } 
  else if(sourceType == "neutronspectrum")
  {  
    randomenergy = h_Spectrum.GetRandom();
    //cout << "Random Energy selected: " << randomenergy << endl;
    buildNeutronSource(randomenergy); //Input energy in MeV
    setNeutronMomentum();
    setNeutronPosition();  
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } 
  else if(sourceType == "GPS")
  {  
    particleSource->GeneratePrimaries(anEvent);
  } 
  else
    std::cerr << "ERROR in PrimaryGeneratorAction::GeneratePrimaries: Invalid source type selected!" << std::endl;

  // Actually generate the event

}


void PrimaryGeneratorAction::buildGammaSource(G4double energy)
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(energy*MeV);
}

void PrimaryGeneratorAction::buildNeutronSource(G4double energy)
{
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
  fParticleGun->SetParticleDefinition(particle);
  //std::cout << "Input Energy (MeV): " << energy*MeV << std::endl;
  fParticleGun->SetParticleEnergy(energy*MeV); // The default unit for energy in Geant4 is MeV and it seems to convert all input energies into MeV
}

void PrimaryGeneratorAction::setGammaPosition()
{
  // generate particles on a cylinder around the detector, with momenta pointing
  // radially inward
  //G4double d_shieldingheight = 7000.0*mm;
  G4double d_surfacesides = 2. * TMath::Pi() * gammaheight * gammaradius;
  G4double d_surfacetop = TMath::Pi() * gammaradius * gammaradius;
  G4double d_totalsurface = 2.*d_surfacetop + d_surfacesides;
  G4double d_probtop = d_surfacetop/d_totalsurface;
  G4double d_probside = d_surfacesides/d_totalsurface;
  G4double d_picksurface = G4UniformRand();
  G4ThreeVector gammaSourcePos(0.,0.,0.);
  if (d_picksurface < d_probtop)
  {
    gammaSourcePos = GenerateTopEvent(gammaradius,(gammaheight/2.0)+sourceoffsetz);
  } 
  else if (d_picksurface <= d_probtop+d_probside && d_picksurface >= d_probtop)
  {
    gammaSourcePos = GenerateSideWallEvent(gammaradius,gammaheight,sourceoffsetz);
  }
  else 
  {
    gammaSourcePos = GenerateTopEvent(gammaradius,-(gammaheight/2.0)+sourceoffsetz);
  }  
  G4ThreeVector gammaMomentum = gammaSourcePos;
  //gammaSourcePos=test_vec;
  
  gammaMomentum.setMag(1.);
  gammaMomentum *= -1.;

  fParticleGun->SetParticleMomentumDirection(gammaMomentum);
  fParticleGun->SetParticlePosition(gammaSourcePos);
}

void PrimaryGeneratorAction::setPointNeutronPosition()
{
  G4ThreeVector neutronMomentum = neutronsourcepos;
  neutronMomentum.setMag(1.);
  neutronMomentum *= -1.;

  fParticleGun->SetParticleMomentumDirection(neutronMomentum);
  fParticleGun->SetParticlePosition(neutronsourcepos);  
}

void PrimaryGeneratorAction::setPointGammaPosition()
{
  G4ThreeVector gammaMomentum = gammasourcepos;
  gammaMomentum.setMag(1.);
  gammaMomentum *= -1.;

  fParticleGun->SetParticleMomentumDirection(gammaMomentum);
  fParticleGun->SetParticlePosition(gammasourcepos);  
}

void PrimaryGeneratorAction::setNeutronPosition()
{
  // generate particles on a cylinder around the detector, with momenta pointing
  // radially inward
  //G4double d_shieldingheight = 7000.0*mm;
  G4double innerradius = neutronradius-(0.5*sourcethickness);
  G4double outerradius = neutronradius+(0.5*sourcethickness);
  G4double volumesides = TMath::Pi() * neutronheight * (TMath::Power(outerradius,2)-TMath::Power(innerradius,2));
  G4double volumetop = TMath::Pi() * TMath::Power(innerradius,2) * sourcethickness;
  G4double totalvolume = volumetop + volumesides;
  G4double probside = volumesides/totalvolume;
  G4double picksurface = G4UniformRand();
  G4double randomaddition = 2*(G4UniformRand()-0.5)*(0.5*sourcethickness);
  G4double randomaddition_bottom = 2*(G4UniformRand()-0.5)*(0.5*sourcethickness);
  G4double bottomdisk =  -0.5*neutronheight+sourceoffsetz-(0.5*sourcethickness);
  G4ThreeVector neutronSourcePos{};
  //std::cout<<"offset: " << sourceoffsetz/mm << endl;
  randomaddition = 0;
  if (picksurface <= probside)
  {
    //std::cout << "Side picked" << std::endl;
    neutronSourcePos = GenerateSideWallEvent(neutronradius+randomaddition,neutronheight,sourceoffsetz-(0.5*sourcethickness));
    //std::cout << "Z position: " << neutronSourcePos.getZ() << std::endl;
  }
  else 
  {
    //std::cout << "Bottom picked" << std::endl;
    neutronSourcePos = GenerateTopEvent(innerradius,bottomdisk+randomaddition_bottom);
  }  

  //std::cout << "Neutron Source Pos: " << neutronSourcePos << std::endl; 
  
  fParticleGun->SetParticlePosition(neutronSourcePos);
  //std::cout << "Zposition of init: " << neutronSourcePos.getZ() << std::endl;
}

void PrimaryGeneratorAction::setNeutronMomentum()
{
  G4ThreeVector neutronMomentum = GenerateIsotropicVector();
  fParticleGun->SetParticleMomentumDirection(neutronMomentum);
}


G4ThreeVector PrimaryGeneratorAction::GenerateIsotropicVector()
{
  G4double phi = 2. * TMath::Pi() * G4UniformRand();
  G4double cosTheta = 2 * (G4UniformRand()-0.5);
  return G4ThreeVector(std::cos(phi)*(1-std::pow(cosTheta,2)), std::sin(phi)*(1-std::pow(cosTheta,2)), cosTheta);

}

G4ThreeVector PrimaryGeneratorAction::GenerateSideWallEvent(G4double radius,G4double height,G4double offset)
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double randz =  height*(G4UniformRand()-0.5)+offset;
  
  return G4ThreeVector (radius * TMath::Cos(randPhi), radius * TMath::Sin(randPhi), randz);
}

G4ThreeVector PrimaryGeneratorAction::GenerateTopEvent(G4double radius,G4double height)
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double sourceradius = std::sqrt(G4UniformRand() * std::pow(radius,2));
  
  return G4ThreeVector(sourceradius * TMath::Cos(randPhi), sourceradius * TMath::Sin(randPhi), height);
}

void PrimaryGeneratorAction::setSourceRadius(G4double radius)
{
    neutronradius = radius;
    gammaradius = radius;
}

void PrimaryGeneratorAction::setSourceHeight(G4double height)
{
    neutronheight = height;
    gammaheight = height;
}
void PrimaryGeneratorAction::setSourceHeightOffset(G4double offset)
{
  sourceoffsetz = offset;
}

void PrimaryGeneratorAction::setNewNeutronSpectrumSource()
{
  sourceType = "neutronspectrum";
}

void PrimaryGeneratorAction::setNewNeutronSpectrumSource_LogX()
{
  logaxis  = true;
}

void PrimaryGeneratorAction::setNewNeutronSource(G4double energy)
{
  sourceType = "neutron";
  buildNeutronSource(energy);
}

void PrimaryGeneratorAction::setNewGammaSource(G4double energy)
{
  sourceType = "gamma";
  buildGammaSource(energy);
}

void PrimaryGeneratorAction::SetNeutronPointSource(G4ThreeVector pos)
{
  sourceType = "neutronpoint";
  neutronsourcepos = pos;  
}

void PrimaryGeneratorAction::SetGammaPointSource(G4ThreeVector pos)
{
  sourceType = "gammapoint";
  gammasourcepos = pos;  
}

void PrimaryGeneratorAction::setSourceThickness(G4double thickness)
{
  sourcethickness = thickness;
}

void PrimaryGeneratorAction::SetSpectralData(G4String filename)
{
  
  std::map<double, double> spectrum;
  double energy, value;
  
  {
    std::ifstream DataFile(filename);
    if(!DataFile.is_open()) std::cout << "ERROR: Cannot open file '" << filename << "'!" << std::endl;
    while(DataFile >> energy >> value) spectrum[energy] = value;

  }

  h_Spectrum = TH1D("h_Spectrum", "h_Spectrum", 1000, std::begin(spectrum)->first, std::end(spectrum)->first);
  for(int jBin = 0; jBin < h_Spectrum.GetNbinsX(); ++jBin){
    
    h_Spectrum.SetBinContent(jBin+1, interpolate(spectrum, h_Spectrum.GetBinCenter(jBin+1)));
  }
}

