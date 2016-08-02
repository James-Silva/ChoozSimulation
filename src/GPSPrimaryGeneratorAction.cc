// ********************************************************************
// * GPSPrimaryGeneratorAction.cc                                     *
// *                                                                  *
// * User class implementing the geant4 general particle source as    *
// * the particle generator for this simulation.					  *
// *                                                                  *
// * Created by: Adam Anderson                                        *
// * adama@mit.edu                                                    *
// * 7 April 2012                                                     *
// ********************************************************************

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "GPSPrimaryGeneratorAction.hh"

GPSPrimaryGeneratorAction::GPSPrimaryGeneratorAction()
{
	gpsParticleSource = new G4GeneralParticleSource();
}


GPSPrimaryGeneratorAction::~GPSPrimaryGeneratorAction()
{
	delete gpsParticleSource;
}


void GPSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	gpsParticleSource->GeneratePrimaryVertex(anEvent);
}