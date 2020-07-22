#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/binomial.hpp>

// DEFINE BINOMIAL COEFFICIENT //
inline double Binomial(double x,double y){ return boost::math::binomial_coefficient<double>(x,y);}

namespace ClusteringAlgorithm {

    ///////////////////////////////////
    // BASIC OPERATIONS ON CLUSTERS //
    //////////////////////////////////
    
    void GetClusterSplitProposal(Cluster &OldClusterOne,Cluster &NewClusterOne, Cluster &NewClusterTwo,int NSplit){
        
        // CREATE PROPOSAL FOR NEW CLUSTERS //
        std::vector<int> ShuffledParticles(OldClusterOne.ParticleIDList.begin(),OldClusterOne.ParticleIDList.end());
        std::random_shuffle(ShuffledParticles.begin(), ShuffledParticles.end());
        
        std::vector<int> ExchangedParticles(ShuffledParticles.begin(),ShuffledParticles.begin()+NSplit);
        std::vector<int> RemainingParticles(ShuffledParticles.begin()+NSplit,ShuffledParticles.end());
        
        NewClusterOne.ParticleIDList=RemainingParticles;
        NewClusterTwo.ParticleIDList=ExchangedParticles;
        
        // CALCULATE CLUSTER PROPERTIES //
        NewClusterOne.Update(); NewClusterTwo.Update();
        
    }
    
    void GetClusterSplitProposal(ClusterConfiguration &OldClusterConfiguration,ClusterConfiguration &NewClusterConfiguration,double &SamplingBias){
        
        // SELECT ONE RANDOM CLUSTERS BY SHUFFLING THE CLUSTER VECTOR AND SELECTING THE FIRST SPLITTABLE ONE //
        std::random_shuffle(OldClusterConfiguration.ClusterList.begin(),OldClusterConfiguration.ClusterList.end());

        Cluster OldClusterOne; int OldClusterPosition;
        
        for(int i=0;i<OldClusterConfiguration.ClusterList.size();i++){
            
            if(OldClusterConfiguration.ClusterList.at(i).isSplittable==true){
                OldClusterOne=OldClusterConfiguration.ClusterList.at(i); OldClusterPosition=i;
                break;
            }
            
        }
        
        
        // SELECT HOW MANY PARTICLES OF THE CLUSTER SHOULD BE SPLIT OF //
        int NSplitMin=ClusteringParameters::MininalParticleNumberPerCluster; int NSplitMax=OldClusterOne.NumberOfParticles/2; int NSplitRange=NSplitMax-NSplitMin+1;
        int NSplit=NSplitMin+rand()%NSplitRange;
        
        
        // NEW CLUSTERS //
        Cluster NewClusterOne;  Cluster NewClusterTwo;
        
        // GET PROPOSAL FOR NEW CLUSTERS //
        GetClusterSplitProposal(OldClusterOne,NewClusterOne,NewClusterTwo,NSplit);
        
        // SET PROPOSAL FOR NEW CLUSTER CONFIGURATION //
        NewClusterConfiguration=OldClusterConfiguration;
        
        // ERASE THE OLD CLUSTERS AND ADD NEW CLUSTERS //
        NewClusterConfiguration.ClusterList.erase(NewClusterConfiguration.ClusterList.begin()+OldClusterPosition,NewClusterConfiguration.ClusterList.begin()+OldClusterPosition+1);
        NewClusterConfiguration.ClusterList.push_back(NewClusterOne);
        NewClusterConfiguration.ClusterList.push_back(NewClusterTwo);
        
        // CALCULATE CLUSTER CONFIGURATION PROPERTIES //
        NewClusterConfiguration.Update();
        
        // CALCULATE SELECTION PROBABILITY OF THE INVERSE PROCESS //
        double SelectionPossibilities=OldClusterConfiguration.NumberOfOptions*OldClusterConfiguration.NumberOfSplittableClusters*NSplitRange*Binomial(OldClusterOne.NumberOfParticles,NSplit);

        
        if(2*NSplit==OldClusterOne.NumberOfParticles){
            SelectionPossibilities/=2;
        }
        
        double InverseSelectionPossibilities=NewClusterConfiguration.NumberOfOptions*NewClusterConfiguration.NumberOfClusters*(NewClusterConfiguration.NumberOfClusters-1)/2.0;
        
        // SET SAMPLING BIAS //
        SamplingBias=InverseSelectionPossibilities/SelectionPossibilities;
        
//        // COMMANDLINE OUTPUT //
//        std::cerr << "#PROPOSING SPLIT " << SelectionPossibilities << " " << InverseSelectionPossibilities  << " " << SamplingBias << std::endl;
//
//        std::cerr << "#OLD CLUSTER CONFIGRUATION" << std::endl;
//        OldClusterConfiguration.Print();
//        std::cerr << "#NEW CLUSTER CONFIGRUATION" << std::endl;
//        NewClusterConfiguration.Print();
//        std::cerr << 1.0/SelectionPossibilities << " " << 1.0/InverseSelectionPossibilities << std::endl;
//        std::cerr << std::endl;
//        std::cerr << std::endl;

        
    }
    
