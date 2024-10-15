//=============================================================================
//
//  Campaign Logic
//
//=============================================================================

#include "GameMgr\GameMgr.hpp"
#include "Demo1\Globals.hpp"
#include "Demo1\fe_Globals.hpp"
#include "UI\UI_Manager.hpp"
#include "HUD\Hud_Manager.hpp"
#include "Objects\Bot\MAI_Manager.hpp"
#include "Objects\Bot\BotObject.hpp"
#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Player\DefaultLoadouts.hpp"
#include "Objects\Vehicles\Bomber.hpp"
#include "Objects\Vehicles\Shrike.hpp"
#include "Objects\Projectiles\Asset.hpp"
#include "Objects\Projectiles\Waypoint.hpp"
#include "Objects\Projectiles\Beacon.hpp"
#include "Objects\Projectiles\Grenade.hpp"
#include "Objects\Projectiles\Bounds.hpp"
#include "GameMgr\MsgMgr.hpp"
#include "Campaign_Logic.hpp"
#include "C06_Logic.hpp"
#include "C07_Logic.hpp"
#include "C08_Logic.hpp"
#include "C09_Logic.hpp"
#include "C10_Logic.hpp"
#include "C11_Logic.hpp"
#include "C12_Logic.hpp"
#include "C13_Logic.hpp"
#include "CT1_Logic.hpp"
#include "CT2_Logic.hpp"
#include "CT3_Logic.hpp"
#include "CT4_Logic.hpp"
#include "CT5_Logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

//=============================================================================
//  VARIABLES
//=============================================================================

campaign_logic Campaign_Logic;

static xtimer s_PadTimer;

xbool g_AutoWinMission   = FALSE;
xbool DebugVehiclePoints = FALSE;

static s32 s_AudioQueue = -1;

//=============================================================================
//  FUNCTIONS
//=============================================================================

campaign_logic::campaign_logic( void )
{
}

//=============================================================================

campaign_logic::~campaign_logic( void )
{
}

//=============================================================================

void campaign_logic::SetCampaignObjectives( s32 Index )
{
    if( Index < 0 )
    {
        tgl.pHUDManager->SetObjective( NULL );
    }
    else
    {
        const xwchar* pString = StringMgr( "Objective", Index );
        ASSERT( pString );
        tgl.pHUDManager->SetObjective( pString );
    }
}

//=============================================================================

void campaign_logic::PrintObjective( s32 Index )
{
    GameText( StringMgr( "Campaign", Index ) );
}

//=============================================================================

void campaign_logic::ErrorText( s32 Index )
{
    xwchar  String[256];
    xwchar* pString = String;
    
    MsgMgr.InjectColor( pString, MsgMgr.GetColor( COLOR_NEGATIVE ) );
    x_wstrcpy( pString, StringMgr( "Campaign", Index ) ); 

    tgl.pHUDManager->Popup( 1, 0, String );
}

//=============================================================================

void campaign_logic::ClearErrorText( void )
{
    tgl.pHUDManager->ClearPopup( 1, 0 );
}

//=============================================================================

void campaign_logic::GameInfo( s32 Index )
{
    xwchar  String[256];
    xwchar* pString = String;

    MsgMgr.InjectColor( pString, MsgMgr.GetColor( COLOR_NEUTRAL ) );
    x_wstrcpy( pString, StringMgr( "Campaign", Index ) ); 
    
    tgl.pHUDManager->Popup( 0, 0, String );
}

//=============================================================================

campaign_logic* campaign_logic::GetCampaignLogic( s32 CampaignMission )
{
    campaign_logic* pCampaignLogic;

    // set the correct campaign mission

    switch( CampaignMission )
    {
        case  6 : pCampaignLogic = &C06_Logic; break;
        case  7 : pCampaignLogic = &C07_Logic; break;
        case  8 : pCampaignLogic = &C08_Logic; break;
//      case  9 : pCampaignLogic = &C09_Logic; break;
        case  9 : pCampaignLogic = &C10_Logic; break;
        case 10 : pCampaignLogic = &C11_Logic; break;
//      case 12 : pCampaignLogic = &C12_Logic; break;
        case 11 : pCampaignLogic = &C13_Logic; break;
        
        default : pCampaignLogic = NULL; DEMAND( 0 );
    }

    return( pCampaignLogic );
}

//=============================================================================

campaign_logic* campaign_logic::GetTrainingLogic( s32 TrainingMission )
{
    campaign_logic* pTrainingLogic;
    
    switch( TrainingMission )
    {
        case 1  : pTrainingLogic = &CT1_Logic; break;
        case 2  : pTrainingLogic = &CT2_Logic; break;
        case 3  : pTrainingLogic = &CT3_Logic; break;
        case 4  : pTrainingLogic = &CT4_Logic; break;
        case 5  : pTrainingLogic = &CT5_Logic; break;
        default : pTrainingLogic = NULL; DEMAND( 0 );
    }

    return( pTrainingLogic );
}

//=============================================================================

void campaign_logic::Initialize( void )
{
    s32 i;
    
    m_Time = 0;
    
    // reset spawn data

    m_nSpawnPoints = 0;

    for( i=0; i<MAX_BOT_SPAWN_POINTS; i++ )
    {
        m_SpawnPoints[i].WaypointID = obj_mgr::NullID;
        m_SpawnPoints[i].Team       = TEAM_ALLIES;
        m_SpawnPoints[i].Wave       = 0;
        m_SpawnPoints[i].nLight     = 0;
        m_SpawnPoints[i].nMedium    = 0;
        m_SpawnPoints[i].nHeavy     = 0;
        m_SpawnPoints[i].nTotal     = 0; 
    }
    
    // reset bot IDs
    
    for( i=0; i<MAX_BOT_PLAYERS; i++ )
    {
        m_AlliesID [i] = obj_mgr::NullID;
        m_EnemiesID[i] = obj_mgr::NullID;
    }

    m_nAllies  = 0;
    m_nEnemies = 0;
    
    // initialize vehicle waypoints
    
    m_nVehPoints   = 0;
    m_NextVehPoint = 0;
    
    for( i=0; i<MAX_VEHICLE_WAYPOINTS; i++ )
    {
        m_VehPoints[i] = obj_mgr::NullID;
    }

    // initialize waypoints
    
    m_nWaypoints = 0;
    
    for( i=0; i<MAX_WAYPOINTS; i++ )
    {
        m_Waypoints[i][0] = obj_mgr::NullID;
        m_Waypoints[i][1] = obj_mgr::NullID;
    }

    // set target team for the player (used when EnterGame is called)

    m_TargetTeam = TEAM_ALLIES;
}

//=============================================================================

static xbool ParseTag( const char* pSpec, s32& nLight, s32& nMedium, s32& nHeavy, s32& iWave )
{
    xbool bLight  = FALSE;
    xbool bMedium = FALSE;
    xbool bHeavy  = FALSE;
    xbool bWave   = FALSE;
    
    nLight  = 0;
    nMedium = 0;
    nHeavy  = 0;
    iWave   = 0;
    
    while( *pSpec )
    {
        // first character of specifier must be a digit
        
        char ch = *pSpec++;
        DEMAND( ch >= '0' && ch <= '9' );
        
        s32 Num = ch - '0';
        
        ch = *pSpec++;

        // check if we have a double digit number?
        
        if( ch >= '0' && ch <= '9' )
        {
            Num = ( Num * 10 ) + ( ch - '0' );
            ch = *pSpec++;
        }

        // determine the number type to set (not allowed to set a number twice)
        
        switch( ch )
        {
            case 'L' : DEMAND( !bLight  ); bLight  = TRUE; nLight  = Num;   break;
            case 'M' : DEMAND( !bMedium ); bMedium = TRUE; nMedium = Num;   break;
            case 'H' : DEMAND( !bHeavy  ); bHeavy  = TRUE; nHeavy  = Num;   break;
            case 'W' : DEMAND( !bWave   ); bWave   = TRUE; iWave   = Num-1; break;  // additional waves start at 2
            default  : DEMAND( FALSE);
        }

        // ensure we have an underscore character or string terminator

        if( *pSpec )
        {
            DEMAND( *pSpec++ == '_' );
        }
    };

    // return FALSE if none of the values were set

    return( bLight | bMedium | bHeavy | bWave );
}

//=============================================================================

