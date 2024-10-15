//==============================================================================
//
// BotStrategy.cpp
//
// Contains methods for the bot_object class relating to "goals"
//==============================================================================

#include "BotObject.hpp"
#include "BotLog.hpp"
#include "Support\GameMgr\GameMgr.hpp"
#include "Graph.hpp" // For EDGE_COST_ANALYSIS define...

//==============================================================================
//  TYPES
//==============================================================================

//==============================================================================
//  DEFINES
//==============================================================================
#define TIMING_UPDATE_GOAL 0

//#define ENABLE_PROFILE

//==============================================================================
//  STORAGE
//==============================================================================
static const f32 PICKUP_INTEREST_RADIUS = 50.0f;
static const f32 PICKUP_INTEREST_RADIUS_SQ = x_sqr( PICKUP_INTEREST_RADIUS );
static const f32 CLOSE_ENOUGH = 0.75f;
static const f32 CLOSE_ENOUGH_SQ = x_sqr( CLOSE_ENOUGH );
static const f32 ROAM_INDOOR_RADIUS = 15.0f;
static const f32 ROAM_OUTDOOR_RADIUS = 200.0f;

extern graph g_Graph;
extern xbool g_BotFrameCount;
extern bot_object::bot_debug g_BotDebug;

#if TESTING_VANTAGE_POINTS
graph::vantage_point g_pVantagePoints[1000];
s32 g_nVantagePoints = 0;
#endif

#if EDGE_COST_ANALYSIS
    extern xbool g_bCostAnalysisOn;
#endif
//==============================================================================
//  FUNCTIONS
//==============================================================================

//==============================================================================
// UpdateGoal()
//
// Bread and butter of goal decision making
//==============================================================================
#ifdef ENABLE_PROFILE
#include "libsn.h"
s32 SampleRate = 40000;
#endif

void bot_object::UpdateGoal( void )
{
#ifdef ENABLE_PROFILE
    snProfSetInterval( 300000000 / SampleRate );
    snProfEnableInt() ;
#endif
    if ( m_ElapsedTimeSinceLastGoalUpdate
        >= m_CurrentGoal.GetUpdateTimeIncrement()
        || m_RequireGoalUpdateNextFrame )
    {
#if TIMING_UPDATE_GOAL
        xtimer Timer;
        Timer.Start();
#endif
        
        m_ElapsedTimeSinceLastGoalUpdate = 0.0f;
        m_RequireGoalUpdateNextFrame = FALSE;

        if ( !m_CreatedByEditor
            && (   g_BotDebug.TestMortar
                || g_BotDebug.TestRepair
                || g_BotDebug.TestDestroy
                || g_BotDebug.TestEngage
                || g_BotDebug.TestAntagonize
                || g_BotDebug.TestDeploy
                || g_BotDebug.TestSnipe
                || g_BotDebug.TestRoam
                || g_BotDebug.TestLoadout) )
        {
            TestUpdate();
            goto end;
        }
        
        xbool InitTask = FALSE;

        if ( m_CurTask )
        {
            InitTask
                = (m_CurTask != m_PrevTask)
                || (m_CurTask->GetID() != m_LastTaskID)
                || (m_CurTask->GetState() != m_LastTaskState)
                || (m_CurrentGoal.GetGoalID() == bot_goal::ID_NO_GOAL)
                || (m_CurTask->GetTargetObject()
                    && m_CurrentGoal.GetTargetObject()
                    && (m_CurTask->GetTargetObject()->GetObjectID()
                        != m_CurrentGoal.GetTargetObject()->GetObjectID()))
                || ( m_CurTask->GetTargetObject() == NULL
                    && m_CurrentGoal.GetTargetObject() != NULL)
                || ( m_CurTask->GetTargetObject() != NULL
                    && m_CurrentGoal.GetTargetObject() == NULL)
                ;

#if 0 // let's you see why we're initializing tasks
            if ( InitTask && m_CurTask )
            {
                x_DebugMsg( "%2d changing because ", m_Num );
                
                if (m_CurTask != m_PrevTask) x_DebugMsg( "task changed " );
                if (m_CurTask->GetID() != m_LastTaskID) x_DebugMsg( "ID changed " );
                if (m_CurTask->GetState() != m_LastTaskState) x_DebugMsg( "state changed " );
                if (m_CurrentGoal.GetGoalID() == bot_goal::ID_NO_GOAL) x_DebugMsg( "had no goal " );
                if ( m_CurTask->GetTargetObject() && 
                    m_CurrentGoal.GetTargetObject() &&
                    (m_CurTask->GetTargetObject()->GetObjectID()
                        != m_CurrentGoal.GetTargetObject()->GetObjectID()) ) x_DebugMsg( "tgt obj changed " );
                if ( m_CurTask->GetTargetObject() == NULL &&
                    m_CurrentGoal.GetTargetObject() != NULL) x_DebugMsg( "had no tgt obj " );
                if ( m_CurTask->GetTargetObject() != NULL &&
                    m_CurrentGoal.GetTargetObject() == NULL) x_DebugMsg( "now have tgt obj " );

                x_DebugMsg( "-- frame %d\n", g_BotFrameCount );
            }
#endif
                
            
        }

        if ( InitTask && !m_CreatedByEditor )
        {
            if ( m_CurTask != NULL )
            {
                // We've been assigned a new task
                InitializeCurrentTask();
            }
            else
            {
                m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            }
        }

        //--------------------------------------------------
        // see if we should grab a pickup
        //--------------------------------------------------
        if ( m_CurTask != NULL
            && !m_CurTask->IsImportant()
            && m_CurrentGoal.GetGoalID() != bot_goal::ID_PICKUP )
        {
            object* pPickup = GetDesiredPickup();

            if ( pPickup )
            {
                InitializePickup( *pPickup );
            }
        }

        //--------------------------------------------------
        // Check to see if we are done getting a pickup
        //--------------------------------------------------
        if ( m_CurrentGoal.GetGoalID() == bot_goal::ID_PICKUP
            && HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) )
        {
            // we've arrived at the pickup, shall we get another?
            object* pPickup = NULL;
            pPickup = GetDesiredPickup();

            if ( pPickup )
            {
                InitializePickup( *pPickup );
            }
            else
            {
                if (m_CurTask)
                {
                    // ok, move on to new task
                    InitializeCurrentTask();
                }
            }
        }

        //--------------------------------------------------
        // Check to see if we need to go to an inventory
        // station
        //--------------------------------------------------
        const xbool NeedLoadout = !HaveIdealLoadout();

        if ( NeedLoadout 
            && (m_PlayerState == PLAYER_STATE_AT_INVENT) )
        {
            if ( !m_InventTimer.IsRunning() )
            {
                // start the clock ticking
                m_InventTimer.Reset();
                m_InventTimer.Start();
            }
        }

        if (NeedLoadout && m_PlayerLastState == player_object::PLAYER_STATE_AT_INVENT)
        {
            //######################################################
            // TO DO:
            // Need to check if we are already on the inven station!
            // (task may have changed while stepping into inven)
            //######################################################

            // Step off inven!  Move it!
        }

        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_LOADOUT
            && m_CurrentGoal.GetGoalID() != bot_goal::ID_PICKUP
            && NeedLoadout
            && !m_CreatedByEditor)
        {
            // are we near an inventory station?
            station NearestInven;
            xbool GetLoadout;
            const f32 DistSQ= GetNearestInven( NearestInven
                    , (!NeedArmor() && !NeedInvenPack()) );

            if ( DistSQ != -1
                && m_CurTask
                && (   m_CurTask->GetID() == bot_task::TASK_TYPE_REPAIR
                    || m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_TURRET
                    || m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_SENSOR
                    || m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_INVEN
                    || m_CurTask->GetID() == bot_task::TASK_TYPE_DESTROY) )
            {
                GetLoadout = TRUE;
            }
            else
            {
                GetLoadout = (DistSQ >= 0.0f) && (DistSQ < x_sqr( 50.0f ));
            }
            
            if ( GetLoadout )
            {
                // Ok, get loadout
                InitializeLoadout( NearestInven );
            }
        }
        //--------------------------------------------------
        // Check to see if we formerly needed a loadout, but
        // have it now
        //--------------------------------------------------
        if ( m_CurrentGoal.GetGoalID() == bot_goal::ID_LOADOUT
            && !NeedLoadout
            && m_CurTask )
        {
#ifdef mreed
            // Make sure we have a pack -- we should never come out of an 
            // inven w/out a pack
            s32 i;
            xbool HavePack = FALSE;

            for ( i = INVENT_TYPE_PACK_START; i <= INVENT_TYPE_PACK_END; ++i )
            {
                if ( m_Loadout.Inventory.Counts[i] > 0 )
                {
                    HavePack = TRUE;
                    break;
                }
            }
     
            //ASSERT( HavePack );

            if ( !HavePack )
            {
                HaveIdealLoadout();
            }
#endif
            // stop loadout, move on
            InitializeCurrentTask();
        }

        //--------------------------------------------------
        // keep the pursuit current as needed
        //--------------------------------------------------
        if ( (m_CurTask != NULL) &&
            (m_CurTask->GetState() == bot_task::TASK_STATE_PURSUE ) &&
            (m_CurrentGoal.GetGoalID() != bot_goal::ID_LOADOUT) )
        {
            m_CurrentGoal.SetTargetLocation( m_CurTask->GetTargetPos() );
        }

        //--------------------------------------------------
        // Update our goals
        //--------------------------------------------------
        switch ( m_CurrentGoal.GetGoalID() ) 
        {
        case bot_goal::ID_NO_GOAL:
        {
            break;
        }
        case bot_goal::ID_PURSUE_OBJECT:
        {
            break;
        }
        case bot_goal::ID_GO_TO_LOCATION:
        {
            if ( HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) )
            {
#if EDGE_COST_ANALYSIS
                if (g_bCostAnalysisOn)
                    m_PathKeeper.AnalyzeCost();
#endif
            }
        
            break;
        }
        case bot_goal::ID_PATROL:
        {
            //--------------------------------------------------
            // See if we need to pick another patrol point
            //--------------------------------------------------
            if ( AmNear( m_CurrentGoal.GetTargetLocation() ) )
            {
                InitializeGoTo( PickNextPatrolPoint() );

                // Set the goal back to patrol
                m_CurrentGoal.SetGoalID( bot_goal::ID_PATROL );
            }
        
            break;
        }

        case bot_goal::ID_ROAM:
        {
            const f32 Radius = InsideBuilding( m_CurrentGoal.GetTargetLocation() )
                ? ROAM_INDOOR_RADIUS
                : ROAM_OUTDOOR_RADIUS;

            if ( m_CurTask 
                && (m_CurTask->GetTargetPos() - m_CurrentGoal.GetTargetLocation()).LengthSquared()
                > x_sqr( Radius ) )
            {
                InitializeCurrentTask();
            }
        }
        break;

        case bot_goal::ID_ENGAGE:
        {
            break;
        }

        case bot_goal::ID_DEPLOY:
            // possibly want to change this to check whether an init is needed earlier
            if (m_CurTask && (m_CurTask->GetTargetPos() != m_CurrentGoal.GetTargetLocation()))
                InitializeCurrentTask();
            break;
        
        case bot_goal::ID_SNIPE:
            // possibly want to change this to check whether an init is needed earlier
            if (m_CurTask && (m_CurTask->GetTargetPos() != m_CurrentGoal.GetTargetLocation()))
                InitializeCurrentTask();
            break;
        
        default:
            break;
        }

        //--------------------------------------------------
        // Update task stuff
        //--------------------------------------------------
        if ( m_CurTask != NULL )
        {
            m_PrevTask      = m_CurTask;
            m_LastTaskState = m_CurTask->GetState();
            m_LastTaskID    = m_CurTask->GetID();
        }
        else
        {
            m_LastTaskState = -1;
            m_LastTaskID    = bot_task::TASK_TYPE_NONE;
        }

