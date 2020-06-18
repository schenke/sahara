
#include <gsl/gsl_sf.h>

namespace Generator{
    
    
    void SampleThermalGluon(double T,double L){
        
        
        // SAMPLE COORDINATE SPACE RAPIDITY //
        double x=-L/2.0+L*drand48();
        double y=-L/2.0+L*drand48();
        double z=-L/2.0+L*drand48();
        double t=0.0;
        
        // SAMPLE ENERGY //
        bool Accept=false; double E;
        
        while(Accept==false){
            
            E=10.0*T*drand48();
            
            if(drand48()<E*E*exp(-E/T)){
                Accept=true;
            }
            
        }
        
        // SAMPLE DIRECTION //
        double CosTheta=-1.0+2.0*drand48();
        double SinTheta=sqrt(1.0-CosTheta*CosTheta);
        
        double Phi=2.0*M_PI*drand48();
        double CosPhi=cos(Phi);
        double SinPhi=sin(Phi);
        
        double Px=E*SinTheta*CosPhi;
        double Py=E*SinTheta*SinPhi;
        double Pz=E*CosTheta;
        
        Particle NewGluon;
        
        NewGluon.Mom.Px()=Px;
        NewGluon.Mom.Py()=Py;
        NewGluon.Mom.Pz()=Pz;
        NewGluon.Mom.E()=E;
        
        NewGluon.Pos.X()=x;
        NewGluon.Pos.Y()=y;
        NewGluon.Pos.Z()=z;
        NewGluon.Pos.T()=t;
        
        NewGluon.FLAVOR=gluon;
        
        GlobalPartonList.push_back(NewGluon);
        
        
    }
    
    void SampleThermalQuark(double T,double L,FLAVOR_TYPE f){
        
        
        // SAMPLE COORDINATE SPACE RAPIDITY //
        double x=-L/2.0+L*drand48();
        double y=-L/2.0+L*drand48();
        double z=-L/2.0+L*drand48();
        double t=0.0;
        
        // SAMPLE ENERGY //
        bool Accept=false; double E;
        
        while(Accept==false){
            
            E=10.0*T*drand48();
            
            if(drand48()<E*E*exp(-E/T)){
                Accept=true;
            }
            
        }
        
        // SAMPLE DIRECTION //
        double CosTheta=-1.0+2.0*drand48();
        double SinTheta=sqrt(1.0-CosTheta*CosTheta);
        
        double Phi=2.0*M_PI*drand48();
        double CosPhi=cos(Phi);
        double SinPhi=sin(Phi);
        
        double Px=E*SinTheta*CosPhi;
        double Py=E*SinTheta*SinPhi;
        double Pz=E*CosTheta;
        
        Particle NewQuark;
        
        NewQuark.Mom.Px()=Px;
        NewQuark.Mom.Py()=Py;
        NewQuark.Mom.Pz()=Pz;
        NewQuark.Mom.E()=E;
        
        NewQuark.Pos.X()=x;
        NewQuark.Pos.Y()=y;
        NewQuark.Pos.Z()=z;
        NewQuark.Pos.T()=t;
        
        NewQuark.FLAVOR=f;
        
        GlobalPartonList.push_back(NewQuark);
        
        
    }
    
    void SampleThermalAntiQuark(double T,double L,FLAVOR_TYPE f){
        
        // SAMPLE COORDINATE SPACE RAPIDITY //
        double x=-L/2.0+L*drand48();
        double y=-L/2.0+L*drand48();
        double z=-L/2.0+L*drand48();
        double t=0.0;
        
        // SAMPLE ENERGY //
        bool Accept=false; double E;
        
        while(Accept==false){
            
            E=10.0*T*drand48();
            
            if(drand48()<E*E*exp(-E/T)){
                Accept=true;
            }
            
        }
        
        // SAMPLE DIRECTION //
        double CosTheta=-1.0+2.0*drand48();
        double SinTheta=sqrt(1.0-CosTheta*CosTheta);
        
        double Phi=2.0*M_PI*drand48();
        double CosPhi=cos(Phi);
        double SinPhi=sin(Phi);
        
        double Px=E*SinTheta*CosPhi;
        double Py=E*SinTheta*SinPhi;
        double Pz=E*CosTheta;
        
        Particle NewAntiQuark;
        
        NewAntiQuark.Mom.Px()=Px;
        NewAntiQuark.Mom.Py()=Py;
        NewAntiQuark.Mom.Pz()=Pz;
        NewAntiQuark.Mom.E()=E;
        
        NewAntiQuark.FLAVOR=f;
        
        GlobalPartonList.push_back(NewAntiQuark);
        
        
    }
    
    
    
    void SampleThermalGluons(double T,double L,int N){
        
        // CLEAR GLOBAL PARTON LIST //
        GlobalPartonList.clear();
        
        for(int i=0;i<N;i++){
            SampleThermalGluon(T,L);
        }
        
    }
    
