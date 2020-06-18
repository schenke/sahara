#include <ctime>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <complex>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::string GetName(int pID){
    if(pID==+11){
        return "e-";
    }
    else if(pID==-11){
        return "e+";
    }
    else if(pID==21){
        return "g";
    }
    else if(pID==22){
        return "gamma";
    }
    else{
        return "NN";
    }
}

// DEFINES PARTICLE WITH PROPERTIES //
class Particle{
 
private:
    int pID; int Status; int Parent1; int Parent2; int Color1; int Color2; double pX; double pY; double pZ; double E; double Q;

public:
    
    std::string Print(int i,std::string ProcessInfo){
        
        std::stringstream ss;
        ss << i << "\t" << GetName(pID) << "\t" <<pID << "\t" << ProcessInfo << "\t" << std::endl;
        ss << "\t" << "\t" << "\t" << pX << "\t" << pY << "\t" << pZ << "\t" << E << "\t" << Q << std::endl;
        
        return ss.str();
    }
    
    
    Particle(int pID, int Status, int Parent1, int Parent2, int Color1, int Color2, double pX, double pY, double pZ, double E,double Q){
        this->pID=pID; this->Status=Status; this->Parent1=Parent1; this->Parent2=Parent2; this->Color1=Color1; this->Color2=Color2; this->pX=pX; this->pY=pY; this->pZ=pZ; this->E=E; this->Q=Q;
    }
    
    ~Particle(){
        
    }
    
};

// GLOBAL NUMBER OF PARTICLES //
int NumberOfParticles;

// VECTORS OF INCOMING, INTERMEDIATE AND OUTGOING PARTICLES //
std::vector<Particle> Incoming;
std::vector<Particle> Intermediate;
std::vector<Particle> Outgoing;


// GET EVENT DATA FROM LES HOUCHES FILE //
void GetLesHouchesData(std::string fname){
    
    // COMMANDLINE OUTPUT //
    std::cerr << "#STARTING FAKE HERWIG 1.0 -- " << fname << std::endl;
    
    // CREATE INPUT FILE STREAM //
    std::ifstream InStream;
    
    InStream.open(fname.c_str());
    
    // READ INDIVIDUAL LINES //
    std::string InLine;
    
    // SET EVENT ENVIRONMENT //
    int EventEnvironment=0; int CheckSum=0;
    
    // READ FROM INPUT FILES //
    while(InStream.good()){
        
        // GET LINE //
        getline(InStream,InLine);
        
        // SELECT ONLY INFORMATION WITHIN EVENT ENVIRONMENT //
        if (InLine.find("<event>") != std::string::npos) {
            
            // SET EVENT ENVIRONMENT //
            EventEnvironment=1;
            
            // GET NEXT LINE WITH THE EVENT INFO   //
            if(InStream.good()){
                
                // GET NEXT LINE //
                getline(InStream,InLine);
                
                // STRING TOKENIZE //
                std::stringstream Values(InLine);
                
                // GET NUMBER OF PARTICLES INCLUDING e+/e- AND INTERMEDIATE gamma //
                Values >> NumberOfParticles;
                
            }
            
            else{
                std::cerr << "#ERROR -- CAN NOT PARSE LHE FILE" << std::endl;
                exit(0);
            }
        }
        
        else if (InLine.find("</event>") != std::string::npos) {
            EventEnvironment=0;
        }
        
        // INCREASE POSITION COUNT //
        else if(EventEnvironment && !(InLine.empty())){
            
            // STRING TOKENIZE //
            std::stringstream Values(InLine);
            
            // GET PARTICLE INFO //
            int pID; int Status; int Parent1; int Parent2; int Color1; int Color2; double pX; double pY; double pZ; double E;
            
            Values >> pID; Values >> Status; Values >> Parent1; Values >> Parent2; Values >> Color1; Values >> Color2; Values >> pX; Values >> pY; Values >> pZ; Values >> E;
            
            double Q2=E*E-pX*pX-pY*pY-pZ*pZ;
            double Q;
            if(std::abs(Q2)>1e-4){
                Q=sqrt(Q2);
            }
            else{
                Q=0.0;
            }
            
            // CREATE NEW PARTICLE //
            Particle NewParticle=Particle(pID,Status,Parent1,Parent2,Color1,Color2,pX,pY,pZ,E,Q);
            
            // INCOMING //
            if(Status==-1){
                Incoming.push_back(NewParticle);
            }
            
            // INTERMEDIATE //
            else if(Status==+2){
                Intermediate.push_back(NewParticle);
            }
            
            // OUTGOING //
            else if(Status==+1){
                Outgoing.push_back(NewParticle);
            }
            else{
                std::cerr << "#ERROR -- COULD NOT CORRECTLY DETECT PARTICLE" << std::endl;
                std::cerr << pID << " " << Status << " " << Parent1 << " " << Parent2 << " " << Color1 << " " << Color2 << " " << pX << " " << pY << " " << pZ << " " << E << std::endl;
                exit(0);
            }
        
            // INCREMENT INDEX //
            CheckSum++;
            
        }
    }
    
    // CLOSE STREAM //
    InStream.close();
    
    // CHECK THAT DATA WAS READ CORRECTLY //
    if(CheckSum!=NumberOfParticles){
        std::cerr << "#ERROR -- LHE DATA FILE NOT READ CORRECTLY -- " << NumberOfParticles << " != " << CheckSum << std::endl;
    }
    
}

