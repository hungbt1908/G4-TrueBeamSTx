#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4SystemOfUnits.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4NistManager.hh"
#include "G4VisAttributes.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
	fDetMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
	delete fDetMessenger;
}

G4VPhysicalVolume *DetectorConstruction :: Construct()
{
	// clear old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	DefineMaterial();
	return ConstructVolume();
}

void DetectorConstruction::DefineMaterial()
{
	// NIST
	G4NistManager* nist = G4NistManager::Instance();

	// Air
	Air = nist->FindOrBuildMaterial("G4_AIR");

	// Copper
	Copper = nist->FindOrBuildMaterial("G4_Cu");
	copperAtt = new G4VisAttributes(G4Colour(0.5, 0.1, 0.1, 0.1));
	copperAtt->SetVisibility(true);
	copperAtt->SetForceSolid(true);

	// Tungsten
	Tungsten = nist->FindOrBuildMaterial("G4_W");
	tungstenAtt = new G4VisAttributes(G4Colour(1., 0.3, 0.3));
	tungstenAtt->SetVisibility(true);
	tungstenAtt->SetForceWireframe(true);
	tungstenAtt->SetForceAuxEdgeVisible(true);

	// 
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	//
	Lead = nist->FindOrBuildMaterial("G4_Pb");

	//
	Berylium = nist->FindOrBuildMaterial("G4_Be"); 

	//
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");

	//
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");

	//
	Water = nist->FindOrBuildMaterial("G4_WATER");

	//
	G4Element* elW = nist->FindOrBuildElement("W");
	G4Element* elNi = nist->FindOrBuildElement("Ni");
	G4Element* elFe = nist->FindOrBuildElement("Fe");
	Steel = new G4Material("Steel", 18*g/cm3, 3);
	Steel->AddElement(elW, 95*perCent);
	Steel->AddElement(elNi, 3.75*perCent);
	Steel->AddElement(elFe, 1.25*perCent);

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::ConstructVolume()
{

	// world volume
	solidWorld = new G4Box("solidWorld", 500 * cm, 500 * cm, 500 * cm);
	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlap);
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	ConstructAcc();
	ConstructWaterPhantom();
	return physWorld;
}

void DetectorConstruction::ConstructAcc()
{
	ConstructTarget();
	ConstructPrimaryCollimator();
	ConstructVacuumWindow();
	ConstructIonizationChamber();
	ConstructJaw1X();
	ConstructJaw2X();
	ConstructJaw1Y();
	ConstructJaw2Y();
	ConstructBasePlate();
	ConstructMylar();
}

void DetectorConstruction::ConstructTarget()
{
	G4Box *targetABox = new G4Box("targetABox", 5.*mm, 5.*mm, (.035*25.4/2.)*mm);
	G4LogicalVolume *targetALV = new G4LogicalVolume(targetABox, Tungsten, "targetALV", 0, 0, 0);
	targetALV->SetVisAttributes(tungstenAtt);

	G4Box *targetBBox = new G4Box("targetABox", 5.*mm, 5.*mm, (0.062*25.4/2.)*mm);
	G4LogicalVolume *targetBLV = new G4LogicalVolume(targetBBox, Copper, "targetBLV", 0, 0, 0);

	// specific translations for the various parts of the component
	new G4PVPlacement(
			0,
			G4ThreeVector(0.,0.,targetABox->GetZHalfLength()),
			"targetAPV",
			targetALV,
			physWorld,
			false,
			0);

	new G4PVPlacement(
			0,
			G4ThreeVector(0.,0.,targetABox->GetZHalfLength()*2.+targetBBox->GetZHalfLength()),
			"targetBPV",
			targetBLV,
			physWorld,
			false,
			0);
}