    void GetClusterMergeProposal(Cluster &OldClusterOne,Cluster &OldClusterTwo,Cluster &NewClusterOne){
        
        // CREATE PROPOSAL FOR NEW CLUSTERS //
        NewClusterOne.ParticleIDList.assign(OldClusterOne.ParticleIDList.begin(),OldClusterOne.ParticleIDList.end());
        NewClusterOne.ParticleIDList.insert(NewClusterOne.ParticleIDList.end(), OldClusterTwo.ParticleIDList.begin(), OldClusterTwo.ParticleIDList.end());
        
        // CALCULATE CLUSTER PROPERTIES //
        NewClusterOne.Update();
        
    }
    
    void GetClusterMergeProposal(ClusterConfiguration &OldClusterConfiguration,ClusterConfiguration &NewClusterConfiguration,double &SamplingBias){
        
        // SELECT TWO RANDOM CLUSTERS BY SHUFFLING THE CLUSTER VECTOR AND SELECTING THE FIRST TWO //
        std::random_shuffle(OldClusterConfiguration.ClusterList.begin(),OldClusterConfiguration.ClusterList.end());

        Cluster OldClusterOne=OldClusterConfiguration.ClusterList.at(0);
        Cluster OldClusterTwo=OldClusterConfiguration.ClusterList.at(1);
        
        // NEW CLUSTERS //
        Cluster NewClusterOne;
        
        // GET PROPOSAL FOR NEW CLUSTERS //
        GetClusterMergeProposal(OldClusterOne,OldClusterTwo,NewClusterOne);
        
        // SET PROPOSAL FOR NEW CLUSTER CONFIGURATION //
        NewClusterConfiguration=OldClusterConfiguration;

        // ERASE THE OLD CLUSTERS AND ADD NEW CLUSTERS //
        NewClusterConfiguration.ClusterList.erase(NewClusterConfiguration.ClusterList.begin(),NewClusterConfiguration.ClusterList.begin()+2);
        NewClusterConfiguration.ClusterList.push_back(NewClusterOne);
        
        // CALCULATE CLUSTER CONFIGURATION PROPERTIES //
        NewClusterConfiguration.Update();
        
        // CALCULATE SELECTION PROBABILITY //
        double SelectionPossibilities=OldClusterConfiguration.NumberOfOptions*OldClusterConfiguration.NumberOfClusters*(OldClusterConfiguration.NumberOfClusters-1)/2.0;
        
        // CALCULATE SELECTION PROBABILITY OF THE INVERSE PROCESS //
        int NSplitMin=ClusteringParameters::MininalParticleNumberPerCluster; int NSplitMax=NewClusterOne.NumberOfParticles/2; int NSplitRange=NSplitMax-NSplitMin+1; int NSplit=std::min(OldClusterOne.NumberOfParticles,OldClusterTwo.NumberOfParticles);
        
        double InverseSelectionPossibilities=NewClusterConfiguration.NumberOfOptions*NewClusterConfiguration.NumberOfSplittableClusters*NSplitRange*Binomial(NewClusterOne.NumberOfParticles,NSplit);

        
        if(2*NSplit==NewClusterOne.NumberOfParticles){
            InverseSelectionPossibilities/=2;
        }
        
        // SET SAMPLING BIAS //
        SamplingBias=InverseSelectionPossibilities/SelectionPossibilities;
        
//        // COMMANDLINE OUTPUT //
//        std::cerr << "#PROPOSING MERGING " << SelectionPossibilities << " " << InverseSelectionPossibilities  << " " << SamplingBias << std::endl;
//
//        std::cerr << "#OLD CLUSTER CONFIGRUATION" << std::endl;
//        OldClusterConfiguration.Print();
//        std::cerr << "#NEW CLUSTER CONFIGRUATION" << std::endl;
//        NewClusterConfiguration.Print();
//        std::cerr << 1.0/SelectionPossibilities << " " << 1.0/InverseSelectionPossibilities << std::endl;
//        std::cerr << std::endl;
//        std::cerr << std::endl;
    }
    