#if TIMING_UPDATE_GOAL
        x_printf( "UpdateGoal() time: %fms\n", Timer.ReadMs() );
#endif
    }
end:
#ifdef ENABLE_PROFILE
        snProfDisableInt() ;
#endif
    return;
}

//==============================================================================
// InitializePursuit()
// Sets up ID_PURSUE_OBJECT goal
//==============================================================================
void bot_object::InitializePursuit( const object& Object ) 
{
    InitializeGoTo( Object.GetPosition() + vector3( 0, 0.1f, 0 ) );
    m_CurrentGoal.SetGoalID( bot_goal::ID_PURSUE_OBJECT );
    m_CurrentGoal.SetTargetObject( Object.GetObjectID() );
}

//==============================================================================
// InitializePursuit()
// Sets up ID_PURSUE_LOCATION goal
//==============================================================================
void bot_object::InitializePursuit() 
{
    if ( !m_CurTask )
    {
        x_DebugLog( "Bot: m_CurTask NULL in InitializePursuit()" );
        ASSERT( m_CurTask );
        return;
    }

    InitializeGoTo( m_CurTask->GetTargetPos() );
    m_CurrentGoal.SetGoalID( bot_goal::ID_PURSUE_LOCATION );
}

//==============================================================================
// InitializeGoTo()
// Sets up ID_GO_TO_LOCATION goal
//==============================================================================
void bot_object::InitializeGoTo( const vector3& Location
    , xbool Reinit /* = FALSE*/ ) 
{
    if ( g_BotDebug.DrawNavigationMarkers )
    {
        DrawSphere( GetBBox().GetCenter(), 3.0f, XCOLOR_GREEN );
    }
    
    m_CurrentGoal.SetGoalID( bot_goal::ID_GO_TO_LOCATION );
    m_CurrentGoal.SetTargetLocation( Location );

    m_PathKeeper.PlotCourse(
        Reinit ? m_PathKeeper.GetCurrentPoint().Pos : m_WorldPos
        , Location, GetMaxAir() );
    
    m_PrevTargetLocation = m_WorldPos;
    m_ClosestDistToNextPointSQ = F32_MAX;
    m_DistToNextPointTimer.Reset();
    m_DistToNextPointTimer.Start();
    m_JettedWhileStuck = FALSE;
}

//==============================================================================
// InitializePatrol()
// Sets up ID_PATROL goal
//==============================================================================
void bot_object::InitializePatrol( void ) 
{
    //----------------------------------------------------------------------
    // First off, let's fake like we're goto-ing a point, then
    // change our goal to ID_PATROl
    //----------------------------------------------------------------------

    InitializeGoTo( PickNextPatrolPoint() );

    // Set the goal to patrol so the next point will be picked
    // automatically
    m_CurrentGoal.SetGoalID( bot_goal::ID_PATROL );
}

//==============================================================================
// InitializeEngage()
// Sets up ID_ENGAGE goal
//==============================================================================
void bot_object::InitializeEngage( player_object& Enemy ) 
{
    InitializePursuit( Enemy );

    m_CurrentGoal.SetGoalID( bot_goal::ID_ENGAGE );
    m_CurrentGoal.SetGoalState( bot_goal::STATE_ENGAGE_GOTO );

    m_EngageSuperiorityHeight = x_frand( 10.0f, 30.0f );
}

//==============================================================================
// InitializeMortar()
// Sets up ID_MORTAR goal
//==============================================================================
void bot_object::InitializeMortar( const object& Target ) 
{
    InitializePursuit( Target );

    m_CurrentGoal.SetTargetObject( Target.GetObjectID() );
    m_CurrentGoal.SetGoalID( bot_goal::ID_MORTAR );
}