void campaign_logic::InitSpawnPoint( object::id ItemID, team Team, s32 Wave, s32 nLight, s32 nMedium, s32 nHeavy )
{
    spawnpt& Point = m_SpawnPoints[ m_nSpawnPoints++ ];

    DEMAND( m_nSpawnPoints <= MAX_BOT_SPAWN_POINTS );

    Point.Team       = Team;
    Point.Wave       = Wave;
    Point.nLight     = nLight;
    Point.nMedium    = nMedium;
    Point.nHeavy     = nHeavy;
    Point.nTotal     = nLight + nMedium + nHeavy;
    Point.WaypointID = ItemID;
}

//=============================================================================

xbool campaign_logic::AddSpawnPoint( object::id ItemID, const char* pTag )
{
    s32 nLight, nMedium, nHeavy, Wave;
    
    //
    // add bioderm spawn points
    //
    
    if( !x_strncmp( pTag, "DERM_\n", 5 ))
    {
        DEMAND( ParseTag( pTag + 5, nLight, nMedium, nHeavy, Wave ));
        
        InitSpawnPoint( ItemID, TEAM_ENEMY, Wave, nLight, nMedium, nHeavy  );
        return( TRUE );
    }

    //
    // add bot spawn points
    //
    
    if( !x_strncmp( pTag, "BOT_\n", 4 ))
    {
        DEMAND( ParseTag( pTag + 4, nLight, nMedium, nHeavy, Wave ));

        InitSpawnPoint( ItemID, TEAM_ALLIES, Wave, nLight, nMedium, nHeavy  );
        return( TRUE );
    }

    return( FALSE );
}

//=============================================================================

void campaign_logic::PlayerDied( const pain& Pain )
{
    // check if it is the player or a bot who died

    if( Pain.VictimID == m_PlayerID )
    {
        m_bPlayerDied = TRUE;
    }
    else
    {
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( Pain.VictimID );
        DEMAND( pPlayer );
        
        if( pPlayer->GetTeamBits() & ALLIED_TEAM_BITS )
        {
            m_bAlliedDied = TRUE;

            // set the armor type of allied bot that died
        
            m_DeadArmorType = pPlayer->GetArmorType();
        }
    }

    game_logic::PlayerDied( Pain );
}

//=============================================================================

void campaign_logic::PlayerOnVehicle( object::id PlayerID, object::id VehicleID )
{
    (void)VehicleID;
    
    if( PlayerID == m_PlayerID )
    {
        m_bPlayerOnVehicle = TRUE;
        m_VehicleID        = VehicleID;
    }
}

//=============================================================================

void campaign_logic::SwitchTouched( object::id SwitchID, object::id PlayerID )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( PlayerID );
    DEMAND( pPlayer );
    (void)SwitchID;
    
    if( pPlayer->GetTeamBits() & ALLIED_TEAM_BITS )
    {
        m_bSwitchLost = FALSE;
        m_SwitchState = TEAM_ALLIES;

        // order a bot to go and reclaim the switch         
        
        // OrderEnemies( TASK_GOTO, SwitchID );
    }
    else
    {
        m_bSwitchLost = TRUE;
        m_SwitchState = TEAM_ENEMY;
    }
    
    m_bSwitchTouched = TRUE;
    m_SwitchID       = SwitchID;
}

//=============================================================================

f32 campaign_logic::DistanceToObject( object::id FromID, object::id ToID )
{
    object* pObject1 = ObjMgr.GetObjectFromID( FromID );
    object* pObject2 = ObjMgr.GetObjectFromID(  ToID  );
    DEMAND( pObject1 );
    DEMAND( pObject2 );

    // calculate distance from first object to the second

    vector3 VectorTo = pObject2->GetPosition() - pObject1->GetPosition();
    return( VectorTo.Length() );
}

//=============================================================================

f32 campaign_logic::DistanceToObject( object::id ID )
{
    // calculate distance to object from the player

    return( DistanceToObject( m_PlayerID, ID ));
}

//=============================================================================

f32 campaign_logic::DistanceToClosestPlayer( object::id ID, team Team )
{
    object* pObject;

    f32 MinDistance = 100000.0f;
    u32 TeamBits;

    if( Team == TEAM_ALLIES )
    {
        TeamBits = ALLIED_TEAM_BITS;
    }
    else
    {
        TeamBits = ENEMY_TEAM_BITS;
    }

    //
    // loop through all bots in the specified team and compute the minimum distance from the object
    //
    
    ObjMgr.Select( object::ATTR_PLAYER );
    while(( pObject = ObjMgr.GetNextSelected() ))
    {
        // ensure we dont compute the distance to ourselves
    
        if( pObject->GetObjectID() == ID )
        {
            continue;
        }
    
        // are we interested in this bot?
    
        if( pObject->GetTeamBits() & TeamBits )
        {
            f32 Distance = DistanceToObject( ID, pObject->GetObjectID() );
            
            if( Distance < MinDistance )
            {
                MinDistance = Distance;
            }
        }
    }
    ObjMgr.ClearSelection();
    
    return( MinDistance );
}

//=============================================================================

object::id campaign_logic::CreateBot( vector3&                   Position,
                                      bot_object::character_type CharType,
                                      bot_object::skin_type      SkinType,
                                      bot_object::voice_type     VoiceType,
                                      default_loadouts::type     LoadoutType,
                                      s32                        Wave )
{
    s32 PlayerIndex;
    object::id  BotID;
    bot_object* pBot;
    
    xwstring Name( "Bot" );

    GameMgr.Connect( PlayerIndex,
                     0,
                     (const xwchar*)Name,
                     eng_GetTVRefreshRate(),
                     CharType,
                     SkinType,
                     VoiceType,
                     TRUE );

    //
    // add a spawn point for this bot just in case there isnt one in the mission file
    //

    ASSERT( tgl.NSpawnPoints < MAX_SPAWN_POINTS );

    u32 TeamBits = ( CharType == player_object::CHARACTER_TYPE_BIO ) ? ENEMY_TEAM_BITS : ALLIED_TEAM_BITS;

    tgl.SpawnPoint[ tgl.NSpawnPoints ].Setup( TeamBits, Position.X, Position.Y, Position.Z, R_0, R_0 );
    tgl.NSpawnPoints++;
    
    BotID = GameMgr.EnterGame( PlayerIndex );
    pBot  = (bot_object*)ObjMgr.GetObjectFromID( BotID );
    
    if( pBot )
    {
        player_object::loadout Loadout;

        default_loadouts::GetLoadout( CharType, LoadoutType, Loadout );
        
        pBot->SetInventoryLoadout( Loadout );
        pBot->InventoryReload();

        // warp this bot to the correct positon

        pBot->SetPos( Position );
        pBot->SetRespawn( FALSE );

        pBot->DisableLoadout();
        
        pBot->SetWaveBit( 1 << Wave );
    }

    return( BotID );
}

//=============================================================================

void campaign_logic::DestroyBot( object::id BotID )
{
    GameMgr.ExitGame  ( BotID.Slot );
    GameMgr.Disconnect( BotID.Slot );
}

//=============================================================================

static s32 FindFreeSlot( object::id* pID )
{
    for( s32 i=0; i<MAX_BOT_PLAYERS; i++ )
    {
        if( pID[i] == obj_mgr::NullID )
        {
            return( i );
        }
    }

    // there must be no free slots!

    DEMAND( FALSE );
    return( 0 );
}

//=============================================================================

void campaign_logic::AddBot( team Team, object::id BotID )
{
    if( Team == TEAM_ALLIES )
    {
        s32 Index = FindFreeSlot( m_AlliesID );
        
        m_AlliesID[ Index ] = BotID;
        m_nAllies++;
    }
    else
    {
        s32 Index = FindFreeSlot( m_EnemiesID );

        m_EnemiesID[ Index ] = BotID;
        m_nEnemies++;
    }
}

//=============================================================================