    void GetClusterExchangeProposal(Cluster &OldClusterOne,Cluster &OldClusterTwo,Cluster &NewClusterOne,Cluster &NewClusterTwo, int NExchange){
        
        // CREATE PROPOSAL FOR NEW CLUSTERS //
        std::vector<int> ShuffledParticles = OldClusterOne.ParticleIDList;
        std::random_shuffle(ShuffledParticles.begin(), ShuffledParticles.end());
        
        
        std::vector<int> ExchangedParticles(ShuffledParticles.begin(),ShuffledParticles.begin()+NExchange);
        std::vector<int> RemainingParticles(ShuffledParticles.begin()+NExchange,ShuffledParticles.end());
        
        
        NewClusterOne.ParticleIDList = RemainingParticles;
        
        
        NewClusterTwo.ParticleIDList = OldClusterTwo.ParticleIDList;
        NewClusterTwo.ParticleIDList.insert(NewClusterTwo.ParticleIDList.end(),ExchangedParticles.begin(), ExchangedParticles.end());
        
        
        // CALCULATE CLUSTER PROPERTIES //
        NewClusterOne.Update(); NewClusterTwo.Update();
        
    }
    
    void GetClusterExchangeProposal(ClusterConfiguration &OldClusterConfiguration,ClusterConfiguration &NewClusterConfiguration,double &SamplingBias){
        
        // SELECT TWO RANDOM CLUSTERS BY SHUFFLING THE CLUSTER VECTOR AND SELECTING THE FIRST TWO //
        std::random_shuffle(OldClusterConfiguration.ClusterList.begin(),OldClusterConfiguration.ClusterList.end());
        
        Cluster OldClusterOne; int OldClusterOnePosition;
        Cluster OldClusterTwo; int OldClusterTwoPosition;
        
        // SELECT FIRST SPLITTABLE CLUSTER //
        for(int i=0;i<OldClusterConfiguration.ClusterList.size();i++){
            
            if(OldClusterConfiguration.ClusterList.at(i).isExchangeable==true){
                OldClusterOne=OldClusterConfiguration.ClusterList.at(i); OldClusterOnePosition=i;
                break;
            }
            
        }
        
        // SELECT SECOND CLUSTER DIFFERENT FROM FIRST ONE //
        for(int i=0;i<OldClusterConfiguration.ClusterList.size();i++){
            
            if(i!=OldClusterOnePosition){
                OldClusterTwo=OldClusterConfiguration.ClusterList.at(i); OldClusterTwoPosition=i;
                break;
            }
            
        }
        
        // NEW CLUSTERS //
        Cluster NewClusterOne; Cluster NewClusterTwo;
        
        // SELECT HOW MANY PARTICLES OF THE CLUSTER SHOULD BE SPLIT OF //
        int NExchangeMin=1; int NExchangeMax=OldClusterOne.NumberOfParticles-ClusteringParameters::MininalParticleNumberPerCluster; int NExchangeRange=NExchangeMax-NExchangeMin+1;
        int NExchange=NExchangeMin+rand()%NExchangeRange;
        
        // GET PROPOSAL FOR NEW CLUSTERS //
        GetClusterExchangeProposal(OldClusterOne,OldClusterTwo,NewClusterOne,NewClusterTwo,NExchange);
        
        // SET PROPOSAL FOR NEW CLUSTER CONFIGURATION //
        NewClusterConfiguration=OldClusterConfiguration;
        
        // ERASE THE OLD CLUSTERS AND ADD NEW CLUSTERS //
        NewClusterConfiguration.ClusterList.erase(NewClusterConfiguration.ClusterList.begin()+std::max(OldClusterOnePosition,OldClusterTwoPosition),NewClusterConfiguration.ClusterList.begin()+std::max(OldClusterOnePosition,OldClusterTwoPosition)+1);
        NewClusterConfiguration.ClusterList.erase(NewClusterConfiguration.ClusterList.begin()+std::min(OldClusterOnePosition,OldClusterTwoPosition),NewClusterConfiguration.ClusterList.begin()+std::min(OldClusterOnePosition,OldClusterTwoPosition)+1);

        NewClusterConfiguration.ClusterList.push_back(NewClusterOne);
        NewClusterConfiguration.ClusterList.push_back(NewClusterTwo);
        
        // CALCULATE CLUSTER CONFIGURATION PROPERTIES //
        NewClusterConfiguration.Update();
        
        // CALCULATE SELECTION PROBABILITY //
        double SelectionPossibilities=OldClusterConfiguration.NumberOfOptions*OldClusterConfiguration.NumberOfExchangeableClusters*(OldClusterConfiguration.NumberOfClusters-1)*NExchangeRange*Binomial(OldClusterOne.NumberOfParticles,NExchange);
        
        // CALCULATE SELECTION PROBABILITY OF THE INVERSE PROCESS //
        int NInverseExchangeMin=1; int NInverseExchangeMax=NewClusterTwo.NumberOfParticles-ClusteringParameters::MininalParticleNumberPerCluster; int NInverseExchangeRange=NInverseExchangeMax-NInverseExchangeMin+1;

        double InverseSelectionPossibilities=NewClusterConfiguration.NumberOfOptions*NewClusterConfiguration.NumberOfExchangeableClusters*(NewClusterConfiguration.NumberOfClusters-1)*NInverseExchangeRange*Binomial(NewClusterTwo.NumberOfParticles,NExchange);
        
        // SET SAMPLING BIAS //
        SamplingBias=InverseSelectionPossibilities/SelectionPossibilities;
        
//        // COMMANDLINE OUTPUT //
//        std::cerr << "#PROPOSING EXCHANGE " << SelectionPossibilities << " " << InverseSelectionPossibilities  << " " << SamplingBias << std::endl;
//
//        std::cerr << "#OLD CLUSTER CONFIGRUATION" << std::endl;
//        OldClusterConfiguration.Print();
//        std::cerr << "#NEW CLUSTER CONFIGRUATION" << std::endl;
//        NewClusterConfiguration.Print();
//        std::cerr << 1.0/SelectionPossibilities << " " << 1.0/InverseSelectionPossibilities << std::endl;
//        std::cerr << std::endl;
//        std::cerr << std::endl;
        
    }
        
