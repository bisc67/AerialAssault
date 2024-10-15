//=============================================================================
//
//  Training Mission 1 Logic
//
//=============================================================================

#include "ct1_logic.hpp"
#include "GameMgr.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "..\Demo1\fe_Globals.hpp"

//=============================================================================

enum
{
    INTRO,
    HEALTH_METER,
    TEXT1,
    SCOREBOARD,
    INVENTORY,
    RETICLE,
    MISSILE_LOCK,
    COMPLETE,
};

enum
{
    LOOK_UP,
    LOOK_DOWN,
    LOOK_LEFTRIGHT,
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_SIDEWAYS,
    JUMP,
    JETPACK,
    TOUCH_SWITCHES,
    SUCCESS,
};

//=============================================================================
//  VARIABLES
//=============================================================================

ct1_logic CT1_Logic;

//=============================================================================

void ct1_logic::BeginGame( void )
{
    ASSERT( m_Switch1   != obj_mgr::NullID );
    ASSERT( m_Switch2   != obj_mgr::NullID );
    ASSERT( m_Switch3   != obj_mgr::NullID );
    ASSERT( m_Switch4   != obj_mgr::NullID );
    ASSERT( m_Switch5   != obj_mgr::NullID );
    ASSERT( m_Generator != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MOVEMENT );

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    fegl.PlayerInvulnerable = TRUE;
}

//=============================================================================

void ct1_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE     : return;
        case STATE_HUD      : HUDTraining(); break;
        case STATE_MOVEMENT : Movement();    break;
        default             :                break;
    }

    m_Time += DeltaTime;
}

//=============================================================================

void ct1_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "SWITCH1" )) m_Switch1   = ItemID;
    if( !x_strcmp( pTag, "SWITCH2" )) m_Switch2   = ItemID;
    if( !x_strcmp( pTag, "SWITCH3" )) m_Switch3   = ItemID;
    if( !x_strcmp( pTag, "SWITCH4" )) m_Switch4   = ItemID;
    if( !x_strcmp( pTag, "SWITCH5" )) m_Switch5   = ItemID;
    if( !x_strcmp( pTag, "GEN1"    )) m_Generator = ItemID;
}
//=============================================================================

void ct1_logic::AdvanceStates( void )
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

void ct1_logic::NextState( void )
{
    m_NewState++;
    ClearText();
}

//=============================================================================

void ct1_logic::Training( s32 AudioID, hud_glow Glow )
{
    if( m_bInitState == FALSE )
    {
        GameAudio( AudioID );
        tgl.pHUDManager->SetHUDGlow( Glow );
    }
    else
    {
        if( audio_IsPlaying( m_ChannelID ) == FALSE )
        {
            NextState();
        }
    }
}

//=============================================================================

void ct1_logic::HUDTraining( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_NewState     = 0;
        m_State        = -1;
    }

    AdvanceStates();

    if( m_State == INTRO )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T1_INTRO );
        }
        else
        {
            if( audio_IsPlaying( m_ChannelID ) == FALSE )
            {
                NextState();
            }
        }
    }
    
    if( m_State == HEALTH_METER )
    {
        Training( T1_HEALTH_METER, HUDGLOW_HEALTHMETER );
    }

    if( m_State == TEXT1 )
    {
        Training( T1_HUD_TEXT1, HUDGLOW_TEXT1 );
    }
    
    if( m_State == SCOREBOARD )
    {
        Training( T1_SCOREBOARD, HUDGLOW_SCOREBOARD );
    }

    if( m_State == INVENTORY )
    {
        Training( T1_INVENTORY, HUDGLOW_STATBAR );
    }
    
    if( m_State == RETICLE )
    {
        Training( T1_RETICLE, HUDGLOW_RETICLE );
    }
    
    if( m_State == MISSILE_LOCK )
    {
        player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
        ASSERT( pPlayer );
    
        if( m_bInitState == FALSE )
        {
            GameAudio( T1_MISSILE_LOCK );
            tgl.pHUDManager->SetHUDGlow( HUDGLOW_NONE );
        }
        else
        {
            pPlayer->MissileLock();
            if( audio_IsPlaying( m_ChannelID ) == FALSE )
            {
                NextState();
            }
        }
    }
    
    if( m_State == COMPLETE )
    {
        Success( T1_SUCCESS );
    }
}

//=============================================================================

