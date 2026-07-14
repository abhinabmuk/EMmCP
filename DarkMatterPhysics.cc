#include "DarkMatterPhysics.hh"
#include "DarkMatter.hh"
#include "ALP.hh"
#include "DMProcessPrimakoffALP.hh"
#include "DMParticleALP.hh"

//--------------------------------//

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

#include "G4BuilderType.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"


DarkMatterPhysics::DarkMatterPhysics() 
: G4VPhysicsConstructor("DarkMatterPhysics")
{
  SetPhysicsType(bUnknown);
  //fMessenger = new DarkMatterPhysicsMessenger();

  if(!DarkMatterPhysicsConfigure()) {
    G4cout << "Dark Matter physics is not properly configured, exiting" << G4endl;
    exit(1);
  }
  this->Init();
 }


DarkMatterPhysics::DarkMatterPhysics(void *ptr)
: G4VPhysicsConstructor("DarkMatterPhysics")
{
  SetPhysicsType(bUnknown);
  //fMessenger = new DarkMatterPhysicsMessenger();

  if(!DarkMatterPhysicsConfigure(ptr)) {
    G4cout << "Dark Matter physics is not properly configured, exiting" << G4endl;
    exit(1);
  }
  this->Init();
}


DarkMatterPhysics::~DarkMatterPhysics()
{
  if(myDarkMatter) delete myDarkMatter;
}

void DarkMatterPhysics::Init(){
  //call an instance of the class
  DarkMatterParametersRegistry* DMpar = DarkMatterParametersRegistry::GetInstance();

  G4double EThresh = DMpar->GetRegisteredParam("EThresh");
  G4int DMProcessType = DMpar->GetRegisteredParam("DMProcessType");
  double DMMass   = DMpar->GetRegisteredParam("DMMass");
  double Epsilon  = DMpar->GetRegisteredParam("Epsilon");
  G4double ANucl     = DMpar->GetRegisteredParam("ANucl");
  G4double ZNucl     = DMpar->GetRegisteredParam("ZNucl");
  G4double Density   = DMpar->GetRegisteredParam("Density");
  G4int DecayType = DMpar->GetRegisteredParam("DecayType");

  G4double RDM =  DMpar->GetRegisteredParam("RDM",1./3);
  G4double fFactor = DMpar->GetRegisteredParam("Ffactor",0.1);
  G4int BranchingType = DMpar->GetRegisteredParam("BranchingType",0);

/*
 * A.C. all quantities obtained from DMPar have intrinsic G4 units
 * In the following, we pass them to DarkMatter classes, that use following convention:
 *
 * Masses, energies => GeV
 * Density => g/cm3
 *
 * We convert them here
 */

  EThresh/=GeV;
  DMMass/=GeV;
  Density/=(g/cm3);



  switch(DMProcessType)
    {
    case 21:
      G4cout << "Initialize ALP\n";
      myDarkMatter = new ALP(DMMass, EThresh, 1., ANucl, ZNucl, Density,  Epsilon);
      break;
     default:
       G4cout << G4endl << "Wrong DM process type specified: " << DMProcessType << " , exiting" << G4endl << G4endl;
       exit(1);
     }
  // BiasSigmaFactor = DMpar->GetRegisteredParam("BiasSigmaFactor0") * (myDarkMatter->GetepsilBench()*myDarkMatter->GetepsilBench()) / (myDarkMatter->Getepsil()*myDarkMatter->Getepsil());
  // removal on coupling dependence 
  
}


void DarkMatterPhysics::ConstructParticle()
{
  /*A.C.
   * The following lines are necessary to construct the particles that will be propagated for annihilation
   *
   */

  G4int DMProcessType = 21;
//  G4int DecayType = (G4int)(DMpar->GetRegisteredParam("DecayType"));
//  G4int BranchingType = (G4int)(DMpar->GetRegisteredParam("BranchingType", 0.));

  switch(DMProcessType)
    {
    case 21:
      DMParticleALP::Definition();
      break;
    default:
      break;
    }
}


void DarkMatterPhysics::ConstructProcess()
{
  // Which DM particle?
  G4ParticleDefinition* theDMParticlePtr = 0;
//  if(myDarkMatter->GetParentPDGID() == 22) {
  theDMParticlePtr = DMParticleALP::Definition();
//  }

  if(!theDMParticlePtr) {G4cout << "DarkMatterPhysics::ConstructProcess: did not manage to determine the DM particle type, exiting" << G4endl; exit(1);}

  myDarkMatter->SetMA(theDMParticlePtr->GetPDGMass()/GeV);
  myDarkMatter->SetDMPDGID(theDMParticlePtr->GetPDGEncoding());
  myDarkMatter->PrepareTable();

  G4PhysicsListHelper * phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();

  phLHelper->DumpOrdingParameterTable();

  // if one need to (re-)associate certain process with a particle, note
  // the following snippet
  //G4ProcessManager * pMgr = Mocktron::Definition()->GetProcessManager();
  //pmanager->RemoveProcess(idxt);
  //pmanager->AddProcess(new G4MonopoleTransportation(fMpl),-1, 0, 0);

  // ... here one can set up the model parameters from external config
  //     sources, internal attributes previously set by messengers, etc

  // ... here the processes asociated with new physics should be registered
  //     as follows
  
//  if(myDarkMatter->GetParentPDGID() == 22) {
    DMProcessPrimakoffALP* DMPrimakoffALPPointer = new DMProcessPrimakoffALP(myDarkMatter, theDMParticlePtr, BiasSigmaFactor);

    // Instead of using ordtable:
    G4ProcessManager* processManager = (G4Gamma::GammaDefinition())->GetProcessManager();
    processManager->AddDiscreteProcess(DMPrimakoffALPPointer);

    //phLHelper->RegisterProcess( DMPrimakoffALPPointer, G4Gamma::GammaDefinition() );
//  }
}
