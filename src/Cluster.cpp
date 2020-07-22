// DEFINE CLUSTERING PARAMETERS //
namespace ClusteringParameters {
    
    int MininalParticleNumberPerCluster=4;
    double alpha=10.0;
    
    // PRINT //
    void Print(){
        
        std::cerr << "#CLUSTERING PARAMETERS ARE" << std::endl;
        std::cerr << "#MininalParticleNumberPerCluster=" << MininalParticleNumberPerCluster << std::endl;
        std::cerr << "#alpha=" << alpha << std::endl;

    }
    
}

// DEFINITION OF CLUSTERS //
class Cluster{
    
public:
    
    // CLUSTER VARIABLES //
    std::vector<int> ParticleIDList;
    
    std::vector<Particle> PartonList;
    std::vector<Particle> BoostedPartonList;
    
    std::vector<Particle> HadronList;
    std::vector<Particle> BoostedHadronList;
    
    VectorEPxPyPz PartonicClusterMom;
    VectorEPxPyPz HadronicClusterMom;
    
    VectorEPxPyPz CenterOfMassMom;
    VectorEPxPyPz Beta;
    
    int NumberOfParticles;
    
    double ClusterDCAVolume;
    double S;
    
    bool isSplittable;
    bool isExchangeable;
    
private:
    
    // CENTER OF MASS VELOCITY AND MOMENTUM //
    void CalcCenterOfMass(){
        
        PartonicClusterMom=VectorEPxPyPz(0,0,0,0);
        CenterOfMassMom=VectorEPxPyPz(0,0,0,0);
        Beta=VectorEPxPyPz(0,0,0,0);
        
        for (int i=0;i<ParticleIDList.size();i++){
            
            PartonicClusterMom+=GlobalPartonList.at(ParticleIDList.at(i)).Mom;
            CenterOfMassMom+=GlobalPartonList.at(ParticleIDList.at(i)).Mom;
            Beta+=GlobalPartonList.at(ParticleIDList.at(i)).Mom;
        }
        
        Beta=Beta.NormalizeToE();
        CenterOfMassMom   *= 1./ParticleIDList.size();
    }
    
    void CalcDCAVolume(){
        
        using namespace ClusteringParameters;
        
        // CALCULATE CLUSTER DCA VOLUME //
        lorentz L;
        ClusterDCAVolume=0.0;
        
        for (int i1=0;i1<ParticleIDList.size();i1++){
            for (int i2=0;i2<ParticleIDList.size();i2++){
                if (i1!=i2){
                    ClusterDCAVolume+=L.getConstrainedDCAsquared(GlobalPartonList.at(ParticleIDList.at(i1)).Pos,GlobalPartonList.at(ParticleIDList.at(i2)).Pos,GlobalPartonList.at(ParticleIDList.at(i1)).Mom,GlobalPartonList.at(ParticleIDList.at(i2)).Mom);
                }
            }
        }
        if(ParticleIDList.size()>1)
        {
            ClusterDCAVolume /= (  ParticleIDList.size()*(ParticleIDList.size()-1));
        }
        
        // SET CLUSTER ACTION //
        S=alpha*std::sqrt(ClusterDCAVolume)*ParticleIDList.size();
    }
    
    
public:
    
    void Update(){
        
        // SET BASIC CLUSTER PROPERTIES //
        NumberOfParticles=ParticleIDList.size();
        
        if(NumberOfParticles>=2*ClusteringParameters::MininalParticleNumberPerCluster){
            isSplittable=true;
        }
        else{
            isSplittable=false;
        }
        
        if(NumberOfParticles>ClusteringParameters::MininalParticleNumberPerCluster){
            isExchangeable=true;
        }
        else{
            isExchangeable=false;
        }
        
        // CALCULATE CENTER OF MASS AND DCA VOLUME //
        CalcCenterOfMass();
        CalcDCAVolume();
    }
    
public:
    
