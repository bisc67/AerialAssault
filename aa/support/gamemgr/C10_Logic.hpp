//========================================================================================================================================================
//
//  Campaign 10 Logic
//
//========================================================================================================================================================

#ifndef C10_LOGIC_HPP
#define C10_LOGIC_HPP

//========================================================================================================================================================
//  INCLUDES
//========================================================================================================================================================

#include "Campaign_Logic.hpp"

//========================================================================================================================================================
//  DEFINES
//========================================================================================================================================================

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

class c10_logic : public campaign_logic
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Public Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

public:

virtual void    BeginGame       ( void );
virtual void    EndGame         ( void );
virtual void    AdvanceTime     ( f32 DeltaTime );
virtual void    RegisterItem    ( object::id ItemID, const char* pTag );
virtual void    ItemDestroyed   ( object::id ItemID, object::id OriginID );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Types
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    enum game_status
    {
        STATE_IDLE,
        STATE_GETTING_READY,
        STATE_TRAVEL,
        STATE_ATTACK,
    };
    
//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Functions
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:
        
        void        GettingReady    ( void );
        void        Travel          ( void );
        void        Attack          ( void );

//--------------------------------------------------------------------------------------------------------------------------------------------------------
//  Private Data
//--------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    //
    // Objects
    //

    object::id      m_InventoryStation;
    object::id      m_VehicleStation;
    object::id      m_Destination;
    object::id      m_BomberTarget;
    object::id      m_Turret1;
    object::id      m_Turret2;
    object::id      m_Turret3;
    object::id      m_Trigger1;
    object::id      m_Trigger2;
    object::id      m_Switch;

    object::id      m_Bomber;
    
    //
    // Flags
    //

    xbool           m_bAttackWave2;
    xbool           m_bInventoryStation;
    xbool           m_bVehicleStation;
    xbool           m_bGetInVehicleWarning;
    xbool           m_bTravelWarning;
    xbool           m_bBomberStarted;
    xbool           m_bBombsDropped;
    xbool           m_bDefendBase;
    xbool           m_bCleanup;
    xbool           m_bHitSwitch;
    xbool           m_bGetEnemies;
    xbool           m_bGetSwitchBack;
    xbool           m_bTurretDestroyed;
    
    //
    // Variables
    //
    
    s32             m_nTurretsDestroyed;
    f32             m_Time2;
};

//========================================================================================================================================================
//  GLOBAL VARIABLES
//========================================================================================================================================================

extern c10_logic C10_Logic;

//========================================================================================================================================================
#endif
//========================================================================================================================================================

