//=============================================================================
//
//  Training Mission 2 Logic
//
//=============================================================================

#include "ct2_logic.hpp"
#include "GameMgr.hpp"
#include "Objects\Pickups\Pickups.hpp"
#include "Objects\Projectiles\WayPoint.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "..\Demo1\fe_Globals.hpp"

//=============================================================================

enum
{
    INTRO,
    EQUIP_WEAPON,
    FIRE_WEAPON,
    CYCLE_DELAY,
    CYCLE_WEAPON,
    EXCHANGE_DELAY,
    EXCHANGE_WEAPON,
};

enum
{
    SNIPER_DELAY,
    GOTO_SNIPER_RANGE,
    ZOOM_MODE,
    USE_SNIPER_RIFLE,
    KILL_BOT,
};

enum
{
    AUTOLOCK,
    LOCK_ON,
    KILL_BOT_AUTOLOCK,
};

static s32 s_FirstState = 0;

f32 WeaponAudioDelay = 4.0f;
f32 SniperAudioDelay = 4.0f;

//=============================================================================
//  VARIABLES
//=============================================================================

ct2_logic CT2_Logic;

//=============================================================================

void ct2_logic::BeginGame( void )
{
    ASSERT( m_SniperGen       != obj_mgr::NullID );
    ASSERT( m_BotGen          != obj_mgr::NullID );
    ASSERT( m_DiskLauncher    != obj_mgr::NullID );
    ASSERT( m_Plasma          != obj_mgr::NullID );
    ASSERT( m_GrenadeLauncher != obj_mgr::NullID );
    ASSERT( m_ChainGun        != obj_mgr::NullID );
    ASSERT( m_Blaster         != obj_mgr::NullID );
    ASSERT( m_Sniper          != obj_mgr::NullID );
    ASSERT( m_Waypoint1       != obj_mgr::NullID );
    ASSERT( m_Waypoint2       != obj_mgr::NullID );
    ASSERT( m_FFGen           != obj_mgr::NullID );
    ASSERT( m_BotPt           != obj_mgr::NullID );
    ASSERT( m_ChuteGen        != obj_mgr::NullID );
    ASSERT( m_WeaponPt        != obj_mgr::NullID );
    ASSERT( m_AmmoPt          != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_WEAPONS );

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );
    
    fegl.PlayerInvulnerable = TRUE;

    // Change pickup flags so they dont respawn
    
    ObjMgr.StartTypeLoop( object::TYPE_PICKUP );

    pickup* pPickup;
    while( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
    {
        pPickup->SetFlags( pickup::FLAG_IMMORTAL );
    }
    
    ObjMgr.EndTypeLoop();
}

//=============================================================================

void ct2_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE     : return;
        case STATE_WEAPONS  : Weapons();    break;
        case STATE_SNIPER   : Sniper();     break;
        case STATE_AUTOLOCK : AutoLock();   break;
        default             :               break;
    }

    m_Time += DeltaTime;
}

//=============================================================================

void ct2_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "GENSNIPER"       )) m_SniperGen       = ItemID;
    if( !x_strcmp( pTag, "BOTGEN"          )) m_BotGen          = ItemID;
    if( !x_strcmp( pTag, "SPINFUSOR"       )) m_DiskLauncher    = ItemID;
    if( !x_strcmp( pTag, "PLASMARIFLE"     )) m_Plasma          = ItemID;
    if( !x_strcmp( pTag, "GRENADELAUNCHER" )) m_GrenadeLauncher = ItemID;
    if( !x_strcmp( pTag, "CHAINGUN"        )) m_ChainGun        = ItemID;
    if( !x_strcmp( pTag, "BLASTER"         )) m_Blaster         = ItemID;
    if( !x_strcmp( pTag, "SNIPER"          )) m_Sniper          = ItemID;
    if( !x_strcmp( pTag, "WAYPT1"          )) m_Waypoint1       = ItemID;
    if( !x_strcmp( pTag, "WAYPT2"          )) m_Waypoint2       = ItemID;
    if( !x_strcmp( pTag, "FFGEN"           )) m_FFGen           = ItemID; 
    if( !x_strcmp( pTag, "BOTPT"           )) m_BotPt           = ItemID; 
    if( !x_strcmp( pTag, "CHUTEGEN"        )) m_ChuteGen        = ItemID; 
    if( !x_strcmp( pTag, "WEAPONPT"        )) m_WeaponPt        = ItemID;
    if( !x_strcmp( pTag, "AMMOPT"          )) m_AmmoPt          = ItemID;
}

