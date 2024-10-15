#include "ObjectMgr/ObjectMgr.hpp"
#include "AssetEditor.hpp"
#include "e_draw.hpp"
#include "Entropy.hpp"
#include "Objects/Bot/BotObject.hpp"
#include "Objects/Player/DefaultLoadouts.hpp"
#include "Objects/Projectiles/Station.hpp"
#include "Objects/Projectiles/Sensor.hpp"
#include "resource.h" 
#include "GameMgr/GameMgr.hpp"
#include <stdio.h>
#include "Objects/Bot/BotLog.hpp"

#define LOADINIT 0
#define TEMP_FILE "temp.gph"
#define VERSION_NUMBER 2
#define MAX_TIME_SLICE 3.0
#define DISPLAY_ACTUAL_ASSETS 0

#define DEPLOYED_SENSE_RADIUS   50  //40 Indoor, 60 outdoor
#define   SENTRY_SENSE_RADIUS   60
#define  MISSILE_SENSE_RADIUS   80  // All base turrets sense 80 meters
#define       AA_SENSE_RADIUS   80
#define   MORTAR_SENSE_RADIUS   80
#define   PLASMA_SENSE_RADIUS   80
#define      ELF_SENSE_RADIUS   80

#define  DEPLOYED_FIRE_RADIUS   80 // 60 Indoor, 100 outdoor
#define    SENTRY_FIRE_RADIUS   60
// Base Turret Types:
#define   MISSILE_FIRE_RADIUS   250
#define        AA_FIRE_RADIUS   200
#define    MORTAR_FIRE_RADIUS   160
#define    PLASMA_FIRE_RADIUS   120
#define       ELF_FIRE_RADIUS   75

#define  AVERAGE_SENSE_RADIUS   80
#define   AVERAGE_FIRE_RADIUS   MISSILE_FIRE_RADIUS + \
                                AA_FIRE_RADIUS +      \
                                MORTAR_FIRE_RADIUS +  \
                                PLASMA_FIRE_RADIUS +  \
                                ELF_FIRE_RADIUS / 5

#define DROP_HEIGHT 0.70f

//==============================================================================
//  VARIABLES
//==============================================================================
extern xbool   g_RunningPathEditor;

xbool   g_DeployMine = FALSE;

s32     g_SelectedAssetType;
s32     g_StormSelected;
xbool   g_bNeutral;
s32     g_SelectedPriority;
s32     g_AssetsChanged = FALSE;

s32     g_SAType = -1;
const void *g_SelectedAsset = NULL;
const void *g_AltSelected = NULL;
const   char g_TeamName[2][8] = {"Storm", "Inferno",};

player_object *g_pPlayer = NULL;

//==============================================================================
//  LOCAL FUNCTION DECLARATIONS
//==============================================================================
void ClearDeployables ( void );

void SelectAsset( s32 Type );
void SelectPriority ( s32 Priority);
void SelectTeam(s32 Team, xbool Neutral);

//==============================================================================
//  FUNCTIONS
//==============================================================================

void asset_editor::InitClass( void )
{
    m_StormAssets.m_nInvens = 0;
    m_StormAssets.m_nTurrets = 0;
    m_StormAssets.m_nMines = 0;
    m_StormAssets.m_Team = asset_spot::TEAM_STORM;
    m_StormAssets.m_nSnipePoints = 0;

    m_InfernoAssets.m_nInvens = 0;
    m_InfernoAssets.m_nTurrets = 0;
    m_InfernoAssets.m_nMines = 0;
    m_InfernoAssets.m_Team = asset_spot::TEAM_INFERNO;
    m_InfernoAssets.m_nSnipePoints = 0;

    m_Limits = 0;
    m_SensorLimits = 0;
}

//=========================================================================

asset_editor::asset_editor ( void )
{
    g_SelectedAssetType = 0;
    g_StormSelected = 1;
    g_bNeutral = FALSE;
    g_SelectedPriority = 0;

    InitClass();
}

//=========================================================================

asset_editor::~asset_editor( void )
{
    InitClass();
}

//=========================================================================

xbool asset_editor::AddInven                ( void )
{
    team_assets* Team;
    if (g_StormSelected)
        Team = &m_StormAssets;
    else
        Team = &m_InfernoAssets;

    if (g_SelectedAsset == NULL && Team->m_nInvens >= asset_spot::MAX_INVENS)
    {
        MessageBox(NULL, "Maximum Deployable Invens Reached", "Deploy Error", MB_OK);
        return FALSE;
    }

    collider                Segment;
    collider::collision     Collision;
    radian3                 VRot;
    f32                     VFOV;
    vector3                 VZ, VP;

    ASSERT(g_pPlayer);
    g_pPlayer->Get1stPersonView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );
    VZ *= 8.0;//DEPLOY_DIST; - defined in PlayerLogic.cpp

    // check for collision with world
    Segment.RaySetup( g_pPlayer, VP, VP+VZ );
    ObjMgr.Collide( object::ATTR_SOLID_STATIC, Segment );

    if( Segment.HasCollided() )
    {
        Segment.GetCollision( Collision );

        // If slope of surfce is worse than 30 degrees from vertical.
        if( Collision.Plane.Normal.Y < 0.8660f )
        {
//            MessageBox(NULL, "Terrain Too Steep for Inven", "Deploy Error", MB_OK);
            return FALSE;
        }
        if (g_SelectedAsset == NULL)
        {
            // No duplicates
            s32 i;
            for (i = 0; i < Team->m_nInvens; i++)
            {
                if ((Collision.Point - Team->m_pInvenList[i].GetAssetPos()).LengthSquared() < 1.0)
                {
                    MessageBox(NULL, "Already have Inven Station there!", "Deploy Error", MB_OK);
                    return FALSE;
                }
            }
        }
        /*
        station* pStation = (station*)ObjMgr.CreateObject( object::TYPE_STATION_DEPLOYED );
        pStation->Initialize( Collision.Point,
                              VRot.Yaw,
                              g_pPlayer->GetObjectID(),
                              g_pPlayer->GetTeamBits() );
        ObjMgr.AddObject( pStation, obj_mgr::AVAILABLE_SERVER_SLOT );
        */
        if (GameMgr.AttemptDeployStation(g_pPlayer->GetObjectID(),
            Collision.Point, Collision.Plane.Normal,
            ((object*)Collision.pObjectHit)->GetType()))
        {
            station* pStation;
            ObjMgr.StartTypeLoop(object::TYPE_STATION_DEPLOYED);
            while ((pStation = (station*)ObjMgr.GetNextInType()) != NULL)
            {
                if (pStation->GetPosition() == Collision.Point)
                    break;
            }
            ObjMgr.EndTypeLoop();
            ASSERT(pStation);

            if (g_SelectedAsset)
                DeleteSelected();

            RegisterInven( (asset*)pStation, VRot.Yaw );
            if (g_bNeutral)
            {
                g_StormSelected ^= 1;
                RegisterInven( (asset*)pStation, VRot.Yaw );
            }
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================

xbool asset_editor::AddTurret                ( void )
{
    team_assets* Team;
    if (g_StormSelected)
        Team = &m_StormAssets;
    else
        Team = &m_InfernoAssets;

    if (g_SelectedAsset == NULL && Team->m_nTurrets >= asset_spot::MAX_TURRETS)
    {
        MessageBox(NULL, "Maximum Deployable Turrets Reached", "Deploy Error", MB_OK);
        return FALSE;
    }

    collider                Segment;
    collider::collision     Collision;
    radian3                 VRot;
    f32                     VFOV;
    vector3                 VZ, VP;
    vector3                 TurretLoc;
    g_pPlayer->Get1stPersonView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );
    VZ *= 8.0f;//DEPLOY_DIST; - defined in PlayerLogic.cpp

    // check for collision with world
    Segment.RaySetup( g_pPlayer, VP, VP+VZ );
    ObjMgr.Collide( object::ATTR_SOLID_STATIC, Segment );

    if( Segment.HasCollided() )
    {
        Segment.GetCollision( Collision );

#if 0
        VZ.Normalize();
        // Check the angle of the collision.
        if (VZ.Dot(Collision.Plane.Normal) < 0.3 && VZ.Dot(Collision.Plane.Normal) > -0.3  )
        {
            x_printf("%2.6f",VZ.Dot(Collision.Plane.Normal));
            x_printf("*** Turret placement angle too steep ***\nMake it easier for the bot, give him a flat surface to aim at.");
            return FALSE;
        }

        // Check turret spacing
        for (s32 i = 0; i < Team->m_nTurrets; i++)
        {
            TurretLoc = Team->m_pTurretList[i].AssetPos;
            if ((Collision.Point - TurretLoc).LengthSquared() < 400)
            {
                x_printf("*** Turrets Cannot Be Deployed Within 20 meters of eachother.***");
                return FALSE;
            }
        }
#endif
        if (!g_SelectedAsset)
        {
            s32 i;
            for (i = 0; i < Team->m_nTurrets; i++)
            {
                if ((Collision.Point - Team->m_pTurretList[i].GetAssetPos()).LengthSquared() < 0.1)
                {
                    MessageBox(NULL, "Already have Turret there!", "Deploy Error", MB_OK);
                    return FALSE;
                }
            }
        }
/*
        turret* pTurret = (turret*)ObjMgr.CreateObject( object::TYPE_TURRET_CLAMP );
        pTurret->Initialize( Collision.Point,
                             Collision.Plane.Normal,
                             g_pPlayer->GetObjectID(),
                             g_pPlayer->GetTeamBits() );
        ObjMgr.AddObject( pTurret, obj_mgr::AVAILABLE_SERVER_SLOT );

*/       
        if (GameMgr.AttemptDeployTurret(g_pPlayer->GetObjectID(),
                                    Collision.Point,
                                    Collision.Plane.Normal,
                                    ((object*)Collision.pObjectHit)->GetType()))
        {
            turret *pTurret;
            ObjMgr.StartTypeLoop(object::TYPE_TURRET_CLAMP);
            while ((pTurret = (turret*)ObjMgr.GetNextInType()) != NULL)
            {
                if (pTurret->GetPosition() == Collision.Point)
                    break;
            }
            ObjMgr.EndTypeLoop();
            ASSERT(pTurret);
            if (g_SelectedAsset)
                DeleteSelected();

            RegisterTurret(pTurret, Collision.Plane.Normal);
            if (g_bNeutral)
            {
                g_StormSelected ^= 1;
                RegisterTurret(pTurret, Collision.Plane.Normal);
            }

            return TRUE;
        }
    } 
    return FALSE;
}


//==============================================================================

xbool asset_editor::AddSensor                ( void )
{
    team_assets* Team;
    if (g_StormSelected)
        Team = &m_StormAssets;
    else
        Team = &m_InfernoAssets;

    if (g_SelectedAsset == NULL && Team->m_nSensors >= asset_spot::MAX_SENSORS)
    {
        MessageBox(NULL, "Maximum Deployable Sensors Reached", "Deploy Error", MB_OK);
        return FALSE;
    }

    collider                Segment;
    collider::collision     Collision;
    radian3                 VRot;
    f32                     VFOV;
    vector3                 VZ, VP;
    vector3                 SensorLoc;
    g_pPlayer->Get1stPersonView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );
    VZ *= 8.0f;//DEPLOY_DIST; - defined in PlayerLogic.cpp

    // check for collision with world
    Segment.RaySetup( g_pPlayer, VP, VP+VZ );
    ObjMgr.Collide( object::ATTR_SOLID_STATIC, Segment );

    if( Segment.HasCollided() )
    {
        Segment.GetCollision( Collision );

        // If slope of surfce is worse than 30 degrees from vertical.
        if( Collision.Plane.Normal.Y < 0.8660f )
        {
            MessageBox(NULL, "Terrain Too Steep for Sensor", "Deploy Error", MB_OK);
            return FALSE;
        }

        if (!g_SelectedAsset)
        {
            s32 i;
            for (i = 0; i < Team->m_nSensors; i++)
            {
                if ((Collision.Point - Team->m_pSensorList[i].GetAssetPos()).LengthSquared() < 1.0)
                {
                    MessageBox(NULL, "Already have Sensor there!", "Deploy Error", MB_OK);
                    return FALSE;
                }
            }
        }
/*
        sensor* pSensor = (sensor*)ObjMgr.CreateObject( object::TYPE_SENSOR_REMOTE );
        pSensor->Initialize( Collision.Point,
                             Collision.Plane.Normal,
                             g_pPlayer->GetTeamBits() );
        ObjMgr.AddObject( pSensor, obj_mgr::AVAILABLE_SERVER_SLOT );
*/
        if (GameMgr.AttemptDeploySensor(g_pPlayer->GetObjectID(),
                            Collision.Point, 
                            Collision.Plane.Normal,
                            ((object*)Collision.pObjectHit)->GetType()))
        {
            sensor *pSensor;
    
            ObjMgr.StartTypeLoop(object::TYPE_SENSOR_REMOTE);
            while ((pSensor = (sensor*)ObjMgr.GetNextInType()) != NULL)
            {
                if (pSensor->GetPosition() == Collision.Point)
                    break;
            }
            ObjMgr.EndTypeLoop();
            ASSERT(pSensor);

            if (g_SelectedAsset)
                DeleteSelected();
            RegisterSensor(pSensor, Collision.Plane.Normal);
            if (g_bNeutral)
            {
                g_StormSelected ^= 1;
                RegisterSensor(pSensor, Collision.Plane.Normal);
            }

            return TRUE;
        }
    } 
    return FALSE;
}

