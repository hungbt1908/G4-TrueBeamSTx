#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Material;
class G4VisAttributes;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();
	
	virtual G4VPhysicalVolume* Construct();
	void ConstructSDandField();
	void DefineMaterial();
	G4VPhysicalVolume* ConstructVolume();
	void ConstructAcc();
	void ConstructTarget();
	void ConstructPrimaryCollimator();
	void ConstructVacuumWindow();
	void ConstructIonizationChamber();
	void ConstructMirror();
	void ConstructJaw1X();
    void ConstructJaw2X();
    void ConstructJaw1Y();
    void ConstructJaw2Y();
	void ConstructWaterPhantom();

    inline void SetJaw1XAperture(G4double val){jaw1XAperture = val;}
    inline void SetJaw2XAperture(G4double val){jaw2XAperture = val;}
    inline void SetJaw1YAperture(G4double val){jaw1YAperture = val;}
    inline void SetJaw2YAperture(G4double val){jaw2YAperture = val;}
    inline G4double GetJaw1XAperture(){return jaw1XAperture;}
    inline G4double GetJaw2XAperture(){return jaw2XAperture;}
    inline G4double GetJaw1YAperture(){return jaw1YAperture;}
    inline G4double GetJaw2YAperture(){return jaw2YAperture;}
	inline void SetIsocenter(G4double val){isoCentre = val;}
    inline G4double GetIsocenter(){return isoCentre;}
	void ConstructBasePlate();
    void ConstructMylar();
	inline void SetPhantomXDim(G4double val){phantomXDim = val;}
    inline void SetPhantomYDim(G4double val){phantomYDim = val;}
    inline void SetPhantomZDim(G4double val){phantomZDim = val;}
    inline G4double GetPhantomXDim(){return phantomXDim;}
    inline G4double GetPhantomYDim(){return phantomYDim;}
    inline G4double GetPhantomZDim(){return phantomZDim;}
	inline void SetSSD(G4double val){ssd = val;}
	inline G4double GetSSD(){return ssd;}
	inline void SetNumberOfVoxelsAlongX(G4int val){numberOfVoxelsAlongX = val;}
    inline void SetNumberOfVoxelsAlongY(G4int val){numberOfVoxelsAlongY = val;}
    inline void SetNumberOfVoxelsAlongZ(G4int val){numberOfVoxelsAlongZ = val;}
    inline G4int GetNumberOfVoxelsAlongX(){return numberOfVoxelsAlongX;}
    inline G4int GetNumberOfVoxelsAlongY(){return numberOfVoxelsAlongY;}
    inline G4int GetNumberOfVoxelsAlongZ(){return numberOfVoxelsAlongZ;}

private:
	G4bool checkOverlap = false;
    G4Material* Air, * Copper, * Tungsten, * Vacuum, * Lead, * Berylium, * Kapton, * Mylar, * Water, * Steel;
	G4VisAttributes* worldAtt, * copperAtt, * tungstenAtt, * Tungsten_Color;

	G4Box* solidWorld;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
	G4double isoCentre, jaw1XAperture, jaw2XAperture, jaw1YAperture, jaw2YAperture;
	G4double ssd;
    G4double phantomXDim, phantomYDim, phantomZDim;
    G4int numberOfVoxelsAlongX, numberOfVoxelsAlongY, numberOfVoxelsAlongZ;
    G4Box* RODetectorXDivision, * RODetectorYDivision, * RODetectorZDivision;
	G4LogicalVolume* RODetectorXDivisionLog, * RODetectorYDivisionLog, * RODetectorZDivisionLog;
	G4VPhysicalVolume* RODetectorXDivisionPhys, * RODetectorYDivisionPhys, * RODetectorZDivisionPhys;
	DetectorMessenger* fDetMessenger;
};

#endif