//==============================================================================
// InitializeRepair()
// Sets up ID_REPAIR goal
//==============================================================================
void bot_object::InitializeRepair( const object& Target ) 
{
    m_CurrentGoal.SetTargetObject( Target.GetObjectID() );

    m_NeedRepairPack 
        = (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_REPAIR] == 0);

    m_GoingToInven = FALSE;
    
    if ( m_NeedRepairPack )
    {
        // We need to get a repair pack. See which is closer, a repair pack
        // or an inventory station.
        station Station;
        vector3 NearestInvenPos;
        f32 InvenDistSQ = GetNearestInven( Station, !NeedArmor() );
        if (InvenDistSQ >= 0.0f)
        {
            NearestInvenPos = Station.GetPosition();
            InvenDistSQ += (NearestInvenPos -
                m_CurrentGoal.GetTargetObject()->GetPosition()).LengthSquared();
        }

        vector3 NearestRepairPos;
        f32 RepairDistSQ = GetNearestRepairPackPos( NearestRepairPos );
        if (RepairDistSQ >= 0.0f)
        {
            RepairDistSQ += (NearestRepairPos - m_WorldPos).LengthSquared();
        }

        if ( (RepairDistSQ < 0.0f && InvenDistSQ >= 0.0f)
            || InvenDistSQ >= 0.0f && InvenDistSQ < RepairDistSQ )
        {
            // get the loadout
            m_GoingToInven = TRUE;
            InitializeLoadout( Station );
            m_CurrentGoal.SetGoalID( bot_goal::ID_REPAIR );
        }
        else if ( RepairDistSQ >= 0.0f )
        {
            // get the pack
            InitializeGoTo( NearestRepairPos );
        }
        else
        {
            m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            return;
        }
    }
    else
    {
        InitializePursuit( Target );
    }

    m_CurrentGoal.SetGoalID( bot_goal::ID_REPAIR );
}

//==============================================================================
// InitializeDestroy()
// Sets up ID_DESTROY goal
//==============================================================================
void bot_object::InitializeDestroy( const object& Target ) 
{
#if TESTING_VANTAGE_POINTS
    if ( FALSE && g_nVantagePoints == 0 )
    {
        g_nVantagePoints = GetPermanentVantagePoints( g_pVantagePoints, 1000 );
    }
#endif
    
    m_WaitingForDestroyVantagePoint = FALSE;

    if ( Target.GetAttrBits() & (object::ATTR_VEHICLE | object::ATTR_PLAYER) )
    {
        InitializePursuit( Target );
    }
    else
    {
        vector3 Point;
        GetDestroyVantagePoint( Point, Target );

        if ( !m_WaitingForDestroyVantagePoint )
        {
            InitializeGoTo( Point );
        }
        else
        {
            InitializeGoTo( Target.GetPosition() );
        }
    }
    
    m_CurrentGoal.SetTargetObject( Target.GetObjectID() );
    m_CurrentGoal.SetGoalID( bot_goal::ID_DESTROY );
}

//==============================================================================
// InitializeAntagonize()
//==============================================================================
void bot_object::InitializeAntagonize( void ) 
{
    m_AntagonizePos = m_WorldPos;

    m_AntagonizeTimer.Reset();
    m_AntagonizeTimer.Start();

    m_AntagonizeTarget = obj_mgr::NullID;

    m_CurrentGoal.SetGoalID( bot_goal::ID_ANTAGONIZE );

    m_CurrentGoal.SetTargetLocation( m_AntagonizePos );
}

//==============================================================================
// PickNextPatrolPoint()
//==============================================================================
vector3 bot_object:: PickNextPatrolPoint( void ) const
{
    // For now, we're just picking patrol points randomly...
    const s32 NodeIdx = x_irand( 0, g_Graph.m_nNodes-1 );
    const vector3 Dest = g_Graph.m_pNodeList[NodeIdx].Pos;
    return Dest;
}

    
//==============================================================================
// InitializeCurrentTask()
//==============================================================================
xtimer BotInitTask[13];
void bot_object::InitializeCurrentTask( void )
{
    if ( !m_CurTask )
    {
        x_DebugLog( "Bot: m_CurTask NULL in InitializeCurrentTask()" );
        ASSERT( m_CurTask );
        return;
    }

    if ( !m_PrevTask || (m_PrevTask->GetID() != m_CurTask->GetID()) )
    {
        //--------------------------------------------------
        // Play a voice sound, if appropriate
        //--------------------------------------------------
        switch ( m_CurTask->GetID() )
        {
        case bot_task::TASK_TYPE_DEFEND_FLAG:
            Speak( 90 );
            break;
        
        case bot_task::TASK_TYPE_LIGHT_DEFEND_FLAG:
            Speak( 90 );
            break;
        
        case bot_task::TASK_TYPE_RETRIEVE_FLAG:
            Speak( 41 );
            break;
        
        case bot_task::TASK_TYPE_CAPTURE_FLAG:
            Speak( 83 );
            break;
        
        case bot_task::TASK_TYPE_ESCORT_FLAG_CARRIER:
            Speak( 103 );
            break;
        
        case bot_task::TASK_TYPE_DEPLOY_TURRET:
            Speak( 108 );
            break;
        
        case bot_task::TASK_TYPE_DEPLOY_INVEN:
            Speak( 106 );
            break;
        
        case bot_task::TASK_TYPE_DEPLOY_SENSOR:
            Speak( 107 );
            break;
        
        case bot_task::TASK_TYPE_REPAIR:
            Speak( 97 );
            break;
        
        case bot_task::TASK_TYPE_ENGAGE_ENEMY:
            if ( m_PrevTask && Defending() )
            {
                Speak( 131 );
            }
            
            break;
        
        case bot_task::TASK_TYPE_DESTROY:
        {
            const object* TargetObject = m_CurTask->GetTargetObject();

            if ( TargetObject )
            {
                switch ( TargetObject->GetType() )
                {
                case object::TYPE_GENERATOR:
                    Speak( 84 );
                    break;

                case object::TYPE_TURRET_FIXED:
                case object::TYPE_TURRET_CLAMP:
                case object::TYPE_TURRET_SENTRY:
                    Speak( 86 );
                    break;

                case object::TYPE_SENSOR_LARGE:
                case object::TYPE_SENSOR_REMOTE:
                case object::TYPE_SENSOR_MEDIUM:
                    Speak( 85 );
                    break;

                default:
                    Speak( 82 ); 
                }
                
                break;
            }
        }
        
        
        case bot_task::TASK_TYPE_SNIPE:
            Speak( 89 );
            break;

        default:
            ;
        }
    }

BotInitTask[0].Start();
    switch ( m_CurTask->GetState() )
    {
    case bot_task::TASK_STATE_INACTIVE:
        m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
        break;
        
    case bot_task::TASK_STATE_GOTO_POINT:
BotInitTask[1].Start();
        InitializeGoTo( m_CurTask->GetTargetPos() );
BotInitTask[1].Stop();
        break;
        
    case bot_task::TASK_STATE_PURSUE:
BotInitTask[2].Start();
        InitializePursuit();
BotInitTask[2].Stop();
        break;
        
    case bot_task::TASK_STATE_ROAM_AREA:
BotInitTask[3].Start();
        InitializeRoam();
BotInitTask[3].Stop();
        break;
        
    case bot_task::TASK_STATE_ATTACK:
    {
BotInitTask[4].Start();
        player_object* pPlayer = (player_object*)(m_CurTask->GetTargetObject());
        if( pPlayer )   // Target may be dead.
        {
            InitializeEngage( *pPlayer );
        }
        else
        {
            // Target to repair has been destroyed (deployable).
            m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            m_CurTask->ForceReset();
        }
BotInitTask[4].Stop();
        break;
    }

    case bot_task::TASK_STATE_MORTAR:
    {
BotInitTask[5].Start();
        const object* pObject = m_CurTask->GetTargetObject();
        // Target may have been destroyed.
        if( pObject )
        {   
            InitializeMortar( *pObject );
        }
        else
        {
            // Target to repair has been destroyed (deployable).
            m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            m_CurTask->ForceReset();
        }
BotInitTask[5].Stop();
        break;
    }

    case bot_task::TASK_STATE_DESTROY:
    {
BotInitTask[6].Start();
        const object* pObject = m_CurTask->GetTargetObject();
        // Target may have been destroyed.
        if( pObject )
        {   
            InitializeDestroy( *pObject );
        }
        else
        {
            // Target to repair has been destroyed (deployable).
            m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            m_CurTask->ForceReset();
        }
BotInitTask[6].Stop();
        break;
    }
    
    case bot_task::TASK_STATE_REPAIR:
    {
BotInitTask[7].Start();
        const object* pObject = m_CurTask->GetTargetObject();
        // Target may have been destroyed.
        if ( pObject )
        {
            InitializeRepair( *pObject );
        }
        else
        {
            // Target to repair has been destroyed (deployable).
            m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
            m_CurTask->ForceReset();
        }
BotInitTask[7].Stop();
        break;
    }
    
    case bot_task::TASK_STATE_ANTAGONIZE:
BotInitTask[8].Start();
        InitializeAntagonize();
BotInitTask[8].Stop();
        break;

    case bot_task::TASK_STATE_DEPLOY_TURRET:
BotInitTask[9].Start();
        InitializeDeploy(m_CurTask->GetTargetPos()
            , m_CurrentGoal.GetAuxLocation()
            , object::TYPE_TURRET_CLAMP);
BotInitTask[9].Stop();
        break;

    case bot_task::TASK_STATE_DEPLOY_INVEN:
BotInitTask[10].Start();
        InitializeDeploy(m_CurTask->GetTargetPos()
            , m_CurrentGoal.GetAuxLocation()
            , object::TYPE_STATION_DEPLOYED);
BotInitTask[10].Stop();
        break;

    case bot_task::TASK_STATE_DEPLOY_SENSOR:
BotInitTask[11].Start();
        InitializeDeploy(m_CurTask->GetTargetPos()
            , m_CurrentGoal.GetAuxLocation()
            , object::TYPE_SENSOR_REMOTE);
BotInitTask[11].Stop();
        break;

    case bot_task::TASK_STATE_SNIPE:
BotInitTask[12].Start();
        InitializeSnipe( m_CurTask->GetTargetPos() );
BotInitTask[12].Stop();
        break;

    default:
        // This task state is not implemented
        ASSERT( 0 );
        m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
    }

    if ( m_CurTask->GetTargetObject() == NULL )
    {
        m_CurrentGoal.SetTargetObject( obj_mgr::NullID );
    }
    else if ( m_CurrentGoal.GetTargetObject() == NULL )
    {
        m_CurrentGoal.SetTargetObject(
            m_CurTask->GetTargetObject()->GetObjectID() );
    }
BotInitTask[0].Stop();
}

