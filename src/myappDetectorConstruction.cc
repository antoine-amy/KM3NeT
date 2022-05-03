//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "myappDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"


myappDetectorConstruction::myappDetectorConstruction():G4VUserDetectorConstruction(), fScoringVolume(0){}
myappDetectorConstruction::~myappDetectorConstruction(){}


G4VPhysicalVolume* myappDetectorConstruction::Construct() {
  G4NistManager* nist=G4NistManager::Instance(); //Get nist material manager
  G4bool checkOverlaps = true; //Option to switch on/off checking of volumes overlaps

  // Envelope parameters
  G4double env_sizeXY = 100*cm, env_sizeZ = 100*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  //World
  G4double world_sizeXY = 1.2*env_sizeXY, world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

  //Envelope
  G4Box* solidEnv = new G4Box("Envelope", 0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ);
  G4LogicalVolume* logicEnv=new G4LogicalVolume(solidEnv, env_mat, "Envelope");
  new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  //photomultiplicator
  //G4ThreeVector pos1 = G4ThreeVector(0, 2*cm, -7*cm);
  G4Material* pm_mat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
  G4ThreeVector pospm = G4ThreeVector(0,0,0);
  //G4Box* solidpm=new G4Box("pm",20*cm,20*cm,20*cm);
  G4Sphere* solidpm=new G4Sphere("pm",0*cm,20*cm,0*deg,360*deg,0*deg,360*deg);
  G4LogicalVolume* logicpm = new G4LogicalVolume(solidpm, pm_mat, "pm");
  new G4PVPlacement(0, pospm, logicpm, "pm", logicEnv, false, 0, checkOverlaps);
  fScoringVolume = logicpm;

  return physWorld;
}
