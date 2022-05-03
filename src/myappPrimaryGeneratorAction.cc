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

#include "myappPrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


myappPrimaryGeneratorAction::myappPrimaryGeneratorAction():G4VUserPrimaryGeneratorAction(), fParticleGun(0), fEnvelopeBox(0){
  G4int n_particle=1;
  fParticleGun=new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable=G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle=particleTable->FindParticle(particleName="chargedgeantino");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);
}

myappPrimaryGeneratorAction::~myappPrimaryGeneratorAction(){
  delete fParticleGun;
}

void myappPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  G4double envSizeXY=0;
  G4double envSizeZ=0;
  G4ParticleDefinition* particle=fParticleGun->GetParticleDefinition();
  if (particle==G4ChargedGeantino::ChargedGeantino()){
    G4int Z=19, A=40;
    G4double ionCharge=0.*eplus;
    G4double excitEnergy=0.*keV;
    G4ParticleDefinition* ion=G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge);
  }

  if (!fEnvelopeBox){
    G4LogicalVolume* envLV=G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if (envLV) fEnvelopeBox=dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if (fEnvelopeBox){
    envSizeXY=fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ=fEnvelopeBox->GetZHalfLength()*2.;
  }else{
    G4ExceptionDescription msg;
    msg<<"Envelope volume of box shape not found.\n";
    msg<<"Perhaps you have changed geometry.\n";
    msg<<"The gun will be place at the center.";
    G4Exception("myappPrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002",JustWarning,msg);
  }

  G4double x0=envSizeXY*(G4UniformRand()-0.5);
  G4double y0=envSizeXY*(G4UniformRand()-0.5);
  G4double z0=envSizeXY*(G4UniformRand()-0.5);

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
