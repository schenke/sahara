namespace OSCARInterface{

    // WRITE OSCAR HEADER //
  void OSCARHeader(std::string filename){
    std::ofstream OutStream; OutStream.open(filename, std::ofstream::out);// | std::ofstream::app);
    OutStream << "OSC1997A \nfinal_id_p_x \nSAHARA <version>" << std::endl;
  }

  // RUN CHUN'S HADRONIC AFTERBURNER TOOLKIT //
  void ExecuteToolkit(std::string EventClass, int EventID){
    
    std::stringstream command;
    command << "./run_toolkit.sh " << EventClass << " " << EventID;
    std::string SystemCommand=command.str();
    system(SystemCommand.c_str());       

    }

  // CREATE OUTPUT OF PARTON LIST //
  void OutputPartonList(std::string filename, int eventID){

    std::ofstream OutStream; OutStream.open(filename, std::ofstream::app);
    
    OutStream << eventID << " " << GlobalPartonList.size() << std::endl;

    for(int i=0;i<GlobalPartonList.size();i++){
      
      // number  pdgID  px  py  pz  E  mass   x  y  z  t
      // WRITE CHARGED PIONS, KAONS, (ANTI-)PROTONS 
      OutStream << i << " " 
                  << Particle::mapToPDG(GlobalPartonList.at(i).FLAVOR) << " " 
                  << GlobalPartonList.at(i).Mom.Px() << " " 
                  << GlobalPartonList.at(i).Mom.Py() << " " 
                  << GlobalPartonList.at(i).Mom.Pz() << " " 
                  << GlobalPartonList.at(i).Mom.E() << " " 
                  << GlobalPartonList.at(i).m << " " 
                  << GlobalPartonList.at(i).Pos.X() << " " 
                  << GlobalPartonList.at(i).Pos.Y() << " " 
                  << GlobalPartonList.at(i).Pos.Z() << " " 
                  << GlobalPartonList.at(i).Pos.T() << " " << std::endl;
    }
    OutStream.close();
  }
    
  // CREATE OUTPUT OF CHARGED HADRON LIST //
  void OutputChargeHadronList(std::string filename, int eventID){
    
    std::ofstream OutStream; OutStream.open(filename, std::ofstream::app);
    
    // COUNT CHARGED HADRONS //
    int n_chargedHadrons=0;

    for(int i=0;i<GlobalHadronList.size();i++){
      
      int PDGSpecies=GlobalHadronList.at(i).PDGSpecies;
      
      if(abs(PDGSpecies)==211 || abs(PDGSpecies)==321 || abs(PDGSpecies)==2212){
        n_chargedHadrons++;
      }
    }


    OutStream << eventID << " " << n_chargedHadrons+GlobalPartonList.size() << std::endl;
    
    // WRITE PARTICLE LIST IN OSCAR FORMAT //
    for(int i=0;i<GlobalHadronList.size();i++){
      
      int PDGSpecies=GlobalHadronList.at(i).PDGSpecies;
      
      // number  pdgID  px  py  pz  E  mass   x  y  z  t
      // WRITE CHARGED PIONS, KAONS, (ANTI-)PROTONS 
      if(abs(PDGSpecies)==211 || abs(PDGSpecies)==321 || abs(PDGSpecies)==2212){
        OutStream << i << " " 
                  << PDGSpecies << " " 
                  << GlobalHadronList.at(i).Mom.Px() << " " 
                  << GlobalHadronList.at(i).Mom.Py() << " " 
                  << GlobalHadronList.at(i).Mom.Pz() << " " 
                  << GlobalHadronList.at(i).Mom.E() << " " 
                  << GlobalHadronList.at(i).m << " " 
                  << GlobalHadronList.at(i).Pos.X() << " " 
                  << GlobalHadronList.at(i).Pos.Y() << " " 
                  << GlobalHadronList.at(i).Pos.Z() << " " 
                  << GlobalHadronList.at(i).Pos.T() << " " << std::endl;
      }
      
    }
    //NOW DO GLUONS IN SAME LIST
    for(int i=0;i<GlobalPartonList.size();i++){
      
      // number  pdgID  px  py  pz  E  mass   x  y  z  t
      OutStream << i << " " 
                  << Particle::mapToPDG(GlobalPartonList.at(i).FLAVOR) << " " 
                  << GlobalPartonList.at(i).Mom.Px() << " " 
                  << GlobalPartonList.at(i).Mom.Py() << " " 
                  << GlobalPartonList.at(i).Mom.Pz() << " " 
                  << GlobalPartonList.at(i).Mom.E() << " " 
                  << GlobalPartonList.at(i).m << " " 
                  << GlobalPartonList.at(i).Pos.X() << " " 
                  << GlobalPartonList.at(i).Pos.Y() << " " 
                  << GlobalPartonList.at(i).Pos.Z() << " " 
                  << GlobalPartonList.at(i).Pos.T() << " " << std::endl;
    }

    
    OutStream.close();
    
  }  
  
}
