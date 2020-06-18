#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <sys/types.h> 
#include <sys/stat.h> 

// DEFINITION OF LORENTZ VECTORS AND TRANSFORMATIONS //
#include "LORENTZ/Vector4D.cpp"
#include "LORENTZ/lorentz.cpp"

// DEFINITION OF PARTICLES //
#include "Particle.cpp"

// GLOBAL LISTS OF PARTONS AND HADRONS //
std::vector<Particle> GlobalPartonList;
std::vector<Particle> GlobalHadronList;

// DEFINITIONS OF CLUSTERS //
#include "Cluster.cpp"

// GLOBAL CLUSTER CONFIGURATION //
ClusterConfiguration Clusters;

// CLUSTERING ALGORITHM OF SAHARA //
#include "ClusteringAlgorithm.cpp"

// HERWIG INTERFACE //
#include "HERWIGInterface.cpp"

// GENERATOR FOR SAMPLING THERMAL DISTRIBUTIONS //
#include "Generator.cpp"

// IP-GLASMA INTERFACE //
#include "IPGLASMAInterface.cpp"

// OSCAR INTERFACE //
#include "OSCARInterface.cpp"

// GET PARTICLE LIST AS INPUT //
void GetParticleList(std::string filename){
    
    std::cerr << "#Getting particle list from file " << filename << std::endl;
    std::fstream file( filename.c_str(), std::ios::in );
    
    bool setEnergyMassless =true;
    
    if ( file )
    {
        std::string line;
        std::stringstream inStream;
        double content;
        
        do
        {
            std::getline( file, line );
            
            if ( line.find( "#", 0 ) == std::string::npos && line.length() != 0 )    //ignore empty lines and lines with leading "#"
            {
                if ( line.find( "-inf", 0 ) == std::string::npos )  //the string "-inf" can't be pushed into a vector<double>
                {
                    
                    Particle NewParton;
                    
                    inStream.str("");
                    inStream.clear();
                    
                    inStream.str(line);
                    inStream >> NewParton.Mom.Px() >> NewParton.Mom.Py() >> NewParton.Mom.Pz() >> NewParton.Mom.E() >> NewParton.Pos.X() >> NewParton.Pos.Y() >> NewParton.Pos.Z() >> NewParton.Pos.T();
                    
                    if(setEnergyMassless){
                        NewParton.Mom.E() = sqrt(NewParton.Mom.vec2());
                    }
                    
                    
                    if ( !inStream.fail() )
                    {
                        GlobalPartonList.push_back(NewParton);
                    }
                    else
                    {
                        std::cerr << "#Critical error in loading particle list from file " << filename << std::endl;
                        exit(0);
                    }
                }
                else
                {
                    
                }
                
            }
        }
        while ( !file.eof() );
    }
    std::cerr << "#Succesfully loaded list of " << GlobalPartonList.size() << " GlobalPartonList from file " << filename << std::endl;
}

// CREATE OUTPUT OF PARTON LIST //
void OutputPartonList(std::string filename){
    
    std::ofstream OutStream; OutStream.open(filename,std::ofstream::out | std::ofstream::app);
    
    for(int i=0;i<GlobalPartonList.size();i++){
        OutStream << GlobalPartonList.at(i).Mom.Px() << " " << GlobalPartonList.at(i).Mom.Py() << " " << GlobalPartonList.at(i).Mom.Pz() << " " << GlobalPartonList.at(i).Mom.E() << " " << Particle::mapToPDG(GlobalPartonList.at(i).FLAVOR) << std::endl;
    }
    
    OutStream.close();
    
}

