//=============================================================================
//
//  Training Mission 5 Logic
//
//=============================================================================

#include "ct5_logic.hpp"
#include "GameMgr.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "..\..\Demo1\dlg_Vehicle.hpp"
#include "..\..\Demo1\data\ui\ui_strings.h"

#include "Objects\Vehicles\GravCycle.hpp"
#include "Objects\Vehicles\Shrike.hpp"
#include "Objects\Vehicles\Bomber.hpp"
#include "Objects\Vehicles\Transport.hpp"
#include "Objects\Vehicles\AssTank.hpp"
#include "Objects\Vehicles\MPB.hpp"

#include "..\Demo1\fe_Globals.hpp"

//=============================================================================

static s32 s_VehiclePadBits = PAD_RIGHT_STICK | PAD_LEFT_STICK     | PAD_JET_BUTTON      |
                              PAD_ZOOM_BUTTON | PAD_ZOOM_IN_BUTTON | PAD_ZOOM_OUT_BUTTON |
                              PAD_FIRE_BUTTON | PAD_DROP_GRENADE_BUTTON;

enum
{
    GET_VEHICLE,
    FLY_VEHICLE,
    EXIT_VEHICLE,
};

static s32 s_FirstState = GET_VEHICLE;

//=============================================================================
//  VARIABLES
//=============================================================================

ct5_logic CT5_Logic;

//=============================================================================

s32 veh = 0;

extern s32 VEHICLE_TEST;
extern s32 BYPASS;

void ct5_logic::BeginGame( void )
{
    ASSERT( m_Pad1   != obj_mgr::NullID );
    ASSERT( m_Pad2   != obj_mgr::NullID );
    ASSERT( m_Pad3   != obj_mgr::NullID );
    ASSERT( m_Pad4   != obj_mgr::NullID );
    ASSERT( m_Waypt1 != obj_mgr::NullID );
    ASSERT( m_Waypt2 != obj_mgr::NullID );
    ASSERT( m_Waypt3 != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_INTRO );
    
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    PowerLoss( m_Pad1 );
    PowerLoss( m_Pad2 );
    PowerLoss( m_Pad3 );
    PowerLoss( m_Pad4 );

    fegl.PlayerInvulnerable = TRUE;
    m_bInMenu = FALSE;

    // Give the player some flares to play with
    SetInventCount( player_object::INVENT_TYPE_GRENADE_BASIC,      0 );
    SetInventCount( player_object::INVENT_TYPE_GRENADE_CONCUSSION, 0 );
    SetInventCount( player_object::INVENT_TYPE_GRENADE_FLASH,      0 );
    SetInventCount( player_object::INVENT_TYPE_GRENADE_FLARE,      5 );
    SetInventCount( player_object::INVENT_TYPE_MINE,               0 );

    // Default to grav cycle
    SetVehicleLimits( 1, 0, 0, 0 );

    if( VEHICLE_TEST & BYPASS )
    {
        switch( veh )
        {
            case 0 :
            {
                gravcycle* pGravCycle = (gravcycle*)ObjMgr.CreateObject( object::TYPE_VEHICLE_WILDCAT );
                pGravCycle->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pGravCycle, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }

            case 1 :
            {
                shrike* pShrike = (shrike*)ObjMgr.CreateObject( object::TYPE_VEHICLE_SHRIKE );
                pShrike->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pShrike, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }
    
            case 2 :
            {
                bomber* pBomber = (bomber*)ObjMgr.CreateObject( object::TYPE_VEHICLE_THUNDERSWORD );
                pBomber->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pBomber, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }

            case 3 :
            {
                transport* pTransport = (transport*)ObjMgr.CreateObject( object::TYPE_VEHICLE_HAVOC );
                pTransport->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pTransport, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }

            case 4 :
            {
                asstank* pAssTank = (asstank*)ObjMgr.CreateObject( object::TYPE_VEHICLE_BEOWULF );
                pAssTank->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pAssTank, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }

            case 5 :
            {
                mpb* pMPB = (mpb*)ObjMgr.CreateObject( object::TYPE_VEHICLE_JERICHO2 );
                pMPB->Initialize( vector3( 215.0f, 210.0f, 548.0f ), 0, 0x01 );
                ObjMgr.AddObject( pMPB, obj_mgr::AVAILABLE_SERVER_SLOT );
                break;
            }
        }
    }
}

//=============================================================================

