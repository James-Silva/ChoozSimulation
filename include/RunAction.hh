#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class HistoManager;

class RunAction : public G4UserRunAction
{
public:
  RunAction(HistoManager*);
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  inline void SetAutoSeed (const G4bool val) {fAutoSeed = val;}

private:
  HistoManager* fHistoManager;
  G4int fSaveRndm;
  G4bool fAutoSeed;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
