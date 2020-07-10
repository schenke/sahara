// STANDARD INCLUSIONS //
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring> 
#include <complex>

#include <cstdlib>
#include <cmath>

// COMMANDLINES //
#include "IO/cfile.h"
#include "IO/cfile.c"


typedef int INT;
typedef double DOUBLE;
typedef std::complex<double> COMPLEX;

//BASIC CONSTANTS
#ifndef ComplexI
#define ComplexI COMPLEX(DOUBLE(0.0),DOUBLE(1.0))// i
#endif

#ifndef PI
#define PI         DOUBLE(3.141592653589793238462643383279502884197169399375105820974944592)   // pi
#endif

#ifndef M_SQRT2
#define M_SQRT2    DOUBLE(1.414213562373095048801688724209698078569671875376948073176679738)   // sqrt(2)
#endif

#ifndef M_SQRT3
#define M_SQRT3    DOUBLE(1.732050807568877293527446341505872366942805253810380628055806979)   // sqrt(3)
#endif

#ifndef M_HBARC
#define M_HBARC   DOUBLE(0.197)   // hbar * c
#endif

//BASIC MACROS
#define DABS(x)  (((x)>=(0))?(x):(-x))  // |x|
#define SQR(x)      ((x)*(x))                        // x^2
#define SQR_ABS(x)  (SQR(real(x)) + SQR(imag(x)))  // |x|^2
#define MOD(x,y)    ((x)>=(0)?((x)%(y)):(((x)+(y))%(y)))       // x mod y
#define DELTA(x,y)    (((x)==(y))?(1):(0))       // Kronecker delta
#define DSQRT(x) (((x)>=0)?(sqrt(x)):(ComplexI*sqrt(-(x)))) // PRINCIPAL SQUARE ROOT OF REAL NUMBER
#define SIGN(x)  ((x)>0?(1):((x)<0?(-1):(0))) // SIGN FUNCTION

namespace Parameters{
    
    // SCALE PARAMETER SQRT(B_g) IN LATTICE UNITS //
    DOUBLE ProtonRadiusScale=19.7;
    
    // PROTON PARAMETERS  SQRT(B_g) [fm] //
    DOUBLE ProtonRadiusInFermi=0.394;
    
}



// SCALE CONSERVSION //
#define GEV_SCALE (M_HBARC/Parameters::ProtonRadiusInFermi)
#define INVERSE_GEV_SCALE (Parameters::ProtonRadiusInFermi/M_HBARC)
#define FERMI_SCALE (Parameters::ProtonRadiusInFermi)
#define INVERSE_FERMI_SCALE (1.0/Parameters::ProtonRadiusInFermi)



namespace Lattice {
    
    INT N[2]={128,128}; DOUBLE a[2]={1.0,1.0};
    DOUBLE ScaleConstant=Parameters::ProtonRadiusScale;

}

namespace Dynamics {
    DOUBLE gUpMetric[2]={1.0,1.0};
}


#define GetTransverseMomenta(pXIndex,pYIndex,cDpx,cDpy) \
\
cDpx=-ComplexI*(DOUBLE(1.0)-exp(-ComplexI*DOUBLE(2.0)*PI*DOUBLE(pXIndex)/DOUBLE(Lattice::N[0])))/Lattice::a[0]; \
cDpy=-ComplexI*(DOUBLE(1.0)-exp(-ComplexI*DOUBLE(2.0)*PI*DOUBLE(pYIndex)/DOUBLE(Lattice::N[1])))/Lattice::a[1];


#include "DiscreteSpectrum.cpp"
#include "InterpolatedSpectrum.cpp"
#include "IndependentFragmentation.cpp"


//////////////////
// MAIN ROUTINE //
//////////////////

int main(int argc,char **argv){
    
    /////////////////////////////////////
    // CONFIGURE COMMANDLINE ARGUMENTS //
    /////////////////////////////////////
    
    Konfig arguments(argc,argv);
    
    char EventInputFile[256]="EventInput.txt";
    arguments.Getval("i",EventInputFile);
    
    char vNGluonOutputFile[256]="vNGluonOutput.txt";
    arguments.Getval("ovg",vNGluonOutputFile);
    
    char vNHadronOutputFile[256]="vNHadronOutput.txt";
    arguments.Getval("ovh",vNHadronOutputFile);
    
    char SpectrumGluonOutputFile[256]="GluonSpectrumOutput.txt";
    arguments.Getval("osg",SpectrumGluonOutputFile);
    
    char SpectrumHadronOutputFile[256]="HadronSpectrumOutput.txt";
    arguments.Getval("osh",SpectrumHadronOutputFile);
    
    DiscreteSpectrum *OriginalSpectrum=new DiscreteSpectrum(EventInputFile);
    
    // GLUON ANALYSIS //
    InterpolatedSpectrum *GluonSpectrum=new InterpolatedSpectrum(71,48,0.25,18.0,0.0,2.0*M_PI);
    GluonSpectrum->PerformInterpolation(OriginalSpectrum);
    
    std::cerr << "#GLUON MULTIPLICITY " << GluonSpectrum->GetdNOverdY(0.25,8.0) << " " << GluonSpectrum->GetdNOverdY(0.4,8.0) << std::endl;
    
    GluonSpectrum->GetHarmonics(vNGluonOutputFile,4,0.0,8.0);
    GluonSpectrum->GetSpectrum(SpectrumGluonOutputFile);

    
    // HADRON ANALYSIS //
    InterpolatedSpectrum *HadronSpectrum=new InterpolatedSpectrum(159,48,0.05,8.0,0.0,2.0*M_PI);
    
    PerformFragmentation(128,0.05,1.0,7,GluonSpectrum,HadronSpectrum);
    
    std::cerr << "#CHARGED HADRON MULTIPLICITY " << GluonSpectrum->GetdNOverdY(0.05,8.0) << " " << GluonSpectrum->GetdNOverdY(0.25,8.0) << std::endl;

    HadronSpectrum->GetHarmonics(vNHadronOutputFile,4,0.0,8.0);
    HadronSpectrum->GetSpectrum(SpectrumHadronOutputFile);


}
    