void ct5_logic::AdvanceTime( f32 DeltaTime )
{
    if( VEHICLE_TEST && BYPASS )
        return;

    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE      : return;
        case STATE_INTRO     : Intro();     break;
        case STATE_GRAVCYCLE : GravCycle(); break;
        case STATE_TRANSPORT : Transport(); break;
        case STATE_BOMBER    : Bomber();    break;
        case STATE_SCOUT     : Scout();     break;
        default              :              break;
    }

    m_Time += DeltaTime;
}

//=============================================================================

void ct5_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "GRAV"      )) m_Pad1   = ItemID;
    if( !x_strcmp( pTag, "TRANSPORT" )) m_Pad2   = ItemID;
    if( !x_strcmp( pTag, "BOMBER"    )) m_Pad3   = ItemID;
    if( !x_strcmp( pTag, "SCOUT"     )) m_Pad4   = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"    )) m_Waypt1 = ItemID;
    if( !x_strcmp( pTag, "WAYPT2"    )) m_Waypt2 = ItemID;
    if( !x_strcmp( pTag, "WAYPT3"    )) m_Waypt3 = ItemID;
}

//=============================================================================

void ct5_logic::StationUsed( object::id PlayerID )
{
    (void)PlayerID;
    m_bStationUsed = TRUE;
}

//=============================================================================

void ct5_logic::AdvanceStates( void )
{
    if( m_State != m_NewState )
    {
        if( audio_IsPlaying( m_ChannelID ) == FALSE )
        {
            // Switch to the new state
            m_State      = m_NewState;
            m_bInitState = FALSE;
        }
        else
        {
            // Switch the current state to idle until audio is complete
            m_State = -1;
        }
    }
    else
    {
        m_bInitState = TRUE;
    }
}

//=============================================================================

void ct5_logic::NextState( void )
{
    m_NewState++;
    ClearText();
}

//=============================================================================

xbool ct5_logic::Mission( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_bStationUsed = FALSE;
        m_bPadPower    = FALSE;
        m_bHint        = FALSE;
        m_NewState     = s_FirstState;
        m_State        = -1;
    }

    AdvanceStates();

    //
    // Get Vehicle
    //
    
    if( m_State == GET_VEHICLE )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( m_Audio1 );
            m_bObjective = FALSE;
            m_bGetIn     = FALSE;
            AddWaypoint( m_CurrPad );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( m_Obj1 );
            }

            if( m_bObjective )
            {
                // Prompt player to get into the vehicle
                if( !m_bGetIn && IsVehiclePresent( m_Vehicle ))
                {
                    m_bGetIn = TRUE;
                    if( m_GameState == STATE_GRAVCYCLE )
                        GameAudio( T5_GET_IN_VEHICLE );
                    PrintObjective( TEXT_T5_GET_IN_VEHICLE );
                }
            
                // Wait until player is in vehicle

                if( IsAttachedToVehicle( m_Vehicle ) )
                {
                    if( GetVehicleSeat() == 0 )
                    {
                        // Turn off all controls until vehicle info is finished
                        EnablePadButtons( 0 );

                        NextState();
                        ClearErrorText();
                    }
                    else
                    {
                        if( m_bHint == FALSE )
                        {
                            m_bHint  = TRUE;
                            //GameAudio( T5_WRONG_SEAT );
                        }
                        ErrorText( TEXT_T5_WRONG_SEAT );
                    }
                }
                else
                {
                    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
                    
                    if( tgl.pUIManager->GetNumUserDialogs( pPlayer->GetUserID() ) > 0 )
                    {
                        m_bInMenu = TRUE;
                    }
                    else
                    {
                        // TODO: player chooses correct vehicle BEFORE this state - eg. in transit to next waypoint
                    
                        if( m_bInMenu )
                        {
                            m_bInMenu = FALSE;
                    
                            // Only power up the vehicle pad when the correct vehicle is selected and the audio prompt has finished
                            
                            const player_object::loadout& Loadout = GetInventoryLoadout();
                            
                            if( Loadout.VehicleType == m_Vehicle )
                            {
                                if( m_bPadPower == FALSE )
                                {
                                    m_bPadPower = TRUE;
                                    PowerGain( m_CurrPad );
                                }
                            
                                ClearErrorText(); // commented out: if another vehicle was on pad - it faded that error
                            }
                            else
                            {
                                if( m_bPadPower == TRUE )
                                {
                                    m_bPadPower = FALSE;
                                    PowerLoss( m_CurrPad );
                                }
                            
                                ErrorText( TEXT_T5_WRONG_VEHICLE );
                            }
                        }
                    }
                }
            }
        }
    }

    //
    // Fly Vehicle
    //
    
    if( m_State == FLY_VEHICLE )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( m_Audio2 );
            
            m_bObjective = FALSE;
            m_bHint      = FALSE;

            // Set destination waypoint
            ClearWaypoint( m_CurrPad );
            AddWaypoint  ( m_NextPad );
            
            // The grav cycle training has 2 extra waypoints for guidance
            if( m_CurrPad == m_Pad1 )
            {
                AddWaypoint( m_Waypt1 );
                AddWaypoint( m_Waypt2 );
            }

            // Power down this vehicle pad so it cant be used again
            if( m_bPadPower == TRUE )
            {
                m_bPadPower = FALSE;
                PowerLoss( m_CurrPad );
            }
            
            // Turn off all controls until vehicle info is finished
            EnablePadButtons( 0 );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( m_Obj2 );
                EnablePadButtons( s_VehiclePadBits );
            }

            if( m_bObjective )
            {
                if( IsAttachedToVehicle() )
                {
                    xbool CheckButton = TRUE;
                
                    if( DistanceToObject( m_NextPad ) < 150.0f )
                    {
                        // Special case: mission completes when last vehicle is within range of final waypoint
                        if( m_NextPad == m_Pad1 )
                            return( TRUE );
                    
                        if( m_bHint == FALSE )
                        {
                            m_bHint  = TRUE;
                            GameAudio( T5_JUMP_OUT );
                        }
                        
                        if( audio_IsPlaying( m_ChannelID ) == FALSE )
                            CheckButton = FALSE;
                    }
                    
                    if( m_bHint == TRUE )
                    {
                        // wait for audio to finish up before allowing the player to eject
                        if( audio_IsPlaying( m_ChannelID ) == FALSE )
                        {
                            PrintObjective( TEXT_T5_JUMP_OUT );
                            
                            // Player has flown to the waypoint so allow him to jump out of vehicle
                            EnablePadButtons( PAD_JUMP_BUTTON );
                        }
                    }
                
                    if( CheckButton && IsButtonPressed( PAD_SPECIAL_JUMP_BUTTON ) )
                        ErrorText( TEXT_T5_JUMP_DISABLED );
                    else
                        ClearErrorText();
                }
                else
                {
                    // Player has jumped out of vehicle or the vehicle has been destroyed
                    DisablePadButtons( 0 );
                    NextState();
                    ClearErrorText();
                }
            }
        }
    }
    
    //
    // Exit Vehicle
    //
    
    if( m_State == EXIT_VEHICLE )
    {
        ClearWaypoint( m_NextPad );
        
        if( m_CurrPad == m_Pad1 )
        {
            ClearWaypoint( m_Waypt1 );
            ClearWaypoint( m_Waypt2 );
        }
        
        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}