//=============================================================================

void ct2_logic::WeaponFired( object::id PlayerID )
{
    (void)PlayerID;
    m_bWeaponFired = TRUE;
}

//=============================================================================

void ct2_logic::WeaponExchanged( object::id PlayerID )
{
    (void)PlayerID;
    m_bWeaponExchanged = TRUE;
}

//=============================================================================

void ct2_logic::ItemAutoLocked( object::id ItemID, object::id PlayerID )
{
    (void)PlayerID;
    
    if( ItemID == m_Bot )
    {
        m_bAutoLocked = TRUE;
    }
}

//=============================================================================

void ct2_logic::PickupTouched( object::id PickupID, object::id PlayerID )
{
    (void)PlayerID;
    
    pickup* pPickup = (pickup*)ObjMgr.GetObjectFromID( PickupID );
    
    if( pPickup )
    {
        if( pPickup->GetKind() == pickup::KIND_AMMO_BOX )
            m_bAmmoTaken = TRUE;
    }
}

//=============================================================================

void ct2_logic::AdvanceStates( void )
{
    if( m_State != m_NewState )
    {
        if( audio_IsPlaying( m_ChannelID ) == FALSE )
        {
            // Switch to the new state
            m_State      = m_NewState;
            m_bInitState = FALSE;
            m_Time       = 0.0f;
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

void ct2_logic::NextState( xbool DoClearText )
{
    m_NewState++;
    
    if( DoClearText )
        ClearText();
}

//=============================================================================

void ct2_logic::SetNextState( s32 State )
{
    m_NewState = State;
    ClearText();
}

//=============================================================================

void ct2_logic::Weapons( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized     = TRUE;
        m_bDiskLauncher    = FALSE;
        m_bPlasma          = FALSE;
        m_bGrenadeLauncher = FALSE;
        m_bChainGun        = FALSE;
        m_bBlaster         = FALSE;
        m_bWeaponFired     = FALSE;
        m_bCycleWeapon     = FALSE;
        m_bExchangeWeapon  = FALSE;
        m_LastWeapon       = player_object::INVENT_TYPE_NONE;
        m_NewState         = s_FirstState;
        m_State            = -1;
        m_NumWeaponsUsed   = 0;

        AddWaypoint( m_DiskLauncher    );
        AddWaypoint( m_Plasma          );
        AddWaypoint( m_GrenadeLauncher );
        AddWaypoint( m_ChainGun        );
        AddWaypoint( m_Blaster         );
    }

    AdvanceStates();

    //
    // Audio Delay
    //
    
    if( (m_State == CYCLE_DELAY) || (m_State == EXCHANGE_DELAY) )
    {
        if( m_Time > WeaponAudioDelay )
            NextState();
    }

    //
    // Wait for introduction speech to finish up
    //
    
    if( m_State == INTRO )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            EnablePadButtons( 0 );
            EnablePadButtons( PAD_RIGHT_STICK | PAD_LEFT_STICK | PAD_JUMP_BUTTON | PAD_JET_BUTTON );
            GameAudio( T2_INTRO );
        }
        else    
        {
            if( audio_IsPlaying( m_ChannelID ) == FALSE )
            {
                NextState();
            }
        }
    }

    //
    // Remove waypoint on weapon if its picked up
    //
    
    if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER    ) ) ClearWaypoint( m_DiskLauncher    );
    if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_PLASMA_GUN       ) ) ClearWaypoint( m_Plasma          );
    if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER ) ) ClearWaypoint( m_GrenadeLauncher );
    if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_CHAIN_GUN        ) ) ClearWaypoint( m_ChainGun        );
    if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_BLASTER          ) ) ClearWaypoint( m_Blaster         );
    
    //
    // Wait until new weapon is equipped for the first time
    //

    if( m_State == EQUIP_WEAPON )
    {
        player_object::invent_type Type = GetCurrentWeapon();
        xbool* pSelected = NULL;
        object::id WeaponID;
    
        switch( Type )
        {
            case player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER    :
                pSelected = &m_bDiskLauncher;
                m_Audio   = T2_DISKLAUNCHER;
                m_Prompt  = TEXT_T2_DISKLAUNCHER;
                WeaponID  = m_DiskLauncher;
                break;
                
            case player_object::INVENT_TYPE_WEAPON_PLASMA_GUN       :
                pSelected = &m_bPlasma;
                m_Audio   = T2_PLASMA;
                m_Prompt  = TEXT_T2_PLASMA;
                WeaponID  = m_Plasma;
                break;
                
            case player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER :
                pSelected = &m_bGrenadeLauncher;
                m_Audio   = T2_GRENADELAUNCHER;
                m_Prompt  = TEXT_T2_GRENADELAUNCHER;
                WeaponID  = m_GrenadeLauncher;
                break;
                
            case player_object::INVENT_TYPE_WEAPON_CHAIN_GUN        :
                pSelected = &m_bChainGun;
                m_Audio   = T2_CHAINGUN;
                m_Prompt  = TEXT_T2_CHAINGUN;
                WeaponID  = m_ChainGun;
                break;
                
            case player_object::INVENT_TYPE_WEAPON_BLASTER          :
                pSelected = &m_bBlaster;
                m_Audio   = T2_BLASTER;
                m_Prompt  = TEXT_T2_BLASTER;
                WeaponID  = m_Blaster;
                break;
                
            default : break;
        };
    
        if( pSelected )
        {
            if( *pSelected == FALSE )
            {
                *pSelected  = TRUE;
                SetNextState( FIRE_WEAPON );
            }
            else
            {
                // At this point at least 1 weapon is held and has been fired
            
                s32 NumWeaponsHeld = GetNumWeaponsHeld();
                xbool AudioPrompt  = FALSE;
            
                // Check if we need to teach player about exchanging weapons
                if( m_bCycleWeapon == TRUE )
                {
                    if( !m_bExchangeWeapon && (m_NumWeaponsUsed == 3) )
                    {
                        AudioPrompt = TRUE;
                        SetNextState( EXCHANGE_DELAY );
                    }
                }
                else
                {
                    // Check if we need to teach player about cycling weapons
                    if( NumWeaponsHeld > 1 )
                    {
                        AudioPrompt = TRUE;
                        SetNextState( CYCLE_DELAY  );
                    }
                }

                // If there are no audio prompts then add some additional text prompts for clarity
                if( AudioPrompt == FALSE )
                {
                    if( NumWeaponsHeld > 1 )
                    {
                        // Determine whether all currently held weapons have been fired
                        s32 NumFired = 0;
                        
                        if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER    ) && m_bDiskLauncher    ) NumFired++;
                        if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_PLASMA_GUN       ) && m_bPlasma          ) NumFired++;
                        if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER ) && m_bGrenadeLauncher ) NumFired++;
                        if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_CHAIN_GUN        ) && m_bChainGun        ) NumFired++;
                        if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_BLASTER          ) && m_bBlaster         ) NumFired++;
                        
                        ASSERT( (NumFired >= 1) && (NumFired <= 3) );
                        ASSERT(  NumFired <= NumWeaponsHeld );

                        if( NumFired < NumWeaponsHeld )
                        {
                            // Player is holding weapons that havent been fired yet
                            PrintObjective( TEXT_T2_CYCLE_WEAPONS );
                        }
                        else
                        {
                            if( NumWeaponsHeld == 3 )
                            {
                                // All 3 weapons held have been fired so player must exchange one
                                PrintObjective( TEXT_T2_EXCHANGE_WEAPON );
                            }
                            else
                            {
                                // Player has fired all held weapons but can hold more
                                PrintObjective( TEXT_T2_INTRO );
                            }
                        }
                    }
                    else
                    {
                        // Only 1 weapon held so instruct player to go get another
                        PrintObjective( TEXT_T2_INTRO );
                    }
                }
            }
        }
        else
        {
            PrintObjective( TEXT_T2_INTRO );
        }
    }
    
    //
    // Wait until player has fired at least 1 round with new weapon
    //

    if( m_State == FIRE_WEAPON )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            DisablePadButtons( PAD_CHANGE_WEAPON_BUTTONS | PAD_FIRE_BUTTON );
            GameAudio( m_Audio );
            m_bWeaponFired = FALSE;
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                EnablePadButtons( PAD_FIRE_BUTTON );
                PrintObjective( m_Prompt );
            }
        
            if( m_bObjective == TRUE )
            {
                // Wait until weapon is fired
                if( m_bWeaponFired == TRUE )
                {
                    // Enable the pad buttons if the button function has been explained to player
                    if( m_bCycleWeapon    == TRUE ) EnablePadButtons( PAD_NEXT_WEAPON_BUTTON );
                    if( m_bExchangeWeapon == TRUE ) EnablePadButtons( PAD_EXCHANGE_WEAPON_BUTTON );
            
                    m_NumWeaponsUsed++;
                    SetNextState( EQUIP_WEAPON );
                }
            }
        }
    }

    //
    // Wait until player uses weapon cycle button
    //
    
    if( m_State == CYCLE_WEAPON )
    {
        if( m_bInitState == FALSE )
        {
            m_bCycleWeapon = FALSE;
            GameAudio( T2_CYCLE_WEAPONS );
        }
        else
        {
            if( !m_bCycleWeapon && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bCycleWeapon = TRUE;
                PrintObjective( TEXT_T2_CYCLE_WEAPONS );
                EnablePadButtons( PAD_NEXT_WEAPON_BUTTON );
            }
        
            if( m_bCycleWeapon == TRUE )
            {
                if( IsButtonPressed( PAD_NEXT_WEAPON_BUTTON ) == TRUE )
                {
                    SetNextState( EQUIP_WEAPON );
                }
            }
        }
    }
    
    //
    // Allow player to exchange weapon
    //
    
    if( m_State == EXCHANGE_WEAPON )
    {
        if( m_bInitState == FALSE )
        {
            m_bExchangeWeapon  = FALSE;
            m_bWeaponExchanged = FALSE;
            GameAudio( T2_EXCHANGE_WEAPON );
        }
        else
        {
            if( !m_bExchangeWeapon && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bExchangeWeapon = TRUE;
                PrintObjective( TEXT_T2_EXCHANGE_WEAPON );
                EnablePadButtons( PAD_EXCHANGE_WEAPON_BUTTON );
            }
            
            if( m_bExchangeWeapon == TRUE )
            {
                if( m_bWeaponExchanged == TRUE )
                {
                    m_bWeaponExchanged = FALSE;
                    SetNextState( EQUIP_WEAPON );
                }
            }
        }
    }
    
    //
    // Wait until all weapons used by player
    //
    
    if( m_NumWeaponsUsed == 5 )
    {
        ClearWaypoint( m_DiskLauncher    );
        ClearWaypoint( m_Plasma          );
        ClearWaypoint( m_GrenadeLauncher );
        ClearWaypoint( m_ChainGun        );
        ClearWaypoint( m_Blaster         );

        SetState( STATE_SNIPER );
    }
}

