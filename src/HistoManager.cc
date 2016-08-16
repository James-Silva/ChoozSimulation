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
    fileName = "ChoozSimulation"+runNumString+".root";
  else if(outputPath == "")
    fileName = outputName;
  else if(outputName == "")
    fileName = outputPath+"/ChoozSimulation"+runNumString+".root";
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
  eventtree->Branch("NuDetector", &edep_nudetector);
  eventtree->Branch("int_Gdflag", &i_Gdflag);
  
  othervolumestree = new TTree("othervolumetree", "eventtree");
  othervolumestree->Branch("NuDetector_crosscheck", &edep_nudetector_crosscheck);
  othervolumestree->Branch("Vetodetector", &edep_veto);

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


void HistoManager::setEnergy_Nudetector(double energy)
{
  edep_nudetector_crosscheck = energy;
}

void HistoManager::setEnergy_Veto(double energy)
{
  edep_veto = energy;
}


void HistoManager::fill()
{
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
  edep_nudetector = 0.;
  GdCaptureX0  = -99999;
  GdCaptureY0  = -99999;
  GdCaptureZ0  = -99999;
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
  for(G4int i_hitcounter = 0; i_hitcounter < nhits; i_hitcounter++)
  {
    s_currentvol = (*theHits)[i_hitcounter]->GetVolume();
    s_currentprocess = (*theHits)[i_hitcounter]->GetProcessID();
    i_PDGID = (*theHits)[i_hitcounter]->GetPDGID();
    if (i_PDGID == 1000641580)
    {
       b_gdflag = true;
       std::cout << "Gd Detected" <<  std::endl;
       GdCaptureX0 = (*theHits)[i_hitcounter]->GetPosition0().getX();
       GdCaptureY0 = (*theHits)[i_hitcounter]->GetPosition0().getY();
       GdCaptureZ0 = (*theHits)[i_hitcounter]->GetPosition0().getZ();
       std::cout << "Position: " << (*theHits)[i_hitcounter]->GetPosition0() << std::endl;
    }  
    if (i_PDGID == 2112 && s_currentvol == "NuDetector")
    {  
      edep_nudetector += (*theHits)[i_hitcounter]->GetEdep();
      if (s_currentprocess == "nCapture")
      {
        b_ncapflag = true;
        std::cout << "Neutron Capture Detected" <<  std::endl;
      }  
    }  

  }   
  if (b_ncapflag == true && b_gdflag == true)
  {
    i_Gdflag = 1;
  }  
  edep_nudetector = edep_nudetector/keV;
  std::cout << "Primary Energy: " << primaryEnergy << " keV"  <<  std::endl;
  std::cout << "Total: " << edep_nudetector << " keV"  << std::endl;

  if(eventtree)
    eventtree->Fill();
  if (primarytree)
    primarytree->Fill();
}