//========================================================================================================================================================
//
//  Campaign 10 by JP
//
//========================================================================================================================================================

#include "c10_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

#define M10_MISSION_FAILED          M08_SCRUBBED
#define M10_HIT_SWITCH              M13_HIT_SWITCH
#define M10_MOP_UP                  M08_MOP_UP
#define M10_TURRET_DESTROYED        M08_TURRET_DESTROYED

#define TEXT_M10_TURRET_DESTROYED   TEXT_M08_TURRET_DESTROYED

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c10_data
{
    s32     INVEN_RADIUS;
    s32     VSTATION_RADIUS;
    s32     GET_IN_VEHICLE_WARN_TIME;
    s32     GET_IN_VEHICLE_FAIL_TIME;
    s32     DESTINATION_RADIUS;
    s32     TRAVEL_WARN_TIME;
    s32     TRAVEL_FAIL_TIME;
    s32     BOMBER_START_TIME;
    s32     BOMBER_TARGET_RADIUS;
    s32     BOMBER_AUDIO_TIME;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c10_data  C10_Data = { 2, 2, 120, 240, 200, 60, 120, 120, 100, 30 };

c10_logic C10_Logic;

//========================================================================================================================================================

void c10_logic::BeginGame( void )
{
    ASSERT( m_InventoryStation != obj_mgr::NullID );
    ASSERT( m_VehicleStation   != obj_mgr::NullID );
    ASSERT( m_Destination      != obj_mgr::NullID );
    ASSERT( m_Turret1          != obj_mgr::NullID );
    ASSERT( m_Turret2          != obj_mgr::NullID );
    ASSERT( m_Turret3          != obj_mgr::NullID );
    ASSERT( m_Switch           != obj_mgr::NullID );
    ASSERT( m_Trigger1         != obj_mgr::NullID );
    ASSERT( m_Trigger2         != obj_mgr::NullID );

    m_Bomber = obj_mgr::NullID;
    
    campaign_logic::BeginGame();
    SetVehicleLimits( 1, 0, 0, 0 );
    
    SetState( STATE_GETTING_READY );
    SetCampaignObjectives( 29 );
}

//========================================================================================================================================================

void c10_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c10_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE          : return;
        case STATE_GETTING_READY : GettingReady(); break;
        case STATE_TRAVEL        : Travel();       break;
        case STATE_ATTACK        : Attack();       break;
        default                  :                 break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M10_MISSION_FAILED );
    }

    //
    // update the bomber
    //

    xbool Destroyed = UpdateVehicle( m_Bomber, DeltaTime );

    if( Destroyed )
    {
        Failed( M10_BOMBER_LOST );
    }
    
    m_Time  += DeltaTime;
    m_Time2 += DeltaTime;
}

//========================================================================================================================================================

void c10_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint  ( ItemID, pTag )) return;
    if( AddVehiclePoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "INVEN1"    )) m_InventoryStation = ItemID;
    if( !x_strcmp( pTag, "VSTATION1" )) m_VehicleStation   = ItemID;
    if( !x_strcmp( pTag, "TURRET1"   )) m_Turret1          = ItemID;
    if( !x_strcmp( pTag, "TURRET2"   )) m_Turret2          = ItemID;
    if( !x_strcmp( pTag, "TURRET3"   )) m_Turret3          = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"    )) m_Destination      = ItemID;
    if( !x_strcmp( pTag, "WAYPT2"    )) m_BomberTarget     = ItemID;
    if( !x_strcmp( pTag, "WAYPT3"    )) m_Trigger1         = ItemID;
    if( !x_strcmp( pTag, "WAYPT4"    )) m_Trigger2         = ItemID;
    if( !x_strcmp( pTag, "SWITCH1"   )) m_Switch           = ItemID;
}

//========================================================================================================================================================

void c10_logic::ItemDestroyed( object::id ItemID,  object::id OriginID )
{
    (void)OriginID;
    
    //
    // check if a turret has been destroyed
    //

    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    
    if( pObject->GetType() == object::TYPE_TURRET_FIXED )
    {
        GameInfo( TEXT_M10_TURRET_DESTROYED );
        //GamePrompt( M10_TURRET_DESTROYED );
    }
    
    if(( ItemID == m_Turret1 ) ||
       ( ItemID == m_Turret2 ) ||
       ( ItemID == m_Turret3 ))
    {
        m_nTurretsDestroyed++;
        ClearWaypoint( ItemID );
    }
}

//========================================================================================================================================================

