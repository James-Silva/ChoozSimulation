// PrimaryGeneratorAction.cc

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "TMath.h"
#include "G4UnitsTable.hh"
#include "GPSPrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detectorConstruction_, HistoManager* histo)
: G4VUserPrimaryGeneratorAction(),
  messenger(new PrimaryGeneratorMessenger(this)),
  particleGun(new G4ParticleGun(1)),
  detectorConstruction(detectorConstruction_),
  fHistoManager(histo)
{
  
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
  if(sourceType == "gamma")
  {  
    setGammaPosition();
    particleGun->GeneratePrimaryVertex(anEvent);
  }    
  else if(sourceType == "neutron")
  {  
    setNeutronMomentum();
    setNeutronPosition();
    particleGun->GeneratePrimaryVertex(anEvent);
  }  
  else if(sourceType == "neutronpoint")
  { 
    setNeutronMomentum();
    setPointNeutronPosition();
    particleGun->GeneratePrimaryVertex(anEvent);
  }
  else if(sourceType == "gammapoint")
  {  
    setPointGammaPosition();
    particleGun->GeneratePrimaryVertex(anEvent);
  }
  else if(sourceType == "neutronspectrum" && logaxis == true)
  {  
    auto kineticEnergy = TMath::Power(10,h_Spectrum.GetRandom());
    buildSource("neutron",kineticEnergy);
    setNeutronMomentum();
    setNeutronPosition();  
    particleGun->GeneratePrimaryVertex(anEvent);
  } 
  else if(sourceType == "neutronspectrum")
  {  
    auto kineticEnergy = h_Spectrum.GetRandom();
    buildSource("neutron", kineticEnergy); //Input energy in MeV
    setNeutronMomentum();
    setNeutronPosition();
    particleGun->GeneratePrimaryVertex(anEvent);
  } 
  else if(sourceType == "GPS")
  {  
    particleSource->GeneratePrimaries(anEvent);
  } 
  else
    std::cerr << "ERROR in PrimaryGeneratorAction::GeneratePrimaries: Invalid source type selected!" << std::endl;

  // Actually generate the event

}

void PrimaryGeneratorAction::buildSource(const G4String& particleName, G4double energy)
{
  particleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle(particleName));
  particleGun->SetParticleEnergy(energy*MeV); // The default unit for energy in Geant4 is MeV and it seems to convert all input energies into MeV
}

void PrimaryGeneratorAction::setGammaPosition()
{
  // generate particles on a cylinder around the detector, with momenta pointing
  // radially inward
  //G4double d_shieldingheight = 7000.0*mm;
  G4double d_surfacesides = 2. * TMath::Pi() * sourceHeight * sourceRadius;
  G4double d_surfacetop = TMath::Pi() * sourceRadius * sourceRadius;
  G4double d_totalsurface = 2.*d_surfacetop + d_surfacesides;
  G4double d_probtop = d_surfacetop/d_totalsurface;
  G4double d_probside = d_surfacesides/d_totalsurface;
  G4double d_picksurface = G4UniformRand();
  G4ThreeVector gammaSourcePos(0.,0.,0.);
  if (d_picksurface < d_probtop)
  {
    gammaSourcePos = GenerateTopEvent(sourceRadius,(sourceHeight/2.0)+sourceoffsetz);
  } 
  else if (d_picksurface <= d_probtop+d_probside && d_picksurface >= d_probtop)
  {
    gammaSourcePos = GenerateSideWallEvent(sourceRadius,sourceHeight,sourceoffsetz);
  }
  else 
  {
    gammaSourcePos = GenerateTopEvent(sourceRadius,-(sourceHeight/2.0)+sourceoffsetz);
  }  
  G4ThreeVector gammaMomentum = gammaSourcePos;
  //gammaSourcePos=test_vec;
  
  gammaMomentum.setMag(1.);
  gammaMomentum *= -1.;

  particleGun->SetParticleMomentumDirection(gammaMomentum);
  particleGun->SetParticlePosition(gammaSourcePos);
}

void PrimaryGeneratorAction::setPointNeutronPosition()
{
  G4ThreeVector neutronMomentum = neutronsourcepos;
  neutronMomentum.setMag(1.);
  neutronMomentum *= -1.;

  particleGun->SetParticleMomentumDirection(neutronMomentum);
  particleGun->SetParticlePosition(neutronsourcepos);  
}

void PrimaryGeneratorAction::setPointGammaPosition()
{
  G4ThreeVector gammaMomentum = gammasourcepos;
  gammaMomentum.setMag(1.);
  gammaMomentum *= -1.;

  particleGun->SetParticleMomentumDirection(gammaMomentum);
  particleGun->SetParticlePosition(gammasourcepos);  
}

void PrimaryGeneratorAction::setNeutronPosition()
{	
  if (G4UniformRand() < bottomProbability) particleGun->SetParticlePosition(GenerateTopEvent(sourceRadius, -0.5 * sourceHeight));
  else particleGun->SetParticlePosition(GenerateSideWallEvent(sourceRadius,sourceHeight,sourceoffsetz));

}

void PrimaryGeneratorAction::setNeutronMomentum()
{
  G4ThreeVector neutronMomentum = GenerateIsotropicVector();
  particleGun->SetParticleMomentumDirection(neutronMomentum);
}


G4ThreeVector PrimaryGeneratorAction::GenerateIsotropicVector()
{
  G4double phi = 2. * TMath::Pi() * G4UniformRand();
  G4double cosTheta = 2 * (G4UniformRand()-0.5);
  return G4ThreeVector(std::cos(phi)*std::sqrt(1-std::pow(cosTheta,2)), std::sin(phi)*std::sqrt(1-std::pow(cosTheta,2)), cosTheta);

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
  radius = std::sqrt(G4UniformRand() * std::pow(radius,2));
  
  return G4ThreeVector(radius * TMath::Cos(randPhi), radius * TMath::Sin(randPhi), height);
}

void PrimaryGeneratorAction::setSourceRadius(G4double sourceRadius_)
{
    sourceRadius = sourceRadius_;
    updateBottomProbability();
}

void PrimaryGeneratorAction::setSourceHeight(G4double sourceHeight_)
{
    sourceHeight = sourceHeight_;
    updateBottomProbability();
}

void PrimaryGeneratorAction::updateBottomProbability()
{
    bottomProbability = sourceRadius / (sourceRadius + 2 * sourceHeight); // surface ratio reads (pi r^2) / (pi r^2 + 2 pi r h)
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
  buildSource("neutron",energy);
}

void PrimaryGeneratorAction::setNewGammaSource(G4double energy)
{
  sourceType = "gamma";
  buildSource("gamma", energy);
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
  
  if(!spectrum.empty()){

    h_Spectrum = TH1D("h_Spectrum", "h_Spectrum", 1000, spectrum.begin()->first, spectrum.rbegin()->first);
    for(int k = 0; k < h_Spectrum.GetNbinsX(); ++k)
      h_Spectrum.SetBinContent(k+1, interpolate(spectrum, h_Spectrum.GetBinCenter(k+1)));
  
  }
  else throw std::runtime_error("The spectrum read from "+filename+"is empty!");

}