//=============================================================================

void ct2_logic::Sniper( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_bBotGenPower = TRUE;
        m_NewState     = s_FirstState;
        m_State        = -1;

        AddWaypoint( m_Sniper    );
        AddWaypoint( m_Waypoint1 );

        SpawnBots( TEAM_ENEMY, 0 );
        EnablePadButtons( PAD_NEXT_WEAPON_BUTTON );
        EnablePadButtons( PAD_EXCHANGE_WEAPON_BUTTON );
    }

    AdvanceStates();

    //
    // Audio Delay
    //
    
    if( m_State == SNIPER_DELAY )
    {
        if( m_Time > SniperAudioDelay )
            NextState();
    }

    //
    // Instruct player to go to sniper range
    //
    
    if( m_State == GOTO_SNIPER_RANGE )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T2_GOTO_SNIPER_RANGE );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PowerLoss( m_SniperGen );
                PowerLoss( m_ChuteGen );
                PrintObjective( TEXT_T2_GOTO_SNIPER_RANGE );
            }
            
            if( m_bObjective == TRUE )
            {
                if( IsItemHeld( player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE ))
                {
                    ClearWaypoint( m_Sniper    );
                    ClearWaypoint( m_Waypoint1 );
                    NextState();
                }
            }
        }
    }

    //
    // Teach player about Zoom button
    //

    if( m_State == ZOOM_MODE )
    {
        if( m_bInitState == FALSE )
        {
            PowerGain( m_ChuteGen );
            m_bObjective = FALSE;
            DisablePadButtons( PAD_FIRE_BUTTON | PAD_CHANGE_WEAPON_BUTTONS );
            GameAudio( T2_USE_ZOOM );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T2_USE_ZOOM );
                EnablePadButtons( PAD_ZOOM_BUTTON | PAD_ZOOM_IN_BUTTON | PAD_ZOOM_OUT_BUTTON );
            }
            
            if( m_bObjective == TRUE )
            {
                if( IsButtonPressed( PAD_ZOOM_BUTTON ) == TRUE )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Sniper Rifle practice
    //

    if( m_State == USE_SNIPER_RIFLE )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T2_SNIPERRIFLE );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T2_SNIPERRIFLE );
                
                PowerLoss( m_BotGen );
                
                // Dont enable weapon exchange since player could lose sniper rifle!
                EnablePadButtons( PAD_FIRE_BUTTON );
            }

            if( m_bObjective == TRUE )
            {
                // Detect Bot being killed
                if( m_nEnemies == 0 )
                {
                    ClearText();
                    SetState( STATE_AUTOLOCK );

                    // Remove the ammo box dropped by the dead bot            
                    ObjMgr.StartTypeLoop( object::TYPE_PICKUP );
                    pickup* pPickup;
                    while( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
                    {
                        if( pPickup->GetKind() == pickup::KIND_AMMO_BOX )
                        {
                            pPickup->SetState( pickup::STATE_DEAD );
                        }
                    }
                    ObjMgr.EndTypeLoop();
                }
            }
        }
    }
}

