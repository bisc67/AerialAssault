//==============================================================================
//
//  MPB.hpp
//  Class for the MPB vehicle
//
//==============================================================================

#ifndef MPB_HPP
#define MPB_HPP

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

void        MPB_InitFX( void );
object*     MPBCreator( void );

//==============================================================================
//  TYPES
//==============================================================================

class mpb: public gnd_effect
{

//------------------------------------------------------------------------------
//  Functions  

                                    
public:                             

                mpb                 ( void );    

    update_code Impact              ( vector3& Pos, vector3& Norm );

    void        OnAdd               ( void );
    void        Initialize          ( const vector3& Pos, radian InitHdg, u32 TeamBits );
    void        SetupSeat           ( s32 Index, seat& Seat ) ;
    void        OnAcceptInit        ( const bitstream& BitStream );
    void        WriteDirtyBitsData  ( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate ) ;
    xbool       ReadDirtyBitsData   ( const bitstream& BitStream, f32 SecInPast ) ;

                                    
    void        UpdateInstances     ( void ) ;
    void        Render              ( void );
    void        RenderParticles     ( void );
    
    void        Get3rdPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );
    void        Get1stPersonView    ( s32 Seat, vector3& Pos, radian3& Rot );

    void        ApplyMove           ( player_object* pPlayer, s32 SeatIndex, player_object::move& Move ) ;

    update_code OnAdvanceLogic      ( f32 DeltaTime );
    void        OnRemove            ( void );
    f32         GetPainRadius       ( void ) const;

protected:

    void        CreateAssets        ( void );
    void        DestroyAssets       ( void );
    void        EnableAssets        ( xbool Enable );
    xbool       CheckTurretArea     ( void );
    xbool       CheckInvenArea      ( void );
    xbool       CheckForBldgs       ( void );
    xbool       CheckForValidSlope  ( void );
    void        CalcSink            ( void );
    void        SetMPBPos           ( void );

//------------------------------------------------------------------------------
//  Storage

protected:

    enum state
    {
        NOT_DEPLOYED,
        ATTEMPTING_TO_DEPLOY,
        DEPLOYING,
        DEPLOYED,
        ATTEMPTING_TO_UNDEPLOY,
        UNDEPLOYING
    } ;

    pfx_effect          m_Exhaust1;
    pfx_effect          m_Exhaust2;
    
    state               m_DeployedState;
    vector3             m_PreDeployPos;
    vector3             m_PostDeployPos;
    
    f32                 m_DeployTime;
    f32                 m_ImpulseTimer;

    s32                 m_AudioEng;
    s32                 m_AudioAfterBurn;
    s32                 m_DeploySfx;
    xbool               m_ClientProcessedState;

    object::id          m_TurretID;
    object::id          m_StationID;
    xbool               m_AssetsCreated;
};                                 

//==============================================================================
#endif  // MPB_HPP
//==============================================================================