void DetectorConstruction::ConstructPrimaryCollimator()
{
	G4double totalHeight = 80.0*mm;

	// //-------------------- upper part----------------
	G4ThreeVector centre=G4ThreeVector(0.,0.,16.)-G4ThreeVector(0.,0.,(16. + totalHeight-76.)/2.);
	G4Tubs *PCUTube = new G4Tubs("PrimaryCollimatorUTube", 10.*mm, 40.0*mm, 10.*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *PCUTubeLV = new G4LogicalVolume(PCUTube, Lead, "PrimaryCollimatorUTubeLV", 0, 0, 0);

	new G4PVPlacement(0, centre, "PrimaryCollimatorUTubePV", PCUTubeLV, physWorld, false, 0);

	// //-------------------- lower part----------------
	// Tube
	G4Tubs* PCLTube = new G4Tubs("PrimaryCollimatorLTube", 0., 40.*mm, 60./2.*mm, 0.*deg, 360.*deg);
	// Cone
	G4double coneAperture = 14.*deg;
	G4Cons* collimCone = new G4Cons("PrimaryCollimatorLCone", 0., (16.*std::tan(coneAperture))*mm, 0., (76.*std::tan(coneAperture))*mm, 30.*mm,  0.*deg, 360.*deg);

	G4LogicalVolume* PCLTubeLV = new G4LogicalVolume(PCLTube, Lead, "PCLTubeLV",0,0,0);
	G4LogicalVolume* collimConeLV = new G4LogicalVolume(collimCone, Vacuum, "collimConeLV",0,0,0);
	centre = G4ThreeVector(0.,0.,16.+60./2.);
	G4VPhysicalVolume *PCLTubePV = new G4PVPlacement(0, centre, "PCLTubePV", PCLTubeLV, physWorld, false, 0);

	centre = G4ThreeVector(0.,0.,0.);
	new G4PVPlacement(0, centre, "TubeMinusConeLPV", collimConeLV, PCLTubePV, false, 0);
}

void DetectorConstruction::ConstructVacuumWindow()
{
	G4Tubs* BeWTube = new G4Tubs("BeWindowTube", 0., 50.*mm, 0.12*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *BeWTubeLV = new G4LogicalVolume(BeWTube, Berylium, "BeWTubeLV", 0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0.,0.,90.*mm), "BeWTubePV", BeWTubeLV, physWorld, false, 0);
}

void DetectorConstruction::ConstructIonizationChamber()
{
	G4Tubs* ICTubeW = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.005*25.4*mm, 0.*deg, 360.*deg);
	G4Tubs* ICTubeP = new G4Tubs("ionizationChamberTube", 0., 3.75*2.54*10.*mm, 0.002*25.4*mm, 0.*deg, 360.*deg);

	G4ThreeVector centre;
	// W1
	centre.set(0.,0.,148.35*mm);
	G4LogicalVolume *PCUTubeW1LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeW1PV", PCUTubeW1LV, physWorld, false, 0);

	// P1
	centre.set(0.,0.,150.73*mm);
	G4LogicalVolume *PCUTubeP1LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP1LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP1PV", PCUTubeP1LV, physWorld, false, 0);

	// W2
	centre.set(0.,0.,155.5*mm);
	G4LogicalVolume *PCUTubeW2LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeW2PV", PCUTubeW2LV, physWorld, false, 0);

	// P2
	centre.set(0.,0.,153.12*mm);
	G4LogicalVolume *PCUTubeP2LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP2LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP2PV", PCUTubeP2LV, physWorld, false, 0);

	// W3
	centre.set(0.,0.,162.65*mm);
	G4LogicalVolume *PCUTubeW3LV = new G4LogicalVolume(ICTubeW, Kapton, "ionizationChamberTubeW3LV", 0, 0, 0);
	//new G4PVPlacement(0, centre, "ionizationChamberTubeW3PV", PCUTubeW3LV, physWorld, false, 0);

	// P3
	centre.set(0.,0.,157.88*mm);
	G4LogicalVolume *PCUTubeP3LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP3LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP3PV", PCUTubeP3LV, physWorld, false, 0);

	// P4
	centre.set(0.,0.,160.27*mm);
	G4LogicalVolume *PCUTubeP4LV = new G4LogicalVolume(ICTubeP, Kapton, "ionizationChamberTubeP4LV", 0, 0, 0);
	new G4PVPlacement(0, centre, "ionizationChamberTubeP4PV", PCUTubeP4LV, physWorld, false, 0);	
}

