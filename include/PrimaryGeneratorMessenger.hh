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

// An object created in the PrimaryGeneratorAction (PGA) that defines commands relating to the
// PGA that are used in the macro file called when running the program.
class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    // Initializes all of the command variables. Here the command names are created and the
    // type of parameters they accept set.
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);

    // Deletes the command objects that were created.
   ~PrimaryGeneratorMessenger();

    // Automatically called by Geant. Matches the macro command to one of the commands
    // declared in the constructor and calls a function in the PrimaryGeneratorAction.
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    // Object to call methods from the PrimaryGeneratorAction
    PrimaryGeneratorAction*			fGeneratorAction;
    // Creates a dircectory in the UI where the commands are stored. This dircectory can
    // seen in the visualizer.
    G4UIdirectory*        			fGeneratorDir;
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_neutron;
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_gamma;
    G4UIcmdWithADoubleAndUnit*	fTypeCmd_radius;
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_height;
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_heightoffset;
    G4UIcmdWithADoubleAndUnit*  fTypeCmd_thickness;
    G4UIcmdWithAString*	        fTypeCmd_spectrum;
    G4UIcmdWithABool*	          fTypeCmd_spectrum_bool;
    G4UIcmdWithABool*           fTypeCmd_spectrumxlog_bool;
    G4UIcmdWithABool*           fTypeCmd_gps;
    G4UIcmdWith3VectorAndUnit*  fTypeCmd_neutronpoint_vec;
    G4UIcmdWith3VectorAndUnit*  fTypeCmd_gammapoint_vec;
    G4UIcmdWithAString*         fTypeCmd_generateMuon;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
