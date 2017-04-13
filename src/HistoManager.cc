//#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4VProcess.hh"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "G4UnitsTable.hh"

#include "HistoManager.hh"

using namespace CLHEP;

HistoManager::HistoManager() 
{ 
  // create commands for interactive definition of the histo manager
  fHistoMessenger = new HistoManagerMessenger(this);
}

HistoManager::~HistoManager()
{ }


void HistoManager::book(const G4Run* aRun)
{
  // Creating trees to write output
  G4int runID = aRun->GetRunID();
  std::ostringstream int2str;
  int2str << runID;
  G4String runNumString = int2str.str();
  G4String fileName;
  if(outputPath == "" && outputName == "")
    fileName = "RicochetChoozSimulation"+runNumString+".root";
  else if(outputPath == "")
    fileName = outputName;
  else if(outputName == "")
    fileName = outputPath+"/RicochetChoozSimulation"+runNumString+".root";
  else
    fileName = outputPath+"/"+outputName;
  outfile = new TFile(fileName,"RECREATE");

  if(!outfile)
  {
    G4cout << "HistoManager::book: problem creating output ROOT file!"
        << G4endl;
    return;
  }

  primarytree = new TTree("primarytree", "primarytree");
  primarytree->Branch("energy", &primaryEnergy);
  primarytree->Branch("PrimaryX0", &primaryX0);
  primarytree->Branch("PrimaryY0", &primaryY0);
  primarytree->Branch("PrimaryZ0", &primaryZ0);

  eventtree = new TTree("eventtree", "eventtree");
  eventtree->Branch("Detector", &edep_detector);
  eventtree->Branch("int_Gdflag", &i_Gdflag);
  eventtree->Branch("NeutronInteractionX0", &NeutronRecoilX0);
  eventtree->Branch("NeutronInteractionY0", &NeutronRecoilY0);
  eventtree->Branch("NeutronInteractionZ0", &NeutronRecoilZ0);
  eventtree->Branch("NeutronInteractionProcess", s_NeutronProcess,"string/C",1024);
  othervolumestree = new TTree("othervolumetree", "eventtree");
  othervolumestree->Branch("Detector_crosscheck", &edep_detector_crosscheck);
  othervolumestree->Branch("Vetodetector", &edep_veto);
  othervolumestree->Branch("Edep_Poly", &edep_poly);
  othervolumestree->Branch("watershield_lastX0", &watershield_lastX0);
  othervolumestree->Branch("watershield_lastY0", &watershield_lastY0);
  othervolumestree->Branch("watershield_lastZ0", &watershield_lastZ0);
  othervolumestree->Branch("crystal_firstX0", &crystal_firstX0);
  othervolumestree->Branch("crystal_firstY0", &crystal_firstY0);
  othervolumestree->Branch("crystal_firstZ0", &crystal_firstZ0); 

  G4cout << "\n----> Histogram file is opened in " << fileName << G4endl;
}


