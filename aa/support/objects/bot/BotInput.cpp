//==============================================================================
//
// BotInput.cpp
//
// Contains methods for the bot_object class relating to input for the
// virtual game controller
//==============================================================================

#include "BotObject.hpp"
#include "BotLog.hpp"
#include "ctf_masterai.hpp"
#include "Support\GameMgr\GameMgr.hpp"
#include "Support\Objects\Projectiles\Beacon.hpp"
#include "Support\Building\BuildingOBJ.hpp"
#include "MAI_Manager.hpp"
#include "Support\Objects\Vehicles\Vehicle.hpp"
#include "Support\Objects\Projectiles\Sensor.hpp"

//==============================================================================
//  TYPES
//==============================================================================

//==============================================================================
//  DEFINES
//==============================================================================
#define DEPLOY_BEACONS_WITH_INVEN 0

//static       f32 INPUT_UPDATE_TIME = 0.125f;
static const s16 PURSUIT_PREDICTION_CONSTANT = 5  ; // exaggerates pursuit pred.
static const f32 MAX_PATH_SEARCH_TIME = 1000.0f; // Milliseconds

// defined in BotObject.cpp
static const f32 CLOSE_ENOUGH = 0.75f;
static const f32 CLOSE_ENOUGH_SQ = x_sqr( CLOSE_ENOUGH );

const f32 ARRIVAL_CLOSE_ENOUGH = 0.5f;
const f32 ARRIVAL_CLOSE_ENOUGH_SQ = x_sqr( ARRIVAL_CLOSE_ENOUGH );

static const f32 REALLY_CLOSE = 0.01f;
static const f32 REALLY_CLOSE_SQ = x_sqr( REALLY_CLOSE );

extern bot_object* ExamineBot;
extern s32         g_BotFrameCount;

#if TESTING_VANTAGE_POINTS
extern graph::vantage_point g_pVantagePoints[1000];
extern s32 g_nVantagePoints;
#endif

static const f32 MAX_FACE_YAW_SPEED     = 2*R_360;
static const f32 MAX_FACE_PITCH_SPEED   = R_180;
static const f32 MIN_FACE_YAW_SPEED     = R_90;
static const f32 MIN_FACE_PITCH_SPEED   = R_45;

//----------------------------------------------------------------------
// Steering behavior stuff
//----------------------------------------------------------------------
static const f32 AERIAL_MAX_SPEED = 200.0f;
static const f32 TIGHT_AERIAL_MAX_SPEED = 10.0f;
static const f32 GROUND_MAX_SPEED = 200.0f;
static const f32 TIGHT_GROUND_MAX_SPEED = 18.0f;
static const f32 AERIAL_ARRIVAL_SLOWING_DIST = 15.0f;
static const f32 GROUND_ARRIVAL_SLOWING_DIST = 8.0f;
static const f32 PATH_FOLLOWING_LOOSE_AIR_PREDICTION_TIME = 0.75f; // seconds
static const f32 PATH_FOLLOWING_TIGHT_AIR_PREDICTION_TIME = 0.3f; // seconds
static const f32 PATH_FOLLOWING_GROUND_PREDICTION_TIME = 0.8f; // seconds
static const f32 PATH_FOLLOWING_LOOSE_PURSUE_TIME = 0.2f;
static const f32 PATH_FOLLOWING_TIGHT_PURSUE_TIME = 0.1f;
static const f32 PATH_FOLLOWING_DISTANCE = 0.5f;
static const f32 PATH_FOLLOWING_DISTANCE_SQ = x_sqr( PATH_FOLLOWING_DISTANCE );
static const f32 PATH_FOLLOWING_LOOSE_DISTANCE = 0.5f;
static const f32 PATH_FOLLOWING_LOOSE_DISTANCE_SQ
                     = x_sqr( PATH_FOLLOWING_LOOSE_DISTANCE );
static const f32 PURSUE_TIMEOUT_TIME_SEC = 5.0f;
static const f32 STUCK_TIME_SEC = 1.0f;
static const radian STEEP = R_50;
static const f32 STEEP_DY = 0.766f;


//----------------------------------------------------------------------
// Engage stuff
//----------------------------------------------------------------------
static const f32 ENGAGE_START_FIGHTING_DISTANCE = 40.0f;
static const f32 ENGAGE_START_FIGHTING_DISTANCE_SQ
                     = x_sqr( ENGAGE_START_FIGHTING_DISTANCE );
static const f32 ENGAGE_STOP_FIGHTING_DISTANCE = 60.0f;
static const f32 ENGAGE_STOP_FIGHTING_DISTANCE_SQ
                     = x_sqr( ENGAGE_STOP_FIGHTING_DISTANCE );
static const f32 ENGAGE_ELF_START_FIGHTING_DISTANCE = 10.0f;
static const f32 ENGAGE_ELF_START_FIGHTING_DISTANCE_SQ
                     = x_sqr( ENGAGE_ELF_START_FIGHTING_DISTANCE );
static const f32 ENGAGE_ELF_STOP_FIGHTING_DISTANCE = 15.0f;
static const f32 ENGAGE_ELF_STOP_FIGHTING_DISTANCE_SQ
                     = x_sqr( ENGAGE_ELF_STOP_FIGHTING_DISTANCE );
static const f32 ENGAGE_MIN_AIR_COMBAT_START_FLYING_ENERGY = 0.7f;
static const f32 ENGAGE_MIN_AIR_COMBAT_STOP_FLYING_ENERGY = 0.05f;
static const f32 ENGAGE_AIR_SUPERIORITY_ANGLE = R_30;


static const f32 MAX_CAPTURE_FLAG_ATTACK_DIST = 75.0f;
static const f32 MAX_CAPTURE_FLAG_ATTACK_DIST_SQ
                     = x_sqr( MAX_CAPTURE_FLAG_ATTACK_DIST );
static const vector3 CHEST_HEIGHT( 0.0f, 1.0f, 0.0f );
static const f32 MAX_DODGE_TIME_SEC = 2.0f;

//----------------------------------------------------------------------
// Antagonize stuff
//----------------------------------------------------------------------
static const f32 MAX_DANGEROUS_MINE_RADIUS = 20.0f;
static const f32 MAX_DANGEROUS_MINE_RADIUS_SQ
                     = x_sqr( MAX_DANGEROUS_MINE_RADIUS );
static const f32 MAX_DANGEROUS_TURRET_RADIUS = 400.0f;
static const f32 MAX_DANGEROUS_TURRET_RADIUS_SQ
                     = x_sqr( MAX_DANGEROUS_TURRET_RADIUS );
static const f32 ANTAGONIZE_FOV_DIV_2 = R_45;
static const f32 ANTAGONIZE_HEARING_DIST_SQ = x_sqr( 10.0f );
static const f32 ANTAGONIZE_MAX_RANGE_SQ = x_sqr( 200.0f );

//----------------------------------------------------------------------
// Mortar stuff
//----------------------------------------------------------------------
static const f32 MIN_MORTAR_RANGE = 250.0f;
static const f32 MIN_MORTAR_RANGE_SQ
                     = x_sqr( MIN_MORTAR_RANGE );

//----------------------------------------------------------------------
// destroy/repair stuff
//----------------------------------------------------------------------
static const f32 MIN_REPAIR_DIST = 5.0f;
static const f32 MIN_REPAIR_DIST_SQ = x_sqr( MIN_REPAIR_DIST );
static const f32 MIN_DESTROY_DIST = 10.0f;
static const f32 MIN_DESTROY_DIST_SQ = x_sqr( MIN_DESTROY_DIST );
static const f32 MIN_DESTROY_VEHICLE_DIST = 200.0f;
static const f32 MIN_DESTROY_VEHICLE_DIST_SQ
                     = x_sqr( MIN_DESTROY_VEHICLE_DIST );
static const vector3 UP_VEC( 0.0f, 1.0f, 0.0f );
static const f32 DESTROY_COMFORT_DIST = 100.0f;
static const f32 DESTROY_COMFORT_DIST_SQ = x_sqr( DESTROY_COMFORT_DIST );

//==============================================================================
//  STORAGE
//==============================================================================
extern bot_object::bot_debug g_BotDebug;
extern graph g_Graph;
#if EDGE_COST_ANALYSIS
    extern  xbool  g_bCostAnalysisOn;
#endif


#if BOT_UPDATE_TIMING

    struct bot_timing_data;
    bot_timing_data g_BotTiming;
    bot_timing_data* g_BotTimingArray[16];

#endif
//==============================================================================
//  FUNCTIONS
//==============================================================================

//==============================================================================
// UpdateInput()
//==============================================================================
void bot_object::UpdateInput( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

#if BOT_UPDATE_TIMING
    g_BotTiming.UpdateTimer.Start();
    g_BotTiming.nCalls++;
    m_BotTiming.UpdateTimer.Start();
    m_BotTiming.nCalls++;
#endif
    if ( TRUE )
    {
        //--------------------------------------------------
        // reset some values
        //--------------------------------------------------
        m_Aiming = FALSE;
        m_GoingSomewhere = FALSE;
        //--------------------------------------------------

        m_Lines.Clear();
        m_Spheres.Clear();
        m_Markers.Clear();
#if TIMING_UPDATE_INPUT
        xtimer timer;
        timer.Start();
#endif

        if ( ! m_DontClearMove )
        {
            m_Move.Clear();
        }
        
        switch( m_CurrentGoal.GetGoalID() )
        {
        case bot_goal::ID_PURSUE_OBJECT:
        {
            UpdatePursueObject();
            break;
        }
        case bot_goal::ID_PURSUE_LOCATION:
        {
            UpdatePursueLocation();
            break;
        }
        case bot_goal::ID_GO_TO_LOCATION:
        {
            UpdateGoToLocation();
            break;
        }

        case bot_goal::ID_PATROL:
        {
            UpdatePatrol();
            break;
        }

        case bot_goal::ID_ENGAGE:
        {
            UpdateEngage();
            break;
        }

        case bot_goal::ID_ROAM:
        {
            UpdateRoam();
            break;
        }
        
        case bot_goal::ID_LOADOUT:
        {
            UpdateLoadout();
            break;
        }
        
        case bot_goal::ID_PICKUP:
        {
            UpdatePickup();
            break;
        }
        
        case bot_goal::ID_MORTAR:
        {
            UpdateMortar();
            break;
        }
        
        case bot_goal::ID_REPAIR:
        {
            UpdateRepair();
            break;
        }
        
        case bot_goal::ID_DESTROY:
        {
            UpdateDestroy();
            break;
        }
        
        case bot_goal::ID_ANTAGONIZE:
        {
            UpdateAntagonize();
            break;
        }
        
        case bot_goal::ID_DEPLOY:
        {
            UpdateDeploy();
            if (m_DeployFailure > 5)
            {
                m_Move.JumpKey = TRUE;
                m_Move.MoveY = -1;
            }
            break;
        }
        
        case bot_goal::ID_SNIPE:
        {
            UpdateSnipe();
            break;
        }
        
        case bot_goal::ID_NO_GOAL:
        default:
            break;
        }

        //--------------------------------------------------
        // Check to see if we've just become "ill"
        //--------------------------------------------------
        if ( m_AmHealthy
            && GetHealth() < 0.5f
            && x_frand( 0, 1 ) >= (1 - m_Difficulty) )
        {
            m_Move.HealthKitKey = TRUE;
        }

        if ( GetHealth() < 0.5f )
        {
            m_AmHealthy = FALSE;
        }
        else
        {
            m_AmHealthy = TRUE;
        }

        //--------------------------------------------------
        // Check to see if we should toss a flare
        //--------------------------------------------------
        if( (m_MissileTargetLockCount) &&
            (m_WillThrowGrenades) &&
            (m_FlareTimer.ReadSec() > 1.0f) &&
            (m_Loadout.Inventory.Counts[INVENT_TYPE_GRENADE_FLARE] > 0) &&
            (m_GrenadeKeyTime == 0) )
        {
            m_Move.GrenadeKey = TRUE;
            m_FlareTimer.Reset();
            m_FlareTimer.Start();
        }

        if ( !m_Aiming )
        {
            // reset our aim fudge stuff so we don't reacquire too fast
            m_AimFudgeTargetID = obj_mgr::NullID;
        }

#if TESTING_VANTAGE_POINTS
        s32 i;
        for ( i = 0; i < g_nVantagePoints; ++i )
        {
            DrawSphere( g_Graph.m_pNodeList[g_pVantagePoints[i].NodeID].Pos
                , 1.0f, XCOLOR_YELLOW );
            DrawLine( g_Graph.m_pNodeList[g_pVantagePoints[i].NodeID].Pos
                , g_pVantagePoints[i].TargetWorldPos
                , g_pVantagePoints[i].IsMortarPoint
                ? XCOLOR_BLUE : XCOLOR_GREEN );
        }
#endif
        
#if TIMING_UPDATE_INPUT
        const f32 updateTime = timer.ReadMs();
        totalInputUpdateTime += updateTime;
        ++numInputUpdateTimeSamples;
            
        if ( updateTime > maxInputUpdateTime )
        {
            maxInputUpdateTime = updateTime;
        }

        if ( worldTimeForInputUpdate.ReadSec() > 5.0f )
        {
            worldTimeForInputUpdate.Reset();
            worldTimeForInputUpdate.Start();
            x_printf( "UpdateInput() average: %fms  max: %fms\n"
                , totalInputUpdateTime / numInputUpdateTimeSamples
                , maxInputUpdateTime );
        }
#endif
    }
    else
    {
        // we're not doing a full update, but we should do some maintenance

        if ( !((m_CurrentGoal.GetGoalID() == bot_goal::ID_ENGAGE)
            && (m_CurrentGoal.GetGoalState()
                == bot_goal::STATE_ENGAGE_FIGHTING))
            && !m_DontClearMove )
        {
            vector3 HorizVel = m_Vel;
            HorizVel.Y = 0.0f;
            vector3 HorizRot( m_Rot );
            HorizRot.Y = 0.0f;
            
            if ( v3_AngleBetween( HorizVel, HorizRot ) <= R_10 )
            {
                // clear the left-right push on the controls to eliminate
                // wobbling
                m_Move.MoveX = 0.0f;
            }
        }
    }

#if BOT_UPDATE_TIMING
    g_BotTiming.UpdateTimer.Stop();
    m_BotTiming.UpdateTimer.Stop();
#endif
}



