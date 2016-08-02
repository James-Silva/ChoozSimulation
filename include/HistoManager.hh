
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "TFile.h"
#include "TTree.h"

#include "G4ThreeVector.hh"
#include "HistoManagerMessenger.hh"

class G4Run;
class TFile;
class TTree;
class TH1D;

class HistoManager
{
  public:

    HistoManager();
   ~HistoManager();

    void book(const G4Run* aRun);
    void save();
    void SetOutputPath(const G4String path);
    void SetOutputName(const G4String name);
    void setPrimaryEnergy(double energy);
    void setPrimaryPDGID(int id);
    void setEnergy(double energy);
    void fill();

  private:
    HistoManagerMessenger* fHistoMessenger;
    TFile* outfile;
    TTree* eventtree;
    TTree* primarytree;
    
    G4String      outputPath;
    G4String      outputName;    

    double primaryEnergy;
    int primaryPDGID;

    double edep;
};


#endif
