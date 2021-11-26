/// \file persistency/gdml/GramsG4/src/GramsG4GPSGeneratorAction.cc
/// \brief Implementation of the GramsG4GPSGeneratorAction class
//
// 25-May-2020 WGS: Set some preliminary values for testing GRAMS.
//

#include "GramsG4GPSGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

namespace gramsg4 {

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  GPSGeneratorAction::GPSGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
    , m_ParticleSource(nullptr)
  {
    m_ParticleSource = new G4GeneralParticleSource();
    // Just in case: we only want to generate one particle at a time.
    m_ParticleSource->SetNumberOfParticles(1);
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  GPSGeneratorAction::~GPSGeneratorAction()
  {
    delete m_ParticleSource;
  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void GPSGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    // There's nothing to do here; the particles are generated by
    // commands from the Geant4 macro file read by the main program.
    m_ParticleSource->GeneratePrimaryVertex(anEvent);
  }

} // namespace gramsg4