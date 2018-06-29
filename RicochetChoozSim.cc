#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include "Materials.hh"


#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc,char** argv)
{
  // check for correct # of arguments
  if(argc != 2 && argc != 3)
  {
    std::cerr << "Usage: RicochetChoozSim [macro name] [vis (optional)]" << std::endl;
    return 0;
  }

  // Construct the default run manager
  G4RunManager runManager;

  // Create Materials
  detectorcomponents::Materials::CreateMaterials();

  // Set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction;
  runManager.SetUserInitialization(detector);
  runManager.SetUserInitialization(new PhysicsList);
  runManager.SetUserInitialization(new ActionInitialization(detector));

  // Initialize G4 kernel
  runManager.Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // set the run ID
  //runManager.SetRunIDCounter(atoi(argv[2]));

  G4String fileName{argv[1]};
  // Run Command: ./RicochetChoozSim runWithVis.mac --vis
  if(argc == 3 && std::string(argv[2]) == "--vis")
  {
      // enable visualizer if selected
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute "+fileName);
    ui->SessionStart();

    delete ui;
    delete visManager;
  }
  else if(argc == 2) UImanager->ApplyCommand("/control/execute "+fileName);

  return 0;
}