//==============================================================================

xbool asset_editor::AddMine               ( void )
{
    team_assets* Team;
    if (g_StormSelected)
        Team = &m_StormAssets;
    else
        Team = &m_InfernoAssets;

    if (Team->m_nMines >= asset_spot::MAX_MINES)
    {
        MessageBox(NULL, "Maximum Deployable Mines Reached", "Deploy Error", MB_OK);
        return FALSE;
    }

    s32 i;
    for (i = 0; i < Team->m_nMines; i++)
    {
        if (   g_pPlayer->GetPosition() == Team->m_pMineList[i].GetAssetPos()
            && g_pPlayer->GetRot() == Team->m_pMineList[i].GetPlayerRot()    )
        {
            MessageBox(NULL, "Already have Mine there!", "Deploy Error", MB_OK);
            return FALSE;
        }
    }

    g_DeployMine = TRUE;

    // Add mine to team list.
    s32 nMines = Team->m_nMines;
    Team->m_pMineList[nMines].Set(  g_pPlayer->GetPosition(),
                                    g_pPlayer->GetRot(),
    (asset_spot::priority_value)(s32)(x_pow(3.0f,(f32)(2-g_SelectedPriority))),
    (g_bNeutral?asset_spot::TEAM_NEUTRAL:0));
    Team->m_nMines++;
    
    x_printf("%s Mine deployment recorded (%d of %d)\n", g_TeamName[(s32)Team->m_Team], Team->m_nMines, asset_spot::MAX_MINES);

    if (g_bNeutral)
    {
        if (g_StormSelected)
            Team = &m_InfernoAssets;
        else
            Team = &m_StormAssets;

        nMines = Team->m_nMines;

        s32 i;
        for (i = 0; i < nMines; i++)
        {
            if (   g_pPlayer->GetPosition() == Team->m_pMineList[i].GetAssetPos()
                && g_pPlayer->GetRot() == Team->m_pMineList[i].GetPlayerRot()    )
            {
                MessageBox(NULL, "Already have Mine there!", "Deploy Error", MB_OK);
                return FALSE;
            }
        }

        Team->m_pMineList[nMines].Set(  g_pPlayer->GetPosition(),
                                        g_pPlayer->GetRot(),
        (asset_spot::priority_value)(s32)(x_pow(3.0f,(f32)(2-g_SelectedPriority))),
        (g_bNeutral?asset_spot::TEAM_NEUTRAL:0));
        Team->m_nMines++;
    
        x_printf("%s Mine deployment recorded (%d of %d)\n", g_TeamName[(s32)Team->m_Team], Team->m_nMines, asset_spot::MAX_MINES);


    }

    g_SelectedAsset = NULL;
    g_AssetsChanged = TRUE;
    return TRUE;
}

//==============================================================================

void  asset_editor::RegisterInven( asset* Asset, radian Yaw )
{
    team_assets *TeamAssets;
    if (g_StormSelected)
    {
        TeamAssets = &m_StormAssets;
    }
    else
    {
        TeamAssets = &m_InfernoAssets;
    }

    g_SelectedAsset = NULL;
    g_AssetsChanged = TRUE;

    s32 nInvens = TeamAssets->m_nInvens;
    TeamAssets->m_pInvenList[nInvens].Set(g_pPlayer->GetPosition(),
                                Asset->GetPosition(),
                                Yaw,
    (asset_spot::priority_value)(s32)(x_pow(3.0f,(f32)(2-g_SelectedPriority))),
    (g_bNeutral?asset_spot::TEAM_NEUTRAL:0));
    TeamAssets->m_nInvens++;

    x_printf("%s Inventory Station deployment recorded (%d of %d)\n", 
                g_TeamName[(s32)TeamAssets->m_Team], TeamAssets->m_nInvens, asset_spot::MAX_INVENS);
}

//==============================================================================

void  asset_editor::RegisterTurret( asset* Asset, vector3& Normal)
{
    team_assets *TeamAssets;
    if (g_StormSelected)
    {
        TeamAssets = &m_StormAssets;
    }
    else
    {
        TeamAssets = &m_InfernoAssets;
    }

    g_SelectedAsset = NULL;
    g_AssetsChanged = TRUE;

    s32 nTurrets = TeamAssets->m_nTurrets;
    TeamAssets->m_pTurretList[nTurrets].Set(g_pPlayer->GetPosition(),
                                    Asset->GetPosition(),
                                    Normal,
    (asset_spot::priority_value)(s32)(x_pow(3.0f,(f32)(2-g_SelectedPriority))),
    (g_bNeutral?asset_spot::TEAM_NEUTRAL:0));
    TeamAssets->m_nTurrets++;
    x_printf("%s Turret deployment recorded (%d of %d)\n", 
                g_TeamName[(s32)TeamAssets->m_Team], TeamAssets->m_nTurrets, asset_spot::MAX_TURRETS);
}

//==============================================================================

void  asset_editor::RegisterSensor( asset* Asset, vector3& Normal)
{
    team_assets *TeamAssets;
    if (g_StormSelected)
    {
        TeamAssets = &m_StormAssets;
    }
    else
    {
        TeamAssets = &m_InfernoAssets;
    }

    g_SelectedAsset = NULL;
    g_AssetsChanged = TRUE;

    s32 nSensors = TeamAssets->m_nSensors;
    TeamAssets->m_pSensorList[nSensors].Set(g_pPlayer->GetPosition(),
                                    Asset->GetPosition(),
                                    Normal,
    (asset_spot::priority_value)(s32)(x_pow(3.0f,(f32)(2-g_SelectedPriority))),
    (g_bNeutral?asset_spot::TEAM_NEUTRAL:0));
    TeamAssets->m_nSensors++;
    x_printf("%s Sensor deployment recorded (%d of %d)\n", 
                g_TeamName[(s32)TeamAssets->m_Team], TeamAssets->m_nSensors, asset_spot::MAX_SENSORS);
}

//==============================================================================

const inven_spot* asset_editor::GetClosestInven(   const vector3& Pos, 
                                          asset_spot::team     Team,
                                                   f32&     Dist )
{
    team_assets *TeamAssets;
    if (Team == asset_spot::TEAM_STORM)
    {
        TeamAssets = &m_StormAssets;
    }
    else if (Team == asset_spot::TEAM_INFERNO)
    {
        TeamAssets = &m_InfernoAssets;
    }
    else ASSERT(0 && "Invalid Team!");

    s32 Count   = TeamAssets->m_nInvens;
    f32 MinDist = F32_MAX;
    s32 Index   = -1;
    vector3 V;
    f32 d;

    for( s32 i=0; i<Count; i++ )
    {
        inven_spot Inven = TeamAssets->m_pInvenList[i];
        V = Inven.GetAssetPos() - Pos;
        d = V.Dot( V );

        if( d < MinDist )
        {
            MinDist = d;
            Index = i;
        }
    }

    if (Index != -1)
    {
        Dist = MinDist;
        return &TeamAssets->m_pInvenList[Index];
    }
    else
        return NULL;
}

//==============================================================================

const turret_spot* asset_editor::GetClosestTurret(   const vector3& Pos, 
                                            asset_spot::team     Team,
                                                   f32&     Dist )
{
    team_assets *TeamAssets;
    if (Team == asset_spot::TEAM_STORM)
    {
        TeamAssets = &m_StormAssets;
    }
    else if (Team == asset_spot::TEAM_INFERNO)
    {
        TeamAssets = &m_InfernoAssets;
    }
    else ASSERT(0 && "Invalid Team!");

    s32 Count   = TeamAssets->m_nTurrets;
    f32 MinDist = F32_MAX;
    s32 Index   = -1;
    vector3 V;
    f32 d;

    for( s32 i=0; i<Count; i++ )
    {
        turret_spot Turret = TeamAssets->m_pTurretList[i];
        V = Turret.GetAssetPos() - Pos;
        d = V.Dot( V );

        if( d < MinDist )
        {
            MinDist = d;
            Index = i;
        }
    }

    if (Index != -1)
    {
        Dist = MinDist;
        return &TeamAssets->m_pTurretList[Index];
    }
    else
        return NULL;
}

//==============================================================================

const sensor_spot* asset_editor::GetClosestSensor(   const vector3& Pos, 
                                            asset_spot::team     Team,
                                                   f32&     Dist )
{
    team_assets *TeamAssets;
    if (Team == asset_spot::TEAM_STORM)
    {
        TeamAssets = &m_StormAssets;
    }
    else if (Team == asset_spot::TEAM_INFERNO)
    {
        TeamAssets = &m_InfernoAssets;
    }
    else ASSERT(0 && "Invalid Team!");

    s32 Count   = TeamAssets->m_nSensors;
    f32 MinDist = F32_MAX;
    s32 Index   = -1;
    vector3 V;
    f32 d;

    for( s32 i=0; i<Count; i++ )
    {
        sensor_spot Sensor = TeamAssets->m_pSensorList[i];
        V = Sensor.GetAssetPos() - Pos;
        d = V.Dot( V );

        if( d < MinDist )
        {
            MinDist = d;
            Index = i;
        }
    }

    if (Index != -1)
    {
        Dist = MinDist;
        return &TeamAssets->m_pSensorList[Index];
    }
    else
        return NULL;
}

//==============================================================================

const mine_spot* asset_editor::GetClosestMine(   const vector3& Pos, 
                                        asset_spot::team     Team,
                                                   f32&     Dist )

