
#ifndef VECTORCLASS_CPP
#define VECTORCLASS_CPP

/** @file
 * @brief Declaration of 3D and 4D vectors
 *
 * Here we implement the 4D vector classes vector4D, where the basic
 * parameters are (E, pX, pY, pZ).
 *
 *
 * There is no distinction between 3D and 4D vectors. There is some
 * subset of routines, which calculates 3D scalar and vector products.
 *
 */


//#include "allocator.h"
//#include "configBAMPS.h"

#include <iostream>
#include <math.h>
#include <stdint.h> // for uintptr_t

#include <cmath> // std::isinf, std::isnan


/**
 * @brief class to implement 4D (and implicit 3D) vectors.
 *
 * The internal representation is independent of the user interface.

 *
 * The interface seperates between momenta (e.g. Px()) and coordinates
 * (e.g. X()). (At the moment, this distinction is artificial.)
 */
class vector4D
{
public:

  /** @brief The type of the numerical representation */
  typedef double Scalar;

protected:
  /** @brief The internal array to store the values */
  Scalar Mem[4];

public:

  // the class lorentz needs direct access to the stored values:
  friend class lorentz;
  // the class rotation needs direct access to the stored values:
  friend class rotation;

  /**
   * @brief Constructor
   *
   * Default constructor of an empty vector (Px = Py = Pz = E = 0 )
   */
  vector4D( ) : Mem()  { };

  /**
   * @brief Constructor
   *
   * Generic constructor from four scalar values.
   */
  vector4D(const Scalar & E,
           const Scalar & x,
           const Scalar & y,
           const Scalar & z)
  { Mem[0] = E; Mem[1] = z; Mem[2] = y; Mem[3] = x;};

  /**
   * @brief Constructor
   *
   * Generic constructor from only three scalar values
   * (the spatial components). The temporal (0th) component
   * is set to zero.
   */
  vector4D(const Scalar & x,
           const Scalar & y,
           const Scalar & z)
  { Mem[0] = 0.0; Mem[1] = z; Mem[2] = y; Mem[3] = x;};

  /**
   * @brief Set all entries
   *
   * 'Set routine' from four scalar values.
   */
  void SetTXYZ(const Scalar & t,
               const Scalar & x,
               const Scalar & y,
               const Scalar & z)
  { Mem[0] = t; Mem[1] = z; Mem[2] = y; Mem[3] = x; };

  /**
   * @brief Set all entries
   *
   * 'Set routine' from array of four scalar values. (no checking)
   */
  void SetTXYZ(const Scalar *Arr)
  { Mem[0]=Arr[0]; Mem[1]=Arr[3]; Mem[2]=Arr[2]; Mem[3]=Arr[1]; };

  /** @brief Get x component */
  Scalar Px() const { return Mem[3]; }
  /** @brief Get y component */
  Scalar Py() const { return Mem[2]; }
  /** @brief Get z component */
  Scalar Pz() const { return Mem[1]; }
  /** @brief Get 0th component */
  Scalar E() const { return Mem[0]; }

  /** @brief Set x component */
  Scalar & Px() { return Mem[3]; }
  /** @brief Set y component */
  Scalar & Py() { return Mem[2]; }
  /** @brief Set z component */
  Scalar & Pz() { return Mem[1]; }
  /** @brief Set 0th component */
  Scalar & E() { return Mem[0]; }

  /** @brief Get x component */
  Scalar X() const { return Mem[3]; }
  /** @brief Get y component */
  Scalar Y() const { return Mem[2]; }
  /** @brief Get z component */
  Scalar Z() const { return Mem[1]; }
  /** @brief Get 0th component */
  Scalar T() const { return Mem[0]; }

  /** @brief Set x component */
  Scalar & X() { return Mem[3]; }
  /** @brief Set y component */
  Scalar & Y() { return Mem[2]; }
  /** @brief Set z component */
  Scalar & Z() { return Mem[1]; }
  /** @brief Set 0th component */
  Scalar & T() { return Mem[0]; }


  /** @brief Get x component squared */
  Scalar Px2() const { return Mem[3]*Mem[3]; }
  /** @brief Get y component squared */
  Scalar Py2() const { return Mem[2]*Mem[2]; }
  /** @brief Get z component squared */
  Scalar Pz2() const { return Mem[1]*Mem[1]; }
  /** @brief Get 0th component squared */
  Scalar E2() const { return Mem[0]*Mem[0]; }

  /** @brief Get x component squared */
  Scalar X2() const { return Mem[3]*Mem[3]; }
  /** @brief Get y component squared */
  Scalar Y2() const { return Mem[2]*Mem[2]; }
  /** @brief Get z component squared */
  Scalar Z2() const { return Mem[1]*Mem[1]; }
  /** @brief Get 0th component squared */
  Scalar T2() const { return Mem[0]*Mem[0]; }

