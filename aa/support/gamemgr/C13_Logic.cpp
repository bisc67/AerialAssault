//========================================================================================================================================================
//
//  Campaign 13 by JP
//
//========================================================================================================================================================

#include "c13_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

#define M13_GET_SWITCH      M10_GET_SWITCH
#define M13_MOP_UP          M08_MOP_UP

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c13_data
{
    s32     ALLIED_BOT_RADIUS;
    s32     MOVING_OUT_TIME;
    s32     ENEMY_BASE_RADIUS;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c13_data  C13_Data = { 300, 15, 200 };

c13_logic C13_Logic;

//========================================================================================================================================================

void c13_logic::BeginGame( void )
{
    ASSERT( m_StartPos  != obj_mgr::NullID );
    ASSERT( m_EnemyBase != obj_mgr::NullID );
    ASSERT( m_Switch    != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MOVING_OUT );
    SetCampaignObjectives( 31 );
}

//========================================================================================================================================================

void c13_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c13_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE       : return;
        case STATE_MOVING_OUT : MovingOut(); break;
        case STATE_BATTLE     : Battle();    break;
        default               :              break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M13_BYE );
    }

    m_Time += DeltaTime;
}

//========================================================================================================================================================

void c13_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "STARTPOS" )) m_StartPos  = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"   )) m_EnemyBase = ItemID;
    if( !x_strcmp( pTag, "SWITCH1"  )) m_Switch    = ItemID;
}

//========================================================================================================================================================

void c13_logic::MovingOut( void )
{
    c13_data& D = C13_Data;

    //
    // initialize Moving Out state
    //

    if( !m_bInitialized )
    {
        m_bInitialized       = TRUE;
        m_bAlliesStarted     = FALSE;
        m_bMovingOutWarning1 = FALSE;
        m_bMovingOutWarning2 = FALSE;

        AddWaypoint( m_EnemyBase );

        SpawnAllies();
        SpawnEnemies();
        
        OrderAllies ( TASK_GOTO, m_EnemyBase );
        OrderEnemies( TASK_ANTAGONIZE );
        
        GamePrompt( M13_INTRO );
    }

    //
    // ensure player stays with his squad
    //

    if( m_nAllies > 0 )
    {
        if( DistanceToClosestPlayer( m_PlayerID, TEAM_ALLIES ) > D.ALLIED_BOT_RADIUS )
        {
            if( !m_bMovingOutWarning1 )
            {
                m_bMovingOutWarning1 = TRUE;
                m_Time               = 0;
                
                GamePrompt( M13_STAY_TOGETHER );
            }
            
            if( !m_bMovingOutWarning2 && (m_Time > D.MOVING_OUT_TIME) )
            {
                m_bMovingOutWarning2 = TRUE;
                m_Time               = 0;
                
                GamePrompt( M13_GO_TO_UNIT );
            }
            
            if( m_Time > D.MOVING_OUT_TIME )
            {
                Failed( M13_FAIL );
            }
        }
        else
        {
            m_Time = 0;
        }
    }

    //
    // check if allies reach the destination
    //

    if( DistanceToObject( m_EnemyBase ) < D.ENEMY_BASE_RADIUS )
    {
        ClearWaypoint( m_EnemyBase );
        SetState( STATE_BATTLE );
    }
}

//========================================================================================================================================================

void c13_logic::Battle( void )
{
    //
    // initialize Battle state
    //

    if( !m_bInitialized )
    {
        m_bInitialized   = TRUE;
        m_bHitSwitch     = FALSE;
        m_bGetEnemies    = FALSE;
        m_bGetSwitchBack = FALSE;
        m_bSwitchLost    = FALSE;
        m_SwitchState    = TEAM_ENEMY;

        AddWaypoint( m_Switch );

        OrderEnemies( TASK_DEATHMATCH );
        OrderAllies ( TASK_DEATHMATCH );
        
        GamePrompt( M13_KICK_ASS );
    }

    //
    // check for all enemies dead and switch not hit
    //
    
    if( !m_bHitSwitch && ( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ENEMY ))
    {
        m_bHitSwitch = TRUE;
        
        if( m_nAllies )
            OrderAllies( TASK_PATROL, m_EnemyBase );
        
        GamePrompt( M13_HIT_SWITCH );
    }

    //
    // check for switch hit but enemies still remaining
    //

    if( !m_bGetEnemies && ( m_nEnemies > 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        m_bGetEnemies = TRUE;
        
        GamePrompt( M13_MOP_UP );
    }

    //
    // check for switch being taken back by enemy
    //
    
    if( !m_bGetSwitchBack && m_bSwitchLost )
    {
        m_bGetSwitchBack = TRUE;
        
        GamePrompt( M13_GET_SWITCH );
    }
    
    //
    // check for mission success
    //
    
    if(( m_nEnemies == 0 ) && ( m_SwitchState == TEAM_ALLIES ))
    {
        m_Time = 0;
        Success( M13_SUCCESS );
    }
}

//========================================================================================================================================================

