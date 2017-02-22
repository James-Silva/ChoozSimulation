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
	virtual ~PrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);
	void buildGammaSource(G4double energy);
	void setGammaPosition();
	void buildNeutronSource(G4double energy);
	void setNeutronPosition();
	void setNewNeutronSource(G4double energy);
    void setNewGammaSource(G4double energy);
	void setSourceRadius(G4double radius);
	void setSourceHeight(G4double radius);
	void setSourceHeightOffset(G4double offset);
	void SetSpectralData(G4String filename);
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
private:
    TH1D 							h_Spectrum;
    std::vector<double> 			vec_SpectralEnergies;
	std::vector<double> 			vec_SpectralVals;
	G4double                        neutronradius;
	G4double                        sourceoffsetz;
	G4double                        gammaradius;
	G4double                        neutronheight;
	G4double                        gammaheight;
	G4double                        sourcethickness;
	PrimaryGeneratorMessenger*		fMessenger;
	G4ParticleGun*           		fParticleGun;  //pointer a to G4 class
	DetectorConstruction*    		fDetector;     //pointer to the geometry
	HistoManager*					fHistoManager;
	G4ThreeVector                   neutronsourcepos;
	G4ThreeVector                   gammasourcepos;
	G4VUserPrimaryGeneratorAction*  particleSource;    //pointer a to G4  class

	G4String sourceType; 
    bool logaxis; 
};


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