//==============================================================================
// PursueLocation()
//==============================================================================

xtimer BotPursueLocTimer[10];

void bot_object::PursueLocation(
    const vector3& TargetLocation
    , const vector3& NextTargetLocation
    , xbool Arrive /* = FALSE */
    , xbool ForceAerial /* = FALSE */ )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

BotPursueLocTimer[0].Start();
    if( g_BotDebug.DrawNavigationMarkers )
    {
        const xcolor Color = m_PathKeeper.BotShouldBeInAir()
            ? XCOLOR_BLUE : XCOLOR_GREEN;
        DrawSphere( TargetLocation, 1.0f, Color );
        if ( m_PathKeeper.FollowingTightEdge() )
        {
            DrawSphere( TargetLocation, 0.5f, Color );
        }
        draw_Label( TargetLocation, Color, "Next Pos" );

        if ( m_CurTask && (m_CurTask->GetID() == bot_task::TASK_TYPE_REPAIR) )
        {
            // draw the path
            s32 i;
            xarray<u16>& EdgeIdxs = m_PathKeeper.GetEdgeIdxs();
            for ( i = 0; i < EdgeIdxs.GetCount(); ++i )
            {
                const s32 NodeA = g_Graph.m_pEdgeList[EdgeIdxs[i]].NodeA;
                const s32 NodeB = g_Graph.m_pEdgeList[EdgeIdxs[i]].NodeB;
                const vector3 PosA = g_Graph.m_pNodeList[NodeA].Pos;
                const vector3 PosB = g_Graph.m_pNodeList[NodeB].Pos;
                DrawLine( PosA, PosB, XCOLOR_WHITE );
            }
        }
        
        
        DrawLine( m_PathKeeper.GetPrevPoint().Pos
            , m_PathKeeper.GetCurrentPoint().Pos
            , XCOLOR_PURPLE );
        DrawLine( m_PathKeeper.GetCurrentPoint().Pos
            , m_PathKeeper.GetNextPoint().Pos
            , XCOLOR_AQUA );

    }

    if ( Arrive
        && ((m_WorldPos - TargetLocation).LengthSquared()
            <= GetArrivalSlowingDist()) )
    {
BotPursueLocTimer[1].Start();
        ArriveAtPosition( TargetLocation );
BotPursueLocTimer[1].Stop();

BotPursueLocTimer[0].Stop();
        return;
    }
    
    //----------------------------------------------------------------------
    // Stay on the path
    //----------------------------------------------------------------------
BotPursueLocTimer[2].Start();
    const vector3 FuturePos = m_WorldPos
        + (m_Vel * GetPathFollowingPredictionTime() );

    // We need to figure out what line segment to consider...
    // If our FuturePos is further than our next path point,
    // then consider the next line segment
    const vector3 ToFuturePos = FuturePos - m_WorldPos;
    const f32 DistToFuturePosSq = ToFuturePos.LengthSquared();
    const f32 DistToCurrentPointSq
        = (TargetLocation - m_WorldPos).LengthSquared();
    const xbool FuturePosPastCurSeg
        =  (TargetLocation - m_PrevTargetLocation).LengthSquared()
        < (FuturePos - m_PrevTargetLocation).LengthSquared();

    // build the segment to consider for comparison with FuturePos
    vector3 SegmentStart;
    vector3 SegmentEnd;

    if ( DistToFuturePosSq > DistToCurrentPointSq
        || FuturePosPastCurSeg )
    {
        SegmentStart = TargetLocation;
        SegmentEnd = NextTargetLocation;
    }
    else
    {
        SegmentStart = m_PrevTargetLocation;
        SegmentEnd = TargetLocation;
    }
BotPursueLocTimer[2].Stop();

    if ( (SegmentStart - SegmentEnd).LengthSquared() < REALLY_CLOSE_SQ )
    {
BotPursueLocTimer[3].Start();
        // There is no segment to consider, so just arrive
        ArriveAtPosition( TargetLocation );
BotPursueLocTimer[3].Stop();
    }
    else
    {
BotPursueLocTimer[4].Start();
        const vector3 Segment = SegmentEnd - SegmentStart;
        vector3 SegDir = Segment;
        SegDir.Normalize();

        vector3 TempBotPos = m_WorldPos; // We'll use this for bot pos...
        
        if ( !ForceAerial && !m_PathKeeper.BotShouldBeInAir() )
        {
            // We need to ignore verticallity so that we don't get stuck
            // trying to fly up to the path when we can't because we're
            // on a ground edge, or trying to go through the ground to get
            // to an underground edge
            //
            // To do this, build a plane that intersects the segment ends,
            // and is otherwise horizontal. Raise or lower TempBotPos such
            // that it intersects this plane.
            vector3 Horiz = Segment.Cross( UP_VEC );
            if( Horiz.LengthSquared() < 0.000001f )
                Horiz = Segment.Cross( vector3(1,0,0) );

            const vector3 ThirdPoint = SegmentStart + Horiz;
            
            plane Plane(SegmentStart, ThirdPoint, SegmentEnd );

            f32 t;

            //arbitrarily long line
            static const f32 HALF_TEST_LINE_LENGTH = 500.0f; 
            if ( Plane.Intersect(
                t
                , TempBotPos + vector3( 0.0f, -HALF_TEST_LINE_LENGTH, 0.0f )
                , TempBotPos + vector3( 0.0f, HALF_TEST_LINE_LENGTH, 0.0f ) ) )
            {
                // we are above or below the plane, so we need to move to it
                const f32 VerticalMove = t * 2 * HALF_TEST_LINE_LENGTH
                    - HALF_TEST_LINE_LENGTH;
                TempBotPos.Y += VerticalMove;
            }
        }
BotPursueLocTimer[4].Stop();
BotPursueLocTimer[5].Start();


        vector3 PursueLocation;

        const vector3 TempFuturePos = TempBotPos
            + (m_Vel * GetPathFollowingPredictionTime() );
        // Determine the location we will seek
        const vector3 SegStartToFuturePos = TempFuturePos - SegmentStart;
        const f32 Projection = ABS( SegStartToFuturePos.Dot( SegDir ) );
        const vector3 ProjectionVec = SegDir * Projection;
        const vector3 FuturePosToSeg = ProjectionVec - SegStartToFuturePos;

        const f32 DisFromSegmentSq = FuturePosToSeg.LengthSquared();

        if (g_BotDebug.ShowTaskDistribution)
        {
            if (this == ExamineBot && !IsDead())
            {
                x_printfxy(4, 36, "DisFromSegmentSq = %f", DisFromSegmentSq);
            }
        }

        if ( DisFromSegmentSq > GetPathFollowingDistanceSQ() )
        {
            // we will be off the path, so we need to pursue a location
            // on the path...

            // project our position onto the segment, and determine
            // how far down the path to go from that point by multiplying
            // our speed by some amount of time
            const vector3 SegmentStartToBot = TempBotPos - SegmentStart;
            const f32 ProjDist = SegmentStartToBot.Dot( SegDir );
            const f32 MoveDist = MAX( 5.0f, m_Vel.Length()) * GetPathFollowingPursueTime();
            vector3 MoveDir = Segment;
            MoveDir.Normalize();
            PursueLocation = SegmentStart + MoveDir * ProjDist;
            MovePointAlongSegment( PursueLocation
                , SegmentStart
                , SegmentEnd
                , MoveDist );

            if( g_BotDebug.DrawSteeringMarkers )
            {
                DrawLine( FuturePos, PursueLocation, XCOLOR_RED );
            }
        }
        else
        {
            // if we're moving noticeably, keep moving in this direction
            if ( m_Vel.LengthSquared() > 1.0f )
            {
                vector3 PursueDelta = m_Vel;
                PursueDelta.Normalize();
                PursueDelta *= GetMaxSpeed();
                PursueLocation = TempBotPos + PursueDelta;
            }
            else
            {
                // start moving in the right direction
                PursueLocation = TargetLocation;
            }
        }
BotPursueLocTimer[5].Stop();
        
        // Determine if we need a rubber band pull to get back on path
        if ( m_PathKeeper.FollowingTightEdge() )
        {
BotPursueLocTimer[6].Start();

            // Find the component of our velocity away from the edge
            // and apply a scaled amount of accelration
            // in the opposite direction.
            vector3 EdgeStart;
            vector3 EdgeEnd;

            if ( m_PathKeeper.GetNumEdges() > 0 )
            {
                EdgeStart = m_PathKeeper.GetPrevPoint().Pos;
                EdgeEnd = m_PathKeeper.GetCurrentPoint().Pos;
            }
            else
            {
                EdgeStart = EdgeEnd = TargetLocation;
            }

            const vector3 Edge = EdgeEnd - EdgeStart;
            const vector3 EdgeStartToWorldPos = TempBotPos - EdgeStart;
            vector3 EdgeDir = Edge;

            if( EdgeDir.Normalize() )
            {
                const f32 Proj = ABS( EdgeStartToWorldPos.Dot( EdgeDir ) );
                const vector3 ProjVec = EdgeDir * Proj;
    /*
                const f32 DistFromPathSq
                    = (EdgeStartToWorldPos - ProjVec).LengthSquared();
    */
                const vector3 WorldPosToEdge = ProjVec - EdgeStartToWorldPos;

                vector3 AwayFromEdgeDir = -WorldPosToEdge;
                if( AwayFromEdgeDir.Normalize() )
                {
                    const f32 SpeedAwayFromEdge = m_Vel.Dot( AwayFromEdgeDir );

                    if ( SpeedAwayFromEdge > 0 )
                    {
                        // we're going the wrong way, so we need to correct
                        // just eliminate the "bad" velocity and keep the rest
                        m_CheatDeltaVelocity = AwayFromEdgeDir * -SpeedAwayFromEdge;
                    }
                }
            }
BotPursueLocTimer[6].Stop();

        }
BotPursueLocTimer[7].Start();

        // if we're supposed to be on the ground, move PursueLocation
        // back from TempBotPos to m_WorldPos
        if ( !ForceAerial || !m_PathKeeper.BotShouldBeInAir() )
        {
            PursueLocation -= TempBotPos - m_WorldPos;
        }

        if( g_BotDebug.DrawSteeringMarkers )
        {
            DrawSphere( FuturePos, 0.5f, XCOLOR_RED );
            DrawSphere( PursueLocation, 1.5f, XCOLOR_AQUA );
        }

        SeekPosition( PursueLocation );
BotPursueLocTimer[7].Stop();

        if ( !m_PathKeeper.BotShouldBeInAir() &&
            !m_PathKeeper.FollowingTightEdge() &&
            !m_PathKeeper.FollowingSkiEdge() )
        {
            //--------------------------------------------------
            // move FAST...
            //--------------------------------------------------
BotPursueLocTimer[8].Start();

            // first, determine where we are in relation to the current edge
            // and where we're going in relation to it's horizontal direction
            static const f32 MAX_DIST_TO_VPLANE = 5.0f;
            static const radian MAX_DIRECTION_DIFF = R_10;
            static const f32 MIN_TIME_LEFT_TO_FLY = 1.5f; // seconds
            static const f32 MIN_ENERGY_TO_START_JETTING = 0.5f;
            static const f32 MIN_ENERGY_TO_KEEP_JETTING = 0.3f;
            static const f32 MAX_ALTITUDE = 10.0f;
            static const f32 MAX_VERTICAL_VELOCITY = 2.0f;
            
            const graph::edge& Edge = m_PathKeeper.GetCurrentEdge();
            const vector3& NodeAPos = g_Graph.m_pNodeList[Edge.NodeA].Pos;
            const vector3& NodeBPos = g_Graph.m_pNodeList[Edge.NodeB].Pos;

            const plane VPlane( NodeAPos, NodeBPos, NodeAPos + UP_VEC );
            const f32 DistToVPlane = x_abs( VPlane.Distance( m_WorldPos ) );

            const vector3& CurPoint = m_PathKeeper.GetCurrentPoint().Pos;
            
            vector3 RightDir = CurPoint - m_WorldPos;
            RightDir.Normalize();
            f32 RightDirDY = RightDir.Y;
            vector3 GoingDir = m_Vel;
            RightDir.Y = 0.0f;
            GoingDir.Y = 0.0f;

            const radian DirectionDiff = v3_AngleBetween( GoingDir, RightDir );
            const f32 TerrHeight
                = m_pTerr->GetHeight( m_WorldPos.Z, m_WorldPos.X );
            const f32 GroundDistance
                = (vector3( m_WorldPos.X, TerrHeight, m_WorldPos.Z) - CurPoint)
                .Length();
            const f32 Speed = m_Vel.Length();
            
            const xbool HaveRoom = GroundDistance
                > (Speed * MIN_TIME_LEFT_TO_FLY);

            if ( (DistToVPlane <= MAX_DIST_TO_VPLANE)
                && ((DirectionDiff < MAX_DIRECTION_DIFF) || (RightDirDY > STEEP_DY)) )
            {
                // ok, close enough. Is jetting appropriate?
                if ( HaveRoom && OnGround()
                    && (m_Energy > MIN_ENERGY_TO_START_JETTING) )
                {
                    // start hovering
                    m_Move.JumpKey = TRUE;
                    m_Move.JetKey = TRUE;
                }
                else if ( !OnGround() )
                {
                    vector3 HVec = (NodeBPos - NodeAPos).Cross( UP_VEC );
                    const plane HPlane( NodeAPos, NodeBPos, NodeAPos + HVec );
                    const f32 DistToHPlane = HPlane.Distance( m_WorldPos );
                    
                    if ( HaveRoom && (m_Energy > MIN_ENERGY_TO_KEEP_JETTING)
                        && (((m_WorldPos.Y - TerrHeight) < MAX_ALTITUDE)
                            || (DistToHPlane < MAX_ALTITUDE))
                        && (m_Vel.Y < MAX_VERTICAL_VELOCITY) )
                    {
                        // keep hovering
                        m_Move.JetKey = TRUE;
                    }
                }
            }
BotPursueLocTimer[8].Stop();
        }
    }
