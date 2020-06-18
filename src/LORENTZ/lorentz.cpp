#include "lorentz.h"

///// set Beta /////

void lorentz::setBeta( const vector4D & beta0 )
{

  setBetaScalar( beta0 );


}

void lorentz::setBetaScalar( const vector4D & beta0 )
{

  vector4D beta = beta0;

  if (fabs(beta(0)-1.0)>1e-5)
  {
    // std::cout << "WARNING: beta(0) != 1 !!!" << std::endl;
    // std::cout << beta << std::endl << std::endl;

    // std::string errMsg = "WARNING: beta(0) != 1 !!!";
    // throw eLorentz_error( errMsg );

    beta(0) = 1.0;
  }

  double g = beta.M2_Scalar(); // = 1 - b^2
  if (g < 0)
  {
    std::cout << "ERROR: beta^2 > 1: g=" << g << std::endl;
    //std::cout << beta << std::endl << std::endl;
    return;
  }
  g = 1/sqrt(g);
  double gg = g*g/(1+g);

  for (unsigned int i=1;i<4;i++)
    for (unsigned int j=1;j<4;j++)
      Arr[i+4*j] = gg * beta[i] * beta[j];
  for (unsigned int i=1;i<4;i++)
    Arr[i] = Arr[4*i] = g * beta[i];
  Arr[0] = g;
  for (unsigned int i=1;i<4;i++)
    Arr[i+4*i] += 1;
}

void lorentz::setBetaCM( const vector4D & x1 )
{

  setBetaCMScalar( x1 );

}

void lorentz::setBetaCMScalar( const vector4D & x1 )
{
  vector4D beta = x1;
  beta *= 1/beta[0];
  setBetaScalar( beta );
}

void lorentz::setBetaCM( const vector4D & x1, const vector4D & x2 )
{

  setBetaCMScalar( x1, x2 );

}

void lorentz::setBetaCMScalar( const vector4D & x1, const vector4D & x2 )
{
  vector4D beta = x1 + x2;
  beta *= 1/beta[0];
  setBetaScalar( beta );
}

void lorentz::setBetaCM( const vector4D & x1, const vector4D & x2, const vector4D & x3)
{
  setBeta( (x1+x2+x3).NormalizeToE() );
}

///// BOOST ONE VECTOR /////

vector4D lorentz::boost( const vector4D & x ) const
{

  return boostScalar( x );


}

vector4D lorentz::boostScalar( const vector4D & x ) const
{

  return vector4D( Arr[0+4*0]*x[0]-Arr[1+4*0]*x[1]-Arr[2+4*0]*x[2]-Arr[3+4*0]*x[3],
                  -Arr[0+4*3]*x[0]+Arr[1+4*3]*x[1]+Arr[2+4*3]*x[2]+Arr[3+4*3]*x[3],
                  -Arr[0+4*2]*x[0]+Arr[1+4*2]*x[1]+Arr[2+4*2]*x[2]+Arr[3+4*2]*x[3],
                  -Arr[0+4*1]*x[0]+Arr[1+4*1]*x[1]+Arr[2+4*1]*x[2]+Arr[3+4*1]*x[3]);
}


vector4D lorentz::boostInv( const vector4D & x ) const
{


  return boostInvScalar( x );


}

vector4D lorentz::boostInvScalar( const vector4D & x ) const
{
  return vector4D( Arr[0+4*0]*x[0]+Arr[1+4*0]*x[1]+Arr[2+4*0]*x[2]+Arr[3+4*0]*x[3],
                   Arr[0+4*3]*x[0]+Arr[1+4*3]*x[1]+Arr[2+4*3]*x[2]+Arr[3+4*3]*x[3],
                   Arr[0+4*2]*x[0]+Arr[1+4*2]*x[1]+Arr[2+4*2]*x[2]+Arr[3+4*2]*x[3],
                   Arr[0+4*1]*x[0]+Arr[1+4*1]*x[1]+Arr[2+4*1]*x[2]+Arr[3+4*1]*x[3]);
}


///// BOOST TWO VECTORS /////


