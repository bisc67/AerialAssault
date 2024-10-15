//=============================================================================
//
//  Training Mission 4 Logic
//
//=============================================================================

#include "ct4_logic.hpp"
#include "GameMgr.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "Objects\Projectiles\Beacon.hpp"
#include "Objects\Projectiles\Station.hpp"
#include "..\Demo1\fe_Globals.hpp"

//=============================================================================

enum
{
    GOTO_REPAIR_PACK,
    GOTO_BUNKER,
    REPAIR_INVEN,
    SHIELD_PACK,
    GET_LOADOUT,
    DEPLOY_INVEN,
    GET_MISSILE_BARREL,
    SWAP_BARREL,
    DEPLOY_TURRET,
    DEPLOY_SENSOR,
    DEPLOY_SATCHEL,
    ACTIVATE_SATCHEL,
    COMPLETE_DELAY,
    COMPLETE,
};

static s32 s_FirstState = GOTO_REPAIR_PACK;
const  f32 CompleteAudioDelay = 3.0f;

//=============================================================================
//  VARIABLES
//=============================================================================

ct4_logic CT4_Logic;

//=============================================================================

void ct4_logic::BeginGame( void )
{
    ASSERT( m_RepairPack != obj_mgr::NullID );
    ASSERT( m_Bunker     != obj_mgr::NullID );
    ASSERT( m_Inven      != obj_mgr::NullID );
    ASSERT( m_InvenPos   != obj_mgr::NullID );
    ASSERT( m_Turret     != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MISSION );
    
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );

    fegl.PlayerInvulnerable = TRUE;
    
    // Change repair pack pickup flags so is doesnt respawn
    ObjMgr.StartTypeLoop( object::TYPE_PICKUP );

    pickup* pPickup;
    while( (pPickup = (pickup*)ObjMgr.GetNextInType()) )
    {
        if( pPickup->GetKind() == pickup::KIND_ARMOR_PACK_REPAIR )
            pPickup->SetFlags( pickup::FLAG_IMMORTAL );
    }
    
    ObjMgr.EndTypeLoop();
}

//=============================================================================

void ct4_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );

    switch( m_GameState )
    {
        case STATE_IDLE     : return;
        case STATE_MISSION  : Mission();    break;
        default             :               break;
    }

    m_Time += DeltaTime;
}

//=============================================================================

void ct4_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "WAYPT1" )) m_RepairPack = ItemID;
    if( !x_strcmp( pTag, "WAYPT2" )) m_Bunker     = ItemID;
    if( !x_strcmp( pTag, "WAYPT3" )) m_InvenPos   = ItemID;
    if( !x_strcmp( pTag, "INVEN"  )) m_Inven      = ItemID;
    if( !x_strcmp( pTag, "TURRET" )) m_Turret     = ItemID;
}

//=============================================================================

void ct4_logic::ItemRepaired( object::id ItemID, object::id PlayerID, xbool Score )
{
    (void)PlayerID;
    (void)Score;

    if( ItemID == m_Inven )
    {
        m_bInvenRepaired = TRUE;
    }
}

//=============================================================================

void ct4_logic::ItemDeployed( object::id ItemID, object::id OriginID )
{
    (void)OriginID;
    object* pObject = ObjMgr.GetObjectFromID( ItemID );
    DEMAND( pObject );

    if( pObject->GetType() == object::TYPE_SATCHEL_CHARGE )
    {
        m_bSatchelDeployed = TRUE;
    }
}

//=============================================================================

void ct4_logic::SatchelDetonated( object::id PlayerID )
{
    (void)PlayerID;
    
    m_bSatchelDestroyed = TRUE;
}

//=============================================================================

void ct4_logic::StationUsed( object::id PlayerID )
{
    (void)PlayerID;
    m_bInvenUsed = TRUE;
}

//=============================================================================

void ct4_logic::AdvanceStates( void )
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

void ct4_logic::NextState( void )
{
    m_NewState++;
    ClearText();
}

//=============================================================================

void ct4_logic::CanOnlyUsePack( player_object::invent_type PackType )
{
    const player_object::loadout& Loadout = GetLoadout();

    // Ensure player cant use pack if he changes it        
    if( Loadout.Inventory.Counts[ PackType ] > 0 )
        EnablePadButtons( PAD_USE_PACK_BUTTON );
    else
        DisablePadButtons( PAD_USE_PACK_BUTTON );
}                

//=============================================================================

