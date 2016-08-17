#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

using namespace std;

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* PrimaryGen)
:G4UImessenger(),fGeneratorAction(PrimaryGen),
 fGeneratorDir(0)
{
  fGeneratorDir = new G4UIdirectory("/choozsim/generator/");
  fGeneratorDir->SetGuidance("generator control");

  fTypeCmd_gps = new G4UIcmdWithABool("/choozsim/generator/SetGPSSource",this);
  fTypeCmd_gps->SetGuidance("SetGPSsource (true or false)");
  fTypeCmd_gps->SetDefaultValue("false");

  fTypeCmd_neutron = new G4UIcmdWithADoubleAndUnit("/choozsim/generator/neutronsource",this);
  fTypeCmd_neutron->SetGuidance("Set type of particle generator and set energy");
  
  fTypeCmd_gamma = new G4UIcmdWithADoubleAndUnit("/choozsim/generator/gammasource",this);
  fTypeCmd_gamma->SetGuidance("Set type of particle generator and set energy");

  fTypeCmd_radius = new G4UIcmdWithADoubleAndUnit("/choozsim/generator/setsourceradius",this);
  fTypeCmd_radius->SetGuidance("Set source radius (sets both gamma and neutron generator radius)");
  fTypeCmd_radius->SetDefaultValue(100*CLHEP::mm);

  fTypeCmd_height = new G4UIcmdWithADoubleAndUnit("/choozsim/generator/setsourceheight",this);
  fTypeCmd_height->SetGuidance("Set source height (cylinder) (sets both gamma and neutron generator radius)");
  fTypeCmd_height->SetDefaultValue(100*CLHEP::mm);

  fTypeCmd_spectrum = new G4UIcmdWithAString("/choozsim/generator/setneutronsourcespectrum",this);
  fTypeCmd_spectrum->SetGuidance("Set source file for generating neutron spectrum");

  fTypeCmd_spectrum_bool = new G4UIcmdWithABool("/choozsim/generator/SetNeutronSource", this);
  fTypeCmd_spectrum_bool->SetGuidance("Set neutron source (bool type true or false)");
  fTypeCmd_spectrum_bool->SetDefaultValue("false");
  
  fTypeCmd_neutronpoint_vec = new G4UIcmdWith3VectorAndUnit("/choozsim/generator/SetPointNeutronSource", this);
  fTypeCmd_neutronpoint_vec->SetGuidance("Set point neutron source (coordinates)");

  fTypeCmd_gammapoint_vec = new G4UIcmdWith3VectorAndUnit("/choozsim/generator/SetPointGammaSource", this);
  fTypeCmd_gammapoint_vec->SetGuidance("Set point gamma source (coordinates)");

  fTypeCmd_spectrumxlog_bool = new G4UIcmdWithABool("/choozsim/generator/SetNeutronSource_xlog", this);
  fTypeCmd_spectrumxlog_bool->SetGuidance("Set if neutron spectrum source has units of logx (bool type true or false)");
  fTypeCmd_spectrumxlog_bool->SetDefaultValue("false");
  //fTypeCmd->SetCandidates("gamma");
  //fTypeCmd->SetParameterName("type",false);
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fTypeCmd_neutron;
  delete fGeneratorDir;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fTypeCmd_neutron)
  {     
    fGeneratorAction->setNewNeutronSource(fTypeCmd_neutron->GetNewDoubleValue(newValue));
  }
  if(command == fTypeCmd_gamma)
  {     
    fGeneratorAction->setNewGammaSource(fTypeCmd_gamma->GetNewDoubleValue(newValue));
  }  
  if(command == fTypeCmd_radius)
  {
    fGeneratorAction->setSourceRadius(fTypeCmd_radius->GetNewDoubleValue(newValue));
  }
  if(command == fTypeCmd_height)
  {
    fGeneratorAction->setSourceHeight(fTypeCmd_height->GetNewDoubleValue(newValue));
  }

  if(command == fTypeCmd_spectrum)
  {
    fGeneratorAction->SetSpectralData(newValue);
  }
  if(command == fTypeCmd_spectrum_bool && newValue == "true")
  {
    fGeneratorAction->setNewNeutronSpectrumSource();
  }
  if(command == fTypeCmd_spectrumxlog_bool && newValue == "true")
  {
    fGeneratorAction->setNewNeutronSpectrumSource_LogX();
  }
  if(command == fTypeCmd_neutronpoint_vec)
  { 
      fGeneratorAction->SetNeutronPointSource(fTypeCmd_neutronpoint_vec->GetNew3VectorValue(newValue));  
  }  
  if(command == fTypeCmd_gammapoint_vec)
  { 
      fGeneratorAction->SetGammaPointSource(fTypeCmd_neutronpoint_vec->GetNew3VectorValue(newValue));  
  }    
  if(command == fTypeCmd_gps)
  { 
      fGeneratorAction->SetGenerator("GPS");  
  } 
}
