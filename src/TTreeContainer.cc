
#include "TTreeContainer.hh"

namespace primarygentools {

TTreeContainer::TTreeContainer() {
  // Set the TFile and TTree
  file = new TFile("DC_MUSIC_sim_ND_Full_Info.root");
  if (!file || file->IsZombie()) {
      return;
   }
  tree = (TTree*)file->Get("MUSIC_OUTPUT");
  // Set the branch addresses
  tree->SetBranchAddress("px", &px);
  tree->SetBranchAddress("py", &py);
  tree->SetBranchAddress("pz", &pz);

  // Seed the random generator
  gen.seed(std::random_device()());
}

G4ThreeVector TTreeContainer::getMomentumVec() {
  // NOTE: Check if tree is alive
  std::uniform_int_distribution<int> dist(0, tree->GetEntries()-1);
  tree->GetEntry(dist(gen));
  //Check if the entry is good
  return {px,py,pz};
}

}
