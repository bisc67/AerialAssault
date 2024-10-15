//==============================================================================
//
//  AssTank.hpp
//  Class for the AssTank vehicle
//
//==============================================================================

#ifndef ASSTANK_HPP
#define ASSTANK_HPP

//==============================================================================
//  INCLUDES
//==============================================================================
#include "gndeffect.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

//==============================================================================
//  FUNCTIONS
//==============================================================================

void        AssTank_InitFX( void );
object*     AssTankCreator( void );

//==============================================================================
//  TYPES
//==============================================================================

class asstank: public gnd_effect
{

// Functions
public:          
    
    update_code Impact              ( vector3& Pos, vector3& Norm );

    void        OnAdd               ( void );
    void        Initialize          ( const vector3& Pos, radian InitHdg, u32 TeamBits );
    void        SetupSeat           ( s32 Index, seat& Seat ) ;
    void        OnAcceptInit        ( const bitstream& BitStream );
                                    
    void        UpdateInstances     ( void ) ;
    void        Render              ( void );
    void        RenderParticles     ( void );
    
    void        Get3rdPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );
    void        Get1stPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );
    
    s32         GetTurretBarrel     ( void ) const ;
    void        SetTurretBarrel     ( s32 Barrel ) ;
    xbool       ObjectInWayOfTurret ( void ) ;
    f32         GetTurretSpeedScaler( void ) ;

    void        ApplyMove           ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move ) ;

    void        WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate ) ;
    xbool       ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast ) ;

    update_code OnAdvanceLogic      ( f32 DeltaTime );
    void        OnRemove            ( void );

    void        OnCollide           ( u32 AttrBits, collider& Collider ) ;
    matrix4*    GetNodeL2W          ( s32 ID, s32 NodeIndex ) ;

    f32         GetPainRadius       ( void ) const;

private:

    shape_instance              m_TurretBaseInstance ;          // Base object
    shape_instance              m_TurretBarrelMortarInstance ;  // Mortar turret
    shape_instance              m_TurretBarrelChainInstance ;   // Chain turret

    hot_point*                  m_TurretBarrelMortarHotPoint ;  // Position for mortar turret
    hot_point*                  m_TurretBarrelChainHotPoint ;   // Position for chain turret

    s32                         m_TurretCurrentBarrel ;         // Current barrel firing out of
    
    pfx_effect                  m_Exhaust1;
    pfx_effect                  m_Exhaust2;
    pfx_effect                  m_Exhaust3;

    s32                         m_AudioEng;
    s32                         m_AudioAfterBurn;
    s32                         m_AudioChainGunFire ;
    f32                         m_ChainGunFxDelay ;
    xbool                       m_ChainGunHasFired ;
    xbool                       m_ChainGunFiring ;
};


#endif