    int ExchangeProposals=0; int SplitProposals=0; int MergeProposals=0;
    int ExchangeProposalsAccepted=0; int SplitProposalsAccepted=0; int MergeProposalsAccepted=0;

    int Step(){
        
        // NEW CLUSTER PROPOSAL //
        ClusterConfiguration OldClusters;
        ClusterConfiguration NewClusters;
        
        OldClusters.ClusterList.assign(Clusters.ClusterList.begin(),Clusters.ClusterList.end());
        NewClusters.ClusterList.assign(Clusters.ClusterList.begin(),Clusters.ClusterList.end());
        
        OldClusters.Update();
        NewClusters.Update();
        
        double SamplingBias=1.0;
        
        // GENERATE PROPOSAL //
        double r=drand48();
        
        int ExchangeProposal=0;
        int SplitProposal=0;
        int MergeProposal=0;
        

        // SELECT VALID OPERATION TO BE PERFORMED ON THE CURRENT CONFIGURATION //
        if(r<1.0/3.0){
            if(OldClusters.CanPerformClusterExchange()==1){
                GetClusterExchangeProposal(OldClusters,NewClusters,SamplingBias);
                ExchangeProposal=1;
            }
            else{
                return 0;
            }
        }
        else if(r<2.0/3.0){
            if(OldClusters.CanPerformClusterSplit()==1){
                GetClusterSplitProposal(OldClusters,NewClusters,SamplingBias);
                SplitProposal=1;
            }
            else{
                return 0;
            }
        }
        else{
            if(OldClusters.CanPerformClusterMerge()==1){
                GetClusterMergeProposal(OldClusters,NewClusters,SamplingBias);
                MergeProposal=1;
            }
            else{
                return 0;
            }
        }
        
        // PROPOSAL STATISTIC //
        ExchangeProposals+=ExchangeProposal;
        SplitProposals+=SplitProposal;
        MergeProposals+=MergeProposal;

        
        // CALCULATE ACCEPTANCE PROBABILITY //
        double AcceptanceProbability=std::exp(-(NewClusters.S-OldClusters.S))/SamplingBias;
        
        // ACCEPT/REJECT PROPOSAL //
        if(drand48()<AcceptanceProbability){
            
            // ACCEPT PROPOSAL //
            Clusters=NewClusters;
            
            // PROPOSAL ACCEPTANCE STATISTIC //
            ExchangeProposalsAccepted+=ExchangeProposal;
            SplitProposalsAccepted+=SplitProposal;
            MergeProposalsAccepted+=MergeProposal;
        }
        
        return 1;
        
    }
    
