#pragma once

#include "DarkMatter.hh"

/*Update 27 June by*/
/* 

1] Removal of SigmNormIn



*/

class ALP : public DarkMatter
{

  public:

    ALP(double MAIn, double EThreshIn, double ANuclIn, double ZNuclIn, double DensityIn);

    virtual ~ALP();

    virtual double TotalCrossSectionCalc(double E0);
    virtual double GetSigmaTot(double E0);
    virtual double CrossSectionDSDX(double Xev, double E0);
    virtual double CrossSectionDSDXDU(double Xev, double UThetaEv, double E0);
    virtual double CrossSectionDSDTheta(double ThetaEv, double E0);
    virtual double CrossSectionDSDThetaMAX(double E0);
    virtual double Width();



    // Primakoff overloads — receive A and Z from Geant4 at runtime
    virtual double TotalCrossSectionCalcPrimakoff(double E0, double A, double Z) override;
    virtual double CrossSectionDSDThetaPrimakoff(double ThetaEv, double E0, double A, double Z) override;
    virtual double CrossSectionDSDThetaMAXPrimakoff(double E0, double A, double Z) override;
    
  private:

};