void campaign_logic::RemoveBot( object::id BotID )
{
    object* pObject = ObjMgr.GetObjectFromID( BotID );
    DEMAND( pObject );

    xbool bAllied = pObject->GetTeamBits() & ALLIED_TEAM_BITS;
    object::id *pID;

    // get pointer to the team ID list
    
    if( bAllied )
    {
        pID = m_AlliesID;
    }
    else
    {
        pID = m_EnemiesID;
    }

    s32 nRemoved = 0;

    //
    // look for object and remove it from list
    //

    for( s32 i=0; i<MAX_BOT_PLAYERS; i++ )
    {
        if( pID[i] == BotID )
        {
            pID[i] = obj_mgr::NullID;

            // decrement counters
            
            if( bAllied )
            {
                m_nAllies--;
                DEMAND( m_nAllies >= 0 );
            }
            else
            {
                m_nEnemies--;
                DEMAND( m_nEnemies >= 0 );
            }
            
            nRemoved++;
        }
    }

    // ensure the passed object was found and it was only registered once

    //DEMAND( nRemoved == 1 );
}

//=============================================================================

void campaign_logic::RemoveDeadBots( object::id* pBotID )
{
    for( s32 i=0; i<MAX_BOT_PLAYERS; i++ )
    {
        object::id BotID = pBotID[i];
    
        if( BotID != obj_mgr::NullID )
        {
            player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( BotID );

            // make sure player is still in object manager

            if( pPlayer )
            {
                // check if the player is dead
            
                if( pPlayer->GetHealth() == 0 )
                {
                    RemoveBot ( BotID );
                    DestroyBot( BotID );
                }
            }
        }
    }
}

//=============================================================================

s32 SpawnRadius = 20;

void campaign_logic::SpawnBots( team Team, s32 Wave )
{
    DEMAND(( Team  == TEAM_ALLIES ) || ( Team == TEAM_ENEMY ));

    //
    // set the type of bot were going to create
    //

    bot_object::character_type CharType;
    bot_object::skin_type      SkinType;
    bot_object::voice_type     VoiceType;
    default_loadouts::type     DefaultLoadoutType = default_loadouts::STANDARD_BOT_LIGHT;

    if( Team == TEAM_ALLIES )
    {
        if( x_rand() & 1 ) CharType = player_object::CHARACTER_TYPE_FEMALE; 
        else               CharType = player_object::CHARACTER_TYPE_MALE;
        
        VoiceType = player_object::VOICE_TYPE_MALE_HERO;        // bots dont ever speak so just use any type
    }
    else
    {
        CharType  = player_object::CHARACTER_TYPE_BIO;
        VoiceType = player_object::VOICE_TYPE_BIO_MONSTER;
    }

    switch( x_rand() & 3 )
    {
        case 0  : SkinType = player_object::SKIN_TYPE_BEAGLE; break;
        case 1  : SkinType = player_object::SKIN_TYPE_COTP;   break;
        case 2  : SkinType = player_object::SKIN_TYPE_DSWORD; break;
        default : SkinType = player_object::SKIN_TYPE_SWOLF;  break;
    }

    //
    // find all spawn points matching this Team and attack Wave
    //

    for( s32 i=0; i<m_nSpawnPoints; i++ )
    {
        spawnpt& Point = m_SpawnPoints[i];
        
        if(( Point.Team != Team ) || ( Point.Wave != Wave ))
        {
            continue;
        }
        
        // get the position of the waypoint
                
        object* pObject = ObjMgr.GetObjectFromID( Point.WaypointID );
        DEMAND( pObject );
        
        vector3 Centre  = pObject->GetPosition();

        radian Ang = R_360 * ( (f32)x_rand() / X_RAND_MAX );
        radian Add = R_360 / Point.nTotal;

        s32 nLight  = Point.nLight;
        s32 nMedium = Point.nMedium;
        s32 nHeavy  = Point.nHeavy;

        //
        // add all light, medium and heavy bot types
        //

        for( s32 n=0; n<Point.nTotal; n++ )
        {
            if( nLight )
            {
                nLight--;
                DefaultLoadoutType = default_loadouts::STANDARD_BOT_LIGHT ;
            }
            else
            {
                if( nMedium )
                {
                    nMedium--;
                    DefaultLoadoutType = default_loadouts::STANDARD_BOT_MEDIUM ;
                }
                else
                {
                    if( nHeavy )
                    {
                        nHeavy--;
                        DefaultLoadoutType = default_loadouts::STANDARD_BOT_HEAVY ;
                    }
                }
            }

            //
            // generate a random-ish point around waypoint
            //

            f32 R = 3.0f + (( SpawnRadius - 3.0f ) * ABS( x_sin( Ang )) );
            f32 X = R * x_sin( Ang );
            f32 Z = R * x_cos( Ang );
        
            vector3 Position;

            // if this is a single bot spawn point, then use the exact position because
            // these are typically used only inside buildings

            if( Point.nTotal == 1 )
            {
                // Single bot creation
                Position = Centre;
            }
            else
            {
                // Multiple bot creation
                Position.X = Centre.X + X;
                Position.Z = Centre.Z + Z;
                Position.Y = tgl.pTerr->GetHeight( Position.Z, Position.X ) + 5.0f;
            }

            object::id BotID = obj_mgr::NullID;

            // set the team that this bot will be assigned to
            // note that this is used when the bot calls EnterGame()

            m_TargetTeam = Team;

            //
            // create the bot
            //

            BotID = CreateBot( Position,
                               CharType,
                               SkinType,
                               VoiceType,
                               DefaultLoadoutType,
                               Wave );

            DEMAND( BotID != obj_mgr::NullID );

            // turn off respawning for this bot
            
            player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( BotID );
            pPlayer->SetRespawn( FALSE );

            // add the object ID into our internal list of bots
        
            AddBot( Team, BotID );
            
            Ang += Add;
        }
    }
}

//=============================================================================

void campaign_logic::SpawnAllies( s32 Wave )
{
    SpawnBots( TEAM_ALLIES, Wave );
}

//=============================================================================

void campaign_logic::SpawnAllies( void )
{
    SpawnBots( TEAM_ALLIES, 0 );
}

//=============================================================================

void campaign_logic::SpawnEnemies( s32 Wave )
{
    SpawnBots( TEAM_ENEMY, Wave );
}

//=============================================================================

void campaign_logic::SpawnEnemies( void )
{
    SpawnBots( TEAM_ENEMY, 0 );
}

//=============================================================================

void campaign_logic::OrderBots( team Team, task Task, object::id ObjectID, u32 WaveBits )
{
    s32 TeamBits;
    vector3 Position;

    if( Team == TEAM_ALLIES )
    {
        TeamBits = ALLIED_TEAM_BITS;
    }
    else
    {
        TeamBits = ENEMY_TEAM_BITS;
    }

    //
    // get any required object information
    //
    
    if( ObjectID != obj_mgr::NullID )
    {
        object* pObject = ObjMgr.GetObjectFromID( ObjectID );
        DEMAND( pObject );
        
        Position = pObject->GetPosition();
    }
    else
    {
        // DeathMatch and Antagonize are the only tasks that don't require an object passed in
        
        DEMAND(( Task == TASK_DEATHMATCH ) || ( Task == TASK_ANTAGONIZE ));
    }

    //
    // call the necessary AI functions
    //

    switch( Task )
    {
        case TASK_ATTACK     :
            
            MAI_Mgr.Attack( TeamBits, ObjectID, WaveBits );
            break;
        
        case TASK_GOTO       :
            
            MAI_Mgr.Roam( TeamBits, Position, WaveBits );
            break;
        
        case TASK_DEFEND     :
        
            MAI_Mgr.Defend( TeamBits, Position, WaveBits );
            break;
        
        case TASK_PATROL     :
        
            // Patrol uses the Roam() function since it makes the bots roam within a radius of the Position
            
            MAI_Mgr.Roam( TeamBits, Position, WaveBits );
            break;

        case TASK_MORTAR     :    
            
            MAI_Mgr.Mortar( TeamBits, ObjectID, WaveBits );
            break;
            
        case TASK_REPAIR     :
        
            MAI_Mgr.Repair( TeamBits, ObjectID );
            break;
            
        case TASK_DESTROY    :

            // Destroy is different from attack in that it's used to destroy OBJECTS and not PLAYERS
        
            MAI_Mgr.Destroy( TeamBits, ObjectID, WaveBits );
            break;
        
        case TASK_DEATHMATCH :    
            
            MAI_Mgr.DeathMatch( TeamBits, WaveBits );
            break;

        case TASK_ANTAGONIZE :
            
            MAI_Mgr.Antagonize( TeamBits, WaveBits );
            break;
            
        default              :
            
            DEMAND( FALSE );
            break;
    }
}

//=============================================================================

