#ifndef LORENTZ_H
#define LORENTZ_H

/** 
 * @brief class to implement Lorentz boosts for vector4D
 * 
 * The internal representation is independent of the user interface.
 *
 * 
 */
class lorentz
{
public: 
  /** 
   * @brief The type of the numerical representation
   **/
  typedef double Scalar;
  
protected:
  /** 
   * @brief The internal array to store the values
   **/
  Scalar Arr[16];
  
public:
  
  /** 
   * @brief Constructor
   * 
   * Default constructor of an empty boost
   */
  lorentz( ) 
  { 
    static const Scalar Arr1[16] = 
      { 1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0 };
    for (unsigned int i=0;i<16;i++) Arr[i] = Arr1[i];
  };

  /** 
   * @brief Constructor
   * 
   * Default constructor with given beta vector
   */
  lorentz( const vector4D & beta )
  {
    setBeta( beta );
  }

  /** 
   * @brief Set the beta vector
   * 
   * This routine sets the beta vector and calculates the 
   * transformation matrix.
   */
  void setBeta( const vector4D & beta );

  /** 
   * @brief Set the beta vector for transformation to CM
   *
   * This routine calculates the beta vector for a boost to the CM
   * of the given momentum. It sets the beta vector and the
   * transformation matrix.
   **/
  void setBetaCM( const vector4D & x1 );

  /** 
   * @brief Set the beta vector for transformation to CM
   *
   * This routine calculates the beta vector for a boost to the CM
   * of the given momenta. It sets the beta vector and the
   * transformation matrix.
   **/
  void setBetaCM( const vector4D & x1, const vector4D & x2 );

  /** 
   * @brief Set the beta vector for transformation to CM
   *
   * This routine calculates the beta vector for a boost to the CM
   * of the given momenta. It sets the beta vector and the
   * transformation matrix.
   **/
  void setBetaCM( const vector4D & x1, const vector4D & x2, const vector4D & x3 );
  
   /** 
   * @brief get the spatial distance between two vectors
   *
   * 
   **/ 
  double getSpatialDistance(const vector4D & x1, const vector4D & x2);
  
   /** 
   * @brief get the distance of closest approach DCA
   *
   * 
   **/ 
  double getDCAsquared(const vector4D & x1, const vector4D & x2,const vector4D & p1, const vector4D & p2);
    
    /**
     * @brief get the distance of closest approach DCA constrained to positive times
     *
     *
     **/
    double getConstrainedDCAsquared(const vector4D& x1, const vector4D& x2, const vector4D& p1, const vector4D& p2);
  
   
  

public:
  /** 
   * @brief Return the spatial length of the beta vector
   *
   * This is a shortcut to sqrt(beta().vec2()).
   * It is questionable, how often it is used.
   **/
  Scalar betaVal(void)
  { 
    return sqrt( (Arr[1]*Arr[1]+Arr[2]*Arr[2]+Arr[3]*Arr[3]) )/Arr[0]; 
  };

  /** 
   * @brief Return the boost gamma
   **/
  Scalar gammaVal(void)
  { 
    return 1./(1.-pow ( sqrt( (Arr[1]*Arr[1]+Arr[2]*Arr[2]+Arr[3]*Arr[3]) )/Arr[0],2.0 ) ); 
  };  
  
  
  
  /** 
   * @brief Return the beta vector
   **/
  vector4D beta(void) const
  {
    return vector4D(1.0, Arr[3]/Arr[0], Arr[2]/Arr[0], Arr[1]/Arr[0]); 
  }

  ///// BOOST ONE VECTOR /////

  /** 
   * @brief Return a boosted vector
   **/
  vector4D boost(const vector4D & x) const;

  /** 
   * @brief Return a boosted-back vector
   **/
  vector4D boostInv(const vector4D & x) const;

  /** 
   * @brief Operator overload: Return a boosted vector
   *
   * The multiplication of a boost with a vector returns a boosted vector.
   **/
  vector4D operator* (const vector4D & x) const 
  { return boost(x); }

  /** 
   * @brief Boost one vector
   **/
  void boost(const vector4D & x, vector4D & xNew) const 
  { xNew = boost(x); }; 

  /** 
   * @brief Boost-back one vector
   **/
  void boostInv(const vector4D & x, vector4D & xNew) const 
  { xNew = boostInv(x); }; 


