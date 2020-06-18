
#include <iostream>
#include <math.h>
#include <string>

#ifndef PARTICLE_CPP
#define PARTICLE_CPP

/** @file
 * @brief Declarations for the Particle class
 */


/**
 * @brief Enumeration type for the flavor of a particle
 *
 * With this, the names given in the enum list can be used instead of
 * integers (though their use is perfectly legal still).
 */
enum FLAVOR_TYPE {
    gluon,                  ///< 0: g (gluon)
    up,                     ///< 1: u (up)
    anti_up,                ///< 2: ub (anti-up)
    down,                   ///< 3: d (down)
    anti_down,              ///< 4: db (anti-down)
    strange,                ///< 5: s (strange)
    anti_strange,           ///< 6: sb (anti-strange)
    charm,                  ///< 7: c (charm)
    anti_charm,             ///< 8: cb (anti-charm)
    bottom,                 ///< 9: b (bottom)
    anti_bottom,            ///< 10: bb (anti-bottom)
    photon          =   13, ///< 13: p (photon)
    electron        = 1000, ///< 1000: electron
    positron        = 1001, ///< 1001: positron
    // generalized flavor type which should not assigned to an individual
    // particle, but is used for analysis purposes
    quark           =   77, ///< 77: quark = light quark or anti-light-quark
    light_quark     =   88, ///< 88: light quark (generic)
    anti_light_quark=   99, ///< 99: light anti-quark (generic)
    allFlavors      =  111, ///< 111: any flavor (generic)
    electron_gen    =  335       ///< 335:  all electrons/positrons (generic)
};


/**
 * @brief Provides generic properties of a particle
 *
 * This class encapsulates properties of particles that are needed for
 * the simulation process, such as position and momentum variables.
 *
 */
class Particle
{
    public:
    /** @brief Provide standard constructor (for completeness) */
    Particle() :
    Pos( 0,0,0,0 ),
    Mom( 0,0,0,0 ),
    m( 0 ),
    FLAVOR( gluon ),
    unique_id( -1 )
    { };
    
    /** @brief time [fm/c] and spatial [fm] coordinates */
    VectorTXYZ Pos;
    
    /** @brief energy [GeV] and momentum [GeV/c] */
    VectorEPxPyPz Mom;
    
    /** @brief mass [GeV] */
    double m;
    
    /** @brief flavor of the particle mapped to int according to FLAVOR_TYPE */
    FLAVOR_TYPE FLAVOR;
    
    /** @brief Species according to PDG notation -- only used for HERWIG Input */
    int PDGSpecies;
    
    /** @brief unique particle ID */
    int unique_id;
    
    /** @brief counter for unique particle IDs (static) */
    static int unique_id_counter;
    
    /**
     * @brief Map a flavor to a generic particle type (gluon, quark, anti-quark, etc.).
     *
     * Do not group heavy quarks since we are interested in rates for
     * charm and bottom individually
     */
    static FLAVOR_TYPE mapToGenericFlavorType( const FLAVOR_TYPE _flav )
    {
        switch ( _flav )
        {
            case up:
            case down:
            case strange:
            return light_quark;
            
            case anti_up:
            case anti_down:
            case anti_strange:
            return anti_light_quark;
            
            default:
            return _flav;
        }
    }
    
    /**
     * @brief Map a flavor to generic particle type quark with no difference quark-antiquark.
     */
    static FLAVOR_TYPE mapQuarkGluon( const FLAVOR_TYPE _flav )
    {
        switch ( _flav )
        {
            case up:
            case down:
            case strange:
            case anti_up:
            case anti_down:
            case anti_strange:
            return quark;
            
            default:
            return _flav;
        }
    }
    /**
     * @brief Return the name of the flavor as a string
     */
    std::string getName( const FLAVOR_TYPE _flav )
    {
        std::string name;
        
        switch ( _flav )
        {
            case gluon:
            name = "gluon";
            break;
            case up:
            name = "up";
            break;
            case down:
            name = "down";
            break;
            case strange:
            name = "strange";
            break;
            case charm:
            name = "charm";
            break;
            case bottom:
            name = "bottom";
            break;
            case anti_up:
            name = "anti_up";
            break;
            case anti_down:
            name = "anti_down";
            break;
            case anti_strange:
            name = "anti_strange";
            break;
            case photon:
            name = "photon";
            break;
            case electron:
            name = "electron";
            break;
            case positron:
            name = "positron";
            break;
            case quark:
            name = "quark";
            break;
            case light_quark:
            name = "light_quark";
            break;
            case anti_light_quark:
            name = "anti_light_quark";
            break;
            case allFlavors:
            name = "allFlavors";
            break;
            case electron_gen:
            name = "all_electron";
            break;
            default:
            name = "unknown";
            break;
        }
        
        
        
        return name;
    }
    
    
    
    
    /**
     * @brief Map a flavor to a generic particle type (gluon, quark, anti-quark, etc.).
     *
     * Do not group heavy quarks since we are interested in rates for
     * charm and bottom individually
     */
    static int mapToPDG( const FLAVOR_TYPE _flav )
    {
        switch ( _flav )
        {
            case up:
            return 2;
            case down:
            return 1;
            case strange:
            return 3;
            
            case anti_up:
            return -2;
            case anti_down:
            return -1;
            case anti_strange:
            return -3;
            
            case gluon:
            return 21;
            
            default:
            return 0;
        }
    }
    
    
    
    
    
    /** @brief Whether particle is gluon or light quark */
    inline bool isLightFlavor() const
    {
        return (FLAVOR <= 2 * N_light_flavor);
    }
    
    /** @brief Whether particle is gluon */
    inline bool isGluon() const
    {
        return (FLAVOR == 0);
    }
    
    /** @brief number of active light flavors
     * ( only gluons (0), up (1), down (2), strange (3))
     */
    static int N_light_flavor;
    
    
};


/** @brief number of active light flavors */
int Particle::N_light_flavor = 3;

/**
* Initialize static member for unique numbering of particles
*/
int Particle::unique_id_counter = 0;


#endif
