#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"
#include "time.h"

#include <cstdlib>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc,char** argv)
{
  // check for correct # of arguments
  if(argc != 3 && argc != 4)
  {
    std::cerr << "Usage: ShieldingSim [macro name] [output file label] [vis (optional)]" << std::endl;
    return 0;
  }

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserInitialization(new ActionInitialization(detector));

  // Initialize G4 kernel
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // set the run ID
  runManager->SetRunIDCounter(atoi(argv[2]));

  // enable visualizer if selected
  G4VisManager* visManager;
  G4UIExecutive* ui;
  visManager = new G4VisExecutive;
  visManager->Initialize();
  G4String fileName;
  fileName = argv[1];
  if(argc == 4 && std::string(argv[3]) == "vis")
  {

    ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+fileName);    
  }
  if(argc == 3)
  {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+fileName);    
  }
  // actually execute the command
  //G4String command = "/control/execute ";
  //UImanager->ApplyCommand(command+fileName);
  // Job termination
  if(argc == 4 && std::string(argv[3]) == "vis")
  {
    delete ui;
    delete visManager;
  }
  delete runManager;
  return 0;
}