    // ENERGY AND INVARIANT MASS OF CLUSTER //
    double ECluster; double QCluster;
    
private:

    
    // COLORS AND ANTI-COLORS OF CLUSTER PARTICLES //
    std::vector<int> Colors;
    std::vector<int> AntiColors;
    
    void SetPartonList(){
        
        PartonList.clear(); BoostedPartonList.clear();
        
        for(int i=0;i<ParticleIDList.size();i++){
            PartonList.push_back(GlobalPartonList.at(ParticleIDList.at(i)));
        }
        
    }
    
    void BoostToCenterOfMass(){
        
        // GET BETA AND PERFORM BOOST //
        lorentz BoostToCoM;
        BoostToCoM.setBeta(this->Beta);
        
        for(int i=0;i<PartonList.size();i++){
            BoostedPartonList.push_back(GlobalPartonList.at(ParticleIDList.at(i)));
            BoostedPartonList.at(i).Mom=BoostToCoM.boost(PartonList.at(i).Mom);
        }
        
        // CHECK THAT NET-MOMENTUM IS ZERO //
        VectorEPxPyPz P_LRF=VectorEPxPyPz(0,0,0,0);
        
        for(int i=0;i<BoostedPartonList.size();i++){
            P_LRF+=BoostedPartonList.at(i).Mom;
        }
        
        
        if(std::abs(P_LRF.Px())>1e-12 || std::abs(P_LRF.Py())>1e-12 || std::abs(P_LRF.Pz())>1e-12){
            std::cerr << "#WARNING -- BOOST TO REST-FRAME NOT SUCCESSFUL" << std::endl;
            std::cerr << "#CLUSTER MOMENTUM IS E=" << P_LRF.Px() << " Px=" << P_LRF.Px() << " Py=" << P_LRF.Py() << " Pz=" << P_LRF.Pz() << std::endl;
        }
        else{
	  //  std::cerr << "#CLUSTER BOOSTED BY " << Beta.E() << " " << Beta.Px() << " " << Beta.Py() << " " << Beta.Pz() << std::endl;
          //  std::cerr << "#CLUSTER MOMENTUM IS E=" << P_LRF.Px() << " Px=" << P_LRF.Px() << " Py=" << P_LRF.Py() << " Pz=" << P_LRF.Pz() << std::endl;
        }
        
        ECluster=P_LRF.E(); QCluster=sqrt(P_LRF.E()*P_LRF.E()-P_LRF.vec2());
        
    }
    
