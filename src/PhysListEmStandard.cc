#include "PhysListEmStandard.hh"

#include "G4BuilderType.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4EmParameters.hh"              // CHANGE: Explicit EM parameters

// Gamma processes
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4KleinNishinaModel.hh"

// Electron processes
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4eMultipleScattering.hh"       // CHANGE: Added electron MSC

// Muon processes
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuMultipleScattering.hh"      // CHANGE: Added muon MSC

// Hadron EM processes
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4hMultipleScattering.hh"       // CHANGE: Added hadron MSC

// Ion processes
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandard::PhysListEmStandard(const G4String& name)
   : G4VPhysicsConstructor(name)
{

  G4EmParameters* param = G4EmParameters::Instance();


  // ===============================
  // CHANGE: EM Option4-like settings
  // ===============================

  param->SetDefaults();

  param->SetMinEnergy(10*eV);          // CHANGE
  param->SetMaxEnergy(100*TeV);         // CHANGE

  param->SetNumberOfBinsPerDecade(20);  // CHANGE

  param->SetBuildCSDARange(true);
  param->SetMaxEnergyForCSDARange(100*TeV);


  // CHANGE: Better multiple scattering accuracy
  param->SetMscRangeFactor(0.04);
  param->SetMscStepLimitType(fUseDistanceToBoundary);


  SetPhysicsType(bElectromagnetic);


  param->SetVerbose(0);
  param->Dump();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandard::~PhysListEmStandard()
{}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmStandard::ConstructProcess()
{

  G4PhysicsListHelper* list =
      G4PhysicsListHelper::GetPhysicsListHelper();



  auto particleIterator = GetParticleIterator();
  particleIterator->reset();


  while((*particleIterator)()) {


    G4ParticleDefinition* particle =
        particleIterator->value();


    G4String particleName =
        particle->GetParticleName();



    // ==================================================
    // GAMMA
    // ==================================================

    if(particleName == "gamma") {


      // CHANGE:
      // Enabled Rayleigh scattering
      list->RegisterProcess(
          new G4RayleighScattering(),
          particle);


      list->RegisterProcess(
          new G4PhotoElectricEffect(),
          particle);



      G4ComptonScattering* compt =
          new G4ComptonScattering();


      compt->SetEmModel(
          new G4KleinNishinaModel());


      list->RegisterProcess(
          compt,
          particle);



      list->RegisterProcess(
          new G4GammaConversion(),
          particle);



    }



    // ==================================================
    // ELECTRON
    // ==================================================

    else if(particleName == "e-") {


      // CHANGE:
      // Added multiple scattering
      list->RegisterProcess(
          new G4eMultipleScattering(),
          particle);



      G4eIonisation* eIoni =
          new G4eIonisation();


      eIoni->SetStepFunction(
          0.1,
          100*um);



      list->RegisterProcess(
          eIoni,
          particle);



      list->RegisterProcess(
          new G4eBremsstrahlung(),
          particle);

    }



    // ==================================================
    // POSITRON
    // ==================================================

    else if(particleName == "e+") {


      // CHANGE:
      // Added multiple scattering
      list->RegisterProcess(
          new G4eMultipleScattering(),
          particle);



      G4eIonisation* eIoni =
          new G4eIonisation();


      eIoni->SetStepFunction(
          0.1,
          100*um);



      list->RegisterProcess(
          eIoni,
          particle);



      list->RegisterProcess(
          new G4eBremsstrahlung(),
          particle);



      list->RegisterProcess(
          new G4eplusAnnihilation(),
          particle);

    }



    // ==================================================
    // MUONS
    // ==================================================

    else if(particleName=="mu+" ||
            particleName=="mu-") {



      // CHANGE:
      // Added muon MSC
      list->RegisterProcess(
          new G4MuMultipleScattering(),
          particle);



      G4MuIonisation* muIoni =
          new G4MuIonisation();


      muIoni->SetStepFunction(
          0.1,
          50*um);



      list->RegisterProcess(
          muIoni,
          particle);



      list->RegisterProcess(
          new G4MuBremsstrahlung(),
          particle);



      list->RegisterProcess(
          new G4MuPairProduction(),
          particle);

    }



    // ==================================================
    // PROTONS AND PIONS
    // ==================================================

    else if(particleName=="proton" ||
            particleName=="pi-" ||
            particleName=="pi+") {



      // CHANGE:
      // Added hadron MSC
      list->RegisterProcess(
          new G4hMultipleScattering(),
          particle);



      G4hIonisation* hIoni =
          new G4hIonisation();


      hIoni->SetStepFunction(
          0.1,
          20*um);



      list->RegisterProcess(
          hIoni,
          particle);



      list->RegisterProcess(
          new G4hBremsstrahlung(),
          particle);



      list->RegisterProcess(
          new G4hPairProduction(),
          particle);

    }



    // ==================================================
    // LIGHT IONS
    // ==================================================

    else if(particleName=="alpha" ||
            particleName=="He3") {


      G4ionIonisation* ionIoni =
          new G4ionIonisation();


      ionIoni->SetStepFunction(
          0.1,
          1*um);



      list->RegisterProcess(
          ionIoni,
          particle);



      list->RegisterProcess(
          new G4NuclearStopping(),
          particle);

    }



    // ==================================================
    // GENERIC IONS
    // ==================================================

    else if(particleName=="GenericIon") {


      G4ionIonisation* ionIoni =
          new G4ionIonisation();


      ionIoni->SetEmModel(
          new G4IonParametrisedLossModel());



      ionIoni->SetStepFunction(
          0.1,
          1*um);



      list->RegisterProcess(
          ionIoni,
          particle);



      list->RegisterProcess(
          new G4NuclearStopping(),
          particle);

    }



    // ==================================================
    // OTHER CHARGED PARTICLES
    // ==================================================

    else if(!particle->IsShortLived() &&
            particle->GetPDGCharge()!=0.0 &&
            particleName!="chargedgeantino") {



      list->RegisterProcess(
          new G4hMultipleScattering(),   // CHANGE
          particle);



      list->RegisterProcess(
          new G4hIonisation(),
          particle);

    }

  }



  // ==================================================
  // Atomic de-excitation
  // ==================================================

  G4VAtomDeexcitation* deex =
      new G4UAtomicDeexcitation();


  G4LossTableManager::Instance()
      ->SetAtomDeexcitation(deex);

}