// CREATE OUTPUT OF CHARGED HADRON LIST //
void OutputChargeHadronList(std::string filename){
    
    std::ofstream OutStream; OutStream.open(filename,std::ofstream::out | std::ofstream::app);
    
    for(int i=0;i<GlobalHadronList.size();i++){
        
        int PDGSpecies=GlobalHadronList.at(i).PDGSpecies;
        
        // EXCLUDE GAMMA,PI0,RHO0,K0L,K0S
        if(PDGSpecies==22 || PDGSpecies==111 || PDGSpecies==113 || PDGSpecies==310 || PDGSpecies==130 || PDGSpecies==221){}
        else{
            OutStream << GlobalHadronList.at(i).Mom.Px() << " " << GlobalHadronList.at(i).Mom.Py() << " " << GlobalHadronList.at(i).Mom.Pz() << " " << GlobalHadronList.at(i).Mom.E() << " " << GlobalHadronList.at(i).PDGSpecies << std::endl;
        }
        
        
    }
    
    OutStream.close();
    
}

// COMMANDLINE ARGUMENTS //
#include "IO/cfile.c"

// MAIN ROUTINE //
int main ( int argc, char *argv[] ){
    
    // COMMANDLINE OUTPUT //
    std::cerr << "## Welcome to SAHARA - a Stand-Alone HAdronizeR Application! ##" << std::endl;
    
    // GET COMMANDLINE ARGUMENTS //
    Konfig CommandlineArguments(argc,argv);
    
    char EventClass[1024]="0.95MB1.05";
    CommandlineArguments.Getval("EventClass",EventClass);

    int EventID=1511395746;
    CommandlineArguments.Getval("EventID",EventID);
    
    int NumberOfSamplings=64;
    CommandlineArguments.Getval("Samples",NumberOfSamplings);
    
    char OutputDirectory[1024]="OUTPUT";
    CommandlineArguments.Getval("o",OutputDirectory);
    
    char WorkingDirectory[1024]="TMP";
    CommandlineArguments.Getval("w",WorkingDirectory);
    
    // SET CLUSTERING PARAMETERS //
    CommandlineArguments.Getval("alpha",ClusteringParameters::alpha);
    CommandlineArguments.Getval("NMin",ClusteringParameters::MininalParticleNumberPerCluster);
    
    // SEED RNG //
    srand48(time(0));
 
    // GET SMEARED DISTRIBUTIONS FORM IP-GLASMA EVENT AND SETUP SAMPLING //

    IPGlasmaInterface::Setup(EventClass,EventID);
    
    std::stringstream outfolder_stream;
    outfolder_stream << "./OUTPUT/" << EventID;
    std::string outfolder_name = outfolder_stream.str();
	
    int check = mkdir(outfolder_name.c_str(),0777); 


    if (check){
      std::cout << "Folder ./OUTPUT/" << EventID << " created." << std::endl;
      }
    
    std::stringstream outparton_stream;
    std::stringstream outhadron_stream;
    outparton_stream << "./OUTPUT/" << EventID << "/gluons.txt";
    std::string outparton_name = outparton_stream.str();
    outhadron_stream << "./OUTPUT/" << EventID << "/OSCAR.dat";
    std::string outhadron_name = outhadron_stream.str();

    OSCARInterface::OSCARHeader(outhadron_name);
    
    for(int s=0; s<2; s++){
      cout << "_________ " << s << " ________" << endl;

        // SAMPLE GLUONS FROM IP-GLASMA EVENT //
        IPGlasmaInterface::Sample();
               
        OutputPartonList(outparton_name);
        
        // SETUP CLUSTERING ALGORITHM //
        ClusteringAlgorithm::Setup();
        
        // PERFORM CLUSTERING //
        ClusteringAlgorithm::Cluster("TMP/ClusteringLog.txt","TMP/ClusterConfigurationLog.txt",5000);
        
        // GENERATE LES HOUCHES FILES //
        Clusters.GenerateLesHouchesFile("TMP/Cluster");
        
        // EXCEUTE HERWIG AND GET HADRONS //
        HERWIGInterface::Execute("TMP/Cluster");
        
        // CREATE OUTPUT OF CHARGED HADRONS //
        OSCARInterface::OutputChargeHadronList(outhadron_name, EventID);
  
    }
    
    OSCARInterface::ExecuteToolkit(EventID);
      
    // EXIT //
    return EXIT_SUCCESS;
}


