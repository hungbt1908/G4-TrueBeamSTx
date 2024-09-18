#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* pDetectorConstruction)
    : G4UImessenger(), fDetectorConstruction(pDetectorConstruction)
{
    //
    linacDir = new G4UIdirectory("/LINAC/Geometry/");
    linacDir -> SetGuidance("Command to change LGK setups");

    //    
    setJaw1XApertureCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetJaw1XAperture", this);
    setJaw1XApertureCmd->SetGuidance("Set Jaw 1X aperture.");
    setJaw1XApertureCmd->SetParameterName("jaw1XAperture", false);
    setJaw1XApertureCmd->SetUnitCategory("Length");
	setJaw1XApertureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //    
    SetJaw2XApertureCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetJaw2XAperture", this);
    SetJaw2XApertureCmd->SetGuidance("Set Jaw 2X aperture.");
    SetJaw2XApertureCmd->SetParameterName("jaw2XAperture", false);
    SetJaw2XApertureCmd->SetUnitCategory("Length");
	SetJaw2XApertureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //    
    SetJaw1YApertureCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetJaw1YAperture", this);
    SetJaw1YApertureCmd->SetGuidance("Set Jaw 1Y aperture.");
    SetJaw1YApertureCmd->SetParameterName("jaw1YAperture", false);
    SetJaw1YApertureCmd->SetUnitCategory("Length");
	SetJaw1YApertureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //    
    SetJaw2YApertureCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetJaw2YAperture", this);
    SetJaw2YApertureCmd->SetGuidance("Set Jaw 2Y aperture.");
    SetJaw2YApertureCmd->SetParameterName("jaw2YAperture", false);
    SetJaw2YApertureCmd->SetUnitCategory("Length");
	SetJaw2YApertureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //  
    SetIsocenterCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetIsocenter", this);
    SetIsocenterCmd->SetGuidance("Set isocenter distance.");
    SetIsocenterCmd->SetParameterName("isocenter", false);
    SetIsocenterCmd->SetUnitCategory("Length");
	SetIsocenterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    // 
    SetSSDCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetSSD", this);
    SetSSDCmd->SetGuidance("Set SSD distance.");
    SetSSDCmd->SetParameterName("ssd", false);
    SetSSDCmd->SetUnitCategory("Length");
    SetSSDCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setPhantomXDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomXDim", this);
    setPhantomXDimCmd->SetGuidance("Set phantom dimension along X axis.");
    setPhantomXDimCmd->SetParameterName("phantomXDim", false);
    setPhantomXDimCmd->SetUnitCategory("Length");
	setPhantomXDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setPhantomYDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomYDim", this);
    setPhantomYDimCmd->SetGuidance("Set phantom dimension along Y axis.");
    setPhantomYDimCmd->SetParameterName("phantomYDim", false);
    setPhantomYDimCmd->SetUnitCategory("Length");
	setPhantomYDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setPhantomZDimCmd = new G4UIcmdWithADoubleAndUnit("/LINAC/Geometry/SetPhantomZDim", this);
    setPhantomZDimCmd->SetGuidance("Set phantom dimension along Z axis.");
    setPhantomZDimCmd->SetParameterName("phantomZDim", false);
    setPhantomZDimCmd->SetUnitCategory("Length");
	setPhantomZDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //
    setNumberOfVoxelsAlongXCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongX", this);
    setNumberOfVoxelsAlongXCmd->SetGuidance("Set number of voxels along X axis.");
    setNumberOfVoxelsAlongXCmd->SetParameterName("nOfVoxelsX", false);
    setNumberOfVoxelsAlongXCmd->AvailableForStates(G4State_Idle, G4State_PreInit);

    //
    setNumberOfVoxelsAlongYCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongY", this);
    setNumberOfVoxelsAlongYCmd->SetGuidance("Set number of voxels along Y axis.");
    setNumberOfVoxelsAlongYCmd->SetParameterName("nOfVoxelsY", false);
    setNumberOfVoxelsAlongYCmd->AvailableForStates(G4State_Idle, G4State_PreInit);

    //
    setNumberOfVoxelsAlongZCmd = new G4UIcmdWithAnInteger("/LINAC/Geometry/SetNumberOfVoxelsAlongZ", this);
    setNumberOfVoxelsAlongZCmd->SetGuidance("Set number of voxels along Z axis.");
    setNumberOfVoxelsAlongZCmd->SetParameterName("nOfVoxelsZ", false);
    setNumberOfVoxelsAlongZCmd->AvailableForStates(G4State_Idle, G4State_PreInit);
}

DetectorMessenger::~DetectorMessenger()
{
    delete linacDir;
    delete setJaw1XApertureCmd;
    delete SetJaw2XApertureCmd;
    delete SetJaw1YApertureCmd;
    delete SetJaw2YApertureCmd;
    delete SetIsocenterCmd;
    delete SetSSDCmd;
    delete setPhantomXDimCmd;
    delete setPhantomYDimCmd;
    delete setPhantomZDimCmd;
    delete setNumberOfVoxelsAlongXCmd;
    delete setNumberOfVoxelsAlongYCmd;
    delete setNumberOfVoxelsAlongZCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == setJaw1XApertureCmd)
    {
       fDetectorConstruction->SetJaw1XAperture(setJaw1XApertureCmd->GetNewDoubleValue(newValue));
    }

    if(command == SetJaw2XApertureCmd)
    {
       fDetectorConstruction->SetJaw2XAperture(SetJaw2XApertureCmd->GetNewDoubleValue(newValue));
    }

    if(command == SetJaw1YApertureCmd)
    {
       fDetectorConstruction->SetJaw1YAperture(SetJaw1YApertureCmd->GetNewDoubleValue(newValue));
    }

    if(command == SetJaw2YApertureCmd)
    {
       fDetectorConstruction->SetJaw2YAperture(SetJaw2YApertureCmd->GetNewDoubleValue(newValue));
    }

    if(command == SetIsocenterCmd)
    {
       fDetectorConstruction->SetIsocenter(SetIsocenterCmd->GetNewDoubleValue(newValue));
    }

    if(command == SetSSDCmd)
    {
        fDetectorConstruction->SetSSD(SetSSDCmd->GetNewDoubleValue(newValue));
    }

    if(command == setPhantomXDimCmd)
    {
       fDetectorConstruction->SetPhantomXDim(setPhantomXDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setPhantomYDimCmd)
    {
       fDetectorConstruction->SetPhantomYDim(setPhantomYDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setPhantomZDimCmd)
    {
       fDetectorConstruction->SetPhantomZDim(setPhantomZDimCmd->GetNewDoubleValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongXCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongX(setNumberOfVoxelsAlongXCmd->GetNewIntValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongYCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongY(setNumberOfVoxelsAlongYCmd->GetNewIntValue(newValue));
    }

    if(command == setNumberOfVoxelsAlongZCmd)
    {
        fDetectorConstruction->SetNumberOfVoxelsAlongZ(setNumberOfVoxelsAlongZCmd->GetNewIntValue(newValue));
    }

}



