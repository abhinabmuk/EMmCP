#include "DMProcessPrimakoffALP.hh"

#include "DarkMatter.hh"
#include "ALP.hh"

#include "DMParticleALP.hh"

#include "G4ProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"

//Added on 27th June by Abhinab 
// needed for parsing material properties

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"

/*

Removal of SigmaNorm dependecne*/

DMProcessPrimakoffALP::DMProcessPrimakoffALP(DarkMatter* DarkMatterPointerIn, G4ParticleDefinition* theDMParticlePtrIn)
: G4VDiscreteProcess( "DMProcessPrimakoffALP", fUserDefined ),  // fElectromagnetic
  myDarkMatter(DarkMatterPointerIn),
  theDMParticlePtr(theDMParticlePtrIn)
{
  SetProcessSubType( 1 ); //fBremsstrahlung? // TODO: verify this
}

G4bool DMProcessPrimakoffALP::IsApplicable(const G4ParticleDefinition & pDef)
{
  return ("gamma" == pDef.GetParticleName());
}



// ---------------------------------------------------------------------
// Picks the dominant element of a (possibly compound) material.
// Primakoff cross section scales as Z^2, so the highest-Z element
// dominates; this mirrors what Geant4's own gamma-conversion process
// does internally when selecting a target element per interaction.
// ---------------------------------------------------------------------
const G4Element* DMProcessPrimakoffALP::GetDominantElement(const G4Material* mat) const
{
  const G4ElementVector* elements = mat->GetElementVector();
  const G4Element* dominant = (*elements)[0];
  for (std::size_t i = 1; i < elements->size(); ++i) {
    if ((*elements)[i]->GetZ() > dominant->GetZ()) {
      dominant = (*elements)[i];
    }
  }
  return dominant;
}



G4double DMProcessPrimakoffALP::GetMeanFreePath( const G4Track& aTrack,
                                                 G4double, /*previousStepSize*/
                                                 G4ForceCondition* /*condition*/ )
{
  G4double DensityMat = aTrack.GetMaterial()->GetDensity()/(g/cm3);
  G4double ekin = aTrack.GetKineticEnergy()/GeV;

  // adding to get Z and A values 
  const G4Material* mat = aTrack.GetMaterial();
  const G4Element*  elm = GetDominantElement(mat);
 
  G4double Z          = elm->GetZ();   
  G4double A          = elm->GetN();          // atomic mass number (amu)

  if( myDarkMatter->EmissionAllowed(ekin, DensityMat) ) {

    //  G4double CrossSection = myDarkMatter->GetSigmaTot(ekin); //A.C. by DarkMatter definition, this is in picobarn
    // Z and A values comes from current step in material   

   std::cout << "CrossSection  Z" << Z << std::endl;
   std::cout << "CrossSection  A " << A << std::endl;

   // both below give same result
   //G4double CrossSection = myDarkMatter->GetSigmaTot(ekin);
   G4double CrossSection = myDarkMatter->TotalCrossSectionCalcPrimakoff(ekin, A, Z); //picobarn

    CrossSection *= picobarn;

    std::cout << "CrossSection  before benching " << CrossSection << std::endl;


      //The DarkMatter classes compute the cross section for eps = epsilBench. Here, we revert back to epsilon
      CrossSection *= (myDarkMatter->Getepsil()* myDarkMatter->Getepsil())/(myDarkMatter->GetepsilBench()* myDarkMatter->GetepsilBench());
    
      //sigma norm removed
       //CrossSection /= myDarkMatter->GetSigmaNorm();


      G4double n = aTrack.GetMaterial()->GetTotNbOfAtomsPerVolume();
      G4double XMeanFreePath = 1./(n*CrossSection);

//      XMeanFreePath /= BiasSigmaFactor;

       //debug check
       std::cout << "CrossSection " << CrossSection << std::endl;
       std::cout << "picobarn value " << picobarn << std::endl;
       std::cout << "GetepsilBench() value " << myDarkMatter->GetepsilBench() << std::endl;
       std::cout << "Getepsil() value " << myDarkMatter->Getepsil() << std::endl;
     //  std::cout << "GetSigmaNorm() value " << myDarkMatter->GetSigmaNorm() << std::endl;
      // std::cout << "BiasSigmaFactor value " << BiasSigmaFactor << std::endl;
       std::cout << "n value" << n << std::endl;
       std::cout << "XMeanFreePath value " << XMeanFreePath << std::endl;





      return XMeanFreePath;

  }
  return DBL_MAX;
}

G4VParticleChange* DMProcessPrimakoffALP::PostStepDoIt( const G4Track& aTrack,
                                                        const G4Step & aStep )
{
  const G4double incidentE = aTrack.GetKineticEnergy();
  //const G4double DMMass = theDMParticleAPrimePtr->GetPDGMass();
  G4ThreeVector incidentDir = aTrack.GetMomentumDirection();

  G4double angles[2];


 /*Same material lookup as GetMeanFreePath — must stay consistent,
   since the angular/energy sampling depends on Z, A too.  */ 
 // see SimulateEmissionWithAngle3 function overload in DarkMatter.cc

 const G4Material* mat = aTrack.GetMaterial();
  const G4Element*  elm = GetDominantElement(mat);
  G4double Z = elm->GetZ();
  G4double A = elm->GetN();


  G4double XAcc = myDarkMatter->SimulateEmissionWithAngle3(incidentE/GeV, angles, A, Z);

  

  // Check if it failed? In this case XAcc = 0

  if(XAcc > 0.001) myDarkMatter->EmissionSimulated();

  G4double DMTheta = angles[0], DMPhi = angles[1];
  G4double DME = incidentE * XAcc;
  G4double DMM = myDarkMatter->GetMA()*GeV;
  G4double DMKinE = DME - DMM;
  if(DMKinE < 0.) DMKinE = 0.;

  // Initialize DM direction vector:
  G4ThreeVector DMDirection(0., 0., .1);
  {
    DMDirection.setMag(1.);
    DMDirection.setTheta( DMTheta );
    DMDirection.setPhi( DMPhi );
    DMDirection.rotateUz(incidentDir);
  }
  
  G4DynamicParticle* movingDM = new G4DynamicParticle( theDMParticlePtr,
                                                       DMDirection,
                                                       DMKinE );
  aParticleChange.Initialize( aTrack );

  // Set DM:
  aParticleChange.SetNumberOfSecondaries( 1 );
  aParticleChange.AddSecondary( movingDM );
  // Kill projectile:
  aParticleChange.ProposeEnergy( 0. );
  aParticleChange.ProposeTrackStatus( fStopAndKill );

 std::cout << "DM PDG ID = " << theDMParticlePtr->GetPDGEncoding()
            << " emitted by " << aTrack.GetDefinition()->GetParticleName()
            << " in material " << mat->GetName() << " (Z=" << Z << ", A=" << A << ")"
            << " with energy = " << incidentE/GeV << " DM energy = " << DME/GeV << std::endl;
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}