//=============================================================================

void ct5_logic::Intro( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        GameAudio( T5_INTRO );
    }
    
    if( audio_IsPlaying( m_ChannelID ) == FALSE )
    {
        SetState( STATE_GRAVCYCLE );
    }
}

//=============================================================================

void ct5_logic::GravCycle( void )
{
    m_Audio1  = T5_GET_GRAV_CYCLE;
    m_Audio2  = T5_INFO_GRAV_CYCLE;
    m_Obj1    = TEXT_T5_GET_GRAV_CYCLE;
    m_Obj2    = TEXT_T5_INFO_GRAV_CYCLE;

    m_CurrPad = m_Pad1;
    m_NextPad = m_Pad2;
    
    m_Vehicle = object::TYPE_VEHICLE_WILDCAT;

    //SetVehicleUI( IDS_WILDCAT );
    SetVehicleLimits( 1, 0, 0, 0 );
    
    if( Mission() == TRUE )
    {
        SetState( STATE_TRANSPORT );
    }
}

//=============================================================================

void ct5_logic::Transport( void )
{
    m_Audio1  = T5_GET_TRANSPORT;
    m_Audio2  = T5_INFO_TRANSPORT;
    m_Obj1    = TEXT_T5_GET_TRANSPORT;
    m_Obj2    = TEXT_T5_INFO_TRANSPORT;

    m_CurrPad = m_Pad2;
    m_NextPad = m_Pad3;

    m_Vehicle = object::TYPE_VEHICLE_HAVOC;

    //SetVehicleUI( IDS_HAVOC );
    SetVehicleLimits( 0, 0, 0, 1 );
    
    if( Mission() == TRUE )
    {
        SetState( STATE_BOMBER );
    }
}

//=============================================================================

