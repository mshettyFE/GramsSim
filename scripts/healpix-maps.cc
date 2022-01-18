// healpix-maps.cc
// 18-Jan-2022 WGS
//
// A test to help understand how to load healpix maps, derived from
// AllSkyPrimaryGen by Hiroki Yoneda and/or Naomi Tsuji. 
//

#include <healpix_cxx/healpix_base.h>
#include <healpix_cxx/healpix_map.h>
#include <healpix_cxx/healpix_map_fitsio.h>
#include <healpix_cxx/fitshandle.h>

#include <string>
#include <iostream>
#include <memory>
#include <vector>

int main( int argc, char** argv ) {

  // Check if the filename argument exists.
  if ( argc < 1 ) {
    std::cerr << "Require a .fits filename as argument"
	      << std::endl;
    exit(EXIT_FAILURE);
  }

  // In the actual GramsSky implementation this will come in via
  // Options.
  std::string fitsFileName(argv[1]);

  // HDU = "Header Data Unit". For more on what an HDU is, see
  // https://heasarc.gsfc.nasa.gov/docs/software/fitsio/user_f/node17.html
  // I found this value experimentally, and it probably only applies to file
  // AliceSprings_Australia_2021_3_21_alt30000m_map_photon.fits
  int hduNumber(2);

  // Open the fits file.
  auto input = std::make_shared<fitshandle>();
  input->open(fitsFileName);
  input->goto_hdu(hduNumber);

  // Get the number of maps in this header.
  int numberMaps;
  input->get_key("NMAP", numberMaps);

  std::vector< Healpix_Map<double> > images; // normalization map
  std::vector<double> energies;

  // Read all the maps in the file.
  double energy;
  for ( int map = 0; map < numberMaps; ++map ) {
    std::string key("ENE"+std::to_string(map+1));
    input->get_key(key, energy);

    std::cout << "key= '" << key 
	      << "' energy=" << energy
	      << std::endl;

    Healpix_Map<double> image;

    // Get an image map from the file. (The goto_hdu is probably
    // unnecessary, but I'm copying code from different sources and I
    // want to be sure.)
    input->goto_hdu(hduNumber);
    read_Healpix_map_from_fits( *input, image, map+1 ) ;

    // Save the images and its associated energy.
    images.push_back(image);
    energies.push_back(energy);
  }

  // In this sample code, numberMaps and numberImages will be the
  // same. In GramsSky, there will be energyMin and energyMax
  // parameters that might cut down on the number of images in these
  // vectors.
  auto numberImages = energies.size();
  std::cout << "number of maps read = " << numberImages << std::endl;
  if ( numberImages < 2 ) {
    std::cerr << "Need at least 2 maps to interpolate between energies" 
	      << std::endl;
    exit(EXIT_FAILURE);
  }

  auto npix = images[0].Npix();
  auto nside = images[0].Nside();

  std::cout << "npix = " << npix 
	    << "  nside = " 
	    << nside << std::endl;
}