BotPursueLocTimer[0].Stop();
}


//==============================================================================
// FaceLocation()
//==============================================================================
void bot_object::FaceLocation( const vector3& TargetLocation )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    FaceDirection( TargetLocation - GetWeaponPos() );
}

//==============================================================================
// FaceDirection()
//==============================================================================
void bot_object::FaceDirection( const vector3& Direction )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    m_FaceDirection = Direction;
    m_FaceDirection.Normalize();
    ASSERT( m_FaceDirection.IsValid() );
    m_FaceLocation = GetMuzzlePosition() + (m_FaceDirection * 100.0f );
}


//==============================================================================
// UpdateFaceLocation()
//==============================================================================
s32 FRAMES_PER_AIM = 5;
void bot_object::UpdateFaceLocation( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_CurrentGoal.GetGoalID() == bot_goal::ID_NO_GOAL ) return;

    if ( m_Aiming
        && ((g_BotFrameCount % FRAMES_PER_AIM)
            == ((m_Num + 1) % FRAMES_PER_AIM)) )
    {
        const object* pTarget = ObjMgr.GetObjectFromID( m_AimingTarget );

        if ( pTarget )
        {
            AimAtTarget( *pTarget );
        }
    }
    
    if ( g_BotDebug.DrawAimingMarkers )
    {
        DrawSphere( m_WorldPos + (m_FaceDirection * 20.0f)
            , 0.5f
            , XCOLOR_GREEN );
    }

    //
    // Move in Yaw
    //
    {
        radian DeltaAngle
            = radian_DeltaAngle( m_Rot.Yaw, m_FaceDirection.GetYaw() );

        const f32 MaxYawSpeed = MIN_FACE_YAW_SPEED + (m_Difficulty * (MAX_FACE_YAW_SPEED - MIN_FACE_YAW_SPEED));
        const f32 YawSpeed = MIN( x_abs(DeltaAngle / m_DeltaTime), MaxYawSpeed );
        const f32 YawDelta  = (DeltaAngle>=0) 
                            ? (YawSpeed * m_DeltaTime) 
                            : (-YawSpeed * m_DeltaTime );

        m_Rot.Yaw += YawDelta;
    }

    //
    // Move in Pitch
    //
    {
        radian DeltaAngle
            = radian_DeltaAngle( m_Rot.Pitch, m_FaceDirection.GetPitch() );

        const f32 MaxPitchSpeed = MIN_FACE_PITCH_SPEED + (m_Difficulty * (MAX_FACE_PITCH_SPEED - MIN_FACE_PITCH_SPEED));
        const f32 PitchSpeed = MIN( x_abs(DeltaAngle / m_DeltaTime), MaxPitchSpeed );
        const f32 PitchDelta  = (DeltaAngle>=0) 
                            ? (PitchSpeed * m_DeltaTime) 
                            : (-PitchSpeed * m_DeltaTime );

        m_Rot.Pitch += PitchDelta;
    }
}

//==============================================================================
// AmNear()
//==============================================================================
xbool bot_object::AmNear( const vector3& Point ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    return IsNear( Point, m_WorldPos );
}

//==============================================================================
// IsNear()
//==============================================================================
xbool bot_object::IsNear( const vector3& Point1
    , const vector3& Point2 ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    return (Point1 - Point2).LengthSquared() <= CLOSE_ENOUGH_SQ;
}

//==============================================================================
// HaveArrivedAt()
//==============================================================================
xbool bot_object::HaveArrivedAt( const vector3& Point )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    return ((Point - m_WorldPos).LengthSquared() <= ARRIVAL_CLOSE_ENOUGH_SQ)
        || (!m_PathKeeper.BotShouldBeInAir() && HaveArrivedAbove( Point ));
}

//==============================================================================
// HaveArrivedAbove()
//==============================================================================
xbool bot_object::HaveArrivedAbove( const vector3& Point ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    vector3 Difference = Point - m_WorldPos;

    if ( Difference.Y > 0.0f )
    {
        return FALSE;
    }
    
    Difference.Y = 0.0f;
    return Difference.LengthSquared() < ARRIVAL_CLOSE_ENOUGH_SQ;
}

//==============================================================================
// HavePassedPoint
//==============================================================================
xbool bot_object::HavePassedPoint( const vector3& Point
    , const vector3& PrevPoint
    , const vector3& NextPoint ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( (Point - PrevPoint).LengthSquared() < 0.01f )
    {
        return FALSE;
    }
    
    vector3 PointToUsDir = m_WorldPos - Point;
    PointToUsDir.Normalize();

    vector3 PointToPrevDir = PrevPoint - Point;
    PointToPrevDir.Normalize();

    if ( x_acos( PointToPrevDir.Dot( PointToUsDir ) ) > R_90 )
    {
        return TRUE;
    }
    
    if ( (Point - NextPoint).LengthSquared() < 0.01f )
    {
        return FALSE;
    }

#if 1 // This seems to cause an advance too early on tight corners
    //----------------------------------------------------------------------
    // Set up a plane that angularly bisects the two segments, and
    // see which side of the plane we're on.
    //----------------------------------------------------------------------
    vector3 PointToNextDir = NextPoint - Point;
    PointToNextDir.Normalize();
    const vector3 MidPoint = Point + PointToNextDir + PointToPrevDir;
    const vector3 PerpPoint = Point + PointToPrevDir.Cross( PointToNextDir );

    // now use Point, PerpPoint, and MidPoint to define the plane
    plane TestPlane( Point, MidPoint, PerpPoint );

    return TestPlane.InBack( m_WorldPos );
#else
    return FALSE;
#endif
}

//==============================================================================
// Steer()
//==============================================================================
void bot_object::Steer( const vector3& Steering )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    // Check to see if we want to go up
    if ( Steering.Y > 0.0f )
    {
        m_Move.JetKey = TRUE;

        if ( m_ContactInfo.RunSurface
            && Steering.Y > (ABS(Steering.X) + ABS(Steering.Y)) / 2  ) 
        {
            m_Move.JumpKey = TRUE;
        }
    }
    else
    {
        m_Move.JetKey = FALSE;
    }

    // Check to see if we want to ski
    if ( m_PathKeeper.FollowingSkiEdge()
        && (m_WorldPos.Y > m_PathKeeper.GetCurrentPoint().Pos.Y) ) // going down
    {
        m_CurrentlySkiing = TRUE;
        m_Move.JumpKey = TRUE;
    }
    else
    {
        m_CurrentlySkiing = FALSE;
    }
    
    
    // Set up the controller in the direction of the horizontal
    // component of Steering
    vector3 HorizSteering = Steering;
    HorizSteering.Y = 0.0f;

    if ( HorizSteering.LengthSquared() > 0.0f )
    {
        // If we're on the ground, and need to reverse direction,
        // just release that direction's button for now

#if 0
        if ( OnGround() )
        {
            if ( (HorizSteering.X < 0.0f && m_Vel.X > 0.0f)
                || (HorizSteering.X > 0.0f && m_Vel.X < 0.0f) )
            {
                HorizSteering.X = 0.0f;
            }
            
            if ( (HorizSteering.Z < 0.0f && m_Vel.Z > 0.0f)
                || (HorizSteering.Z > 0.0f && m_Vel.Z < 0.0f) )
            {
                HorizSteering.Z = 0.0f;
            }
        }
#endif
        
        // Normalize
        HorizSteering.Normalize();
        HorizSteering.RotateY( -m_Rot.Yaw );

        // Handle rounding errors to keep us <= 1.0 magnitude
        HorizSteering *= 0.9999f;
        
        // Move
        m_Move.MoveX
            = (HorizSteering.X > 0)
            ? MIN( 1.0f, HorizSteering.X )
            : MAX( -1.0f, HorizSteering.X );
        m_Move.MoveY
            = (HorizSteering.Z > 0)
            ? MIN( 1.0f, HorizSteering.Z )
            : MAX( -1.0f, HorizSteering.Z );
    }
}

//==============================================================================
// SeekPosition()
//==============================================================================
void bot_object::SeekPosition( const vector3& SeekPos )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    vector3 DesiredDir = SeekPos - m_WorldPos;
    if( DesiredDir.LengthSquared() < 0.00001f )
        return;

    DesiredDir.Normalize();
    vector3 DesiredVelocity = DesiredDir*GetMaxSpeed();
    vector3 Steering;

    
    object* pDangerousTurret = NULL;
    //if( x_irand(0,100) < 5 )
    //    pDangerousTurret = GetDangerousTurret();
    
    if ( pDangerousTurret != NULL
        && !m_PathKeeper.FollowingTightEdge()
        && m_TurretDodgeTimer.ReadSec() >= 0.5f ) // start after .5 sec
    {
        // We're dodging
        
        if ( m_TurretDodgeTimer.ReadSec() >= 1.0f ) // keep going 'til 1 secs
        {
            // stop dodging
            m_TurretDodgeTimer.Reset();
            m_TurretDodgeTimer.Start();
        }
        else
        {
            // keep dodging
            vector3 HorizSeekDir = SeekPos - m_WorldPos;
            HorizSeekDir.Y = 0.0f;
            
            vector3 HorizTurretDelta
                = m_WorldPos - pDangerousTurret->GetPosition();
            HorizTurretDelta.Y = 0.0f;

            vector3 Try1 = HorizTurretDelta;
            Try1.RotateY( R_90 );

            const vector3 Try2 = -Try1;

            if ( v3_AngleBetween( Try1, HorizSeekDir )
                > v3_AngleBetween( Try2, HorizSeekDir ) )
            {
                Steering = Try1;
            }
            else
            {
                Steering = Try2;
            }

            Steering.Y = 10.0f; // force jets
        }
    }
    else
    {
        const xbool ShouldBeInAir = m_PathKeeper.BotShouldBeInAir();
        if ( ShouldBeInAir )
        {
            DesiredVelocity.Y += 1.5f; // a little cheat to make sure we stay high
        }

        Steering = DesiredVelocity - m_Vel;

        const xbool Steep = (DesiredDir.Y >= STEEP_DY);
        //const xbool Steep
        //    = x_atan( DesiredVelocity.Y
        //        / x_sqrt( x_sqr( DesiredVelocity.X ) + x_sqr( DesiredVelocity.Z ) ) )
        //    >= STEEP;

        if ( !ShouldBeInAir && !Steep ) {
            Steering.Y = 0.0f;
        }
    }

    if( g_BotDebug.DrawSteeringMarkers ) 
    {
        DrawLine( m_WorldPos + CHEST_HEIGHT, m_WorldPos + DesiredVelocity + CHEST_HEIGHT, XCOLOR_WHITE );
        DrawLine( m_WorldPos + CHEST_HEIGHT, m_WorldPos + m_Vel + CHEST_HEIGHT, XCOLOR_GREEN );
        DrawLine( m_WorldPos + CHEST_HEIGHT, m_WorldPos + Steering + CHEST_HEIGHT, XCOLOR_BLUE );
    }

    Steer( Steering );
}