void campaign_logic::OrderAllies( task Task, object::id ObjectID, s32 Wave )
{
    u32 WaveBits = ( Wave == ATTACK_WAVE_ALL ) ? 0xFFFFFFFF : ( 1 << Wave );

    OrderBots( TEAM_ALLIES, Task, ObjectID, WaveBits );
}

//=============================================================================

void campaign_logic::OrderAllies( task Task, s32 Wave )
{
    OrderAllies( Task, obj_mgr::NullID, Wave );
}

//=============================================================================

void campaign_logic::OrderEnemies( task Task, object::id ObjectID, s32 Wave )
{
    u32 WaveBits = ( Wave == ATTACK_WAVE_ALL ) ? 0xFFFFFFFF : ( 1 << Wave );
    
    OrderBots( TEAM_ENEMY, Task, ObjectID, WaveBits );
}

//=============================================================================

void campaign_logic::OrderEnemies( task Task, s32 Wave )
{
    OrderEnemies( Task, obj_mgr::NullID, Wave );
}

//=============================================================================

void campaign_logic::AddWaypoint( object::id ID )
{
    object* pObject = ObjMgr.GetObjectFromID( ID );
    s32 i;
    
    // check if object is in the object manager

    if( !pObject ) return;
    
    // check if the object requiring a waypoint IS a waypoint
    
    if( pObject->GetType() == object::TYPE_WAYPOINT )
    {
        waypoint* pWaypoint = (waypoint*)pObject;
        pWaypoint->SetHidden( FALSE );
    }
    else
    {
        // check if this object is an asset
    
        if( pObject->GetAttrBits() & object::ATTR_ASSET )
        {
            // look for the next free slot
    
            for( i=0; i<MAX_WAYPOINTS; i++ )
            {
                if( m_Waypoints[i][0] == obj_mgr::NullID )
                {
                    // create a waypoint on this object
            
                    waypoint* pWaypoint = (waypoint*)ObjMgr.CreateObject( object::TYPE_WAYPOINT );
                    DEMAND( pWaypoint );

                    // set the position and switch circuit of the waypoint to that of the object
                    // note: all objects that can have waypoints added to them are ASSETS

                    asset* pAsset = (asset*)pObject;
                    s32 Switch    = pAsset->GetSwitchCircuit();
                
                    pWaypoint->Initialize( pObject->GetPosition(), Switch );
                    ObjMgr.AddObject( pWaypoint, obj_mgr::AVAILABLE_SERVER_SLOT );

                    // store ID of waypoint object
                
                    m_Waypoints[i][0] = ID;
                    m_Waypoints[i][1] = pWaypoint->GetObjectID();
                    
                    break;
                }
            }
            
            // no slots free!
        
            DEMAND( i != MAX_WAYPOINTS );
        }
    }
}

//=============================================================================

void campaign_logic::ClearWaypoint( object::id ID )
{
    object* pObject = ObjMgr.GetObjectFromID( ID );

    // check if object is in the object manager

    if( !pObject ) return;

    // check if the object IS a waypoint

    if( pObject->GetType() == object::TYPE_WAYPOINT )
    {
        waypoint* pWaypoint = (waypoint*)pObject;
        pWaypoint->SetHidden( TRUE );
    }
    else
    {
        s32 i;
        
        // look for the object in the list
    
        for( i=0; i<MAX_WAYPOINTS; i++ )
        {
            if( ID == m_Waypoints[i][0] )
            {
                waypoint* pWaypoint = (waypoint*)ObjMgr.GetObjectFromID( m_Waypoints[i][1] );
                DEMAND( pWaypoint );
                
                ObjMgr.RemoveObject ( pWaypoint );
                ObjMgr.DestroyObject( pWaypoint );
                
                m_Waypoints[i][0] = obj_mgr::NullID;
                m_Waypoints[i][1] = obj_mgr::NullID;
                break;
            }
        }
    }
}

//=============================================================================

void campaign_logic::SetState( s32 State )
{
    //
    // when an IDLE state is set, ignore any subsequent state changes as we are exiting the mission
    //
    
    if( m_bExit )
    {
        return;
    }
    
    if( State == STATE_IDLE )
    {
        m_bExit = TRUE;
    }

    m_GameState    = State;
    m_bInitialized = FALSE;
    m_Time         = 0;
}

//=============================================================================

void campaign_logic::GamePrompt( s32 ID, xbool Override )
{
    //
    // dont accept any more games prompts when we are going to be exiting the mission
    //
    
    if( m_bExit )
    {
        return;
    }

    (void)Override;
    /*
    s32 Flags = AUDFLAG_QUEUED;

    Override = TRUE;    // for now just override all audio requests

    if( Override )
    {
        // this flag will cancel current speech and start the new one immediately
    
        Flags |= AUDFLAG_CHANNELSAVER;
    }
    
    m_ChannelID = audio_Play( ID, NULL, Flags );
    */

    if( audio_IsPlaying( m_ChannelID ) == TRUE )
    {
        s_AudioQueue = ID;
    }
    else
    {
        GameAudio( ID );
    }
}

//=============================================================================

void campaign_logic::GameAudio( s32 ID )
{
    audio_Stop( m_ChannelID );
    
    m_ChannelID = audio_Play( ID );
}

//=============================================================================

void campaign_logic::GameText( const xwchar* pText )
{
    tgl.pHUDManager->SetObjective( pText );
}

//=============================================================================

void campaign_logic::ClearText( void )
{
    tgl.pHUDManager->SetObjective( NULL );
}

//=============================================================================

const f32 EndGameTimeout = 2.0f;

void campaign_logic::BeginGame( void )
{
    m_PlayerID             = GameMgr.m_PlayerID[0];
    m_GameState            = STATE_IDLE;
    m_bExit                = FALSE;
    m_Timeout              = EndGameTimeout;
    m_nAllies              = 0;
    m_nEnemies             = 0;
    m_nWaypoints           = 0;
    m_ChannelID            = 0;
    m_VehicleID            = obj_mgr::NullID;
    
    m_bPlayerOnVehicle     = FALSE;
    m_bPlayerDied          = FALSE;
    m_bAlliedDied          = FALSE;
    m_bSwitchTouched       = FALSE;
    m_bSwitchLost          = FALSE;
    m_bHasRepairPack       = FALSE;
    m_bPickedUpRepairPack  = FALSE;
    m_bPickupTouched       = FALSE;
    m_bNewTarget           = FALSE;
    m_bFireWeapon          = FALSE;
    m_bWeaponFired         = FALSE;
    m_bPathCompleted       = FALSE;

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    pPlayer->SetRespawn( FALSE );

    // disable onscreen timer and mission timeout
    GameMgr.SetTimeLimit( -1 );

    s_PadTimer.Reset();
    ClearText();

    bounds_SetFatal( TRUE );
    
    audio_SetContainerVolume( tgl.MusicPackageId, tgl.MusicVolume * 0.5f );
    
    g_AutoWinMission = FALSE;
}

//=============================================================================

void campaign_logic::EndGame( void )
{
    DisablePadButtons( 0 );
    fegl.PlayerInvulnerable = FALSE;

    audio_SetContainerVolume( tgl.MusicPackageId, tgl.MusicVolume );
}

//=============================================================================

void campaign_logic::Success( s32 ID )
{
    tgl.MissionSuccess = TRUE;
    if( ID > 0 ) GameAudio( ID );
    SetState( STATE_IDLE );
    
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // HACK: Remember to REMOVE these lines when the debrief screen goes in.
    
    // Increment the players available missions
/*
    if( tgl.HighestCampaignMission == fegl.LastPlayedCampaign )
    {
        tgl.HighestCampaignMission = 16;
    }
*/
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}

//=============================================================================

void campaign_logic::Failed( s32 ID )
{
    tgl.MissionSuccess = FALSE;
    GameAudio( ID );
    SetState( STATE_IDLE );
}

//=============================================================================

xbool KillAudio = FALSE;

