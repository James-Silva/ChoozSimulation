// PrimaryGeneratorAction.hh

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include <fstream>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "TH1D.h"
#include "PrimaryGeneratorMessenger.hh"
#include "HistoManager.hh"
#include "TTreeContainer.hh"

class G4Event;
class DetectorConstruction;
class TFile;
class TTree;

// The required Primary Generator Action class used by the Action Initialization
// Example:
//    void ActionInitialization::Build() const {
//      DetectorConstruction* detector = new DetectorConstruction();
//      HistoManager* histoManager = new HistoManager();
//      SetUserAction(new PrimaryGeneratorAction(detector, histoManager));
//    }
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    // Initializes the DetectorConstruction and HistoManager objects.
    // Sets the messenger object with the type of PrimaryGeneratorMessenger
    // so the UI can recieve commands that call methods of this class.
    PrimaryGeneratorAction(DetectorConstruction*, HistoManager*);
    virtual ~PrimaryGeneratorAction() = default;

    // Automatically called by Geant. This method requires the sourceType to
    // be set before it is called. The sourceType is usually set by a messenger
    // command that calls a function within this class to set sourceType's value.
    // Once the correct sourceType is found, the particle definition, position,
    // and momentum or momentum direction and energy are set.
    virtual void GeneratePrimaries(G4Event*);

    // TODO: Add comments to these methods
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

    // Sets the sourceType to "muonspectrum"
    void genMuFromSpectrum(const std::string& fileName);

    G4ThreeVector GenerateSideWallEvent(G4double radius,G4double height,G4double offset);
    G4ThreeVector GenerateTopEvent(G4double radius,G4double height);
    G4ThreeVector GenerateIsotropicVector();
    virtual void print(std::ostream& output, double scalingUnit) const;//print in HEPEvt format if scalingUnit == CLHEP::GeV
private:
    G4VUserPrimaryGeneratorAction*  particleSource;    //pointer a to G4  class
    TH1D 			                  h_Spectrum;
    std::vector<double> 	      vec_SpectralEnergies;
    std::vector<double> 	      vec_SpectralVals;
    G4double                    neutronradius;
    G4double                    sourceoffsetz;
    G4double                    sourceRadius;
    G4double                    sourceHeight;
    G4double 			              bottomProbability; //ratio of the bottom surface to the side surface
    G4double                    sourcethickness;
    PrimaryGeneratorMessenger		messenger;
    G4ParticleGun          		  particleGun;
    DetectorConstruction*    	  detectorConstruction;
    HistoManager*						    fHistoManager;
    G4ThreeVector               neutronsourcepos;
    G4ThreeVector               gammasourcepos;
    G4String sourceType;
    bool     logaxis;

    // Contains the TTree with the muon data. Data like momentum can be accessed.
    primarygentools::TTreeContainer muonTreeContatiner;

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