void ct5_logic::Bomber( void )
{
    m_Audio1  = T5_GET_BOMBER;
    m_Audio2  = T5_INFO_BOMBER;
    m_Obj1    = TEXT_T5_GET_BOMBER;
    m_Obj2    = TEXT_T5_INFO_BOMBER;

    m_CurrPad = m_Pad3;
    m_NextPad = m_Pad4;

    m_Vehicle = object::TYPE_VEHICLE_THUNDERSWORD;

    //SetVehicleUI( IDS_THUNDERSWORD );
    SetVehicleLimits( 0, 0, 1, 0 );
    
    if( Mission() == TRUE )
    {
        SetState( STATE_SCOUT );
    }
}

//=============================================================================

void ct5_logic::Scout( void )
{
    m_Audio1  = T5_GET_SCOUT;
    m_Audio2  = T5_INFO_SCOUT;
    m_Obj1    = TEXT_T5_GET_SCOUT; 
    m_Obj2    = TEXT_T5_INFO_SCOUT;

    m_CurrPad = m_Pad4;
    m_NextPad = m_Pad1;

    m_Vehicle = object::TYPE_VEHICLE_SHRIKE;

    //SetVehicleUI( IDS_SHRIKE );
    SetVehicleLimits( 0, 1, 0, 0 );
    
    if( Mission() == TRUE )
    {
        Success( T5_COMPLETE );
    }
}

//=============================================================================

void ct5_logic::SetVehicleUI( s32 StringID )
{
    static s32 StringIDS[]=
    {
        IDS_WILDCAT, IDS_BEOWULF, IDS_JERICHO, IDS_SHRIKE, IDS_THUNDERSWORD, IDS_HAVOC
    };

    s32 NumVehicles = sizeof( StringIDS ) / sizeof( StringIDS[0] );
    
    for( s32 i=0; i<NumVehicles; i++ )
    {
        SetVehicleButtonState( StringIDS[i], FALSE );
    }
    
    SetVehicleButtonState( StringID, TRUE );
}

//==============================================================================

void ct5_logic::EnforceBounds( f32 DeltaTime )
{   
    (void)DeltaTime;

    // Attempt to keep the player in bounds.

    bbox Bounds = GameMgr.GetBounds();
    Bounds.Min.X += 5.0f;
    Bounds.Min.Z += 5.0f;
    Bounds.Max.X -= 5.0f;
    Bounds.Max.Z -= 5.0f;

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromSlot( 0 );

    if( (pPlayer) && !(pPlayer->IsAttachedToVehicle()) )
    {
        xbool   Dirty    = FALSE;
        vector3 Position = pPlayer->GetPosition();
        vector3 Velocity = pPlayer->GetVelocity();

        if( Position.X < Bounds.Min.X )
        {
            Velocity.X = 15.0f;
            Dirty      = TRUE;
        }

        if( Position.X > Bounds.Max.X )
        {
            Velocity.X = -15.0f;
            Dirty      = TRUE;
        }

        if( Position.Z < Bounds.Min.Z )
        {
            Velocity.Z = 15.0f;
            Dirty      = TRUE;
        }

        if( Position.Z > Bounds.Max.Z )
        {
            Velocity.Z = -15.0f;
            Dirty      = TRUE;
        }

        if( Dirty )
        {
            Velocity.Y = 15.0f;
            pPlayer->SetVel( Velocity );
        }
    }
    
    // Attempt to keep vehicles in bounds.

    s32 Type;

    for( Type = object::TYPE_VEHICLE_START; Type <= object::TYPE_VEHICLE_END; Type++ )
    {
        vehicle* pVehicle;
        ObjMgr.StartTypeLoop( (object::type)Type );
        while( (pVehicle = (vehicle*)ObjMgr.GetNextInType()) )
        {
            xbool   Dirty    = FALSE;
            vector3 Position = pVehicle->GetPosition();
            vector3 Velocity = pVehicle->GetVelocity();

            if( Position.X < Bounds.Min.X )
            {
                Velocity.X = 15.0f;
                Dirty      = TRUE;
            }

            if( Position.X > Bounds.Max.X )
            {
                Velocity.X = -15.0f;
                Dirty      = TRUE;
            }

            if( Position.Z < Bounds.Min.Z )
            {
                Velocity.Z = 15.0f;
                Dirty      = TRUE;
            }

            if( Position.Z > Bounds.Max.Z )
            {
                Velocity.Z = -15.0f;
                Dirty      = TRUE;
            }

            if( Dirty )
            {
                Velocity.Y = 15.0f;
                pVehicle->SetVel( Velocity );
            }
        }
        ObjMgr.EndTypeLoop();
    }
}

//=============================================================================