void lorentz::boost( const vector4D & x1, const vector4D & x2, 
                     vector4D & x1New, vector4D & x2New ) const
{


  boostScalar( x1, x2, x1New, x2New );



}

void lorentz::boostScalar( const vector4D & x1, const vector4D & x2, 
                     vector4D & x1New, vector4D & x2New ) const
{
  x1New = vector4D( Arr[0+4*0]*x1[0]-Arr[1+4*0]*x1[1]-Arr[2+4*0]*x1[2]-Arr[3+4*0]*x1[3],
                   -Arr[0+4*3]*x1[0]+Arr[1+4*3]*x1[1]+Arr[2+4*3]*x1[2]+Arr[3+4*3]*x1[3],
                   -Arr[0+4*2]*x1[0]+Arr[1+4*2]*x1[1]+Arr[2+4*2]*x1[2]+Arr[3+4*2]*x1[3],
                   -Arr[0+4*1]*x1[0]+Arr[1+4*1]*x1[1]+Arr[2+4*1]*x1[2]+Arr[3+4*1]*x1[3]);
  
  x2New = vector4D( Arr[0+4*0]*x2[0]-Arr[1+4*0]*x2[1]-Arr[2+4*0]*x2[2]-Arr[3+4*0]*x2[3],
                   -Arr[0+4*3]*x2[0]+Arr[1+4*3]*x2[1]+Arr[2+4*3]*x2[2]+Arr[3+4*3]*x2[3],
                   -Arr[0+4*2]*x2[0]+Arr[1+4*2]*x2[1]+Arr[2+4*2]*x2[2]+Arr[3+4*2]*x2[3],
                   -Arr[0+4*1]*x2[0]+Arr[1+4*1]*x2[1]+Arr[2+4*1]*x2[2]+Arr[3+4*1]*x2[3]);

}



double lorentz::getSpatialDistance(const vector4D& x1, const vector4D& x2)
{
  double xsq = pow(x1[1]-x2[1],2.0);
  double ysq = pow(x1[2]-x2[2],2.0);
  double zsq = pow(x1[3]-x2[3],2.0);
  return sqrt(  xsq + ysq + zsq );
}

/*
double lorentz::getDCAsquared(const vector4D& x1, const vector4D& x2, const vector4D& p1, const vector4D& p2)
{
  double dvx,dvy,dvz,dx,dy,dz;
  dvx = p1[1]/p1[0] - p2[1]/p2[0];
  dvy = p1[2]/p1[0] - p2[2]/p2[0];
  dvz = p1[3]/p1[0] - p2[3]/p2[0];
    
  double dv2 = dvx*dvx +dvy*dvy + dvz*dvz;
  
  dx = x1[1]-x2[1];
  dy = x1[2]-x2[2];
  dz = x1[3]-x2[3];
  
  double dx2 = dx*dx + dy*dy + dz*dz;
  double dvdx = dx*dvx + dy*dvy + dz*dvz;
  
  //  double t=-(dvdx)/dv2;  // bps: why computed?
  // bps: fixed a problem of division by zero when two particles have zero relative velocity - then just return their spatial distance 
  if (dv2 == 0)
    return(dx2);
  else
    return (dx2 - dvdx*dvdx/dv2);
}*/


