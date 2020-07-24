class DiscreteSpectrum{
    
    //////////////
    // INDEXING //
    //////////////
    
    
    // VALUE TO LOWER X-INDEX  //
public:
    INT GetXLowPos(DOUBLE pxGev){
        
        DOUBLE pxLat=pxGev*(INVERSE_GEV_SCALE/Parameters::ProtonRadiusScale);
        
        INT pxPos=INT((Lattice::N[0]/M_PI)*asin(0.5*fabs(pxLat)));
        
        if(pxLat>=0){
            return pxPos;
        }
        else{
            return Lattice::N[0]-pxPos-1;
        }
    }
    
    // VALUE TO LOWER Y-INDEX //
public:
    INT GetYLowPos(DOUBLE pyGev){
        
        DOUBLE pyLat=pyGev*(INVERSE_GEV_SCALE/Parameters::ProtonRadiusScale);
        
        INT pyPos=INT((Lattice::N[1]/M_PI)*asin(0.5*fabs(pyLat)));
        
        if(pyLat>=0){
            return pyPos;
        }
        else{
            return Lattice::N[1]-pyPos-1;
        }
        
    }
    
    
    // VALUE TO X-INDEX //
public:
    DOUBLE GetX(INT xPos){
        
        DOUBLE pxLat=2.0*sin(M_PI*DOUBLE(xPos)/DOUBLE(Lattice::N[0]));
        
        DOUBLE pxGev=pxLat*(Parameters::ProtonRadiusScale/INVERSE_GEV_SCALE);
        
        if(xPos<=Lattice::N[0]/2){
            return pxGev;
        }
        else{
            return -pxGev;
        }
    }
    
    // VALUE TO Y-INDEX //
public:
    DOUBLE GetY(INT yPos){
        
        DOUBLE pyLat=2.0*sin(M_PI*DOUBLE(yPos)/DOUBLE(Lattice::N[1]));
        
        DOUBLE pyGev=pyLat*(Parameters::ProtonRadiusScale/INVERSE_GEV_SCALE);
        
        
        if(yPos<=Lattice::N[1]/2){
            return pyGev;
        }
        else{
            return -pyGev;
        }
    }
    
    
    // MULTI-DIMENSIONAL INDEXING //
public:
    INT Index(INT x,INT y){
        
        if(x>=0 && x<Lattice::N[0] && y>=0 && y<Lattice::N[1]){
            return x+Lattice::N[0]*y;
        }
        
        else{
            std::cerr << "#INDEX OUT OF RANGE " << x << " " << y << std::endl;
            exit(0);
        }
        
    }
    
    
    /////////////////////
    // DATA MANAGEMENT //
    /////////////////////
    
    // DISCRETE VALUES //
public:
    DOUBLE *Values;
    
    
    /////////////////////
    // I/O MANAGEMENT  //
    /////////////////////
    
    // LOAD DISCRETE SPECTRUM FROM FILE //
private:
    void Load(std::string fname){
        
        //LOAD FROM FILE
        std::ifstream VIn;
        
        VIn.open(fname.c_str());
        
        std::string VLine;
        
        // GET MOMENTUM VALUES //
        DOUBLE pxIN,pyIN;
        
        // GET MOMENTUM INDICES //
        INT pxINPos,pyINPos;
        
        //NUMBER OF LINES READ FROM FILE
        INT InputCount=0;
        
        //GET DATA FROM INPUT FILES
        while(VIn.good()){
            
            //READ LINES
            getline(VIn,VLine);
            
            //PROCESS FILE LINE BY LINE
            if(!(VLine.empty())){
                
                //STRING TOKENIZE
                std::stringstream VLineValues(VLine);
                
                // GET POSITIONS IN FILE //
                VLineValues >> pxIN; VLineValues >> pyIN;
                                
                pxINPos=GetXLowPos(pxIN); pyINPos=GetYLowPos(pyIN);
                
                if(fabs(GetX(pxINPos)-pxIN)>fabs(GetX(pxINPos+1)-pxIN)){
                    pxINPos++;
                }
                
                if(fabs(GetY(pyINPos)-pyIN)>fabs(GetY(pyINPos+1)-pyIN)){
                    pyINPos++;
                }
                
                
                if(fabs(GetX(pxINPos)-pxIN)>fabs(GetX(pxINPos-1)-pxIN)){
                    pxINPos--;
                }
                
                if(fabs(GetY(pyINPos)-pyIN)>fabs(GetY(pyINPos-1)-pyIN)){
                    pyINPos-- ;
                }
                                
                //CHECK POSITIONS AND SET VALUES TO WILSON LINE ARRAY
                if(InputCount==Index(pxINPos,pyINPos)){
                    
                    VLineValues >> Values[Index(pxINPos,pyINPos)];
                    
                }
                
                //BREAK IF POSITIONS DO NOT MATCH
                else{
                    std::cerr << "#SPECTRUM INPUT CAUSED FATAL ERROR -- POSITIONS DO NOT MATCH" << std::endl;
                    exit(0);
                }
                
                //INCREASE POSITION COUNT
                InputCount++;
            }
        }
        
        // CHECK THAT SPECTRUM WAS LOADED CORRECTLY //
        if(InputCount!=Lattice::N[0]*Lattice::N[1]){
            std::cerr << "#SPECTRUM INPUT CAUSED FATAL ERROR -- COULD NOT LOAD DATA CORRECTLY" << std::endl;
            exit(0);
        }
        
    }
    
    void LoadSmeared(std::string fname){
        
        //LOAD FROM FILE
        std::ifstream VIn;
        
        VIn.open(fname.c_str());
        
        std::string VLine;
        
        // GET MOMENTUM VALUES //
        DOUBLE pxIN,pyIN;
        
        // GET MOMENTUM INDICES //
        INT pxINPos,pyINPos;
        
        //NUMBER OF LINES READ FROM FILE
        INT InputCount=0;
        
        //GET DATA FROM INPUT FILES
        while(VIn.good()){
            
            //READ LINES
            getline(VIn,VLine);
            
            //PROCESS FILE LINE BY LINE
            if(!(VLine.empty())){
                
                //STRING TOKENIZE
                std::stringstream VLineValues(VLine);
                
                // GET POSITIONS IN FILE //
                VLineValues >> pxIN; VLineValues >> pyIN;
                
                pxINPos=GetXLowPos(pxIN); pyINPos=GetYLowPos(pyIN);
                
                if(fabs(GetX(pxINPos)-pxIN)>fabs(GetX(pxINPos+1)-pxIN)){
                    pxINPos++;
                }
                
                if(fabs(GetY(pyINPos)-pyIN)>fabs(GetY(pyINPos+1)-pyIN)){
                    pyINPos++;
                }
                
                
                if(fabs(GetX(pxINPos)-pxIN)>fabs(GetX(pxINPos-1)-pxIN)){
                    pxINPos--;
                }
                
                if(fabs(GetY(pyINPos)-pyIN)>fabs(GetY(pyINPos-1)-pyIN)){
                    pyINPos-- ;
                }
                
                //CHECK POSITIONS AND SET VALUES TO WILSON LINE ARRAY
                VLineValues >> Values[Index(pxINPos,pyINPos)];
                
                //INCREASE POSITION COUNT
                InputCount++;
            }
        }
        
        std::cerr << "#SMEARED SPECTRUM LOADED WITH " << InputCount << " INPUT COUNTS" << std::endl;

        
    }
    
    // SAVE DISCRETE SPECTRUM //
    
public:
    
    void Save(std::string fname){
        
        std::ofstream OutStream;
        OutStream.open(fname.c_str());
        
        DOUBLE pXValue,pYValue; COMPLEX cDpx,cDpy;
        
        for(INT pYIndex=0;pYIndex<=Lattice::N[1]-1;pYIndex++){
            for(INT pXIndex=0;pXIndex<=Lattice::N[0]-1;pXIndex++){
                
                // GET MOMENTA //
                GetTransverseMomenta(pXIndex,pYIndex,cDpx,cDpy);
                
                pXValue=SIGN(real(cDpx))*sqrt(Dynamics::gUpMetric[0]*SQR(Lattice::ScaleConstant)*SQR_ABS(cDpx))*(GEV_SCALE);
                pYValue=SIGN(real(cDpy))*sqrt(Dynamics::gUpMetric[1]*SQR(Lattice::ScaleConstant)*SQR_ABS(cDpy))*(GEV_SCALE);
                
                // CREATE OUTPUT //
                OutStream << pXValue << " " << pYValue << " " << Values[Index(pXIndex,pYIndex)] << std::endl;
                
                
            }
            
            OutStream << std::endl;
        }
        
        OutStream.close();
        
    }
    
    ////////////////////////
    // GET AND SET VALUES //
    ////////////////////////
    
    void SetValue(INT pXIndex,INT pYIndex,DOUBLE MyValue){
        
        Values[this->Index(pXIndex,pYIndex)]=MyValue;
    }
    
    DOUBLE GetValue(INT pXIndex,INT pYIndex){
        return Values[this->Index(pXIndex,pYIndex)];
    }
    
    /////////////////////////
    // OVERALL MULTIPLICTY //
    /////////////////////////
    
    DOUBLE GetdNOverdY(DOUBLE pTCutLow,DOUBLE pTCutHigh){
        
        // MOMENTA //
        DOUBLE pXValue,pYValue; COMPLEX cDpx,cDpy;
        
        // NORMALIZATION FACTOR //
        DOUBLE MomentumSpaceVolumeFactor=SQR(2.0*M_PI)/(Lattice::N[0]*Lattice::N[1]*SQR(INVERSE_GEV_SCALE/Parameters::ProtonRadiusScale));
        
        
        // GLUON MULTIPLICITY dN/dy PER UNIT RAPIDITY //
        DOUBLE dNOverdY=0.0;
        
        
        // COMPUTE dN/dy //
        for(INT pYIndex=0;pYIndex<=Lattice::N[1]-1;pYIndex++){
            for(INT pXIndex=0;pXIndex<=Lattice::N[0]-1;pXIndex++){
                
                // GET MOMENTA //
                GetTransverseMomenta(pXIndex,pYIndex,cDpx,cDpy);
                
                pXValue=SIGN(real(cDpx))*sqrt(Dynamics::gUpMetric[0]*SQR(Lattice::ScaleConstant)*SQR_ABS(cDpx))*(GEV_SCALE);
                pYValue=SIGN(real(cDpy))*sqrt(Dynamics::gUpMetric[1]*SQR(Lattice::ScaleConstant)*SQR_ABS(cDpy))*(GEV_SCALE);
        
                // APPLY KINEMATIC CUTS //
                if((SQR(pXValue)+SQR(pYValue))>=SQR(pTCutLow) && (SQR(pXValue)+SQR(pYValue))<=SQR(pTCutHigh)){
        
                    dNOverdY+=GetValue(pXIndex,pYIndex)*MomentumSpaceVolumeFactor;
                }
                
            }
        }
        
        // RETURN VALUE //
        return dNOverdY;
        
    }
    
    
    ///////////////////////
    // OBJECT MANAGEMENT //
    ///////////////////////
    
    // CONSTRUCTORS //
public:
    
    DiscreteSpectrum(){
        
        // ALLOCATE MEMEORY //
        Values=new DOUBLE[Lattice::N[0]*Lattice::N[1]];
        
        for(int i=0;i<Lattice::N[0]*Lattice::N[1];i++){
            Values[i]=0.0;
        }
        
    }
    
    DiscreteSpectrum(std::string fname,int Smearing){
        
        // ALLOCATE MEMEORY //
        Values=new DOUBLE[Lattice::N[0]*Lattice::N[1]];
        
        for(int i=0;i<Lattice::N[0]*Lattice::N[1];i++){
            Values[i]=0.0;
        }
        
        // LOAD SPECTRUM //
        if(Smearing==0){
            Load(fname);
        }
        else if(Smearing==1){
            LoadSmeared(fname);
        }
        else{
            std:cerr << "#SMAERING NOT SPECIFIED" << std::endl;
            exit(0);
        }
        
    };
    
    // DESTRUCTOR //
public:
    ~DiscreteSpectrum(){
        delete[] Values;
    }
    
    
};
