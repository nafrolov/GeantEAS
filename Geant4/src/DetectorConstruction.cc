#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
//#include "G4NeutronHPThermalScatteringNames.hh"
#include "G4SystemOfUnits.hh"
#include "Globals.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

//---------------------------------------------MATERIALS-------------------------------------------------

  G4double density, fractionmass;
  G4int ncomp, natoms;

  G4Element* elO  = nist->FindOrBuildElement("O");
  G4Element* elC  = nist->FindOrBuildElement("C");
  //G4Element* elH  = new G4Element("TS_H_of_Polyethylene", "H", 1., 1.0079*g/mole);//
  G4Element* elH  = nist->FindOrBuildElement("H");

  G4Element* elSi = nist->FindOrBuildElement("Si");
  G4Element* elZn = nist->FindOrBuildElement("Zn");
  G4Element* elS = nist->FindOrBuildElement("S");
  G4Element* elB = nist->FindOrBuildElement("B");

  G4Material* ZnS=new G4Material("CinkSulfide",density=4.09*g/cm3,ncomp=2);
  ZnS->AddElement(elZn, natoms=1);
  ZnS->AddElement(elS, natoms=1);

  G4Material* B2O3=new G4Material("BoronOxid",density=1.85*g/cm3,ncomp=2);
  B2O3->AddElement(elB, natoms=2);
  B2O3->AddElement(elO, natoms=3);

  G4Material* Silicon = new G4Material("Silicon", density=1*g/cm3, ncomp=4);
  Silicon->AddElement(elSi, natoms=1);
  Silicon->AddElement(elO, natoms=1);
  Silicon->AddElement(elC, natoms=2);
  Silicon->AddElement(elH, natoms=6);

  G4Material* LRB2 = new G4Material("BoronSc", density=3.3*g/cm3, ncomp=2);
  LRB2->AddMaterial(ZnS, fractionmass=65.0*perCent);
  LRB2->AddMaterial(B2O3, fractionmass=35.0*perCent);

  G4Material* Air = nist->BuildMaterialWithNewDensity("Air_YBJ","G4_AIR",0.8*mg/cm3);
  G4Material* Concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
  G4Material* Dry_Soil_Haizi = nist->BuildMaterialWithNewDensity("Soil_YBJ","G4_CONCRETE",1.8*g/cm3);
  G4Material* PE = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");

  G4Material* Wet_Soil_Haizi = new G4Material("GND with Water", \
             density=(Dry_Soil_Haizi->GetDensity() + waterfraction*0.02*g/cm3), ncomp=2);
  Wet_Soil_Haizi->AddMaterial(Dry_Soil_Haizi, fractionmass=(100 - waterfraction)*perCent);
  Wet_Soil_Haizi->AddMaterial(Water, fractionmass=waterfraction*perCent);


  G4Material* scintillator = LRB2;

  //-------------------------------SIZES-------------------------------------------------------

  G4double world_sizeX = 500*m;
  G4double world_sizeY = 500*m;
  G4double world_sizeZ = 1500*m;
  G4double soil_thickness = 10*m;
  G4double SciThick = 1*mm; // ---------------!!!!!-------------unreal thickness, real is 0.15 for LRB2
  G4double BrlSize_Z = 70*cm;
  G4double BrlSize_XY = 80*cm; //-----------------!!!!!----------unrealsize, real is 80 cm
  G4double BrlThick = PEthick*mm;
  G4double SiliconThick = 3*mm;
  G4double ScintSize_XY = 69*cm; //---------------!!!!!----------unrealsize, real is 69 cm
  G4double dy=0*m;
  G4double dx=0*m;
  G4double dz=(0.05*m+0.5*BrlSize_Z+BrlThick);
  G4ThreeVector D=G4ThreeVector(dx,dy,dz);

  G4bool checkOverlaps = true;

  //-------------------------------VOLUMES-----------------------------------------------------

  G4Box *solidWorld = new G4Box("World", world_sizeX, world_sizeY, world_sizeZ);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, Air, "World");
  G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);

  G4Box *solidFloor = new G4Box("Ground", world_sizeX, world_sizeY, soil_thickness/2);
  G4LogicalVolume *logicFloor = new G4LogicalVolume(solidFloor, Wet_Soil_Haizi,	"Ground");
  new G4PVPlacement(nullptr,	G4ThreeVector(0*m, 0*m, -soil_thickness/2), logicFloor, "Ground", logicWorld, false, 0, checkOverlaps);

  G4Box *solidHole = new G4Box("Hole", 0.5*m, 0.5*m, 0.5*m);
  G4LogicalVolume *logicHole = new G4LogicalVolume(solidHole, Air,	"Hole");
  new G4PVPlacement(nullptr,	G4ThreeVector(0*m, 0*m, soil_thickness/2 - 0.5*m), logicHole, "Hole", logicFloor, false, 0, checkOverlaps);


  G4Tubs* solidDec;
  G4LogicalVolume* logicDec;

  G4Tubs* solidDecAir;
  G4LogicalVolume* logicDecAir;

  G4Tubs* solidDet;
  G4LogicalVolume* logicDet;

  G4Tubs* solidSil;
  G4LogicalVolume* logicSil;

  solidDec = new G4Tubs("Brl", 0, (0.5*BrlSize_XY+BrlThick), (0.5*BrlSize_Z+BrlThick), 0., 2*M_PI*rad); //solid
  logicDec = new G4LogicalVolume(solidDec, PE, "Brl");
  new G4PVPlacement(nullptr, D, logicDec, "Brl", logicWorld, false, 0, checkOverlaps);

  solidDecAir = new G4Tubs("Air", 0, (0.5*BrlSize_XY), (0.5*BrlSize_Z), 0., 2*M_PI*rad); //solid
  logicDecAir = new G4LogicalVolume(solidDecAir, Air, "Air");
  new G4PVPlacement(nullptr, G4ThreeVector(0*m, 0*m, 0*m), logicDecAir, "Air", logicDec, false, 0, checkOverlaps);

  solidSil = new G4Tubs("Sil", 0, (0.5*ScintSize_XY), 0.5*SiliconThick, 0., 2*M_PI*rad); //solid
  logicSil = new G4LogicalVolume(solidSil, Silicon, "Sil");
  new G4PVPlacement(nullptr, G4ThreeVector(0*m, 0*m, -0.5*BrlSize_Z + 2*cm), logicSil, "Sil", logicDecAir, false, 0, checkOverlaps);

  solidDet = new G4Tubs("Detector", 0, (0.5*ScintSize_XY), 0.5*SciThick, 0., 2*M_PI*rad); //solid
  logicDet = new G4LogicalVolume(solidDet, scintillator, "Detector");
  new G4PVPlacement(nullptr, G4ThreeVector(0*m, 0*m, 0*cm), logicDet, "Detector", logicSil, false, 0, checkOverlaps);

  return physWorld;
}
