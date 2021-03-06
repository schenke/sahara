
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/program_options.hpp>
#include <boost/regex.hpp>


#include "Configuration.h"

using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;
using std::stringstream;
using std::vector;

/**
 * The only constructor provided for this class. Defaults for all parameters are set in case there is
 * no input file or it cannot be read.
 */
config::config() :
 // ----  options ----
 T(0.4),  
 pTRefMin(1.),
 pTRefMax(8.),
 Testparticles(1),
 minParticleNumber(4),
 maxParticleNumber(500),
 kappa(1.),
 UnitlessFactor(1.),
 // ---- output options ----
  standardOutputDirectoryName("output"),
 //---- program_options groups ----
  usage_information("usage information"),
  hidden_options("Hidden options"),
  general_options("General options"),
  simulation_parameters("Parameters for the BAMPS simulation"),
  output_options("Options for the output generated by the BAMPS simulation"),
  // ---- general options ----
  jobName("DefaultExample"),
  particlesFile("ExampleParticleList.txt")
{
  // populate the program_options::options_description groups with names and types of the possible parameters
  // this can add to the options_description groups provided by the base class
  initializeProgramOptions();
}

/**
 * This provides the interface for the actual reading and processing of program options. It needs to be explicitly called
 * after an instance of the configPrototype class has been created.
 * 
 * @param[in] argc number of command line arguments, passed from the calling process
 * @param[in] argv[] command line arguments, passed from the calling processes
 */
void config::readAndProcessProgramOptions ( const int argc, char* argv[] )
{
  
  groupProgramOptions();
  
  readProgramOptions(argc, argv);
  
  processProgramOptions();
  
  checkOptionsForSanity();
  
  printUsedConfigurationParameters();
}

/**
 * This is the place where post-processing of values that have been stored in the boost::program_options::variables_map
 * should go. For example conversion from integer to enum types etc.
 */
void config::processProgramOptions()
{
  boost::filesystem::path outputDirectory( standardOutputDirectoryName );
  checkAndCreateOutputDirectory( outputDirectory );
  

//  if( !( vm["simulation.fixed_Coupling"].defaulted() || vm["simulation.fixed_Coupling"].empty() ) )
//  {
//    fixedCouplingValue_set = true;
//  }
  
}

/**
 * This routine is provided in case future implementations should contain sanity checks for the given values of parameters and
 * options.
 */
void config::checkOptionsForSanity()
{
  // sanity checks of parameters and options can go here  
  
}
/**
 * This routine initializes the boost::program_options::options_description objects that give structure to the handling of
 * user provided input.
 * By using a name pattern <group_name>.<option_name> the options can be given in a configuration file in INI format using 
 * group names such as
 * [group_name]
 * option_name1 = value1
 * option_name2 = value2
 */
