// ********************************************************************
// * HistoManagerMessenger.cc                                         *
// *                                                                  *
// * Messenger class for controlling the simulation output to the	  *
// * data trees.													  *
// *                                                                  *
// * Created by: Adam Anderson                                        *
// * adama@mit.edu                                                    *
// * 28 April 2013                                                    *
// ********************************************************************

#include "HistoManagerMessenger.hh"
#include "HistoManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

HistoManagerMessenger::HistoManagerMessenger(HistoManager* man)
:manager(man)
{
	treeDirectory = new G4UIdirectory("/ricochetchoozsim/tree/", this);
 	treeDirectory->SetGuidance("Commands for setting the tree output.");

	outputPathCmd = new G4UIcmdWithAString("/ricochetchoozsim/tree/setOutputFilePath", this);
	outputPathCmd->SetGuidance("Set path for output ROOT file.");
	outputPathCmd->SetDefaultValue("");

	outputNameCmd = new G4UIcmdWithAString("/ricochetchoozsim/tree/setOutputFileName", this);
	outputNameCmd->SetGuidance("Set name for output ROOT file.");
	outputNameCmd->SetDefaultValue("");
}

HistoManagerMessenger::~HistoManagerMessenger()
{
  delete treeDirectory;
  delete outputNameCmd;
  delete outputPathCmd;
}

void HistoManagerMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if(command == outputPathCmd)
	  	manager->SetOutputPath(newValue);
	if(command == outputNameCmd)
		manager->SetOutputName(newValue);
}