void ct4_logic::Mission( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized      = TRUE;
        m_bHint             = FALSE;
        m_bInvenRepaired    = FALSE;
        m_bInvenUsed        = FALSE;
        m_bSatchelDeployed  = FALSE;
        m_bSatchelDestroyed = FALSE;
        m_bPowerDownInven   = FALSE;
        m_NewState          = s_FirstState;
        m_State             = -1;

        EnablePadButtons( 0 );
        EnablePadButtons( PAD_RIGHT_STICK | PAD_LEFT_STICK | PAD_JUMP_BUTTON | PAD_JET_BUTTON );
        EnablePadButtons( PAD_ZOOM_BUTTON | PAD_ZOOM_IN_BUTTON | PAD_ZOOM_OUT_BUTTON );
    
        DestroyItem( m_Inven );
        PowerLoss(   m_Inven );
    }

    AdvanceStates();

    //
    // Pickup Repair Pack
    //

    if( m_State == GOTO_REPAIR_PACK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            AddWaypoint( m_RepairPack );
            GameAudio( T4_GOTO_REPAIR_PACK );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_GOTO_REPAIR_PACK );
            }

            if( m_bObjective )
            {
                // Wait until repair pack is picked up
                if( m_bPickupTouched == TRUE )
                {
                    ASSERT( m_PickupType == pickup::KIND_ARMOR_PACK_REPAIR );
                    
                    ClearWaypoint( m_RepairPack );
                    NextState();
                }
            }
        }
    }
    
    //
    // Ski down to bunker
    //
    
    if( m_State == GOTO_BUNKER )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T4_SKI_TO_BUNKER );
            AddWaypoint( m_Inven );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_SKI_TO_BUNKER );
            }

            if( m_bObjective )
            {
                if( (audio_IsPlaying( m_ChannelID ) == FALSE) && (DistanceToObject( m_Inven ) < 20.0f) )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Repair Inventory Station
    //

    if( m_State == REPAIR_INVEN )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            GameAudio( T4_REPAIR_INVEN );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_REPAIR_INVEN );
                EnablePadButtons( PAD_USE_PACK_BUTTON | PAD_FIRE_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( GetCurrentWeapon() == player_object::INVENT_TYPE_WEAPON_REPAIR_GUN )
                    EnablePadButtons ( PAD_FIRE_BUTTON );
                else
                    DisablePadButtons( PAD_FIRE_BUTTON );
                
                if( m_bInvenRepaired )
                {
                    DisablePadButtons( PAD_FIRE_BUTTON );
                    NextState();
                }
            }
        }
    }

    //
    // Get and use the shield pack
    //
    
    if( m_State == SHIELD_PACK )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            m_bInvenUsed = FALSE;
            GameAudio( T4_SHIELD_PACK );
            DisablePadButtons( PAD_USE_PACK_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_SHIELD_PACK );
                PowerGain( m_Inven );
            }
            
            if( m_bObjective )
            {
                // Wait until inven station is used
                if( m_bInvenUsed )
                {
                    m_bInvenUsed = FALSE;
                    
                    const player_object::loadout& Loadout = GetLoadout();

                    // Check loadout is correct
                    if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_ARMOR_PACK_SHIELD ] > 0 )
                    {
                        EnablePadButtons( PAD_USE_PACK_BUTTON );
                        PrintObjective( TEXT_T4_USE_SHIELD_PACK );
                    }
                    else
                    {
                        DisablePadButtons( PAD_USE_PACK_BUTTON );
                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
                
                // Wait until player activates shield pack        
                if( IsPlayerShielded() )
                {
                    NextState();
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }
        }
    }

    //
    // Get medium armor and deployable inven
    //
    
    if( m_State == GET_LOADOUT )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            m_bInvenUsed = FALSE;
            GameAudio( T4_USE_INVEN );
            DisablePadButtons( PAD_USE_PACK_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_USE_INVEN );
            }
            
            if( m_bObjective )
            {
                const player_object::loadout& Loadout = GetLoadout();

                // Check loadout is correct
                if( (Loadout.Armor == player_object::ARMOR_TYPE_MEDIUM) &&
                    (Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION ] > 0) )
                {
                    ClearWaypoint( m_Inven );
                    NextState();
                    m_bPowerDownInven = TRUE;
                    ClearErrorText();
                }
                else
                {
                    // Wait until inven station is used
                    if( m_bInvenUsed )
                    {
                        m_bInvenUsed = FALSE;

                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }
        }
    }

    //
    // Power down inven when player steps off
    //
    
    if( m_bPowerDownInven == TRUE )
    {
        if( IsPlayerArmed() )
        {
            m_bPowerDownInven = FALSE;
            PowerLoss( m_Inven );
        }
    }
    
    //
    // Deploy Inven
    //
    
    if( m_State == DEPLOY_INVEN )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            AddWaypoint( m_InvenPos );
            GameAudio( T4_DEPLOY_INVEN );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_DEPLOY_INVEN );
            }
            
            if( m_bObjective )
            {
                CanOnlyUsePack( player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION );
                
                const player_object::loadout& Loadout = GetLoadout();
                
                // Check inven has been deployed
                if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION ] == 0 )
                {
                    ClearWaypoint( m_InvenPos );
                    DisablePadButtons( PAD_USE_PACK_BUTTON );
                    NextState();
                }
            }
        }    
    }

    //
    // Get Missile Barrel
    //

    if( m_State == GET_MISSILE_BARREL )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            DisablePadButtons( PAD_USE_PACK_BUTTON );
            GameAudio( T4_USE_DEPLOYED_INVEN );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_USE_DEPLOYED_INVEN );
            }
            
            if( m_bObjective )
            {
                const player_object::loadout& Loadout = GetLoadout();

                // Check loadout is correct
                if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE ] > 0 )
                {
                    NextState();
                    ClearErrorText();
                }
                else
                {
                    if( m_bInvenUsed )
                    {
                        m_bInvenUsed = FALSE;
                    
                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }            
        }
    }

    //
    // Swap Turret Barrel
    //
    
    if( m_State == SWAP_BARREL )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            DisablePadButtons( PAD_USE_PACK_BUTTON );
            AddWaypoint( m_Turret );
            GameAudio( T4_SWAP_TURRET_BARREL );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_SWAP_TURRET_BARREL );
            }

            if( m_bObjective )
            {
                CanOnlyUsePack( player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE );
            
                // NOTE: this function checks if the barrel on the turret is a missile barrel!
                if( IsItemDeployed( object::TYPE_TURRET_FIXED ) )
                {
                    ClearWaypoint( m_Turret );
                    NextState();
                }
            }
        }
    }

    //
    // Deploy Clamp Turret
    //

    if( m_State == DEPLOY_TURRET )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            DisablePadButtons( PAD_USE_PACK_BUTTON );
            GameAudio( T4_DEPLOY_TURRET );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_DEPLOY_TURRET );
            }
            
            if( m_bObjective )
            {
                if( m_bInvenUsed )
                {
                    m_bInvenUsed = FALSE;
                
                    const player_object::loadout& Loadout = GetLoadout();
                    if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR ] == 0 )
                    {
                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
            
                CanOnlyUsePack( player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR );
                
                if( IsItemDeployed( object::TYPE_TURRET_CLAMP ) )
                {
                    NextState();
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }
        }
    }

    //
    // Deploy Sensor
    //

    if( m_State == DEPLOY_SENSOR )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            DisablePadButtons( PAD_USE_PACK_BUTTON );
            GameAudio( T4_DEPLOY_SENSOR );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_DEPLOY_SENSOR );
            }
            
            if( m_bObjective )
            {
                if( m_bInvenUsed )
                {
                    m_bInvenUsed = FALSE;
                
                    const player_object::loadout& Loadout = GetLoadout();
                    if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR ] == 0 )
                    {
                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
                
                CanOnlyUsePack( player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR );
                
                if( IsItemDeployed( object::TYPE_SENSOR_REMOTE ) )
                {
                    NextState();
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }
        }
    }

    //
    // Deploy Satchel
    //

    if( m_State == DEPLOY_SATCHEL )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T4_DEPLOY_SATCHEL );
            m_bObjective = FALSE;
            DisablePadButtons( PAD_USE_PACK_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_DEPLOY_SATCHEL );
            }
            
            if( m_bObjective )
            {
                if( m_bInvenUsed )
                {
                    m_bInvenUsed = FALSE;
                
                    const player_object::loadout& Loadout = GetLoadout();
                    if( Loadout.Inventory.Counts[ player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE ] == 0 )
                    {
                        GameAudio( T4_WRONG_LOADOUT );
                        ErrorText( TEXT_T4_WRONG_LOADOUT );
                    }
                }
                
                CanOnlyUsePack( player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE );
                
                if( IsButtonPressed( PAD_USE_PACK_BUTTON ) )
                {
                    // Find deployed inven and disable it
                    ObjMgr.StartTypeLoop( object::TYPE_STATION_DEPLOYED );
                    station* pStation;
                    pStation = (station*)ObjMgr.GetNextInType();
                    ObjMgr.EndTypeLoop();
                
                    ASSERT( pStation );
                    pStation->ForceDisabled();
                    
                    DisablePadButtons( PAD_USE_PACK_BUTTON );
                    NextState();
                }
            }
            else
            {
                m_bInvenUsed = FALSE;
            }
        }
    }

    //
    // Activate Satchel
    //
    
    if( m_State == ACTIVATE_SATCHEL )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T4_ACTIVATE_SATCHEL );
            m_bObjective = FALSE;
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T4_ACTIVATE_SATCHEL );
                EnablePadButtons( PAD_USE_PACK_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( m_bSatchelDestroyed )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Delay
    //
    
    if( m_State == COMPLETE_DELAY )
    {
        if( m_Time > CompleteAudioDelay )
            NextState();
    }
    
    //
    // Training Complete
    //

    if( m_State == COMPLETE )
    {
        Success( T4_COMPLETE );
    }
}

//==============================================================================

void ct4_logic::EnforceBounds( f32 DeltaTime )
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

