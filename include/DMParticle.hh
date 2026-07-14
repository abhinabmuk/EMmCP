#pragma once

#include <G4ParticleDefinition.hh>

#include "ALP.hh"   // add at top


class DMParticle : public G4ParticleDefinition {
  private:
    DMParticle();
    ~DMParticle();
  public:
    inline void CalculateLifeTime() {SetPDGLifeTime(CLHEP::hbar_Planck/GetPDGWidth());}
    inline void SetLongLived() {SetPDGLifeTime(1000.);}
};