//==============================================================================
// ArriveAtPosition()
//==============================================================================
void bot_object::ArriveAtPosition( const vector3& ArrivalPos )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    vector3 TargetOffset = ArrivalPos - m_WorldPos;
    const f32 Distance = TargetOffset.Length();
    TargetOffset *= 1.0f / Distance;
    const f32 MaxSpeed = GetMaxSpeed();
    ASSERT( MaxSpeed >= 0.0f );
    
    const f32 SlowingDist = GetArrivalSlowingDist();
    ASSERT( SlowingDist >= 0.0f );
    
    const f32 RampedSpeed = Distance * (MaxSpeed / SlowingDist);
    const f32 ClippedSpeed = MIN( RampedSpeed, MaxSpeed );
    vector3 DesiredVelocity = ClippedSpeed * TargetOffset;

#if 0 // not sure what this used to do, but it does nothing now
    if ( v3_AngleBetween( DesiredVelocity, TargetOffset ) > R_90 )
    {
        // Find the direction closest desired that is 90 deg off of m_Vel
        const vector3 Perp = DesiredVelocity.Cross( TargetOffset );
        vector3 Dir = TargetOffset.Cross( Perp );
        Dir.Normalize();

        // Project desired onto dir
        const f32 d = DesiredVelocity.Dot( Dir );
        Dir *= d;
    }
#endif
    
    vector3 Steering = DesiredVelocity - m_Vel;

    // Since DesiredVelocity's direction comes from TargetOffset we can
    // use the unit vector target offset.
    const xbool Steep = (TargetOffset.Y >= STEEP_DY);

    //const xbool Steep
    //    = x_atan( DesiredVelocity.Y
    //        / x_sqrt( x_sqr( DesiredVelocity.X ) + x_sqr( DesiredVelocity.Z ) ) )
    //        >= STEEP;

    if ( Steering.Y > 0.0f
        && !m_PathKeeper.BotShouldBeInAir()
        && !Steep)
    {
        Steering.Y = 0.0f;
    }

    if( Steering.LengthSquared() > 0.00001f )
    {
        // scale according to SlowingDist
        Steering.Normalize();
        Steering *= MIN( Distance / SlowingDist, 1.0f );

        if ( g_BotDebug.DrawSteeringMarkers )
        {
            DrawSphere( ArrivalPos, 1.5f, XCOLOR_AQUA );
            DrawLine( m_WorldPos + vector3( 0, 1.0f, 0 )
                , m_WorldPos + DesiredVelocity + vector3( 0, 1.0f, 0 )
                , XCOLOR_WHITE );
        }
    
        Steer( Steering );
    }
}

//==============================================================================
// MovePointAlongSegment()
//==============================================================================
void bot_object::MovePointAlongSegment(
    vector3& Point
    , const vector3& SegmentStart
    , const vector3& SegmentEnd
    , f32 Dist ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    const vector3 Segment = SegmentEnd - SegmentStart;
    if ( Segment.LengthSquared() < (Dist * Dist) )
    {
        Point = SegmentEnd;
    }
    else
    {
        vector3 Move = Segment;
        Move.Normalize();
        Move *= Dist;
        Point += Move;
    }
}

//==============================================================================
// GetMaxSpeed()
// Determines the maximum speed to be used based on whether we should be
// on the ground or in the air, or we're following a tight or loose edge
//==============================================================================

f32 bot_object::GetMaxSpeed( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_PathKeeper.BotShouldBeInAir() )
    {
        // Aerial
        if ( m_PathKeeper.FollowingTightEdge() )
        {
            // Tight
            return TIGHT_AERIAL_MAX_SPEED;
        }
        else
        {
            // Loose
            return AERIAL_MAX_SPEED;
        }
    }
    else
    {
        // Ground
        if ( m_PathKeeper.FollowingTightEdge() )
        {
            // Tight
            return TIGHT_GROUND_MAX_SPEED;
        }
        else
        {
            // Loose
            return GROUND_MAX_SPEED;
        }
    }
}

//==============================================================================
// GetArrivalSlowingDist()
// Determines the maximum speed to be used based on whether we should be
// on the ground or in the air, or we're following a tight or loose edge
//==============================================================================

f32 bot_object::GetArrivalSlowingDist( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_PathKeeper.BotShouldBeInAir() )
    {
        return AERIAL_ARRIVAL_SLOWING_DIST;
    }
    else
    {
        return GROUND_ARRIVAL_SLOWING_DIST;
    }
}

//==============================================================================
// GetPathFollowingPredictionTime()
//==============================================================================

f32 bot_object::GetPathFollowingPredictionTime( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_PathKeeper.BotShouldBeInAir() )
    {
        if ( m_PathKeeper.FollowingTightEdge() )
        {
            return PATH_FOLLOWING_TIGHT_AIR_PREDICTION_TIME;
        }
        else
        {
            return PATH_FOLLOWING_LOOSE_AIR_PREDICTION_TIME;
        }
        
    }
    else
    {
        return PATH_FOLLOWING_GROUND_PREDICTION_TIME;
    }
}


//==============================================================================
// GetPathFollowingPursueTime()
//==============================================================================

f32 bot_object::GetPathFollowingPursueTime( void ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_PathKeeper.FollowingTightEdge() )
    {
        return PATH_FOLLOWING_TIGHT_PURSUE_TIME;
    }
    else
    {
        return PATH_FOLLOWING_LOOSE_PURSUE_TIME;
    }
}

//==============================================================================
// GetPathFollowingDistanceSQ()
//==============================================================================

f32 bot_object::GetPathFollowingDistanceSQ( void ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_PathKeeper.FollowingTightEdge() )
    {
        return PATH_FOLLOWING_DISTANCE_SQ;
    }
    else
    {
        return PATH_FOLLOWING_LOOSE_DISTANCE_SQ;
    }
}


//==============================================================================
// UpdatePursueObject()
//==============================================================================
void bot_object::UpdatePursueObject( xbool FaceGoToLocation /* = TRUE */ )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if (m_CurrentGoal.GetTargetObject())
    {
        const object& Object = *(m_CurrentGoal.GetTargetObject());
        UpdatePursue( Object.GetPosition() + vector3( 0, 0.1f, 0 )
        , FaceGoToLocation );
    }
}

//==============================================================================
// UpdatePursueLocation()
//==============================================================================
void bot_object::UpdatePursueLocation( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( !m_CurTask )
    {
        ASSERT( m_CurTask );
        x_DebugLog( "Bot: UpdatePursueLocation called with null m_CurTask\n" );
        return;
    }

    UpdatePursue( m_CurTask->GetTargetPos() );
}

//==============================================================================
// UpdatePursue()
//==============================================================================
void bot_object::UpdatePursue( const vector3& Position
    , xbool FaceGoToLocation /* = TRUE */ )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( g_BotDebug.DrawNavigationMarkers )
    {
        DrawSphere( Position, 1.0f, XCOLOR_WHITE );
        draw_Label( Position, XCOLOR_WHITE, "Pursue" );
    }
    
    xbool KeepGoing;

    // check to see if the target has moved too far from the goal
    const f32 DistToGoalSQ = (m_WorldPos - m_CurrentGoal.GetTargetLocation())
        .LengthSquared();
    const f32 GoalToTargetDistSQ = ( m_CurrentGoal.GetTargetLocation()
        - Position).LengthSquared();

    if ( GoalToTargetDistSQ > (DistToGoalSQ/2) )
    {
        KeepGoing = FALSE;
    }
    else
    {
        // check to see if our goal's goto location can still see the target
        const vector3 TempPos = m_WorldPos;
        m_WorldPos = Position;
        KeepGoing
            =  AmNear( m_CurrentGoal.GetTargetLocation() )
            || CanSeePoint( m_CurrentGoal.GetTargetLocation() );
        m_WorldPos = TempPos;
    }
    
    if ( !KeepGoing )
    {
        // we won't be able to see it from there, so reinit
        const bot_goal::goal_id CurGoalID = m_CurrentGoal.GetGoalID();
        InitializeGoTo( Position, TRUE );        
        m_CurrentGoal.SetGoalID( CurGoalID );
    }
        
    UpdateGoToLocation( FaceGoToLocation );
}

//==============================================================================
// UpdateGoToLocation()
//==============================================================================

xtimer BotUpdateGoToLocTimer[10];
void bot_object::UpdateGoToLocation( xbool FaceThisLocation /* = TRUE */ )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    m_GoingSomewhere = TRUE;
BotUpdateGoToLocTimer[0].Start();
    xbool AntagonizingEnemies = FALSE;

    if ( g_BotDebug.DrawNavigationMarkers )
    {
        DrawSphere( m_CurrentGoal.GetTargetLocation(), 1.0f, XCOLOR_PURPLE );
        char ObjIDLabelStr[30] = "";
        x_strcpy( ObjIDLabelStr, xfs( "%d/%d\n", m_ObjectID.Slot, m_ObjectID.Seq ) );
        draw_Label( m_CurrentGoal.GetTargetLocation(), XCOLOR_PURPLE, "%s", ObjIDLabelStr );
    }

BotUpdateGoToLocTimer[1].Start();
    AntagonizingEnemies = AntagonizeEnemies();
BotUpdateGoToLocTimer[1].Stop();
    