//=============================================================================

object::id ct2_logic::CreateDerm( object::id WaypointID )
{
    object* pObject = ObjMgr.GetObjectFromID( WaypointID );
    ASSERT( pObject );

    vector3 Pos( pObject->GetPosition() );
    
    object::id Bot = CreateBot( Pos,
                                player_object::CHARACTER_TYPE_BIO,
                                player_object::SKIN_TYPE_DSWORD,
                                player_object::VOICE_TYPE_BIO_MONSTER,
                                default_loadouts::STANDARD_EMPTY );

    player_object* pBot = (player_object*)ObjMgr.GetObjectFromID( Bot );
    pBot->SetRespawn ( FALSE );
    pBot->SetTeamBits( ENEMY_TEAM_BITS );
    
    return( Bot );
}

//=============================================================================

f32 BotWalkTime = 5.0f;
f32 BotJetTime  = 1.0f;
f32 BotLookX    = 0.25f;
f32 BotMoveY    = 0.50f;

void ct2_logic::UpdateDerm( object::id BotID )
{
    player_object::move Move;
    
    Move.Clear();
    Move.LookX = BotLookX;
    Move.MoveY = BotMoveY;

    if( m_bBotJet )
    {
        Move.JetKey = 1;
        
        if( m_Time > BotJetTime )
        {
            m_bBotJet = FALSE;
            m_Time    = 0;
        }
    }
    else
    {
        if( m_Time > BotWalkTime )
        {
            m_bBotJet = TRUE;
            m_Time    = 0;
        }
    }
    
    ControlBot( BotID, Move );
}