double lorentz::getConstrainedDCAsquared(const vector4D& x1, const vector4D& x2, const vector4D& p1, const vector4D& p2)
{
    // GET RELATIVE VELOCITY //
    double dvx,dvy,dvz,dx,dy,dz;
    dvx = p1[1]/p1[0] - p2[1]/p2[0];
    dvy = p1[2]/p1[0] - p2[2]/p2[0];
    dvz = p1[3]/p1[0] - p2[3]/p2[0];
    
    double dv2 = dvx*dvx +dvy*dvy + dvz*dvz;
    
    // GET POSITIONS AT t0=0 //
    double t0=0.0;
    dx = x1[1]+(t0-x1[0])*p1[1]/p1[0]-(x2[1]+(t0-x2[0])*p2[1]/p2[0]);
    dy = x1[2]+(t0-x1[0])*p1[2]/p1[0]-(x2[2]+(t0-x2[0])*p2[2]/p2[0]);
    dz = x1[3]+(t0-x1[0])*p1[3]/p1[0]-(x2[3]+(t0-x2[0])*p2[3]/p2[0]);
    
    double dx2 = dx*dx + dy*dy + dz*dz;
    double dvdx = dx*dvx + dy*dvy + dz*dvz;
    
    // GET DCA TIME //
    double tDCA=-dvdx/dv2;
    
    // GET CONSTRAINED DCA FOR POSITIVE TIMES //
    if(dv2==0){
        return dx2;
    }
    else if(tDCA>=0){
        return (dx2 - dvdx*dvdx/dv2);
        
    }
    else{
        return (dx2 - dvdx*dvdx/dv2)+dv2*tDCA*tDCA;
    }

}





void lorentz::boostInv( const vector4D & x1, const vector4D & x2, 
                        vector4D & x1New, vector4D & x2New ) const
{


  boostInvScalar( x1, x2, x1New, x2New );


}

void lorentz::boostInvScalar( const vector4D & x1, const vector4D & x2, 
                              vector4D & x1New, vector4D & x2New ) const
{
  x1New = vector4D(Arr[0+4*0]*x1[0]+Arr[1+4*0]*x1[1]+Arr[2+4*0]*x1[2]+Arr[3+4*0]*x1[3],
                   Arr[0+4*3]*x1[0]+Arr[1+4*3]*x1[1]+Arr[2+4*3]*x1[2]+Arr[3+4*3]*x1[3],
                   Arr[0+4*2]*x1[0]+Arr[1+4*2]*x1[1]+Arr[2+4*2]*x1[2]+Arr[3+4*2]*x1[3],
                   Arr[0+4*1]*x1[0]+Arr[1+4*1]*x1[1]+Arr[2+4*1]*x1[2]+Arr[3+4*1]*x1[3]);
  
  x2New = vector4D(Arr[0+4*0]*x2[0]+Arr[1+4*0]*x2[1]+Arr[2+4*0]*x2[2]+Arr[3+4*0]*x2[3],
                   Arr[0+4*3]*x2[0]+Arr[1+4*3]*x2[1]+Arr[2+4*3]*x2[2]+Arr[3+4*3]*x2[3],
                   Arr[0+4*2]*x2[0]+Arr[1+4*2]*x2[1]+Arr[2+4*2]*x2[2]+Arr[3+4*2]*x2[3],
                   Arr[0+4*1]*x2[0]+Arr[1+4*1]*x2[1]+Arr[2+4*1]*x2[2]+Arr[3+4*1]*x2[3]);
}


///// BOOST THREE VECTORS /////



void lorentz::boost( const vector4D & x1, const vector4D & x2, const vector4D & x3,
                     vector4D & x1New, vector4D & x2New, vector4D & x3New ) const
{


  boostScalar( x1, x2, x3, x1New, x2New, x3New );


}