//    if ( m_DangerAlert == TRUE )
//    {
//        return; 
//    }
    
    
    if ( HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) )
    {
        // We don't need to do anything
        m_CurrentGoal.SetGoalID(bot_goal::ID_NO_GOAL);
#if EDGE_COST_ANALYSIS
        if (g_bCostAnalysisOn)
        {
            m_PathKeeper.AnalyzeCost();
        }
#endif
BotUpdateGoToLocTimer[0].Stop();
        return;
    }
        
    if ( !m_PathKeeper.IsWaiting() )
    {
BotUpdateGoToLocTimer[2].Start();

        // check to see if we've gotten closer to current point
#if 0 // mar 7/28/02 -- no longer just a horiz test so we can get off the top of stuff
        vector3 WorldHoriz = m_WorldPos;
        WorldHoriz.Y = 0;
        vector3 CurPointHoriz = m_PathKeeper.GetCurrentPoint().Pos;
        CurPointHoriz.Y = 0;
#endif
        
        const f32 DistToCurPointSQ = (m_WorldPos - m_PathKeeper.GetCurrentPoint().Pos).LengthSquared();
        
        if ( DistToCurPointSQ < (m_ClosestDistToNextPointSQ - x_sqr( 0.1f )) )
        {
            // ok compare speeds
            const f32 Dist = (x_sqrt( m_ClosestDistToNextPointSQ ) - x_sqrt( DistToCurPointSQ ));
            const f32 Speed = Dist / m_DeltaTime;

            if ( Speed > 0.5f )
            {
                // we have gotten closer, record the new distSQ
                // and reset the timer
                m_ClosestDistToNextPointSQ = DistToCurPointSQ;
                m_DistToNextPointTimer.Reset();
                m_DistToNextPointTimer.Start();
            }
        }
            
        // If we are too far from our current point, or we have gotten
        // stuck, replot the course
        vector3 FlatDeltaPos
            = (m_PathKeeper.GetCurrentPoint().Pos - m_WorldPos);
        FlatDeltaPos.Y = 0;
BotUpdateGoToLocTimer[2].Stop();
                
        if ( m_DistToNextPointTimer.ReadSec() > PURSUE_TIMEOUT_TIME_SEC )
        {
BotUpdateGoToLocTimer[3].Start();

//            if (!g_bCostAnalysisOn)
                m_PathKeeper.BlockCurrentEdge((m_TeamBits == 1) ? graph::EDGE_FLAGS_TEAM_0_BLOCK
                    : (m_TeamBits == 2) ? graph::EDGE_FLAGS_TEAM_1_BLOCK 
                    : graph::EDGE_FLAGS_TEAM_0_BLOCK | graph::EDGE_FLAGS_TEAM_1_BLOCK);

            const bot_goal::goal_id CurGoalID = m_CurrentGoal.GetGoalID();

            InitializeGoTo( m_CurrentGoal.GetTargetLocation() );

            // Make sure we don't overrun any goals other than GOTO
            if ( m_CurrentGoal.GetGoalID() != CurGoalID )
            {
                m_CurrentGoal.SetGoalID( CurGoalID );
            }

            const xbool TargetNearCurrentPoint
                = IsNear( m_CurrentGoal.GetTargetLocation()
                    , m_PathKeeper.GetCurrentPoint().Pos );

            PursueLocation( m_PathKeeper.GetCurrentPoint().Pos
                , m_PathKeeper.GetNextPoint().Pos
                , TargetNearCurrentPoint );
BotUpdateGoToLocTimer[3].Stop();
        }
        else
        {
BotUpdateGoToLocTimer[4].Start();
            // If we get to or overshoot a path point, move
            // on to the next point
            if ( AmNear( m_PathKeeper.GetCurrentPoint().Pos )
                || ((m_PathKeeper.GetCurrentPoint().Pos
                    - m_PathKeeper.GetNextPoint().Pos).LengthSquared()
                    > 1.0f
                    && HavePassedPoint( m_PathKeeper.GetCurrentPoint().Pos
                        , m_PathKeeper.GetPrevPoint().Pos
                        , m_PathKeeper.GetNextPoint().Pos )) )
                //|| AmNearlyAbove(m_PathKeeper.GetCurrentPoint().Pos))
                // MAR 6/11: I don't think nearly above is a reason
                // to advance any more, because of interior
                // fall-through paths...
            {
                // update m_PrevTargetLocation
                m_PrevTargetLocation
                    = m_PathKeeper.GetCurrentPoint().Pos;

                // advance our path point
                m_PathKeeper.AdvancePoint();

                // reset our stuck detection while we're on a new edge
                m_JettedWhileStuck = FALSE;
                m_BackedUpWhileStuck = FALSE;

                // start tracking our progress along this edge
                m_ClosestDistToNextPointSQ = F32_MAX;
                m_DistToNextPointTimer.Reset();
                m_DistToNextPointTimer.Start();
            }
BotUpdateGoToLocTimer[4].Stop();
BotUpdateGoToLocTimer[5].Start();

            const xbool TargetNearCurrentPoint
                = IsNear( m_CurrentGoal.GetTargetLocation()
                    , m_PathKeeper.GetCurrentPoint().Pos );

            PursueLocation( m_PathKeeper.GetCurrentPoint().Pos
                , m_PathKeeper.GetNextPoint().Pos
                , TargetNearCurrentPoint );

            if ( m_DistToNextPointTimer.ReadSec() > STUCK_TIME_SEC )
            {
                if ( !m_JettedWhileStuck && m_ContactInfo.JumpSurface )
                {
                    // Try jump/jetting over first
                    m_JettedWhileStuck = TRUE;
                    m_LastJetStuckTimer.Reset();
                    m_LastJetStuckTimer.Start();
                    m_Move.JumpKey = TRUE;
                    m_Move.JetKey = TRUE;

                    // Try some back stick?
                    if ( x_irand( 0, 100 ) > 50 )
                    {
                        m_Move.MoveX = m_StuckMoveX;
                        m_Move.MoveY = -1;
                    }
                }
                else
                {
                    // Should we back up a bit?
                    if ( !m_BackedUpWhileStuck && OnGround() )
                    {
                        // Move back at full-speed
                        m_BackedUpWhileStuck = TRUE;
                        m_Move.MoveY = -1;
                    }
                    else
                    {
                        // Move forward at half-speed
                        m_Move.MoveY = 0.3f;
                    }
                    
                    // Try moving around
                    m_Move.MoveX = m_StuckMoveX;

                    // Should we jet again?
                    if ( m_LastJetStuckTimer.ReadSec() > 2.5f )
                    {
                        // this will cause the jet
                        m_JettedWhileStuck = FALSE;

                        // pick a new dir to dodge next time
                        m_StuckMoveX = -m_StuckMoveX;
                    }
                    else if ( !m_LastJetStuckTimer.IsRunning() )
                    {
                        m_LastJetStuckTimer.Reset();
                        m_LastJetStuckTimer.Start();
                    }
                }
            }
BotUpdateGoToLocTimer[5].Stop();

        }
                
    }
    else
    {
BotUpdateGoToLocTimer[6].Start();
        if ( !m_RunningEditorPath
            && m_PathKeeper.GetElapsedTime() > MAX_PATH_SEARCH_TIME )
        {
            // Taking too long, start over...
            //                    bot_log::Log("Path search took too long, starting over\n");
            if ( g_BotDebug.DrawNavigationMarkers )
            {
                DrawSphere( GetBBox().GetCenter(), 3.0f, XCOLOR_BLUE );
            }

            m_PathKeeper.PlotCourse( m_WorldPos
                , m_CurrentGoal.GetTargetLocation(), GetMaxAir());
        }
                
        const xbool TargetNearCurrentPoint
            = IsNear( m_CurrentGoal.GetTargetLocation()
                , m_PathKeeper.GetCurrentPoint().Pos );

        PursueLocation( m_PathKeeper.GetCurrentPoint().Pos
            , m_PathKeeper.GetNextPoint().Pos
            , TargetNearCurrentPoint );
BotUpdateGoToLocTimer[6].Stop();

    }

    if ( FaceThisLocation && !AntagonizingEnemies )
    {
BotUpdateGoToLocTimer[7].Start();

        const vector3 EdgeDirection
            = m_PathKeeper.GetCurrentPoint().Pos
            - m_PathKeeper.GetPrevPoint().Pos;
        const f32 DistSQ
            = (m_WorldPos - m_PathKeeper.GetCurrentPoint().Pos).LengthSquared();
        static const f32 LOOK_AHEAD_DIST_SQ = 10.0f;
        vector3 Temp;
        radian3 TempRad;
        f32     TempF32;
        Get1stPersonView( Temp, TempRad, TempF32 );
        TempF32 = Temp.Y;

        if ( EdgeDirection.LengthSquared() > 0.0001f )
        {
            f32 Run = x_sqrt(
                x_sqr(EdgeDirection.X) + x_sqr(EdgeDirection.Z) );
            if ( DistSQ <= LOOK_AHEAD_DIST_SQ
                || EdgeDirection.Y / Run >= 10.0f )
            {
                // this is steep, face elsewhere
                Temp = m_PathKeeper.GetNextPoint().Pos;
                Temp.Y = TempF32;
                FaceLocation( Temp );
            }
            else
            {
                Temp = m_PathKeeper.GetCurrentPoint().Pos;
                Temp.Y = TempF32;
                FaceLocation( Temp );
            }
        }
        else if ( DistSQ <= LOOK_AHEAD_DIST_SQ )
        {
            Temp = m_PathKeeper.GetNextPoint().Pos;
            Temp.Y = TempF32;
            FaceLocation( Temp );
        }
        else
        {
            Temp = m_PathKeeper.GetCurrentPoint().Pos;
            Temp.Y = TempF32;
            FaceLocation( Temp );
        }
BotUpdateGoToLocTimer[7].Stop();

    }
    object* pPainOriginObj = ObjMgr.GetObjectFromID(m_pPainOriginObjID);

    // last resort hit reaction
    if (
        // has it been recent pain?
        (m_PainTimer.ReadSec() <= 2.5f)

        // was it a player that caused the pain?
        && ((pPainOriginObj)
            && (pPainOriginObj->GetAttrBits() & object::ATTR_PLAYER))

        // make sure we're not in CTF carrying the flag
        && !((GameMgr.GetGameType() == GAME_CTF) && HasFlag())

        // make sure we're not capping and approaching the flag
        && !(m_CurTask
             && ((m_CurTask->GetID() == bot_task::TASK_TYPE_CAPTURE_FLAG)
                 && NearEnemyFlag())) )
    {
        if ( OnGround() )
        {
            m_Move.JumpKey = TRUE;
        }

        m_Move.JetKey = TRUE;
    }
BotUpdateGoToLocTimer[0].Stop();
}

//==============================================================================
// NearEnemyFlag()
//==============================================================================
xbool bot_object::NearEnemyFlag( void ) const
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    xbool Near = FALSE;

    object::type ObjType;
    switch (GameMgr.GetGameType())
    {
        case ( GAME_CTF ):
        case ( GAME_HUNTER ):
            ObjType = object::TYPE_FLAG;
        break;

        case ( GAME_CNH ):
            ObjType = object::TYPE_FLIPFLOP;
        break;

        default:
            return FALSE;
        break;
    }


    object* pObject;
    ObjMgr.StartTypeLoop( ObjType );

    while ( (pObject = ObjMgr.GetNextInType()) != NULL )
    {
        if (
            // make sure it's not our flag
            !(pObject->GetTeamBits() & m_TeamBits)

            // make sure we're "near" it
            && ((pObject->GetPosition() - m_WorldPos).LengthSquared()
                < (75.0f * 75.0f)) )
        {
            Near = TRUE;
        }
    }
    ObjMgr.EndTypeLoop();

    return Near;
    
}

//==============================================================================
// UpdatePatrol()
//==============================================================================
void bot_object::UpdatePatrol( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    //--------------------------------------------------
    // Keep moving
    //--------------------------------------------------
    UpdateGoToLocation( !m_Move.FireKey );

    if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_PATROL )
    {
        // We've arrived, but that will be handled in the strategy
        // section, so undo this part for now...
        m_CurrentGoal.SetGoalID( bot_goal::ID_PATROL );
    }
    
}

//==============================================================================
// UpdateEngage()
//==============================================================================
void bot_object::UpdateEngage( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    ASSERT( ENGAGE_START_FIGHTING_DISTANCE < ENGAGE_STOP_FIGHTING_DISTANCE );
    ASSERT( ENGAGE_ELF_START_FIGHTING_DISTANCE
        < ENGAGE_ELF_STOP_FIGHTING_DISTANCE );
    const player_object* TargetPlayer
        = (player_object*)(m_CurrentGoal.GetTargetObject());

    if (!TargetPlayer)
    {
        UpdatePursueObject();
        return;
    }
    const vector3& TargetPos = TargetPlayer->GetPosition();

    if ( g_BotDebug.DrawNavigationMarkers )
    {
        DrawLine( m_WorldPos + CHEST_HEIGHT
            , TargetPos + CHEST_HEIGHT
            , XCOLOR_WHITE );
        draw_Label( TargetPos, XCOLOR_WHITE, "Target" );
    }

    const xbool Elfing
        = (m_RequestedWeaponSlot == WeaponInSlot( INVENT_TYPE_WEAPON_ELF_GUN ));

    switch ( m_CurrentGoal.GetGoalState() )
    {
    case bot_goal::STATE_ENGAGE_GOTO:
        if ( !TargetPlayer->IsDead()
            && ((TargetPos- m_WorldPos ).LengthSquared()
                <= (Elfing
                    ? ENGAGE_ELF_START_FIGHTING_DISTANCE_SQ
                    : ENGAGE_START_FIGHTING_DISTANCE_SQ))
            && CanHitTarget( *TargetPlayer )
            && OnGround() )
        {
            m_CurrentGoal.SetGoalState( bot_goal::STATE_ENGAGE_FIGHTING );
            m_EngageDodging = FALSE;
            UpdateEngage();
        }
        else
        {
            UpdatePursueObject();
            TryToFireOnTarget( *TargetPlayer );
        }
        
        break;

    case bot_goal::STATE_ENGAGE_FIGHTING:
        if ( !TargetPlayer->IsDead()
            && ((TargetPos - m_WorldPos ).LengthSquared()
                <= (Elfing
                    ? ENGAGE_ELF_STOP_FIGHTING_DISTANCE_SQ
                    : ENGAGE_STOP_FIGHTING_DISTANCE_SQ))
            && CanHitTarget( *TargetPlayer ) )
        {
            // Ok, game on...
            if ( !InsideBuilding()

                // we're not trying to elf them
                && !Elfing

                // we're not at a steep enough angle
                && ((-GetElevationToPos( TargetPos )
                    < ENGAGE_AIR_SUPERIORITY_ANGLE )

                    // we're not high enough
                    || ((m_WorldPos.Y
                        - m_pTerr->GetHeight( m_WorldPos.Z, m_WorldPos.X ))
                        < m_EngageSuperiorityHeight)) )
            {
                // See if we've been dodging in the same direction
                // too long
                if ( m_EngageDodging
                    && m_DodgingTimer.ReadSec() >= MAX_DODGE_TIME_SEC )
                {
                    m_EngageDodging = FALSE;
                }

                const f32 EnergyPct = m_Energy / m_MoveInfo->MAX_ENERGY;
                
                // Ok, try to get above the enemy
                if ( m_ContactInfo.BelowSurface
                    && EnergyPct >= ENGAGE_MIN_AIR_COMBAT_START_FLYING_ENERGY )
                {
                    m_Move.JumpKey = TRUE;
                    m_Move.JetKey = TRUE;
                    m_EngageDodging = FALSE;
                }
                else if ( !m_ContactInfo.BelowSurface
                    && EnergyPct >= ENGAGE_MIN_AIR_COMBAT_STOP_FLYING_ENERGY )
                {
                    m_Move.JetKey = TRUE;
                    m_EngageDodging = FALSE;
                }
                else if ( !m_EngageDodging )
                {
                    // we'll be on the ground for now, so dodge some
                    m_EngageDodging = TRUE;
                    m_DodgingTimer.Reset();
                    m_DodgingTimer.Start();

                    if ( x_irand( 1, 100 ) < 50 )
                    {
                        m_DodgeXKey = -1.0f;
                    }
                    else
                    {
                        m_DodgeXKey = 1.0f;
                    }
                }

                if ( m_EngageDodging )
                {
                    m_Move.MoveX = m_DodgeXKey;
                }
            }

            TryToFireOnTarget( *TargetPlayer, TRUE );
        }
        else
        {
            // stop fighting and goto
            m_CurrentGoal.SetGoalState( bot_goal::STATE_ENGAGE_GOTO );
            UpdateEngage();
        }
        
        break;

    default:
        ASSERT( 0 ); // Invalid goal state
    };
}