//=============================================================================

void ct2_logic::AutoLock( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_bAutoLocked  = FALSE;
        m_bBotJet      = FALSE;
        m_bBotKilled   = FALSE;
        m_bAmmoBox     = FALSE;
        m_bAmmoTaken   = FALSE;
        m_NewState     = s_FirstState;
        m_State        = -1;
        m_Bot          = obj_mgr::NullID;
        
        DisablePadButtons( PAD_FIRE_BUTTON );
    }

    AdvanceStates();

    //
    // AutoLock
    //
    
    if( m_State == AUTOLOCK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T2_AUTOLOCK );
            EnablePadButtons( PAD_CHANGE_WEAPON_BUTTONS );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T2_AUTOLOCK );
            }
            
            if( m_bObjective == TRUE )
            {
                if( GetCurrentWeapon() != player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE )
                {
                    NextState( FALSE );
                }
            }
        }
    }
    
    //
    // Lock On to Bot
    //
    
    if( m_State == LOCK_ON )
    {
        if( m_bInitState == FALSE )
        {
            // Create BioDerm cannon fodder
            m_Bot = CreateDerm( m_BotPt );
            
            // Create pickups
            CreatePickup( pickup::KIND_WEAPON_DISK_LAUNCHER, m_WeaponPt, TRUE ); 
            CreatePickup( pickup::KIND_AMMO_DISC,            m_AmmoPt,   TRUE );
        
            PowerLoss( m_FFGen );
            
            DisablePadButtons( PAD_FIRE_BUTTON );
            EnablePadButtons(  PAD_AUTOLOCK_BUTTON );
        }
        else
        {
            if( m_bAutoLocked == TRUE )
            {
                NextState();
            }
        }
    }

    //
    // Kill Bot
    //

    if( m_State == KILL_BOT_AUTOLOCK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T2_KILL_BOT );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T2_KILL_BOT );
                EnablePadButtons( PAD_FIRE_BUTTON );
            }
        }
    }

    //
    // Bot control
    //

    if( m_Bot != obj_mgr::NullID )
    {
        // Drive the bot around
        UpdateDerm( m_Bot );
        
        // Check if the bot has been killed
        object* pObject = ObjMgr.GetObjectFromID( m_Bot );
        if( pObject )
        {
            if( pObject->GetHealth() == 0.0f )
                m_bBotKilled = TRUE;
        }
        
        if( m_bBotKilled == TRUE )
        {
            // Find the ammo box pickup in the world
            ObjMgr.StartTypeLoop( object::TYPE_PICKUP );
            pickup* pPickup;
            while( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
            {
                if( pPickup->GetKind() == pickup::KIND_AMMO_BOX )
                {
                    // Dont let the pickup timeout
                    pPickup->SetFlags( pickup::FLAG_IMMORTAL );
            
                    // Wait until the pickup stops moving                
                    if( pPickup->GetState() == pickup::STATE_IDLE )
                    {
                        if( m_bAmmoBox == FALSE )
                        {
                            m_bAmmoBox = TRUE;
                            
                            // Create a waypoint at the ammo box to help the player locate it
                            waypoint* pWaypoint = (waypoint*)ObjMgr.CreateObject( object::TYPE_WAYPOINT );
                            DEMAND( pWaypoint );
                            
                            pWaypoint->Initialize( pPickup->GetPosition(), 1, NULL );
                            ObjMgr.AddObject( pWaypoint, obj_mgr::AVAILABLE_SERVER_SLOT );
                        }
                    }
                }
            }
            ObjMgr.EndTypeLoop();
        
            // Mission ends when the player picks up the ammo box
            if( m_bAmmoTaken )
            {
                Success( T2_WELLDONE );
            }
        }
    }
}

//==============================================================================

void ct2_logic::EnforceBounds( f32 DeltaTime )
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