//==============================================================================
// InitializeRoam()
//==============================================================================
void bot_object::InitializeRoam( void )
{
    // We need a task to know where to roam...
    if ( !m_CurTask )
    {
        ASSERT( m_CurTask );
        x_DebugLog( "Bot: m_CurTask NULL in InitializeRoam()" );
        return;
    }

    InitializeRoam( m_CurTask->GetTargetPos() );
}

    
//==============================================================================
// InitializeRoam()
//==============================================================================
void bot_object::InitializeRoam( const vector3& RoamCenter )
{
    const xbool Outdoors = !InsideBuilding( RoamCenter);
    const f32 RoamRadius 
        = Outdoors 
        ? ROAM_OUTDOOR_RADIUS
        : ROAM_INDOOR_RADIUS;

    // Randomly pick a valid point that is within roaming distance
    // of our target location
    InitializeGoTo( PickPointInRadius(
        RoamCenter
        , RoamRadius
        , Outdoors) );
    m_CurrentGoal.SetGoalID( bot_goal::ID_ROAM );
    m_CurrentGoal.SetAuxLocation( RoamCenter );
}

xbool LoadoutHasNoPack( bot_object::loadout Loadout )
{
    s32 i;

    for ( i = player_object::INVENT_TYPE_PACK_START; i < player_object::INVENT_TYPE_PACK_END; ++i )
    {
        if ( Loadout.Inventory.Counts[i] > 0 )
        {
            return FALSE;
        }
    }

    return TRUE;
}

//==============================================================================
// InitializeLoadout()
//==============================================================================
void bot_object::InitializeLoadout( const station& Inven )
{
    m_InventTimer.Reset(); // but don't start until we're on a station.

    if ( !m_CurTask )
    {
        ASSERT( m_CurTask );
        x_DebugLog( "Bot: m_CurTask NULL in InitializeLoadout()" );
        return;
    }

    m_InvenId = Inven.GetObjectID();
    m_SteppingOffInven = FALSE;
    
    //--------------------------------------------------
    // set the correct loadout in the player
    //--------------------------------------------------
    bot_object::loadout IdealLoadout = GetIdealLoadout();
    if ( LoadoutHasNoPack( IdealLoadout ) )
    {
        IdealLoadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_ENERGY] = 1;
    }

    SetInventoryLoadout( GetIdealLoadout() );

    //--------------------------------------------------
    // Make sure we are going to the right place
    //--------------------------------------------------
    if ( Inven.GetKind() == station::DEPLOYED )
    {
        m_UsingFixedInven = FALSE;

        // Pick a point out in front of the inven for our approach
        vector3 Rot( Inven.GetRotation() );
        Rot *= 3.0f; // meters out in front...
        InitializeGoTo( Inven.GetPosition() - Rot );
    }
    else
    {
        m_UsingFixedInven = TRUE;

        if ( (Inven.GetPosition() - m_WorldPos).LengthSquared() < 4.0f )
        {
            // we need to step off the inven station to reset it
            m_SteppingOffInven = TRUE;
            vector3 Rot( Inven.GetRotation() );
            Rot *= 5.0f; // meters out in front...
            InitializeGoTo( Inven.GetPosition() - Rot );
        }
        else
        {
            InitializeGoTo( Inven.GetPosition() );
        }
    }
    
    //--------------------------------------------------
    // set up m_CurrentGoal
    //--------------------------------------------------
    m_CurrentGoal.SetGoalID( bot_goal::ID_LOADOUT );
}

//==============================================================================
// InitializePickup()
//==============================================================================
void bot_object::InitializePickup( const object& Pickup )
{
    //--------------------------------------------------
    // set up m_CurrentGoal
    //--------------------------------------------------
    const vector3& PickupPosition = Pickup.GetPosition();

    vector3 PickupBottom = PickupPosition;
    PickupBottom.Y = Pickup.GetBBox().Min.Y;

    if ( PickupPosition.Y != PickupBottom.Y )
    {
        collider            Ray;
        Ray.RaySetup( &Pickup
            , PickupPosition
            , PickupBottom
            , Pickup.GetObjectID().GetAsS32() );
        ObjMgr.Collide( ATTR_SOLID, Ray );

        if ( Ray.HasCollided() == TRUE )
        {
            collider::collision Collision;
            Ray.GetCollision( Collision );
            PickupBottom = Collision.Point;
        }
    }

    InitializeGoTo( PickupBottom );
    m_CurrentGoal.SetGoalID( bot_goal::ID_PICKUP );
}

//==============================================================================
// InitializeDeploy()
//==============================================================================
#ifdef acheng
extern s32 g_LastActualDeployInited;
#endif
void bot_object::InitializeDeploy( const vector3& StandPos
    , const vector3& DeployPos
    , object::type ObjectType )
{
#ifdef acheng
    g_LastActualDeployInited = m_MAIIndex;
#endif
    InitializeGoTo( StandPos );
    m_CurrentGoal.SetGoalID( bot_goal::ID_DEPLOY );
    m_CurrentGoal.SetAuxLocation( DeployPos );
    m_CurrentGoal.SetObjectType( ObjectType );
}

//==============================================================================
// InitializeSnipe()
//==============================================================================
void bot_object::InitializeSnipe( const vector3& StandPos )
{
    m_SnipeFirstGoto = TRUE;
    m_SnipeLocation = StandPos;
    InitializeGoTo( m_SnipeLocation );
    m_CurrentGoal.SetGoalID( bot_goal::ID_SNIPE );
    m_SniperTarget = obj_mgr::NullID;
    m_SniperSenseTimer.Reset();
    m_SniperSenseTimer.Start();
    m_SniperSenseTime = x_frand( 2.0f, 7.0f );
    m_SniperWanderTimer.Stop();
}

