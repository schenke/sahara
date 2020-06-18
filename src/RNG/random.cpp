
#include <iostream>
#include <fstream>
#include <time.h>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include "random.h"

using std::cout;
using std::endl;


/** 
 * @brief definition of global RNG object, defined extern in random.h 
 **/
#if (USE_RNG == 1)
#if _OPENMP
randomNumberGeneratorBoost_OpenMP UniformRandomGenerator;
#else
randomNumberGeneratorBoost UniformRandomGenerator;
#endif
#elif (USE_RNG == 2)
#if _OPENMP
#error "USE_RNG == 2 && _OPENMP is not implemented yet."
#else
randomNumberGeneratordSFMT UniformRandomGenerator;
#endif
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/**
 * This routine computes and sets a (hopefully) unique seed from either
 * /dev/urandom provided by the system or the system time as a
 * fallback. 
 * Using /dev/urandom is prefered and should work on all *nix
 * system. /dev/urandom is a special device that holds (pseudo) random
 * bits generated from user input, network traffic and black
 * voodoo. Use /dev/random for even more unpredictable voodoo. 
 *
 * @return The computed seed.
 */
uint32_t randomNumberGenerator::findSeed()
{
  uint32_t s;
  
  // open /dev/urandom as an ifstream
  std::ifstream urand("/dev/urandom", std::ios::binary );
  
  // get the seed from /dev/urandom, fall back to a seed generated from the current time if /dev/urandom could not have been opened.
  if ( urand.is_open() && urand.good() )
  {
    // read random characters from the input stream assigned to /dev/urandom
    // for this the integer type s is reinterpreted as a character pointer and sizeof(unit32_t) characters are read into s
    urand.read( reinterpret_cast<char *>(&s), sizeof(uint32_t) );
  }
  else
  {
    s = static_cast<uint32_t>( time(NULL) );
  }
  
  // return the seed for the user to have fun with
  return s;
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


using std::cout;
using std::endl;


/** 
 * @brief definition of global RNG object, defined extern in randomGauss.h 
 **/
randomGaussGenerator GaussRandomGenerator;


/**
* This routine computes and sets a (hopefully) unique seed from either
* /dev/urandom provided by the system or the system time as a
* fallback. 
* Using /dev/urandom is prefered and should work on all *nix
* system. /dev/urandom is a special device that holds (pseudo) random
* bits generated from user input, network traffic and black
* voodoo. Use /dev/random for even more unpredictable voodoo. 
*
* @return The computed seed.
*/
uint32_t randomGaussGenerator::setSeed()
{
  uint32_t s;
  
  // open /dev/urandom as an ifstream
  std::ifstream urand("/dev/urandom", std::ios::binary );
  
  // get the seed from /dev/urandom, fall back to a seed generated from the current time if /dev/urandom could not have been opened.
  if ( urand.is_open() && urand.good() )
  {
    // read random characters from the input stream assigned to /dev/urandom
    // for this the integer type s is reinterpreted as a character pointer and sizeof(unit32_t) characters are read into s
    urand.read( reinterpret_cast<char *>(&s), sizeof(uint32_t) );
  }
  else
  {
    s = static_cast<uint32_t>( time(NULL) );
  }
  
  // use s obtained from /dev/urandom to seed the Boost RNG
  rng_mt_boost.seed( s );
  
  // return the seed for the user to have fun with
  return s;
}

//Mean and Standard Deviation=width
double randomGaussGenerator::getRan(double mean, double width)
{

  boost::normal_distribution<> nd( mean, width );
  boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor( rng_mt_boost, nd);
  
  return var_nor();
}
