/////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------
//      File:        FCPPhyics.cc                                   /////
//      Description: Register process and models for FCPs           /////
//      21 December 2019, Samir Banik                               /////
// **********************************************************************
// ----------------------------------------------------------------------
// Edited by Abhinab M.
/////////////////////////////////////////////////////////////////////////

#include "FCPPhysics.hh"
#include "G4FCPPlus.hh"
#include "G4FCPMinus.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "FCPIonisation.hh"
#include "FCPBremsstrahlung.hh"
#include "FCPMultipleScattering.hh"
#include "FCPBetheBlochModel.hh"

#include "G4CoulombScattering.hh"
#include "FCPPairProduction.hh"
#include "G4PAIModel.hh"
#include "G4PhysicsListHelper.hh"
#include "G4VModularPhysicsList.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4LossTableManager.hh"

// adding Ionization A model
#include "G4ICRU73QOModel.hh"

#include "G4WentzelVIModel.hh"


/*Abhinab comments*/
// it should be noted that there is no nuclear stopping power in this simulation which should be carefully considered for low energy heavy charged mass particles 
// Check passage of time comments in pdg for nuclear stopping power


#include "G4NuclearStopping.hh"   //nuclear stopping module 
#include "G4EmParameters.hh"
#include "G4EmBuilder.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VProcess.hh"

FCPPhysics::FCPPhysics(G4double charge, G4double mass)
  :G4VPhysicsConstructor("FCPPhysics"),lipcharge(charge),lipmass(mass)
{
  G4cout<<"Physics List For FCP"<<G4endl;
  
  // G4EmParameters* param = G4EmParameters::Instance();
  // param->SetMaxNIELEnergy(1*CLHEP::MeV);

}
FCPPhysics::~FCPPhysics()
{
}
void FCPPhysics::ConstructParticle()
{
  G4FCPPlus::MakeFCPPlus(lipcharge,lipmass);
  G4FCPMinus::MakeFCPMinus(lipcharge,lipmass);
}
void FCPPhysics::ConstructProcess()
{
  G4PhysicsListHelper*  ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4ParticleDefinition* lipp = G4FCPPlus::Definition();
  G4ParticleDefinition* lipm = G4FCPMinus::Definition();
 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //############################## Register Physics processes for FCPPlus ##############################
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  FCPIonisation* FCPPlus_Ioni               = new FCPIonisation();
  FCPBremsstrahlung* FCPPlus_Brem           = new FCPBremsstrahlung();
  FCPMultipleScattering* FCPPlus_msc        = new FCPMultipleScattering();
  G4CoulombScattering* FCPPlus_CoulombSC    = new G4CoulombScattering();
  FCPPairProduction* FCPPlus_PairProduction = new FCPPairProduction();
//ph->RegisterProcess(FCPPlus_msc, lipp);
ph->RegisterProcess(FCPPlus_Ioni, lipp);
//ph->RegisterProcess(FCPPlus_Brem, lipp);
ph->RegisterProcess(FCPPlus_PairProduction, lipp);
//ph->RegisterProcess(FCPPlus_CoulombSC, lipp); 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //############################## Register Physics processes for FCPMinus #############################
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   FCPIonisation* FCPMinus_Ioni               = new FCPIonisation();
    FCPBremsstrahlung* FCPMinus_Brem           = new FCPBremsstrahlung();
  FCPMultipleScattering* FCPMinus_msc        = new FCPMultipleScattering();
  G4CoulombScattering* FCPMinus_CoulombSC     = new G4CoulombScattering();
  FCPPairProduction* FCPMinus_PairProduction = new FCPPairProduction();
  // FCPMinus_msc->AddEmModel(0, new G4WentzelVIModel()); //combined scattering model
// ph->RegisterProcess(FCPMinus_msc, lipm);
 ph->RegisterProcess(FCPMinus_Ioni, lipm);
//ph->RegisterProcess(FCPMinus_Brem, lipm);
 ph->RegisterProcess(FCPMinus_PairProduction, lipm);
//ph->RegisterProcess(FCPMinus_CoulombSC, lipm);


G4ProcessManager* pm = lipm->GetProcessManager();
G4ProcessVector* pv = pm->GetProcessList();
G4cout << "\n=== Processes registered for " 
       << lipm->GetParticleName() << " ===" << G4endl;
for (int i = 0; i < pv->size(); i++) {
    G4cout << "  " << (*pv)[i]->GetProcessName() << G4endl;
}

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
}