void campaign_logic::AdvanceTime( f32 DeltaTime )
{
    // TODO: REMOVE THIS!
    if( KillAudio )
    {
        KillAudio = FALSE;
        audio_Stop( m_ChannelID );
    }

    EnforceBounds( DeltaTime );

    if( s_AudioQueue != -1 )
    {
        if( audio_IsPlaying( m_ChannelID ) == FALSE )
        {
            GameAudio( s_AudioQueue );
            s_AudioQueue = -1;
        }
    }

    if( g_AutoWinMission )
    {
        g_AutoWinMission = FALSE;
        Success( -1 );
    }

    RemoveDeadBots( m_AlliesID  );
    RemoveDeadBots( m_EnemiesID );

    //
    // check for the end of the mission
    //

    if( m_bExit )
    {
        //s32 Status = audio_GetStatus( (u32)-1 );
        //if( Status == AUDSTAT_FREE )
        
        if( audio_IsPlaying( m_ChannelID ) == FALSE )
        {
            m_Timeout -= DeltaTime;
            
            if( m_Timeout <= 0 )
            {
                tgl.Playing = FALSE;
                GameMgr.EndGame();
            }
        }
    }

    //
    // check if player has a repair pack
    //
    
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );
    
    if( pPlayer->GetInventCount( player_object::INVENT_TYPE_ARMOR_PACK_REPAIR ))
    {
        // check if player has just picked up a repair pack
        
        if( !m_bHasRepairPack )
        {
            m_bPickedUpRepairPack = TRUE;
        }
    
        m_bHasRepairPack = TRUE;
    }
    else
    {
        m_bHasRepairPack = FALSE;
    }
    
    // Check if player goes past the outer bounds
    if( pPlayer->GetPos().Length() > 5120.0f )
        Failed( M13_FAIL );
}

//=============================================================================

void campaign_logic::EnterGame( s32 PlayerIndex )
{
    s32 Team;

    // Need to choose a team for this player.
    
    if( m_TargetTeam == TEAM_ALLIES )
    {
        Team = 0;
    }
    else 
    {
        Team = 1;
    }

    GameMgr.m_Score.Player[PlayerIndex].Team = Team;
    GameMgr.m_Score.Team[Team].Size++;
}

//=============================================================================

void campaign_logic::ExitGame( s32 PlayerIndex )
{
    s32 Team = GameMgr.m_Score.Player[PlayerIndex].Team;
    GameMgr.m_Score.Team[Team].Size--;
}

//=============================================================================

void campaign_logic::EnforceBounds( f32 DeltaTime )
{   
    object::id ID( 0, -1 );
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( ID );

    if( !pPlayer )
        return;

    vector3 Position = pPlayer->GetPosition();

    if( (Position.X < GameMgr.m_Bounds.Min.X) ||
        (Position.Z < GameMgr.m_Bounds.Min.Z) ||
        (Position.X > GameMgr.m_Bounds.Max.X) ||
        (Position.Z > GameMgr.m_Bounds.Max.Z) )
    {
        ObjMgr.InflictPain( pain::MISC_BOUNDS_HURT,
                            pPlayer, 
                            Position, 
                            ObjMgr.NullID, 
                            ID, 
                            TRUE,
                            DeltaTime );
    }           
}

//=============================================================================

void campaign_logic::GetLaserTargets( void )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    if( pPlayer->GetWeaponCurrentType() == player_object::INVENT_TYPE_WEAPON_TARGETING_LASER )
    {
        object::id Target = pPlayer->GetWeaponTargetID();

        // have we got a new target?

        if( Target != obj_mgr::NullID )
        {
            m_TargetID   = Target;
            m_bNewTarget = TRUE;
        }
    }
}

//=============================================================================

xbool campaign_logic::AddVehiclePoint( object::id ItemID, const char* pTag )
{
    if( !x_strncmp( pTag, "VEHPT", 5 ))
    {
        // convert vehicle waypoint number to an index (numbering starts at 1)

        s32 Index = x_atoi( pTag + 5 ) - 1;
        
        DEMAND(( Index >= 0 ) && ( Index < MAX_VEHICLE_WAYPOINTS ));    // ensure we dont get too many waypoints
        DEMAND( m_VehPoints[Index] == obj_mgr::NullID );                // ensure we dont get 2 waypoints the same
        
        m_VehPoints[Index] = ItemID;
        
        m_nVehPoints++;
        DEMAND( m_nVehPoints < MAX_VEHICLE_WAYPOINTS );
    
        return( TRUE );
    }

    return( FALSE );
}

//=============================================================================

object::id campaign_logic::InitVehicle( object::type Type )
{
    DEMAND( m_nVehPoints >= 2 );
    
    //
    // go through list and check for missing waypoints
    //
    
    for( s32 i=0; i<m_nVehPoints; i++ )
    {
        if( m_VehPoints[i] == obj_mgr::NullID )
        {
            DEMAND( FALSE );
        }
    }

    //
    // calculate initial position and heading for vehicle
    //

    object* pVehPoint0 = ObjMgr.GetObjectFromID( m_VehPoints[0] );
    object* pVehPoint1 = ObjMgr.GetObjectFromID( m_VehPoints[1] );
    DEMAND( pVehPoint0 );
    DEMAND( pVehPoint1 );

    vector3 Position = pVehPoint0->GetPosition();
    vector3 NextPos  = pVehPoint1->GetPosition();
    vector3 Vector   = NextPos - Position;
    radian  Heading  = Vector.GetYaw();

    //
    // create the vehicle object
    //
        
    object* pObject = ObjMgr.CreateObject( Type );
    DEMAND( pObject );

    switch( Type )
    {
        case object::TYPE_VEHICLE_THUNDERSWORD :
        {
            bomber* pBomber = (bomber*)pObject;
            pBomber->Initialize( Position, Heading, 0x01 );
            ObjMgr.AddObject( pBomber, obj_mgr::AVAILABLE_SERVER_SLOT );

            // calculate initial positions of bots

            vector3 PilotPos  = pBomber->GetSeatPos( 0 );
            
            PilotPos.Y  += 2.0f;

            m_TargetTeam = TEAM_ALLIES;
            
            m_PilotID  = CreateBot( PilotPos,
                                    player_object::CHARACTER_TYPE_MALE,
                                    player_object::SKIN_TYPE_BEAGLE,
                                    player_object::VOICE_TYPE_MALE_HERO,
                                    default_loadouts::STANDARD_BOT_LIGHT );

            // turn off respawning for these bots
            
            player_object* pPlayer1 = (player_object*)ObjMgr.GetObjectFromID( m_PilotID  );
            ASSERT( pPlayer1 );
            
            pPlayer1->SetRespawn( FALSE );
            
            // allow these bots to enter the vehicle
            
            bot_object* pBot1 = (bot_object*)pPlayer1;
            pBot1->SetAllowedInVehicle( TRUE );
            break;
        }
        
        case object::TYPE_VEHICLE_SHRIKE :
        {
            shrike* pShrike = (shrike*)pObject;
            pShrike->Initialize( Position, Heading, 0x01 );
            ObjMgr.AddObject( pShrike, obj_mgr::AVAILABLE_SERVER_SLOT );
            break;
        }

        default : DEMAND( 0 );
    }
    
    m_NextVehPoint   = 0;
    m_VehicleDelay   = 0;
    m_bPathCompleted = FALSE;

    return( pObject->GetObjectID() );
}

//=============================================================================

xbool campaign_logic::LaunchBombs( vehicle* pVehicle )
{
    vector3 Position = pVehicle->GetPosition();
    vector3 Velocity = pVehicle->GetVel();
    vector3 Normal   = Velocity;
    Normal.Normalize();
    
    f32 Gravity     = 25.0f;
    f32 BlastRadius = 35.0f;

    // calculate the vehicle height above ground

    f32 Y = tgl.pTerr->GetHeight( Position.Z, Position.X );
    f32 H = Position.Y - Y;
    
    // do not launch bombs if the vehicle is not high enough to avoid being damaged from the blast
           
    if( H < BlastRadius ) return( FALSE );

    // calculate the time required for bomb to hit ground, and horizontal distance it will travel
    
    f32 T = x_sqrt(( 2.0f * H ) / Gravity );
    f32 V = Velocity.Length();
    f32 L = V * T;

    //
    // loop through all players looking for enemies within range
    //

    object* pObject;
    ObjMgr.Select( object::ATTR_PLAYER );
    while( (pObject = ObjMgr.GetNextSelected()) )
    {
        if( pObject->GetTeamBits() & ENEMY_TEAM_BITS )
        {
            vector3 Target = pObject->GetPosition();

            if( Target.Y > Position.Y ) continue;
            
            // calculate the location of the bomb epicentre ignoring ground elevation
            
            vector3 Centre = Position + (Normal * L);

            // check if enemy player is within the blast radius

            vector2 V0 = vector2( Centre.X, Centre.Z );
            vector2 V1 = vector2( Target.X, Target.Z );
        
            vector2 Vector = V1 - V0;
            f32 Distance   = Vector.Length();

            if( Distance < BlastRadius )
            {
                // got an enemy player in range of blast so bomb him!
                break;
            }
        }
    };
    ObjMgr.ClearSelection();

    return( pObject != NULL );
}