void ct1_logic::Movement( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_NewState     = 0;
        m_State        = -1;
        
        EnablePadButtons( 0 );
    }

    AdvanceStates();

    //
    // Look Up
    //

    if( m_State == LOOK_UP )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_LOOK_UP );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_LOOK_UP );
                EnablePadButtons( PAD_RIGHT_STICK_DOWN );
            }
            
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_RIGHT_STICK_DOWN, StickHoldTime ) )
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // Look Down
    //

    if( m_State == LOOK_DOWN )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_LOOK_DOWN );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_LOOK_DOWN );
                EnablePadButtons( PAD_RIGHT_STICK_UP );
            }
            
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_RIGHT_STICK_UP, StickHoldTime ) )
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // Look Left and Right
    //
    
    if( m_State == LOOK_LEFTRIGHT )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            m_bLookLeft  = FALSE;
            m_bLookRight = FALSE;
            GameAudio( T1_LOOK_LEFTRIGHT );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_LOOK_LEFTRIGHT );
                EnablePadButtons( PAD_RIGHT_STICK_LEFT | PAD_RIGHT_STICK_RIGHT );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_RIGHT_STICK_LEFT,  StickHoldTime )) m_bLookLeft  = TRUE;
                if( IsStickPressed( PAD_RIGHT_STICK_RIGHT, StickHoldTime )) m_bLookRight = TRUE;
            
                if( m_bLookLeft && m_bLookRight )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Move Forward
    //
    
    if( m_State == MOVE_FORWARD )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_MOVE_FORWARD );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_MOVE_FORWARD );
                EnablePadButtons( PAD_LEFT_STICK_UP );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_LEFT_STICK_UP, StickHoldTime ))
                {
                    NextState();
                }
            }
        }
    }

    //
    // Move Back
    //

    if( m_State == MOVE_BACK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_MOVE_BACK );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_MOVE_BACK );
                EnablePadButtons( PAD_LEFT_STICK_DOWN );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_LEFT_STICK_DOWN, StickHoldTime ))
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // Move Left and Right
    //
    
    if( m_State == MOVE_SIDEWAYS )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            m_bMoveLeft  = FALSE;
            m_bMoveRight = FALSE;
            GameAudio( T1_MOVE_SIDEWAYS );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_MOVE_SIDEWAYS );
                EnablePadButtons( PAD_LEFT_STICK_LEFT | PAD_LEFT_STICK_RIGHT );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsStickPressed( PAD_LEFT_STICK_LEFT,  StickHoldTime )) m_bMoveLeft  = TRUE;
                if( IsStickPressed( PAD_LEFT_STICK_RIGHT, StickHoldTime )) m_bMoveRight = TRUE;
            
                if( m_bMoveLeft && m_bMoveRight )
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // Jump
    //
    
    if( m_State == JUMP )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_JUMP );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_JUMP );
                EnablePadButtons( PAD_JUMP_BUTTON );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsButtonPressed( PAD_JUMP_BUTTON ))
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // JetPack
    //
    
    if( m_State == JETPACK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T1_JETPACK );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_JETPACK );
                EnablePadButtons( PAD_JET_BUTTON );
            }
        
            if( m_bObjective == TRUE )
            {
                if( IsButtonPressed( PAD_JET_BUTTON, 0.5f ))
                {
                    NextState();
                }
            }
        }
    }
    
    //
    // Touch 5 switches
    //
    
    if( m_State == TOUCH_SWITCHES )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            m_nSwitches  = 0;

            AddWaypoint( m_Switch1 );
            AddWaypoint( m_Switch2 );
            AddWaypoint( m_Switch3 );
            AddWaypoint( m_Switch4 );
            AddWaypoint( m_Switch5 );
    
            GameAudio( T1_TOUCH_SWITCHES );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T1_TOUCH_SWITCHES );
                PowerLoss( m_Generator );
            }
            
            if( m_bSwitchTouched )
            {
                m_bSwitchTouched = FALSE;
                m_nSwitches++;
                ClearWaypoint( m_SwitchID );
                PrintObjective( TEXT_T1_FOUR_TO_GO + m_nSwitches - 1 );
            }

            if( m_bObjective == TRUE )
            {
                if( m_nSwitches == 5 )
                {
                    ClearText();
                    SetState( STATE_HUD );
                }
            }
        }
    }
}

//==============================================================================

void ct1_logic::EnforceBounds( f32 DeltaTime )
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
}

//=============================================================================

