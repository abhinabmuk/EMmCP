#include "DMParticleALP.hh"
//#include "DarkMatterParametersRegistry.hh"

#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"
#include "G4DecayTable.hh"

#include "G4SystemOfUnits.hh"


DMParticleALP * DMParticleALP::theInstance = nullptr;

DMParticleALP* DMParticleALP::Definition()
{
  if( theInstance ) {
    return theInstance;
  }
  //get parameters from registry (NOTE: mass is parsed in GeV)
  //DarkMatterParametersRegistry* DMpar = DarkMatterParametersRegistry::GetInstance();
  G4double MassIn    = 0.0167; //in GeV
  G4double epsilIn   = 0.001;  // 1/GeV coupling constant
  G4double DecayType = 1; //always set to 0 , ALP always decays 

  G4String name = "DMParticleDarkALP";
  // search in particle table]
  G4ParticleTable * pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition * anInstance = pTable->FindParticle(name);

  G4bool isStable = DecayType > 0 ? false : true;
  G4int IDPDG = 5300022; // https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf
  G4double WidthIn = 0.;
  if(!isStable) {
  WidthIn = 1./(64.*pi)*MassIn*MassIn*MassIn*epsilIn*epsilIn;   //previous code with unit conversion 

 // WidthIn = (MassIn/GeV)*(MassIn/GeV)*(MassIn/GeV)*epsilIn*epsilIn*1./(64.*pi);
    IDPDG = 5300122;
    name = "DMParticleALP";
  }
  if( !anInstance ) {
    anInstance = new G4ParticleDefinition(
        /* Name ..................... */ name,
        /* Mass ..................... */ MassIn*GeV,
        /* Decay width .............. */ WidthIn*GeV,
        /* Decay width .............. */ // WidthIn*GeV,

        /* Charge ................... */ 0.,
        /* 2*spin ................... */ 0,
        /* parity ................... */ -1,
        /* C-conjugation ............ */ 0,
        /* 2*Isospin ................ */ 0,
        /* 2*Isospin3 ............... */ 0,
        /* G-parity ................. */ 0,
        /* type ..................... */ "boson",
        /* lepton number ............ */ 0,
        /* baryon number ............ */ 0,
        /* PDG encoding ............. */ IDPDG,
        /* stable ................... */ isStable,
        /* lifetime.................. */ 0,
        /* decay table .............. */ NULL,
        /* shortlived ............... */ false,
        /* subType .................. */ "DMParticleALP",
        /* anti particle encoding ... */ IDPDG
          );

    if(!isStable)
    {
      // Life time is given from width
      ((DMParticle*)anInstance)->CalculateLifeTime();

      //create Decay Table
      G4DecayTable* table = new G4DecayTable();

      // create a decay channel
      G4VDecayChannel* mode;
      // ALP -> gamma + gamma
      mode = new G4PhaseSpaceDecayChannel(name, 1., 2, "gamma", "gamma");
      table->Insert(mode);

      anInstance->SetDecayTable(table);
    }
  }
  theInstance = reinterpret_cast<DMParticleALP*>(anInstance);
  
  // Added these debug lines:
  G4cout << "========== ALP Particle Info ==========" << G4endl;
  G4cout << "Name: " << theInstance->GetParticleName() << G4endl;
  G4cout << "Mass: " << theInstance->GetPDGMass()/GeV << " GeV" << G4endl;
  G4cout << "Mass Inputted in decay width: " << MassIn << G4endl;
  G4cout << "Epsilon: " << epsilIn << " 1/GeV" << G4endl;
  G4cout << "Width: " << theInstance->GetPDGWidth()/GeV << " GeV" << G4endl;
  G4cout << "Width calculated: " << WidthIn << " GeV" << G4endl;
  G4cout << "Lifetime: " << theInstance->GetPDGLifeTime()/s << " s" << G4endl;
  G4cout << "cτ (decay length): " << theInstance->GetPDGLifeTime() * c_light / m << " m" << G4endl;
  G4cout << "Is Stable: " << theInstance->GetPDGStable() << G4endl;
  G4cout << "=======================================" << G4endl;




  return theInstance;
}
