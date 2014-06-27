#include "SpinICDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"

#include "G4NistManager.hh"

// Constructor
SpinICDetectorConstruction::SpinICDetectorConstruction()
: fCheckOverlaps(false)
//verbosity(0)
{ }

// Destructor
SpinICDetectorConstruction::~SpinICDetectorConstruction()
{ }

// Materials
void SpinICDetectorConstruction::DefineMaterials()
{
    // Get nist material manager
    G4NistManager* nistManager = G4NistManager::Instance();
    
    nistManager->FindOrBuildMaterial("G4_Galactic");
    nistManager->FindOrBuildMaterial("G4_Fe");
    nistManager->FindOrBuildMaterial("G4_Al");
    nistManager->FindOrBuildMaterial("G4_Ar");
	nistManager->FindOrBuildMaterial("G4_Pb");
    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* SpinICDetectorConstruction::Construct()
{
    // Materials
    DefineMaterials();
    

    // World    
    G4double worldSizeXY = 2.*m;
    G4double worldSizeZ = 40.*m;
    G4Material* worldMaterial = G4Material::GetMaterial("G4_Galactic");
    
    G4VSolid* solidWorld
    = new G4Box("World",                                     // its name
                worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size
    
    flogicalWorld
    = new G4LogicalVolume(
                          solidWorld,       // its solid
                          worldMaterial,    // its material
                          "World");         // its name
    
    G4VPhysicalVolume* physicalWorld
    = new G4PVPlacement(
                        0,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        flogicalWorld,    // its logical volume
                        "World",          // its name
                        0,                // its mother  volume
                        false,            // no boolean operation
                        0,                // copy number
                        fCheckOverlaps);  // checking overlaps
    
    // World Visualization attributes
    //
    G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    worldVisAtt->SetVisibility(true);
    worldVisAtt->SetForceWireframe(true);
    flogicalWorld->SetVisAttributes(worldVisAtt);

    
    // Define Everything else
    DefineMagnets();
    DefineIC();
    
    
    // Always return physical volume of world
    return physicalWorld;
}

void SpinICDetectorConstruction::DefineMagnets()
{
    //Magnet Materials
    G4Material* magnetIron = G4Material::GetMaterial("G4_Fe");
    G4Material* magnetVacuum = G4Material::GetMaterial("G4_Galactic");
    G4Material* magnetColl = G4Material::GetMaterial("G4_Pb");
	
	// ****** Magnets: 1 - 3 ******

    // Strength of field
    G4double stdfield = 0.22*tesla;

    // Magnet Geometry Parameters
    G4double magnetPlateThickness = 5.*cm;
    G4double magnetPlateSeparation = 10.*cm;
    G4double magnetXSize = 30.*cm;
    G4double magnetSmallZsize = 1.5*m;
    G4double magnetLargeZsize = magnetSmallZsize;
    
    // Magnet Positions
    G4double magnet1ZPos = -5*m - (magnetSmallZsize/2);
    G4double magnet2ZPos = magnet1ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet3ZPos = magnet2ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet4ZPos = magnet3ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet5ZPos = magnet4ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet6ZPos = magnet5ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet7ZPos = magnet6ZPos - (20.*cm) - magnetSmallZsize;
    G4double magnet8ZPos = magnet7ZPos - (20.*cm) - magnetSmallZsize;
    
    
    // ****** Small (with fields up in Y) Magnets - Magnets 1,2,7,8******
    
    // Define small magnet outer volume as large block of iron
    G4Box* smallMagnetOuter
    = new G4Box("s_smallMagnet_outer",              // its name
                magnetXSize/2,
                (magnetPlateThickness*2.+magnetPlateSeparation)/2,
                magnetSmallZsize/2);                // its size
    
    G4LogicalVolume* logicalSmallMagnetOuter
    = new G4LogicalVolume(smallMagnetOuter,         // its solid
                          magnetIron,               // its material
                          "l_smallMagnet_outer");   // its name

    // Define small magnet inner volume as block of vacuum
    G4Box* smallMagnetInner
    = new G4Box("s_smallMagnet_inner",              // its name
                magnetXSize/2,
                magnetPlateSeparation/2,
                magnetSmallZsize/2);                // its size
    
    
    G4LogicalVolume* logicalSmallMagnetInner
    = new G4LogicalVolume(smallMagnetInner,         // its solid
                          magnetVacuum,             // its material
                          "l_smallMagnet_inner");   // its name
    
    // Place Inner magnet volume as a daugter of outer magnet volume
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(),          // at (0,0,0) in outer volume
                      logicalSmallMagnetInner,  // its logical volume
                      "pv_smallMagnet_inner",   // its name
                      logicalSmallMagnetOuter,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

    // Place small magnet twice, this will include outer and inner    
    // 1st magnet

#if 1
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet1ZPos), // position
                      logicalSmallMagnetOuter,  // its logical volume
                      "pv_Magnet1",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

#endif
    
    // 2rd magnet

#if 1
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet2ZPos), // position
                      logicalSmallMagnetOuter,  // its logical volume
                      "pv_Magnet3",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
    
#endif
    // 7th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet7ZPos), // position
                      logicalSmallMagnetOuter,  // its logical volume
                      "pv_Magnet3",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
    
    // 8th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet8ZPos), // position
                      logicalSmallMagnetOuter,  // its logical volume
                      "pv_Magnet3",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
        


    // ****** Large (Field down in Y) Magnet - Magnet 3,4,5,6 ******
    
    // Define large magnet outer volume as large block of iron
    G4Box* largeMagnetOuter
    = new G4Box("s_largeMagnet_outer",              // its name
                magnetXSize/2,
                (magnetPlateThickness*2.+magnetPlateSeparation)/2,
                magnetLargeZsize/2);                // its size
    
    G4LogicalVolume* logicalLargeMagnetOuter
    = new G4LogicalVolume(largeMagnetOuter,         // its solid
                          magnetIron,               // its material
                          "l_largeMagnet_outer");   // its name
    

    // Define large magnet inner volume as block of vacuum
    G4Box* largeMagnetInner
    = new G4Box("s_largeMagnet_inner",              // its name
                magnetXSize/2,
                magnetPlateSeparation/2,
                magnetLargeZsize/2);                // its size
    
    
    G4LogicalVolume* logicalLargeMagnetInner
    = new G4LogicalVolume(largeMagnetInner,         // its solid
                          magnetVacuum,             // its material
                          "l_largeMagnet_inner");   // its name

    // Place Inner magnet volume as a daugter of outer magnet volume
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(),          // at (0,0,0) in outer volume
                      logicalLargeMagnetInner,  // its logical volume
                      "pv_largeMagnet_inner",   // its name
                      logicalLargeMagnetOuter,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
    
    // Place Large magnet, this will include inner and outer

#if 1
    // 3th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet3ZPos), // position
                      logicalLargeMagnetOuter,          // its logical volume
                      "pv_Magnet2",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
                      
#endif    
    // 4th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet4ZPos), // position
                      logicalLargeMagnetOuter,          // its logical volume
                      "pv_Magnet2",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
                      
                      
    // 5th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet5ZPos), // position
                      logicalLargeMagnetOuter,          // its logical volume
                      "pv_Magnet2",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
                      
                      
    // 6th magnet
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(0.,0.,magnet6ZPos), // position
                      logicalLargeMagnetOuter,          // its logical volume
                      "pv_Magnet2",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
                       
    // **** Magnetic Fields ****
    // Create magnetic fields and managers

    //Field in +Y
    G4UniformMagField* magFieldUp
    = new G4UniformMagField(G4ThreeVector(0.,stdfield,0.));
    G4FieldManager* localFieldManagerUp = new G4FieldManager();
    localFieldManagerUp->SetDetectorField(magFieldUp);
    localFieldManagerUp->CreateChordFinder(magFieldUp);
    // Assign to small magnets
    logicalSmallMagnetInner->SetFieldManager(localFieldManagerUp, true);

    //Field in -Y
    G4UniformMagField* magFieldDown
    = new G4UniformMagField(G4ThreeVector(0.,-1.*stdfield,0.));
    G4FieldManager* localFieldManagerDown = new G4FieldManager();
    localFieldManagerDown->SetDetectorField(magFieldDown);
    localFieldManagerDown->CreateChordFinder(magFieldDown);
    // Assign to large magnet
    logicalLargeMagnetInner->SetFieldManager(localFieldManagerDown, true);


    
    // Magnet Visualization attributes
    //
    G4VisAttributes* outerMagVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1., 0.5));
    outerMagVisAtt->SetVisibility(true);
    outerMagVisAtt->SetForceSolid(true);
    logicalLargeMagnetOuter->SetVisAttributes(outerMagVisAtt);
    logicalSmallMagnetOuter->SetVisAttributes(outerMagVisAtt);

	G4VisAttributes* innerMagVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
    innerMagVisAtt->SetVisibility(true);
    innerMagVisAtt->SetForceSolid(true);
    logicalLargeMagnetInner->SetVisAttributes(innerMagVisAtt);
    logicalSmallMagnetInner->SetVisAttributes(innerMagVisAtt);

	//***********************************************************************************************
	//****** Collimators Geometries ******
	// |  |  |
	// |  |  |	
	// |  |  *\
	// |  *  | \
	// *  |  |  \ 
	// |  |  |---\collTheta angle
	// *  |  |
	// |  *  |
	// |  |  *
	// |  |  |
	// |  |  |

	G4double collPlateX = magnetXSize*3;
	G4double collPlateY = (magnetPlateThickness*2.+magnetPlateSeparation);



	//****** Collimators ******

	//*****! Main Collimators on Magnet faces !*****
	
	//!Attn: MANUAL INPUT
	G4double collThetaAngle1 = 0.04;//Angle subtended by collimator to its serving magnet's center
	G4double outerRadius = 2.0*cm;	   //radius of the beam hole
    G4double collPlateZ = 10.0*mm;	   //Collimator Thickness

	// Hole for beam
	G4double innerRadius = 0.*cm;
    G4double collHoleHz = collPlateZ;
    G4double startAngle = 0.*deg;
    G4double spanningAngle = 360.*deg;

    G4Tubs* collHole
      = new G4Tubs("coll_hole",
                   innerRadius, 
                   outerRadius,
                   collHoleHz,
                   startAngle, 
                   spanningAngle);

   G4LogicalVolume* logicalCollHole
    = new G4LogicalVolume(collHole,         // its solid
                          magnetVacuum,     // its material
                          "l_coll_hole");   // its name

	//##### MagnetNumber = 1, forward #####

	// Define 5mm block of Pb
    G4Box* collBlockm1f 
    = new G4Box("coll_block_m1f",              // its name
                collPlateX/2,
                collPlateY/2,
                collPlateZ/2);                // its size
    
    G4LogicalVolume* logicalCollBlockm1f
    = new G4LogicalVolume(collBlockm1f,         // its solid
                          magnetColl,               // its material
                          "l_coll_block_m1f");   // its name

	//{Hole,Slits} at MagnetNumber = 1, forward: Placement

	//Hole

    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(),          // at (0,0,0) in outer volume
                      logicalCollHole,  // its logical volume
                      "pv_coll_block_m1f",   // its name
                      logicalCollBlockm1f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	
	
	// Collimator Slits
	G4double collSlitWm1f1 = (16.*0.06125)*cm;	   //slit width of the collimator

    G4Box* collSlitm1f1 
    = new G4Box("coll_block_m1f1",              // its name
                collSlitWm1f1/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm1f1
     = new G4LogicalVolume(collSlitm1f1,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m1f1");   // its name

	G4double collSlitWm1f2 = (16.*0.125)*cm;	   //slit width of the collimator

    G4Box* collSlitm1f2 
    = new G4Box("coll_block_m1f2",              // its name
                collSlitWm1f2/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm1f2
     = new G4LogicalVolume(collSlitm1f2,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m1f2");   // its name


	//Slits 
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet1ZPos-magnet3ZPos+(magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm1f1,  // its logical volume
                      "pv_coll_slit_m1f1",   // its name
                      logicalCollBlockm1f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps		

	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet1ZPos-magnet2ZPos+(magnetSmallZsize)),0.,0.),
                      logicalCollSlitm1f2,  // its logical volume
                      "pv_coll_slit_m1f2",   // its name
                      logicalCollBlockm1f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//MagnetNumber = 1, forward: Placement

#if 1
	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,magnet1ZPos+((magnetSmallZsize/2)+(collPlateZ/2))),
                      logicalCollBlockm1f,  // its logical volume
                      "pv_coll_slit_m1f",   // its name
                      flogicalWorld,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
#endif

#if 0
 	//##### MagnetNumber = 1, Backward #####

	// Define 5mm block of Pb
    G4Box* collBlockm1b 
    = new G4Box("coll_block_m1b",              // its name
                collPlateX/2,
                collPlateY/2,
                collPlateZ/2);                // its size
    
    G4LogicalVolume* logicalCollBlockm1b
    = new G4LogicalVolume(collBlockm1b,         // its solid
                          magnetColl,               // its material
                          "l_coll_block_m1b");   // its name

	//{Hole,Slits} at MagnetNumber = 1, Backward: Placement

	//Hole

    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(1.0*mm,0.,0.),          // at (0,0,0) in outer volume
                      logicalCollHole,  // its logical volume
                      "pv_coll_block_m1b",   // its name
                      logicalCollBlockm1b,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Collimator Slits
	G4double collSlitWm1b1 = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm1b1 
    = new G4Box("coll_block_m1b1",              // its name
                collSlitWm1b1/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm1b1
     = new G4LogicalVolume(collSlitm1b1,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m1b1");   // its name

	G4double collSlitWm1b2 = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm1b2 
    = new G4Box("coll_block_m1b2",              // its name
                collSlitWm1b2/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm1b2
     = new G4LogicalVolume(collSlitm1b2,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m1b2");   // its name

	//Slits 
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet1ZPos-magnet3ZPos-(magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm1b1,  // its logical volume
                      "pv_coll_slit_m1b1",   // its name
                      logicalCollBlockm1f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps		

	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet1ZPos-magnet2ZPos),0.,0.),
                      logicalCollSlitm1b2,  // its logical volume
                      "pv_coll_slit_m1b2",   // its name
                      logicalCollBlockm1b,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//MagnetNumber = 1, Backward: Placement


	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,magnet1ZPos-((magnetSmallZsize/2)+(collPlateZ/2))),
                      logicalCollBlockm1b,  // its logical volume
                      "pv_coll_slit_m3f",   // its name
                      flogicalWorld,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//##### MagnetNumber = 2, forward #####

	// Define 5mm block of Pb
    G4Box* collBlockm2f 
    = new G4Box("coll_block_m2f",              // its name
                collPlateX/2,
                collPlateY/2,
                collPlateZ/2);                // its size
    
    G4LogicalVolume* logicalCollBlockm2f
    = new G4LogicalVolume(collBlockm2f,         // its solid
                          magnetColl,               // its material
                          "l_coll_block_m2f");   // its name

	//{Hole,Slits} at MagnetNumber = 2, forward: Placement

	//Hole

    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(16.80*mm,0.,0.),      	//16.59515*mm    
                      logicalCollHole,  // its logical volume
                      "pv_coll_block_m2f",   // its name
                      logicalCollBlockm2f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Collimator Slits
	G4double collSlitWm2f1 = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm2f1 
    = new G4Box("coll_block_m2f1",              // its name
                collSlitWm2f1/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm2f1
     = new G4LogicalVolume(collSlitm2f1,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m2f1");   // its name

	G4double collSlitWm2f2 = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm2f2 
    = new G4Box("coll_block_m2f2",              // its name
                collSlitWm2f2/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm2f2
     = new G4LogicalVolume(collSlitm2f2,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m2f2");   // its name	

	//Slits 
	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet2ZPos-magnet3ZPos+(2.*magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm2f1,  // its logical volume
                      "pv_coll_slit_m2f1",   // its name
                      logicalCollBlockm2f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*((3.*magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm2f2,  // its logical volume
                      "pv_coll_slit_m2f2",   // its name
                      logicalCollBlockm2f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps




	//MagnetNumber = 2, Forward: Placement


	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,magnet2ZPos + ((2.*magnetSmallZsize/2)+(collPlateZ/2))),
                      logicalCollBlockm2f,  // its logical volume
                      "pv_coll_slit_m2f",   // its name
                      flogicalWorld,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//##### MagnetNumber = 2, backward #####

	// Define 5mm block of Pb
    G4Box* collBlockm2b 
    = new G4Box("coll_block_m2b",              // its name
                collPlateX/2,
                collPlateY/2,
                collPlateZ/2);                // its size
    
    G4LogicalVolume* logicalCollBlockm2b
    = new G4LogicalVolume(collBlockm2b,         // its solid
                          magnetColl,               // its material
                          "l_coll_block_m2b");   // its name

	//{Hole,Slits} at MagnetNumber = 2, backward: Placement
	
	//Hole

    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(16.80*mm,0.,0.),      	//16.59515*mm    
                      logicalCollHole,  // its logical volume
                      "pv_coll_block_m2b",   // its name
                      logicalCollBlockm2b,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Collimator Slits
	G4double collSlitWm2b = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm2b 
    = new G4Box("coll_block_m2b",              // its name
                collSlitWm2b/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm2b
     = new G4LogicalVolume(collSlitm2b,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m2b");   // its name

	//Slits 
	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*(magnet2ZPos-magnet3ZPos-(2.*magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm2b,  // its logical volume
                      "pv_coll_slit_m2b",   // its name
                      logicalCollBlockm2b,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//MagnetNumber = 2, backward: Placement

	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,magnet2ZPos - ((2.*magnetSmallZsize/2)+(collPlateZ/2))),
                      logicalCollBlockm2b,  // its logical volume
                      "pv_coll_slit_m2b",   // its name
                      flogicalWorld,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//##### MagnetNumber = 3, forward #####

	// Define 5mm block of Pb
    G4Box* collBlockm3f 
    = new G4Box("coll_block_m3f",              // its name
                collPlateX/2,
                collPlateY/2,
                collPlateZ/2);                // its size
    
    G4LogicalVolume* logicalCollBlockm3f
    = new G4LogicalVolume(collBlockm3f,         // its solid
                          magnetColl,               // its material
                          "l_coll_block_m3f");   // its name

	//{Hole,Slits} at MagnetNumber = 3, forward: Placement

	//Hole

    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(1.*mm,0.,0.),          
                      logicalCollHole,  // its logical volume
                      "pv_coll_block_m3f",   // its name
                      logicalCollBlockm3f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Collimator Slits
	G4double collSlitWm3f = 0.25*cm;	   //slit width of the collimator

    G4Box* collSlitm3f 
    = new G4Box("coll_block_m3f",              // its name
                collSlitWm3f/2,
                collPlateY/2,
                collPlateZ/2);                // its size

    G4LogicalVolume* logicalCollSlitm3f
     = new G4LogicalVolume(collSlitm3f,         // its solid
                           magnetVacuum,               // its material
                           "l_coll_slit_m3f");   // its name

	//Slits 
	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(collThetaAngle1*((magnetSmallZsize/2)),0.,0.),
                      logicalCollSlitm3f,  // its logical volume
                      "pv_coll_slit_m3f",   // its name
                      logicalCollBlockm3f,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


	//MagnetNumber = 3, forward: Placement

	new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,magnet3ZPos + ((magnetSmallZsize/2)+(collPlateZ/2))),
                      logicalCollBlockm3f,  // its logical volume
                      "pv_coll_slit_m3f",   // its name
                      flogicalWorld,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps


#endif

}

void SpinICDetectorConstruction::DefineIC()
{

#if 1    
	// Define the IC geometry here
	
	//IC Geometry Parameters
	G4double icXYsize = 40.*cm;
	G4double icZsize = 1.0*m;
	G4double icShellThick = 1.*mm;
	G4double icWinXYsize = icXYsize - 2.*cm;

	//IC Position
	G4double icPosZ = 5.5*m; /*Y = 0*/
	G4double icPosX = (icXYsize/2.) + (4.*cm); /*Depends on icXYsize*/

	//materials
	G4Material* icShell = G4Material::GetMaterial("G4_Al");
	G4Material* icGas = G4Material::GetMaterial("G4_Ar");
    G4Material* icPb = G4Material::GetMaterial("G4_Pb");    

	// Define icOuter volume as large block of Al
    G4Box* icOuter
    = new G4Box("s_ic_Outer",              // its name
                (icXYsize + (icShellThick*2.))/2.,
                (icXYsize + (icShellThick*2.))/2.,
                (icZsize + (icShellThick*2.))/2.);                // its size
    
    G4LogicalVolume* logicalIcOuter
    = new G4LogicalVolume(icOuter,         // its solid
                          icShell,               // its material
                          "l_ic_Outer");   // its name

 	// Define icInner volume as block of Ar
    G4Box* icInner
    = new G4Box("s_ic_Inner",              // its name
                (icXYsize)/2.,
                (icXYsize)/2.,
                (icZsize)/2.);                // its size
    
    G4LogicalVolume* logicalIcInner
    = new G4LogicalVolume(icInner,         // its solid
                          icGas,               // its material
                          "l_ic_Inner");   // its name

	// Define icWin volume as block of Ar
    G4Box* icWin
    = new G4Box("s_ic_Win",              // its name
                (icWinXYsize)/2.,
                (icWinXYsize)/2.,
                (icShellThick)/2.);                // its size
    
    G4LogicalVolume* logicalIcWin
    = new G4LogicalVolume(icWin,         // its solid
                          icPb,               // its material = {icGas, icPb}
                          "l_ic_Win");   // its name
	
	// Place icInner volume as a daugter of icOuter volume
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(),          // at (0,0,0) in outer volume
                      logicalIcInner,  // its logical volume
                      "pv_ic_merge",   // its name
                      logicalIcOuter,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Place icWin volume as a daugter of icOuter volume
    new G4PVPlacement(0,                        // no rotation
                      G4ThreeVector(0.,0.,-(icZsize + icShellThick)/2.),// at (x,y,z) in outer volume
                      logicalIcWin,  // its logical volume
                      "pv_ic_merge_win",   // its name
                      logicalIcOuter,  // ** its mother volume **
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// Physically Place icInner + icOuter combo volume

#if 0
    new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(-icPosX,0.,icPosZ), // position
                      logicalIcOuter,          // its logical volume
                      "pv_ic1",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps
#endif
	

	new G4PVPlacement(0,                                // no rotation
                      G4ThreeVector(icPosX,0.,icPosZ), // position
                      logicalIcOuter,          // its logical volume
                      "pv_ic2",             // its name
                      flogicalWorld,            // its mother volume
                      false,                    // no boolean operation
                      0,                        // copy number
                      fCheckOverlaps);          // checking overlaps

	// IC Visualization attributes
    //
    G4VisAttributes* innerICVisAtt = new G4VisAttributes(G4Colour(0.7, 0.7, 0.5, 1.0));
    innerICVisAtt->SetVisibility(true);
    innerICVisAtt->SetForceSolid(true);
    logicalIcInner->SetVisAttributes(innerICVisAtt);

#if 0
    G4VisAttributes* outerICVisAtt = new G4VisAttributes(G4Colour(0.0, 0.5, 0.5, 0.5));
    outerICVisAtt->SetVisibility(true);
    outerICVisAtt->SetForceSolid(true);
    logicalIcOuter->SetVisAttributes(outerICVisAtt);
#endif

#endif
    
}


