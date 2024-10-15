//========================================================================================================================================================
//
//  Campaign 07 by JP
//
//========================================================================================================================================================

#include "c07_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

#define M07_MISSION_FAILED      M08_SCRUBBED
#define M07_MOP_UP              M08_MOP_UP

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c07_data
{
    s32     ENEMY_BASE_RADIUS;
    s32     MOVING_IN_INFO_TIME;
    s32     MOVING_IN_WARN_TIME;
    s32     MOVING_IN_FAIL_TIME;
    s32     GENERATOR_ROOM_RADIUS;
    s32     GENERATOR_ROOM_TIME;
    s32     MIN_NUM_ALLIED_BOTS;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c07_data  C07_Data = { 200, 60, 120, 240, 20, 120, 3 };

c07_logic C07_Logic;

//========================================================================================================================================================

void c07_logic::BeginGame( void )
{
    ASSERT( m_EnemyBase     != obj_mgr::NullID );
    ASSERT( m_GeneratorRoom != obj_mgr::NullID );
    ASSERT( m_Generator1    != obj_mgr::NullID );
    ASSERT( m_Generator2    != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MOVING_IN );
    SetCampaignObjectives( 27 );
}

//========================================================================================================================================================

void c07_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c07_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE         : return;
        case STATE_MOVING_IN    : MovingIn();     break;
        case STATE_INFILTRATION : Infiltration(); break;
        default                 :                 break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M07_MISSION_FAILED );
    }

    m_Time += DeltaTime;
}

//========================================================================================================================================================

void c07_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "WAYPT1" )) m_EnemyBase     = ItemID;
    if( !x_strcmp( pTag, "WAYPT2" )) m_GeneratorRoom = ItemID;
    if( !x_strcmp( pTag, "GEN1"   )) m_Generator1    = ItemID;
    if( !x_strcmp( pTag, "GEN2"   )) m_Generator2    = ItemID;
}

//========================================================================================================================================================

void c07_logic::ItemDisabled( object::id ItemID,  object::id OriginID )
{
    (void)OriginID;

    //
    // check if one of our generators has been disabled
    //
    
    if( ItemID == m_Generator1 )
    {
        m_bGenerator1Disabled = TRUE;
    
        //OrderEnemies( TASK_REPAIR, m_Generator1 );
    }
    
    if( ItemID == m_Generator2 )
    {
        m_bGenerator2Disabled = TRUE;
        
        //OrderEnemies( TASK_REPAIR, m_Generator2 );
    }

    m_bGeneratorsDisabled = m_bGenerator1Disabled & m_bGenerator2Disabled;
}

//========================================================================================================================================================

void c07_logic::ItemRepaired( object::id ItemID,  object::id OriginID, xbool Score )
{
    (void)OriginID;
    (void)Score;

    //
    // check if one of our generators has been repaired
    //
    
    if( ItemID == m_Generator1 )
    {
        m_bGenerator1Disabled = FALSE;
        
        if( m_bGenerator2Disabled )
        {
            //OrderEnemies( TASK_REPAIR, m_Generator2 );
        }
    }
    
    if( ItemID == m_Generator2 )
    {
        m_bGenerator2Disabled = FALSE;
        
        if( m_bGenerator1Disabled )
        {
            //OrderEnemies( TASK_REPAIR, m_Generator1 );
        }
    }
}

//========================================================================================================================================================

void c07_logic::MovingIn( void )
{
    c07_data& D = C07_Data;

    //
    // initialize Moving In state
    //

    if( !m_bInitialized )
    {
        m_bInitialized     = TRUE;
        m_bMovingInInfo    = FALSE;
        m_bMovingInWarning = FALSE;

        SpawnEnemies( ATTACK_WAVE_1 );
        SpawnEnemies( ATTACK_WAVE_2 );
        
        OrderEnemies( TASK_PATROL, m_EnemyBase, ATTACK_WAVE_1 );
        OrderEnemies( TASK_ANTAGONIZE,          ATTACK_WAVE_2 );

        AddWaypoint( m_GeneratorRoom );
        GamePrompt( M07_INTRO );
    }

    //
    // check if its time to give the player a little hint
    //

    if( !m_bMovingInInfo && m_Time > D.MOVING_IN_INFO_TIME )
    {
        m_bMovingInInfo = TRUE;
        GamePrompt( M07_BASE_WAYPOINT );
    }

    //
    // ensure player gets to enemy base in the allotted time limit
    //    

    if( DistanceToObject( m_EnemyBase ) < D.ENEMY_BASE_RADIUS )
    {
        SetState( STATE_INFILTRATION );
    }
    else
    {
        if( m_Time > D.MOVING_IN_FAIL_TIME )
        {
            Failed( M07_SCRUBBED );
        }
        else
        {
            if( !m_bMovingInWarning && m_Time > D.MOVING_IN_WARN_TIME )
            {
                m_bMovingInWarning = TRUE;
                GamePrompt( M07_GET_MOVING );
            }
        }
    }
}

//========================================================================================================================================================

void c07_logic::Infiltration( void )
{
    c07_data& D = C07_Data;

    //
    // initialize Infiltration state
    //

    if( !m_bInitialized )
    {
        m_bInitialized         = TRUE;
        m_bInfiltrationWarning = FALSE;
        m_bGeneratorsDisabled  = FALSE;
        m_bGenerator1Disabled  = FALSE;
        m_bGenerator2Disabled  = FALSE;
        m_bMopUp               = FALSE;
        
        SpawnAllies();
        OrderAllies( TASK_DEATHMATCH );
        
        GamePrompt( M07_BASE_AHEAD );
    }

    //
    // ensure player is within radius of generator room within the allotted time
    //

    if( !m_bInfiltrationWarning && !m_bGeneratorsDisabled && DistanceToObject( m_GeneratorRoom ) > D.GENERATOR_ROOM_RADIUS )
    {
        if( m_Time > D.GENERATOR_ROOM_TIME )
        {
            m_bInfiltrationWarning = TRUE;
        
            GamePrompt( M07_DESTROY_GEN );
        }
    }
    
    //
    // once generators are destroyed, instruct the player to kill remaining derms
    //
    
    if( !m_bMopUp && m_bGeneratorsDisabled )
    {
        m_bMopUp = TRUE;
        GamePrompt( M07_MOP_UP );
    }

    //
    // check for allies being killed
    //
    
    if( m_bAlliedDied )
    {
        m_bAlliedDied = FALSE;
        //GamePrompt( M07_HEAVY_DESTROYED );
        ErrorText( TEXT_M07_HEAVY_DESTROYED );
    }
    
    //
    // success and fail states
    //
    
    if( m_nAllies < D.MIN_NUM_ALLIED_BOTS )
    {
        Failed( M07_PULLING_BACK );
    }
    else
    {
        if( m_nEnemies == 0 )
        {
            Success( M07_SUCCESS );
        }
    }
}

//========================================================================================================================================================