    void SetColorConfiguration(){
        
        ///////////////////////////////////////
        // SET LIST OF COLORS TO BE ASSIGNED //
        ///////////////////////////////////////
        
        int ColorCounter=500; int AntiColorCounter=500;
        
        for(int i=0;i<PartonList.size();i++){
            
            // GLUON -- COLOR AND ANTI-COLOR //
            if (PartonList.at(i).FLAVOR==gluon){
                
                ColorCounter++; AntiColorCounter++;
                
                Colors.push_back(ColorCounter);
                AntiColors.push_back(AntiColorCounter);
            }
            
            // QUARK -- COLOR ONLY //
            else if (PartonList.at(i).FLAVOR==up || PartonList.at(i).FLAVOR==down || PartonList.at(i).FLAVOR==strange){
                ColorCounter++;
                Colors.push_back(ColorCounter);
            }
            
            // ANTI-QUARK -- ANTI-COLOR ONLY //
            else if (PartonList.at(i).FLAVOR==anti_up || PartonList.at(i).FLAVOR==anti_down || PartonList.at(i).FLAVOR==anti_strange){
                AntiColorCounter++;
                AntiColors.push_back(AntiColorCounter);
            }
            else{
                std::cerr << "#NO COLOR ASSIGNMENT FOR PARTICLE " << PartonList.at(i).FLAVOR << std::endl;
            }
            
        }
        
        // CHECK THAT CLUSTER IS COLOR NEUTRAL //
        if(ColorCounter!=AntiColorCounter){
            std::cerr << "#ERROR -- CLUSTER IS NOT COLOR NEUTRAL" << std::endl;
            exit(0);
        }
        
        //////////////////////////////////////////////////////////////////////
        // SHUFFLE AND RE-ASSIGN COLORS UNTIL NO SINGLET GLUONS ARE PRESENT //
        //////////////////////////////////////////////////////////////////////
        
        bool NoSingletGluons=false;
        
        std::vector<int> ShuffledColors=Colors;
        std::vector<int> ShuffledAntiColors=AntiColors;
        
        while(NoSingletGluons==false){
            
            NoSingletGluons=true;
            
            std::random_shuffle(ShuffledColors.begin(),ShuffledColors.end());
            std::random_shuffle(ShuffledAntiColors.begin(),ShuffledAntiColors.end());
            
            // COUNTERS FOR COLORS AND ANTICOLORS //
            int iC=0; int iA=0;
            
            Colors.clear(); AntiColors.clear();
            
            for(int i=0;i<PartonList.size();i++){
                
                // GLUON -- COLOR AND ANTI-COLOR //
                if (PartonList.at(i).FLAVOR==gluon){
                    
                    Colors.push_back(ShuffledColors.at(iC));
                    AntiColors.push_back(ShuffledAntiColors.at(iA));
                    
                    // CHECK THAT GLUON IS NOT SINGLET //
                    if(Colors.at(i)==AntiColors.at(i)){
                        NoSingletGluons=false;
                    }
                    
                    iC++; iA++;
                    
                }
                
                // QUARK -- COLOR ONLY //
                else if (PartonList.at(i).FLAVOR==up || PartonList.at(i).FLAVOR==down || PartonList.at(i).FLAVOR==strange){
                    
                    Colors.push_back(ShuffledColors.at(iC));
                    AntiColors.push_back(0);
                    
                    iC++;
                    
                }
                
                // ANTI-QUARK -- ANTI-COLOR ONLY //
                else if (PartonList.at(i).FLAVOR==anti_up || PartonList.at(i).FLAVOR==anti_down || PartonList.at(i).FLAVOR==anti_strange){
                    
                    Colors.push_back(0);
                    AntiColors.push_back(ShuffledAntiColors.at(iA));
                    
                    iA++;
                    
                }
                
                
            }            
            
        }
        
    }
    
private:
    
    float round_down(double val){
        float rounded_down = floorf(val * 100000) / 100000;
        return rounded_down;
    }
    
    float round_up(double val){
        float rounded_up = ceilf(val*100000) / 100000;
        return rounded_up;
    }
    
public:
    