{
    team_assets *TeamAssets;
    if (Team == asset_spot::TEAM_STORM)
    {
        TeamAssets = &m_StormAssets;
    }
    else if (Team == asset_spot::TEAM_INFERNO)
    {
        TeamAssets = &m_InfernoAssets;
    }
    else ASSERT(0 && "Invalid Team!");

    s32 Count   = TeamAssets->m_nMines;
    f32 MinDist = F32_MAX;
    s32 Index   = -1;
    vector3 V;
    f32 d;

    for( s32 i=0; i<Count; i++ )
    {
        mine_spot Mine = TeamAssets->m_pMineList[i];
        V = (Mine.GetStandPos() + vector3(Mine.GetPlayerRot()) ) - Pos;
        d = V.Dot( V );

        if( d < MinDist )
        {
            MinDist = d;
            Index = i;
        }
    }

    if (Index != -1)
    {
        Dist = MinDist;
        return &TeamAssets->m_pMineList[Index];
    }
    else
        return NULL;
}

//==============================================================================

const vector3* asset_editor::GetClosestSP(   const vector3& Pos, 
                                        asset_spot::team     Team,
                                                   f32&     Dist )

{
    team_assets *TeamAssets;
    if (Team == asset_spot::TEAM_STORM)
    {
        TeamAssets = &m_StormAssets;
    }
    else if (Team == asset_spot::TEAM_INFERNO)
    {
        TeamAssets = &m_InfernoAssets;
    }
    else ASSERT(0 && "Invalid Team!");

    s32 Count   = TeamAssets->m_nSnipePoints;
    f32 MinDist = F32_MAX;
    s32 Index   = -1;
    vector3 V;
    f32 d;

    for( s32 i=0; i<Count; i++ )
    {
        V = TeamAssets->m_pSnipePoint[i] - Pos;
        d = V.Dot( V );

        if( d < MinDist )
        {
            MinDist = d;
            Index = i;
        }
    }

    if (Index != -1)
    {
        Dist = MinDist;
        return &TeamAssets->m_pSnipePoint[Index];
    }
    else
        return NULL;
}

//==============================================================================

const void* asset_editor::GetNextBadAsset( void )
{
    team_assets* CurrTeam = &m_StormAssets;
    s32 i,j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < CurrTeam->m_nInvens; j++)
        {
            if (CurrTeam->m_pInvenList[j].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                // Set up the asset type.

                g_SelectedAssetType = 4;
                g_SelectedPriority = (CurrTeam->m_pInvenList[j].GetPriority() == 9 ? 2 : 
                (CurrTeam->m_pInvenList[j].GetPriority() == 3 ? 0 : 1));
                
                if (CurrTeam->m_pInvenList[j].GetFlags()& asset_spot::TEAM_NEUTRAL)
                {
                    g_bNeutral = FALSE; // i know, this looks wrong, but it's right.
                    g_StormSelected = FALSE;

                    // Neutral- set up alt
                    ASSERT(CurrTeam == &m_StormAssets);
                    f32 Dist;
                    g_AltSelected = GetClosestInven(CurrTeam->m_pInvenList[j].GetAssetPos(), 
                        asset_spot::TEAM_INFERNO, Dist);
                }
                else if ( i == 0)
                {
                    g_bNeutral = TRUE;
//                    g_StormSelected = DOESN'T MATTER.;
                }
                else
                {
                    g_bNeutral = FALSE;
                    g_StormSelected = TRUE;
                }
                
                g_pPlayer->SetPos(CurrTeam->m_pInvenList[j].GetStandPos());
                g_pPlayer->SetRot(radian3((CurrTeam->m_pInvenList[j].GetAssetPos() - 
                            CurrTeam->m_pInvenList[j].GetStandPos()).GetPitch(),
                            (CurrTeam->m_pInvenList[j].GetAssetPos() - 
                            CurrTeam->m_pInvenList[j].GetStandPos()).GetYaw(),
                            0));

                SelectAsset(g_SelectedAssetType);
                SelectPriority(g_SelectedPriority);
                SelectTeam(g_StormSelected, g_bNeutral);

                return &CurrTeam->m_pInvenList[j];
            }
        }
        
        for (j = 0; j < CurrTeam->m_nTurrets; j++)
        {
            if (CurrTeam->m_pTurretList[j].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                // Set up the asset type.

                g_SelectedAssetType = 0;
                g_SelectedPriority = (CurrTeam->m_pTurretList[j].GetPriority() == 9 ? 2 : 
                (CurrTeam->m_pTurretList[j].GetPriority() == 3 ? 0 : 1));
                if (CurrTeam->m_pTurretList[j].GetFlags()& asset_spot::TEAM_NEUTRAL)
                {
                    g_bNeutral = FALSE; // i know, this looks wrong, but it's right.
                    g_StormSelected = FALSE;

                    // Neutral- set up alt
                    ASSERT(CurrTeam == &m_StormAssets);
                    f32 Dist;
                    g_AltSelected = GetClosestTurret(CurrTeam->m_pTurretList[j].GetAssetPos(), 
                        asset_spot::TEAM_INFERNO, Dist);
                }
                else if ( i == 0)
                {
                    g_bNeutral = TRUE;
//                    g_StormSelected = DOESN'T MATTER.;
                }
                else
                {
                    g_bNeutral = FALSE;
                    g_StormSelected = TRUE;
                }
                
                g_pPlayer->SetPos(CurrTeam->m_pTurretList[j].GetStandPos());
                g_pPlayer->SetRot(radian3(
                            (CurrTeam->m_pTurretList[j].GetAssetPos() - 
                            CurrTeam->m_pTurretList[j].GetStandPos()).GetPitch(),
                            (CurrTeam->m_pTurretList[j].GetAssetPos() - 
                            CurrTeam->m_pTurretList[j].GetStandPos()).GetYaw(),
                            0));
                
                SelectAsset(g_SelectedAssetType);
                SelectPriority(g_SelectedPriority);
                SelectTeam(g_StormSelected, g_bNeutral);

                return &CurrTeam->m_pTurretList[j];
            }
        }
        
        for (j = 0; j < CurrTeam->m_nSensors; j++)
        {
            if (CurrTeam->m_pSensorList[j].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                // Set up the asset type.

                g_SelectedAssetType = 1;
                g_SelectedPriority = (CurrTeam->m_pSensorList[j].GetPriority() == 9 ? 2 : 
                (CurrTeam->m_pSensorList[j].GetPriority() == 3 ? 0 : 1));
                if (CurrTeam->m_pSensorList[j].GetFlags()& asset_spot::TEAM_NEUTRAL)
                {
                    g_bNeutral = FALSE; // i know, this looks wrong, but it's right.
                    g_StormSelected = FALSE;
                    // Neutral- set up alt
                    ASSERT(CurrTeam == &m_StormAssets);
                    f32 Dist;
                    g_AltSelected = GetClosestSensor(CurrTeam->m_pSensorList[j].GetAssetPos(), 
                        asset_spot::TEAM_INFERNO, Dist);
                }
                else if ( i == 0)
                {
                    g_bNeutral = TRUE;
//                    g_StormSelected = DOESN'T MATTER.;
                }
                else
                {
                    g_bNeutral = FALSE;
                    g_StormSelected = TRUE;
                }
                g_pPlayer->SetPos(CurrTeam->m_pSensorList[j].GetStandPos());
                g_pPlayer->SetRot(radian3(
                    (CurrTeam->m_pSensorList[j].GetAssetPos() - 
                            CurrTeam->m_pSensorList[j].GetStandPos()).GetPitch(),
                            (CurrTeam->m_pSensorList[j].GetAssetPos() - 
                            CurrTeam->m_pSensorList[j].GetStandPos()).GetYaw(),
                            0));
                
                SelectAsset(g_SelectedAssetType);
                SelectPriority(g_SelectedPriority);
                SelectTeam(g_StormSelected, g_bNeutral);

                return &CurrTeam->m_pSensorList[j];
            }
        }
        CurrTeam = &m_InfernoAssets;
    }  
    return NULL;
}

//==============================================================================

const void* asset_editor::GetClosestAsset(const vector3& Pos)
{
    const s32 NUM_ASS = 10;
    const void* ClosestAsset[NUM_ASS];
    f32         ClosestDist [NUM_ASS] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 
                                         -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, };

    asset_spot::team Team = asset_spot::TEAM_STORM;
    s32 i;
    for (i = 0; i < NUM_ASS; i+=NUM_ASS/2)
    {
        ClosestAsset[i + 0] = GetClosestInven (Pos, Team, ClosestDist[i + 0]);
        ClosestAsset[i + 1] = GetClosestTurret(Pos, Team, ClosestDist[i + 1]);
        ClosestAsset[i + 2] = GetClosestSensor(Pos, Team, ClosestDist[i + 2]);
        ClosestAsset[i + 3] = GetClosestMine  (Pos, Team, ClosestDist[i + 3]);
        ClosestAsset[i + 4] = GetClosestSP    (Pos, Team, ClosestDist[i + 4]);

        Team = asset_spot::TEAM_INFERNO;
    }

    f32 BestDist = F32_MAX;
    s32 BestAsset = -1;
    for (i = 0; i < NUM_ASS; i++)
    {
        if (ClosestDist[i] != -1 && ClosestDist[i] < BestDist)
        {
            BestAsset = i;
            BestDist = ClosestDist[i];
        }
    }

    if (BestAsset != -1)
    {
        if (BestAsset < 5 &&
            ClosestDist[BestAsset] == ClosestDist[BestAsset + 5])
        {
            // Looks like a neutral asset.
            g_AltSelected = ClosestAsset[BestAsset+5];
        }
        else
            g_AltSelected = NULL;

        g_SAType = BestAsset%5;
        return ClosestAsset[BestAsset];
    }
    else
        return NULL;
}

//==============================================================================