void config::initializeProgramOptions()
{
  // Define some option groups. This gives more structure to the code and also structures the help message that is displayed
  // to the user.
  
  // Usage information. This is meant for the command line.
  usage_information.add_options()
  ("help,h", "print help message")
  ("detailed-help,d", "print detailed information on all options and parameters");
  
  
  // Read the name of the configuration file (from the command line)
  hidden_options.add_options()
  ("config-file,c", boost::program_options::value< string >(), "configuration file");
  
  // Make "config-file" a positional option, i.e. it can be used without explicitly specifying "--config-file=blabla.txt"
  pos_options.add("config-file", 1);
  
  // Group some general options  
  general_options.add_options()
  ("general.jobname", boost::program_options::value< string >( &jobName )->default_value( jobName ), "name of the simulation job, assigned to output files")
  ("general.particlesfile", boost::program_options::value< string >( &particlesFile )->default_value( particlesFile ), "name of the file withe the particles")
  ("general.pTRefMax", boost::program_options::value<double>( &pTRefMax )->default_value( pTRefMax ), "pTRefMax [GeV]")
  ("general.pTRefMin", boost::program_options::value<double>( &pTRefMin )->default_value( pTRefMin ), "pTRefMin [GeV]")
  ("general.minParticleInCluster", boost::program_options::value<int>( &minParticleNumber )->default_value( minParticleNumber ), "Minimum Number of Particles per Cluster")
  ("general.maxParticleInCluster", boost::program_options::value<int>( &maxParticleNumber )->default_value( maxParticleNumber ), "Maximum Number of Particles per Cluster")
  ("general.stringtension", boost::program_options::value<double>( &kappa )->default_value( kappa ), "stringtension kappa [GeV/fm]")
  ("general.unitlessFactor", boost::program_options::value<double>( &UnitlessFactor )->default_value( UnitlessFactor ), "Unitless factor ")
  ("general.temperature", boost::program_options::value<double>( &T )->default_value( T ), "temperature [GeV]")
  ("general.testparticles", boost::program_options::value<int>( &Testparticles )->default_value( Testparticles ), "Testparticles")
  ;

  
  // Group some simulation parameters
  simulation_parameters.add_options()
  ;

  // Group some options related to the program output  
  output_options.add_options()
  ("output.directory", boost::program_options::value<string>( &standardOutputDirectoryName )->default_value( standardOutputDirectoryName ), "directory to which general output should be written")
  ;
 ;


 }
 
/**
 * This routine groups the options_description groups into categories that can control which parameters are accessible via
 * the command line or the configuration file, which parameters are visible when requesting detailed help messages etc.
 */
void config::groupProgramOptions()
{ 
  // Group options that are meant to be provided via the command line
  command_line_options.add(usage_information).add(general_options).add(output_options).add(hidden_options);
  
  // Group options that are meant to be provided via a configuration file
  config_file_options.add(general_options).add(simulation_parameters).add(output_options).add(hidden_options);
  
  // Group options that are to be printed in a detailed help message
  //visible_options.add(usage_information).add(general_options).add(simulation_parameters).add(output_options).add(misc_options).add(hydroAnalysis_options).add(crossSection_options);
 
  // Add some groups that are meant to be provided via a configuration file
  //config_file_options.add(box_options);
  
  // Add option groups that are to be printed in a detailed help message
  //visible_options.add(box_options);
  
  
  
  
  
}

/**
 * This routine actually reads the parameters from the command line and the configuration file and stores them into 
 * the variable map. 
 */
void config::readProgramOptions( const int argc, char* argv[] )
{ 
  // parse the command line arguments
  //cout << command_line_options << endl;
  boost::program_options::store( boost::program_options::command_line_parser(argc, argv).options(command_line_options).positional(pos_options).run(), vm );
  boost::program_options::notify(vm); // needed to actually populate the map "vm" with all the argument values

  // check whether help messages are requested and print them
  if (vm.count("help"))
  {
    cout << "Usage: cascade [options] [configuration file]" << endl;
    cout << usage_information << "\n";
    throw( HELP_MESSAGE_REQUESTED );
  }
  if (vm.count("detailed-help"))
  {
    cout << "Usage: cascade [options] [configuration file]" << endl;
    cout << visible_options << "\n";
    throw( HELP_MESSAGE_REQUESTED );
  }
  
  // check whether a configuration file name has been provided and parse this file if applicable
  if ( vm.count("config-file") )
  {
    boost::filesystem::path configFileName( vm["config-file"].as< string >() );
    if ( boost::filesystem::exists( configFileName ) )
    {
      cout << "Using configuration-file: " << vm["config-file"].as< string >() << endl;
      boost::filesystem::ifstream configFile( configFileName );
      boost::program_options::store( boost::program_options::parse_config_file(configFile, config_file_options), vm);
      boost::program_options::notify(vm);
    }
    else
    {
      string errMsg = "Configuration file \"" + vm["config-file"].as< string >() + "\" could not be found.";
      throw eConfig_error( errMsg );
    }
  }
  else
  {
    // read a dummmy (i.e. non-existing) configuration file in case no real configuration file has been specified
    // this way the variable map vm gets to know about the options in the group config_file_options and the (default) 
    // values of the parameters can still be printed via configBase::printOptionsDescriptionToIniFormat
    boost::filesystem::ifstream configFile( "" );
    boost::program_options::store( boost::program_options::parse_config_file(configFile, config_file_options), vm);
  }
}