//==============================================================================
// UpdateMortar()
//==============================================================================
void bot_object::UpdateMortar( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    const object* Target = m_CurrentGoal.GetTargetObject();
    if (!Target)
    {
        // Target has been destroyed already.  Quit.
        m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );

        if ( !m_CurTask )
        {
            ASSERT( m_CurTask );
            x_DebugLog( "Bot: UpdateMortar called with null m_CurTask\n" );
            return;
        }
        else
        {
            m_CurTask->ForceReset();
        }

        return;
    }
    vector3 Bullseye;
    GetTargetBullseye( Bullseye,
        *Target,
        player_object::INVENT_TYPE_WEAPON_MORTAR_GUN );

    if ( WeaponInSlot( player_object::INVENT_TYPE_WEAPON_MORTAR_GUN ) < 0 )
    {
        // don't have a mortar gun
        m_CurrentGoal.SetGoalID( bot_goal::ID_NO_GOAL );
        return;
    }

    //--------------------------------------------------
    // See if we can hit the target from here
    //--------------------------------------------------
    if ( !AntagonizeEnemies() // Safety first!
        && (Target->GetHealth() > 0.0f)
        && ((Bullseye - m_WorldPos).LengthSquared() < MIN_MORTAR_RANGE_SQ)
        && CanHitTargetArcing(
            player_object::INVENT_TYPE_WEAPON_MORTAR_GUN, *Target ) )
    {
        // we can shoot, so let's do that
        const xbool Aiming = MortarTarget( *Target );

        // see if we can really aim at it yet
        if ( !Aiming )
        {
            // guess not :-/
            UpdatePursueObject( FALSE );
        }
    }
    else
    {
        UpdatePursueObject();
    }
}

//==============================================================================
// UpdateRepair()
//==============================================================================
void bot_object::UpdateRepair( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    //--------------------------------------------------
    // Do we have a repair pack?
    //--------------------------------------------------
    if ( m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_REPAIR] == 0 )
    {
        // Nope, drop our current pack and continue

        if ( (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_AMMO] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_ENERGY] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_CLOAKING] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_SENSOR_JAMMER] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_ARMOR_PACK_SHIELD] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_AA] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_ELF] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_MOTION_SENSOR] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_OUTDOOR] > 0)
            || (m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE] > 0) )
        {
            m_Move.DropPackKey = TRUE;
        }
        

        if ( m_GoingToInven )
        {
            UpdateLoadout();
        }
        else
        {
            // going to pickup repair pack

            // make sure the pack is still there
            object* Pack = ObjMgr.GetObjectFromID( m_NearestRepairPack );
            if ( Pack 
                && ((Pack->GetPosition() - m_CurrentGoal.GetTargetLocation()).LengthSquared()
                    < 0.1f) )
            {
                UpdateGoToLocation();
            }
            else
            {
                InitializeRepair( *(m_CurrentGoal.GetTargetObject()) );
            }
        }
        
        m_CurrentGoal.SetGoalID( bot_goal::ID_REPAIR );
    }
    else
    {
        if ( m_NeedRepairPack )
        {
            // well, we have one now...
            
            const object* pTarget = m_CurrentGoal.GetTargetObject();

            if ( pTarget )
            {
                InitializeRepair( *pTarget );
            }
        }
        
        //--------------------------------------------------
        // Are we close enough to begin the repair?
        //--------------------------------------------------
        const object* Target = m_CurrentGoal.GetTargetObject();

        if (!Target)
        {
//            ASSERT( 0 ); // Some strange bug -- we should have a repair target
            // The deployed asset we are trying to repair has been destroyed.
            m_CurrentGoal.SetGoalID(bot_goal::ID_NO_GOAL);

            if ( !m_CurTask )
            {
                ASSERT( m_CurTask );
                x_DebugLog( "Bot::UpdateRepair() called with null m_CurTask\n" );
                return;
            }
            else
            {
                m_CurTask->ForceReset();
            }

            return;
        }

        const vector3& TargetPos = Target->GetPosition();

        if ( //OnGround()
            /*&&*/ ((m_WorldPos - TargetPos).LengthSquared() <= MIN_REPAIR_DIST_SQ)
            && CanHitTarget( *Target ) )
        {
            // repair
            RepairTarget( *Target);
        }
        else
        {
            // keep moving
            UpdatePursueObject();
        }
    }
}

//==============================================================================
// UpdateDestroy()
//==============================================================================
void bot_object::UpdateDestroy( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( g_BotDebug.DrawNavigationMarkers )
    {
        if ( !m_CurTask )
        {
            ASSERT( m_CurTask );
            x_DebugLog( "Bot: UpdateDestroy called with null m_CurTask\n" );
        }
        else if ( m_CurTask->GetTargetObject())
        {
            DrawSphere( m_CurrentGoal.GetTargetLocation(), 1.0f, XCOLOR_GREEN );
            draw_Label( m_CurrentGoal.GetTargetLocation(), XCOLOR_RED, 
                "Destroy VP %d/%d", m_ObjectID.Seq, m_ObjectID.Slot);
            DrawSphere( m_CurTask->GetTargetObject()->GetPosition(), 1.0f, XCOLOR_GREEN );
            draw_Label( m_CurTask->GetTargetObject()->GetPosition(), XCOLOR_RED, 
                "Destroy T %d/%d", m_ObjectID.Seq, m_ObjectID.Slot);
        }

    }

    const object* pTarget = m_CurrentGoal.GetTargetObject();
    if ( !pTarget )
    {
        // nothing to do here :)
        AntagonizeEnemies();
        return;
    }

    //--------------------------------------------------
    // Figure out if we're destroying a vehicle
    //--------------------------------------------------
    xbool TargetIsVehicle = pTarget->GetAttrBits()
        & (object::ATTR_VEHICLE | object::ATTR_PLAYER);

    if ( pTarget->GetHealth() <= 0.0f && !TargetIsVehicle )
    {
        //--------------------------------------------------
        // nothing to do here :)
        // say something, then just bother people
        //--------------------------------------------------
        switch ( pTarget->GetType() )
        {
        case object::TYPE_TURRET_CLAMP:
        case object::TYPE_TURRET_FIXED:
        case object::TYPE_TURRET_SENTRY:
            Speak( 35 );
            break;
        
        case object::TYPE_SENSOR_REMOTE:
        case object::TYPE_SENSOR_LARGE:
        case object::TYPE_SENSOR_MEDIUM:
            Speak( 34 );
            break;
        
        case object::TYPE_GENERATOR:
            Speak( 32 );
            break;

        default:
            Speak( 112 );
        }
        
        InitializeAntagonize();
        return;
    }

    vector3 Point;
    xbool GotPoint = FALSE;
    if ( m_WaitingForDestroyVantagePoint ) 
    {
        GetDestroyVantagePoint( Point, *pTarget );
        GotPoint = TRUE;
    }

    if ( m_WaitingForDestroyVantagePoint )
    {
        // Keep going toward the target
        UpdateGoToLocation();
    }
    else
    {
        if ( GotPoint )
        {
            // Stop going towards the target, and head for the point
            InitializeGoTo( Point );
            m_CurrentGoal.SetGoalID( bot_goal::ID_DESTROY );
        }

        //--------------------------------------------------
        // Should we begin shooting?
        //--------------------------------------------------
        const f32 DistSQ =  
            (m_WorldPos - m_CurrentGoal.GetTargetLocation()).LengthSquared();
    
        xbool TryingToFire = FALSE;
        const xbool NearTarget
            = HaveArrivedAt( m_CurrentGoal.GetTargetLocation() );
        const xbool NearVehicle
            = (TargetIsVehicle && DistSQ < MIN_DESTROY_VEHICLE_DIST_SQ);
        
        SelectWeapon( *pTarget );
        const xbool CanHit = CanHitTarget( *pTarget );
        
        if ( CanHit )
        {
            // destroy
            TryToFireOnTarget( *pTarget, TRUE );
            TryingToFire = TRUE;
        }
        else 
        {
            // are we destroying a fixed turret?
            const xbool bDestroyingTurret = pTarget->GetType() == object::TYPE_TURRET_FIXED;

            // do we have a mortar cannon and ammo?
            const s32 MortarSlot = WeaponInSlot( INVENT_TYPE_WEAPON_MORTAR_GUN );
            const xbool HaveMortarCannon = MortarSlot > -1;
            const xbool EnoughMortars = HaveMortarCannon
                && (GetWeaponAmmoCount( INVENT_TYPE_WEAPON_MORTAR_GUN )
                    >= s_WeaponInfo[INVENT_TYPE_WEAPON_MORTAR_GUN].AmmoNeededToAllowFire);

            if ( bDestroyingTurret && HaveMortarCannon && EnoughMortars )
            {
                // can we hit the turret with the mortar?
                const xbool CanHitArcing = CanHitTargetArcing(
                    INVENT_TYPE_WEAPON_MORTAR_GUN, *pTarget );

                if ( CanHitArcing )
                {
                    // fire away
                    m_RequestedWeaponSlot = MortarSlot;
                    TryToFireOnTarget( *pTarget, TRUE );
                    TryingToFire = TRUE;
                }
            }
        }

        if ( (!NearTarget && !NearVehicle) || !TryingToFire )
        {
            // keep moving
            if ( TargetIsVehicle )
            {
                UpdatePursueObject( !TryingToFire );
            }
            else
            {
                UpdateGoToLocation( !TryingToFire );
            }

            //--------------------------------------------------
            // Throw down a mine?
            //--------------------------------------------------
            if (
                // do we have a mine?
                (m_Loadout.Inventory.Counts[INVENT_TYPE_MINE] > 0)

                // is it time?
                && (m_MineDropTimer.ReadSec() >= m_NextMineDropTime)

                // are we close enough to the target?
                && ((m_WorldPos - pTarget->GetPosition()).LengthSquared()
                    < (50.0f * 50.0f)) )
            {
                // are we inside a building's bbox?
                xbool bInsideBuilding = FALSE;
                building_obj* pBuilding = NULL;

                ObjMgr.StartTypeLoop( object::TYPE_BUILDING );
                while( (pBuilding = (building_obj*)ObjMgr.GetNextInType()) )
                {
                    if( pBuilding->GetBBox().Intersect( m_WorldPos ) )
                    {
                        bInsideBuilding = TRUE;
                        break;
                    };
                }
                ObjMgr.EndTypeLoop();

                if ( bInsideBuilding )
                {
                    // ok, drop a mine
                    m_Move.MineKey = TRUE;
                    m_MineDropTimer.Reset();
                    m_MineDropTimer.Start();
                    m_NextMineDropTime = x_frand( 2.0f, 10.0f );
                }
            }
        }
    }
}

//==============================================================================
// UpdateAntagonize()
//==============================================================================
void bot_object::UpdateAntagonize( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_AntagonizeTarget != obj_mgr::NullID )
    {
        //
        // we're engaging somebody
        //

        // see if they are still a valid target
        const player_object* pPlayer = (player_object*)(ObjMgr.GetObjectFromID( m_AntagonizeTarget ));             
        if ( pPlayer && !pPlayer->IsDead() )
        {
            // ok, keep engaging
            UpdateEngage();
            m_CurrentGoal.SetGoalID( bot_goal::ID_ANTAGONIZE );
        }
        else
        {
            // we got him, head back to our post
            m_AntagonizeTarget = obj_mgr::NullID;
            InitializeGoTo( m_AntagonizePos );
            UpdateGoToLocation();
            m_CurrentGoal.SetGoalID( bot_goal::ID_ANTAGONIZE );
        }

    }
    else
    {
        // we're not after anybody, see if we want anyone
        player_object* Enemy = GetNearestVisibleEnemy();

        if ( Enemy )
        {
            // Do we know about this guy?
            const vector3 ToEnemy = Enemy->GetPos() - GetPos();

            if ( (ToEnemy.LengthSquared() > ANTAGONIZE_MAX_RANGE_SQ)                       // Too far
                || ((v3_AngleBetween( ToEnemy, vector3( m_Rot ) ) > ANTAGONIZE_FOV_DIV_2)   // Out of FOV
                    && (ToEnemy.LengthSquared() > x_sqr( ANTAGONIZE_HEARING_DIST_SQ ))) )   // Can't hear
            {
                // Nope, ignore him
                Enemy = NULL;
            }
        }

        if ( !Enemy )
        {
            // ok, see if someone just hurt us
            object* pPainOriginObj = ObjMgr.GetObjectFromID(m_pPainOriginObjID);
            if ( (m_PainTimer.ReadSec() <= 5.0f)
                && pPainOriginObj
                && (pPainOriginObj->GetAttrBits() & object::ATTR_PLAYER) )
            {
                // he's been a bad boy...
                Enemy = (player_object*)pPainOriginObj;   
            }
        }

        if ( Enemy && !Enemy->IsDead() )
        {
            // Ok, engage this bastard
            m_AntagonizeTarget = Enemy->GetObjectID();
            InitializeEngage( *Enemy );
            m_CurrentGoal.SetGoalID( bot_goal::ID_ANTAGONIZE );
        }
        else
        {
            // See if we need to continue moving toward our post
            if ( !HaveArrivedAt( m_AntagonizePos ) )
            {
                UpdateGoToLocation();
                m_CurrentGoal.SetGoalID( bot_goal::ID_ANTAGONIZE );
            }
            else
            {
                // nothing to do, just fidget

                //--------------------------------------------------
                // see if it's been a while, move/look around
                //--------------------------------------------------
                if ( m_AntagonizeTimer.ReadSec() >= m_AntagonizeFidgetTime )
                {
                    // fidget by looking around
                    s32 Rand = x_irand( 1, 100 );

                    if ( Rand < 30 )
                    {
                        // Look at the center of the graph
                        FaceLocation( g_Graph.GetCenter() );
                    }
                    else
                    {
                        // Look at a nearby ally
                        object::id Allies[16];
                        s32 nAllies = 0;
                        object* pObject;
    
                        ObjMgr.Select( object::ATTR_PLAYER, m_WorldPos, 100.0f );
                        while ( (pObject = ObjMgr.GetNextSelected()) != NULL )
                        {
                            if ( (pObject->GetTeamBits() & GetTeamBits())
                                && (pObject->GetObjectID() != GetObjectID()) )
                            {
                                Allies[nAllies] = pObject->GetObjectID();
                                ++nAllies;
                            }

                            if ( nAllies >= 16 )
                            {
                                break;
                            }
                        }
                        ObjMgr.ClearSelection();

                        if ( nAllies > 0 )
                        {
                            pObject = ObjMgr.GetObjectFromID(
                                Allies[x_irand(0,nAllies-1)] );
                            ASSERT( pObject );
                            FaceLocation( pObject->GetPosition() );
                        }
                        else
                        {
                            FaceLocation( g_Graph.GetCenter() );
                        }
                    }
                    m_AntagonizeTimer.Reset();
                    m_AntagonizeTimer.Start();
                    m_AntagonizeFidgetTime = 2 + x_irand( 0, 2 );
                }
            }
        }
    }
}

