//========================================================================================================================================================
//
//  Campaign 08 by JP
//
//========================================================================================================================================================

#include "c08_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "GameMgr\MsgMgr.hpp"
#include "Demo1\Data\UI\Messages.h"

#define M08_GET_SWITCH      M10_GET_SWITCH

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c08_data
{
    s32     MOVEOUT_RADIUS;
    s32     MOVEOUT_TIME;
    s32     GROUP_RADIUS;
    s32     GROUP_TIME;
    s32     DESTINATION_RADIUS;
    s32     MIN_NUM_ALLIES;
    s32     TARGET_TIME;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c08_data  C08_Data = { 50, 15, 100, 10, 150, 2, 5 };

c08_logic C08_Logic;

//========================================================================================================================================================

void c08_logic::BeginGame( void )
{
    ASSERT( m_InventoryStation != obj_mgr::NullID );
    ASSERT( m_Destination      != obj_mgr::NullID );
    ASSERT( m_EnemyBase        != obj_mgr::NullID );
    ASSERT( m_Switch           != obj_mgr::NullID );

    for( s32 i=0; i<5; i++ )
    {
        ASSERT( m_Turrets  [i] != obj_mgr::NullID );
        m_bTurretsDestroyed[i]  = FALSE;
    }

    campaign_logic::BeginGame();

    SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MISSILE, 12 );
    SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_DISK,    40 );
    SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_BULLET, 200 );
    SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MORTAR,  20 );
    SetInventCount( player_object::INVENT_TYPE_GRENADE_BASIC,       18 );

    SetState( STATE_TRANSIT );
    SetCampaignObjectives( 28 );
}

//========================================================================================================================================================

void c08_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c08_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE        : return;
        case STATE_TRANSIT     : Transit();     break;
        case STATE_BOMBARDMENT : Bombardment(); break;
        default                :                break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M08_FAIL );
    }

    //
    // check for too many allies being killed
    //

    //c08_data& D = C08_Data;
    //
    //if( m_nAllies < D.MIN_NUM_ALLIES )
    //{
    //    Failed( M08_TERMINATED );
    //}

    m_Time        += DeltaTime;
    m_TargetTimer += DeltaTime;
}

//========================================================================================================================================================

void c08_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "INVEN1"  )) m_InventoryStation = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"  )) m_Destination      = ItemID;
    if( !x_strcmp( pTag, "WAYPT2"  )) m_EnemyBase        = ItemID;
    if( !x_strcmp( pTag, "TURRET1" )) m_Turrets[0]       = ItemID;
    if( !x_strcmp( pTag, "TURRET2" )) m_Turrets[1]       = ItemID;
    if( !x_strcmp( pTag, "TURRET3" )) m_Turrets[2]       = ItemID;
    if( !x_strcmp( pTag, "TURRET4" )) m_Turrets[3]       = ItemID;
    if( !x_strcmp( pTag, "TURRET5" )) m_Turrets[4]       = ItemID;
    if( !x_strcmp( pTag, "SWITCH1" )) m_Switch           = ItemID;
}

//========================================================================================================================================================

void c08_logic::ItemDestroyed( object::id ItemID,  object::id OriginID )
{
    (void)OriginID;

    //
    // check if one of our turrets has been destroyed
    //

    for( s32 i=0; i<5; i++ )
    {
        if( ItemID == m_Turrets[i] )
        {
            m_bTurretsDestroyed[i] = TRUE;
            m_nTurretsDestroyed++;
            ClearWaypoint( ItemID );
            
            GameInfo( TEXT_M08_TURRET_DESTROYED );
            //GamePrompt( M08_TURRET_DESTROYED );   // Commented out: this audio is cancelling out audio you need to hear
        }
    }
}

//========================================================================================================================================================

void c08_logic::Transit( void )
{
    c08_data& D = C08_Data;

    //
    // initialize Transit state
    //

    if( !m_bInitialized )
    {
        m_bInitialized      = TRUE;
        m_bMoveOut          = FALSE;
        m_bTransitWarning1  = FALSE;
        m_bTransitWarning2  = FALSE;
        m_nTurretsDestroyed = 0;
        m_TargetTimer       = 0.0f;
        m_SwitchState       = TEAM_ENEMY;

        // enemies will just stay in and around their base for now

        SpawnEnemies( ATTACK_WAVE_1 );
        SpawnEnemies( ATTACK_WAVE_2 );
        
        OrderEnemies( TASK_DEFEND, m_EnemyBase, ATTACK_WAVE_2 );
        OrderEnemies( TASK_ANTAGONIZE,          ATTACK_WAVE_1 );
        
        // allies will just patrol the area around the player        
        
        SpawnAllies();
        OrderAllies( TASK_ANTAGONIZE );
        
        AddWaypoint( m_Destination );
        
        GamePrompt( M08_INTRO );
    }
    
    //
    // check if player leaves the inventory station area
    //
    
    if( !m_bMoveOut && ((m_Time > D.MOVEOUT_TIME) || (DistanceToObject( m_InventoryStation ) > D.MOVEOUT_RADIUS)) )
    {
        m_bMoveOut = TRUE;

        OrderAllies( TASK_GOTO, m_Destination );
        GamePrompt( M08_MOVE_OUT );
    }
    
    //
    // ensure that player stays within radius of the allied group
    //

    if( DistanceToClosestPlayer( m_PlayerID, TEAM_ALLIES ) > D.GROUP_RADIUS )
    {
        if( !m_bTransitWarning1 ) //&& (m_TargetTimer > D.GROUP_TIME) )
        {
            m_bTransitWarning1 = TRUE;
            m_TargetTimer      = 0;
                
            GamePrompt( M08_STAY_TOGETHER );
        }
        
        if( !m_bTransitWarning2 && (m_TargetTimer > D.GROUP_TIME) )
        {
            m_bTransitWarning2 = TRUE;
            m_TargetTimer      = 0;
            
            GamePrompt( M08_DONT_WANDER );
        }
        
        if( m_TargetTimer > D.GROUP_TIME )
        {
            Failed( M08_SCRUBBED );
        }
    }
    else
    {
        m_TargetTimer = 0;
    }

    //
    // Make sure player doesnt kill all his own guys
    //
    
    if( m_nAllies == 0 )
    {
        Failed( M08_SCRUBBED );
    }
    
    //
    // wait for Allies to reach destination
    //
    
    //if( DistanceToClosestPlayer( m_Destination, TEAM_ALLIES ) < D.DESTINATION_RADIUS )
    if( DistanceToObject( m_Destination ) < D.DESTINATION_RADIUS )
    {
        ClearWaypoint( m_Destination );
    
        SetState( STATE_BOMBARDMENT );
    }
}

