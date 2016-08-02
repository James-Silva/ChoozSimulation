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
// * CrystalHit.cc                                                    *
// *                                                                  *
// * Definition of Hit class for Crystal                              *
// *                                                                  *
// * Created by: Alexander Leder                                      *
// * aleder@mit.edu                                                   *
// * 23 March 2016                                                    *
// * Updated: 23 March 2016 by Alexander Leder (aleder@mit.edu)       *
// ********************************************************************

#include "CrystalHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


G4Allocator<CrystalHit> CrystalHitAllocator;

CrystalHit::CrystalHit() {}

CrystalHit::~CrystalHit() {}

CrystalHit::CrystalHit(const CrystalHit& right)
  : G4VHit()
{
    edep      = right.edep;
    pos       = right.pos;
}

void CrystalHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(pos);
        circle.SetScreenSize(2.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

void CrystalHit::Print()
{
    G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
    G4cout << "  energy deposit: " << G4BestUnit(edep,"Energy")
           << "  position: " << G4BestUnit(pos,"Length") << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Constructor

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Print 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