//==============================================================================
// UpdateDefendFlag()
//==============================================================================
void bot_object::UpdateRoam( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( g_BotDebug.DrawNavigationMarkers )
    {
        DrawSphere( m_CurrentGoal.GetTargetLocation(), 1.0f, XCOLOR_GREEN );
        draw_Label( m_CurrentGoal.GetTargetLocation(), XCOLOR_GREEN, "Roam" );
    }

    // Where are we in our patrol?
    if ( HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) )
    {
        if ( !m_CurTask )
        {
            ASSERT( m_CurTask );
            x_DebugLog( "Bot: UpdateDefendFlag called with null m_CurTask\n" );
        }
        else if ( m_RoamTimer.ReadMs() >= m_RoamStandAroundTime )
        {
            // Arrived at patrol point, pick another
            InitializeRoam( m_CurrentGoal.GetAuxLocation() );
        }
        else
        {
            if ( !AntagonizeEnemies() )
            {
                // look around
                FaceLocation( GetAverageEnemyPos() );
            }
        }
    }
    else
    {
        // Keep going
        UpdateGoToLocation();
        
        m_RoamTimer.Reset();
        m_RoamTimer.Start();
        m_RoamStandAroundTime = x_frand( 5000.0f, 10000.0f );
    }
}

//==============================================================================
// GetAverageEnemyPos()
//==============================================================================
vector3 bot_object::GetAverageEnemyPos( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    vector3 Pos;
    Pos.Zero();
    s32 nEnemies = 0;
    object* pObject;

    ObjMgr.Select( object::ATTR_PLAYER );
    while ( (pObject = ObjMgr.GetNextSelected()) != NULL )
    {
        if ( (pObject->GetTeamBits() & GetTeamBits()) == 0 )
        {
            Pos += pObject->GetPosition();
            ++nEnemies;
        }
    }
    ObjMgr.ClearSelection();

    Pos *= (1 / (f32)nEnemies);

    return Pos;
}

//==============================================================================
// AntagonizeEnemies()
// This function checks for any objects in the area we could be firing on
// at this time.
//==============================================================================
xbool bot_object::AntagonizeEnemies( void ) 
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( m_CreatedByEditor
        && !g_BotDebug.AntagonizeEnemiesIfCreatedByEditor )
    {
        return FALSE;
    }
    
    // Check for mines in our path
    mine* pMine = GetMostDangerousMine();

    if ( pMine && CanHitTarget( *pMine ) )
    {
        TryToFireOnTarget( *pMine, TRUE );
        return TRUE;
    }

    //----------------------------------
    // Check for turrets
    //----------------------------------

    // If we're not heavy, only mess with clamp turrets....
    const xbool CheckClampsOnly = m_Loadout.Armor != ARMOR_TYPE_HEAVY;

    turret* pTurret = GetMostDangerousTurret( CheckClampsOnly );
                     // was (turret*)ObjMgr.GetObjectFromID(m_LastAttackTurret);

    if ( pTurret 
        && ((pTurret->GetType() != TYPE_TURRET_CLAMP)
            || m_AntagonizingTurrets) )
    {
        SelectWeapon( *pTurret );

        if  ( CanHitTarget( *pTurret ) )
        {
            TryToFireOnTarget( *pTurret );
            return TRUE;
        }
    }

    // Check for nearby enemy players
    player_object* Enemy = GetNearestEnemy();
    object* pPainOriginObj = ObjMgr.GetObjectFromID(m_pPainOriginObjID);

    if ( Enemy
        && !Enemy->IsDead())
    {
        const vector3 DeltaEnemy = Enemy->GetPos() - m_WorldPos;
        const xbool SpidySense
            = (
                // are we roaming?
                (m_CurrentGoal.GetGoalID() == bot_goal::ID_ROAM)

                // are we fighting?
                || (m_CurrentGoal.GetGoalID() == bot_goal::ID_ENGAGE)

                // is pain recent and from another player?
                || ((m_PainTimer.ReadSec() <= 5.0f)
                    && pPainOriginObj
                    && (pPainOriginObj->GetAttrBits()
                        & object::ATTR_PLAYER))
                // are we carrying the flag in CTF?
                || ((GameMgr.GetGameType() == GAME_CTF) && HasFlag()) );
        const f32 Range = (100.0f * m_Difficulty) + 100.0f;

        if(
            (DeltaEnemy.LengthSquared()
                < x_sqr( SpidySense ? Range : Range * 0.5f))
            && (SpidySense
                || (v3_AngleBetween( vector3( m_Rot), DeltaEnemy ) < R_80))
            && CanHitTarget( *Enemy ) )
        {
            TryToFireOnTarget( *Enemy, TRUE );
            return TRUE;
        }
    }

    // Check for nearby enemy vehicles
    {
        vehicle* EnemyVehicle = GetNearestEnemyVehicle();
    
        if ( EnemyVehicle && CanHitTarget( *EnemyVehicle ) )
        {
            TryToFireOnTarget( *EnemyVehicle, TRUE );
            return TRUE;
        }
    }


    // Check for nearby enemy invens
    {
        object* EnemyStation = GetNearestEnemyStation();

        if ( EnemyStation && CanHitTarget( *EnemyStation ) )
        {
            TryToFireOnTarget( *EnemyStation, TRUE );
            return TRUE;
        }
    }

    
    // Check for nearby enemy sensors
    {
        object* EnemySensor = GetNearestEnemySensor();

        if ( EnemySensor && CanHitTarget( *EnemySensor ) )
        {
            TryToFireOnTarget( *EnemySensor, TRUE );
            return TRUE;
        }
    }


    return FALSE;
}

