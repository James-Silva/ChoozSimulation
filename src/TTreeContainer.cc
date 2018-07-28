// Created by James Silva on 7/26/18

#include "TTreeContainer.hh"

namespace primarygentools {

TTreeContainer::TTreeContainer() {
  gen.seed(std::random_device()()); // Seed the random generator
}

// Open the given file and tree and save them to the global file and tree
// variables. The branch addresses for the px, py, and pz member variables
// are set from the values of the branches in the ttree.
void TTreeContainer::setPxPyPzBranchAddress(const char* fileName,
                                            const char* treeName) {
  file = new TFile(fileName);
  if (!file || file->IsZombie()) {
    std::cerr <<'\n'<< fileName << " is not found or accessible" <<'\n'<< std::endl;
    // Program should not continue. It will be difficult to see where the
    // problem occured unless the program is stopped.
    std::exit(-1);
  }
  if (!file->GetListOfKeys()->Contains(treeName)) {
    std::cerr <<'\n'<< treeName << " is not found in " << fileName <<'\n'<< std::endl;
    std::exit(-1);
  }
  tree = dynamic_cast<TTree*>(file->Get(treeName));

  // The names of the momentum branches in the TTree must match
  // "px","py", and "pz" exactly
  tree->SetBranchAddress("px", &momentumAddress.px);
  tree->SetBranchAddress("py", &momentumAddress.py);
  tree->SetBranchAddress("pz", &momentumAddress.pz);
}

G4ThreeVector TTreeContainer::getMomentumVec() {
  if (!tree) {
    std::cerr << "\nTTree is not found or was not created properly\n"<< std::endl;
    std::exit(-1);
  }
  // Generates and int between 0 and the largest entry in the TTree
  std::uniform_int_distribution<int> dist(0, tree->GetEntries()-1);
  tree->GetEntry(dist(gen)); // Set px,py,and pz in momentumAddress
  // Check if Branch addresses have been set correctly
  if (momentumAddress.px == -1 || momentumAddress.py == -1 ||
      momentumAddress.pz == -1) {
    std::cerr << "\nBranch addresses were not set.\n" << std::endl;
    std::exit(-1);
  }
  return {momentumAddress.px, momentumAddress.py, momentumAddress.pz};
}

} // namespace primarygentools