  /**
   * @brief Get entry
   *
   * return the ith entry: i=0..3 -> t,x,y,z
   */
  Scalar operator()(unsigned int i) const { return Mem[i==0?i:4-i]; }
  /**
   * @brief Set entry
   *
   * set the ith entry: i=0..3 -> t,x,y,z
   */
  Scalar & operator()(unsigned int i) { return Mem[i==0?i:4-i]; }

protected:
  /**
   * @brief Get entry
   *
   * return the ith entry, implementation dependent!
   */
  Scalar operator[](unsigned int i) const { return Mem[i]; }
  /**
   * @brief Set entry
   *
   * set the ith entry, implementation dependent!
   */
  Scalar & operator[](unsigned int i) { return Mem[i]; }

public:

  /**
   * @brief Get all entries
   *
   * get routine from array of four scalar values. (no checking!)
   */
  void GetTXYZ(Scalar *Arr) const
  { Arr[0]=T(); Arr[1]=X(); Arr[2]=Y(); Arr[3]=Z(); };

  /**
   * @brief Get all entries
   *
   * get routine from four scalar values.
   */
  void GetTXYZ(Scalar & t,
               Scalar & x,
               Scalar & y,
               Scalar & z) const
  { t=T(); x=X(); y=Y(); z=Z(); };

  /**
   * @brief Get all entries
   *
   * get routine from array of four scalar values. (no checking!)
   */
  void GetEPxPyPz(Scalar *Arr) const
  { Arr[0]=T(); Arr[1]=X(); Arr[2]=Y(); Arr[3]=Z(); };

  /**
   * @brief Check all entries against std::isinf
   */
  bool isInf() const
  {
    return std::isinf(T()) || std::isinf(X()) || std::isinf(Y()) || std::isinf(Z());
  }

  /**
   * @brief Check all entries against std::isnan
   */
  bool isNan() const
  {
    return std::isnan(T()) || std::isnan(X()) || std::isnan(Y()) || std::isnan(Z());
  }


  /**
   * @brief Set Energy as sqrt( p^2+m^2 )
   **/
  void SetEbyM(double mass)
  {
    E() = sqrt( vec2() + mass*mass );
  };

  /**
   * @brief Negate the 3D component
   **/
  void Minus3()
  {
    X() = -X(); Y() = -Y(); Z() = -Z();
  };

  /** @brief Unary minus. */
  inline vector4D operator - () const;

  /** @brief The Plus Operator */
  inline vector4D operator + (const vector4D &) const;

  /** @brief The Plus Operator with a scalar */
  inline vector4D operator + (const Scalar a) const;


  /** @brief The Increment Operator */
  inline vector4D & operator += (const vector4D &);

  /** @brief The Minus Operator */
  inline vector4D operator - (const vector4D &) const;

  /** @brief The Minus Operator with a scalar*/
  inline vector4D operator - (const Scalar a) const;

  /** @brief The Decrement Operator */
  inline vector4D & operator -= (const vector4D &);

  /**
   * @brief The Times Operator
   *
   * This is the multiplication with a scalar.
   * We intentionaly do not implement the division operator, because
   * it is better to use "*(1/a)" instead.
   **/
  inline vector4D operator * (const Scalar a) const;

  /**
   * @brief The Multiplicated Operator
   *
   * This is the multiplication with a scalar.
   * We intentionaly do not implement the division operator, because
   * it is better to use "*(1/a)" instead.
   **/
  inline vector4D & operator *= (const Scalar a);


  /**
   * @brief The Minimum
   *
   * returns a vector which holds the minimum of each component
   **/
  friend inline vector4D min(const vector4D &a, const vector4D &b );

  /**
   * @brief The Maximum
   *
   * returns a vector which holds the maximum of each component
   **/
  friend inline vector4D max(const vector4D &a, const vector4D &b );

  /**
   * @brief Normalize to the zeroth component
   *
   * i.e.: x[] *= 1/x[0]
   **/
  inline vector4D NormalizeToE(void);


  /** @brief The squared length of the 3D part */
  inline Scalar vec2() const;

  /** @brief The squared length of the vector (+--- metric) */
  inline Scalar M2() const;

  /** @brief The squared length of the vector (+--- metric) */
  inline Scalar M2_Scalar() const
  {
    return( Mem[0]*Mem[0] - Mem[1]*Mem[1] - Mem[2]*Mem[2] - Mem[3]*Mem[3] );
  }


  /** @brief The transverse momentum squared */
  Scalar Pt2() const { return X()*X()+Y()*Y(); };