//==============================================================================
// GetMostDangerousMine()
//==============================================================================
mine* bot_object::GetMostDangerousMine( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    mine* pMine = NULL;
    mine* pClosestMine = NULL;
    f32 ClosestMineDistSQ = MAX_DANGEROUS_MINE_RADIUS_SQ;

    ObjMgr.StartTypeLoop( object::TYPE_MINE );

    while ( (pMine = (mine*)ObjMgr.GetNextInType()) != NULL )
    {
        // team mines aren't dangerous
        if ( pMine->GetTeamBits() & m_TeamBits ) continue;
        
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ = (pMine->GetPosition() - m_WorldPos).LengthSquared();
        const radian AngleBetween
            = v3_AngleBetween( m_Vel, pMine->GetPosition() - m_WorldPos );

        if ( (DistSQ < ClosestMineDistSQ)
            && ( AngleBetween <= R_22) )
        {
            // Best candidate so far
            pClosestMine = pMine;
            ClosestMineDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    return pClosestMine;
}

//==============================================================================
// GetMostDangerousTurret()
//==============================================================================
turret* bot_object::GetMostDangerousTurret( xbool CheckClampsOnly /* = FALSE */ )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    turret* pTurret = NULL;
    turret* pClosestTurret = NULL;
    f32 ClosestTurretDistSQ = MAX_DANGEROUS_TURRET_RADIUS_SQ;

    ObjMgr.StartTypeLoop( object::TYPE_TURRET_CLAMP );

    while ( (pTurret = (turret*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pTurret->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pTurret->GetTeamBits())  // Our team's?
            && pTurret->GetEnabled()    // Don't bother with disabled turrets!
            && (DistSQ < ClosestTurretDistSQ)
            && CanHitTarget( *pTurret ) )
        {
            // Best candidate so far
            pClosestTurret = pTurret;
            ClosestTurretDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    if ( CheckClampsOnly )
    {
        return pClosestTurret;    
    }

    ObjMgr.StartTypeLoop( object::TYPE_TURRET_FIXED );

    while ( (pTurret = (turret*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pTurret->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pTurret->GetTeamBits())  // Our team's?
            && pTurret->GetEnabled()    // Don't bother with disabled turrets!
            && (DistSQ < ClosestTurretDistSQ)
            && CanHitTarget( *pTurret ) )
        {
            // Best candidate so far
            pClosestTurret = pTurret;
            ClosestTurretDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop( object::TYPE_TURRET_SENTRY );

    while ( (pTurret = (turret*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pTurret->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pTurret->GetTeamBits())  // Our team's?
            && pTurret->GetEnabled()    // Don't bother with disabled turrets!
            && (DistSQ < ClosestTurretDistSQ)
            && CanHitTarget( *pTurret ) )
        {
            // Best candidate so far
            pClosestTurret = pTurret;
            ClosestTurretDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    return pClosestTurret;
}

//==============================================================================
vehicle* bot_object::GetNearestEnemyVehicle ( void )
{
    vehicle* pVehicle = NULL;
    vehicle* pClosestVehicle = NULL;
    f32 ClosestDistSQ = x_sqr( 100.0f );

    ObjMgr.Select( object::ATTR_VEHICLE, m_WorldPos, 100.0f );

    while ( (pVehicle = (vehicle*)(ObjMgr.GetNextSelected())) )
    {
        const f32 DistSQ = (pVehicle->GetPosition() - m_WorldPos).LengthSquared();

        if ( !(m_TeamBits & pVehicle->GetTeamBits())
            && DistSQ < ClosestDistSQ )
        {
            pClosestVehicle = pVehicle;
            ClosestDistSQ = DistSQ;
        }
    }

    ObjMgr.ClearSelection();

    return pClosestVehicle;
}

//==============================================================================
station* bot_object::GetNearestEnemyStation ( void )
{
    station* pStation = NULL;
    station* pClosestStation = NULL;
    f32 ClosestDistSQ = x_sqr( 100.0f );

    ObjMgr.StartTypeLoop( object::TYPE_STATION_DEPLOYED );

    while ( (pStation = (station*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pStation->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pStation->GetTeamBits())
            && pStation->GetEnabled() 
            && (DistSQ < ClosestDistSQ)
            && CanHitTarget( *pStation ) )
        {
            // Best candidate so far
            pClosestStation = pStation;
            ClosestDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    return pClosestStation;
}


//==============================================================================
sensor* bot_object::GetNearestEnemySensor   ( void )
{
    sensor* pSensor = NULL;
    sensor* pClosestSensor = NULL;
    f32 ClosestDistSQ = x_sqr( 100.0f );

    ObjMgr.StartTypeLoop( object::TYPE_SENSOR_REMOTE );

    while ( (pSensor = (sensor*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pSensor->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pSensor->GetTeamBits())
            && pSensor->GetEnabled() 
            && (DistSQ < ClosestDistSQ)
            && CanHitTarget( *pSensor ) )
        {
            // Best candidate so far
            pClosestSensor = pSensor;
            ClosestDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop( object::TYPE_SENSOR_MEDIUM );

    while ( (pSensor = (sensor*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pSensor->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pSensor->GetTeamBits())
            && pSensor->GetEnabled() 
            && (DistSQ < ClosestDistSQ)
            && CanHitTarget( *pSensor ) )
        {
            // Best candidate so far
            pClosestSensor = pSensor;
            ClosestDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop( object::TYPE_SENSOR_LARGE );

    while ( (pSensor = (sensor*)ObjMgr.GetNextInType()) != NULL )
    {
        // see if it's close enough and if we're heading (towards) it
        const f32 DistSQ =
            (pSensor->GetPosition() - m_WorldPos).LengthSquared();
        
        if ( !(m_TeamBits & pSensor->GetTeamBits())
            && pSensor->GetEnabled() 
            && (DistSQ < ClosestDistSQ)
            && CanHitTarget( *pSensor ) )
        {
            // Best candidate so far
            pClosestSensor = pSensor;
            ClosestDistSQ = DistSQ;
        }
    }
    
    ObjMgr.EndTypeLoop();

    return pClosestSensor;

}


//==============================================================================
// UpdateLoadout()
//==============================================================================
void bot_object::UpdateLoadout( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    //--------------------------------------------------
    // Make sure our inven is still there and healthy
    //--------------------------------------------------
    station* pInven = (station*)(ObjMgr.GetObjectFromID( m_InvenId ));

    if (   pInven
        && pInven->GetEnabled() )
    {
        object::id PlayerID = pInven->OccupiedBy();
        if ( PlayerID == obj_mgr::NullID
            || PlayerID == m_ObjectID
            || ((m_WorldPos - pInven->GetPosition()).LengthSquared()
                > x_sqr( 10.0f ))
            || m_SteppingOffInven )
        {
            UpdateGoToLocation();
        }

        if ( !m_UsingFixedInven )
        {
            // Check to see if we our next point is the approach point, and if
            // we are in line with the station. If this is the case, we can init
            // a new path straight to the station, and skip the approach point.
            if ( (m_PathKeeper.GetNextPoint().Pos - m_PathKeeper.GetDst().Pos).LengthSquared()
                < REALLY_CLOSE_SQ )
            {
                // Compute the vector to the station from the approach
                station* pInven = (station*)(ObjMgr.GetObjectFromID( m_InvenId ));
                ASSERT( pInven );
                const vector3 ToInven = pInven->GetPosition() - m_PathKeeper.GetDst().Pos;

                // See if we're going in this general direction
                if ( v3_AngleBetween( ToInven, m_Vel ) < R_10 )
                {
                    // Ok, skip the approach point and head straight for the inven
                    vector3 Rot( pInven->GetRotation() );
                    Rot *= 1.25f; // meters out in front, where the inven activates...
                    InitializeGoTo( pInven->GetPosition() - Rot );
                    m_CurrentGoal.SetGoalID( bot_goal::ID_LOADOUT );
                    m_UsingFixedInven = TRUE;
                    m_SteppingOffInven = FALSE;
                }
            }

        }
        
        if ( m_CurrentGoal.GetGoalID() == bot_goal::ID_NO_GOAL )
        {
            if ( m_UsingFixedInven && !m_SteppingOffInven )
            {
                // Goto has arrived, set it back to loadout and we will
                // leave this ID when the loadout takes effect (the inven
                // finishes with us).
                m_CurrentGoal.SetGoalID( bot_goal::ID_LOADOUT );
            }
            else
            {
                // We have arrived at our approach point, now go to station
                station* pInven = (station*)(ObjMgr.GetObjectFromID( m_InvenId ));
                ASSERT( pInven );

                if ( pInven->GetKind() == station::DEPLOYED )
                {
                    vector3 Rot( pInven->GetRotation() );
                    Rot *= 1.25f; // meters out in front, where the inven activates...
                    InitializeGoTo( pInven->GetPosition() - Rot );
                }
                else
                {
                    InitializeGoTo( pInven->GetPosition() );
                }

                m_CurrentGoal.SetGoalID( bot_goal::ID_LOADOUT );
                m_UsingFixedInven = TRUE;
                m_SteppingOffInven = FALSE;
            }
        }
    }
    else
    {
        // something happened to the one we were after
        station Inven;

        if ( GetNearestInven( Inven, (!NeedArmor() && !NeedInvenPack()) )
            != -1 )
        {
            InitializeLoadout( Inven );
        }
        else
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
    }
}

//==============================================================================
// UpdatePickup()
//==============================================================================
void bot_object::UpdatePickup( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    // make sure the pickup hasn't moved -- if it has, reinit goto
    object* pPickup = ObjMgr.GetObjectFromID( m_pRetrievedPickups[0] );

    if ( !pPickup )
    {
        InitializeCurrentTask();
        return;
    }
    
    if ( !IsNear( m_CurrentGoal.GetTargetLocation() , pPickup->GetPosition() ) )
    {
        // it has moved, so reinit...
        InitializePickup( *pPickup );
    }
    
    UpdateGoToLocation();

    if ( m_CurrentGoal.GetGoalID() == bot_goal::ID_NO_GOAL )
    {
        // Goto has arrived, set it back to pickup and we will
        // leave this in the goal update
        m_CurrentGoal.SetGoalID( bot_goal::ID_PICKUP );
    }
}

//==============================================================================
// UpdateDeploy()
//==============================================================================
void bot_object::UpdateDeploy( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( !m_CurTask )
    {
        ASSERT( m_CurTask );
        x_DebugLog( "Bot: UpdateDeploy called with null m_CurTask\n" );
    }

    if ( HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) || ((m_DeployFailure > 5) && (m_DeployFailure < 10)))
    {
        // Aim
        const vector3& DeployPos = m_CurrentGoal.GetAuxLocation();
        FaceLocation( DeployPos );

        // Deploy?
        if ( v3_AngleBetween(DeployPos - GetWeaponPos()
            , vector3( m_Rot ) )
            <= R_10 )
        {
            switch ( m_CurrentGoal.GetObjectType() )
            {
            case object::TYPE_TURRET_CLAMP:
                // initialize wasn't properly called ( InitializeCurrentTask() )
                ASSERT(m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_TURRET);
                if ( m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR] > 0 )
                {
                    
                    if( GameMgr.AttemptDeployTurret( m_ObjectID,
                                                     m_CurrentGoal.GetAuxLocation(),
                                                     m_CurrentGoal.GetAuxDirection(),
                                                     object::TYPE_TERRAIN ) )
                    {
                        AddInvent( INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR, -1 );
                        m_DeployFailure = 0;
                    }
                    else
                    {
                        m_DeployFailure++;
                    }
                }
                
                break;

            case object::TYPE_STATION_DEPLOYED:
                // initialize wasn't properly called ( InitializeCurrentTask() )
                ASSERT(m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_INVEN);
                if ( m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_INVENT_STATION] > 0 )
                {
                    // Temporarily change the guy's rotation.
                    radian CurYaw = m_DrawRot.Yaw;
                    ASSERT(m_CurrentGoal.GetAuxDirection().X == -1 &&
                           m_CurrentGoal.GetAuxDirection().Z == -1);

                    m_DrawRot.Yaw = radian(m_CurrentGoal.GetAuxDirection().Y);

                    if( GameMgr.AttemptDeployStation( m_ObjectID,
                                                      m_CurrentGoal.GetAuxLocation(),
                                                      vector3(0,1,0),
                                                      object::TYPE_TERRAIN ) )
                    {
                        AddInvent( INVENT_TYPE_DEPLOY_PACK_INVENT_STATION, -1 );
                        m_DeployFailure = 0;
#if DEPLOY_BEACONS_WITH_INVEN
                        if(  GetInventCount( INVENT_TYPE_BELT_GEAR_BEACON ) > 0 )
                        {
                            vector3 A = GetMuzzlePosition();
                            vector3 B = m_CurrentGoal.GetAuxLocation();

                            vector3 Start(B - A);
                            Start.RotateY(x_frand(R_15, R_45)*(x_rand()%2*2 - 1));

                            // Cast a ray from the muzzle to the ground.
                            vector3 End(A + (Start * 3));

                            Start = A;

                            collider Ray;
                            Ray.RaySetup(NULL, Start, End);
                            ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray);
                            if (Ray.HasCollided())
                            {
                                collider::collision Collision;
                                Ray.GetCollision(Collision);

                                // Deploy friendly beacon
                                if( GameMgr.AttemptDeployBeacon( m_ObjectID,
                                                                 Collision.Point,
                                                                 Collision.Plane.Normal,
                                                                 object::TYPE_TERRAIN ) )
                                {
                                    AddInvent( INVENT_TYPE_BELT_GEAR_BEACON, -1 );

                                    // Change the beacon mode
                                    object* pObject;
                                    ObjMgr.Select(object::ATTR_DAMAGEABLE, Collision.Point, 1.0);
                                    while ( (pObject = ObjMgr.GetNextSelected()) != NULL )
                                    {
                                        if (pObject->GetType() == object::TYPE_BEACON)
                                        {
                                            ((beacon*)pObject)->ToggleMode();
                                            break;
                                        }
                                    }
                                    ObjMgr.ClearSelection();
                                }
                            }
                        }                    
#endif// deploy beacons with invens

                    }
                    else
                        m_DeployFailure++;

                    // Change rotation back.
                    m_DrawRot.Yaw = CurYaw;
                }

                break;
            
            case object::TYPE_SENSOR_REMOTE:
                // initialize wasn't properly called ( InitializeCurrentTask() )
                ASSERT(m_CurTask->GetState() == bot_task::TASK_STATE_DEPLOY_SENSOR);
                if ( m_Loadout.Inventory.Counts[INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR] > 0 )
                {
                    
                    if( GameMgr.AttemptDeploySensor( m_ObjectID,
                                                     m_CurrentGoal.GetAuxLocation(),
                                                     m_CurrentGoal.GetAuxDirection(),
                                                     object::TYPE_TERRAIN ) )
                    {
                        AddInvent( INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR, -1 );
                        m_DeployFailure = 0;
                    }
                    else
                        m_DeployFailure++;
                }
                
                break;
            default:
                ASSERT( 0 ); // don't know how to deploy this type of object
                break;
            }
            
        }
    }
    else
    {
        if (m_DeployFailure >= 10)
        {
            // Flag this as failed.
            MAI_Mgr.TurretSpotFailed(m_TeamBits, m_MAIIndex);
        }
        m_DeployFailure = 0;
        UpdateGoToLocation();
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_DEPLOY )
        {
            // we've arrived...
            m_CurrentGoal.SetGoalID( bot_goal::ID_DEPLOY );
        }
    }
}

//==============================================================================
// UpdateSnipe()
//==============================================================================
void bot_object::UpdateSnipe( void )
{
    // Should only be called on the server
    ASSERT(tgl.ServerPresent) ;

    if ( !HaveArrivedAt( m_CurrentGoal.GetTargetLocation() ) )
    {
        UpdateGoToLocation( m_SnipeFirstGoto );
        if ( m_CurrentGoal.GetGoalID() != bot_goal::ID_SNIPE )
        {
            // we've arrived...
            m_CurrentGoal.SetGoalID( bot_goal::ID_SNIPE );
        }
    }
    else
    {
        m_SnipeFirstGoto = FALSE; // so next time we won't face the goto point
        
        if ( !m_CurTask )
        {
            ASSERT( m_CurTask );
            x_DebugLog( "Bot: UpdatePursueLocation called with null m_CurTask\n" );
        }

        if ( !m_SniperWanderTimer.IsRunning()
            && ((m_CurTask && (m_CurTask->GetState() == bot_task::TASK_STATE_SNIPE))
                || g_BotDebug.TestSnipe) )

        {
            // Start the timer
            m_SniperWanderTimer.Reset();
            m_SniperWanderTimer.Start();

            // Set up our wander time
            m_SniperWanderTime = x_frand( 2.0f, 10.0f );
        }

        if ( m_SniperWanderTimer.ReadSec() >= m_SniperWanderTime
            && ((m_CurTask && (m_CurTask->GetState() == bot_task::TASK_STATE_SNIPE))
                || g_BotDebug.TestSnipe) )
        {
            // time to wander somewhere
            InitializeGoTo( m_SnipeLocation
                + vector3(
                    x_frand( -2.5f, 2.5f )
                    , 0.0f
                    , x_frand( -2.5f, 2.5f ) ) );
            
            m_CurrentGoal.SetGoalID( bot_goal::ID_SNIPE );
            m_SniperTarget = obj_mgr::NullID;
            m_SniperSenseTimer.Reset();
            m_SniperSenseTimer.Start();
            m_SniperSenseTime = x_frand( 0.5f, 2.5f );
            m_SniperWanderTimer.Stop();
        }
        
        // snipe
        m_RequestedWeaponSlot = WeaponInSlot( INVENT_TYPE_WEAPON_SNIPER_RIFLE );

        player_object* pTarget = NULL;

        if ( m_CurTask )
        {
            const object* pObject = m_CurTask->GetTargetObject();

            if ( pTarget && pTarget->GetAttrBits() & object::ATTR_PLAYER )
            {
                pTarget = (player_object*)pObject;
            }
        }

        if ( pTarget == NULL )
        {
            if( m_SniperSenseTimer.ReadSec() >= m_SniperSenseTime )
            {
                pTarget = GetNearestVisibleEnemy();
                if ( pTarget )
                {
                    m_SniperTarget = pTarget->GetObjectID();
                    m_SniperSenseTimer.Reset();
                    m_SniperSenseTimer.Start();
                    m_SniperSenseTime = x_frand( 2.0f, 7.0f );
                }
            }
            else
            {
                pTarget = (player_object*)ObjMgr.GetObjectFromID( m_SniperTarget );
            }
        }

        if ( pTarget )
        {
            // snipe 'em
            AimAtTarget( *pTarget );
            xbool HoldingSniperRifle
                = (m_WeaponCurrentSlot == m_RequestedWeaponSlot);

            if ( !HoldingSniperRifle && m_RequestedWeaponSlot != -1 )
            {
                m_Move.NextWeaponKey = TRUE;
            }
            else 
            {
                m_Move.NextWeaponKey = FALSE;
            }

            if ( HoldingSniperRifle
                && m_Energy > (m_MoveInfo->MAX_ENERGY * 0.95f)
                && CanHitTarget( *pTarget )
                && FacingAimDirection(
                    (pTarget->GetPosition() - m_WorldPos).Length() ) )
            {
                m_Move.FireKey = TRUE;
            }
        }
    }
}