void DetectorConstruction::ConstructMirror()
{
	G4Tubs* MirrorTube = new G4Tubs("MirrorTube", 0., 86.*mm, 1.*mm, 0.*deg, 360.*deg);
	G4LogicalVolume *MirrorTubeLV = new G4LogicalVolume(MirrorTube, Mylar, "MirrorTubeLV", 0, 0, 0);
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	cRotation -> rotateY(35.0*deg);
	new G4PVPlacement(cRotation, G4ThreeVector(0., 0., 220.*mm), "MirrorTubePV", MirrorTubeLV,physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw1X()
{	
	G4String name = "Jaws1X";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	centre.set(0.,0.,(367.+78./2.)*mm);
	halfSize.set(50.*mm, 90.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(1, centre, halfSize,cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw1XAperture/isoCentre));
	centre.set(z*sin(theta)+dx*cos(theta), y, z*cos(theta)-dx*sin(theta));
	cRotation->rotateY(-theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw2X()
{
	G4String name = "Jaws2X";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation = new G4RotationMatrix();
	centre.set(0.,0.,(367.+78./2.)*mm);
	halfSize.set(50.*mm, 90.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(2, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw2XAperture/isoCentre));
	centre.set(-(z*sin(theta)+dx*cos(theta)), y, z*cos(theta)-dx*sin(theta));
	cRotation->rotateY(theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw1Y()
{
	G4String name="Jaws1Y";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();
	centre.set(0.,0.,(280.+78./2.)*mm);
	halfSize.set(90.*mm, 50.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(3, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw1YAperture/isoCentre));
	centre.set(x, z*sin(theta)+dy*cos(theta), z*cos(theta)-dy*sin(theta));
	cRotation->rotateX(theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructJaw2Y()
{
	G4String name = "Jaws2Y";
	G4ThreeVector centre, halfSize;
	G4RotationMatrix *cRotation=new G4RotationMatrix();
	centre.set(0.,0.,(280.+78./2.)*mm);
	halfSize.set(90.*mm, 50.*mm, 78./2.*mm);
	G4Box *box = new G4Box(name+"Box", halfSize.getX(), halfSize.getY(), halfSize.getZ());
	G4LogicalVolume *logVol = new G4LogicalVolume(box, Tungsten, name+"LV", 0, 0, 0);
	//SetJawAperture(4, centre, halfSize, cRotation);
	G4double theta, x, y, z, dx, dy; //, dz, top;
	x=centre.getX();
	y=centre.getY();
	z=centre.getZ();
	dx=halfSize.getX();
	dy=halfSize.getY();
	theta=fabs(atan(jaw2YAperture/isoCentre));
	centre.set(x, -(z*sin(theta)+dy*cos(theta)), z*cos(theta)-dy*sin(theta));
	cRotation->rotateX(-theta);
	new G4PVPlacement(cRotation, centre, name+"PV", logVol, physWorld, false, 0);
}

void DetectorConstruction::ConstructBasePlate()
{
	G4Tubs* solidPlate = new G4Tubs("solidPlate", 115.57*mm, 302.26*mm, 15.24/2*mm, 0., 360*deg);
	G4LogicalVolume* logicPlate = new G4LogicalVolume(solidPlate, Steel, "logicPlate");
  	// G4VPhysicalVolume* physPlate =
	new G4PVPlacement(0, G4ThreeVector(0, 0, 459.38*mm), logicPlate, "physPlate", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::ConstructMylar()
{
	G4Box* solidMylar =  new G4Box("solidMylar", 50./2*cm, 50./2*cm, 0.1016/2*mm);
	G4LogicalVolume* logicMylar = new G4LogicalVolume(solidMylar, Air, "logicMylar");
	// G4VPhysicalVolume* physMylar =
	new G4PVPlacement(0, G4ThreeVector(0, 0, 557*mm), logicMylar, "physMylar", logicWorld, false, 0, checkOverlap);
}

void DetectorConstruction::ConstructWaterPhantom()
{
	G4ThreeVector centre, halfSize;
    G4ThreeVector fPhantomSize;   // Size of Water Phantom
	
	fPhantomSize.setX(phantomXDim);
  	fPhantomSize.setY(phantomYDim);
  	fPhantomSize.setZ(phantomZDim);
	halfSize.setX(fPhantomSize.getX()/2.);
	halfSize.setY(fPhantomSize.getY()/2.);
	halfSize.setZ(fPhantomSize.getZ()/2.);

	centre.set(0., 0., ssd+halfSize.getZ());

	G4Box *fullWaterPhantomBox = new G4Box("fullWaterPhantomBox", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 	G4LogicalVolume *fullWaterPhantomLV = new G4LogicalVolume(fullWaterPhantomBox, Water, "fullWaterPhantomLV", 0, 0, 0);
	G4VPhysicalVolume* physWaterPhantom = new G4PVPlacement(0, centre, "fullWaterPhantomPV", fullWaterPhantomLV, physWorld, false, 0);

	// Division along X axis
	G4double halfXVoxelSizeX = fPhantomSize.getX()/2./numberOfVoxelsAlongX;
	G4double halfXVoxelSizeY = fPhantomSize.getY()/2.;
	G4double halfXVoxelSizeZ = fPhantomSize.getZ()/2.;
	G4double voxelXThickness = 2*halfXVoxelSizeX;

	RODetectorXDivision = new G4Box("RODetectorXDivision", halfXVoxelSizeX, halfXVoxelSizeY, halfXVoxelSizeZ);
	RODetectorXDivisionLog = new G4LogicalVolume(RODetectorXDivision, Water, "RODetectorXDivisionLog", 0,0,0);
	RODetectorXDivisionPhys = new G4PVReplica("RODetectorXDivisionPhys", RODetectorXDivisionLog, physWaterPhantom, kXAxis, numberOfVoxelsAlongX, voxelXThickness);

	// Division along Y axis
	G4double halfYVoxelSizeX = halfXVoxelSizeX;
  	G4double halfYVoxelSizeY = fPhantomSize.getY()/2./numberOfVoxelsAlongY;
  	G4double halfYVoxelSizeZ = fPhantomSize.getZ()/2.;
  	G4double voxelYThickness = 2*halfYVoxelSizeY;

	RODetectorYDivision = new G4Box("RODetectorYDivision", halfYVoxelSizeX, halfYVoxelSizeY, halfYVoxelSizeZ);
  	RODetectorYDivisionLog = new G4LogicalVolume(RODetectorYDivision, Water, "RODetectorYDivisionLog", 0,0,0);
  	RODetectorYDivisionPhys = new G4PVReplica("RODetectorYDivisionPhys", RODetectorYDivisionLog, RODetectorXDivisionPhys, kYAxis, numberOfVoxelsAlongY, voxelYThickness);

	// Division along Z axis
	G4double halfZVoxelSizeX = halfXVoxelSizeX;
  	G4double halfZVoxelSizeY = halfYVoxelSizeY;
  	G4double halfZVoxelSizeZ = fPhantomSize.getZ()/2./numberOfVoxelsAlongZ;
  	G4double voxelZThickness = 2*halfZVoxelSizeZ;

	RODetectorZDivision = new G4Box("RODetectorZDivision",  halfZVoxelSizeX, halfZVoxelSizeY, halfZVoxelSizeZ);
  	RODetectorZDivisionLog = new G4LogicalVolume(RODetectorZDivision, Water, "RODetectorZDivisionLog", 0,0,0);
  	RODetectorZDivisionPhys = new G4PVReplica("RODetectorZDivisionPhys", RODetectorZDivisionLog, RODetectorYDivisionPhys, kZAxis, numberOfVoxelsAlongZ, voxelZThickness);
}

void DetectorConstruction::ConstructSDandField()
{}