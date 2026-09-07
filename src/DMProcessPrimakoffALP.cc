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

// 27 JUly 
/*
1] Double checking of mean free path computation for composite material
// 2nd August 

2] Mean free path should involve all the materials and its respective cross-section it should not include probabilities here


7th September 

3] Writing the output to file 

*/

#include "G4EventManager.hh" 
#include "G4Event.hh" 
#include <fstream> 
#include <sstream>


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

/* More accurate representation ALP Presentation*/

void DMProcessPrimakoffALP::GetEffectiveZA(const G4Material* mat,
                                           G4double& Zeff,
                                           G4double& Aeff,
                                           G4double& sumAtoms) const
{
  const G4ElementVector* elements = mat->GetElementVector();
  const G4int* atomsVector        = mat->GetAtomsVector();   // a_i per molecule
  const std::size_t nElm          = elements->size();

  Zeff = 0.0;
  Aeff = 0.0;
  sumAtoms = 0.0;

  /* N --> Mass of Nucleons */

  for (std::size_t i = 0; i < nElm; ++i) {
    const G4double ai = static_cast<G4double>(atomsVector[i]);
    Zeff     += ai * (*elements)[i]->GetZ();
    Aeff     += ai * (*elements)[i]->GetN();   // GetN() = mass number (amu), matches your original
    sumAtoms += ai;
  }
}



// G4double DMProcessPrimakoffALP::GetMeanFreePath( const G4Track& aTrack,
//                                                  G4double, /*previousStepSize*/
//                                                  G4ForceCondition* /*condition*/ )
// {
//   G4double DensityMat = aTrack.GetMaterial()->GetDensity()/(g/cm3);
//   G4double ekin = aTrack.GetKineticEnergy()/GeV;

//   // adding to get Z and A values 
//   const G4Material* mat = aTrack.GetMaterial();
//   const G4Element*  elm = GetDominantElement(mat);
 
//   G4double Z          = elm->GetZ();   
//   G4double A          = elm->GetN();          // atomic mass number (amu)

//   if( myDarkMatter->EmissionAllowed(ekin, DensityMat) ) {

//     //  G4double CrossSection = myDarkMatter->GetSigmaTot(ekin); //A.C. by DarkMatter definition, this is in picobarn
//     // Z and A values comes from current step in material   

//  //  std::cout << "CrossSection  Z" << Z << std::endl;
//   // std::cout << "CrossSection  A " << A << std::endl;

//    // both below give same result
//    //G4double CrossSection = myDarkMatter->GetSigmaTot(ekin);
//    G4double CrossSection = myDarkMatter->TotalCrossSectionCalcPrimakoff(ekin, A, Z); //picobarn

//     CrossSection *= picobarn;

//  //   std::cout << "CrossSection  before benching " << CrossSection << std::endl;


//       //The DarkMatter classes compute the cross section for eps = epsilBench. Here, we revert back to epsilon
//       CrossSection *= (myDarkMatter->Getepsil()* myDarkMatter->Getepsil())/(myDarkMatter->GetepsilBench()* myDarkMatter->GetepsilBench());
    
//       //sigma norm removed
//        //CrossSection /= myDarkMatter->GetSigmaNorm();

//       CrossSection *= 1000000; // scaling

//       G4double n = aTrack.GetMaterial()->GetTotNbOfAtomsPerVolume();
//       G4double XMeanFreePath = 1./(n*CrossSection);

// //      XMeanFreePath /= BiasSigmaFactor;

//        //debug check
//     //   std::cout << "CrossSection " << CrossSection << std::endl;
//     //   std::cout << "picobarn value " << picobarn << std::endl;
//     //   std::cout << "GetepsilBench() value " << myDarkMatter->GetepsilBench() << std::endl;
//   //     std::cout << "Getepsil() value " << myDarkMatter->Getepsil() << std::endl;
//      //  std::cout << "GetSigmaNorm() value " << myDarkMatter->GetSigmaNorm() << std::endl;
//       // std::cout << "BiasSigmaFactor value " << BiasSigmaFactor << std::endl;
//     //   std::cout << "n value" << n << std::endl;
//     //   std::cout << "XMeanFreePath value " << XMeanFreePath << std::endl;





//       return XMeanFreePath;

//   }
//   return DBL_MAX;
// }

/* Further simplification of Mean Free path 31 July */


