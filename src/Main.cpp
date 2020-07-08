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

// // CREATE OUTPUT OF PARTON LIST //
// void OutputPartonList(std::string filename){
    
//     std::ofstream OutStream; OutStream.open(filename,std::ofstream::out | std::ofstream::app);
    
//     for(int i=0;i<GlobalPartonList.size();i++){
//         OutStream << GlobalPartonList.at(i).Mom.Px() << " " << GlobalPartonList.at(i).Mom.Py() << " " << GlobalPartonList.at(i).Mom.Pz() << " " << GlobalPartonList.at(i).Mom.E() << " " << Particle::mapToPDG(GlobalPartonList.at(i).FLAVOR) << std::endl;
//     }
    
//     OutStream.close();
    
// }

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
    
    // SETUP FOLDER STRUCTURE FOR OUTPUTS AND TEMPORARY FILES //
    std::stringstream outfolder_stream;
    outfolder_stream << "./OUTPUT/" << EventClass << "/" << EventID;
    std::string outfolder_name = outfolder_stream.str();
	
    int check = mkdir(outfolder_name.c_str(),0777); 

    if (check){
      std::cout << "Folder ./OUTPUT/" << EventClass << "/" << EventID << " created." << std::endl;
      }

    std::stringstream tmp_stream;
    tmp_stream << "./OUTPUT/" << EventClass << "/" << EventID << "/TMP";
    std::string tmp_name = tmp_stream.str();
	
    check = mkdir(tmp_name.c_str(),0777); 

    if (check){
      std::cout << "Folder " << tmp_stream.str() << " created." << std::endl;
      }
    
    std::stringstream outparton_stream;
    std::stringstream outhadron_stream;
    outparton_stream << "./OUTPUT/" << EventClass << "/" << EventID << "/OSCARpartons.dat";
    std::string outparton_name = outparton_stream.str();
    outhadron_stream << "./OUTPUT/" << EventClass << "/" << EventID << "/OSCAR.dat";
    std::string outhadron_name = outhadron_stream.str();

    std::stringstream log_str;
    log_str << outfolder_stream.str() << "/ClusteringLog.txt";
    std::stringstream conf_str;
    conf_str << outfolder_stream.str() << "/ClusterConfigurationLog.txt";
    std::stringstream tempFile_str;
    tempFile_str << tmp_stream.str() << "/Cluster";

    OSCARInterface::OSCARHeader(outhadron_name);
    OSCARInterface::OSCARHeader(outparton_name);
    
    HERWIGInterface::BACKUPClusters::Setup();
    
    
    for(int s=0; s<10; s++){
      cout << "_________ " << s << " ________" << endl;

        // SAMPLE GLUONS FROM IP-GLASMA EVENT //
        IPGlasmaInterface::Sample();
               
        // SETUP CLUSTERING ALGORITHM //
        ClusteringAlgorithm::Setup();
        
        // PERFORM CLUSTERING //
        ClusteringAlgorithm::Cluster(log_str.str(),conf_str.str(),5000);
        
        // GENERATE LES HOUCHES FILES //
        Clusters.GenerateLesHouchesFile(tempFile_str.str());
        
        // EXCEUTE HERWIG AND GET HADRONS //
        HERWIGInterface::Execute(tempFile_str.str());
        
        // CREATE OUTPUT OF CHARGED HADRONS //
        OSCARInterface::OutputChargeHadronList(outhadron_name, EventID);

        // CREATE OUTPUT OF PARTONS //
        OSCARInterface::OutputPartonList(outparton_name, EventID);
 
    }

    // COMPUTE OBSERVABLES FOR A GIVEN IPGLASMA EVENT (AVERAGE OVER HERWIG RUNS) //
    //OSCARInterface::ExecuteToolkit(EventClass, EventID);
     
    // EXIT //
    return EXIT_SUCCESS;
}