//=============================================================================

void campaign_logic::ControlBot( object::id BotID, player_object::move& Movement )
{
    bot_object* pBot = (bot_object*)ObjMgr.GetObjectFromID( BotID );

    if( pBot )
    {
        if( pBot->GetHealth() == 0.0f )
            Movement.Clear();
        
        pBot->SetMove( Movement );
    }
}

//=============================================================================

const f32 VehiclePointRadius = 50.0f;
const f32 VehicleThrust      =  0.5f;
const f32 VehicleMaxAngle    =  R_35;

xbool campaign_logic::UpdateVehicle( object::id VehicleID, f32 DeltaTime )
{
    player_object::move Movement;
    
    //
    // dont do anything until the vehicle object is actually created in the world
    //

    if( VehicleID == obj_mgr::NullID )
    {
        return( FALSE );
    }

    //
    // dont control vehicle until bot is correctly seated
    //
    
    if( m_VehicleDelay < 2.0f )
    {
        m_VehicleDelay += DeltaTime;
        return( FALSE );
    }

    //
    // check for the pilot dying
    //
    
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PilotID  );

    if( !pPlayer || ( pPlayer->GetHealth() == 0 )) return( TRUE );

    //
    // check for vehicle being destroyed
    //

    gnd_effect* pVehicle = (gnd_effect*)ObjMgr.GetObjectFromID( VehicleID );

    if( !pVehicle || pVehicle->GetHealth() == 0 )
    {
        // Kill the pilot
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PilotID  );
        ASSERT( pPlayer );
        ObjMgr.InflictPain( pain::EXPLOSION_BOMBER, pPlayer, pPlayer->GetPos(), obj_mgr::NullID, pPlayer->GetObjectID(), TRUE, 100.0f );
        return( TRUE );
    }

    //
    // check for vehicle completing path
    //
    
    if( m_bPathCompleted )
    {
        Movement.Clear();
        ControlBot( m_PilotID, Movement );
    
        return( FALSE );
    }
    
    //
    // calculate the pitch and yaw to head us toward the next waypoint
    //
    
    vector3 Position;
    radian3 Rotation;
    radian  Pitch, Yaw;
    f32     LookX, LookY;
    
    Rotation = pVehicle->GetRot() ;
    Position = pVehicle->GetPosition();

    object* pObject = ObjMgr.GetObjectFromID( m_VehPoints[ m_NextVehPoint ] );
    DEMAND( pObject );
    
    vector3 VehPoint = pObject->GetPosition();
    vector3 Vector   = VehPoint - Position;

    // within range of target waypoint?

    if( Vector.Length() < VehiclePointRadius )
    {
        m_NextVehPoint++;
        
        if( m_NextVehPoint == m_nVehPoints )
        {
            m_bPathCompleted = TRUE;
            m_NextVehPoint   = 0;
        }
    }
    
    Vector.GetPitchYaw( Pitch, Yaw );
    
    radian PDiff = x_ModAngle2( Rotation.Pitch - Pitch );
    radian YDiff = x_ModAngle2( Rotation.Yaw   - Yaw   );
    
    YDiff = MINMAX( -VehicleMaxAngle, YDiff, VehicleMaxAngle );
    LookY = MINMAX( -1.0f, -PDiff / VehicleMaxAngle, 1.0f );
    LookX = MINMAX( -1.0f, -YDiff / VehicleMaxAngle, 1.0f );
    
    //
    // setup movement structure for pilot
    //

    Movement.Clear();
    
    Movement.LookX     = LookX;         // set yaw input
    Movement.LookY     = LookY;         // set pitch input
    Movement.MoveX     = LookX;         // set bank
    Movement.MoveY     = VehicleThrust; // set thrust
    Movement.DeltaTime = DeltaTime;

    // if we are attempting to climb, then add a little turbo boost
    
    if( LookY < 0 )
    {
        //Movement.JetKey = 1;
        //Movement.MoveY  = 1.0f;
    }

    if( m_bFireWeapon )
    {
        m_bFireWeapon    = FALSE;
        Movement.FireKey = 1;
        Movement.MoveY  *= 0.5f;    // Slow down the vehicle when bombing
    }

    // Dont control the pilot until he is in the seat (delay due to vehicle spawning)
    if( pPlayer->IsAttachedToVehicle() == NULL )
        Movement.Clear();

    ControlBot( m_PilotID, Movement );

    return( FALSE );
}

//=============================================================================

void campaign_logic::FireVehicleWeapon( void )
{
    m_bFireWeapon = TRUE;
}

//=============================================================================

void campaign_logic::RenderVehiclePoints( void )
{
    for( s32 i=0; i<m_nVehPoints; i++ )
    {
        object* pObject = ObjMgr.GetObjectFromID( m_VehPoints[i] );
        if( pObject )
        {
            vector3 Pos( pObject->GetPosition() );
        
            draw_Point ( Pos,       XCOLOR_RED   );
            draw_Sphere( Pos, 2.0f, XCOLOR_GREEN );
        }
    }
}

//=============================================================================

void campaign_logic::DestroyItem( object::id ItemID )
{
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );
    
    if( pObject->GetAttrBits() & object::ATTR_ASSET )
    {
        asset* pAsset = (asset*)pObject;
        
        pAsset->ForceDestroyed();
    }
}

//=============================================================================

void campaign_logic::PowerLoss( object::id ItemID )
{
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );
    
    if( pObject->GetAttrBits() & object::ATTR_ASSET )
    {
        asset* pAsset = (asset*)pObject;
        GameMgr.PowerLoss( pAsset->GetPowerCircuit() );
    }
}

//=============================================================================

void campaign_logic::PowerGain( object::id ItemID )
{
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );
    
    if( pObject->GetAttrBits() & object::ATTR_ASSET )
    {
        asset* pAsset = (asset*)pObject;
        GameMgr.PowerGain( pAsset->GetPowerCircuit() );
    }
}

//=============================================================================

static void SetButtonMask( u32 PadBits, player_object::player_input& Input, s32 Value )
{
    if( PadBits & PAD_JUMP_BUTTON            ) Input.JumpKey           = Value;
    if( PadBits & PAD_JET_BUTTON             ) Input.JetKey            = Value;
    if( PadBits & PAD_FIRE_BUTTON            ) Input.FireKey           = Value;
    if( PadBits & PAD_EXCHANGE_WEAPON_BUTTON ) Input.ExchangeKey       = Value;
    if( PadBits & PAD_NEXT_WEAPON_BUTTON     ) Input.NextWeaponKey     = Value;
    if( PadBits & PAD_ZOOM_BUTTON            ) Input.ZoomKey           = Value;
    if( PadBits & PAD_ZOOM_IN_BUTTON         ) Input.ZoomInKey         = Value;
    if( PadBits & PAD_ZOOM_OUT_BUTTON        ) Input.ZoomOutKey        = Value;
    if( PadBits & PAD_DROP_GRENADE_BUTTON    ) Input.GrenadeKey        = Value;
    if( PadBits & PAD_USE_PACK_BUTTON        ) Input.PackKey           = Value;
    if( PadBits & PAD_VIEW_CHANGE_BUTTON     ) Input.ViewChangeKey     = Value;
    if( PadBits & PAD_AUTOLOCK_BUTTON        ) Input.TargetLockKey     = Value;
    
    // Always enable the view change change and options buttons
    Input.ViewChangeKey  = 0;
    Input.OptionsMenuKey = 0;
    Input.FocusKey       = 0;
    
    // Always allow mine key since mines are now dropped using the grenade key
    Input.MineKey = 0;
    
    // Always allow weapon and pack dropping since they are used by the exchange key
    Input.DropWeaponKey = 0;
    Input.DropPackKey   = 0;
    
    // Always allow the special jump key so we can detect Jump being pressed in vehicle training
    Input.SpecialJumpKey = 0;
}

//=============================================================================

