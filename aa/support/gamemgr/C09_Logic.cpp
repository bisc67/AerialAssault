//========================================================================================================================================================
//
//  Campaign 09 by JP
//
//========================================================================================================================================================

#include "c09_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c09_data
{
    s32     STARTPOS_RADIUS;
    s32     DESTINATION_RADIUS;
    s32     DEPLOY_WARN_TIME;
    s32     DEPLOY_FAIL_TIME;
    s32     ATTACK_TIME;
    s32     ATTACK_RADIUS;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c09_data  C09_Data = { 20, 50, 120, 180, 30, 200 };

c09_logic C09_Logic;

//========================================================================================================================================================

void c09_logic::BeginGame( void )
{
    ASSERT( m_StartPos    != obj_mgr::NullID );
    ASSERT( m_Destination != obj_mgr::NullID );
    
    campaign_logic::BeginGame();

    SetState( STATE_DEPLOY );
}

//========================================================================================================================================================

void c09_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c09_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE   : return;
        case STATE_DEPLOY : Deploy(); break;
        case STATE_DEFEND : Defend(); break;
        default           :           break;
    }

    if( m_bPlayerDied == TRUE )
    {
        //Failed( SFX_VOICE_MISSION_NEW_09_070 );
    }

    m_Time += DeltaTime;
}

//========================================================================================================================================================

void c09_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "STARTPOS" )) m_StartPos    = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"   )) m_Destination = ItemID;
}

//========================================================================================================================================================

void c09_logic::ItemDestroyed( object::id ItemID, object::id OriginID )
{
    (void)OriginID;
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );

    //
    // check if the MPB has blown up
    //

    if( pObject->GetType() == object::TYPE_VEHICLE_JERICHO2 )
    {
        m_bMPBDestroyed = TRUE;
    }
}

//========================================================================================================================================================

void c09_logic::ItemDeployed( object::id ItemID, object::id OriginID )
{
    (void)OriginID;
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );
    
    c09_data& D = C09_Data;

    //
    // check for the MPB being deployed in the correct location
    //
    
    if( pObject->GetType() == object::TYPE_VEHICLE_JERICHO2 )
    {
        if( DistanceToObject( ItemID, m_Destination ) < D.DESTINATION_RADIUS )
        {
            m_bMPBDeployed = TRUE;
        }
        else
        {
            if( !m_bDeployWarning )
            {
                m_bDeployWarning = TRUE;

                //TODO: correct audio cue!
                x_DebugMsg( "MPB deployed in wrong location... deploy it at the waypoint shown on your display\n" );
                //GamePrompt( SFX_VOICE_MISSION_NEW_09_ );
            }
        }
    }
}

//========================================================================================================================================================

void c09_logic::Deploy( void )
{
    c09_data& D = C09_Data;
    
    //
    // initialize Deploy state
    //

    if( !m_bInitialized )
    {
        m_bInitialized   = TRUE;
        m_bMPBDeployed   = FALSE;
        m_bMPBDestroyed  = FALSE;
        m_bTimeWarning   = FALSE;
        m_bDeployWarning = FALSE;
        m_bParkMPB       = FALSE;

        AddWaypoint( m_Destination );

        //GamePrompt( SFX_VOICE_MISSION_NEW_09_068 );
    }

    //
    // check for MPB being destroyed
    //
    
    if( m_bMPBDestroyed )
    {
        //Failed( SFX_VOICE_MISSION_NEW_09_075 );
    }

    //
    // check for MPB reaching destination waypoint
    //
    
    if( !m_bParkMPB && m_bPlayerOnVehicle )
    {
        if( DistanceToObject( m_VehicleID, m_Destination ) < D.DESTINATION_RADIUS )
        {
            m_bParkMPB = TRUE;
            
            //GamePrompt( SFX_VOICE_MISSION_NEW_09_071 );
        }
    }

    //
    // check for MPB being deployed correctly within the time limit
    //

    if( !m_bMPBDeployed )
    {
        if( m_Time > D.DEPLOY_FAIL_TIME )
        {
            //Failed( SFX_VOICE_MISSION_NEW_09_070 );
        }
        else
        {
            if( !m_bTimeWarning && ( m_Time > D.DEPLOY_WARN_TIME ))
            {
                m_bTimeWarning = TRUE;
                
                //GamePrompt( SFX_VOICE_MISSION_NEW_09_069 );
            }
        }
    }
    else
    {
        ClearWaypoint( m_Destination );
        SetState( STATE_DEFEND );
    }
}

//========================================================================================================================================================

void c09_logic::Defend( void )
{
    c09_data& D = C09_Data;
    
    //
    // initialize Defend state
    //

    if( !m_bInitialized )
    {
        m_bInitialized  = TRUE;
        m_bEnemyWarning = FALSE;
        m_bEnemyAttack  = FALSE;

        //GamePrompt( SFX_VOICE_MISSION_NEW_09_072 );
    }

    //
    // warn the player about enemy on its way
    //

    if( !m_bEnemyWarning && ( m_Time > D.ATTACK_TIME ))
    {
        m_bEnemyWarning = TRUE;
    
        SpawnEnemies();
        OrderEnemies( TASK_DESTROY, m_VehicleID );
        
        //GamePrompt( SFX_VOICE_MISSION_NEW_09_073 );
    }

    //
    // check for enemy near MPB
    //

    if( !m_bEnemyAttack && ( DistanceToClosestPlayer( m_VehicleID, TEAM_ENEMY ) < D.ATTACK_RADIUS ))
    {
        m_bEnemyAttack = TRUE;
    
        //GamePrompt( SFX_VOICE_MISSION_NEW_09_074 );
    }

    //
    // check for mission fail and success
    //

    if( m_bMPBDestroyed )
    {
        //Failed( SFX_VOICE_MISSION_NEW_09_075 );
    }
    
    if( m_bEnemyAttack && ( m_nEnemies == 0 ))
    {
        //Success( SFX_VOICE_MISSION_NEW_09_077 );
    }
}

//========================================================================================================================================================