//==============================================================================
// HaveIdealLoadout()
//==============================================================================
xbool bot_object::HaveIdealLoadout( void ) const
{
    if ( (m_CurrentGoal.GetGoalID() == bot_goal::ID_LOADOUT)
        && m_InventTimer.IsRunning() 
        && (m_InventTimer.ReadSec() > 3.0f) )
    {
#if mreed
        x_DebugMsg( "Stuck here\n" );
        //ASSERT( 0 ); // we should check to see why this is failing...
#endif
        return TRUE;
    }

    game_type Type = GameMgr.GetGameType();
    const xbool IgnoreDisable = (
        // playing CTF?
        (Type == GAME_CTF)

        // Capping?
        && (m_CurTask && m_CurTask->GetID() == bot_task::TASK_TYPE_CAPTURE_FLAG)
            
        // Far enough from flag?
        && !NearEnemyFlag());
    
    if (!m_CurTask || (m_LoadoutDisabled && !IgnoreDisable)) return TRUE;
    
    const player_object::loadout IdealLoadout = GetIdealLoadout();

    if (!(Type == GAME_DM || Type == GAME_TDM || Type == GAME_HUNTER))
    {
        if (IdealLoadout.NumWeapons == 0 
            || m_CurTask->GetID() == bot_task::TASK_TYPE_ENGAGE_ENEMY )
        {
            // loadout doesn't matter
            return TRUE;
        }

        // Light armor and flares are all I need.
        if ( m_CurTask->GetID() == bot_task::TASK_TYPE_CAPTURE_FLAG )
        {
            ASSERT( (IdealLoadout.Armor == ARMOR_TYPE_LIGHT)
                && (IdealLoadout.Inventory.Counts[INVENT_TYPE_GRENADE_FLARE] > 0) );
            return ((m_Loadout.Armor == ARMOR_TYPE_LIGHT)
                && (m_Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_FLARE] > 0));
        }

        // Repair pack is all i need.
        if (m_CurTask->GetID() == bot_task::TASK_TYPE_REPAIR
            || m_CurTask->GetState() == bot_task::TASK_STATE_REPAIR)
        {
            return HasRepairPack();
        }

        // Deployable turret is all i need.
        if (m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_TURRET
            || m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_TURRET)
        {
            return m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR];
        }

        // Deployable SENSOR is all i need.
        if (m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_SENSOR
            || m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_SENSOR)
        {
            return m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR];
        }

        // Deployable inven is all i need.
        if (m_CurTask->GetID() == bot_task::TASK_TYPE_DEPLOY_INVEN
            || m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_INVEN)
        {
            return m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION];
        }
    }
    if ( !NeedArmor()
        && WeaponsMatch( IdealLoadout )
        && AProjectileWeaponCanFire()
        && PacksMatch( IdealLoadout )
        && BeltGearMatches( IdealLoadout ) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//==============================================================================
// NeedArmor()
//==============================================================================
xbool bot_object::NeedArmor( void ) const
{
    const player_object::loadout IdealLoadout = GetIdealLoadout();

    return ( IdealLoadout.Armor != m_Loadout.Armor );
}


//==============================================================================
// NeedInvenPack()
//==============================================================================
xbool bot_object::NeedInvenPack( void ) const
{
    const player_object::loadout IdealLoadout = GetIdealLoadout();

    return (
        (IdealLoadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION]
            > 0)
        && (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION]
            == 0));
}

//==============================================================================
// GetIdealLoadout()
//==============================================================================
bot_object::loadout bot_object::GetIdealLoadout( void ) const
{
    player_object::loadout Loadout;

    if ( !m_CurTask )
    {
        ASSERT( m_CurTask );

        x_DebugLog( "Bot: m_CurTask NULL in GetIdealLoadout()" );

        // use random loadout.
        default_loadouts::GetLoadout(
            GetCharacterType()
            , static_cast<default_loadouts::type>(
            default_loadouts::TASK_TYPE_START + m_RandLoadoutID - 1)
            , Loadout );

        return Loadout;
    }
    
    game_type Type = GameMgr.GetGameType();
    if (Type == GAME_TDM || 
        Type == GAME_DM  ||
        Type == GAME_HUNTER)
    {
        ASSERT(m_RandLoadoutID >= -3 && m_RandLoadoutID < bot_task::TASK_TYPE_TOTAL);
        // use random loadout.
        default_loadouts::GetLoadout(
            GetCharacterType()
            , static_cast<default_loadouts::type>(
            default_loadouts::TASK_TYPE_START + m_RandLoadoutID - 1)
            , Loadout );
    }
    else
    {
        default_loadouts::GetLoadout(
            GetCharacterType()
            , static_cast<default_loadouts::type>(
            default_loadouts::TASK_TYPE_START + m_CurTask->GetID() - 1)
            , Loadout );
    }
    const s32 Rand = x_irand( 0, 100 );
    const xbool WantGrenades
        = (Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC] > 0);

    if ( m_CurTask->GetID() == bot_task::TASK_TYPE_CAPTURE_FLAG )
    {
        // use flare grenades instead
        Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_FLARE]
            = Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC];
        Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC] = 0;
    }
    else if ( (Rand < 25) && WantGrenades )
    {
        // use concussion grenades instead
        Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_CONCUSSION]
            = Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC];
        Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_BASIC] = 0;
    }
    
    return Loadout;
}

//==============================================================================
// GetNearestInven()
//
// Returns the distance squared to the nearest inventory station, and stores
// its position in InventoryPos. The position is the center-bottom of the
// inventory station's bounding box.
//
// If no inventory station is found, returns < 0
//==============================================================================
f32 bot_object::GetNearestInven( station& Inven
    , xbool DeployedOK /* = TRUE */ ) const 
{
    station* pInven = NULL;
    station* pClosestInven = NULL;
    f32 ClosestInvenDistSQ = F32_MAX;

    ObjMgr.StartTypeLoop( object::TYPE_STATION_FIXED );

    while ( (pInven = (station*)ObjMgr.GetNextInType()) != NULL )
    {
        const f32 DistSQ = (pInven->GetPosition() - m_WorldPos).LengthSquared();
        if ( !(pInven->GetEnabled())
            || !(m_TeamBits & pInven->GetTeamBits()) )
        {
            continue;
        }
        
        // see if it's closer

        if ( DistSQ < ClosestInvenDistSQ )
        {
            // Best candidate so far
            pClosestInven = pInven;
            ClosestInvenDistSQ = DistSQ;
        }
    }

    ObjMgr.EndTypeLoop();
    
    if ( DeployedOK )
    {
        // check deployed stations
        ObjMgr.StartTypeLoop( object::TYPE_STATION_DEPLOYED );

        while ( (pInven = (station*)ObjMgr.GetNextInType()) != NULL )
        {
            const f32 DistSQ = (pInven->GetPosition() - m_WorldPos).LengthSquared();
            if ( !(pInven->GetEnabled())
                || !(m_TeamBits & pInven->GetTeamBits()) )
            {
                continue;
            }
        
            // see if it's closer

            if ( DistSQ < ClosestInvenDistSQ )
            {
                // Best candidate so far
                pClosestInven = pInven;
                ClosestInvenDistSQ = DistSQ;
            }
        }

        ObjMgr.EndTypeLoop();
    }
    
    if ( pClosestInven )
    {
        Inven = *pClosestInven;

        return ClosestInvenDistSQ;
    }
    else
    {
        return -1.0f;
    }
}