void campaign_logic::EnablePadButtons( u32 PadBits )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    player_object::player_input& Input = pPlayer->GetInputMask();

    if( PadBits == 0 )
    {
        Input.Set();
        return;
    }

    if( PadBits & PAD_RIGHT_STICK_UP    ) *(u32*)&Input.LookY &= ~0x1;
    if( PadBits & PAD_RIGHT_STICK_DOWN  ) *(u32*)&Input.LookY &= ~0x2;
    if( PadBits & PAD_RIGHT_STICK_LEFT  ) *(u32*)&Input.LookX &= ~0x2;
    if( PadBits & PAD_RIGHT_STICK_RIGHT ) *(u32*)&Input.LookX &= ~0x1;
                                                     
    if( PadBits & PAD_LEFT_STICK_UP     ) *(u32*)&Input.MoveY &= ~0x1;
    if( PadBits & PAD_LEFT_STICK_DOWN   ) *(u32*)&Input.MoveY &= ~0x2;
    if( PadBits & PAD_LEFT_STICK_LEFT   ) *(u32*)&Input.MoveX &= ~0x2;
    if( PadBits & PAD_LEFT_STICK_RIGHT  ) *(u32*)&Input.MoveX &= ~0x1;

    SetButtonMask( PadBits, Input, 0 );
}    

//=============================================================================

void campaign_logic::DisablePadButtons( u32 PadBits )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    player_object::player_input& Input = pPlayer->GetInputMask();

    if( PadBits == 0 )
    {
        Input.Clear();
        return;
    }

    if( PadBits & PAD_RIGHT_STICK_UP    ) *(u32*)&Input.LookY |= 0x1;
    if( PadBits & PAD_RIGHT_STICK_DOWN  ) *(u32*)&Input.LookY |= 0x2;
    if( PadBits & PAD_RIGHT_STICK_LEFT  ) *(u32*)&Input.LookX |= 0x2;
    if( PadBits & PAD_RIGHT_STICK_RIGHT ) *(u32*)&Input.LookX |= 0x1;
                                                      
    if( PadBits & PAD_LEFT_STICK_UP     ) *(u32*)&Input.MoveY |= 0x1;
    if( PadBits & PAD_LEFT_STICK_DOWN   ) *(u32*)&Input.MoveY |= 0x2;
    if( PadBits & PAD_LEFT_STICK_LEFT   ) *(u32*)&Input.MoveX |= 0x2;
    if( PadBits & PAD_LEFT_STICK_RIGHT  ) *(u32*)&Input.MoveX |= 0x1;

    SetButtonMask( PadBits, Input, 1 );
}

//=============================================================================

f32 campaign_logic::GetStickValue( u32 PadBits )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    player_object::player_input& Input = pPlayer->GetLastInput();

    f32 Value = 0;
    
    if( PadBits & ( PAD_RIGHT_STICK_UP   | PAD_RIGHT_STICK_DOWN  )) Value = Input.LookY;
    if( PadBits & ( PAD_RIGHT_STICK_LEFT | PAD_RIGHT_STICK_RIGHT )) Value = Input.LookX;
    if( PadBits & ( PAD_LEFT_STICK_UP    | PAD_LEFT_STICK_DOWN   )) Value = Input.MoveY;
    if( PadBits & ( PAD_LEFT_STICK_LEFT  | PAD_LEFT_STICK_RIGHT  )) Value = Input.MoveX;

    return( Value );
}

//=============================================================================

xbool campaign_logic::IsStickPressed( u32 PadBits, f32 Sec )
{
    xbool Flag = FALSE;
    f32 Value  = GetStickValue( PadBits );
    
    if( Value )
    {
        u32 Neg = PAD_RIGHT_STICK_DOWN | PAD_RIGHT_STICK_LEFT  | PAD_LEFT_STICK_DOWN | PAD_LEFT_STICK_LEFT;
        u32 Pos = PAD_RIGHT_STICK_UP   | PAD_RIGHT_STICK_RIGHT | PAD_LEFT_STICK_UP   | PAD_LEFT_STICK_RIGHT;
        xbool IsPressed = FALSE;

        if( PadBits & Neg )
        {
            if( Value < 0 ) IsPressed = TRUE;
        }
        
        if( PadBits & Pos )
        {
            if( Value > 0 ) IsPressed = TRUE;
        }

        if( IsPressed )
        {
            if( Sec > 0 )
            {
                if( s_PadTimer.IsRunning() == FALSE )
                    s_PadTimer.Start();
                
                if( s_PadTimer.ReadSec() >= Sec )
                {
                    Flag = TRUE;
                }
            }
            else
            {
                Flag = TRUE;
            }
        }
    }
    else
    {
        if( Sec > 0 )
            s_PadTimer.Reset();
    }

    return( Flag );
}

//=============================================================================

xbool campaign_logic::IsButtonPressed( u32 PadBits, f32 Sec )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    player_object::player_input& Input = pPlayer->GetLastInput();

    xbool IsPressed = FALSE;

    if( PadBits & PAD_JUMP_BUTTON            ) IsPressed |= Input.JumpKey;
    if( PadBits & PAD_JET_BUTTON             ) IsPressed |= Input.JetKey;
    if( PadBits & PAD_FIRE_BUTTON            ) IsPressed |= Input.FireKey;
    if( PadBits & PAD_EXCHANGE_WEAPON_BUTTON ) IsPressed |= Input.ExchangeKey;
    if( PadBits & PAD_NEXT_WEAPON_BUTTON     ) IsPressed |= Input.NextWeaponKey;
    if( PadBits & PAD_ZOOM_BUTTON            ) IsPressed |= Input.ZoomKey;
    if( PadBits & PAD_ZOOM_IN_BUTTON         ) IsPressed |= Input.ZoomInKey;
    if( PadBits & PAD_ZOOM_OUT_BUTTON        ) IsPressed |= Input.ZoomOutKey;
    if( PadBits & PAD_DROP_GRENADE_BUTTON    ) IsPressed |= Input.GrenadeKey;
    if( PadBits & PAD_USE_PACK_BUTTON        ) IsPressed |= Input.PackKey;
    if( PadBits & PAD_VIEW_CHANGE_BUTTON     ) IsPressed |= Input.ViewChangeKey;
    if( PadBits & PAD_AUTOLOCK_BUTTON        ) IsPressed |= Input.TargetLockKey;
    if( PadBits & PAD_SPECIAL_JUMP_BUTTON    ) IsPressed |= Input.SpecialJumpKey;

    xbool Flag = FALSE;

    if( IsPressed )
    {
        if( Sec > 0 )
        {
            if( s_PadTimer.IsRunning() == FALSE )
                s_PadTimer.Start();
            
            if( s_PadTimer.ReadSec() >= Sec )
            {
                Flag = TRUE;
            }
        }
        else
        {
            Flag = TRUE;
        }
    }
    else
    {
        if( Sec > 0 )
            s_PadTimer.Reset();
    }

    return( Flag );
}

//=============================================================================

player_object::invent_type campaign_logic::GetCurrentWeapon( void )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetWeaponCurrentType() );
}

//=============================================================================

s32 campaign_logic::GetNumWeaponsHeld( void )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetNumWeaponsHeld() );
}

//=============================================================================

static s32 GetNumInstances( object::type Type, player_object::invent_type InventType )
{
    grenade::type GrenadeType = grenade::TYPE_LAUNCHED;

    switch( InventType )
    {
        case player_object::INVENT_TYPE_GRENADE_BASIC      : GrenadeType = grenade::TYPE_BASIC;      break;
        case player_object::INVENT_TYPE_GRENADE_CONCUSSION : GrenadeType = grenade::TYPE_CONCUSSION; break;
        case player_object::INVENT_TYPE_GRENADE_FLASH      : GrenadeType = grenade::TYPE_FLASH;      break;
        case player_object::INVENT_TYPE_GRENADE_FLARE      : GrenadeType = grenade::TYPE_FLARE;      break;
    }

    object* pObject;
    s32     Count = 0;

    ObjMgr.StartTypeLoop( Type );

    while( (pObject = ObjMgr.GetNextInType()) )
    {
        if( Type == object::TYPE_GRENADE )
        {
            grenade* pGrenade = (grenade*)pObject;
            
            if( pGrenade->GetType() == GrenadeType )
            {
                Count++;
            }
        }
        else
        {
            if( Type == object::TYPE_TURRET_FIXED )
            {
                turret* pTurret = (turret*)pObject;
                
                // check if turret has a missile barrel
                if( pTurret->GetKind() == turret::MISSILE )
                {
                    Count++;
                }
            }
            else
            {
                Count++;
            }
        }
    }

    ObjMgr.EndTypeLoop();
    
    return( Count );
}

