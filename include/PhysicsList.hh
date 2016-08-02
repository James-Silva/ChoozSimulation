#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class StepMax;

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();

  void SetCuts();


private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;

};

#endif