void asset_editor::DeleteSelected ( void )
{
    if (!g_SelectedAsset)
    {
        MessageBox(NULL, "No asset selected!", "DELETE ERROR", MB_OK);
        return;
    }

    team_assets *TeamAssets = &m_StormAssets;

    s32 j;
    for (j = 0; j < 2; j++)
    {
        if (AttemptDeletion(TeamAssets->m_pInvenList, TeamAssets->m_nInvens))
        {
            x_printf("** %s Inventory station deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            TeamAssets = &m_InfernoAssets;
            g_SelectedAsset = g_AltSelected;
            if (g_SelectedAsset)
            {
                DEMAND(AttemptDeletion(TeamAssets->m_pInvenList, TeamAssets->m_nInvens));
                x_printf("** %s Inventory station deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            }
            break;
        }
        if (AttemptDeletion(TeamAssets->m_pTurretList, TeamAssets->m_nTurrets))
        {
            x_printf("** %s Turret deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            TeamAssets = &m_InfernoAssets;
            g_SelectedAsset = g_AltSelected;
            if (g_SelectedAsset)
            {
                DEMAND(AttemptDeletion(TeamAssets->m_pTurretList, TeamAssets->m_nTurrets));
                x_printf("** %s Turret deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            }
            break;
        }
        if (AttemptDeletion(TeamAssets->m_pSensorList, TeamAssets->m_nSensors))
        {
            x_printf("** %s Sensor deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            TeamAssets = &m_InfernoAssets;
            g_SelectedAsset = g_AltSelected;
            if (g_SelectedAsset)
            {
                DEMAND(AttemptDeletion(TeamAssets->m_pSensorList, TeamAssets->m_nSensors));
                x_printf("** %s Sensor deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            }
            break;
        }
        if (AttemptDeletion(TeamAssets->m_pMineList, TeamAssets->m_nMines))
        {
            x_printf("** %s Mine deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            TeamAssets = &m_InfernoAssets;
            g_SelectedAsset = g_AltSelected;
            if (g_SelectedAsset)
            {
                DEMAND(AttemptDeletion(TeamAssets->m_pMineList, TeamAssets->m_nMines));
                x_printf("** %s Mine deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            }
            break;
        }
        if (AttemptDeletion(TeamAssets->m_pSnipePoint, TeamAssets->m_nSnipePoints))
        {
            x_printf("** %s Snipe Point deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            TeamAssets = &m_InfernoAssets;
            g_SelectedAsset = g_AltSelected;
            if (g_SelectedAsset)
            {
                DEMAND(AttemptDeletion(TeamAssets->m_pSnipePoint, TeamAssets->m_nSnipePoints));
                x_printf("** %s Snipe Point deleted **", g_TeamName[(s32)TeamAssets->m_Team]);
            }
            break;
        }
        TeamAssets = &m_InfernoAssets;
    }
    g_AssetsChanged = TRUE;
    ASSERT(g_SelectedAsset == NULL);
}

//==============================================================================

xbool asset_editor::AttemptDeletion ( void* List, s32& Size )
{
    s32 i,k;

    // This is kinda a hack
    xbool bI = FALSE;
    xbool bT = FALSE;
    xbool bS = FALSE;
    xbool bM = FALSE;
    xbool bSP = FALSE;

    if (List == m_StormAssets.m_pInvenList
        || List == m_InfernoAssets.m_pInvenList)
        bI = TRUE;
    else if (List == m_StormAssets.m_pTurretList
        || List == m_InfernoAssets.m_pTurretList)
        bT = TRUE;
    else if (List == m_StormAssets.m_pSensorList
        || List == m_InfernoAssets.m_pSensorList)
        bS = TRUE;
    else if (List == m_StormAssets.m_pMineList
        || List == m_InfernoAssets.m_pMineList)
        bM = TRUE;
    else if (List == m_StormAssets.m_pSnipePoint
        || List == m_InfernoAssets.m_pSnipePoint)
        bSP = TRUE;
    else
        ASSERT(FALSE && "invalid list address");


    for (i = 0; i < Size; i++)
    {
        if (    bI && &(((inven_spot*)List)[i]) == g_SelectedAsset
            ||  bT && &(((turret_spot*)List)[i]) == g_SelectedAsset
            ||  bS && &(((sensor_spot*)List)[i]) == g_SelectedAsset
            ||  bM && &(((mine_spot*)List)[i]) == g_SelectedAsset
            ||  bSP && &(((vector3*)List)[i]) == g_SelectedAsset)
        {
            if (!bSP)
            {
                asset* Asset = NULL;

                // Remove the object (mines are not removed- we don't know where they are exactly)
                ObjMgr.Select(object::ATTR_ASSET, ((asset_spot*)g_SelectedAsset)->GetStandPos(), 8.0f);
                while ((Asset = (asset*)ObjMgr.GetNextSelected()) != NULL)
                {
                    vector3 AssetPos;
                    if (bI)
                    {
                        inven_spot* Spot = (inven_spot*)g_SelectedAsset;
                        AssetPos = Spot->GetAssetPos();
                    }
                    else if (bT)
                    {
                        turret_spot* Spot = (turret_spot*)g_SelectedAsset;
                        AssetPos = Spot->GetAssetPos();
                    }
                    else if (bS)
                    {
                        sensor_spot* Spot = (sensor_spot*)g_SelectedAsset;
                        AssetPos = Spot->GetAssetPos();
                    }
                    else
                    {
                        mine_spot* Spot = (mine_spot*)g_SelectedAsset;
                        AssetPos = Spot->GetAssetPos() + vector3(Spot->GetPlayerRot());
                    }
    
                    if (Asset->GetPosition() == AssetPos)
                    {
                        ObjMgr.RemoveObject(Asset);
                        ObjMgr.DestroyObject(Asset);
                    }
                }
                ObjMgr.ClearSelection();
            }
            ASSERT(g_SelectedAsset);
            g_SelectedAsset = NULL;
            // Shift all entries up
            for (k = i+1; k < Size; k++)
            {
                if (bI)
                    ((inven_spot*)List)[k - 1] = ((inven_spot*)List)[k];
                else if (bT)
                    ((turret_spot*)List)[k - 1] = ((turret_spot*)List)[k];
                else if (bS)
                    ((sensor_spot*)List)[k - 1] = ((sensor_spot*)List)[k];
                else if (bM)
                    ((mine_spot*)List)[k - 1] = ((mine_spot*)List)[k];
                else
                    ((vector3*)List)[k - 1] = ((vector3*)List)[k];
            }
                
            if (bI)
                ((inven_spot*)List)[Size - 1].Clear();
            else if (bT)
                ((turret_spot*)List)[Size - 1].Clear();
            else if (bS)
                ((sensor_spot*)List)[Size - 1].Clear();
            else if (bM)
                ((mine_spot*)List)[Size - 1].Clear();
            else if (bSP)
                ((vector3*)List)[Size - 1].Zero();

            Size--;
            return TRUE;
        }

    }
    return FALSE;
}

//==============================================================================

void asset_editor::Render( void )
{
    const view&     View        = *eng_GetView(0);
    eng_Begin();

    s32 i, nInvens, nTurrets, nSensors, nMines, Priority;
    vector3 PlayerPos;
    vector3 AssetPos;
    team_assets *TeamAssets = &m_StormAssets;
    vector3 MinVector, MaxVector;
    const vector3 BBoxSize( 0.25f,0.25f,0.25f );
    xcolor Color;

    const char PriorityLetter[15] = "?L?M?????H????";

    s32 BadInvens = 0;
    s32 BadTurrets = 0;
    s32 BadSensors = 0;
    for (s32 j = 0; j < 2; j++)
    {
        nInvens     = TeamAssets->m_nInvens;
        nTurrets    = TeamAssets->m_nTurrets;
        nMines      = TeamAssets->m_nMines;
        nSensors    = TeamAssets->m_nSensors;

        //
        // Render the inven data
        //
        for(i=0; i<nInvens; i++ )
        {
            if (TeamAssets->m_pInvenList[i].GetFlags() & asset_spot::TEAM_NEUTRAL)
            {
                if (j==0)
                    Color.Set(255,0,255);
                else
                    continue;
            }
            else
            {
                if (j==0)
                    Color.Set(0,0,255);
                else
                    Color.Set(255,0,0);
            }

            PlayerPos = TeamAssets->m_pInvenList[i].GetStandPos();
            AssetPos = TeamAssets->m_pInvenList[i].GetAssetPos();
            Priority = TeamAssets->m_pInvenList[i].GetPriority();

            draw_BBox( bbox( PlayerPos-BBoxSize, PlayerPos+BBoxSize), Color );
            draw_BBox( bbox( AssetPos-BBoxSize, AssetPos+BBoxSize), Color );

            if (TeamAssets->m_pInvenList[i].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                draw_Marker(AssetPos, XCOLOR_YELLOW);
                BadInvens++;
            }

            if ((View.GetPosition() - AssetPos).LengthSquared() < 100)
                draw_Label(AssetPos, XCOLOR_WHITE, "INVEN - %c - %c",
        TeamAssets->m_pInvenList[i].GetFlags()&asset_spot::TEAM_NEUTRAL?'N':(TeamAssets->m_Team ? 'I' : 'S'),
                    PriorityLetter[Priority]);

            if (&TeamAssets->m_pInvenList[i] == g_SelectedAsset)
            {
                bbox OverBox;
                OverBox.Min.X = MIN((vector3(PlayerPos-BBoxSize)).X, (vector3(AssetPos-BBoxSize)).X) - 0.1f; 
                OverBox.Min.Y = MIN((vector3(PlayerPos-BBoxSize)).Y, (vector3(AssetPos-BBoxSize)).Y) - 0.1f; 
                OverBox.Min.Z = MIN((vector3(PlayerPos-BBoxSize)).Z, (vector3(AssetPos-BBoxSize)).Z) - 0.1f; 
                OverBox.Max.X = MAX((vector3(PlayerPos+BBoxSize)).X, (vector3(AssetPos+BBoxSize)).X) + 0.1f; 
                OverBox.Max.Y = MAX((vector3(PlayerPos+BBoxSize)).Y, (vector3(AssetPos+BBoxSize)).Y) + 0.1f; 
                OverBox.Max.Z = MAX((vector3(PlayerPos+BBoxSize)).Z, (vector3(AssetPos+BBoxSize)).Z) + 0.1f; 
                draw_BBox(OverBox, XCOLOR_GREEN);

                draw_Line( PlayerPos, AssetPos, XCOLOR_GREEN);
            }
            else
                draw_Line( PlayerPos, AssetPos);
        }

        //
        // Render the turret data
        //
        for(i=0; i<nTurrets; i++ )
        {
            if (TeamAssets->m_pTurretList[i].GetFlags() & asset_spot::TEAM_NEUTRAL)
            {
                if (j==0)
                    Color.Set(255,0,255);
                else
                    continue;
            }
            else
            {
                if (j==0)
                    Color.Set(0,0,255);
                else
                    Color.Set(255,0,0);
            }

            PlayerPos = TeamAssets->m_pTurretList[i].GetStandPos();

            AssetPos = TeamAssets->m_pTurretList[i].GetAssetPos();
            Priority = TeamAssets->m_pTurretList[i].GetPriority();

            if (TeamAssets->m_pTurretList[i].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                draw_Marker(AssetPos, XCOLOR_YELLOW);
                BadTurrets++;
            }

            draw_BBox( bbox( PlayerPos-BBoxSize, PlayerPos+BBoxSize), Color );
            draw_BBox( bbox( AssetPos-BBoxSize, AssetPos+BBoxSize), Color );
            if ((View.GetPosition() - AssetPos).LengthSquared() < 225)
            {
                draw_Label(AssetPos, XCOLOR_WHITE, "TURRET - %c - %c",
        TeamAssets->m_pTurretList[i].GetFlags()&asset_spot::TEAM_NEUTRAL?'N':(TeamAssets->m_Team ? 'I' : 'S'),
                    PriorityLetter[Priority]);
            }

            if (&TeamAssets->m_pTurretList[i] == g_SelectedAsset)
            {
                bbox OverBox;
                OverBox.Min.X = MIN((vector3(PlayerPos-BBoxSize)).X, (vector3(AssetPos-BBoxSize)).X) - 0.1f; 
                OverBox.Min.Y = MIN((vector3(PlayerPos-BBoxSize)).Y, (vector3(AssetPos-BBoxSize)).Y) - 0.1f; 
                OverBox.Min.Z = MIN((vector3(PlayerPos-BBoxSize)).Z, (vector3(AssetPos-BBoxSize)).Z) - 0.1f; 
                OverBox.Max.X = MAX((vector3(PlayerPos+BBoxSize)).X, (vector3(AssetPos+BBoxSize)).X) + 0.1f; 
                OverBox.Max.Y = MAX((vector3(PlayerPos+BBoxSize)).Y, (vector3(AssetPos+BBoxSize)).Y) + 0.1f; 
                OverBox.Max.Z = MAX((vector3(PlayerPos+BBoxSize)).Z, (vector3(AssetPos+BBoxSize)).Z) + 0.1f; 
                draw_BBox(OverBox, XCOLOR_GREEN);

                draw_Line( PlayerPos, AssetPos, XCOLOR_GREEN);
            }
            else
                draw_Line( PlayerPos, AssetPos);
        }

        //
        // Render the sensor data
        //
        for(i=0; i<nSensors; i++ )
        {
            if (TeamAssets->m_pSensorList[i].GetFlags() & asset_spot::TEAM_NEUTRAL)
            {
                if (j==0)
                    Color.Set(255,0,255);
                else
                    continue;
            }
            else
            {
                if (j==0)
                    Color.Set(0,0,255);
                else
                    Color.Set(255,0,0);
            }

            PlayerPos = TeamAssets->m_pSensorList[i].GetStandPos();

            AssetPos = TeamAssets->m_pSensorList[i].GetAssetPos();
            Priority = TeamAssets->m_pSensorList[i].GetPriority();

            if (TeamAssets->m_pSensorList[i].GetFlags() & asset_spot::FLAG_BAD_SPOT)
            {
                draw_Marker(AssetPos, XCOLOR_YELLOW);
                BadSensors++;
            }

            draw_BBox( bbox( PlayerPos-BBoxSize, PlayerPos+BBoxSize), Color );
            draw_BBox( bbox( AssetPos-BBoxSize, AssetPos+BBoxSize), Color );
            if ((View.GetPosition() - AssetPos).LengthSquared() < 225)
            {
                draw_Label(AssetPos, XCOLOR_WHITE, "SENSOR - %c - %c",
TeamAssets->m_pSensorList[i].GetFlags()&asset_spot::TEAM_NEUTRAL?'N':(TeamAssets->m_Team ? 'I' : 'S'),
                                    PriorityLetter[Priority]);
            }

            if (&TeamAssets->m_pSensorList[i] == g_SelectedAsset)
            {
                bbox OverBox;
                OverBox.Min.X = MIN((vector3(PlayerPos-BBoxSize)).X, (vector3(AssetPos-BBoxSize)).X) - 0.1f; 
                OverBox.Min.Y = MIN((vector3(PlayerPos-BBoxSize)).Y, (vector3(AssetPos-BBoxSize)).Y) - 0.1f; 
                OverBox.Min.Z = MIN((vector3(PlayerPos-BBoxSize)).Z, (vector3(AssetPos-BBoxSize)).Z) - 0.1f; 
                OverBox.Max.X = MAX((vector3(PlayerPos+BBoxSize)).X, (vector3(AssetPos+BBoxSize)).X) + 0.1f; 
                OverBox.Max.Y = MAX((vector3(PlayerPos+BBoxSize)).Y, (vector3(AssetPos+BBoxSize)).Y) + 0.1f; 
                OverBox.Max.Z = MAX((vector3(PlayerPos+BBoxSize)).Z, (vector3(AssetPos+BBoxSize)).Z) + 0.1f; 
                draw_BBox(OverBox, XCOLOR_GREEN);

                draw_Line( PlayerPos, AssetPos, XCOLOR_GREEN);
            }
            else
                draw_Line( PlayerPos, AssetPos);
        }

        // Render the mine data
        //
        for(i=0; i<nMines; i++ )
        {
            if (TeamAssets->m_pMineList[i].GetFlags() & asset_spot::TEAM_NEUTRAL)
            {
                if (j==0)
                    Color.Set(255,0,255);
                else
                    continue;
            }
            else
            {
                if (j==0)
                    Color.Set(0,0,255);
                else
                    Color.Set(255,0,0);
            }

            PlayerPos = TeamAssets->m_pMineList[i].GetStandPos() + vector3(0.0f, 1.5f, 0.0f);
            AssetPos = PlayerPos + vector3(TeamAssets->m_pMineList[i].GetPlayerRot());
            Priority = TeamAssets->m_pMineList[i].GetPriority();

            draw_BBox( bbox( PlayerPos-BBoxSize, PlayerPos+BBoxSize), Color );

            if ((View.GetPosition() - AssetPos).LengthSquared() < 100)
                draw_Label(AssetPos, XCOLOR_WHITE, "MINE - %c - %c", 
                TeamAssets->m_pMineList[i].GetFlags()&asset_spot::TEAM_NEUTRAL?'N':(TeamAssets->m_Team ? 'I' : 'S'),
                PriorityLetter[Priority]);

            if (&TeamAssets->m_pMineList[i] == g_SelectedAsset)
            {
                bbox OverBox;
                OverBox.Min.X = MIN((vector3(PlayerPos-BBoxSize)).X, (vector3(AssetPos-BBoxSize)).X) - 0.1f; 
                OverBox.Min.Y = MIN((vector3(PlayerPos-BBoxSize)).Y, (vector3(AssetPos-BBoxSize)).Y) - 0.1f; 
                OverBox.Min.Z = MIN((vector3(PlayerPos-BBoxSize)).Z, (vector3(AssetPos-BBoxSize)).Z) - 0.1f; 
                OverBox.Max.X = MAX((vector3(PlayerPos+BBoxSize)).X, (vector3(AssetPos+BBoxSize)).X) + 0.1f; 
                OverBox.Max.Y = MAX((vector3(PlayerPos+BBoxSize)).Y, (vector3(AssetPos+BBoxSize)).Y) + 0.1f; 
                OverBox.Max.Z = MAX((vector3(PlayerPos+BBoxSize)).Z, (vector3(AssetPos+BBoxSize)).Z) + 0.1f; 
                draw_BBox(OverBox, XCOLOR_GREEN);

                draw_Line( PlayerPos, AssetPos, XCOLOR_GREEN);
            }
            else
                draw_Line( PlayerPos, AssetPos);
        }

        if (j==0)
            Color.Set(0,0,255);
        else
            Color.Set(255,0,0);

        // Render the sniping data
        //
        for (i = 0; i < TeamAssets->m_nSnipePoints; i++)
        {
            PlayerPos = TeamAssets->m_pSnipePoint[i] + vector3(0.0f, 1.5f, 0.0f);
            draw_BBox( bbox( PlayerPos-BBoxSize, PlayerPos+BBoxSize), Color);

            if ((View.GetPosition() - PlayerPos).LengthSquared() < 100)
                draw_Label(PlayerPos, XCOLOR_WHITE, "SNIPE POS - %c", 
                TeamAssets->m_Team ? 'I' : 'S');

            if (&TeamAssets->m_pSnipePoint[i] == g_SelectedAsset)
            {
                bbox OverBox;
                OverBox.Min.X = vector3(PlayerPos-BBoxSize).X - 0.1f; 
                OverBox.Min.Y = vector3(PlayerPos-BBoxSize).Y - 0.1f; 
                OverBox.Min.Z = vector3(PlayerPos-BBoxSize).Z - 0.1f; 
                OverBox.Max.X = vector3(PlayerPos+BBoxSize).X + 0.1f; 
                OverBox.Max.Y = vector3(PlayerPos+BBoxSize).Y + 0.1f; 
                OverBox.Max.Z = vector3(PlayerPos+BBoxSize).Z + 0.1f; 
                draw_BBox(OverBox, XCOLOR_GREEN);
            }
        }

        TeamAssets = &m_InfernoAssets;
    }

        if (BadInvens)
            x_printfxy(30, 30, "Bad Invens: %d", BadInvens);
        if (BadSensors)
            x_printfxy(30, 31, "Bad Sensors: %d", BadSensors);
        if (BadTurrets)
            x_printfxy(30, 32, "Bad Turrets: %d", BadTurrets);

    xbool InTurretSenseArea = FALSE;
    xbool InSensorSenseArea = FALSE;
    xbool InFireRange = FALSE;

    turret*    Turret;
    sensor*    Sensor;
    f32 DistFromTurret;
    f32 SenseRadius;
    f32 FireRange;
    xbool SelectedOnly = FALSE;
    vector3 SelectedPos;
    if ( m_Limits & 0x01 ) 
    {
        if (g_SelectedAsset && g_SAType == 1)
        {
            // highlight only the selected asset.
            SelectedOnly = TRUE;
            SelectedPos = ((turret_spot*)g_SelectedAsset)->GetAssetPos();
        }
        // Display limits of turret sense & fire
        ObjMgr.StartTypeLoop(object::TYPE_TURRET_CLAMP);
        while ((Turret = (turret*)ObjMgr.GetNextInType()) != NULL)
        {
            AssetPos = Turret->GetPosition();
            if (SelectedOnly && AssetPos != SelectedPos)
                continue;
            
            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if (DistFromTurret < DEPLOYED_SENSE_RADIUS*DEPLOYED_SENSE_RADIUS)
            {
                InTurretSenseArea = TRUE;
            }
            if (DistFromTurret < DEPLOYED_FIRE_RADIUS*DEPLOYED_FIRE_RADIUS)
            {
                InFireRange = TRUE;
            }
            // Draw something.
            draw_Sphere(AssetPos, DEPLOYED_SENSE_RADIUS, XCOLOR_RED);
            draw_Sphere(AssetPos, DEPLOYED_FIRE_RADIUS, XCOLOR_GREEN);
        }
        ObjMgr.EndTypeLoop();
    }

    SelectedOnly = FALSE;
    if ( m_SensorLimits & 0x01 )
    {
        if (g_SelectedAsset && g_SAType == 2)
        {
            // highlight only the selected asset.
            SelectedOnly = TRUE;
            SelectedPos = ((sensor_spot*)g_SelectedAsset)->GetAssetPos();
        }
        // Display limits of deployed sensors
        ObjMgr.StartTypeLoop(object::TYPE_SENSOR_REMOTE);
        while ((Sensor = (sensor*)ObjMgr.GetNextInType() ) != NULL)
        {
            AssetPos = Sensor->GetPosition();
            if (SelectedOnly && AssetPos != SelectedPos)
                continue;
            SenseRadius = Sensor->GetSenseRadius();
            
            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if (DistFromTurret < SenseRadius*SenseRadius)
            {
                InSensorSenseArea = TRUE;
            }
            // Draw something.
            draw_Sphere(AssetPos, SenseRadius, XCOLOR_YELLOW);
        }
        ObjMgr.EndTypeLoop();
    }

    if (m_Limits & 0x02)
    {
        ObjMgr.StartTypeLoop(object::TYPE_TURRET_FIXED);
        while ((Turret = (turret*)ObjMgr.GetNextInType()) != NULL)
        {
            AssetPos = Turret->GetPosition();
            switch (Turret->GetKind())
            {
                case (turret::MISSILE):
                    SenseRadius = MISSILE_SENSE_RADIUS;
                    FireRange   = MISSILE_FIRE_RADIUS;
                break;
                case (turret::AA):
                    SenseRadius = AA_SENSE_RADIUS;
                    FireRange   = AA_FIRE_RADIUS;
                break;
                case (turret::MORTAR):
                    SenseRadius = MORTAR_SENSE_RADIUS;
                    FireRange   = MORTAR_FIRE_RADIUS;
                break;
                case (turret::PLASMA):
                    SenseRadius = PLASMA_SENSE_RADIUS;
                    FireRange   = PLASMA_FIRE_RADIUS;
                break;
                case (turret::ELF):
                    SenseRadius = ELF_SENSE_RADIUS;
                    FireRange   = ELF_FIRE_RADIUS;
                break;
                default:
                    x_printf("UNKNOWN TURRET TYPE!\nUsing Average Fire Range\n");
                    SenseRadius = AVERAGE_SENSE_RADIUS;
                    FireRange   = AVERAGE_FIRE_RADIUS;
                break;
            }

            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if (DistFromTurret < SenseRadius*SenseRadius)
            {
                InTurretSenseArea = TRUE;
            }
            if (DistFromTurret < FireRange*FireRange)
            {
                InFireRange = TRUE;
            }
            // Draw something.
            draw_Sphere(AssetPos, SenseRadius, XCOLOR_RED);
            draw_Sphere(AssetPos, FireRange, XCOLOR_GREEN);
        }
        ObjMgr.EndTypeLoop();

    
        // sentry sense radius == sentry fire radius, no need for additional sensors.
        ObjMgr.StartTypeLoop(object::TYPE_TURRET_SENTRY);
        while ((Turret = (turret*)ObjMgr.GetNextInType()) != NULL)
        {
            AssetPos = Turret->GetPosition();
            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if ( DistFromTurret < SENTRY_FIRE_RADIUS*SENTRY_FIRE_RADIUS*1.5)
            {
                if (DistFromTurret < SENTRY_SENSE_RADIUS*SENTRY_SENSE_RADIUS)
                {
                    InTurretSenseArea = TRUE;
                }
                if (DistFromTurret < SENTRY_FIRE_RADIUS*SENTRY_FIRE_RADIUS)
                {
                    InFireRange = TRUE;
                }
                // Draw something.
                draw_Sphere(AssetPos, SENTRY_SENSE_RADIUS, XCOLOR_RED);
                draw_Sphere(AssetPos, 175, XCOLOR_GREEN);
            }
        }
        ObjMgr.EndTypeLoop();
    }
    if (m_SensorLimits & 0x02)
    {
        // Display limits of existing sensors
        ObjMgr.StartTypeLoop(object::TYPE_SENSOR_MEDIUM);
        while ((Sensor = (sensor*)ObjMgr.GetNextInType()) != NULL)
        {
            AssetPos = Sensor->GetPosition();
            SenseRadius = Sensor->GetSenseRadius();
            
            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if (DistFromTurret < SenseRadius*SenseRadius)
            {
                InSensorSenseArea = TRUE;
            }
            // Draw something.
            draw_Sphere(AssetPos, SenseRadius, XCOLOR_YELLOW);
        }
        ObjMgr.EndTypeLoop();
        // Display limits of existing sensors
        ObjMgr.StartTypeLoop(object::TYPE_SENSOR_LARGE);
        while ((Sensor = (sensor*)ObjMgr.GetNextInType()) != NULL)
        {
            AssetPos = Sensor->GetPosition();
            SenseRadius = Sensor->GetSenseRadius();
            
            DistFromTurret = (View.GetPosition() - AssetPos).LengthSquared();
            if (DistFromTurret < SenseRadius*SenseRadius)
            {
                InSensorSenseArea = TRUE;
            }
            // Draw something.
            draw_Sphere(AssetPos, SenseRadius, XCOLOR_YELLOW);
        }
        ObjMgr.EndTypeLoop();
    }

    xcolor Tint;
    s32 X0,Y0,X1,Y1 ;
    View.GetViewport( X0, Y0, X1, Y1 ) ;
    if (InTurretSenseArea)
    {
        Tint.Set(255,0,0,96);
        // Draw blue alpha rect
        draw_Begin(DRAW_RECTS, DRAW_2D | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER) ;
        draw_Color(xcolor(Tint.R, Tint.G, Tint.B, Tint.A )) ;
        draw_Vertex((f32)X0, (f32)Y0, 0) ;
        draw_Vertex((f32)X1, (f32)Y1, 0) ;
        draw_End() ;
    }
    else if (InSensorSenseArea)
    {
        Tint.Set(255,225,0,96);
        // Draw red alpha rect
        draw_Begin(DRAW_RECTS, DRAW_2D | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER) ;
        draw_Color(xcolor(Tint.R, Tint.G, Tint.B, Tint.A)) ;
        draw_Vertex((f32)X0, (f32)Y0, 0) ;
        draw_Vertex((f32)X1, (f32)Y1, 0) ;
        draw_End() ;
    }
    else if (InFireRange)
    {
        Tint.Set(0,225,0,96);
        // Draw red alpha rect
        draw_Begin(DRAW_RECTS, DRAW_2D | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER) ;
        draw_Color(xcolor(Tint.R, Tint.G, Tint.B, Tint.A)) ;
        draw_Vertex((f32)X0, (f32)Y0, 0) ;
        draw_Vertex((f32)X1, (f32)Y1, 0) ;
        draw_End() ;
    }

    eng_End();
}

//==============================================================================
xbool asset_editor::ValidateInvenSpot ( inven_spot *Spot )
{
    ASSERT(g_pPlayer);

    g_pPlayer->SetPos(Spot->GetStandPos());
	// Test the ground under the player.
	{
		collider Ray;
		vector3 Point = Spot->GetStandPos();
		vector3 Normal(0,1,0);

		vector3 Offset = Normal;
		Offset.Scale(DROP_HEIGHT);;

		vector3 A = Normal;
		A.Scale(0.05f);
		A = Point + A;  // raise it up 5 cm.

		// Drop it down about a foot.
		vector3 B = Point - Offset;

		Ray.RaySetup( NULL, A, B);
		ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
		if( !Ray.HasCollided() )
		{
	        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
			return (FALSE);
		}
	}
    if (GameMgr.AttemptDeployStation(g_pPlayer->GetObjectID(), 
        Spot->GetAssetPos(),
        vector3(0.0f,1.0f,0.0f), 
        object::TYPE_TERRAIN,
        TRUE) == FALSE)
    {
        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
        return FALSE;
    }
    else
    {
        Spot->ClearFlag(asset_spot::FLAG_BAD_SPOT);
        return TRUE;
    }
}

//==============================================================================

xbool asset_editor::ValidateTurretSpot (turret_spot *Spot )
{
    ASSERT(g_pPlayer);
    g_pPlayer->SetPos(Spot->GetStandPos());
	// Test the ground under the player.
	{
		collider Ray;
		vector3 Point = Spot->GetStandPos();
		vector3 Normal(0,1,0);

		vector3 Offset = Normal;
		Offset.Scale(DROP_HEIGHT);;

		vector3 A = Normal;
		A.Scale(0.05f);
		A = Point + A;  // raise it up 5 cm.

		// Drop it down about a foot.
		vector3 B = Point - Offset;

		Ray.RaySetup( NULL, A, B);
		ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
		if( !Ray.HasCollided() )
		{
	        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
			return (FALSE);
		}
	}
    if (GameMgr.AttemptDeployTurret(g_pPlayer->GetObjectID(), 
        Spot->GetAssetPos(),
        Spot->GetNormal(), 
        object::TYPE_TERRAIN,
        TRUE) == FALSE)
    {
        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
        return FALSE;
    }
    else
    {
        Spot->ClearFlag(asset_spot::FLAG_BAD_SPOT);
        return TRUE;
    }
}

//==============================================================================

xbool asset_editor::ValidateSensorSpot (sensor_spot *Spot )
{
    ASSERT(g_pPlayer);
    g_pPlayer->SetPos(Spot->GetStandPos());
	// Test the ground under the player.
	{
		collider Ray;
		vector3 Point = Spot->GetStandPos();
		vector3 Normal(0,1,0);

		vector3 Offset = Normal;
		Offset.Scale(DROP_HEIGHT);;

		vector3 A = Normal;
		A.Scale(0.05f);
		A = Point + A;  // raise it up 5 cm.

		// Drop it down about a foot.
		vector3 B = Point - Offset;

		Ray.RaySetup( NULL, A, B);
		ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
		if( !Ray.HasCollided() )
		{
	        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
			return (FALSE);
		}
	}
    if (GameMgr.AttemptDeploySensor(g_pPlayer->GetObjectID(), 
        Spot->GetAssetPos(),
        Spot->GetNormal(), 
        object::TYPE_TERRAIN,
        TRUE) == FALSE)
    {
        Spot->AddFlag(asset_spot::FLAG_BAD_SPOT);
        return FALSE;
    }
    else
    {
        Spot->ClearFlag(asset_spot::FLAG_BAD_SPOT);
        return TRUE;
    }
}

//==============================================================================

void asset_editor::Save( const char* pFileName )
{
    ASSERT(g_pPlayer);
    // Sort assets by priority value, descending.
    SortAssetsLists();

    X_FILE* Fp;

    Fp = x_fopen( pFileName, "wb" );
    if( !Fp )
    {
        MessageBox(NULL, "Save Failed! (read-only file?)", "Error", MB_OK);
        return;
    }

    s32 i;

    vector3 StandPos;
    vector3 AssetPos;
    vector3 Normal;
    asset_spot::priority_value Priority;
    radian Yaw;
    u32 Flags;
    radian3 Rot;

    vector3 Point;

    team_assets *TeamAssets;
    xbool NormalOrder = TRUE;

    // Clear away all deployables
    ClearDeployables();

    if (NormalOrder)
        TeamAssets = &m_StormAssets;
    else
        TeamAssets = &m_InfernoAssets;
    s32 count = 0;
    for (count = 0; count < 2; count++)
    {
        x_fwrite(&TeamAssets->m_Team,    sizeof(asset_spot::team),     1, Fp);
        x_fwrite(&TeamAssets->m_nInvens, sizeof(s32),                  1, Fp);
        x_fwrite(&TeamAssets->m_nTurrets,sizeof(s32),                  1, Fp);
        x_fwrite(&TeamAssets->m_nSensors,sizeof(s32),                  1, Fp);
        x_fwrite(&TeamAssets->m_nMines,  sizeof(s32),                  1, Fp);
        for (i = 0; i < TeamAssets->m_nInvens; i++)
        {
            ValidateInvenSpot(&TeamAssets->m_pInvenList[i]);

            StandPos = TeamAssets->m_pInvenList[i].GetStandPos();
            AssetPos = TeamAssets->m_pInvenList[i].GetAssetPos();
            Yaw      = TeamAssets->m_pInvenList[i].GetYaw();
            Priority = TeamAssets->m_pInvenList[i].GetPriority();
            Flags    = TeamAssets->m_pInvenList[i].GetFlags();

            x_fwrite( &StandPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &AssetPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &Yaw,        sizeof(radian),                       1, Fp);
            x_fwrite( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
            x_fwrite( &Flags,      sizeof(u32),                          1, Fp);
        }
        for (i = 0; i < TeamAssets->m_nTurrets; i++)
        {
            ValidateTurretSpot (&TeamAssets->m_pTurretList[i]);
            StandPos = TeamAssets->m_pTurretList[i].GetStandPos();
            AssetPos = TeamAssets->m_pTurretList[i].GetAssetPos();
            Normal   = TeamAssets->m_pTurretList[i].GetNormal();
            Priority = TeamAssets->m_pTurretList[i].GetPriority();
            Flags    = TeamAssets->m_pTurretList[i].GetFlags();

            x_fwrite( &StandPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &AssetPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &Normal,     sizeof(vector3),                      1, Fp);
            x_fwrite( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
            x_fwrite( &Flags,      sizeof(u32),                          1, Fp);
        }
        for (i = 0; i < TeamAssets->m_nSensors; i++)
        {
            ValidateSensorSpot (&TeamAssets->m_pSensorList[i]);
            StandPos = TeamAssets->m_pSensorList[i].GetStandPos();
            AssetPos = TeamAssets->m_pSensorList[i].GetAssetPos();
            Normal   = TeamAssets->m_pSensorList[i].GetNormal();
            Priority = TeamAssets->m_pSensorList[i].GetPriority();
            Flags    = TeamAssets->m_pSensorList[i].GetFlags();

            x_fwrite( &StandPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &AssetPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &Normal,     sizeof(vector3),                      1, Fp);
            x_fwrite( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
            x_fwrite( &Flags,      sizeof(u32),                          1, Fp);
        }
        for (i = 0; i < TeamAssets->m_nMines; i++)
        {
            StandPos = TeamAssets->m_pMineList[i].GetStandPos();
            Rot      = TeamAssets->m_pMineList[i].GetPlayerRot();
            Priority = TeamAssets->m_pMineList[i].GetPriority();
            Flags    = TeamAssets->m_pMineList[i].GetFlags();

            x_fwrite( &StandPos,   sizeof(vector3),                      1, Fp);
            x_fwrite( &Rot,        sizeof(radian3),                      1, Fp);
            x_fwrite( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
            x_fwrite( &Flags,      sizeof(u32),                          1, Fp);
        }
        x_fwrite(&TeamAssets->m_nSnipePoints, sizeof(s32),             1, Fp);
        for (i = 0; i < TeamAssets->m_nSnipePoints; i++)
        {
            Point = TeamAssets->m_pSnipePoint[i];
            x_fwrite(&Point,        sizeof(vector3),                     1, Fp);
        }
    
        if (NormalOrder)    
            TeamAssets = &m_InfernoAssets;
        else
            TeamAssets = &m_StormAssets;
    }    
    x_fclose( Fp );

    g_AssetsChanged = FALSE;
}

//==============================================================================

void asset_editor::Load( const char* pFileName)
{
    ObjMgr.StartTypeLoop(object::TYPE_PLAYER);
    g_pPlayer = (player_object*)ObjMgr.GetNextInType();
    ObjMgr.EndTypeLoop();
    ASSERT(g_pPlayer);

    X_FILE* Fp;

    Fp = x_fopen( pFileName, "rb" );
    if (!Fp)
        return;
#ifdef T2_DESIGNER_BUILD
//#if DESIGNER_BUILD
    if( Fp == NULL ) 
    {
        MessageBox(NULL, "Load Failed!", "Error", MB_OK);
        return;
    }
#endif
    ClearDeployables();

    vector3 StandPos;
    vector3 AssetPos;
    vector3 Normal;
    vector3 Point;
    asset_spot::priority_value Priority;
    radian Yaw;
    u32 Flags;
    radian3 Rot;

    s32 i;
    s32 nBadStormInvens = 0;
    s32 nBadStormTurrets = 0;
    s32 nBadStormSensors = 0;

    s32 nBadInfernoInvens = 0;
    s32 nBadInfernoTurrets = 0;
    s32 nBadInfernoSensors = 0;
    // Storm Assets
    x_fread(&m_StormAssets.m_Team,    sizeof(asset_spot::team),     1, Fp);
    x_fread(&m_StormAssets.m_nInvens, sizeof(s32),                  1, Fp);
    x_fread(&m_StormAssets.m_nTurrets,sizeof(s32),                  1, Fp);
    x_fread(&m_StormAssets.m_nSensors,sizeof(s32),                  1, Fp);
    x_fread(&m_StormAssets.m_nMines,  sizeof(s32),                  1, Fp);
    for (i = 0; i < m_StormAssets.m_nInvens; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Yaw,        sizeof(radian),                       1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_StormAssets.m_pInvenList[i].Set(StandPos,AssetPos,Yaw,Priority,Flags);

        if ( !ValidateInvenSpot ( &m_StormAssets.m_pInvenList[i] ))
            nBadStormInvens++;
    }
    for (i = 0; i < m_StormAssets.m_nTurrets; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Normal,     sizeof(vector3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_StormAssets.m_pTurretList[i].Set(StandPos, AssetPos, Normal, Priority, Flags);
        if ( !ValidateTurretSpot ( &m_StormAssets.m_pTurretList[i] ))
            nBadStormTurrets++;
    }
    for (i = 0; i < m_StormAssets.m_nSensors; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Normal,     sizeof(vector3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_StormAssets.m_pSensorList[i].Set(StandPos, AssetPos, Normal, Priority, Flags);
        if ( !ValidateSensorSpot ( &m_StormAssets.m_pSensorList[i] ))
            nBadStormSensors++;
    }
    for (i = 0; i < m_StormAssets.m_nMines; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Rot,        sizeof(radian3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_StormAssets.m_pMineList[i].Set(StandPos, Rot, Priority, Flags);
    }
    x_fread(&m_StormAssets.m_nSnipePoints, sizeof(s32),             1, Fp);
    for (i = 0; i < m_StormAssets.m_nSnipePoints; i++)
    {
        x_fread(&Point,        sizeof(vector3),                     1, Fp);
        m_StormAssets.m_pSnipePoint[i] = Point;
    }

    
    // Inferno Assets
    x_fread(&m_InfernoAssets.m_Team,    sizeof(asset_spot::team),     1, Fp);
    x_fread(&m_InfernoAssets.m_nInvens, sizeof(s32),                  1, Fp);
    x_fread(&m_InfernoAssets.m_nTurrets,sizeof(s32),                  1, Fp);
    x_fread(&m_InfernoAssets.m_nSensors,sizeof(s32),                  1, Fp);
    x_fread(&m_InfernoAssets.m_nMines,  sizeof(s32),                  1, Fp);
    for (i = 0; i < m_InfernoAssets.m_nInvens; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Yaw,        sizeof(radian),                       1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_InfernoAssets.m_pInvenList[i].Set(StandPos,AssetPos,Yaw,Priority,Flags);
        if ( !ValidateInvenSpot ( &m_InfernoAssets.m_pInvenList[i] ))
            nBadInfernoInvens++;
    }
    for (i = 0; i < m_InfernoAssets.m_nTurrets; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Normal,     sizeof(vector3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_InfernoAssets.m_pTurretList[i].Set(StandPos, AssetPos, Normal, Priority, Flags);
        if ( !ValidateTurretSpot ( &m_InfernoAssets.m_pTurretList[i] ))
            nBadInfernoTurrets++;
    }
    for (i = 0; i < m_InfernoAssets.m_nSensors; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &AssetPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Normal,     sizeof(vector3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_InfernoAssets.m_pSensorList[i].Set(StandPos, AssetPos, Normal, Priority, Flags);
        if ( !ValidateSensorSpot ( &m_InfernoAssets.m_pSensorList[i] ))
            nBadInfernoSensors++;
    }
    for (i = 0; i < m_InfernoAssets.m_nMines; i++)
    {
        x_fread( &StandPos,   sizeof(vector3),                      1, Fp);
        x_fread( &Rot,        sizeof(radian3),                      1, Fp);
        x_fread( &Priority,   sizeof(asset_spot::priority_value),   1, Fp);
        x_fread( &Flags,      sizeof(u32),                          1, Fp);

        m_InfernoAssets.m_pMineList[i].Set(StandPos, Rot, Priority, Flags);
    }
    x_fread(&m_InfernoAssets.m_nSnipePoints, sizeof(s32),             1, Fp);
    for (i = 0; i < m_InfernoAssets.m_nSnipePoints; i++)
    {
        x_fread(&Point,        sizeof(vector3),                     1, Fp);
        m_InfernoAssets.m_pSnipePoint[i] = Point;
    }

    x_fclose( Fp );

    g_AssetsChanged = FALSE;

    x_DebugMsg("%s Bad Assets:   Storm: %d Invens, %d Turrets, %d Sensors.\n",
                tgl.MissionName, nBadStormInvens, nBadStormTurrets, nBadStormSensors);
    x_DebugMsg("%s Bad Assets: Inferno: %d Invens, %d Turrets, %d Sensors.\n",
                tgl.MissionName, nBadInfernoInvens, nBadInfernoTurrets, nBadInfernoSensors);

    bot_log::Log("%s Bad Assets:   Storm: %d Invens, %d Turrets, %d Sensors.\n",
                tgl.MissionName, nBadStormInvens, nBadStormTurrets, nBadStormSensors);
    bot_log::Log("%s Bad Assets: Inferno: %d Invens, %d Turrets, %d Sensors.\n",
                tgl.MissionName, nBadInfernoInvens, nBadInfernoTurrets, nBadInfernoSensors);
}

//==============================================================================

void ClearDeployables ( void )
{
    // Remove all existing deployables from the world.
    station*   Inven;
    turret*    Turret;
    sensor*    Sensor;
    mine*      Mine;

    ObjMgr.StartTypeLoop(object::TYPE_STATION_DEPLOYED);
    while ((Inven = (station*)ObjMgr.GetNextInType()) != NULL)
    {
        ObjMgr.RemoveObject(Inven);
        ObjMgr.DestroyObject(Inven);
    }
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop(object::TYPE_TURRET_CLAMP);
    while ((Turret = (turret*)ObjMgr.GetNextInType()) != NULL)
    {
        ObjMgr.RemoveObject(Turret);
        ObjMgr.DestroyObject(Turret);
    }
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop(object::TYPE_SENSOR_REMOTE);
    while ((Sensor = (sensor*)ObjMgr.GetNextInType()) != NULL)
    {
        ObjMgr.RemoveObject(Sensor);
        ObjMgr.DestroyObject(Sensor);
    }
    ObjMgr.EndTypeLoop();

    ObjMgr.StartTypeLoop(object::TYPE_MINE);
    while ((Mine = (mine*)ObjMgr.GetNextInType()) != NULL)
    {
        ObjMgr.RemoveObject(Mine);
        ObjMgr.DestroyObject(Mine);
    }
    ObjMgr.EndTypeLoop();
}

//==============================================================================

void asset_editor::ReplaceDeployables ( void )
{
    if (!DISPLAY_ACTUAL_ASSETS)
        return;
    team_assets *Team = &m_StormAssets;
    
    s32 i,j;
    for (j = 0; j < 2; j++)
    {
        // Place the inventory stations in the world.
        for ( i=0; i < Team->m_nInvens; i++)
        {
            if (j == 1 && 
                (Team->m_pInvenList[i].GetFlags() & asset_spot::TEAM_NEUTRAL) )
                continue;
            station* pInven = (station*)ObjMgr.CreateObject( object::TYPE_STATION_DEPLOYED );
            pInven->Initialize( Team->m_pInvenList[i].GetAssetPos(),
                                Team->m_pInvenList[i].GetYaw(),
                                g_pPlayer->GetObjectID(),
                                g_pPlayer->GetTeamBits());
            ObjMgr.AddObject( pInven, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        // Place the turrets in the world.
        for ( i=0; i < Team->m_nTurrets; i++)
        {
            if (j == 1 && 
                (Team->m_pTurretList[i].GetFlags() & asset_spot::TEAM_NEUTRAL) )
                continue;
            turret* pTurret = (turret*)ObjMgr.CreateObject(object::TYPE_TURRET_CLAMP);
            pTurret->Initialize( Team->m_pTurretList[i].GetAssetPos(),
                                 Team->m_pTurretList[i].GetNormal(),
                                 g_pPlayer->GetObjectID(),
                                 g_pPlayer->GetTeamBits() );
            ObjMgr.AddObject( pTurret, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        // Place the sensors in the world.
        for ( i=0; i < Team->m_nSensors; i++)
        {
            if (j == 1 && 
                (Team->m_pSensorList[i].GetFlags() & asset_spot::TEAM_NEUTRAL) )
                continue;
            sensor* pSensor = (sensor*)ObjMgr.CreateObject(object::TYPE_SENSOR_REMOTE);
            pSensor->Initialize( Team->m_pSensorList[i].GetAssetPos(),
                                 Team->m_pSensorList[i].GetNormal(),
                                 g_pPlayer->GetTeamBits() );
            ObjMgr.AddObject( pSensor, obj_mgr::AVAILABLE_SERVER_SLOT );
        }

        Team = &m_InfernoAssets;
    }
}

//==============================================================================

void asset_editor::SortAssetsLists( void )
{
    team_assets *Team = &m_StormAssets;
    
    s32 i,j, k;
    xbool bChanged;
    for (i = 0; i < 2; i++)
    {
        k = 0;
        do
        {
            bChanged = FALSE;
            for (j = 0; j < Team->m_nInvens; j++)
            {
                if (j+1 >= Team->m_nInvens)
                    break;
                if (Team->m_pInvenList[j].GetPriority() < 
                                        Team->m_pInvenList[j+1].GetPriority())
                {
                    // Swap them.
                    inven_spot Temp;
                    Temp.Set(Team->m_pInvenList[j]);
                    Team->m_pInvenList[j].Set(Team->m_pInvenList[j+1]);
                    Team->m_pInvenList[j+1].Set(Temp);
                    bChanged = TRUE;
                }
            }
            if (bChanged)
            {
                k++;
                if (k > Team->m_nInvens)
                    ASSERT(0 && "Sort failed after too many attempts.");
            }
        } while (bChanged);

        k = 0;
        do
        {
            bChanged = FALSE;
            for (j = 0; j < Team->m_nTurrets; j++)
            {
                if (j+1 >= Team->m_nTurrets)
                    break;
                if (Team->m_pTurretList[j].GetPriority() < 
                                        Team->m_pTurretList[j+1].GetPriority())
                {
                    // Swap them.
                    turret_spot Temp;
                    Temp.Set(Team->m_pTurretList[j]);
                    Team->m_pTurretList[j].Set(Team->m_pTurretList[j+1]);
                    Team->m_pTurretList[j+1].Set(Temp);
                    bChanged = TRUE;
                }
            }
            if (bChanged)
            {
                k++;
                if (k > Team->m_nTurrets)
                    ASSERT(0 && "Sort failed after too many attempts.");
            }
        } while (bChanged);

        k = 0;
        do
        {
            bChanged = FALSE;
            for (j = 0; j < Team->m_nSensors; j++)
            {
                if (j+1 >= Team->m_nSensors)
                    break;
                if (Team->m_pSensorList[j].GetPriority() < 
                                        Team->m_pSensorList[j+1].GetPriority())
                {
                    // Swap them.
                    sensor_spot Temp;
                    Temp.Set(Team->m_pSensorList[j]);
                    Team->m_pSensorList[j].Set(Team->m_pSensorList[j+1]);
                    Team->m_pSensorList[j+1].Set(Temp);
                    bChanged = TRUE;
                }
            }
            if (bChanged)
            {
                k++;
                if (k > Team->m_nSensors)
                    ASSERT(0 && "Sort failed after too many attempts.");
            }
        } while (bChanged);

        k = 0;
        do
        {
            bChanged = FALSE;
            for (j = 0; j < Team->m_nMines; j++)
            {
                if (j+1 >= Team->m_nMines)
                    break;
                if (Team->m_pMineList[j].GetPriority() < 
                                        Team->m_pMineList[j+1].GetPriority())
                {
                    // Swap them.
                    mine_spot Temp;
                    Temp.Set(Team->m_pMineList[j]);
                    Team->m_pMineList[j].Set(Team->m_pMineList[j+1]);
                    Team->m_pMineList[j+1].Set(Temp);
                    bChanged = TRUE;
                }
            }
            if (bChanged)
            {
                k++;
                if (k > Team->m_nMines)
                    ASSERT(0 && "Sort failed after too many attempts.");
            }
        } while (bChanged);

        Team = &m_InfernoAssets;
    }
}

//==============================================================================

void asset_editor::AddSnipePoint( void )
{
    team_assets* Team;
    if (g_StormSelected)
        Team = &m_StormAssets;
    else
        Team = &m_InfernoAssets;

    s32 nPoints = Team->m_nSnipePoints;
    if (nPoints >= MAX_SNIPE_POINTS)
    {
        MessageBox(NULL, "Maximum Snipe Points Reached", "Record Error", MB_OK);
        return ;
    }

    s32 i;
    for (i = 0; i < nPoints; i++)
    {
        if ((g_pPlayer->GetPosition() - Team->m_pSnipePoint[i]).Length() < 5.0)
        {
            MessageBox(NULL, "Already have Snipe Point near there!", "Record Error", MB_OK);
            return;
        }
    }

    // Add pos to team list.
    Team->m_pSnipePoint[nPoints] = g_pPlayer->GetPosition();
    Team->m_nSnipePoints++;
    
    x_printf("%s Sniper Point recorded (%d of %d)", 
        g_TeamName[(s32)Team->m_Team], Team->m_nSnipePoints, MAX_SNIPE_POINTS);
    g_SelectedAsset = NULL;
    g_AssetsChanged = TRUE;

    if (g_bNeutral)
    {
        g_StormSelected ^= 1;
        if (g_StormSelected)
            Team = &m_StormAssets;
        else
            Team = &m_InfernoAssets;

        nPoints = Team->m_nSnipePoints;
        if (nPoints >= MAX_SNIPE_POINTS)
        {
            MessageBox(NULL, "Maximum Snipe Points Reached", "Record Error", MB_OK);
            return ;
        }

        for (i = 0; i < nPoints; i++)
        {
            if ((g_pPlayer->GetPosition() - Team->m_pSnipePoint[i]).Length() < 5.0)
            {
                MessageBox(NULL, "Already have Snipe Point near there!", "Record Error", MB_OK);
                return;
            }
        }

        // Add pos to team list.
        Team->m_pSnipePoint[nPoints] = g_pPlayer->GetPosition();
        Team->m_nSnipePoints++;
    
        x_printf("%s Sniper Point recorded (%d of %d)", 
            g_TeamName[(s32)Team->m_Team], Team->m_nSnipePoints, MAX_SNIPE_POINTS);
    }
}

//==============================================================================

void asset_editor::Report( X_FILE* FP )
{
    if (FP)
    {
        x_fprintf(FP,
            "  Storm Assets:  %2d Turrets, %2d Invens, %2d Snipe Points, %2d Sensors\n",
                                 m_StormAssets.m_nTurrets,
                                 m_StormAssets.m_nInvens,
                                 m_StormAssets.m_nSnipePoints,
                                 m_StormAssets.m_nSensors);

        x_fprintf(FP,
            "Inferno Assets:  %2d Turrets, %2d Invens, %2d Snipe Points, %2d Sensors\n",
                                 m_InfernoAssets.m_nTurrets,
                                 m_InfernoAssets.m_nInvens,
                                 m_InfernoAssets.m_nSnipePoints,
                                 m_InfernoAssets.m_nSensors);
    }
    else
    {
        x_printf("  Storm Assets:  %2d Turrets, %2d Invens, %2d Snipe Points, %2d Sensors\n",
                                 m_StormAssets.m_nTurrets,
                                 m_StormAssets.m_nInvens,
                                 m_StormAssets.m_nSnipePoints,
                                 m_StormAssets.m_nSensors);

        x_printf("Inferno Assets:  %2d Turrets, %2d Invens, %2d Snipe Points, %2d Sensors\n",
                                 m_InfernoAssets.m_nTurrets,
                                 m_InfernoAssets.m_nInvens,
                                 m_InfernoAssets.m_nSnipePoints,
                                 m_InfernoAssets.m_nSensors);
    }
}

//==============================================================================

xbool asset_editor::ReportAll( const char* pFileName )
{
    X_FILE* Fp;

    Fp = x_fopen( pFileName, "wt" );
    if( !Fp )
    {
        return FALSE;
    }

    Load("Data/Missions/Avalon/Avalon.ass");
    x_fprintf(Fp, "\nAvalon\n");
    Report(Fp);
    Load("Data/Missions/Abominable/Abominable.ass");
    x_fprintf(Fp, "\nAbominable\n");
    Report(Fp);
    Load("Data/Missions/BeggarsRun/BeggarsRun.ass");
    x_fprintf(Fp, "\nBeggar's Run\n");
    Report(Fp);
    Load("Data/Missions/Damnation/Damnation.ass");
    x_fprintf(Fp, "\nDamnation\n");
    Report(Fp);
    Load("Data/Missions/DeathBirdsFly/DeathBirdsFly.ass");
    x_fprintf(Fp, "\nDeathBirdsFly\n");
    Report(Fp);
    Load("Data/Missions/Desiccator/Desiccator.ass");
    x_fprintf(Fp, "\nDesiccator\n");
    Report(Fp);
    Load("Data/Missions/Equinox/Equinox.ass");
    x_fprintf(Fp, "\nEquinox\n");
    Report(Fp);
    Load("Data/Missions/Firestorm/Firestorm.ass");
    x_fprintf(Fp, "\nFirestorm\n");
    Report(Fp);
    Load("Data/Missions/Flashpoint/Flashpoint.ass");
    x_fprintf(Fp, "\nFlashpoint\n");
    Report(Fp);
    Load("Data/Missions/Insalubria/Insalubria.ass");
    x_fprintf(Fp, "\nInsalubria\n");
    Report(Fp);
    Load("Data/Missions/JacobsLadder/JacobsLadder.ass");
    x_fprintf(Fp, "\nJacob's Ladder\n");
    Report(Fp);
    Load("Data/Missions/Katabatic/Katabatic.ass");
    x_fprintf(Fp, "\nKatabatic\n");
    Report(Fp);
    Load("Data/Missions/Overreach/Overreach.ass");
    x_fprintf(Fp, "\nOverreach\n");
    Report(Fp);
    Load("Data/Missions/Paranoia/Paranoia.ass");
    x_fprintf(Fp, "\nParanoia\n");
    Report(Fp);
    Load("Data/Missions/Quagmire/Quagmire.ass");
    x_fprintf(Fp, "\nQuagmire\n");
    Report(Fp);
    Load("Data/Missions/Recalescence/Recalescence.ass");
    x_fprintf(Fp, "\nRecalescence\n");
    Report(Fp);
    Load("Data/Missions/Reversion/Reversion.ass");
    x_fprintf(Fp, "\nReversion\n");
    Report(Fp);
    Load("Data/Missions/Sanctuary/Sanctuary.ass");
    x_fprintf(Fp, "\nSanctuary\n");
    Report(Fp);
    Load("Data/Missions/Sirocco/Sirocco.ass");
    x_fprintf(Fp, "\nSirocco\n");
    Report(Fp);
    Load("Data/Missions/SlapDash/SlapDash.ass");
    x_fprintf(Fp, "\nSlapDash\n");
    Report(Fp);
    Load("Data/Missions/ThinIce/ThinIce.ass");
    x_fprintf(Fp, "\nThinIce\n");
    Report(Fp);
    Load("Data/Missions/Tombstone/Tombstone.ass");
    x_fprintf(Fp, "\nTombstone\n");
    Report(Fp);
    
    x_fclose( Fp );

    return TRUE;
}

//==============================================================================

const char* asset_editor::ToggleLimits ( void )
{
    m_Limits = (m_Limits + 1)%4;
    
    if (m_Limits == 0)
        return " ";

    if (m_Limits == 1)
        return "DEPLOYED";

    if (m_Limits == 2)
        return "BASE Turrets";

    return "ALL Turrets";
    
}

//==============================================================================

const char* asset_editor::ToggleSensorLimits ( void )
{
    m_SensorLimits = (m_SensorLimits + 1)%4;
    
    if (m_SensorLimits == 0)
        return " ";

    if (m_SensorLimits == 1)
        return "DEPLOYED";

    if (m_SensorLimits == 2)
        return "BASE Sensors";

    return "ALL Sensors";
    
}

//==============================================================================

void asset_editor::Deselect( void )
{
    g_SelectedAsset = NULL;
}