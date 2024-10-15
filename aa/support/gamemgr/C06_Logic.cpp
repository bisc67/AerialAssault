//========================================================================================================================================================
//
//  Campaign 06 by JP
//
//========================================================================================================================================================

#include "c06_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c06_data
{
    s32     GET_READY_WARN_TIME;
    s32     GET_READY_FAIL_TIME;
    s32     INVEN1_RADIUS1;
    s32     VSTATION_RADIUS2;
    
    s32     AWAY_FROM_BASE_RADIUS;
    s32     AWAY_FROM_BASE_WARN_TIME;
    s32     AWAY_FROM_BASE_FAIL_TIME;
    
    s32     TRAVEL_WARN_TIME;
    s32     TRAVEL_FAIL_TIME;
    s32     DESTINATION_RADIUS;
    
    s32     SIEGE_WAVE1_TIME;
    s32     SIEGE_WAVE1_DELAY;
    s32     MIN_ENEMIES_WAVE1;
    s32     MIN_ENEMIES_WAVE2;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c06_data  C06_Data = { 60, 120, 2, 10, 200, 1, 20, 180, 240, 100, 180, 5, 5, 3 };

c06_logic C06_Logic;

//========================================================================================================================================================

void c06_logic::BeginGame( void )
{
    ASSERT( m_InventoryStation2 != obj_mgr::NullID );
    ASSERT( m_InventoryStation3 != obj_mgr::NullID );
    ASSERT( m_InventoryStation4 != obj_mgr::NullID );
    ASSERT( m_VehicleStation    != obj_mgr::NullID );
    ASSERT( m_DestinationBase   != obj_mgr::NullID );
    ASSERT( m_Generator         != obj_mgr::NullID );
    ASSERT( m_Turret1           != obj_mgr::NullID );
    ASSERT( m_Turret2           != obj_mgr::NullID );

    DestroyItem( m_Generator );
    DestroyItem( m_Turret1   );
    DestroyItem( m_Turret2   );

    campaign_logic::BeginGame();
    SetVehicleLimits( 1, 1, 0, 0 );

    m_nBarrelsDeployed = 0;

    SetState( STATE_GET_READY );
    SetCampaignObjectives( 26 );
}

//========================================================================================================================================================

void c06_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c06_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE      : return;
        case STATE_GET_READY : GetReady(); break;
        case STATE_TRAVEL    : Travel();   break;
        case STATE_SIEGE     : Siege();    break;
        default              :             break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M06_FAIL );
    }

    m_Time  += DeltaTime;
    m_Time2 += DeltaTime;
}

//========================================================================================================================================================

void c06_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "INVEN2"    )) m_InventoryStation2 = ItemID;
    if( !x_strcmp( pTag, "INVEN3"    )) m_InventoryStation3 = ItemID;
    if( !x_strcmp( pTag, "INVEN4"    )) m_InventoryStation4 = ItemID;
    if( !x_strcmp( pTag, "VSTATION1" )) m_VehicleStation    = ItemID;
    if( !x_strcmp( pTag, "GEN1"      )) m_Generator         = ItemID;
    if( !x_strcmp( pTag, "TURRET1"   )) m_Turret1           = ItemID;
    if( !x_strcmp( pTag, "TURRET2"   )) m_Turret2           = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"    )) m_DestinationBase   = ItemID;
}

//========================================================================================================================================================

void c06_logic::ItemRepaired( object::id ItemID,  object::id OriginID, xbool Score )
{
    (void)OriginID;
    (void)Score;

    //
    // check if our generator has been repaired
    //
    
    if( ItemID == m_Generator )
    {
        ClearWaypoint( m_Generator );
        
        AddWaypoint( m_InventoryStation2 );
        AddWaypoint( m_InventoryStation3 );
        AddWaypoint( m_InventoryStation4 );
        
        if( !m_bTurret1Repaired ) AddWaypoint( m_Turret1 );
        if( !m_bTurret2Repaired ) AddWaypoint( m_Turret2 );
    
        if( !m_bTurret1Repaired || !m_bTurret2Repaired )
        {
            GamePrompt( M06_GO_TO_TURRET );
        }
    }
    
    if( ItemID == m_Turret1 )
    {
        m_bTurret1Repaired = TRUE;
        ClearWaypoint( m_Turret1 );
    }

    if( ItemID == m_Turret2 )
    {
        m_bTurret2Repaired = TRUE;
        ClearWaypoint( m_Turret2 );
    }
}

//========================================================================================================================================================

void c06_logic::ItemRepairing( object::id ItemID, object::id OriginID )
{
    (void)OriginID;

    if( ItemID == m_Generator )
    {
        if( !m_bGeneratorHelp )
        {
            m_bGeneratorHelp = TRUE;
            GamePrompt( M06_REPAIR_GEN );
        }
    }
    
    if(( ItemID == m_Turret1 ) || ( ItemID == m_Turret2 ))
    {
        if( !m_bTurretHelp )
        {
            m_bTurretHelp = TRUE;
            GamePrompt( M06_REPAIR_TURRET );
        }
    }
}

//========================================================================================================================================================

void c06_logic::ItemDeployed( object::id ItemID, object::id OriginID )
{
    (void)OriginID;
    
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    
    if( pObject->GetType() == object::TYPE_TURRET_FIXED )
    {
        m_nBarrelsDeployed++;
    }
}

//========================================================================================================================================================

