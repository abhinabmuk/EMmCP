#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class G4ParticleDefinition;
class G4Material;

class DetectorConstruction;
class PrimaryGeneratorAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction();

  public:
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void CountProcesses(G4String procName);

    void TrackLength (G4double step);

    void EnergyDeposited (G4double edepPrim, G4double edepSecond);

    void EnergyTransferedByProcess (G4String procName, G4double energy);

    void EnergyTransfered (G4double energy);

    void TotalEnergyLost (G4double energy);

    void EnergyBalance (G4double energy);

    void TotalEnergyDeposit (G4double energy);

    void EnergySpectrumOfSecondaries (G4String particleName, G4double ekin);

    void CountBremsTotal();
    void CountBremsNonZero();

  public:
    G4double GetEnergyFromRestrictedRange
             (G4double,G4ParticleDefinition*,G4Material*,G4double);
                       
    G4double GetEnergyFromCSDARange
             (G4double,G4ParticleDefinition*,G4Material*,G4double);

  private:

    struct MinMaxData {
      MinMaxData()
        : fCount(0),
          fVsum(0.0),
          fVsum2(0.0),
          fVmin(0.0),
          fVmax(0.0) {}

      MinMaxData(G4int count, G4double vsum, G4double vsum2,
                 G4double vmin, G4double vmax)
        : fCount(count),
          fVsum(vsum),
          fVsum2(vsum2),
          fVmin(vmin),
          fVmax(vmax) {}

      G4int    fCount;
      G4double fVsum;
      G4double fVsum2;
      G4double fVmin;
      G4double fVmax;
    };

  private:

    DetectorConstruction*   fDetector;
    PrimaryGeneratorAction* fPrimary;
    HistoManager*           fHistoManager;

    std::map<G4String,G4int>  fProcCounter;

    G4long   fNbSteps;
    G4double fTrackLength, fStepMin, fStepMax;

    G4double fEdepPrimary, fEdepPrimMin, fEdepPrimMax;
    std::map<G4String,MinMaxData> fEtransfByProcess;

    G4double fEnergyTransfered, fEtransfMin, fEtransfMax;
    G4double fEnergyLost, fElostMin, fElostMax;
    G4double fEnergyBalance, fEbalMin, fEbalMax;

    G4double fEdepSecondary, fEdepSecMin, fEdepSecMax;
    G4double fEdepTotal, fEdepTotMin, fEdepTotMax;

    G4int fNonZeroEdepEvents;

    std::map<G4String,MinMaxData> fEkinOfSecondaries;

    // NEW: event-level process tracking
    std::map<G4String, G4int>  fNonZeroEdepPerProcess;
    std::map<G4String, G4bool> fProcessSeenThisEvent;



    G4int fBremsTotal = 0;
    G4int fBremsNonZero = 0;
};

#endif