G4double DMProcessPrimakoffALP::GetMeanFreePath( const G4Track& aTrack,
                                                 G4double, /*previousStepSize*/
                                                 G4ForceCondition* /*condition*/ )
{
  G4double DensityMat = aTrack.GetMaterial()->GetDensity()/(g/cm3);
  G4double ekin = aTrack.GetKineticEnergy()/GeV;

  const G4Material* mat = aTrack.GetMaterial();
  //----------------------------------------------------//
  if( myDarkMatter->EmissionAllowed(ekin, DensityMat) ) {
  const G4ElementVector* elements = mat->GetElementVector();
  const G4double* nAtomsPerVolume = mat->GetVecNbOfAtomsPerVolume();

  G4double invMFP = 0.0;

for (size_t i = 0; i < mat->GetNumberOfElements(); ++i)
{
    const G4Element* element = (*elements)[i];
    G4double Z = element->GetZ();
    G4double A = element->GetN();
    G4double CrossSection = myDarkMatter->TotalCrossSectionCalcPrimakoff(ekin, A, Z);
    CrossSection *= picobarn;
      // Revert from benchmark epsilon back to physical epsilon
    CrossSection *= (myDarkMatter->Getepsil()* myDarkMatter->Getepsil())
                  / (myDarkMatter->GetepsilBench()*myDarkMatter->GetepsilBench());

    CrossSection *= 1000000; // scaling by 1 million
    G4double contribution =nAtomsPerVolume[i] * CrossSection;
  //  std::cout
  //       << "Material = " << mat->GetName()
  //       << " Element = " << element->GetName()
  //       << " Z = " << Z
  //       << " A = " << A
  //       << " n = " << nAtomsPerVolume[i]
  //       << " sigma = " << CrossSection
  //       << " n*sigma = " << contribution
  //       << std::endl;

    invMFP += contribution;
}

     G4double XMeanFreePath = 1.0 / invMFP;

 
 //    std::cout << "CrossSection for Mean Path " << CrossSection << std::endl;
    // std::cout << "picobarn value " << picobarn << std::endl;
    // std::cout << "GetepsilBench() value " << myDarkMatter->GetepsilBench() << std::endl;
    // std::cout << "Getepsil() value " << myDarkMatter->Getepsil() << std::endl;
    // std::cout << "n (atoms) value " << nAtoms << std::endl;
    // std::cout << "n (molecules) value " << nMolecule << std::endl;
    // std::cout << "sumAtoms (Sum a_i) value " << sumAtoms << std::endl;
  //   std::cout << "XMeanFreePath value " << XMeanFreePath << std::endl;
    // std::cout << "Photon energy = " << ekin << " GeV" << std::endl;

    return XMeanFreePath;
  }
  return DBL_MAX;
}





/* Update following July 31st */

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
  // auto table = BuildTargetTable(mat, ekin);

  // const auto& target = SampleTarget(table);
  const G4Material* mat = aTrack.GetMaterial();
  // const G4Element*  elm = GetDominantElement(mat);
  // G4double Z = elm->GetZ();
  // G4double A = elm->GetN();

  auto table = BuildTargetTable(mat, incidentE);
  const auto& target = SampleTarget(table);

  G4double XAcc = myDarkMatter->SimulateEmissionWithAngle3(incidentE/GeV, angles, target.A,  target.Z);

 


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

  /*Output to write to fill with Event ID instead of verbosing to 2 */

G4int eventID =
    G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
// Write output to file
std::ofstream outfile("DMEmission.txt", std::ios::app);

if (outfile.is_open())
{
    outfile << "Event = " << eventID
            << " | DM PDG ID = " << theDMParticlePtr->GetPDGEncoding()
            << " | emitted by = " << aTrack.GetDefinition()->GetParticleName()
            << " | material = " << mat->GetName()
            << " | Z = " << target.Z
            << " | A = " << target.A
            << " | incident energy = " << incidentE / GeV << " GeV"
            << " | XAcc = " << XAcc
            << " | DM energy = " << DME / GeV << " GeV"
            << std::endl;

    outfile.close();
}
else
{
    G4cerr << "ERROR: Could not open DMEmission.txt" << G4endl;}
 std::cout << "DM PDG ID = " << theDMParticlePtr->GetPDGEncoding()
            << " emitted by " << aTrack.GetDefinition()->GetParticleName()
            << " in material " << mat->GetName() << " (Z=" << target.Z << ", A=" << target.A << ")"
            << " with energy = " << incidentE/GeV << " DM energy = " << DME/GeV << std::endl;
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

std::vector<DMProcessPrimakoffALP::TargetElementData>
DMProcessPrimakoffALP::BuildTargetTable(
        const G4Material* mat,
        G4double ekin) const
{
    std::vector<TargetElementData> table;


    const G4ElementVector* elements =
        mat->GetElementVector();

    const G4double* numberDensity =
        mat->GetVecNbOfAtomsPerVolume();


    for(size_t i=0; i<elements->size(); i++)
    {
        TargetElementData t;

        t.Z = (*elements)[i]->GetZ();
        t.A = (*elements)[i]->GetN();

        t.numberDensity = numberDensity[i];

        t.sigma =
            myDarkMatter->TotalCrossSectionCalcPrimakoff(
                ekin,
                t.A,
                t.Z);


        t.rate = t.numberDensity*t.sigma;

        // DEBUG
        G4cout
        << "ekin = " << ekin
        << ", Z = " << t.Z
        << ", A = " << t.A
        << ", density = " << t.numberDensity
        << ", sigma = " << t.sigma
        << ", rate = " << t.rate
        << G4endl;



        table.push_back(t);
    }


    return table;
}


const DMProcessPrimakoffALP::TargetElementData&
DMProcessPrimakoffALP::SampleTarget(
    const std::vector<TargetElementData>& table) const
{

    G4double totalRate = 0.0;

    for(const auto& t : table)
    {
        totalRate += t.rate;
    }


    // random number between 0 and total rate
    G4double random =
        G4UniformRand()*totalRate;


    G4double cumulative = 0.0;


    for(const auto& t : table)
    {
        cumulative += t.rate;

        if(random < cumulative)
        {
            return t;
        }
    }


    // safety fallback
    return table.back();
}