#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
   ~PrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction*			fGeneratorAction;
    G4UIdirectory*        			fGeneratorDir;     
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_neutron;    
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_gamma;  
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_radius;
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_height;  
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_heightoffset;  
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_thickness;
    G4UIcmdWithAString*	        fTypeCmd_spectrum; 
    G4UIcmdWithABool*	        fTypeCmd_spectrum_bool;
    G4UIcmdWithABool*           fTypeCmd_spectrumxlog_bool;
    G4UIcmdWithABool*           fTypeCmd_gps;
    G4UIcmdWith3VectorAndUnit*  fTypeCmd_neutronpoint_vec;
    G4UIcmdWith3VectorAndUnit*  fTypeCmd_gammapoint_vec;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
