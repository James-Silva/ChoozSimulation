// PrimaryGeneratorAction.hh
//
// Class used to generate primary particles. Current interface simply fires
// 5.19 keV (corresponding to 55Fe) x-rays at the NaCl source, which then
// fluoresces and produces secondary x-rays and photoelectronics that
// strike the detector
//
// Adam Anderson
// adama@mit.edu


#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include <fstream>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "HistoManager.hh"
#include "globals.hh"

#include "TH1D.h"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(DetectorConstruction*, HistoManager*);
    virtual ~PrimaryGeneratorAction() = default;

    virtual void GeneratePrimaries(G4Event*);
    void buildSource(const G4String& particleName, G4double kineticEnergy);
    void setGammaPosition();
    void setNeutronPosition();
    void setNeutronMomentum();
    void setNewNeutronSource(G4double energy);
    void setNewGammaSource(G4double energy);
    void setSourceRadius(G4double radius);
    void setSourceHeight(G4double radius);
    void setSourceHeightOffset(G4double offset);
    void SetSpectralData(const std::string& fileName);
    void setNewNeutronSpectrumSource();
    void setNewNeutronSpectrumSource_LogX();
    void setPointNeutronPosition();
    void setPointGammaPosition();
    void SetNeutronPointSource(G4ThreeVector pos);
    void SetGammaPointSource(G4ThreeVector pos);
    void SetGenerator(G4String generatorType);
    void setSourceThickness(G4double thickness);
    G4ThreeVector GenerateSideWallEvent(G4double radius,G4double height,G4double offset);
    G4ThreeVector GenerateTopEvent(G4double radius,G4double height);
    G4ThreeVector GenerateIsotropicVector();
    virtual void print(std::ostream& output, double scalingUnit) const;//print in HEPEvt format if scalingUnit == CLHEP::GeV
private:
    TH1D 			h_Spectrum;
    std::vector<double> 	vec_SpectralEnergies;
    std::vector<double> 	vec_SpectralVals;
    G4double                    neutronradius;
    G4double                    sourceoffsetz;
    G4double                    sourceRadius;
    G4double                    sourceHeight;
    G4double 			bottomProbability; //ratio of the bottom surface to the side surface
    G4double                    sourcethickness;
    std::unique_ptr<PrimaryGeneratorMessenger>		messenger;
    std::unique_ptr<G4ParticleGun>          		particleGun;
    DetectorConstruction*    				detectorConstruction;
    HistoManager*						fHistoManager;
    G4ThreeVector                   neutronsourcepos;
    G4ThreeVector                   gammasourcepos;
    G4VUserPrimaryGeneratorAction*  particleSource;    //pointer a to G4  class

    G4String sourceType; 
    bool logaxis;
    std::ofstream outputFileStream;//for HEPEvt output
    
    void updateBottomProbability();
};

std::ostream& operator<<(std::ostream& output, const PrimaryGeneratorAction& primaryGeneratorAction);

template <class T, class K>
K interpolate(const std::map<T,K>& map, T x){
  
  auto itUpper = map.upper_bound(x);
  
  if(itUpper == map.end()) return (--itUpper)->second;
  else if(itUpper == map.begin()) return itUpper->second;
  else{
    
    auto itLower = itUpper;
    --itLower;
    return itLower->second + (x - itLower->first) * (itUpper->second - itLower->second) / (itUpper->first - itLower->first) ;//y1 + (x - x1) (y2 - y1) / (x2 - x1)
    
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