void c10_logic::GettingReady( void )
{
    c10_data& D = C10_Data;

    //
    // initialize GettingReady state
    //

    if( !m_bInitialized )
    {
        m_bInitialized         = TRUE;
        m_bAttackWave2         = FALSE;
        m_bInventoryStation    = FALSE;
        m_bVehicleStation      = FALSE;
        m_bGetInVehicleWarning = FALSE;
        m_bPlayerOnVehicle     = FALSE;
        m_nTurretsDestroyed    = 0;
        
        SpawnEnemies( ATTACK_WAVE_1 );
        SpawnEnemies( ATTACK_WAVE_2 );
        SpawnEnemies( ATTACK_WAVE_3 );
        
        OrderEnemies( TASK_PATROL, m_BomberTarget, ATTACK_WAVE_1 );
        OrderEnemies( TASK_ANTAGONIZE,             ATTACK_WAVE_2 );
        OrderEnemies( TASK_ANTAGONIZE,             ATTACK_WAVE_3 );

        AddWaypoint( m_InventoryStation );
        AddWaypoint( m_VehicleStation   );
        
        GamePrompt( M10_INTRO );
    }

    //
    // check for player visiting inventory and vehicle stations
    //

    if( !m_bInventoryStation && DistanceToObject( m_InventoryStation ) < D.INVEN_RADIUS )
    {
        m_bInventoryStation = TRUE;
        
        ClearWaypoint( m_InventoryStation );
    }

    if( !m_bVehicleStation && DistanceToObject( m_VehicleStation ) < D.VSTATION_RADIUS )
    {
        m_bVehicleStation = TRUE;
        
        ClearWaypoint( m_VehicleStation );
    }

    //
    // ensure player gets in a vehicle within the allotted time
    //
    
    if( m_Time > D.GET_IN_VEHICLE_FAIL_TIME )
    {
        Failed( M10_SCRUBBED );
    }
    else
    {
        if( !m_bGetInVehicleWarning && m_Time > D.GET_IN_VEHICLE_WARN_TIME )
        {
            m_bGetInVehicleWarning = TRUE;
            
            GamePrompt( M10_GET_GOING );
        }
    }
    
    //
    // wait until player gets in a vehicle
    //
    
    if( m_bPlayerOnVehicle )
    {
        if( !m_bInventoryStation ) ClearWaypoint( m_InventoryStation );
        if( !m_bVehicleStation   ) ClearWaypoint( m_VehicleStation   );
        
        SetState( STATE_TRAVEL );
    }
}

//========================================================================================================================================================

void c10_logic::Travel( void )
{
    c10_data& D = C10_Data;

    //
    // initialize Travel state
    //

    if( !m_bInitialized )
    {
        m_bInitialized   = TRUE;
        m_bTravelWarning = FALSE;

        AddWaypoint( m_Destination );
        
        GamePrompt( M10_GO_TO_MARK );
    }

    //
    // ensure player gets to destination within the allotted time
    //

    if( DistanceToObject( m_Destination ) < D.DESTINATION_RADIUS )
    {
        SetState( STATE_ATTACK );
    }
    else
    {
        if( m_Time > D.TRAVEL_FAIL_TIME )
        {
            Failed( M10_NO_BOMBER );
        }
        else
        {
            if( !m_bTravelWarning && m_Time > D.TRAVEL_WARN_TIME )
            {
                m_bTravelWarning = TRUE;
        
                GamePrompt( M10_WHY_WAIT );
            }
        }
    }
}

//========================================================================================================================================================

void c10_logic::Attack( void )
{
    c10_data& D = C10_Data;

    //
    // initialize Attack state
    //

    if( !m_bInitialized )
    {
        m_bInitialized      = TRUE;
        m_bBomberStarted    = FALSE;
        m_bBombsDropped     = FALSE;
        m_bDefendBase       = FALSE;
        m_bCleanup          = FALSE;
        m_bHitSwitch        = FALSE;
        m_bGetEnemies       = FALSE;
        m_bGetSwitchBack    = FALSE;
        m_bSwitchLost       = FALSE;
        m_SwitchState       = TEAM_ENEMY;
        m_Time2             = 0.0f;

        OrderEnemies( TASK_DEATHMATCH, ATTACK_WAVE_1 );

        AddWaypoint( m_Turret1 );
        AddWaypoint( m_Turret2 );
        AddWaypoint( m_Turret3 );

        GamePrompt( M10_ON_OWN );
    }

    //
    // check if we need to start up the bomber
    //
    
    if( !m_bBomberStarted && m_Time > D.BOMBER_START_TIME )
    {
        m_bBomberStarted = TRUE;
        
        m_Bomber = InitVehicle( object::TYPE_VEHICLE_THUNDERSWORD );
        
        GamePrompt( M10_INCOMING );
    }
    
    //
    // check if bomber is within range of target
    //

    if( m_bBomberStarted )
    {
        if( DistanceToObject( m_Bomber, m_BomberTarget ) < D.BOMBER_TARGET_RADIUS )
        {
            m_bBombsDropped = TRUE;
            FireVehicleWeapon();
        
            if( m_bDefendBase == FALSE )
            {
                m_bDefendBase = TRUE;
                OrderEnemies( TASK_DEATHMATCH, ATTACK_WAVE_2 );
            }
        }
        else
        {
            // When the bombs are dropped - have some bots run outside to be slaughtered!
            if( m_bBombsDropped )
            {
                if( m_Time2 > D.BOMBER_AUDIO_TIME )
                {
                    if( !m_bCleanup )
                    {
                        m_bCleanup = TRUE;

                        AddWaypoint( m_Switch );

                        if( m_nEnemies && ( m_SwitchState == TEAM_ENEMY ))
                        {
                            GamePrompt( M10_GO_TO_SWITCH );
                        }
                    }
                }
            }
            else
            {
                m_Time2 = 0.0f;
            }
        }
    }
    
    //
    // check for all enemies dead and switch not hit
    //

    if( !m_bHitSwitch && ( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ENEMY ))
    {
        m_bHitSwitch = TRUE;
        
        GamePrompt( M10_HIT_SWITCH );
    }

    //
    // check for switch hit but enemies still remaining
    //
    
    if( !m_bGetEnemies && ( m_nEnemies > 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        m_bGetEnemies = TRUE;
        
        GamePrompt( M10_MOP_UP );
    }
    
    //
    // check for switch being taken back by enemies
    //
    
    if( !m_bGetSwitchBack && m_bSwitchLost )
    {
        m_bGetSwitchBack = TRUE;
        
        GamePrompt( M10_GET_SWITCH );
    }
    
    //
    // check for mission success
    //
    
    if(( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        Success( M10_SUCCESS );
    }
}

//========================================================================================================================================================