    void SampleThermalQuarkAntiQuarkPairs(double T,double L,int N){
        
        // CLEAR GLOBAL PARTON LIST //
        GlobalPartonList.clear();
        
        for(int i=0;i<N;i++){
            
            // SAMPLE COORDINATE SPACE RAPIDITY //
            double x=-L/2.0+L*drand48();
            double y=-L/2.0+L*drand48();
            double z=-L/2.0+L*drand48();
            double t=0.0;
            
            // SAMPLE ENERGY //
            bool Accept=false; double E;
            
            while(Accept==false){
                
                E=10.0*T*drand48();
                
                if(drand48()<E*E*exp(-E/T)){
                    Accept=true;
                }
                
            }
            
            // SAMPLE DIRECTION //
            double CosTheta=-1.0+2.0*drand48();
            double SinTheta=sqrt(1.0-CosTheta*CosTheta);
            
            double Phi=2.0*M_PI*drand48();
            double CosPhi=cos(Phi);
            double SinPhi=sin(Phi);
            
            double Px=E*SinTheta*CosPhi;
            double Py=E*SinTheta*SinPhi;
            double Pz=E*CosTheta;
            
            Particle Quark;
            Particle AntiQuark;
            
            Quark.Mom.Px()=Px;
            Quark.Mom.Py()=Py;
            Quark.Mom.Pz()=Pz;
            Quark.Mom.E()=E;
            
            Quark.Pos.X()=x;
            Quark.Pos.Y()=y;
            Quark.Pos.Z()=z;
            Quark.Pos.T()=t;
            
            AntiQuark.Mom.Px()=-Px;
            AntiQuark.Mom.Py()=-Py;
            AntiQuark.Mom.Pz()=-Pz;
            AntiQuark.Mom.E()=E;
            
            AntiQuark.Pos.X()=x;
            AntiQuark.Pos.Y()=y;
            AntiQuark.Pos.Z()=z;
            AntiQuark.Pos.T()=t;
            
            double r=drand48();
            
            if(r<1.0/3.0){
                Quark.FLAVOR=up;
                AntiQuark.FLAVOR=anti_up;
            }
            else if(r<2.0/3.0){
                Quark.FLAVOR=down;
                AntiQuark.FLAVOR=anti_down;
            }
            else{
                Quark.FLAVOR=strange;
                AntiQuark.FLAVOR=anti_strange;
            }
            
            
            GlobalPartonList.push_back(Quark);
            GlobalPartonList.push_back(AntiQuark);
            
        }
        
    }
    
    void SampleThermalEnsemble(double T,double L){
        
        // CLEAR GLOBAL PARTON LIST //
        GlobalPartonList.clear();
        
        double Zg=16*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        
        double Zu=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        double Zub=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        
        double Zd=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        double Zdb=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        
        double Zs=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        double Zsb=6*(T*L)*(T*L)*(T*L)/(M_PI*M_PI);
        
        
        double ZgTot=exp(Zg);
        double ZuTot=gsl_sf_bessel_I0(2.0*sqrt(Zu*Zub));
        double ZdTot=gsl_sf_bessel_I0(2.0*sqrt(Zu*Zub));
        double ZsTot=gsl_sf_bessel_I0(2.0*sqrt(Zu*Zub));
        
        std::vector<double> PgTable;
        std::vector<double> PuTable;
        std::vector<double> PdTable;
        std::vector<double> PsTable;
        
        double Pg=0.0;
        double Pu=0.0;
        double Pd=0.0;
        double Ps=0.0;
        
        int n=0;
        
        while(Pg<1-1E-12 || Pu<1-1E-12 || Pd<1-1E-12 || Ps<1-1E-12){
            
            Pg+=std::pow(Zg,n)/gsl_sf_gamma(n+1)/ZgTot;
            Pu+=std::pow(Zu*Zub,n)/(gsl_sf_gamma(n+1)*gsl_sf_gamma(n+1))/ZuTot;
            Pd+=std::pow(Zs*Zdb,n)/(gsl_sf_gamma(n+1)*gsl_sf_gamma(n+1))/ZdTot;
            Ps+=std::pow(Zs*Zsb,n)/(gsl_sf_gamma(n+1)*gsl_sf_gamma(n+1))/ZsTot;
            
            //std::cout << n << " " << std::pow(Zg,n)/gsl_sf_gamma(n+1)/ZgTot << " " << std::pow(Zu*Zub,n)/(gsl_sf_gamma(n+1)*gsl_sf_gamma(n+1))/ZuTot << " " << Pg << " " << Pu << std::endl;
            
            PgTable.push_back(Pg);
            PuTable.push_back(Pu);
            PdTable.push_back(Pd);
            PsTable.push_back(Ps);
            
            n++;
            
            
        }
        
        double rg=drand48();
        double ru=drand48();
        double rd=drand48();
        double rs=drand48();
        
        // SAMPLE NUMBERS OF GLUONS, QUARKS AND ANTI-QUARKS //
        int Ng=0; while(PgTable.at(Ng)<rg){Ng++;}
        int Nu=0; while(PuTable.at(Nu)<ru){Nu++;}
        int Nd=0; while(PdTable.at(Nd)<rd){Nd++;}
        int Ns=0; while(PsTable.at(Ns)<rs){Ns++;}
        
        std::cout << Ng <<  " " << Nu << " " << Nd << " " << Ns << std::endl;
        
        // SAMPLE GLUONS //
        for(int i=0;i<Ng;i++){
            SampleThermalGluon(T,L);
        }
        
        // SAMPLE QUARK/ANTI-QUARK PAIRS //
        for(int i=0;i<Nu;i++){
            SampleThermalQuark(T,L,up);
            SampleThermalAntiQuark(T,L,anti_up);
        }

        for(int i=0;i<Nd;i++){
            SampleThermalQuark(T,L,down);
            SampleThermalAntiQuark(T,L,anti_down);
        }
        
        for(int i=0;i<Ns;i++){
            SampleThermalQuark(T,L,strange);
            SampleThermalAntiQuark(T,L,anti_strange);
        }
        
        
    }
    
    
}
