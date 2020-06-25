namespace IPGlasmaInterface{
    
    static const double M_HBARC=0.197;
    
    double *pXValues;
    double *pYValues;
    
    double *xValues;
    double *yValues;
    
    double *dNdyd2pd2xValues;
    double *dNdyd2pd2xCumulativeProbability;
        
    double dNdy;
    
    double tau=0.2005;
    
    double etaMin=-2.0;
    double etaMax=+2.0;
    
    int Ns=128;
    
    int NpX=64; int NpY=64;
    int NsX=32; int NsY=32;
    
    double aXfm=0.02;
    double aYfm=0.02;
    
    double aXGeV=aXfm/M_HBARC;
    double aYGeV=aYfm/M_HBARC;

    
    inline int Index4D(int x,int y,int pX,int pY){
        return pY+NpY*(pX+NpX*(y+NsY*x));
    }
    
    inline int pIndexWrap(int p,int N){
        int pWrap;
        
        if(p<=N/2){
            pWrap=p;
        }
        else{
            pWrap=p-N;
        }
        
        return pWrap+(N/2-1);
        
    }
    
    void Setup(std::string EventClass,int EventID){
        
        // GET SMEARED DISTRIBUTION FROM IP-GLASMA EVENT OUTPUT //
        dNdyd2pd2xValues=new double[NsX*NsY*NpX*NpY];
        
        pXValues=new double[NpX];
        pYValues=new double[NpY];
        
        xValues=new double[NsX];
        yValues=new double[NsY];

        
        for(int xIndex=0;xIndex<NsX;xIndex++){
            for(int yIndex=0;yIndex<NsY;yIndex++){
                
                int xLow=4*xIndex; int xHigh=4*(xIndex+1)-1;
                int yLow=4*yIndex; int yHigh=4*(yIndex+1)-1;
                
                xValues[xIndex]=0.5*((xLow+xHigh+1)-4*NsX)*aXfm;
                yValues[yIndex]=0.5*((yLow+yHigh+1)-4*NsY)*aYfm;

                // GET RELEVANT INPUT FILE //
                std::string fname= "EVENTCLASSES/" + EventClass + "/EventID" + std::to_string(EventID) + "/SMEAREDDISTRIBUTION/SmearedDistributionT0.2005fm" + std::to_string(xLow) + "X" + std::to_string(xHigh) + "_" + std::to_string(yLow) + "Y" + std::to_string(yHigh) + "ID" + std::to_string(EventID) + ".txt";
                
                std::cerr << "#NOW LOADING " << fname << std::endl;

                std::ifstream InStream; InStream.open(fname);
                
                
                // GET DATA LINE BY LINE //
                int Counter=0; std::string InputLine;
                
                int pXIndex=0; int pYIndex=0;
                
                while(InStream.good()){
                    
                    // READ LINE //
                    getline(InStream,InputLine);
                    
                    if(!(InputLine.empty())){
                        
                        std::stringstream InputValues(InputLine);
                        
                        double pX; double pY; double dNdyd2pd2x;
                        
                        InputValues >> pX; InputValues >> pY; InputValues >> dNdyd2pd2x;
                        
                        dNdyd2pd2xValues[Index4D(xIndex,yIndex,pIndexWrap(pXIndex,NpX),pIndexWrap(pYIndex,NpY))]=dNdyd2pd2x;
                        
                        pXValues[pIndexWrap(pXIndex,NpX)]=pX;
                        pYValues[pIndexWrap(pYIndex,NpY)]=pY;
                        
                        pYIndex++; Counter++;
                                       
                        if(Counter%NpY==0){
                            pYIndex=0; pXIndex++;
                        }
                        
                    }
                    
                }
                
                InStream.close();
                
            }
            
        }
        
        
        // CACLULATE dN/dy AND CUMULATIVE PROBABILITIES FOR SAMPLING //
        dNdyd2pd2xCumulativeProbability=new double[NsX*NsY*NpX*NpY];
        
        // COMMANDLINE OUTPUT //
        std::cerr << "#SETTING UP SAMPLING OF IP-GLASMA EVENT" << std::endl;
        
        // CALCULATE TOTAL MULTIPLICITY dN/dy //
        dNdy=0.0;
        
        double NormalizationFactor=(2.0*M_PI)*(2.0*M_PI)/((Ns*aXGeV)*(Ns*aYGeV));
        
        for(int ix=0;ix<NsX;ix++){
            for(int iy=0;iy<NsY;iy++){
                for(int ipX=0;ipX<NpX;ipX++){
                    for(int ipY=0;ipY<NpY;ipY++){
                        dNdy+=NormalizationFactor*std::abs(dNdyd2pd2xValues[Index4D(ix,iy,ipX,ipY)]);
                    }
                }
            }
        }
        
        // COMMANDLINE OUTPUT //
        std::cerr << "#SETTING PROBABILITIES FOR g^2dN/dy=" << dNdy << std::endl;
        
        // SET NORMALIZED CUMULATIVE PROBABILITIES //
        double NormalizedProbability=0.0;
        
        for(int ix=0;ix<NsX;ix++){
            for(int iy=0;iy<NsY;iy++){
                for(int ipX=0;ipX<NpX;ipX++){
                    for(int ipY=0;ipY<NpY;ipY++){
                        
                        NormalizedProbability+=NormalizationFactor*std::abs(dNdyd2pd2xValues[Index4D(ix,iy,ipX,ipY)])/dNdy;
                        dNdyd2pd2xCumulativeProbability[Index4D(ix,iy,ipX,ipY)]=NormalizedProbability;
            
                    }
                }
            }
        }

        
    }
    
    // SAMPLE COORDINATES AND MOMENTA WITHIN A GIVEN BIN //
    double SampleX(int ix){
        
        double xLow,xHigh;
        
        if(ix!=0 && ix!=NsX-1){
            
            xLow=(xValues[ix-1]+xValues[ix])/2.0;
            xHigh=(xValues[ix+1]+xValues[ix])/2.0;
            
        }
        else if(ix==0){

            xLow=xValues[ix]-(xValues[ix+1]-xValues[ix])/2.0;
            xHigh=(xValues[ix+1]+xValues[ix])/2.0;
        }
        else if(ix==NsX-1){
            xLow=(xValues[ix-1]+xValues[ix])/2.0;
            xHigh=xValues[ix]+(xValues[ix]-xValues[ix-1])/2.0;
        }
        else{
            std::cerr << "#ERROR" << std::endl;
            exit(0);
        }
        
        return xLow+(xHigh-xLow)*drand48();

    }
    
    double SampleY(int iy){
        
        double yLow,yHigh;
        
        if(iy!=0 && iy!=NsY-1){
            
            yLow=(yValues[iy-1]+yValues[iy])/2.0;
            yHigh=(yValues[iy+1]+yValues[iy])/2.0;
            
        }
        else if(iy==0){
            
            yLow=yValues[iy]-(yValues[iy+1]-yValues[iy])/2.0;
            yHigh=(yValues[iy+1]+yValues[iy])/2.0;
        }
        else if(iy==NsY-1){
            yLow=(yValues[iy-1]+yValues[iy])/2.0;
            yHigh=yValues[iy]+(yValues[iy]-yValues[iy-1])/2.0;
        }
        else{
            std::cerr << "#ERROR" << std::endl;
            exit(0);
        }
        
        return yLow+(yHigh-yLow)*drand48();
        
    }
    
    double SamplePX(int ipX){
        
        double pXLow,pXHigh;
        
        if(ipX!=0 && ipX!=NpX-1){
            
            pXLow=(pXValues[ipX-1]+pXValues[ipX])/2.0;
            pXHigh=(pXValues[ipX+1]+pXValues[ipX])/2.0;
            
        }
        else if(ipX==0){
            
            pXLow=pXValues[ipX]-(pXValues[ipX+1]-pXValues[ipX])/2.0;
            pXHigh=(pXValues[ipX+1]+pXValues[ipX])/2.0;
        }
        else if(ipX==NpX-1){
            pXLow=(pXValues[ipX-1]+pXValues[ipX])/2.0;
            pXHigh=pXValues[ipX]+(pXValues[ipX]-pXValues[ipX-1])/2.0;
        }
        else{
            std::cerr << "#ERROR" << std::endl;
            exit(0);
        }
        
        return pXLow+(pXHigh-pXLow)*drand48();
        
    }
    
    double SamplePY(int ipY){
        
        double pYLow,pYHigh;
        
        if(ipY!=0 && ipY!=NpY-1){
            
            pYLow=(pYValues[ipY-1]+pYValues[ipY])/2.0;
            pYHigh=(pYValues[ipY+1]+pYValues[ipY])/2.0;
            
        }
        else if(ipY==0){
            
            pYLow=pYValues[ipY]-(pYValues[ipY+1]-pYValues[ipY])/2.0;
            pYHigh=(pYValues[ipY+1]+pYValues[ipY])/2.0;
        }
        else if(ipY==NpY-1){
            
            pYLow=(pYValues[ipY-1]+pYValues[ipY])/2.0;
            pYHigh=pYValues[ipY]+(pYValues[ipY]-pYValues[ipY-1])/2.0;
            
        }
        else{
            std::cerr << "#ERROR" << std::endl;
            exit(0);
        }
        
        return pYLow+(pYHigh-pYLow)*drand48();
        
    }
    
    int SampleSingleGluon(){
        
        double r=drand48();
        
        for(int ix=0;ix<NsX;ix++){
            for(int iy=0;iy<NsY;iy++){
                for(int ipX=0;ipX<NpX;ipX++){
                    for(int ipY=0;ipY<NpY;ipY++){
            
                        if(dNdyd2pd2xCumulativeProbability[Index4D(ix,iy,ipX,ipY)]>r){
                                                        
                            // GET X AND Y COORDINATES //
                            double x=SampleX(ix);
                            double y=SampleY(iy);
                            
                            // GET pX AND pY MOMENTA //
                            double pX=SamplePX(ipX);
                            double pY=SamplePY(ipY);
                            double pT=std::sqrt(pX*pX+pY*pY);
                            
                            // GET COORDINATE SPACE AND MOMENTUM SPACE RAPIDITY //
                            double eta=etaMin+(etaMax-etaMin)*drand48();
                            double yRap=eta;
                            
                            // GET t,z COORDINATES //
                            double t=tau*cosh(eta);
                            double z=tau*sinh(eta);
                            
                            // GET E,pZ MOMENTA //
                            double E=pT*cosh(yRap);
                            double pZ=pT*sinh(yRap);
      
                            // SET NEW PARTICLE //
                            Particle NewGluon;
                            
                            NewGluon.Mom.Px()=pX;
                            NewGluon.Mom.Py()=pY;
                            NewGluon.Mom.Pz()=pZ;
                            NewGluon.Mom.E() =E;
                            
                            NewGluon.Pos.X()=x;
                            NewGluon.Pos.Y()=y;
                            NewGluon.Pos.Z()=z;
                            NewGluon.Pos.T()=t;
                            
                            NewGluon.FLAVOR=gluon;
                                                        
                            // CREATE NEW GLUON IN GLOBAL PARTON LIST //
                            GlobalPartonList.push_back(NewGluon);
                            
                            return 1;
                            
                        }
                        
                    }
                }
                
            }
        }
        
        
        std::cerr << "#ERROR IN SAMPLING" << std::endl;
        exit(0);
        
    }
    
    void Sample(){
        
        // CLEAR GLOBAL PARTON LIST //
        GlobalPartonList.clear();
        
        // SAMPLE GLUONS FROM EVENT //
        for(int i=0;i<(etaMax-etaMin)*dNdy;i++){
            SampleSingleGluon();
        }
        
    }
    
}
