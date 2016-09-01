
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "TFile.h"
#include "TTree.h"
#include "G4Event.hh"

#include "G4ThreeVector.hh"
#include "HistoManagerMessenger.hh"
#include "CrystalHit.hh"

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
    void setEnergy_Nudetector(double energy);
    void setEnergy_Veto(double energy);
    void fill();
    void FillTree(const G4Event* anEvent, CrystalHitsCollection* theHits);

  private:
    HistoManagerMessenger* fHistoMessenger;
    TFile* outfile;
    TTree* eventtree;
    TTree* othervolumestree;
    TTree* primarytree;
    
    G4String      outputPath;
    G4String      outputName;    

    double primaryEnergy;
    double primaryX0,primaryY0,primaryZ0;
    double NeutronRecoilX0,NeutronRecoilY0,NeutronRecoilZ0;
    int primaryPDGID;
    int i_Gdflag;


    double edep_detector;
    double edep_detector_crosscheck;
    double edep_veto;
};


#endif
