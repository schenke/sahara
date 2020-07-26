class InterpolatedSpectrum{
    
    /////////////////
    // BASIC SETUP //
    /////////////////
    
    // NUMBER OF PARTICLES PER RAPIDITY UNIT //
public:
    
    double dNOverdY;
    
    // NUMBER OF INTERPOLATION POINTS //
public:
    int Nk; int NPhi;
    
    // INTERPOLATION RANGE //
public:
    double kMin; double kMax;
    double PhiMin; double PhiMax;
    
    // BIN WIDTH //
public:
    double dK; double dPhi;
    
    // GET BIN SIZE IN kT //
public:
    double GetK(int ik){
        return (ik+0.5)*dK+kMin;
    }
 
public:
    int GetKBin(double k){
        
        if(k<kMin || k>kMax){
            return -1;
        }
        else{
            return int((k-kMin)/dK);
        }
    }
    
    // GET BIN SIZE IN phi //
public:
    double GetPhi(int iphi){
        return (iphi+0.5)*dPhi+PhiMin;
    }
    
public:
    int GetPhiBin(double phi){
        
        if(phi<PhiMin){
            return GetPhiBin(phi+2.0*M_PI);
        }
        if(phi>PhiMax){
            return GetPhiBin(phi-2.0*M_PI);
        }
        
        return int((phi-PhiMin)/dPhi);
        
    }
    
    
    ///////////////////////////
    // INTERPOLATION WEIGHTS //
    ///////////////////////////
    
private:
        
    double Wedge(double xGoal,double xOrigin,double DeltaX){
        
        return (1.0-fabs(xOrigin-xGoal)/DeltaX);
        
    }
    
    double Wedge(double xGoal,double yGoal,double xOrigin,double yOrigin,double DeltaX,double DeltaY){
        
        return Wedge(xGoal,xOrigin,DeltaX)*Wedge(yGoal,yOrigin,DeltaY);
        
    }
    
    ///////////////////////////
    // INTERPOLATION ROUTINE //
    ///////////////////////////
    
    double GetInterpolatedValue(DiscreteSpectrum *Spectrum,double x,double y){

        // GET NEIGHBORING POINTS AND SPACINGS //
        int xLowPos=Spectrum->GetXLowPos(x); int xHighPos=(xLowPos+1)%Lattice::N[0];
        int yLowPos=Spectrum->GetYLowPos(y); int yHighPos=(yLowPos+1)%Lattice::N[1];
        
        double xLow=Spectrum->GetX(xLowPos); double xHigh=Spectrum->GetX(xHighPos);
        double yLow=Spectrum->GetY(yLowPos); double yHigh=Spectrum->GetY(yHighPos);
        
        double DeltaX=xHigh-xLow; double DeltaY=yHigh-yLow;
        
        // CHECK THAT THIS WORKS //
        if(xLow>x || xHigh<x || yLow>y || yHigh<y){
            std::cerr << "#FATAL ERROR" << std::endl;
        }
        
        // COMPUTE WEIGHTED AVERAGE //
        return (Wedge(x,y,xLow,yLow,DeltaX,DeltaY)*Spectrum->Values[Spectrum->Index(xLowPos,yLowPos)]+Wedge(x,y,xLow,yHigh,DeltaX,DeltaY)*Spectrum->Values[Spectrum->Index(xLowPos,yHighPos)]+Wedge(x,y,xHigh,yLow,DeltaX,DeltaY)*Spectrum->Values[Spectrum->Index(xHighPos,yLowPos)]+Wedge(x,y,xHigh,yHigh,DeltaX,DeltaY)*Spectrum->Values[Spectrum->Index(xHighPos,yHighPos)]);
        
        
    }
    
    ///////////////////////
    // AVERAGING ROUTINE //
    ///////////////////////
    
    double GetAverageValue(DiscreteSpectrum *Spectrum,double kLow,double kHigh,double PhiLow,double PhiHigh,int NSamples){
        
        double res=0.0; double k,phi;
        
        for(int n=0;n<NSamples;n++){
            
            k=(kHigh-kLow)*drand48()+kLow; phi=(PhiHigh-PhiLow)*drand48()+PhiLow;
            
            res+=GetInterpolatedValue(Spectrum,k*cos(phi),k*sin(phi));
            
        }
        
        return res/NSamples;
        
    }
    
    /////////////////////////
    // INTEGRATION ROUTINE //
    /////////////////////////
    
    double GetIntegralValue(DiscreteSpectrum *Spectrum,double kLow,double kHigh,double PhiLow,double PhiHigh,int NSamples){
        
        double res=0.0; double k,phi;
        
        for(int n=0;n<NSamples;n++){
            
            k=(kHigh-kLow)*drand48()+kLow; phi=(PhiHigh-PhiLow)*drand48()+PhiLow;
            
            res+=k*GetInterpolatedValue(Spectrum,k*cos(phi),k*sin(phi));
            
        }
        
        return (kHigh-kLow)*(PhiHigh-PhiLow)*(res/NSamples);
        
    }

    

    ////////////////////////
    // DATA INTERPOLATION //
    ////////////////////////

public:
    double **Values;

public:
    
    void PerformInterpolation(DiscreteSpectrum *Spectrum){
        
        // RESET dN/dy //
        dNOverdY=0.0;
        
        // COMPUTE INTERPOLATED INTEGRALS //
        for(int ik=0;ik<Nk;ik++){
            for(int iphi=0;iphi<NPhi;iphi++){
                
                // GET MOMENTUM kT AND ANGLE phi //
                double k=GetK(ik);    double phi=GetPhi(iphi);
                
                Values[ik][iphi]=GetIntegralValue(Spectrum,k-0.5*dK,k+0.5*dK,phi-0.5*dPhi,phi+0.5*dPhi,2000);
                
                dNOverdY+=Values[ik][iphi];
            }
        }
        
        // OUTPUT dN/dY //
        std::cerr << "#GLUON dN/dy=" << dNOverdY << std::endl;
        
    }

  ///////////////
  // GET dN/dy //
  ///////////////
 
public:

  DOUBLE GetdNOverdY(DOUBLE pTCutLow,DOUBLE pTCutHigh){
    
    DOUBLE Result=0.0;

    for(int ik=0;ik<Nk;ik++){
      for(int iphi=0;iphi<NPhi;iphi++){

	// GET MOMENTUM kT AND ANGLE phi //                                                                                                                                                                         
	double k=GetK(ik);

	if(k>=pTCutLow && k<pTCutHigh){
	  Result+=Values[ik][iphi];
	}


      }
    }

      return Result;
  }
    //////////////////////
    // SAVE SPECTRUM    //
    //////////////////////

public:
    
    void Save(std::string fname){
        
        // CREATE OUPUT STREAM //
        std::ofstream OutStream;
        OutStream.open(fname.c_str());
        
        
        // OUTPUT dN/d2kT //
        for(int ik=0;ik<Nk;ik++){
            
            for(int iphi=0;iphi<NPhi;iphi++){
            
                OutStream << GetK(ik) << " " << GetPhi(iphi) << " " << Values[ik][iphi]/(GetK(ik)*dK*dPhi) << std::endl;
                
            }
            
        }
        
        // CLOSE OUTPUT STREAM //
        OutStream.close();
    }
    
    /////////////////////////
    // DETERMINE HARMONICS //
    /////////////////////////

public:
    
    void GetHarmonics(std::string fname,int NumberOfHarmonics,DOUBLE pTCutLow,DOUBLE pTCutHigh){
        
        std::cerr << "# EXTRACTING HARMONICS " << std::endl;
        
        /////////////////////////
        // COEFFICIENT ARRAYS  //
        /////////////////////////
        
        COMPLEX **B=new COMPLEX*[Nk];   COMPLEX *BRef=new COMPLEX[NumberOfHarmonics+1];
        
        // RESET //
        for(int ik=0;ik<Nk;ik++){
            
            B[ik]=new COMPLEX[NumberOfHarmonics+1];
            
            for(int n=0;n<=NumberOfHarmonics;n++){
                
               B[ik][n]=COMPLEX(0.0,0.0);
            }
        }
        
        for(int n=0;n<=NumberOfHarmonics;n++){
            BRef[n]=COMPLEX(0.0,0.0);
        }

        /////////////////////////
        //  GET COEFFICIENTS   //
        /////////////////////////

        for(int ik=0;ik<Nk;ik++){
            
            // DETERMINE FOURIER HARMONICS //
            for(int iphi=0;iphi<NPhi;iphi++){
                
                for(int n=0;n<=NumberOfHarmonics;n++){
                    
                    B[ik][n]+=Values[ik][iphi]*exp(DOUBLE(n)*ComplexI*GetPhi(iphi));
                    
                    if(GetK(ik)>=pTCutLow && GetK(ik)<=pTCutHigh){
                        BRef[n]+=Values[ik][iphi]*exp(DOUBLE(n)*ComplexI*GetPhi(iphi));
                    }
                }
            }
            
        }
        
        //////////////////
        // CREATE OUPUT //
        //////////////////
        
        std::ofstream OutStream;
        
        OutStream.open(fname.c_str());
        
        // OUTPUT REFERENCE //
        OutStream << "#REFERENCE ";
        
        for(int n=0;n<=NumberOfHarmonics;n++){
            OutStream << " " <<  real(BRef[n]) << " " << imag(BRef[n]);
        }
        
        OutStream << std::endl;
        
        // OUTPUT COEFFICENTS //
        for(int ik=0;ik<Nk;ik++){
            
            OutStream << GetK(ik);
            
            for(int n=0;n<=NumberOfHarmonics;n++){
                OutStream << " " <<  real(B[ik][n]) << " " << imag(B[ik][n]);
            }
            
            OutStream << std::endl;
            
        }
        
        OutStream.close();
        
        /////////////
        // CLEANUP //
        /////////////
        
        for(int ik=0;ik<Nk;ik++){
            delete[] B[ik];
        }
        
        delete[] B; delete[] BRef;
        
    }
    
public:
    
    void GetSpectrum(std::string fname){
        
        // CREATE OUPUT STREAM //
        std::ofstream OutStream;
        OutStream.open(fname.c_str());
        
        
        // OUTPUT 1/(2Pi pT) dN/dydpT //
        for(int ik=0;ik<Nk;ik++){
            
            double Value=0.0;
            
            for(int iphi=0;iphi<NPhi;iphi++){
                Value+=Values[ik][iphi];
            }
            
            OutStream << GetK(ik) << " " << Value/(2.0*M_PI*GetK(ik)*dK) << std::endl;
            
        }
        
        // CLOSE OUTPUT STREAM //
        OutStream.close();
    }
    
public:
    
    void GetVNDelta(int NumberOfHarmonics,int nWeight,DOUBLE pTCutLow,DOUBLE pTCutHigh,std::string fname){
                
        /////////////////////////
        // COEFFICIENT ARRAYS  //
        /////////////////////////
        
        COMPLEX *BRef=new COMPLEX[NumberOfHarmonics+1];
        
        // RESET //
        for(int n=0;n<=NumberOfHarmonics;n++){
            BRef[n]=COMPLEX(0.0,0.0);
        }
        
        /////////////////////////
        //  GET COEFFICIENTS   //
        /////////////////////////
        
        for(int ik=0;ik<Nk;ik++){
            
            // DETERMINE FOURIER HARMONICS //
            for(int iphi=0;iphi<NPhi;iphi++){
                
                for(int n=0;n<=NumberOfHarmonics;n++){
                    
                    if(GetK(ik)>=pTCutLow && GetK(ik)<=pTCutHigh){
                        BRef[n]+=std::pow(GetK(ik),nWeight)*Values[ik][iphi]*exp(DOUBLE(n)*ComplexI*GetPhi(iphi));
                    }
                }
            }
            
        }
        
        // CREATE OUTPUT //
        std::ofstream OutStream;
        OutStream.open(fname.c_str(),std::ofstream::out | std::ofstream::app);
        
        for(int n=0;n<=NumberOfHarmonics;n++){
            OutStream << " " <<  SQR_ABS(BRef[n]);
        }
        OutStream << std::endl;
        
        OutStream.close();
        
        
    }
    
    ///////////////////////
    // OBJECT MANAGEMENT //
    ///////////////////////
    
public:
    
    InterpolatedSpectrum(int Nk,int NPhi,double kMin,double kMax,double PhiMin,double PhiMax){
        
        // SET SIZE //
        this->Nk=Nk; this->NPhi=NPhi;
        
        // SET BIN RANGE //
        this->kMax=kMax; this->kMin=kMin; this->PhiMin=PhiMin; this->PhiMax=PhiMax;
        
        // SET BIN SIZE //
        dK=(kMax-kMin)/double(Nk);    dPhi=(PhiMax-PhiMin)/double(NPhi);
        
        
        // INITIALIZE INTERPOLATED DATA ARRAY //
        Values=new double*[Nk];
        
        for(int ik=0;ik<Nk;ik++){
            
            Values[ik]=new double[NPhi];
            
            for(int iphi=0;iphi<NPhi;iphi++){
                
                Values[ik][iphi]=0.0;
                
            }
        }
        
        // RESET MULTIPLICTY //
        dNOverdY=0.0;
        
    }
    
public:    
    ~InterpolatedSpectrum(){
        
        // CLEANUP //
        for(int ik=0;ik<Nk;ik++){
            delete[] Values[ik];
        }
        
        delete[] Values;
    }
    
};