//========================================================================================================================================================

void c08_logic::Bombardment( void )
{
    c08_data& D = C08_Data;
    
    //
    // initialize Bombardment state
    //

    if( !m_bInitialized )
    {
        m_bInitialized     = TRUE;
        m_bTurret          = FALSE;
        m_bDeathmatch      = FALSE;
        m_bHitSwitch       = FALSE;
        m_bGetEnemies      = FALSE;
        m_bGetSwitchBack   = FALSE;
        m_bSwitchLost      = FALSE;
        m_bTurretsFriendly = FALSE;
        m_TargetTimer      = 0.0f;

        for( s32 i=0; i<5; i++ )
        {
            if( m_bTurretsDestroyed[i] == FALSE )
                AddWaypoint( m_Turrets[i] );
        }

        GamePrompt( M08_DESTROY_TURRET );
    }

    //
    // Randomly choose turret for heavies to mortar every few seconds
    //

    if( (m_TargetTimer > D.TARGET_TIME) && (m_bTurretsFriendly == FALSE) )
    {
        m_TargetTimer = 0;
        s32 i;
        s32 n = x_rand() % 5;
        object::id Target;
        
        for( i=0; i<5; i++ )
        {
            Target = m_Turrets[ (n + i) % 5];
            object* pObject = ObjMgr.GetObjectFromID( Target );
            
            // Make sure turret is not destroyed already
            if( pObject->GetHealth() > 0.0f )
            {
                // Only fire on enemy turrets
                if( pObject->GetTeamBits() & ENEMY_TEAM_BITS )
                    break;
                else
                    m_bTurretsFriendly = TRUE;
            }
        }

        // Keep mortarting turrets until all are destroyed
        if( i < 5 )
            OrderAllies( TASK_MORTAR, Target );
    }

    //
    // wait until the first allied attack then make the enemy retaliate
    //
    
    if( !m_bTurret && (m_nTurretsDestroyed >= 1) && (audio_IsPlaying( m_ChannelID ) == FALSE) )
    {
        m_bTurret = TRUE;
        AddWaypoint( m_Switch );
        
        OrderEnemies( TASK_DEATHMATCH, ATTACK_WAVE_2 );
        GamePrompt( M08_HIT_SWITCH );
    }

    //
    // if all turrets are destroyed OR become friendly, then order the allies to move in
    //
    
    if( (!m_bDeathmatch && (m_nTurretsDestroyed >= 5)) || (m_bTurretsFriendly == TRUE) )
    {
        m_bDeathmatch = TRUE;
        OrderAllies( TASK_DEATHMATCH );
    }

    //
    // check for allied heavy being killed
    //
    
    if( m_bAlliedDied )
    {
        m_bAlliedDied = FALSE;
        //GamePrompt( M08_HEAVY_LOST );   // Commented out: this audio is cancelling out audio you need to hear 
        ErrorText( TEXT_M07_HEAVY_DESTROYED );
    }
    
    //
    // check for all enemies dead and switch not hit
    //
    
    //if( !m_bHitSwitch && ( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ENEMY ))
    //{
    //    m_bHitSwitch = TRUE;
    //    
    //    [need new audio sample for this to work!]
    //    GamePrompt( M08_HIT_SWITCH );
    //}

    //
    // check for switch hit but enemies still remaining
    //

    if( !m_bGetEnemies && ( m_nEnemies > 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        m_bGetEnemies = TRUE;
        
        GamePrompt( M08_MOP_UP );
    }

    //
    // check for switch being taken back by enemy
    //
    
    if( !m_bGetSwitchBack && m_bSwitchLost )
    {
        m_bGetSwitchBack = TRUE;
        
        GamePrompt( M08_GET_SWITCH );
    }
    
    //
    // check for mission success
    //
    
    if(( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        Success( M08_SUCCESS );
    }
}

//========================================================================================================================================================