  ///// BOOST TWO VECTORS /////

  /** 
   * @brief Boost two vectors simultanously
   **/
  void boost(const vector4D & x1, const vector4D & x2, 
             vector4D & x1New, vector4D & x2New) const;

  /** 
   * @brief Boost-back two vectors simultanously
   **/
  void boostInv(const vector4D & x1, const vector4D & x2, 
                vector4D & x1New, vector4D & x2New) const;


  ///// BOOST THREE VECTORS /////

  /** 
   * @brief Boost three vectors simultanously
   **/
  void boost(const vector4D & x1, const vector4D & x2, const vector4D & x3, 
             vector4D & x1New, vector4D & x2New, vector4D & x3New) const;

  /** 
   * @brief Boost-back three vectors simultanously
   **/
  void boostInv(const vector4D & x1, const vector4D & x2, const vector4D & x3, 
                vector4D & x1New, vector4D & x2New, vector4D & x3New) const;

  ///// MISC /////

  /** 
   * @brief Output routine
   *
   * Writes out the transformation matrix in human readable form.
   **/
  friend std::ostream& operator<<(std::ostream &os, const lorentz &obj)
  {
    os << std::setprecision(15) << " (" << obj.Arr[0] 
       << ","  << obj.Arr[3] 
       << ","  << obj.Arr[2] 
       << ","  << obj.Arr[1] << ") " << std::endl;
    for (unsigned int i=3;i>0;i--)
      os << std::setprecision(15) << " (" << obj.Arr[0+4*i] 
         << ","  << obj.Arr[3+4*i] 
         << ","  << obj.Arr[2+4*i] 
         << ","  << obj.Arr[1+4*i] << ") " << std::endl;

    return os;
  }

  ////// now the Scalar versions:
  //
  // For testing purposes we may declare the following routines also
  // as public. In the 'final' version, they should be at least
  // 'protected'. 

  //protected:


  /** 
   * @brief Set the beta vector (Scalar version)
   */
  void setBetaScalar( const vector4D & beta );

  /** 
   * @brief Set the beta vector for transformation to CM (Scalar version)
   **/
  void setBetaCMScalar( const vector4D & x1 );

  /** 
   * @brief Set the beta vector for transformation to CM (Scalar version)
   **/
  void setBetaCMScalar( const vector4D & x1, const vector4D & x2 );

  ///// BOOST ONE VECTOR /////

  /** 
   * @brief Return a boosted vector (Scalar version)
   **/
  vector4D boostScalar(const vector4D & x) const;

  /** 
   * @brief Boost one vector (Scalar version)
   **/
  void boostScalar(const vector4D & x, vector4D & xNew) const 
  { xNew = boostScalar(x); }; 

  /** 
   * @brief Return a boosted-back vector (Scalar version)
   **/
  vector4D boostInvScalar(const vector4D & x) const;

  /** 
   * @brief Boost-back one vector (Scalar version)
   **/
  void boostInvScalar(const vector4D & x, vector4D & xNew) const 
  { xNew = boostInvScalar(x); }; 


  ///// BOOST TWO VECTORS /////

  /** 
   * @brief Boost two vectors simultanously (Scalar version)
   **/
  void boostScalar(const vector4D & x1, const vector4D & x2, 
                   vector4D & x1New, vector4D & x2New) const;

  /** 
   * @brief Boost-back two vectors simultanously (Scalar version)
   **/
  void boostInvScalar(const vector4D & x1, const vector4D & x2, 
                      vector4D & x1New, vector4D & x2New) const;

  ///// BOOST THREE VECTORS /////

  void boostScalar( const vector4D & x1, const vector4D & x2, const vector4D & x3,
                    vector4D & x1New, vector4D & x2New, vector4D & x3New ) const;

  void boostInvScalar( const vector4D & x1, const vector4D & x2, const vector4D & x3, 
                       vector4D & x1New, vector4D & x2New, vector4D & x3New ) const;


 
};


/** 
 * @brief exception class for handling unexpected critical behaviour
 * within lorentz class
 */
class eLorentz_error : public std::runtime_error
{
public:
  explicit eLorentz_error(const std::string& what) : std::runtime_error(what) 
  {};
    
  virtual ~eLorentz_error() throw() {};
};




#endif
