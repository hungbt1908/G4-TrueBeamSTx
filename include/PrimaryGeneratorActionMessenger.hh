#ifndef PrimaryGeneratorActionMessenger_h
#define MPrimaryGeneratorActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UImessenger;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class PrimaryGeneratorActionMessenger : public G4UImessenger 
{
public:
	PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* pPrimaryGeneratorAction);
	~PrimaryGeneratorActionMessenger(void);
	void SetNewValue(G4UIcommand* cmd, G4String newValue);

private:
	PrimaryGeneratorAction* fPrimaryGeneratorAction;
	G4UIdirectory* BeamDIR;
	G4UIcmdWithAnInteger* SetNOfParticleCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamEnergyCmd;
	G4UIcmdWithADoubleAndUnit* SetEnergyStdCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamRadiusCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamXPosCmd, * SetBeamYPosCmd, * SetBeamZPosCmd;
	G4UIcmdWithADoubleAndUnit* SetMinSolidAngleCmd, * SetMaxSolidAngleCmd;
	G4UIcmdWithADoubleAndUnit* SetPhiRotationAngleCmd, * SetThetaRotationAngleCmd;

};

#endif

