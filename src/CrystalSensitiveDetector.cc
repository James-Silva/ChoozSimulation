// ********************************************************************
// * CrystalSensitiveDetector.cc                                      *
// *                                                                  *
// * Class for making the Crystal volume sensitive                    *
// *                                                                  *
// * Created by: Alexander Leder                                      *
// * adama@mit.edu                                                    *
// * 23 March 2016                                                    *
// ********************************************************************

#include "CrystalSensitiveDetector.hh"
#include "CrystalHit.hh"
#include "G4VProcess.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"

CrystalSensitiveDetector::CrystalSensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname="CrystalHitsCollection");
}

CrystalSensitiveDetector::~CrystalSensitiveDetector(){ }

void CrystalSensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
	trackerCollection = new CrystalHitsCollection
	                      (SensitiveDetectorName,collectionName[0]);
	static G4int HCID = -1;
	if(HCID<0)
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	HCE->AddHitsCollection( HCID, trackerCollection );
}

G4bool CrystalSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	G4double edep = aStep->GetTotalEnergyDeposit();
	G4StepPoint* postPoint = aStep->GetPostStepPoint();
	CrystalHit* newHit = new CrystalHit();
	newHit->SetEdep(edep);
	newHit->SetPos(postPoint->GetPosition());

	trackerCollection->insert( newHit );

	return true;
}

void CrystalSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}