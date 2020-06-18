namespace HERWIGInterface{
    
    // SPLIT STRING //
    std::vector<std::string> SplitString(std::string const &input)
    {
        std::istringstream buffer(input);
        std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());
        return ret;
    }
    
    void Hadronize(std::string fname,int iCluster){
        
        std::string LHEFilename = fname + "_SAHARAEvent_" + std::to_string(iCluster) + ".lhe";
        std::string HerwigFilename = fname + "_HERWIGOutput_" + std::to_string(iCluster);
        
        // GENERATE HERWIG INPUT FILE //
        std::string HerwigInput="";
        HerwigInput+="##################################################\n";
        HerwigInput+="## Collider type                                ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## LOAD LES HOUCHES STUFF                       ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="library LesHouches.so \n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## CREATE EVENT READER                          ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="cd /Herwig/EventHandlers \n";
        HerwigInput+="create ThePEG::LesHouchesFileReader myReader\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## DEFINE LHE FILE                              ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="set myReader:FileName " + LHEFilename +"\n";
        HerwigInput+="set myReader:CacheFileName cache.tmp\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## CUTS ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="create ThePEG::Cuts NoCuts\n";
        HerwigInput+="set myReader:Cuts NoCuts\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## PDFS ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="set /Herwig/Particles/e-:PDF /Herwig/Partons/NoPDF\n";
        HerwigInput+="set /Herwig/Particles/e+:PDF /Herwig/Partons/NoPDF\n";
        HerwigInput+="\n";
        HerwigInput+="set myReader:InitPDFs 0\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="set /Herwig/EventHandlers/myReader:PDFA /Herwig/Partons/NoPDF\n";
        HerwigInput+="set /Herwig/EventHandlers/myReader:PDFB /Herwig/Partons/NoPDF\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## DECAY HADRONIZATION AND SHOWER ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="cd /Herwig/EventHandlers\n";
        HerwigInput+="\n";
        HerwigInput+="create ThePEG::LesHouchesEventHandler myLesHouchesHandler\n";
        HerwigInput+="set myLesHouchesHandler:WeightOption UnitWeight\n";
        HerwigInput+="\n";
        HerwigInput+="set myLesHouchesHandler:CascadeHandler /Herwig/Shower/ShowerHandler\n";
        HerwigInput+="set myLesHouchesHandler:HadronizationHandler /Herwig/Hadronization/ClusterHadHandler\n";
        HerwigInput+="set myLesHouchesHandler:DecayHandler /Herwig/Decays/DecayHandler\n";
        HerwigInput+="\n";
        HerwigInput+="set myLesHouchesHandler:PartonExtractor /Herwig/Partons/EEExtractor\n";
        HerwigInput+="\n";
        HerwigInput+="set myLesHouchesHandler:CascadeHandler:MPIHandler NULL\n";
        HerwigInput+="\n";
        HerwigInput+="set /Herwig/Shower/KinematicsReconstructor:ReconstructionOption General\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## EVENT READER ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="cd /Herwig/EventHandlers\n";
        HerwigInput+="insert myLesHouchesHandler:LesHouchesReaders 0 myReader\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        HerwigInput+="\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="## CREATE RUN FILE ##\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="cd /Herwig/Generators\n";
        HerwigInput+="cp EventGenerator myLesHouchesGenerator\n";
        HerwigInput+="set myLesHouchesGenerator:EventHandler /Herwig/EventHandlers/myLesHouchesHandler\n";
        HerwigInput+="saverun " + HerwigFilename + " myLesHouchesGenerator\n";
        HerwigInput+="##################################################\n";
        HerwigInput+="\n";
        
        // CREATE HERWIG INPUT FILE //
        std::ofstream OutStream;
        OutStream.open("HERWIGInput.in");
        OutStream << HerwigInput;
        OutStream.close();
        
        // CREATE HERWIG EXECUTION COMMAND //
        std::string SystemCommand="";
        SystemCommand="Herwig read HERWIGInput.in; Herwig run " + HerwigFilename + ".run -N 1";
        
        // CREATE FAKE-HERWIG EXECUTION COMMAND //
        //std::string SystemCommand="";
        //SystemCommand="./FakeHerwig.exe " + LHEFilename + " " + HerwigOutput + ".log";
        
        // EXECUTE SYSTEM COMMAND //
        std::cerr << "#EXECUTING SYSTEM COMMAND --- " << SystemCommand << std::endl;
        system(SystemCommand.c_str());
        
    }
    
    void GetHadrons(std::string fname,int iCluster){
        
        // GET HERWIG OUTPUT FILE //
        std::string HerwigOutput = fname + "_HERWIGOutput_" + std::to_string(iCluster) + ".log";
        std::ifstream readParticles(HerwigOutput.c_str());
        
        // SCAN FILE AND EXTRACT HADRONS //
        int StartReading=0; int noEvents=0; std::string line; int PDGSpecies;
        
        while( std::getline( readParticles, line )){
            
            std::istringstream iss( line );
            std::string result;
            
            if(std::getline( iss, result)){
                
                if( (StartReading==3 || StartReading==2) && result == "------------------------------------------------------------------------------" ){
                    StartReading=0;
                    noEvents++;
                    break;
                }
                
                if(StartReading==3){
                    
                    std::vector<std::string> HadronLine;
                    HadronLine = SplitString(result);
                    
                    Particle NewHadron;
                    
                    NewHadron.PDGSpecies=PDGSpecies;
                    
                    NewHadron.Mom.E()  = atof(HadronLine[3].c_str());
                    NewHadron.Mom.Px() = atof(HadronLine[0].c_str());
                    NewHadron.Mom.Py() = atof(HadronLine[1].c_str());
                    NewHadron.Mom.Pz() = atof(HadronLine[2].c_str());
                    
                    NewHadron.m = atof(HadronLine[4].c_str());
                    
                    Clusters.ClusterList.at(iCluster).BoostedHadronList.push_back(NewHadron);
                    
                    
                    StartReading=2;
                    continue;
                }
                
                if(StartReading==2){
                    
                    std::vector<std::string> HadronLine;
                    HadronLine = SplitString(result);
                    
                    if(HadronLine.size()==4){
                        PDGSpecies=atoi(HadronLine[2].c_str()); StartReading=3;
                    }
                    
                    else{continue;}
                    
                }
                
                
                // CHECK FOR FINAL //
                if(StartReading==1 && result == "--- final:" ){
                    StartReading=2;
                    continue;
                }
                
                // CHECK FOR DECAY HANDLER //
                if(StartReading==0 && result == "Step 4 performed by DecayHandler" ){
                    StartReading=1;
                    continue;
                }
            }
        }
        
        // BOOST HADRONS BACK TO LAB FRAME AND CHECK ENERGY-MOMENTUM CONSERVATION //
        Clusters.ClusterList.at(iCluster).BoostToLabFrame();
        Clusters.ClusterList.at(iCluster).CheckEnergyMomentumConservation();
        
        
    }

    
    void Execute(std::string fname){
        
        // CLEAR GLOBAL HADRON LIST //
        GlobalHadronList.clear();
        
        // EXECUTE HERWIG HADRONIZATION FOR EACH CLUSTER //
        for(int i=0;i<Clusters.ClusterList.size();i++){

            // EXECUTE HERWIG //
            Hadronize(fname,i);
            
            // GET HADRONS //
            GetHadrons(fname,i);
            

            // WRITE HADRONS TO GLOBAL HADRON LIST //
            for(int j=0;j<Clusters.ClusterList.at(i).HadronList.size();j++){
                GlobalHadronList.push_back(Clusters.ClusterList.at(i).HadronList.at(j));
            }
            
        }
        
        
    }
    
    
    
    

}