//==============================================================================
// GetNearestRepairPackPos()
//
// Returns the distance squared to the nearest repair pack pickup, and stores
// its position in PackPos. 
//
// If no pack is found, returns < 0
//==============================================================================
f32 bot_object::GetNearestRepairPackPos( vector3& PackPos )
{
    pickup* pPack = NULL;
    pickup* pClosestPack = NULL;
    f32 ClosestPackDistSQ = F32_MAX;

    ObjMgr.StartTypeLoop( object::TYPE_PICKUP );

    while ( (pPack = (pickup*)ObjMgr.GetNextInType()) != NULL )
    {
        if (pPack->GetKind() != pickup::KIND_ARMOR_PACK_REPAIR) 
        {
            continue;
        }
        
        // see if it's closer
        ASSERT(m_CurrentGoal.GetTargetObject());
        const f32 DistSQ = (pPack->GetPosition() 
            - m_CurrentGoal.GetTargetObject()->GetPosition()).LengthSquared();

        if ( DistSQ < ClosestPackDistSQ )
        {
            // Best candidate so far
            pClosestPack = pPack;
            ClosestPackDistSQ = DistSQ;
        }
    }

    ObjMgr.EndTypeLoop();
    
    if ( pClosestPack )
    {
        PackPos = GetTargetCenter( *pClosestPack );
        m_NearestRepairPack = pClosestPack->GetObjectID();
        return ClosestPackDistSQ;
    }
    else
    {
        return -1.0f;
    }
}

//==============================================================================
// WeaponsMatch()
//==============================================================================
xbool bot_object::WeaponsMatch( const player_object::loadout& IdealLoadout ) const 
{
    s32 i;

    for ( i = player_object::INVENT_TYPE_WEAPON_START + 1
              ; i <= player_object::INVENT_TYPE_WEAPON_END
              ; ++i )
    {
        if ( m_Loadout.Inventory.Counts[i] != IdealLoadout.Inventory.Counts[i] )
        {
            return FALSE;
        }
    }

    return TRUE;
}
    
//==============================================================================
// AProjectileWeaponCanFire()
//==============================================================================
xbool bot_object::AProjectileWeaponCanFire( void ) const
{
    s32 i;
    xbool HaveProjectileWeapon = FALSE;

    for ( i = player_object::INVENT_TYPE_WEAPON_START + 1
              ; i <= player_object::INVENT_TYPE_WEAPON_END
              ; ++i )
    {
        // ignore non-projectile weapons
        if ( i == player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE
            || i == player_object::INVENT_TYPE_WEAPON_BLASTER
            || i == player_object::INVENT_TYPE_WEAPON_ELF_GUN
            || i == player_object::INVENT_TYPE_WEAPON_REPAIR_GUN
            || i == player_object::INVENT_TYPE_WEAPON_ELF_GUN
            || i == player_object::INVENT_TYPE_WEAPON_SHOCKLANCE
            || i == player_object::INVENT_TYPE_WEAPON_TARGETING_LASER
            || i == player_object::INVENT_TYPE_WEAPON_START )
        {
            continue;
        }

        // check for ammo needed to fire
        if ( m_Loadout.Inventory.Counts[i] > 0 )
        {
            HaveProjectileWeapon = TRUE;
            
            if ( GetWeaponAmmoCount((player_object::invent_type)i)
                >= s_WeaponInfo[i].AmmoNeededToAllowFire )
            {
                return TRUE;
            }
        }
        
    }

    return !HaveProjectileWeapon;
}

//==============================================================================
// PacksMatch()
//==============================================================================
xbool bot_object::PacksMatch( const player_object::loadout& IdealLoadout ) const
{
    s32 i;

    for ( i = player_object::INVENT_TYPE_PACK_START
              ; i <= player_object::INVENT_TYPE_PACK_END
              ; ++i )
    {
        if ( m_Loadout.Inventory.Counts[i] != IdealLoadout.Inventory.Counts[i] )
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

//==============================================================================
// BeltGearMatches()
//==============================================================================
xbool bot_object::BeltGearMatches( const player_object::loadout& IdealLoadout ) const
{
    if ( (m_Loadout.Inventory.Counts[INVENT_TYPE_BELT_GEAR_BEACON] == 0)
        && IdealLoadout.Inventory.Counts[INVENT_TYPE_BELT_GEAR_BEACON] > 0 )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

//==============================================================================
// GetDesiredPickupPos()
//
// Search for a pickup that the bot would want, and return a pointer to it.
//==============================================================================
object* bot_object::GetDesiredPickup( void )
{
    // for now, we're interested in the nearest pickup (or corpse that hasn't
    // been tagged by us) that is within the search radius

    // first, find the nearest pickup that's inside the radius
    f32 ClosestDistSQ = PICKUP_INTEREST_RADIUS_SQ;
    object* pObject = NULL;
    object* pReturnObject = NULL;
    
    ObjMgr.StartTypeLoop( object::TYPE_PICKUP );
    while ( (pObject = ObjMgr.GetNextInType()) != NULL )
    {
        const f32 DistSQ
            = (pObject->GetPosition() - m_WorldPos).LengthSquared();
        if ( DistSQ < ClosestDistSQ
            && WantPickup( *((pickup*)pObject) )
            && !FindRetrievedPickup( *pObject ) )
        {
            pReturnObject = pObject;
            ClosestDistSQ = DistSQ;
        }
    }
    ObjMgr.EndTypeLoop();

    if ( ClosestDistSQ < PICKUP_INTEREST_RADIUS_SQ )
    {
        ASSERT( pReturnObject );
        StoreRetrievedPickup( *pReturnObject );
    }

    return pReturnObject;
}


//==============================================================================
// StoreRetrievedPickup()
//==============================================================================
void bot_object::StoreRetrievedPickup( const object& Object )
{
    // rotate all entries up
    s32 i;
    for ( i = MIN( m_nRetrievedPickups, (10-1)); i > 0; --i )
    {
        m_pRetrievedPickups[i] = m_pRetrievedPickups[i - 1];
    }

    // add new entry
    m_pRetrievedPickups[0] = Object.GetObjectID();

    // update the count
    ++m_nRetrievedPickups;
    m_nRetrievedPickups = MIN( m_nRetrievedPickups, 10 );
}

//==============================================================================
// FindRetrievedPickup()
//==============================================================================
xbool bot_object::FindRetrievedPickup( const object& Object ) const
{
    s32 i;
    const object::id& ID = Object.GetObjectID();

    for ( i = 0; i < 10; ++i )
    {
        if ( m_pRetrievedPickups[i] == ID )
        {
            return TRUE;
        }
    }

    return FALSE;
}

//==============================================================================
// WantPickup()
//==============================================================================
xbool bot_object::WantPickup( const pickup& Pickup ) const
{
     if ( !m_CurTask ) return FALSE; // no reason to believe we want it

    // if it's a pack and we have no pack, we want it
    // if it's an energy pack and our cur pack isn't a deployable, we want it
    // we don't want no weapons
    
    switch ( Pickup.GetKind() )
    {
    case pickup::KIND_AMMO_DISC:
    case pickup::KIND_AMMO_CHAIN_GUN:
    case pickup::KIND_AMMO_PLASMA:
    case pickup::KIND_AMMO_GRENADE_LAUNCHER:
    case pickup::KIND_AMMO_GRENADE_BASIC:
    case pickup::KIND_AMMO_MINE:
        //TODO: determine if we could use the ammo...
        return FALSE;
        break;
        
    case pickup::KIND_ARMOR_PACK_AMMO:
    case pickup::KIND_ARMOR_PACK_REPAIR:
    case pickup::KIND_ARMOR_PACK_SENSOR_JAMMER:
    case pickup::KIND_ARMOR_PACK_SHIELD:
    case pickup::KIND_ARMOR_PACK_ENERGY:
        return ( m_PackCurrentType == INVENT_TYPE_NONE );
        break;

    case pickup::KIND_AMMO_BOX:
        // always grab these so someone else doesn't get it
        return TRUE;
        break;
        
    default:
        return FALSE;
    }
    
    return TRUE;
}

//==============================================================================
// WantCorpse()
//==============================================================================
xbool bot_object::WantCorpse( void ) const
{
    return (
        // TODO: check to see if we need ammo...
        
        // need health
        ((m_Health < 70.0f)
            && (m_Loadout.Inventory.Counts[INVENT_TYPE_BELT_GEAR_HEALTH_KIT]
                == 0)) ) ;
}

//==============================================================================
// TestUpdate()
//==============================================================================
void bot_object::TestUpdate( void )
{
    m_Testing = TRUE;
    
    if ( g_BotDebug.TestMortar )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_MORTAR )
        {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_MORTAR)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();

#if 0
            // go...
            player_object* pPlayer = GetFirstPlayer();
            ASSERT( pPlayer );
            InitializeMortar( *pPlayer );
#else
            f32 DistSQ;
            f32 MinDistSQ = F32_MAX;
            object* pObject = NULL;
            object* pNearestObject = NULL;
            ObjMgr.StartTypeLoop( object::TYPE_TURRET_FIXED );
            while( (pObject = ObjMgr.GetNextInType()) )
            {
                if( (pObject->GetTeamBits() & GetTeamBits()) == 0 )
                    
                {
                    DistSQ = (pObject->GetPosition() - m_WorldPos).LengthSquared();
                    if ( DistSQ < MinDistSQ )
                    {
                        MinDistSQ = DistSQ;
                        pNearestObject = pObject;
                    }
                }
            }
            
            ObjMgr.EndTypeLoop();

            ASSERT( pNearestObject );
            InitializeMortar( *pNearestObject );
#endif
        }

        if ( GetWeaponAmmoCount( player_object::INVENT_TYPE_WEAPON_MORTAR_GUN )
            < s_WeaponInfo[player_object::INVENT_TYPE_WEAPON_MORTAR_GUN]
            .AmmoNeededToAllowFire )
        {
            InventoryReload();
        }
    }
    else if ( g_BotDebug.TestRepair )
    {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_REPAIR)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();

        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_REPAIR )
        {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_REPAIR)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();

            f32 DistSQ;
            f32 MinDistSQ = F32_MAX;
            object* pObject = NULL;
            object* pNearestObject = NULL;
            ObjMgr.StartTypeLoop( object::TYPE_TURRET_FIXED );
            while( (pObject = ObjMgr.GetNextInType()) )
            {
                if ( ((pObject->GetTeamBits() & GetTeamBits()) != 0)
                    && (pObject->GetHealth() < 1.0f) )
                {
                    DistSQ = (pObject->GetPosition() - m_WorldPos).LengthSquared();
                    if ( DistSQ < MinDistSQ )
                    {
                        MinDistSQ = DistSQ;
                        pNearestObject = pObject;
                    }
                }
            }
            
            ObjMgr.EndTypeLoop();