    void GenerateLesHouchesFile(std::string fname,int ClusterID){
        
        // SET PARTICLE LIST //
        SetPartonList();
        
        // BOOST TO CENTER OF MASS //
        BoostToCenterOfMass();
        
        // SET COLOR COFINGURATION //
        SetColorConfiguration();
        
        // SET NUMBER OF PARTICLES IN LES-HOUCHES FILE //
        int NumberOfLesHouchesParticles=PartonList.size()+3;
        
        // SET LES-HOUCHES HEADER  //
        std::string LesHouchesString="<LesHouchesEvents version=\"1.0\">\n<header>\n</header>\n<init>\n11 -11 " + std::to_string(QCluster) + " " + std::to_string(QCluster) + " -1 -1 -1 -1 1 1\n0 0  3 1001\n</init>\n<event>\n";
        LesHouchesString+=std::to_string(NumberOfLesHouchesParticles) + "\t1001\t1\t-1\t0.00729927007\t0.3\n";
        
        // SET MOMENTA OF e+- PAIR //
        VectorEPxPyPz ePMom(ECluster/2.,+ECluster/2.,0,0);
        VectorEPxPyPz eMMom(ECluster/2.,-ECluster/2.,0,0);
        
        // WRITE MOMENTA OF e+- PAIR //
        LesHouchesString+="11\t-1\t0\t0\t0\t0\t";
        LesHouchesString+=std::to_string(round_down(ePMom.Px())) + "\t" + std::to_string(round_down(ePMom.Py())) + "\t" + std::to_string(round_down(ePMom.Pz())) + "\t" + std::to_string(round_up(ePMom.E())) + "\t";
        LesHouchesString +=std::to_string(0.0) +"\t"+"0.0\t9.0\n";
        
        LesHouchesString+="-11\t-1\t0\t0\t0\t0\t";
        LesHouchesString+=std::to_string(round_down(eMMom.Px())) + "\t" + std::to_string(round_down(eMMom.Py())) + "\t" + std::to_string(round_down(eMMom.Pz())) + "\t" + std::to_string(round_up(eMMom.E())) + "\t";
        LesHouchesString+=std::to_string(0.0) +"\t"+"0.0\t9.0\n";
        
        // WRITE MOMENTA OF VIRTUAL PHOTON //
        VectorEPxPyPz GammaMom(ECluster,0,0,0);
        
        LesHouchesString+="22\t+2\t1\t2\t0\t0\t";
        LesHouchesString+=std::to_string(round_down(GammaMom.Px())) + "\t" + std::to_string(round_down(GammaMom.Py())) + "\t" + std::to_string(round_down(GammaMom.Pz())) + "\t" + std::to_string(round_up(GammaMom.E())) + "\t";
        LesHouchesString+=std::to_string(QCluster) +"\t"+"0.0\t9.0\n";
        
        // WRITE MOMENTA OF CLUSTER PARTICLES //
        for(int i=0;i<PartonList.size();i++){
            
            LesHouchesString+=std::to_string(Particle::mapToPDG(BoostedPartonList.at(i).FLAVOR)) + "\t+1\t3\t3\t"+std::to_string(Colors.at(i))+"\t"+std::to_string(AntiColors.at(i))+"\t";
            LesHouchesString+=std::to_string(round_down(BoostedPartonList.at(i).Mom.Px())) + "\t" + std::to_string(round_down(BoostedPartonList.at(i).Mom.Py())) + "\t" + std::to_string(round_down(BoostedPartonList.at(i).Mom.Pz())) + "\t" + std::to_string(round_up(BoostedPartonList.at(i).Mom.E())) + "\t";
            LesHouchesString+=std::to_string(0.0) +"\t"+"0.0\t9.0\n";
            
        }
        
        // SET LES-HOUCHES END //
        LesHouchesString+="</event>\n</LesHouchesEvents>";
        
        // CREATE LES-HOUCHES FILE //
        std::string LesHouchesFile = fname + "_SAHARAEvent_" + std::to_string(ClusterID) + ".lhe";
        std::ofstream OutStream; OutStream.open(LesHouchesFile);
        OutStream << LesHouchesString;
        OutStream.close();
        std::cout << "#CREATED LES-HOUCHES FILE --------------" << LesHouchesFile << "--------------\n\n\n";
        
        
        
    }
    
    
    void BoostToLabFrame(){
        
        // GET BETA AND PERFORM BOOST //
        lorentz BoostToLab;
        BoostToLab.setBeta(this->Beta);
        
        for(int i=0;i<BoostedHadronList.size();i++){
            HadronList.push_back(BoostedHadronList.at(i));
            HadronList.at(i).Mom=BoostToLab.boostInv(BoostedHadronList.at(i).Mom);
        }
        
    }
    