  /** @brief The transverse momentum */
  Scalar Pt() const { return sqrt(Pt2()); };

  /** @brief The transverse component squared */
  Scalar Perp2() const { return Pt2(); };

  /** @brief The transverse component */
  Scalar Perp() const { return sqrt(Pt2()); };

  /** @brief The squared transverse mass
   *
   * We use here MT^2 = PT^2+ M^2 with M given as parameter.
   * Instead one can also use E^2-PZ^2, but there are some minor
   * numerical differences.
   */
  Scalar Mt2(const Scalar m) const { return m*m + Pt2(); };

  /** @brief The transverse mass
   *
   * We use here MT^2 = PT^2+ M^2 with M given as parameter.
   * Instead one can also use E^2-PZ^2, but there are some minor
   * numerical differences.
   */
  Scalar Mt(const Scalar m) const { return sqrt( Mt2(m) ); };

  /**
   * @brief The (longitudinal) eigentime tau squared
   **/
  Scalar Eigentime2() const { return T2()-Z2(); };

  /**
   * @brief The (longitudinal) eigentime tau
   **/
  Scalar Eigentime() const { return sqrt(Eigentime2()); };


  /**
   * @brief The (longitudinal) rapidity
   *
   * returns 0.5*log( (E+Pz)/(E-Pz) )
   **/
  Scalar Rapidity() const { return 0.5*log( (E()+Pz())/(E()-Pz()) ); };
  
  /**
   * @brief The (longitudinal) rapidity (with shift)
   *
   * returns 0.5*log( (E+Pz-shift)/(E-Pz-shift) )
   **/
  Scalar Rapidity(const Scalar shift) const { return 0.5*log( (E()+Pz()-shift)/(E()-Pz()-shift) ); };


  /**
   * @brief The (longitudinal) pseudorapidity
   *
   * @param[in] m The mass of the particle
   *
   * returns 0.5*log( (P+Pz)/(P-Pz) )
   *
   * One has to give the mass explicit as argument, since the length
   * of the vector is calculated vis sqrt(E^2-m^2), and not via #vec2.
   * This should be checked and improved!!!
   **/
  Scalar Pseudorapidity(const Scalar m) const;

  /** @brief The 4D scalar product of two vectors */
  friend Scalar Dot(const vector4D &x,const vector4D &y )
  {
    return ( (x[0]*y[0] - x[1]*y[1] - x[2]*y[2] - x[3]*y[3]) );
  }

  /** @brief The 3D scalar product of two vectors */
  friend Scalar Dot3(const vector4D &x,const vector4D &y )
  {
    return ( (x[1]*y[1] + x[2]*y[2] + x[3]*y[3]) );
  }

  /** @brief The angle between two vectors */
  friend Scalar CosTheta(const vector4D &x,const vector4D &y )
  {
    return ( (x[1]*y[1] + x[2]*y[2] + x[3]*y[3])/sqrt(x.vec2()*y.vec2()) );
  }

  /** @brief The angle between two vectors (assumed to be massless) */
  friend Scalar CosThetaMassless(const vector4D &x,const vector4D &y )
  {
    return ( (x[1]*y[1] + x[2]*y[2] + x[3]*y[3])/(x[0]*y[0]) );
  }

  /** @brief The angle between two vectors in transversal plane*/
  friend Scalar CosPhi(const vector4D &x,const vector4D &y )
  {
    return ( (x[2]*y[2] + x[3]*y[3])/sqrt(x.Perp2()*y.Perp2()) );
  }

  /** @brief The angle between the vector and the x-axis in transversal plane
   *
   * @return Principal arc tangent of y/x, in the interval [-pi,+pi] radians.
   */
  inline Scalar Phi_x( ) const
  {
    return ( atan2( Y(), X() ) );
  }

  /** @brief The angle between the vector and the y-axis in transversal plane
   *
   * @return Principal arc tangent of x/y, in the interval [-pi,+pi] radians.
   */

  inline Scalar Phi_y( ) const
  {
    return ( atan2( X(), Y() ) );
  }


  /** @brief The 3D cross product of two vectors */
  friend vector4D Cross(const vector4D &x,const vector4D &y )
  {
    return vector4D( 0.0,
                     x.Y()*y.Z()-x.Z()*y.Y(),
                     x.Z()*y.X()-x.X()*y.Z(),
                     x.X()*y.Y()-x.Y()*y.X() );
  }

  /** @brief Square every component */
  friend vector4D Square(const vector4D &x)
  {
    return vector4D( x[0]*x[0], x[1]*x[1], x[2]*x[2], x[3]*x[3] );
  }

