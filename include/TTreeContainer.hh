// Created by James Silva on 7/26/18

#ifndef TTreeContainer_h
#define TTreeContainer_h 1

#include <TFile.h>
#include <TTree.h>
#include "TRandom3.h"
#include <random>
#include "G4ThreeVector.hh"

namespace primarygentools {

class TTreeContainer {
  public:
    //
    TTreeContainer();
    G4ThreeVector getMomentumVec();

  private:
    std::mt19937 gen;
    TRandom3 randNum;
    TFile*   file;
    TTree*   tree;
    Float_t px, py, pz;
};

} // namespace primarygentools

#endif // TTREECONTAINER_H
