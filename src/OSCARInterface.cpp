namespace OSCARInterface{

    // WRITE OSCAR HEADER //
  void OSCARHeader(std::string filename){
    std::ofstream OutStream; OutStream.open(filename, std::ofstream::out);// | std::ofstream::app);
    OutStream << "OSC1997A \nfinal_id_p_x \nSAHARA <version>" << std::endl;
  }

  // RUN CHUN'S HADRONIC AFTERBURNER TOOLKIT //
  void ExecuteToolkit(int EventID){
    
    std::stringstream command;
    command << "./run_toolkit.sh " << EventID;
    std::string SystemCommand=command.str();
    system(SystemCommand.c_str());       

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

    OutStream << eventID << " " << n_chargedHadrons << std::endl;

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
    
    OutStream.close();
    
  }  
  
}