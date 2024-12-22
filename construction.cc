#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
G4NistManager* nist = G4NistManager::Instance();



// Define Elements
G4Element* Lu = new G4Element("Lutetium", "Lu", 71., 174.967*g/mole);
G4Element* Y  = new G4Element("Yttrium",  "Y",  39., 88.9059*g/mole);
G4Element* Si = new G4Element("Silicon",  "Si", 14., 28.0855*g/mole);
G4Element* O  = new G4Element("Oxygen",   "O",   8., 16.00*g/mole);

// Define LYSO Material
G4Material* LYSO = new G4Material("LYSO", 7.1*g/cm3, 4); // density = 7.1 g/cm³, 4 components

LYSO->AddElement(Lu, 0.714);  // Lutetium: 71.4%
LYSO->AddElement(Y,  0.040);  // Yttrium:  4.0%
LYSO->AddElement(Si, 0.064);  // Silicon:  6.4%
LYSO->AddElement(O,  0.182);  // Oxygen:  18.2%



G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};  // energies should be written in increasing order
G4double rindexLYSO[2]= {1.82, 1.82};
G4double rindexWorld[2]= {1.0, 1.0};

G4MaterialPropertiesTable *mptLYSO = new G4MaterialPropertiesTable();
mptLYSO->AddProperty("RINDEX", energy, rindexLYSO, 2);

LYSO->SetMaterialPropertiesTable(mptLYSO);


G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

worldMat->SetMaterialPropertiesTable(mptWorld);


G4Box* solidWorld = new G4Box("solidWorld", 0.5 * m, 0.5 * m, 0.5 * m);


G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

G4Box *solidRadiator = new G4Box("solidRadiator", 0.4*m, 0.4*m, 0.01*m);

G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, LYSO, "logicalRadiator");

G4VPhysicalVolume *physRadiator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);

G4Box *solidDetector = new G4Box("solidDetector", 0.005*m, 0.005*m, 0.01*m);

logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicalDetector");

for(G4int i = 0; i < 100; i++)
{
for(G4int j =0; j < 100; j++)
{
G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5*m+(i+0.5)*m/100,-0.5*m+(j+0.5)*m/100, 0.49*m), logicDetector, "physDetector", logicWorld, false, j+i*100, true);
}

}

return physWorld;

}

void MyDetectorConstruction::ConstructSDandField()
{
MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

logicDetector->SetSensitiveDetector(sensDet);
}


