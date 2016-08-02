// ********************************************************************
// * CrystalSensitiveDetector.hh                                      *
// *                                                                  *
// * Class for making the Crystal volume sensitive                    *
// *                                                                  *
// * Created by: Adam Anderson                                        *
// * adama@mit.edu                                                    *
// * 21 October 2012                                                  *
// ********************************************************************

#include "G4VSensitiveDetector.hh"
#include "CrystalHit.hh"

class G4Step;
class G4HCofThisEvent;

class CrystalSensitiveDetector: public G4VSensitiveDetector
{
public:
	CrystalSensitiveDetector(G4String);
	~CrystalSensitiveDetector();

	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);

private:
	CrystalHitsCollection* trackerCollection;
};