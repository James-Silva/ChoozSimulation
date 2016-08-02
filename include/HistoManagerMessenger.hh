// ********************************************************************
// * HistoManagerMessenger.hh                                         *
// *                                                                  *
// * Messenger class for controlling the simulation output to the	  *
// * data trees.													  *
// *                                                                  *
// * Created by: Adam Anderson                                        *
// * adama@mit.edu                                                    *
// * 28 April 2013                                                    *
// ********************************************************************

#ifndef HistoManagerMessenger_h
#define HistoManagerMessenger_h 1

#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

class G4UIcommand;
class HistoManager;

class HistoManagerMessenger : public G4UImessenger
{
public:
	HistoManagerMessenger(HistoManager* man);
	~HistoManagerMessenger();

	virtual void SetNewValue(G4UIcommand* command, G4String newValue);

private:
	HistoManager* manager;

	G4UIdirectory* treeDirectory;
  	G4UIcmdWithAString* outputPathCmd;
  	G4UIcmdWithAString* outputNameCmd;
};

#endif
