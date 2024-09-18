#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1


#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4Timer.hh"
#include "Randomize.hh" 
#include "G4RunManager.hh"

#include "G4ParticleDefinition.hh"

class PrimaryGeneratorActionMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();
	void GeneratePrimaries(G4Event *anEvent);

	inline void SetNOfParticle(G4double val){nOfParticle = val;}
	inline void SetGunEnergy(G4double val){energy = val;}
	inline void SetEnergyStd(G4double val){std = val;}
	inline void SetGunRadius(G4double val){GunRadius = val;}
	inline void SetGunXPosition(G4double val){x0 = val;}
	inline void SetGunYPosition(G4double val){y0 = val;}
	inline void SetGunZPosition(G4double val){z0 = val;}
	inline void SetMinSolidAngle(G4double val){minAlpha = val;}
	inline void SetMaxSolidAngle(G4double val){maxAlpha = val;}
	inline void SetThetaRotationAngle(G4double val){theta = val;}
	inline void SetPhiRotationAngle(G4double val){phi = val;}

	inline G4double GetGunEnergy() const {return energy;}

	const G4ParticleGun* GetParticleGun() const {return particleGun;}

private:
	G4double nOfParticle;
	G4double energy, std, GunRadius;
	G4double x0, y0, z0;
	G4ThreeVector dir, pos;
	G4double theta, phi;
	G4double minAlpha, maxAlpha;

	PrimaryGeneratorActionMessenger* fPrimaryGeneratorActionMessenger;
	G4ParticleGun* particleGun = nullptr;
	
};

#endif