/**
 * This routine prints a given boost::program_options::options_description group to INI format that is used for configuration
 * files. The output is written to a given file stream.
 * 
 * @param[in] _desc The options_description group to be printed
 * @param[in] _file The file stream object to which the output is to be printed
 */
void config::printOptionsDescriptionToIniFormat( const boost::program_options::options_description& _desc, boost::filesystem::ofstream& _file )
{
  boost::regex re( "(\\w+)(\\.)(\\w+)" );
  boost::smatch matches;
  
  if ( _desc.options().size() > 0 )
  {
    boost::regex_match(_desc.options()[0]->long_name(),matches,re);
    _file << "[" << matches[1] << "]" << endl;
    
    for ( unsigned int i = 0; i < _desc.options().size(); i++ )
    {
      if ( boost::regex_match(_desc.options()[i]->long_name(),matches,re) )
      {
        _file << matches[3] << " = " << convertBoostAnyToString( vm[matches[0]] ) << endl;
      }
    }
    _file << endl;
  }
}
/**
 * boost::program_options internally stores the parameter values into boost::any datatypes that cannot be printed directly.
 * Therefore this routine provides explicit conversion of the values stored in boost::program_options::variable_value objects
 * to strings.
 * NOTE Unfortunately, due to limitations of boost::any, there is no generic way to do this. Therefore every datatype that occurs
 * in the program options needs to be explicitly checked here.
 * 
 * @param[in] _arg The boost::program_options::variable_value object whose value should be converted to string
 */
std::string config::convertBoostAnyToString ( const boost::program_options::variable_value& _arg )
{
  std::string result;
  if ( _arg.value().type() == typeid( int ) || _arg.value().type() == typeid( unsigned int ) )
  {
    result = boost::lexical_cast<std::string,int>( _arg.as<int>() );
  }
  else if ( _arg.value().type() == typeid( long ) )
  {
    result = boost::lexical_cast<std::string,long>( _arg.as<long>() );
  }
  else if ( _arg.value().type() == typeid( unsigned short int ) )
  {
    result = boost::lexical_cast<std::string,unsigned short int>( _arg.as<unsigned short int>() );
  }
  else if ( _arg.value().type() == typeid( double ) )
  {
    result = boost::lexical_cast<std::string,double>( _arg.as<double>() );
  }
  else if ( _arg.value().type() == typeid( bool ) )
  {
    result = boost::lexical_cast<std::string,int>( static_cast<int>(_arg.as<bool>()) );  
  }
  else if ( _arg.value().type() == typeid( std::string ) )
  {
    result = _arg.as<std::string>();
  }
  else
  {
    std::string errMsg("configPrototype::convertBoostAnyToString: Unknown type, conversion failed.");
    throw eConfig_error( errMsg );
  }
  
  return result;
}

/**
 * Print a file in INI format with all current option values. This file could be used a configuration filename
 * in a subsequent run to reproduce the very same option settings.
 */
void config::printUsedConfigurationParameters()
{
  string filename = standardOutputDirectoryName + "/" + jobName + "_used_configuration";
  boost::filesystem::path outputFile( filename );
  boost::filesystem::ofstream output( outputFile, ios::trunc );
  
  /**
  printOptionsDescriptionToIniFormat( general_options, output );
  printOptionsDescriptionToIniFormat( simulation_parameters, output );
  printOptionsDescriptionToIniFormat( output_options, output );
  */
}










/**
 * Return the temperature as a string object. Specified in MeV.
 */
string config::getString_T() const
{
  return boost::lexical_cast<std::string>( int(T*1000 + 0.001) );
}

// kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; 