void lorentz::boostScalar( const vector4D & x1, const vector4D & x2, const vector4D & x3,
                           vector4D & x1New, vector4D & x2New, vector4D & x3New ) const
{
  x1New = vector4D( Arr[0+4*0]*x1[0]-Arr[1+4*0]*x1[1]-Arr[2+4*0]*x1[2]-Arr[3+4*0]*x1[3],
                   -Arr[0+4*3]*x1[0]+Arr[1+4*3]*x1[1]+Arr[2+4*3]*x1[2]+Arr[3+4*3]*x1[3],
                   -Arr[0+4*2]*x1[0]+Arr[1+4*2]*x1[1]+Arr[2+4*2]*x1[2]+Arr[3+4*2]*x1[3],
                   -Arr[0+4*1]*x1[0]+Arr[1+4*1]*x1[1]+Arr[2+4*1]*x1[2]+Arr[3+4*1]*x1[3]);
  
  x2New = vector4D( Arr[0+4*0]*x2[0]-Arr[1+4*0]*x2[1]-Arr[2+4*0]*x2[2]-Arr[3+4*0]*x2[3],
                   -Arr[0+4*3]*x2[0]+Arr[1+4*3]*x2[1]+Arr[2+4*3]*x2[2]+Arr[3+4*3]*x2[3],
                   -Arr[0+4*2]*x2[0]+Arr[1+4*2]*x2[1]+Arr[2+4*2]*x2[2]+Arr[3+4*2]*x2[3],
                   -Arr[0+4*1]*x2[0]+Arr[1+4*1]*x2[1]+Arr[2+4*1]*x2[2]+Arr[3+4*1]*x2[3]);

  x3New = vector4D( Arr[0+4*0]*x3[0]-Arr[1+4*0]*x3[1]-Arr[2+4*0]*x3[2]-Arr[3+4*0]*x3[3],
                   -Arr[0+4*3]*x3[0]+Arr[1+4*3]*x3[1]+Arr[2+4*3]*x3[2]+Arr[3+4*3]*x3[3],
                   -Arr[0+4*2]*x3[0]+Arr[1+4*2]*x3[1]+Arr[2+4*2]*x3[2]+Arr[3+4*2]*x3[3],
                   -Arr[0+4*1]*x3[0]+Arr[1+4*1]*x3[1]+Arr[2+4*1]*x3[2]+Arr[3+4*1]*x3[3]);

}

void lorentz::boostInv( const vector4D & x1, const vector4D & x2, const vector4D & x3, 
                        vector4D & x1New, vector4D & x2New, vector4D & x3New ) const
{

  boostInvScalar( x1, x2, x3, x1New, x2New, x3New );


}

void lorentz::boostInvScalar( const vector4D & x1, const vector4D & x2, const vector4D & x3, 
                              vector4D & x1New, vector4D & x2New, vector4D & x3New ) const
{
  x1New = vector4D(Arr[0+4*0]*x1[0]+Arr[1+4*0]*x1[1]+Arr[2+4*0]*x1[2]+Arr[3+4*0]*x1[3],
                   Arr[0+4*3]*x1[0]+Arr[1+4*3]*x1[1]+Arr[2+4*3]*x1[2]+Arr[3+4*3]*x1[3],
                   Arr[0+4*2]*x1[0]+Arr[1+4*2]*x1[1]+Arr[2+4*2]*x1[2]+Arr[3+4*2]*x1[3],
                   Arr[0+4*1]*x1[0]+Arr[1+4*1]*x1[1]+Arr[2+4*1]*x1[2]+Arr[3+4*1]*x1[3]);
  
  x2New = vector4D(Arr[0+4*0]*x2[0]+Arr[1+4*0]*x2[1]+Arr[2+4*0]*x2[2]+Arr[3+4*0]*x2[3],
                   Arr[0+4*3]*x2[0]+Arr[1+4*3]*x2[1]+Arr[2+4*3]*x2[2]+Arr[3+4*3]*x2[3],
                   Arr[0+4*2]*x2[0]+Arr[1+4*2]*x2[1]+Arr[2+4*2]*x2[2]+Arr[3+4*2]*x2[3],
                   Arr[0+4*1]*x2[0]+Arr[1+4*1]*x2[1]+Arr[2+4*1]*x2[2]+Arr[3+4*1]*x2[3]);

  x3New = vector4D(Arr[0+4*0]*x3[0]+Arr[1+4*0]*x3[1]+Arr[2+4*0]*x3[2]+Arr[3+4*0]*x3[3],
                   Arr[0+4*3]*x3[0]+Arr[1+4*3]*x3[1]+Arr[2+4*3]*x3[2]+Arr[3+4*3]*x3[3],
                   Arr[0+4*2]*x3[0]+Arr[1+4*2]*x3[1]+Arr[2+4*2]*x3[2]+Arr[3+4*2]*x3[3],
                   Arr[0+4*1]*x3[0]+Arr[1+4*1]*x3[1]+Arr[2+4*1]*x3[2]+Arr[3+4*1]*x3[3]);

}

