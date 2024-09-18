#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorActionMessenger.hh"

#include "G4ParticleDefinition.hh"

using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	fPrimaryGeneratorActionMessenger = new PrimaryGeneratorActionMessenger(this);
	particleGun = new G4ParticleGun();
	particleGun->SetNumberOfParticles(1);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete particleGun;
}
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
    particleGun->SetParticleDefinition(particle);

	//
	G4double ek;
	ek = RandGauss::shoot(energy, std);
	particleGun->SetParticleEnergy(ek);

	// position
	G4double posX, posY, posZ;
    G4double a = 2.*pi*G4UniformRand(); //angle
    G4double rc = GunRadius;            //radius
    G4double r = (rc*std::sqrt(G4UniformRand()));
    posX = r*std::cos(a) + x0;
    posY = r*std::sin(a) + y0;
	posZ = z0;
    particleGun->SetParticlePosition(G4ThreeVector(posX, posY, posZ));

	// direction
	G4double fCosAlphaMin = std::cos(minAlpha);
  	G4double fCosAlphaMax = std::cos(maxAlpha);
	G4double cosAlpha = fCosAlphaMin-G4UniformRand()*(fCosAlphaMin-fCosAlphaMax);
  	G4double sinAlpha = std::sqrt(1. - cosAlpha*cosAlpha);
	G4double psi = twopi*G4UniformRand();  //psi uniform in (0, 2*pi)  
	G4ThreeVector dir(sinAlpha*std::cos(psi), sinAlpha*std::sin(psi), cosAlpha);

  	G4ThreeVector fNewUz = G4ThreeVector(std::sin(theta)*std::cos(phi),
                                         std::sin(theta)*std::sin(phi),
                                         std::cos(theta));
	dir.rotateUz(fNewUz); // rotate around fNewUz
	particleGun->SetParticleMomentumDirection(dir);

	//
	particleGun->GeneratePrimaryVertex(anEvent);
}
