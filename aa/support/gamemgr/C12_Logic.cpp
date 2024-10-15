//========================================================================================================================================================
//
//  Campaign 12 by JP
//
//========================================================================================================================================================

#include "c12_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c12_data
{
    s32     STARTPOS_RADIUS;
    s32     ENEMYPOS_RADIUS;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c12_data  C12_Data = { 100, 200 };

c12_logic C12_Logic;

//========================================================================================================================================================

void c12_logic::BeginGame( void )
{
    ASSERT( m_StartPosition != obj_mgr::NullID );
    ASSERT( m_EnemyPosition != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MISSION );
}

//========================================================================================================================================================

void c12_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c12_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE      : return;
        case STATE_MISSION   : Mission(); break;
        default              :            break;
    }

    if( m_bPlayerDied == TRUE )
    {
        //Failed( SFX_VOICE_MISSION_NEW_12_112 );
    }

    m_Time += DeltaTime;
}

//========================================================================================================================================================

void c12_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "STARTPOS" )) m_StartPosition = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"   )) m_EnemyPosition = ItemID;
}

//========================================================================================================================================================

void c12_logic::Mission( void )
{
    c12_data& D = C12_Data;

    //
    // initialize Mission state
    //

    if( !m_bInitialized )
    {
        m_bInitialized       = TRUE;
        m_bAlliesStarted     = FALSE;
        m_bAlliesWithinRange = FALSE;
        m_bPlayerWithinRange = FALSE;
    
        AddWaypoint( m_EnemyPosition );
        
        SpawnAllies();
        SpawnEnemies();
        
        OrderAllies(  TASK_PATROL, m_StartPosition );
        OrderEnemies( TASK_PATROL, m_EnemyPosition );
        
        //GamePrompt( SFX_VOICE_MISSION_NEW_12_107 );
    }

    //
    // check if player leaves start point
    //
    
    if( !m_bAlliesStarted && DistanceToObject( m_StartPosition ) > D.STARTPOS_RADIUS )
    {
        m_bAlliesStarted = TRUE;
    
        OrderAllies( TASK_GOTO, m_EnemyPosition );
        
        ////GamePrompt( SFX_VOICE_MISSION_NEW_12_ );
    }

    //
    // check if the allied bots are within range of enemy and the player is not
    //
    
    if( !m_bAlliesWithinRange && DistanceToClosestPlayer( m_EnemyPosition, TEAM_ALLIES ) < D.ENEMYPOS_RADIUS )
    {
        if( DistanceToObject( m_EnemyPosition ) > D.ENEMYPOS_RADIUS )
        {
            // skip this message if player has already reached the enemy position
        
            if( !m_bPlayerWithinRange )
            {
                m_bAlliesWithinRange = TRUE;

                OrderEnemies( TASK_DEATHMATCH );
                OrderAllies ( TASK_DEATHMATCH );
        
                //GamePrompt( SFX_VOICE_MISSION_NEW_12_108 );
            }
        }
    }

    //
    // check for allied players being killed
    //
    
    if( m_bAlliedDied )
    {
        m_bAlliedDied = FALSE;
        
//        switch( m_DeadArmorType )
//        {
//            case player_object::ARMOR_TYPE_LIGHT  : GamePrompt( SFX_VOICE_MISSION_NEW_12_110 ); break;
//            case player_object::ARMOR_TYPE_MEDIUM : GamePrompt( SFX_VOICE_MISSION_NEW_12_111 ); break;
//            case player_object::ARMOR_TYPE_HEAVY  : GamePrompt( SFX_VOICE_MISSION_NEW_08_060 ); break;
//        }
    }

    //
    // check if player is within range of enemy
    //

    if( !m_bPlayerWithinRange && DistanceToObject( m_EnemyPosition ) < D.ENEMYPOS_RADIUS )
    {
        m_bPlayerWithinRange = TRUE;

        OrderEnemies( TASK_DEATHMATCH );
        OrderAllies ( TASK_DEATHMATCH );

        ClearWaypoint( m_EnemyPosition );
        
        ////GamePrompt( SFX_VOICE_MISSION_NEW_12_109 );    // this speech doesnt really work... 
    }

    //
    // check for mission success
    //
    
    if( m_nEnemies == 0 )
    {
        //Success( SFX_VOICE_MISSION_NEW_12_113 );
    }
}

//========================================================================================================================================================

