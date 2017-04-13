#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4LossTableManager.hh"
#include "G4StepLimiter.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"


#include "G4Decay.hh"
#include "G4ProcessManager.hh"


PhysicsList::PhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 0.1*CLHEP::mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  this->RegisterPhysics(new G4DecayPhysics());

}


PhysicsList::~PhysicsList()
{}


void PhysicsList::SetCuts()
{
  if (verboseLevel >0) {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verboseLevel>0) DumpCutValuesTable();
}