    void CheckEnergyMomentumConservation(){
        
        HadronicClusterMom=VectorEPxPyPz(0,0,0,0);
        
        for(int i=0;i<HadronList.size();i++){
            HadronicClusterMom+=HadronList.at(i).Mom;
        }
        
        std::cerr << "#CHECK MOMENTUM CONSERVATION -- Px -- Py -- Pz -- E -- HADRONS" << std::endl;
        std::cerr << HadronicClusterMom.Px() << " " << HadronicClusterMom.Py() << " " << HadronicClusterMom.Pz() << " " << HadronicClusterMom.E() << std::endl;
        std::cerr << "#CHECK MOMENTUM CONSERVATION -- Px -- Py -- Pz -- E -- PARTONS" << std::endl;
        std::cerr << PartonicClusterMom.Px() << " " << PartonicClusterMom.Py() << " " << PartonicClusterMom.Pz() << " " << PartonicClusterMom.E() << std::endl;
        
    }
    
    
    void PrintHadrons(){
        
        for(int i=0;i<HadronList.size();i++){
            std::cout << HadronList.at(i).PDGSpecies << " " << HadronList.at(i).Mom.Px() << " " << HadronList.at(i).Mom.Py() << " " << HadronList.at(i).Mom.Pz() << " " << HadronList.at(i).Mom.E() << std::endl;
        }
    }
    
    
public:
    
    // DEFAULT CONSTRUCTOR //
    Cluster():
    PartonicClusterMom(0,0,0,0),
    HadronicClusterMom(0,0,0,0),
    CenterOfMassMom(0,0,0,0),
    Beta(0,0,0,0),
    ClusterDCAVolume(0),
    S(0)
    {};
    
    // DEFAULT DESTRUCTOR //
    ~Cluster() {};
    
    
};

// DEFINITION OF CLUSTER CONFIGURATION //
class ClusterConfiguration{
    
public:
    
    // CLUSTER LIST //
    std::vector<Cluster> ClusterList;
    
    // NUMBER OF CLUSTERS //
    int NumberOfClusters; int NumberOfSplittableClusters; int NumberOfExchangeableClusters;
    
    // NUMBER OF OPTIONS //
    int NumberOfOptions;
    
    // NUMBER OF PARTICLES //
    int NumberOfParticles;
    
    // CLUSTER CONFIGRATION ACTION //
    double S;
    
    // CHECK WHICH OPERATIONS ARE POSSIBLE //
    int CanPerformClusterExchange(){
        if(NumberOfExchangeableClusters>=1 && NumberOfClusters>1){
            return 1;
        }
        else{
            return 0;
        }
    }
    
    int CanPerformClusterSplit(){
        if(NumberOfSplittableClusters>=1){
            return 1;
        }
        else{
            return 0;
        }
    }
    
    int CanPerformClusterMerge(){
        if(NumberOfClusters>1){
            return 1;
        }
        else{
            return 0;
        }
    }
    
    // UPDATE INFORMATION //
    void Update(){
        
        NumberOfClusters=ClusterList.size();
        
        NumberOfSplittableClusters=0; NumberOfExchangeableClusters=0; S=0.0;
        
        for(int i=0;i<ClusterList.size();i++){
            
            if(ClusterList[i].isSplittable){
                NumberOfSplittableClusters++;
            }
            
            if(ClusterList[i].isExchangeable){
                NumberOfExchangeableClusters++;
            }
            
            S+=ClusterList[i].S;
            NumberOfParticles+=ClusterList[i].NumberOfParticles;
            
        }
        
        NumberOfOptions=CanPerformClusterExchange()+CanPerformClusterSplit()+CanPerformClusterMerge();
    }
    
    // PRINT CLUSTER CONFIGURATION //
    void Print(){
        for(int i=0;i<ClusterList.size();i++){
            for(int j=0;j<ClusterList.at(i).NumberOfParticles;j++){
                std::cerr << ClusterList.at(i).ParticleIDList.at(j) << " ";
            }
            std::cerr << std::endl;
        }
    }
    
    
    // GENERATE LES HOUCHES FILES //
    void GenerateLesHouchesFile(std::string fname){
        for(int i=0;i<ClusterList.size();i++){
            ClusterList.at(i).GenerateLesHouchesFile(fname,i);
        }
    }
    
    ClusterConfiguration(){
    };
    
    ~ClusterConfiguration(){};
    
    
};