//=============================================================================

xbool campaign_logic::HasWeaponFired( player_object::invent_type Type )
{
    object::type Ammo = object::TYPE_DISK;
    player_object::invent_type InventType = player_object::INVENT_TYPE_NONE;

    switch( Type )
    {
        case player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER    : Ammo = object::TYPE_DISK;    break;
        case player_object::INVENT_TYPE_WEAPON_PLASMA_GUN       : Ammo = object::TYPE_PLASMA;  break;
        case player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE     : Ammo = object::TYPE_LASER;   break;
        case player_object::INVENT_TYPE_WEAPON_MORTAR_GUN       : Ammo = object::TYPE_MORTAR;  break;
        case player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER : Ammo = object::TYPE_GRENADE; break;
        case player_object::INVENT_TYPE_WEAPON_CHAIN_GUN        : Ammo = object::TYPE_BULLET;  break;
        case player_object::INVENT_TYPE_WEAPON_BLASTER          : Ammo = object::TYPE_BLASTER; break;
        case player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER : Ammo = object::TYPE_MISSILE; break;
        
        case player_object::INVENT_TYPE_MINE                    : Ammo = object::TYPE_MINE;    break;
        case player_object::INVENT_TYPE_GRENADE_BASIC           : 
        case player_object::INVENT_TYPE_GRENADE_CONCUSSION      :
        case player_object::INVENT_TYPE_GRENADE_FLASH           :
        case player_object::INVENT_TYPE_GRENADE_FLARE           :
                                                                  Ammo       = object::TYPE_GRENADE;
                                                                  InventType = Type;
                                                                  break;
        case player_object::INVENT_TYPE_WEAPON_ELF_GUN          :
            if( IsButtonPressed( PAD_FIRE_BUTTON ) == TRUE ) return( TRUE );    // TODO: detect Elf gun firing!
            break;
        
        default : DEMAND( FALSE );
    }

    if( GetNumInstances( Ammo, InventType ) > 0 )
    {
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

//=============================================================================

xbool campaign_logic::IsItemHeld( player_object::invent_type Item )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->IsItemHeld( Item ));
}

//=============================================================================

const player_object::loadout& campaign_logic::GetLoadout( void ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetLoadout() );
}

//=============================================================================

const player_object::loadout& campaign_logic::GetInventoryLoadout( void ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetInventoryLoadout() );
}

//=============================================================================

void campaign_logic::PickupTouched( object::id PickupID, object::id PlayerID )
{
    (void)PlayerID;
    
    pickup* pPickup = (pickup*)ObjMgr.GetObjectFromID( PickupID );
    DEMAND( pPickup );

    m_bPickupTouched = TRUE;
    m_PickupType     = pPickup->GetKind();
}

//=============================================================================

void campaign_logic::CreatePickup( pickup::kind PickupType, const vector3& Position, xbool IsRespawning )
{
    pickup* pPickup = (pickup*)ObjMgr.CreateObject( object::TYPE_PICKUP );
    
    s32 Flags = IsRespawning ? pickup::FLAG_RESPAWNS : 0;
    Flags    |= pickup::FLAG_IMMORTAL;
    
    pPickup->Initialize( Position + vector3( 0.0f, 0.5f, 0.0f ), vector3( 0, 0, 0 ), PickupType,
                         pickup::flags( Flags ), 1.0f );
                         
    ObjMgr.AddObject( pPickup, obj_mgr::AVAILABLE_SERVER_SLOT );
}

//=============================================================================

void campaign_logic::CreatePickup( pickup::kind PickupType, object::id ObjectID, xbool IsRespawning )
{
    object* pObject = ObjMgr.GetObjectFromID( ObjectID );
    
    if( pObject )
    {
        CreatePickup( PickupType, pObject->GetPosition(), IsRespawning );
    }
}

//=============================================================================

void campaign_logic::SetHealth( f32 Health )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    pPlayer->SetHealth( Health );
}

//=============================================================================

void campaign_logic::SetInventCount( player_object::invent_type InventType, s32 Count )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    pPlayer->SetInventCount( InventType, Count );
}

//=============================================================================

s32 campaign_logic::GetInventCount( player_object::invent_type InventType )
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetInventCount( InventType ));
}

//=============================================================================

void campaign_logic::CreateBeacon( object::id ObjectID )
{
    object* pObject = ObjMgr.GetObjectFromID( ObjectID );
    
    if( pObject )
    {
        vector3 S = pObject->GetPosition();
        vector3 E = S + vector3( 0, -2.0f, 0 );

        collider Collider;
        Collider.RaySetup( NULL, S, E );
        ObjMgr.Collide( object::ATTR_SOLID_STATIC, Collider );
        ASSERT( Collider.HasCollided() );

        collider::collision Collision;
        Collider.GetCollision( Collision );
    
        beacon* pBeacon = (beacon*)ObjMgr.CreateObject( object::TYPE_BEACON );
        pBeacon->Initialize( Collision.Point, Collision.Plane.Normal, ALLIED_TEAM_BITS );
        ObjMgr.AddObject( pBeacon, obj_mgr::AVAILABLE_SERVER_SLOT );
    }
}

//=============================================================================

void campaign_logic::RemoveBeacon( object::id ObjectID )
{
    object* pObject = ObjMgr.GetObjectFromID( ObjectID );
    
    if( pObject )
    {
        s32 Count = 0;
        beacon* pBeacon;
        
        ObjMgr.Select( object::ATTR_BEACON, pObject->GetPosition(), 2.0f );
        while( (pBeacon = (beacon*)ObjMgr.GetNextSelected()) )
        {
            ObjMgr.RemoveObject ( pBeacon );
            ObjMgr.DestroyObject( pBeacon );
            Count++;
        }
        ObjMgr.ClearSelection();

        ASSERT( Count <= 1 );
    }
}

//=============================================================================

xbool campaign_logic::IsAttachedToVehicle( object::type VehicleType ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    if( VehicleType == object::TYPE_NULL )
    {
        return( pPlayer->IsAttachedToVehicle() != NULL );
    }
    else
    {
        // Check if player is in specific type of vehicle
        
        vehicle* pVehicle = pPlayer->IsAttachedToVehicle();

        xbool Result = FALSE;
        
        if( pVehicle )
        {
            if( pVehicle->GetType() == VehicleType )
            {
                Result = TRUE;
            }
        }
        
        return( Result );
    }
}

//=============================================================================

xbool campaign_logic::IsItemDeployed( object::type Type ) const
{
    s32 Count = GetNumInstances( Type, player_object::INVENT_TYPE_NONE );
    
    return( Count > 0 );
}

//=============================================================================

s32 campaign_logic::GetVehicleSeat( void ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetVehicleSeat() );
}

//=============================================================================

xbool campaign_logic::IsPlayerShielded( void ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->IsShielded() );
}

//=============================================================================

xbool campaign_logic::IsPlayerArmed( void ) const
{
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    return( pPlayer->GetArmed() );
}

//=============================================================================

xbool campaign_logic::IsVehiclePresent( object::type VehicleType ) const
{
    s32 NumVehicles = 0;
    vehicle* pVehicle;

    ObjMgr.StartTypeLoop( VehicleType );
    
    while( (pVehicle = (vehicle*)ObjMgr.GetNextInType()) )
    {
        NumVehicles++;
    }
    
    ObjMgr.EndTypeLoop();

    ASSERT( NumVehicles < 2 );
    return( NumVehicles > 0 );
}

//=============================================================================

void campaign_logic::SetVehicleLimits( s32 MaxCycles, s32 MaxShrikes, s32 MaxBombers, s32 MaxTransports )
{
    GameMgr.SetVehicleLimits( MaxCycles, MaxShrikes, MaxBombers, MaxTransports );
}

//=============================================================================

void campaign_logic::Render( void )
{
    eng_Begin( "Campaign" );

    if( DebugVehiclePoints )
        RenderVehiclePoints();
    
    eng_End();
}

//=============================================================================

