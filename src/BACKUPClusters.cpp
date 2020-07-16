namespace BACKUPClusters{
    
    // VECTOR OF DEFAULT CLUSTERS //
    std::vector<Cluster> DefaultClusters;
    
    // VECTOR OF ENERGIES AND MINIMAL ENERGIES OF DEFAULY CLUSTERS //
    std::vector<double> DefaultClusterETot;
    std::vector<double> DefaultClusterEMin;
    
    void Setup(){
        
        for(int i=0;i<2210;i++){
            
            // CREATE NEW CLUSTER AT REST //
            Cluster NewCluster;
            
            NewCluster.Beta=VectorEPxPyPz(1,0,0,0);
            DefaultClusters.push_back(NewCluster);
            
            // GET DEFAULT CLUSTER FROM LIST //
            HERWIGInterface::GetHadrons("../HERWIGCLUSTERS/Cluster",i,DefaultClusters);

            // SET HADRONIC ENERGY AND MINIMAL ENERGY (SUM OF HADRON MASSES) AND ADD TO LIST //
            DefaultClusters.at(i).HadronicClusterMom=VectorEPxPyPz(0,0,0,0);
            
            for(int j=0;j<DefaultClusters.at(i).BoostedHadronList.size();j++){
                DefaultClusters.at(i).HadronicClusterMom+=DefaultClusters.at(i).BoostedHadronList.at(j).Mom;
            }
            
            double ETot=DefaultClusters.at(i).HadronicClusterMom.E();
            double EMin=0.0;
            
            for(int j=0;j<DefaultClusters.at(i).BoostedHadronList.size();j++){
                
                double E =DefaultClusters.at(i).BoostedHadronList.at(j).Mom.E();
                double PX=DefaultClusters.at(i).BoostedHadronList.at(j).Mom.Px();
                double PY=DefaultClusters.at(i).BoostedHadronList.at(j).Mom.Py();
                double PZ=DefaultClusters.at(i).BoostedHadronList.at(j).Mom.Pz();
                
                double MSqr=E*E-PX*PX-PY*PY-PZ*PZ;
                
                if(MSqr>0){
                    EMin+=sqrt(MSqr);
                }

            }
            
            DefaultClusterETot.push_back(ETot);
            DefaultClusterEMin.push_back(EMin);

            
	    //            std::cerr << "BACKUP " << DefaultClusterETot.at(i) << " " << DefaultClusterEMin.at(i) << std::endl;
            
        }
        
    }
    
    // GENERATE HADRONIC CLUSTER WITH ENERGY //
    std::vector<Particle> GenerateHadronicCluster(double E){
        
        // SETUP HADRON LIST IN COM FRAME //
        std::vector<Particle> NewHadronList; NewHadronList.clear();
        
        // COMMANDLINE OUTPUT //
        std::cerr << "#GENERATING BACKUP CLUSTER WITH ENERGY E=" << E << std::endl;
        
        // SELECT CLUSTER WITH ENERGY EMinCluster < E < ETotCluster //
        int iCluster; int SUCCESS=0; int Attempts=0;
        
        double ETotCluster=-1;
        double EMinCluster=-1;

        while(SUCCESS==0 && Attempts<10000){
            
            iCluster=rand() % DefaultClusters.size();
            
            ETotCluster=DefaultClusterETot.at(iCluster);
            EMinCluster=DefaultClusterEMin.at(iCluster);
            
            if(E>EMinCluster && ETotCluster>E){
                SUCCESS=1;
            }
            
            Attempts++;
            
        }
        
        if(SUCCESS==0){
            std::cerr << "#CRITICAL ERROR -- COULD NOT GENERATE BACKUP CLUSTER" << std::endl;
            exit(0);
        }
        
        std::cerr << "#STARTING FROM CLUSTER WITH ETot=" << ETotCluster << " EMin=" << EMinCluster << std::endl;
        
        // GET HADRON LIST FROM DEFAULT CLUSTER //
        NewHadronList=DefaultClusters.at(iCluster).BoostedHadronList;

        
        // DETERMINE SCALE FACTOR TO SCALE ALL MOMENTA //
        double EMin=EMinCluster;
        double sMin=0.0;
        
        double EMax=ETotCluster;
        double sMax=1.0;
        
        while((EMax-EMin)>0.001*E){
            
            double sInt=(sMin+sMax)/2.0;
            double EInt=0.0;
            
            for(int j=0;j<NewHadronList.size();j++){
                
                double Ej =NewHadronList.at(j).Mom.E();
                double Pjx=NewHadronList.at(j).Mom.Px();
                double Pjy=NewHadronList.at(j).Mom.Py();
                double Pjz=NewHadronList.at(j).Mom.Pz();
                
                double MjSqr=Ej*Ej-Pjx*Pjx-Pjy*Pjy-Pjz*Pjz;
                
                if(MjSqr>0){
                    EInt+=std::sqrt(MjSqr+sInt*sInt*(Pjx*Pjx+Pjy*Pjy+Pjz*Pjz));
                }
                else{
                    EInt+=std::sqrt(sInt*sInt*(Pjx*Pjx+Pjy*Pjy+Pjz*Pjz));
                }
                
            }
            
            if(EInt>E){
                EMax=EInt;
                sMax=sInt;
            }
            else{
                EMin=EInt;
                sMin=sInt;
            }
            
        }
        
        // PERFORM RANDOM ROTATIONS //
        double q1=2.0*M_PI*drand48();
        double q2=2.0*M_PI*drand48();
        double q3=2.0*M_PI*drand48();
        
        double c1=cos(q1); double s1=sin(q1);
        double c2=cos(q2); double s2=sin(q2);
        double c3=cos(q3); double s3=sin(q3);
        
        // SET LIST OF HADRONS //
        for(int j=0;j<NewHadronList.size();j++){
            
            // SCALE MOMENTA //
            double s=(sMin+sMax)/2.0;
            
            double Ej =NewHadronList.at(j).Mom.E();
            double Pjx=NewHadronList.at(j).Mom.Px();
            double Pjy=NewHadronList.at(j).Mom.Py();
            double Pjz=NewHadronList.at(j).Mom.Pz();
            
            double MjSqr=Ej*Ej-Pjx*Pjx-Pjy*Pjy-Pjz*Pjz;

            double PxNew=s*(c2*c3*Pjx-c2*s3*Pjy+s2*Pjz);
            double PyNew=s*((c1*s3+c3*s1*s2)*Pjx+(c1*c3-s1*s2*s3)*Pjy-c2*s1*Pjz);
            double PzNew=s*((s1*s3-c1*c3*s2)*Pjx+(c3*s1+c1*s2*s3)*Pjy+c1*c2*Pjz);
            
            double ENew;
            
            if(MjSqr>0){
                ENew=sqrt(MjSqr+PxNew*PxNew+PyNew*PyNew+PzNew*PzNew);
            }
            else{
                ENew=sqrt(PxNew*PxNew+PyNew*PyNew+PzNew*PzNew);
            }
            
            NewHadronList.at(j).Mom=VectorEPxPyPz(ENew,PxNew,PyNew,PzNew);
            
        }
        
        // GET CLUSTER ENERGY AND CHECK THAT IT MATCHES //
        double ERec=0.0;
        
        for(int j=0;j<NewHadronList.size();j++){
            ERec+=NewHadronList.at(j).Mom.E();
        }
        
        if(std::abs(ERec/E-1)<0.001){
            std::cerr << "#SUCCESS E=" << E << " ECluster=" << ERec  << std::endl;
        }
        else{
            std::cerr << "#ERROR -- ENERGIES DO NOT MATCH --  E=" << E << " ECluster=" << ERec  << std::endl;
            exit(0);
        }
        
        std::cerr << std::endl;
        
        return NewHadronList;
        
        
    }
    
    
    
}
