#include "PointPrimaryGenerator.h"
#include "PrimaryGenerator.h" // For GetTransform
#include "EnergyGenerator.h" 
#include "TransformCoordinates.h"
#include "Options.h" // in util

// ROOT includes
#include "TVector3.h"

// C++ includes
#include <vector>
#include <memory>
#include <iostream>

namespace gramssky {

  PointPrimaryGenerator::PointPrimaryGenerator()
    : PrimaryGenerator()
  {
    // Get the point from the Options XML file.
    auto options = util::Options::GetInstance();

    std::vector<double> pointSource;
    options->GetOption("PointSource",pointSource);
    m_point.SetXYZ(pointSource[0],pointSource[1],pointSource[2]);

    options->GetOption("PrimaryPDG",m_PDG);
  }

  PointPrimaryGenerator::~PointPrimaryGenerator()
  {}

  std::shared_ptr<ParticleInfo> PointPrimaryGenerator::Generate()
  {
    // Create a new particle.
    auto particle = std::make_shared<ParticleInfo>();

    // Use the values from the Options XML file.
    particle->SetX( m_point.x() );
    particle->SetY( m_point.y() );
    particle->SetZ( m_point.z() );
    particle->SetPDG(m_PDG);

    // GetEnergyGenerator is defined in PrimaryGenerator.h
    double energy = GetEnergyGenerator()->Generate();
    particle->SetE(energy);

    // We don't have to set correct particle direction here, since the
    // Transform routine will rotate the momentum vector to point in
    // the direction of the detector's center. But we do want to get
    // the magnitude right. Note that we're assuming that the particle
    // is massless.
    particle->SetPz(-energy);

    // Transform the particle position and direction from the
    // celestial sphere to the detector coordinates, applying a random
    // shift to simulate a flux from infinity.
    auto transformed = GetTransform()->Transform( particle );

    return transformed;
  }

} // namespace gramssky
