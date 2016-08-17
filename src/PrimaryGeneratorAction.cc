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
#include "Math/Interpolator.h"
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
    buildNeutronSource(1000*randomenergy); //Input energy in MeV
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
    gammaSourcePos = GenerateTopEvent(gammaradius,gammaheight/2.0);
  } 
  else if (d_picksurface <= d_probtop+d_probside && d_picksurface >= d_probtop)
  {
    gammaSourcePos = GenerateSideWallEvent(gammaradius,gammaheight);
  }
  else 
  {
    gammaSourcePos = GenerateTopEvent(gammaradius,-gammaheight/2.0);
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
  G4double d_surfacesides = 2. * TMath::Pi() * neutronheight * neutronradius;
  G4double d_surfacetop = TMath::Pi() * neutronradius * neutronradius;
  G4double d_totalsurface = 2.*d_surfacetop + d_surfacesides;
  G4double d_probtop = d_surfacetop/d_totalsurface;
  G4double d_probside = d_surfacesides/d_totalsurface;
  G4double d_picksurface = G4UniformRand();
  G4ThreeVector neutronSourcePos(0.,0.,0.);
  if (d_picksurface < d_probtop)
  {
    //std::cout << "Top picked" << std::endl;
    neutronSourcePos = GenerateTopEvent(neutronradius,neutronheight/2.0);
  } 
  else if (d_picksurface <= d_probtop+d_probside && d_picksurface >= d_probtop)
  {
    std::cout << "Side picked" << std::endl;
    neutronSourcePos = GenerateSideWallEvent(neutronradius,neutronheight);
  }
  else 
  {
    //std::cout << "Bottom picked" << std::endl;
    neutronSourcePos = GenerateTopEvent(neutronradius,-neutronheight/2.0);
  }  

  //std::cout << "Neutron Source Pos: " << neutronSourcePos << std::endl; 
  G4ThreeVector neutronMomentum = neutronSourcePos;
  
  neutronMomentum.setMag(1.);
  neutronMomentum *= -1.;

  fParticleGun->SetParticleMomentumDirection(neutronMomentum);
  fParticleGun->SetParticlePosition(neutronSourcePos);
}

G4ThreeVector PrimaryGeneratorAction::GenerateSideWallEvent(G4double radius,G4double height)
{
  G4double randPhi = 2. * TMath::Pi() * G4UniformRand();
  G4double randz =  height*(G4UniformRand()-0.5);
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

void PrimaryGeneratorAction::SetSpectralData(G4String filename)
{
  // check file existence
    std::ifstream DataFile(filename);
    if(!DataFile.is_open())
      std::cout << "ERROR: Cannot open file " << filename << "!" << std::endl;

    // read text file
    double energy, value;
    while(DataFile >> energy >> value)
  {
    vec_SpectralEnergies.push_back(energy);
    vec_SpectralVals.push_back(value);
    //std::cout << value << std::endl;
  }
  DataFile.close();

  ROOT::Math::Interpolator SpectrumInterp(vec_SpectralEnergies, vec_SpectralVals, ROOT::Math::Interpolation::kLINEAR);
  h_Spectrum = TH1D("h_Spectrum", "h_Spectrum", 1000, vec_SpectralEnergies.front(), vec_SpectralEnergies.back());
  for(int jBin = 0; jBin < h_Spectrum.GetNbinsX(); jBin++)
  {
    h_Spectrum.SetBinContent(jBin+1, SpectrumInterp.Eval(h_Spectrum.GetBinCenter(jBin+1)));
  }
}

