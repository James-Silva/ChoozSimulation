// Created by James Silva on 7/26/18

#ifndef TTreeContainer_h
#define TTreeContainer_h 1

#include <TFile.h>
#include <TTree.h>
#include <random>
#include "G4ThreeVector.hh"

namespace primarygentools {

struct MomentumAddress {
  Float_t px = -1, py = -1, pz = -1;
};

class TTreeContainer {
  public:
    // Seeds the random number generator.
    TTreeContainer();

    // Set the member variables px,py,pz to the addresses from branches
    // of the given
    void setPxPyPzBranchAddress(const char* fileName, const char* treeName);

    // Randomly generates an index of the given TTree and returns
    // the px, py, pz values at that point.
    G4ThreeVector getMomentumVec(); 

  private:
    std::mt19937 gen;
    TFile*   file;
    TTree*   tree;
    MomentumAddress momentumAddress;
};

} // namespace primarygentools

#endif // TTREECONTAINER_H
