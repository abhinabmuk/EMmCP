#pragma once

#include <G4VDiscreteProcess.hh>

class DarkMatter;
class G4ParticleDefinition;


// added on Jun 27th by Abhinab Mukhopadhyay 
// purpose for prasing material properties needs these libraries 
class G4Material;
class G4Element;


class DMProcessPrimakoffALP : public G4VDiscreteProcess
{
  public:

    DMProcessPrimakoffALP(DarkMatter* DarkMatterPointer, G4ParticleDefinition* theDMParticlePtrIn);

    // Implements final state parameters when the process won.
    virtual G4VParticleChange* PostStepDoIt( const G4Track &, const G4Step & ) override;

    virtual G4double GetMeanFreePath( const G4Track & aTrack,
                                      G4double previousStepSize,
                                      G4ForceCondition * condition ) override;

    virtual G4bool IsApplicable(const G4ParticleDefinition &) override;

  private:

    /*Selects the highest-Z element in a (possibly compound) material,
     since the Primakoff cross section scales as Z^2 and is dominated
     by the heaviest element present. */ 

    /* This is temporary need to do for non-homogenous material*/
    const G4Element* GetDominantElement(const G4Material* mat) const;


    DarkMatter* myDarkMatter;
    G4ParticleDefinition* theDMParticlePtr;
};
