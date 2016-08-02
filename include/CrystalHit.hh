 //
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: HodoscopeHit.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file HodoscopeHit.cc
/// \brief Implementation of the HodoscopeHit class
// 
// ********************************************************************
// * CrystalHit.hh                                                    *
// *                                                                  *
// * Header of Hit class for Crystal                                  *
// *                                                                  *
// * Created by: Alexander Leder                                      *
// * aleder@mit.edu                                                   *
// * 23 March 2016                                                    *
// * Updated: 23 March 2016 by Alexander Leder (aleder@mit.edu)       *
// ********************************************************************

#ifndef CrystalHit_h
#define CrystalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// Crystal hit
///
/// It records:
/// - Edep
/// - the particle time

class CrystalHit: public G4VHit
{
public:
    CrystalHit();
    ~CrystalHit();
    CrystalHit(const CrystalHit&);

    inline void* operator new(size_t);
    inline void  operator delete(void*aHit);   

    void Draw();
    void Print();   
    void SetEdep(G4double de)                   { edep = de; };
    void SetPos(G4ThreeVector xyz)              { pos = xyz; };     

private:
    G4ThreeVector   pos;
    G4double        edep;
};


typedef G4THitsCollection<CrystalHit> CrystalHitsCollection;

extern G4Allocator<CrystalHit> CrystalHitAllocator;    

inline void* CrystalHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) CrystalHitAllocator.MallocSingle();
    return aHit;
}

inline void CrystalHit::operator delete(void *aHit)
{
    CrystalHitAllocator.FreeSingle((CrystalHit*) aHit);
}

#endif
    