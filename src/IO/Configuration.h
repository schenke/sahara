
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#define HELP_MESSAGE_REQUESTED 1

using std::string;

/** @brief This extends the namespace ns_casc (see globalsettings.h) to hold a global particle vector */
namespace ns_casc
{  
  /** @brief Vector to hold all particles in the simulation, GLOBAL 
  *
  * Declared as extern here. There MUST be ONE definition in a cpp-file that includes this header in the global 
  * scope (it's in configuration.cpp).
  * All files that include this header can then access the vector.
  */
  extern std::vector<Particle> particles;
  
  extern std::vector<Particle> hadrons;
    
}
//--------------------------------------------------------//


class config
{
public:
  /** @brief Constructor that internally reads the provided input file */
  config();
  /** @brief Standard constructor */
  ~config() {};
  
  /** @brief processes command line arguments and settings provided via a configuration file */
  void readAndProcessProgramOptions(const int argc, char* argv[]);
  
  /** -------- output options ------- */   
  /** @brief Interface for configPrototype::standardOutputDirectoryName */
  string getStandardOutputDirectoryName() const { return standardOutputDirectoryName; }
  /** ------------------------------- */

  
  /** ------ general options ------- */  
  /** @brief Interface for configPrototype::jobname */
  std::string getJobName() const { return jobName; }
  /** @brief Interface for configPrototype::jobname */
  std::string getParticlesFile() const { return particlesFile; }  
  /* -------- box options ------- */
  /** @brief interface to the (initial) temperature */
  double getTemperature() const {return T;}
  
  /** @brief auxiliary function, return the temperature as a string object */
  std::string getString_T() const;
  
  int getTestparticles() const {return Testparticles;}
  double getPTRefMin() const {return pTRefMin;}
  double getPTRefMax() const {return pTRefMax;}
  int getMinParticleNumber() const {return minParticleNumber;}
  int getMaxParticleNumber() const {return maxParticleNumber;}
  
  double getUnitlessFactor() const {return UnitlessFactor;}
  double getStringtension() const {return kappa;}
  
  protected:  
    
   /** ----- auxiliary routines ----- */
   /** @brief Sort the options into groups */   
   void groupProgramOptions();
   
   /** @brief Actually define all the options */
   void initializeProgramOptions();
   
   /** @brief Read command line arguments and settings provided via a configuration file and via the commmand line */
   void readProgramOptions(const int argc, char* argv[]);
   
   /** @brief Do some processing on program options that have previously been read via configPrototype::readProgramOptions */
   void processProgramOptions();
   
   /** @brief Print a complete configuration file using all current parameter values */
   void printUsedConfigurationParameters();
   
   /** @brief Convert the option descriptions into the INI format of configuration files */
   void printOptionsDescriptionToIniFormat( const boost::program_options::options_description& _desc, boost::filesystem::ofstream& _file );
   
   /** @brief Explicitly convert boost::any data as used in boost::program_options to strings */
   std::string convertBoostAnyToString( const boost::program_options::variable_value& _arg );
  
  /** @brief Do some checks on user-provided options and parameters */
  void checkOptionsForSanity();
  
  /** @brief Create output directory if necessary */
  void checkAndCreateOutputDirectory( boost::filesystem::path& _dir );
  
  
  /** ------ boost::program_options objects ------- */ 
  /** @brief Category that groups all options that can be provided via the command line */
  boost::program_options::options_description command_line_options;
  /** @brief Category that groups all options that can be provided via the configuration file */
  boost::program_options::options_description config_file_options;
  /** @brief Category that groups all options that are visible in a detailed help message */
  boost::program_options::options_description visible_options;
  
  /** @brief Positional option used for the configuration file name
   * 
   * Positional options are options that can be given (on the command line) without specifying a parameter name, e.g.
   * the name of the configuration file can be given as ./my_executable inputfile instead of ./my_executable --config-file=inputfile
   */
  boost::program_options::positional_options_description pos_options;

  /** @brief Variable map that stores all parameter values after command line and configuration file have been parsed */
  boost::program_options::variables_map vm;

  /** @brief Group options that provide usage information */
  boost::program_options::options_description usage_information;
  /** @brief Group options that are "hidden" from detailed help messages */
  boost::program_options::options_description hidden_options;
  
  /** @brief Group general options */
  boost::program_options::options_description general_options;
  /** @brief Group simulation parameters */
  boost::program_options::options_description simulation_parameters;
  /** @brief Group output options */
  boost::program_options::options_description output_options;

  /** -------- output options ------- */ 
  /** @brief Directory to which general output should be written */
  string standardOutputDirectoryName;
  
  /** @brief Directory to which general output should be written, which is always local */
  string localOutputDirectoryName;  
  /** ------------------------------- */
  
  
  /** ------ general options ------- */  
  /** @brief The name of the current job - assigned to output files */
  string jobName;
  
  /** @brief The name of the file with the particles */
  string particlesFile;
  /** ------------------------------ */

  double T,kappa,UnitlessFactor;
  int Testparticles;
  double pTRefMin,pTRefMax;
  int minParticleNumber,maxParticleNumber;
};

/**
 * This routine checks whether the requested output directory exists and creates it in case it does not. 
 */
inline void config::checkAndCreateOutputDirectory(boost::filesystem::path& _dir)
{
  if ( boost::filesystem::exists( _dir ) )
  {
    if ( boost::filesystem::is_directory( _dir ) )
    {
      return;
    }
    else // this handles the rare case where a file (!) with the requested name exists
    {
      boost::filesystem::path renamePath( _dir.string() + ".backup" );
      std::cout << "File with name " << _dir.string() << " blocks the creation of an output folder for offline reconstruction." << std::endl;
      std::cout << "It is renamed to " << renamePath.string() << std::endl;
      boost::filesystem::rename( _dir, renamePath );
      boost::filesystem::create_directory( _dir );       
    }
  }
  else
  {
//     std::cout << "Creating output folder: " << _dir.string() << std::endl;
    boost::filesystem::create_directory( _dir );    
  }
}


/** @brief exception class for handling unexpected critical behaviour within the configuration of the BAMPS run  */
class eConfig_error : public std::runtime_error
{
  public:
    explicit eConfig_error(const std::string& what) : std::runtime_error(what) {};
    
    virtual ~eConfig_error() throw() {};
};















#endif