void HistoManager::save()
{
  if (outfile) {
    outfile->Write();       // Writing the histograms to the file
    outfile->Close();       // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}


// Set the energy of the primary particle for this event
void HistoManager::setPrimaryEnergy(double energy)
{
  primaryEnergy = energy/keV;
}


// Set the ID of the primary particle for this event
void HistoManager::setPrimaryPDGID(int id)
{
  primaryPDGID = id;
}


void HistoManager::setEnergy_detector(double energy)
{
  edep_detector_crosscheck = energy;
}

void HistoManager::setEnergy_Veto(double energy)
{
  edep_veto = energy;
}

void HistoManager::setEnergy_Poly(double energy)
{
  edep_poly = energy;
}

void HistoManager::storeleavepoint_watershield(G4ThreeVector vec_position_watershielding)
{
   vec_preposition_watershielding = vec_position_watershielding;
}

void HistoManager::storeentrypoint_crystal(G4ThreeVector vec_position)
{
   vec_postposition_crystal = vec_position;
}

void HistoManager::fill()
{
  watershield_lastX0 = -99999;
  watershield_lastY0 = -99999;
  watershield_lastZ0 = -99999;
  crystal_firstX0 = -999999;
  crystal_firstY0 = -999999;
  crystal_firstZ0 = -999999;
  watershield_lastX0 = vec_preposition_watershielding.getX();
  watershield_lastY0 = vec_preposition_watershielding.getY();
  watershield_lastZ0 = vec_preposition_watershielding.getZ();
  crystal_firstX0 = vec_postposition_crystal.getX();
  crystal_firstY0 = vec_postposition_crystal.getY();
  crystal_firstZ0 = vec_postposition_crystal.getZ();

  othervolumestree->Fill();
}

void HistoManager::SetOutputPath(const G4String path)
{
  outputPath = path;
}

void HistoManager::SetOutputName(const G4String name)
{
  outputName = name;
}


void HistoManager::FillTree(const G4Event* anEvent, CrystalHitsCollection* theHits)
{
  TString s_currentvol;
  TString s_currentprocess;
  G4int i_PDGID;
  bool b_gdflag = false;
  bool b_ncapflag = false;
  edep_detector = 0.;
  NeutronRecoilX0  = -99999;
  NeutronRecoilY0  = -99999;
  NeutronRecoilZ0  = -99999;

  i_Gdflag = 0;

  if (!anEvent || !theHits)
    return;

  G4int nhits = theHits->entries();
  primaryX0 = anEvent->GetPrimaryVertex()->GetX0();
  primaryY0 = anEvent->GetPrimaryVertex()->GetY0();
  primaryZ0 = anEvent->GetPrimaryVertex()->GetZ0();
  G4ThreeVector primaryR0 = G4ThreeVector(anEvent->GetPrimaryVertex()->GetX0(),
                                          anEvent->GetPrimaryVertex()->GetY0(),
                                          anEvent->GetPrimaryVertex()->GetZ0());  
  primaryEnergy = anEvent->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
  primaryEnergy = primaryEnergy/keV;
  //std::cout << "Primary Energy (keV): " << primaryEnergy << std::endl;
  for(G4int i_hitcounter = 0; i_hitcounter < nhits; i_hitcounter++)
  {
    s_currentvol = (*theHits)[i_hitcounter]->GetVolume();
    s_currentprocess = (*theHits)[i_hitcounter]->GetProcessID();
    s_NeutronProcess = s_currentprocess;
    i_PDGID = (*theHits)[i_hitcounter]->GetPDGID();
    if (i_PDGID == 1000641580)
    {
       b_gdflag = true;
       std::cout << "Gd Detected" <<  std::endl;
    }  
    if (s_currentvol == "Crystal_1")
    {  
      edep_detector += (*theHits)[i_hitcounter]->GetEdep();
      if (s_currentprocess == "hadElastic")
      {
        b_ncapflag = true;
        NeutronRecoilX0 = (*theHits)[i_hitcounter]->GetPosition0().getX();
        NeutronRecoilY0 = (*theHits)[i_hitcounter]->GetPosition0().getY();
        NeutronRecoilZ0 = (*theHits)[i_hitcounter]->GetPosition0().getZ();
        std::cout << "Hit Position: " << (*theHits)[i_hitcounter]->GetPosition0() << std::endl;


        std::cout << "Neutron Recoil Detected" <<  std::endl;
      }  
    }  

  }   
  if (b_ncapflag == true && b_gdflag == true)
  {
    i_Gdflag = 1;
  }  
  edep_detector = edep_detector/keV;
  //std::cout << "Primary Energy: " << primaryEnergy << " keV"  <<  std::endl;
  //std::cout << "Total: " << edep_nudetector << " keV"  << std::endl;
   
  if(eventtree)
    eventtree->Fill();
  if (primarytree)
    primarytree->Fill();
}