// ********************************************************************
// * GPSPrimaryGeneratorAction.hh                                     *
// *                                                                  *
// * User class implementing the geant4 general particle source as    *
// * the particle generator for this simulation.					  *
// *                                                                  *
// * Created by: Adam Anderson                                        *
// * adama@mit.edu                                                    *
// * 7 April 2012                                                     *
// ********************************************************************

#ifndef GPSPrimaryGeneratorAction_h
#define GPSPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"


class GPSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	GPSPrimaryGeneratorAction();    
	virtual ~GPSPrimaryGeneratorAction();

	void GeneratePrimaries(G4Event*);

private:
	G4GeneralParticleSource* gpsParticleSource;
};

#endif