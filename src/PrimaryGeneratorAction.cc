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
using namespace std;

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
    setNeutronPosition();
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }  
  else if(sourceType == "neutronpoint")
  {  
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
    setNeutronPosition();  
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } 
  else if(sourceType == "neutronspectrum")
  {  
    randomenergy = h_Spectrum.GetRandom();
    //cout << "Random Energy selected: " << randomenergy << endl;
    buildNeutronSource(randomenergy); //Input energy in MeV
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
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="neutron");
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
  G4double d_innerradius = neutronradius-(0.5*sourcethickness);
  G4double d_outerradius = neutronradius+(0.5*sourcethickness);
  G4double d_volumesides = TMath::Pi() * (neutronheight) * (TMath::Power(d_outerradius,2)-TMath::Power(d_innerradius,2));
  G4double d_volumetop = TMath::Pi() * TMath::Power(d_innerradius,2) * sourcethickness;
  G4double d_totalvolume = d_volumetop + d_volumesides;
  G4double d_probside = d_volumesides/d_totalvolume;
  G4double d_picksurface = G4UniformRand();
  G4double d_randomaddition = 2*(G4UniformRand()-0.5)*(0.5*sourcethickness);
  G4double d_randomaddition_bottom = 2*(G4UniformRand()-0.5)*(0.5*sourcethickness);
  G4double d_bottomdisk =  -0.5*neutronheight+sourceoffsetz-(0.5*sourcethickness);
  G4ThreeVector neutronSourcePos(0.,0.,0.);
  //std::cout<<"offset: " << sourceoffsetz/mm << endl;
  d_randomaddition = 0;
  if (d_picksurface <= d_probside)
  {
    //std::cout << "Side picked" << std::endl;
    neutronSourcePos = GenerateSideWallEvent(neutronradius+d_randomaddition,neutronheight,sourceoffsetz-(0.5*sourcethickness));
    //std::cout << "Z position: " << neutronSourcePos.getZ() << std::endl;
  }
  else 
  {
    //std::cout << "Bottom picked" << std::endl;
    neutronSourcePos = GenerateTopEvent(d_innerradius,d_bottomdisk+d_randomaddition_bottom);
  }  

  //std::cout << "Neutron Source Pos: " << neutronSourcePos << std::endl; 
  G4ThreeVector neutronMomentum = GenerateIsotropicVector();
  neutronMomentum.setMag(1.);

  fParticleGun->SetParticleMomentumDirection(neutronMomentum);
  fParticleGun->SetParticlePosition(neutronSourcePos);
  //std::cout << "Zposition of init: " << neutronSourcePos.getZ() << std::endl;
}


G4ThreeVector PrimaryGeneratorAction::GenerateIsotropicVector()
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double randTheta = TMath::Pi() * (G4UniformRand()-0.5);
  G4double radius = 1;
  G4double x = radius*TMath::Cos(randPhi)*TMath::Sin(randTheta);
  G4double y = radius*TMath::Sin(randPhi)*TMath::Sin(randTheta);
  G4double z = radius*TMath::Cos(randTheta);
  G4ThreeVector Isovector(x,y,z);  
  return Isovector;

}

G4ThreeVector PrimaryGeneratorAction::GenerateSideWallEvent(G4double radius,G4double height,G4double offset)
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double randz =  height*(G4UniformRand()-0.5)+offset;
  //G4ThreeVector test_vec(9*cm,9*cm,0*cm);
  G4ThreeVector SourcePos(radius * TMath::Cos(randPhi)*mm,
                          radius * TMath::Sin(randPhi)*mm,
                          randz);
  return SourcePos;
}

G4ThreeVector PrimaryGeneratorAction::GenerateTopEvent(G4double radius,G4double height)
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double sourceradius = G4UniformRand() * radius;
  //std::cout << "Bottom r,z:  " << sourceradius << " , " << height << std::endl;
  //G4ThreeVector test_vec(9*cm,9*cm,0*cm);
  G4ThreeVector SourcePos(sourceradius * TMath::Cos(randPhi)*mm,
                          sourceradius * TMath::Sin(randPhi)*mm,
                          height);
  return SourcePos;
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
    if(!DataFile.is_open()) std::cout << "ERROR: Cannot open file " << filename << "!" << std::endl;
    while(DataFile >> energy >> value) spectrum[energy] = value;

  }

  h_Spectrum = TH1D("h_Spectrum", "h_Spectrum", 1000, std::begin(spectrum)->first, std::end(spectrum)->first);
  for(int jBin = 0; jBin < h_Spectrum.GetNbinsX(); ++jBin){
    
    h_Spectrum.SetBinContent(jBin+1, interpolate(spectrum, h_Spectrum.GetBinCenter(jBin+1)));
  }
}

