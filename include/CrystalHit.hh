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
    inline void  operator delete(void*);

    void Draw();
    void Print();
    void SetEvtNum(G4int evnum)                 { evtNum = evnum; };
    void SetEdep(G4double de)                   { edep = de; };
    void SetEdepReco(G4double edepR)            { edepReco = edepR; };
    void SetEdepRecoNoSpaceCharge(G4double de)  { edepRecoNoSpaceCharge = de; };
    void SetPos(G4ThreeVector xyz)              { pos = xyz; };
    void SetPDGID(G4int pid)                    { PDGID = pid; };
    void SetProcessID(G4String PID)             { processID = PID; };
    void SetVolume(G4String vol)                { volume = vol; };
    void SetTrackID(G4int tckID)                { trackID = tckID; };
    void SetStepNum(G4int sNum)                 { stepNum = sNum; };
    void SetParentID(G4int parID)               { parentID = parID; };
    void SetPosition0(G4ThreeVector pos0)       { position0 = pos0; };
    void SetPositionf(G4ThreeVector posf)       { positionf = posf; };
    void SetMomentum(G4ThreeVector mom)         { momentum = mom; };
    void SetKE(G4double ke)                     { kineticEnergy = ke; };
    void SetVertexKE(G4double vertexKE)         { vertexKineticEnergy = vertexKE; };
    void SetSecondaryPDGID(std::vector<G4int> idvec) { PDGIDvec = idvec; };
    void SetSecondaryTrackID(std::vector<G4int> trkvec) { trackvec = trkvec; };
    void SetTrackLength(G4double len)           { trackLength = len; };
    void SetChargeDriftTime(G4double dt)        { chargeDriftTime = dt; }
    void SetStepTime(G4double t)                { stepTime = t; };

    G4int           GetEvtNum()                 { return evtNum; };
    G4double        GetEdep()                   { return edep; };
    G4double        GetEdepReco()               { return edepReco; };
    G4double        GetEdepRecoNoSpaceCharge()  { return edepRecoNoSpaceCharge; };
    G4ThreeVector   GetPos()                    { return pos; };
    G4int           GetPDGID()                  { return PDGID; };
    G4String        GetProcessID()              { return processID; };
    G4String        GetVolume()                 { return volume; };
    G4int           GetTrackID()                { return trackID; };
    G4int           GetStepNum()                { return stepNum; };
    G4int           GetParentID()               { return parentID; };
    G4ThreeVector   GetPosition0()              { return position0;};
    G4ThreeVector   GetPositionf()              { return positionf;};
    G4ThreeVector   GetMomentum()               { return momentum; };
    G4double        GetKE()                     { return kineticEnergy; };
    G4double        GetVertexKE()               { return vertexKineticEnergy; };
    std::vector<G4int> GetSecondaryPDGID()      { return PDGIDvec; };
    std::vector<G4int> GetSecondaryTrackID()    { return trackvec; };
    G4double        GetTrackLength()            { return trackLength; };
    G4double        GetChargeDriftTime()        { return chargeDriftTime; };
    G4double        GetStepTime()               { return stepTime; };


private:
    G4double        evtNum;
    G4double        edep;
    G4double        edepReco;
    G4double        edepRecoNoSpaceCharge;
    G4ThreeVector   pos;
    G4int           PDGID;
    G4String        processID;
    G4String        volume;
    G4int           trackID;
    G4int           stepNum;
    G4int           parentID;
    G4ThreeVector   position0;
    G4ThreeVector   positionf;
    G4ThreeVector   vertexPosition;
    G4ThreeVector   momentum;
    G4double        kineticEnergy;
    G4double        vertexKineticEnergy;
    std::vector<G4int>  PDGIDvec;
    std::vector<G4int>  trackvec;
    G4double        trackLength;
    G4double        chargeDriftTime;
    G4double        stepTime;

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
    