#if 0
            // go...
            player_object* pPlayer = GetFirstPlayer();
            ASSERT( pPlayer );

            // make sure we're on the same team so repair is possible
            SetTeamBits(pPlayer->GetTeamBits() );
            InitializeRepair( *pPlayer );
#else
            if ( pObject ) InitializeRepair( *pNearestObject );
            
#endif
                
        }

    }
    else if ( g_BotDebug.TestDestroy )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_DESTROY )
        {
            if ( m_Health < 1.0f ) AddHealth( 1.0f );
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_DESTROY)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();
            
            f32 DistSQ;
            f32 MinDistSQ = F32_MAX;
            object* pObject = NULL;
            object* pNearestObject = NULL;
            ObjMgr.StartTypeLoop( object::TYPE_TURRET_FIXED );
            while( (pObject = ObjMgr.GetNextInType()) )
            {
                if ( (pObject->GetTeamBits() & GetTeamBits()) == 0 )
                {
                    DistSQ = (pObject->GetPosition() - m_WorldPos).LengthSquared();
                    if ( DistSQ < MinDistSQ )
                    {
                        MinDistSQ = DistSQ;
                        pNearestObject = pObject;
                    }
                }
            }
            
            ObjMgr.EndTypeLoop();

            if ( pNearestObject )
            {
                InitializeDestroy( *pNearestObject );
            }

        }
    }
    else if ( g_BotDebug.TestEngage )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_ENGAGE )
        {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_DESTROY)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();
            
            player_object* pPlayer = GetFirstPlayer();
            ASSERT( pPlayer );

            InitializeEngage( *pPlayer );
        }
    }
    else if ( g_BotDebug.TestAntagonize )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_ANTAGONIZE )
        {
            InitializeAntagonize();
        }
    }
    else if ( g_BotDebug.TestDeploy )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_DEPLOY )
        {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_DESTROY)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();

            InitializeDeploy( vector3( 1432.6f, 86.06f, 478.67f )
                , vector3( 1432.15f, 88.15f, 476.25f )
                , object::TYPE_TURRET_CLAMP);
        }
    }
    else if ( g_BotDebug.TestSnipe )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_SNIPE )
        {
            // make sure we have the loadout
            loadout Loadout;
            default_loadouts::GetLoadout(
                GetCharacterType()
                , static_cast<default_loadouts::type>(
                    default_loadouts::TASK_TYPE_SNIPE)
                , Loadout );
            SetInventoryLoadout( Loadout );
            InventoryReload();

            InitializeSnipe( vector3( 1432.94f, 106.57f, 455.05f ) );
        }
    }
    else if ( g_BotDebug.TestRoam )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_ROAM )
        {
            // make sure we have the loadout
            InitializeRoam( vector3( 1409.48f, 89.79f, 445.77f ) ); // katabatic flag
        }
    }
    else if ( g_BotDebug.TestLoadout )
    {
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_LOADOUT )
        {
            f32 DistSQ;
            f32 MinDistSQ = F32_MAX;
            object* pObject = NULL;
            object* pNearestObject = NULL;
            ObjMgr.StartTypeLoop( object::TYPE_STATION_FIXED );
            while( (pObject = ObjMgr.GetNextInType()) )
            {
                DistSQ = (pObject->GetPosition() - m_WorldPos).LengthSquared();
                if ( DistSQ < MinDistSQ )
                {
                    MinDistSQ = DistSQ;
                    pNearestObject = pObject;
                }
            }
            
            ObjMgr.EndTypeLoop();

            if ( pNearestObject != NULL )
            {
                SetTeamBits( pNearestObject->GetTeamBits() );
                m_CurTask = new bot_task;
                m_CurTask->SetID( bot_task::TASK_TYPE_MORTAR );
                InitializeLoadout( *(station*)pNearestObject );
            }
        }
    }
}

//==============================================================================
// GetFirstPlayer()
//==============================================================================
player_object* bot_object::GetFirstPlayer( void )
{
    player_object* pPlayer = NULL;

    ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
    pPlayer = (player_object*)ObjMgr.GetNextInType();
    ObjMgr.EndTypeLoop();

    return pPlayer;
}

//==============================================================================
//  ComparePointsForDistance()
//==============================================================================
static vector3 TempWorldPos;
static s32 ComparePointsForDistance( vector3* A, vector3* B )
{
    return (s32)( (TempWorldPos - *B).LengthSquared()
           - (TempWorldPos - *A).LengthSquared() );
}

//==============================================================================
//  CompareNodesForDistance()
//==============================================================================
static s32 CompareNodesForDistance( s32* A, s32* B )
{
    return (s32)( (TempWorldPos - g_Graph.m_pNodeList[*B].Pos).LengthSquared()
           - (TempWorldPos - g_Graph.m_pNodeList[*A].Pos).LengthSquared() );
}