  /** @brief The relative velocity of two particles
   *
   * @param[in] p1 4-momentum of particle 1
   * @param[in] p2 4-momentum of particle 2
   * @param[in] M1 mass of particle 1
   * @param[in] M2 mass of particle 2
   *
   * no checking is done, whether sqrt or division by energies is
   * possible.
   *
   * In principle, M1^2 and M2^2 can be calculated by P1^2 and P2^2,
   * but if we know them, giving them as parameter is faster.
   */
  friend Scalar VelRel(const vector4D &p1, const vector4D &p2, const Scalar M1, const Scalar M2)
  {
    return ( sqrt( pow( Dot(p1,p2), 2.0) - pow( M1*M2, 2.0))/( p1.E()*p2.E() ) );
  }

  /** @brief The v2 flow component in x-y-frame */
  Scalar FlowV2(void) const
  {
    return ( (Px2() - Py2())/Perp2() );
  }

  /** @brief The v4 flow component in x-y-frame */
  Scalar FlowV4(void) const
  {
    return ( pow(Px2(),2) - 6.0 * Px2() * Py2() + pow(Py2(),2) ) / pow(Perp2(),2);
  }


  
};



/////
///// Some inline definitions:
/////

inline vector4D vector4D::operator - () const
{
  return vector4D( -T(), -X(), -Y(), -Z() );
}



inline vector4D vector4D::operator + (const vector4D & q) const
{
  return vector4D( T()+q.T(), X()+q.X(), Y()+q.Y(), Z()+q.Z() );
}

inline vector4D vector4D::operator + (const Scalar a) const
{

  return vector4D( T()+a, X()+a, Y()+a, Z()+a );

}

inline vector4D &vector4D::operator += (const vector4D & q)
{


  Mem[0] += q.Mem[0];
  Mem[1] += q.Mem[1];
  Mem[2] += q.Mem[2];
  Mem[3] += q.Mem[3];

  return *this;
}

inline vector4D vector4D::operator - (const vector4D & q) const
{

  return vector4D( T()-q.T(), X()-q.X(), Y()-q.Y(), Z()-q.Z() );

}

inline vector4D vector4D::operator - (const Scalar a) const
{

  return vector4D( T()-a, X()-a, Y()-a, Z()-a );

}

inline vector4D &vector4D::operator -= (const vector4D & q)
{


  Mem[0] -= q.Mem[0];
  Mem[1] -= q.Mem[1];
  Mem[2] -= q.Mem[2];
  Mem[3] -= q.Mem[3];


  return *this;
}

inline vector4D   vector4D::operator * (const vector4D::Scalar a) const
{
  return vector4D( a*T(), a*X(), a*Y(), a*Z() );
}

inline vector4D & vector4D::operator *= (const vector4D::Scalar a)
{
  Mem[0]*=a;
  Mem[1]*=a;
  Mem[2]*=a;
  Mem[3]*=a;
  return *this;
}

inline vector4D min(const vector4D & a, const vector4D & b)
{

  return vector4D( std::min(a.T(),b.T()),
                   std::min(a.X(),b.X()),
                   std::min(a.Y(),b.Y()),
                   std::min(a.Z(),b.Z()) );

}

inline vector4D max(const vector4D & a, const vector4D & b)
{

  return vector4D( std::max(a.T(),b.T()),
                   std::max(a.X(),b.X()),
                   std::max(a.Y(),b.Y()),
                   std::max(a.Z(),b.Z()) );

}

inline vector4D::Scalar vector4D::Pseudorapidity(const vector4D::Scalar m) const
{
  vector4D::Scalar pp = sqrt( E()*E() - m*m );
  return 0.5*log( (pp+Pz())/(pp-Pz()) );
}

inline vector4D vector4D::NormalizeToE(void)
{

  double a = 1/Mem[0];
  Mem[0]*=a;
  Mem[1]*=a;
  Mem[2]*=a;
  Mem[3]*=a;

  return *this;
}

inline vector4D::Scalar vector4D::vec2() const
{

  return( Mem[1]*Mem[1] + Mem[2]*Mem[2] + Mem[3]*Mem[3] );

}

inline vector4D::Scalar vector4D::M2() const
{

  return( Mem[0]*Mem[0] - Mem[1]*Mem[1] - Mem[2]*Mem[2] - Mem[3]*Mem[3] );

}


/**
 * @brief Define the type of a (T,X,Y,Z) - vector
 */
typedef vector4D VectorTXYZ;

/**
 * @brief Define the type of a (E,Px,Py,Pz) - vector
 */
typedef vector4D VectorEPxPyPz;

/**
 * @brief Define the type of a (X,Y,Z) - vector
 *
 * This is identical to a (T,X,Y,Z) - vector (but T is unused)
 */
typedef vector4D VectorXYZ;

#endif