    void Setup(){
        
        std::cerr << "#SETTING UP INITIAL CLUSTERS" << std::endl;
        
        // CLEAR CLUSTER LIST //
        Clusters.ClusterList.clear();
        
        // CREATE ONE BIG CLUSTER //
        Cluster BigCluster;
        BigCluster.ParticleIDList.clear();
        
        for (int i=0;i<GlobalPartonList.size();i++){
            BigCluster.ParticleIDList.push_back(i);
        }
        
        BigCluster.Update();
        
        // CREATE CLUSTER CONFIGURATION WITH ONE BIG CLUSTER //
        Clusters.ClusterList.push_back(BigCluster);
        Clusters.Update();
        
        
    }
    
    void Cluster(std::string ofname,std::string efname,int NumberOfSteps){
        
        srand48(time(0));
        
        // COMMANDLINE OUTPUT //
        std::cerr << "#STARTING CLUSTERING ALGORITHM" << std::endl;
        ClusteringParameters::Print();
        
        // OUTPUT //
        std::ofstream OutStream; OutStream.open(ofname);
        std::ofstream ErrStream; ErrStream.open(efname);

        // STEP COUNTER //
        int StepCounter=0;
        
        // CLUSTERING STATISTICS //
        double AvgNCl=0.0; double AvgNClSqr=0.0; double Counts=0.0;
        
        double *ClusterHistogram=new double[GlobalPartonList.size()+1];
        
        for(int i=0;i<=GlobalPartonList.size();i++){
            ClusterHistogram[i]=0.0;
        }
        
        
        while(StepCounter<NumberOfSteps){
          //std::cout << StepCounter << std::endl;
            int Success=0;
            
            while (Success==0) {
                Success=Step();
            }
            
            StepCounter++;
            
            // if(StepCounter%1000==0){
            //     ErrStream << "#STEP -- NUMBER OF CLUSTERS -- NUMBER OF SPLITTABLE CLUSTERS -- NUMBER OF EXCHANGABLE CLUSTERS -- ACTION" << std::endl;
            //     ErrStream << StepCounter << " " << Clusters.NumberOfClusters << " " << Clusters.NumberOfSplittableClusters << " " << Clusters.NumberOfExchangeableClusters <<  " " << Clusters.S << std::endl;
            // }
            
            // if(StepCounter%1000==0){
                
            //     ErrStream << "#EXCHANGE PROPOSALS -- SPLIT PROPOSALS -- MERGE PROPOSALS" << std::endl;
            //     ErrStream << ExchangeProposals << " " << SplitProposals << " " << MergeProposals << std::endl;
                
            //     ErrStream << "#EXCHANGE ACCEPTANCE -- SPLIT ACCEPTANCE  -- MERGE ACCEPTANCE" << std::endl;
            //     ErrStream << ExchangeProposalsAccepted/double(ExchangeProposals) << " " << SplitProposalsAccepted/double(SplitProposals) << " " << MergeProposalsAccepted/double(MergeProposals) << std::endl;


            // }
            
//            if(StepCounter%1000==0){
//                std::cerr << "#CLUSTER CONFIGURATION AFTER STEP " << StepCounter << std::endl;
//                Clusters.Print();
//            }
            
//            OutStream << StepCounter << " " << Clusters.NumberOfClusters << " " << Clusters.NumberOfSplittableClusters << " " << Clusters.NumberOfExchangeableClusters << " " << Clusters.S << std::endl;
            
            
            if(StepCounter>10000){
                AvgNCl+=Clusters.NumberOfClusters; AvgNClSqr+=Clusters.NumberOfClusters*Clusters.NumberOfClusters; Counts++;
                ClusterHistogram[Clusters.NumberOfClusters]++;
            }

            
        }
        

        
        // // CREATE OUTPUT //
        // std::cerr << "##### <NCl>=" << AvgNCl/Counts << " +/- " << std::sqrt((AvgNClSqr/Counts-(AvgNCl/Counts)*(AvgNCl/Counts))/(Counts-1)) << std::endl;
        
        // std::cerr << "##### CLUSTERING STATISTICS -- NCl -- P(NCl)" << std::endl;
        // for(int i=0;i<=GlobalPartonList.size();i++){
        //     if(ClusterHistogram[i]!=0.0){
        //         std::cerr << i << " " << ClusterHistogram[i]/Counts << std::endl;
        //     }
        // }
        // std::cerr << "##### CLUSTER CONFIGURATION AFTER STEP " << StepCounter << std::endl;
        // Clusters.Print();
        
        delete[] ClusterHistogram;
        
    }

    
    
}
