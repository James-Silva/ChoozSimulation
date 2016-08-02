#include "HistoManager.hh"
#include "HistoManagerMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

#include <iostream>

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
    fileName = "Shieldingsim"+runNumString+".root";
  else if(outputPath == "")
    fileName = outputName;
  else if(outputName == "")
    fileName = outputPath+"/Shieldingsim"+runNumString+".root";
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
  primarytree->Branch("PDGID", &primaryPDGID);

  eventtree = new TTree("eventtree", "eventtree");
  eventtree->Branch("Crystal1", &edep);

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


void HistoManager::setEnergy(double energy)
{
  edep = energy;
}


void HistoManager::fill()
{
  // don't bother doing anything to the event tree unless edep > 0
  if(edep > 0.)
  {
    eventtree->Fill();

    // reset all variables to default values
    edep = 0.;
  }

  // always fill the primary tree
  primarytree->Fill();
}

void HistoManager::SetOutputPath(const G4String path)
{
  outputPath = path;
}

void HistoManager::SetOutputName(const G4String name)
{
  outputName = name;
}