//==============================================================================
// GetDestroyVantagePoint()
//==============================================================================
void bot_object::GetDestroyVantagePoint( vector3& Point
    , const object& Target )
{
    const vector3 Pos = GetTargetCenter( Target );

    if ( !m_WaitingForDestroyVantagePoint )
    {
        m_WaitingForDestroyVantagePoint = TRUE;
        
        //--------------------------------------------------
        // Get the first n path points within x meters
        //--------------------------------------------------
        static const s32 MAX_NUM_POINTS = 200;
        static const f32 VANTAGE_POINT_RADIUS = 250.0f;
    
        m_NumCandidates = m_PathKeeper.GetNPointsInRadius(
            MAX_NUM_POINTS
            , VANTAGE_POINT_RADIUS
            , Pos
            , m_VantagePointCandidates );

        //--------------------------------------------------
        // Add our position to the candidate list
        //--------------------------------------------------
        if ( m_NumCandidates == MAX_NUM_POINTS )
        {
            --m_NumCandidates;
        }
        

        const f32 TerrHeight
            = m_pTerr->GetHeight( m_WorldPos.Z, m_WorldPos.X );
        m_VantagePointCandidates[m_NumCandidates] = m_WorldPos;

        if ( m_WorldPos.Y > TerrHeight )
        {
            m_VantagePointCandidates[m_NumCandidates].Y = TerrHeight;
        }
        
        ++m_NumCandidates;

        m_CurCandidateIndex = m_NumCandidates - 1;

        if ( m_NumCandidates > 0 )
        {
            //--------------------------------------------------
            // Sort these points in ascending order of distance
            // from m_WorldPos
            //--------------------------------------------------
            TempWorldPos = m_WorldPos;
            x_qsort( m_VantagePointCandidates
                , m_NumCandidates
                , sizeof( vector3 )
                , (compare_fn*)ComparePointsForDistance );
        }
    }

    if ( m_NumCandidates <= 0 )
    {
        // No points around
        Point = Pos;
        m_WaitingForDestroyVantagePoint = FALSE;
        return;
    }

    //--------------------------------------------------
    // Now find the farthest point where we can hit the
    // target pos
    //--------------------------------------------------

    const xbool HaveMortar
        = m_Loadout.Inventory.Counts[INVENT_TYPE_WEAPON_MORTAR_GUN] > 0;

    // temporarily move us to the candidate point for hit target checks
    TempWorldPos = m_WorldPos;
    const vector3 TempVel = m_Vel;
    m_WorldPos = m_VantagePointCandidates[m_CurCandidateIndex];
    m_Vel.Zero();

    // test for hit
    if ( CanHitTarget( Target )
        && (!HaveMortar
            || CanHitTargetArcing( INVENT_TYPE_WEAPON_MORTAR_GUN, Target )) )
    {
        // got one
        m_WaitingForDestroyVantagePoint = FALSE;
        Point = m_VantagePointCandidates[m_CurCandidateIndex];
    }

    // move us back to where we were
    m_WorldPos = TempWorldPos;
    m_Vel = TempVel;

    // next time we'll check the next index
    --m_CurCandidateIndex;

    // check to see if we're done checking
    if ( m_CurCandidateIndex < 0 && m_WaitingForDestroyVantagePoint )
    {
        // No good shots...
        Point = Pos;
        m_WaitingForDestroyVantagePoint = FALSE;
    }
}

//==============================================================================
// GetPermanentVantagePoints()
// returns the number of vantage points added
//==============================================================================
s32 bot_object::GetPermanentVantagePoints(
    graph::vantage_point* pVantagePoints
    , s32 MaxNumPoints )
{
    s32 NumPoints = 0;

    //--------------------------------------------------
    // Loop through all permanent objects and find a
    // mortar point and a linear point
    //--------------------------------------------------
    object* pTarget;
    s32 LinearVantageNode;
    s32 MortarVantageNode;
    xbool FoundLinearNode;
    xbool FoundMortarNode;
    
    ObjMgr.Select( object::ATTR_PERMANENT | object::ATTR_DAMAGEABLE );
    while ( (pTarget = ObjMgr.GetNextSelected()) != NULL )
    {
        // some things we really can't destroy
        switch ( pTarget->GetType() )
        {
        case TYPE_TERRAIN:
        case TYPE_SCENIC:
        case TYPE_FLAG:
        case TYPE_BUILDING:
            continue;
        default:
            ;
        }

        // this algorithm really only works with indoor targets
        if ( !InsideBuilding( pTarget->GetPosition() ) )
        {
            continue;
        }
        
        GetDestroyVantageNodes(
            FoundLinearNode
            , LinearVantageNode
            , FoundMortarNode
            , MortarVantageNode
            , *pTarget );

        // If this fails then we have a target with no good nodes nearby...
        ASSERT( FoundLinearNode || FoundMortarNode );

        //--------------------------------------------------
        // Add the vantage point(s) to the list
        //--------------------------------------------------
        if ( FoundLinearNode )
        {
            ASSERT( NumPoints < MaxNumPoints ); // need larger max...

            pVantagePoints[NumPoints].NodeID = LinearVantageNode;
            pVantagePoints[NumPoints].TargetType = pTarget->GetType();
            pVantagePoints[NumPoints].TargetWorldPos = pTarget->GetPosition();
            pVantagePoints[NumPoints].IsMortarPoint = FALSE;

            ++NumPoints;
        }

        if ( FoundMortarNode )
        {
            ASSERT( NumPoints < MaxNumPoints ); // need larger max...

            pVantagePoints[NumPoints].NodeID = MortarVantageNode;
            pVantagePoints[NumPoints].TargetType = pTarget->GetType();
            pVantagePoints[NumPoints].TargetWorldPos = pTarget->GetPosition();
            pVantagePoints[NumPoints].IsMortarPoint = TRUE;

            ++NumPoints;
        }
    }
    ObjMgr.ClearSelection();

    return NumPoints;
}


//==============================================================================
// GetDestroyVantageNodes()
// service method for GetPermanentVantagePoints()
//==============================================================================
void bot_object::GetDestroyVantageNodes(
      xbool&        FoundLinearNode
    , s32&          LinearNode
    , xbool&        FoundMortarNode
    , s32&          MortarNode
    , const object& Target )
{
    FoundLinearNode = FALSE;
    FoundMortarNode = FALSE;

    const vector3 Pos = GetTargetCenter( Target );

    //--------------------------------------------------
    // Get the first n path points within x meters
    //--------------------------------------------------
    static const s32 MAX_NUM_POINTS = 200;
    static const f32 VANTAGE_POINT_RADIUS = 250.0f;
    s32 pVantageNodeCandidates[200];
    
    s32 NumCandidates = m_PathKeeper.GetNNodesInRadius(
        MAX_NUM_POINTS
        , VANTAGE_POINT_RADIUS
        , Pos
        , pVantageNodeCandidates );

    s32 CurCandidateIndex = NumCandidates - 1;

    if ( NumCandidates > 0 )
    {
        //--------------------------------------------------
        // Sort these points in ascending order of distance
        // from m_WorldPos
        //--------------------------------------------------
        TempWorldPos = m_WorldPos;
        x_qsort( pVantageNodeCandidates
            , NumCandidates
            , sizeof( s32 )
            , (compare_fn*)CompareNodesForDistance );
    }

    if ( NumCandidates <= 0 )
    {
        // No points around
        return;
    }

    //--------------------------------------------------
    // Now find the farthest point where we can hit the
    // target pos
    //--------------------------------------------------
    while ( !FoundLinearNode || !FoundMortarNode ) 
    {
        // temporarily move us to the candidate point for hit target checks
        TempWorldPos = m_WorldPos;
        const vector3 TempVel = m_Vel;
        m_WorldPos = g_Graph.m_pNodeList[pVantageNodeCandidates[CurCandidateIndex]].Pos;
        m_Vel.Zero();

        // test for hit
        if ( CanHitTarget( Target ) )
        {
            FoundLinearNode = TRUE;
            LinearNode = pVantageNodeCandidates[CurCandidateIndex];
        }

        if ( CanHitTargetArcing( INVENT_TYPE_WEAPON_MORTAR_GUN, Target ) )
        {
            FoundMortarNode = TRUE;
            MortarNode = pVantageNodeCandidates[CurCandidateIndex];
        }

        // move us back to where we were
        m_WorldPos = TempWorldPos;
        m_Vel = TempVel;

        // next time we'll check the next index
        --CurCandidateIndex;

        // check to see if we're done checking
        if ( CurCandidateIndex < 0 )
        {
            // We've found all we're gonna find
            break;
        }
    }
}


//==============================================================================
// Defending
//==============================================================================
xbool bot_object::Defending( void ) const
{
    return ( m_CurTask
        && ((m_CurTask->GetID() == bot_task::TASK_TYPE_LIGHT_DEFEND_FLAG)
            || (m_CurTask->GetID() == bot_task::TASK_TYPE_DEFEND_FLAG)) );
}