void CreateFakeHerwigOutput(std::string fname,int NumberOfEvents,std::string id){
    
    std::ofstream OutStream; OutStream.open(fname.c_str());
    
    std::cerr << "#CREATING OUTPUT FOR EVENT WITH " << NumberOfParticles << " PARTICLES -- " << fname << std::endl;
    
    // CREATE HEADER //
    OutStream << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
    OutStream << ">>>> Toolkit for HEP Event Generation - ThePEG 2.2.0 <<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    OutStream << ">>>> Thu Apr  2 10:06:23 2020                                             <<<<" << std::endl;
    OutStream << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    OutStream << std::endl;
    OutStream << ">> FakeHerwig 1.0.0 / ThePEG 2.2.0" << std::endl;
    OutStream << std::endl;
    OutStream << "/Herwig/EventHandlers/NoCuts:" << std::endl;
    OutStream << "MHat  = 2 .. 79.95 GeV" << std::endl;
    OutStream << "Scale = 0 .. 1e+12 GeV2" << std::endl;
    OutStream << "YHat  = -100 .. 100" << std::endl;
    OutStream << "X1    = 0 .. 1" << std::endl;
    OutStream << "X2    = 0 .. 1" << std::endl;
    OutStream << std::endl;
    
    for(int EventNumber=1;EventNumber<=NumberOfEvents;EventNumber++){
        
        OutStream << "******************************************************************************" << std::endl;
        OutStream << "Event number " << EventNumber << " (id: "<< id <<") performed by myLesHouchesHandler" << std::endl;
        OutStream << "Weight: 1" << std::endl;
        OutStream << "==============================================================================" << std::endl;
        OutStream << std::endl;
        OutStream << "--- Colliding particles:" << std::endl;
        for(int i=0;i<Incoming.size();i++){
            OutStream << Incoming[i].Print(i,"(3)");
        }
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "Primary sub-process performed by myReader" << std::endl;
        OutStream << "--- incoming:" << std::endl;
        for(int i=0;i<Incoming.size();i++){
            OutStream << Incoming[i].Print(i+1,"(3)");
        }
        OutStream << "--- intermediates:" << std::endl;
        for(int i=0;i<Intermediate.size();i++){
            OutStream << Intermediate[i].Print(Incoming.size()+i+1,"[1,2] (X...Y)");
        }
        OutStream << "--- outgoing:" << std::endl;
        for(int i=0;i<Outgoing.size();i++){
            OutStream << Outgoing[i].Print(Incoming.size()+Intermediate.size()+i+1,"[3] (Z) A>>B {+C,-D}");
        }
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "Step 1 performed by myLesHouchesHandler" << std::endl;
        OutStream << "--- intermediates:" << std::endl;
        for(int i=0;i<Incoming.size();i++){
            OutStream << Incoming[i].Print(i+1,"(3)");
        }
        for(int i=0;i<Intermediate.size();i++){
            OutStream << Intermediate[i].Print(Incoming.size()+i+1,"[1,2] (X...Y)");
        }
        OutStream << "--- final:" << std::endl;
        for(int i=0;i<Outgoing.size();i++){
            OutStream << Outgoing[i].Print(Incoming.size()+Intermediate.size()+i+1,"[3] (Z) A>>B {+C,-D}");
        }
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "\t" << "Sum of momenta:" << "\t" << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << std::endl;
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "Step 2 performed by ShowerHandler" << std::endl;
        OutStream << "--- intermediates:" << std::endl;
        OutStream << "--- final:" << std::endl;
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "\t" << "Sum of momenta:" << "\t" << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << std::endl;
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "Step 3 performed by ClusterHadHandler" << std::endl;
        OutStream << "--- intermediates:" << std::endl;
        for(int i=0;i<Outgoing.size();i++){
            OutStream << Outgoing[i].Print(Incoming.size()+Intermediate.size()+Outgoing.size()+i+1,"[U](V)");
        }
        OutStream << "--- final:" << std::endl;
        for(int i=0;i<Outgoing.size();i++){
            OutStream << Outgoing[i].Print(Incoming.size()+Intermediate.size()+2*Outgoing.size()+i+1,"[U](V)");
        }
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "\t" << "Sum of momenta:" << "\t" << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << std::endl;
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "Step 4 performed by DecayHandler" << std::endl;
        OutStream << "--- intermediates:" << std::endl;
        OutStream << "--- final:" << std::endl;
        for(int i=0;i<Outgoing.size();i++){
            OutStream << Outgoing[i].Print(Incoming.size()+Intermediate.size()+2*Outgoing.size()+i+1,"[U](V)");
        }
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << "\t" << "Sum of momenta:" << "\t" << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0 << std::endl;
        OutStream << "------------------------------------------------------------------------------" << std::endl;
        OutStream << std::endl;
        
    }
    
    OutStream.close();
    
}

// MAIN //
int main(int argc, char *argv[]){
        
    GetLesHouchesData(argv[1]);
    CreateFakeHerwigOutput(argv[2],4,argv[1]);
    
}
