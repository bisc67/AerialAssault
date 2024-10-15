//==============================================================================
//
//  Bomber.hpp
//  Class for the Bomber vehicle
//
//==============================================================================

#ifndef BOMBER_HPP
#define BOMBER_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "gndeffect.hpp"
#include "contrail.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

//==============================================================================
//  FUNCTIONS
//==============================================================================

void        Bomber_InitFX( void );
object*     BomberCreator( void );

//==============================================================================
//  TYPES
//==============================================================================

class bomber: public gnd_effect
{
// Defines

public:

/*    
    // Bomber weapon types
    enum weapon_type
    {
        TURRET,
        BOMB,
        LASER,
    } ;
*/


// Functions
public:

    update_code     Impact              ( vector3& Pos, vector3& Norm );

    void            OnAdd               ( void );
    void            Initialize          ( const vector3& Pos, radian InitHdg, u32 TeamBits );
    void            SetupSeat           ( s32 Index, seat& Seat ) ;
    void            OnAcceptInit        ( const bitstream& BitStream );
                                    
//  void            UpdateInstances     ( void ) ;
    void            Render              ( void );
//  void            RenderBombTarget    ( void );
    void            RenderParticles     ( void );
    
    void            Get3rdPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );
//  void            Get1stPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );

    void            ApplyMove           ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move ) ;

    void            WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate ) ;
    xbool           ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast ) ;

    update_code     OnAdvanceLogic      ( f32 DeltaTime );
    void            OnRemove            ( void );

//  void            SetBomberWeaponType ( weapon_type Type ) ;
//  s32             GetWeaponType       ( void ) const;
    f32             GetPainRadius       ( void ) const;
    void            GetBombPoint        ( vector3& Pos );
    vector3         GetBlendFirePos     ( s32 Index = 0 );

private:
//  void        CalcTargetPos       ( vector3& Pos );

    // pfx_effect                  m_Exhaust1[ NUM_EXHAUST_PTS ];
    
    contrail                    m_LTrail;
    contrail                    m_RTrail;
    hot_point*                  m_LeftHP;
    hot_point*                  m_RightHP;
    hot_point*                  m_RThrust;
    hot_point*                  m_LThrust;

    s32                         m_AudioEng;
    s32                         m_AudioAfterBurn;

    // Belly turret vars
//  shape_instance              m_TurretBaseInstance ;          // Turret base
//  shape_instance              m_TurretBarrelInstance[2] ;     // Left/right turret barrel
//  hot_point*                  m_TurretBarrelHotPoint[2] ;     // Position for barrel turrets
//  s32                         m_TurretBarrelFireSide ;        // Side to fire turret from

//  s32                         m_BomberWeaponType ;            // Current weapon

};


#endif