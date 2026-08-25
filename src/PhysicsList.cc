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
//
/// \file electromagnetic/TestEm18/src/PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "PhysListEmStandard.hh"
#include "PhysListEmLivermore.hh"
#include "PhysListEmPenelope.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"


//added for ALPs Physics on 27th Jun by Abhinab Mukhopadhyay
#include "DMParticleALP.hh"
#include "ALP.hh"
#include "DMProcessPrimakoffALP.hh"
#include "G4ProcessManager.hh"
#include "StepMax.hh"


/* Added EM option 4 best EM option*/
#include "G4EmStandardPhysics_option4.hh"
#include "G4PhysListFactory.hh"
#include "FTFP_BERT.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList(),
  fMessenger(0), 
  fEmPhysicsList(0),
  fALP(nullptr)

{
  G4LossTableManager::Instance();
  fMessenger = new PhysicsListMessenger(this); 
   
  // EM physics
  fEmName = G4String("standard");
  fEmPhysicsList = new PhysListEmStandard(fEmName);
    
  // EM — kept manual (constructed in ConstructProcess via fEmPhysicsList)
  // fEmName = G4String("emstandard_opt4");
  // fEmPhysicsList = new G4EmStandardPhysics_option4();
  // RegisterPhysics(fEmPhysicsList);            // <-- register it now

  // FTFP_BERT content — registered onto THIS modular list.
  // G4VModularPhysicsList::ConstructProcess() (called in your
  // ConstructProcess) will invoke these.
  // RegisterPhysics(new G4HadronElasticPhysics());
  //(new G4HadronPhysicsFTFP_BERT());

   RegisterPhysics(new G4DecayPhysics());
  // RegisterPhysics(new G4StoppingPhysics());
  // RegisterPhysics(new G4IonPhysics());
  // RegisterPhysics(new G4NeutronTrackingCut());

  SetDefaultCutValue(0.01*mm);
    
  SetVerboseLevel(1);

  //register FCP  physics
  AddFCPPhysics(0.1,105.66*MeV);//Default charge and mass of FCP

//  RegisterPhysics( lPhys ); for now remove this line for mCP

   // ---------------------------------------------------------------
  // ALP physics parameters — the only two numbers you need to tune.
  // Material properties (Z, A, density) are NOT set here; they are
  // read automatically per-step from G4Track in DMProcessPrimakoffALP.
  // ---------------------------------------------------------------
  G4double alpMass_GeV = 0.0167;  // ALP mass in GeV
  G4double eThresh_GeV = 0.1;  // Energy cut (based on detector) for now set to 0.1 GeV
 
  // Dummy Z=1, A=1, density=1 — overridden per-step from G4Material
  fALP = new ALP(alpMass_GeV, eThresh_GeV,1,1,1);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
  delete fEmPhysicsList;
  delete fMessenger; 
  delete fALP;
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if (name == fEmName) return;

  if (name == "standard") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmStandard(name);
        
  } else if (name == "livermore") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmLivermore(name);
    
  } else if (name == "penelope") {

    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmPenelope(name);

  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"

// leptons
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

// Mesons
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"

#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

void PhysicsList::ConstructParticle()
{
// pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  
// gamma
  G4Gamma::GammaDefinition();

// leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  
// mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  
// baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
  
// ions
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4GenericIon::GenericIonDefinition();

//Construct FCP
//lPhys->ConstructParticle();

DMParticleALP::Definition();


  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
   // This calls ConstructProcess() on ALL registered physics including lPhys
  G4VModularPhysicsList::ConstructProcess();
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  //AddStepMax();

  // ------------------------------------------------------------------
  // Finalize ALP object now that DMParticleALP singleton exists,
  // then attach Primakoff process to G4Gamma.
  // Z, A, density are NOT set here — DMProcessPrimakoffALP reads them
  // live from G4Track::GetMaterial() on every step.
  // adaption from DarkMatterPhysics.cc
  // ------------------------------------------------------------------
  G4ParticleDefinition* alpParticle = DMParticleALP::Definition();
  fALP->SetMA(alpParticle->GetPDGMass() / GeV);
  fALP->SetDMPDGID(alpParticle->GetPDGEncoding());
  fALP->PrepareTable();
 
  DMProcessPrimakoffALP* primakoffProc =
      new DMProcessPrimakoffALP(fALP, alpParticle);
 
  G4Gamma::GammaDefinition()->GetProcessManager()->AddDiscreteProcess(primakoffProc);





  AddStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  StepMax* stepMaxProcess = new StepMax();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()){
      G4ParticleDefinition* particle = particleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (stepMaxProcess->IsApplicable(*particle) && !particle->IsShortLived())
        {
          pmanager ->AddDiscreteProcess(stepMaxProcess);
        }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