void c06_logic::GetReady( void )
{
    c06_data& D = C06_Data;

    //
    // initialize GetReady state
    //

    if( m_bInitialized == FALSE )
    {
        m_bInitialized       = TRUE;
        m_bVehicleStation    = FALSE;
        m_bGetReadyWarning   = FALSE;
        m_bPlayerOnVehicle   = FALSE;
        m_bTurret1Repaired   = FALSE;
        m_bTurret2Repaired   = FALSE;
        m_bTurretsRepaired   = FALSE;
        m_bGeneratorHelp     = FALSE;
        m_bTurretHelp        = FALSE;

        AddWaypoint( m_VehicleStation );

        GamePrompt( M06_INTRO );
    }

    //
    // check if player has found the vehicle station
    //
    
    if( m_bVehicleStation == FALSE )
    {
        if( DistanceToObject( m_VehicleStation ) < D.VSTATION_RADIUS2 )
        {
            m_bVehicleStation = TRUE;
        
            ClearWaypoint( m_VehicleStation );
        }
    }

    //
    // make sure player does not stray too far from vehicle station
    //

    if( DistanceToObject( m_VehicleStation ) > D.AWAY_FROM_BASE_RADIUS )
    {
        if( m_Time2 > D.AWAY_FROM_BASE_FAIL_TIME )
        {
            Failed( M06_SCRUBBED );
        }
        else
        {
            if( !m_bGetReadyWarning && ( m_Time2 > D.AWAY_FROM_BASE_WARN_TIME ))
            {
                m_bGetReadyWarning = TRUE;
                GamePrompt( M06_GO_TO_STATION );
            }
        }
    }
    else
    {
        m_Time2 = 0;
    }

    //
    // make sure player gets on a vehicle within the time limit
    //
    
    if( m_bPlayerOnVehicle == TRUE )
    {
        if( !m_bVehicleStation ) ClearWaypoint( m_VehicleStation );
        
        SetState( STATE_TRAVEL );
    }
    else
    {
        if( m_Time > D.GET_READY_FAIL_TIME )
        {
            Failed( M06_SCRUBBED );
        }
        else
        {
            if( !m_bGetReadyWarning && ( m_Time > D.GET_READY_WARN_TIME ))
            {
                m_bGetReadyWarning = TRUE;
            
                GamePrompt( M06_GO_TO_STATION );
            }
        }
    }
}

//========================================================================================================================================================

void c06_logic::Travel( void )
{
    c06_data& D = C06_Data;

    //
    // initialize Travel state
    //
    
    if( !m_bInitialized )
    {
        m_bInitialized   = TRUE;
        m_bTravelWarning = FALSE;
        
        AddWaypoint( m_DestinationBase );
        
        GamePrompt( M06_GO_TO_BASE );
    }
    
    //
    // check if player has reached the destination base in the allotted time
    //

    if( DistanceToObject( m_DestinationBase ) < D.DESTINATION_RADIUS )
    {
        ClearWaypoint( m_DestinationBase );
        SetState( STATE_SIEGE );
    }
    else
    {
        if( m_Time > D.TRAVEL_FAIL_TIME )
        {
            Failed( M06_FAIL );
        }
        else
        {
            if( !m_bTravelWarning && m_Time > D.TRAVEL_WARN_TIME )
            {
                m_bTravelWarning = TRUE;
                
                GamePrompt( M06_TIME );
            }
        }
    }
}

//========================================================================================================================================================

void c06_logic::Siege( void )
{
    c06_data& D = C06_Data;

    //
    // initialize Siege state
    //
    
    if( !m_bInitialized )
    {
        m_bInitialized     = TRUE;
        m_bSiegeWave1      = FALSE;
        m_bSiegeWave2      = FALSE;
        m_bSiegeWave3      = FALSE;
        m_bBarrelsDeployed = FALSE;
    
        AddWaypoint( m_Generator );
        GamePrompt( M06_GO_TO_GEN );
    }

    //
    // check for player repairing turrets
    //
    
    if( !m_bTurretsRepaired && m_bTurret1Repaired && m_bTurret2Repaired )
    {
        m_bTurretsRepaired = TRUE;
        
        GamePrompt( M06_SWAP_BARRELS );
    }
    
    //
    // Check for both barrels being deployed
    //
    
    if( !m_bBarrelsDeployed && (m_nBarrelsDeployed >= 2) )
    {
        m_bBarrelsDeployed = TRUE;
        
        if( m_Time < D.SIEGE_WAVE1_TIME )
        {
            m_Time = (f32)D.SIEGE_WAVE1_TIME;
        }
    }

    //
    // setup the first attack wave
    //

    if( !m_bSiegeWave1 && (m_Time > (D.SIEGE_WAVE1_TIME + D.SIEGE_WAVE1_DELAY)) )
    {
        m_bSiegeWave1 = TRUE;
        
        SpawnEnemies( ATTACK_WAVE_1 );
        OrderEnemies( TASK_ATTACK, m_PlayerID );
        
        GamePrompt( M06_DEFEND_BASE );
    }

    //
    // setup the second attack wave
    //

    if( m_bSiegeWave1 && !m_bSiegeWave2 && (m_nEnemies <= D.MIN_ENEMIES_WAVE1) )
    {
        m_bSiegeWave2 = TRUE;
        
        SpawnEnemies( ATTACK_WAVE_2 );
        OrderEnemies( TASK_ATTACK, m_PlayerID );
        
        GamePrompt( M06_HANG_IN_THERE );
    }

    // 
    // Setup the third attack wave
    //

    if( m_bSiegeWave2 && !m_bSiegeWave3 && (m_nEnemies <= D.MIN_ENEMIES_WAVE2) )
    {
        m_bSiegeWave3 = TRUE;
        
        SpawnEnemies( ATTACK_WAVE_3 );
        OrderEnemies( TASK_ATTACK, m_PlayerID );
    }

    //
    // wait until all enemies in the 3rd wave are killed
    //

    if( m_bSiegeWave3 && (m_nEnemies == 0) )
    {
        Success( M06_SUCCESS );
    }
}

//========================================================================================================================================================

