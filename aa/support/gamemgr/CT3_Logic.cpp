//=============================================================================
//
//  Training Mission 3 Logic
//
//=============================================================================

#include "ct3_logic.hpp"
#include "GameMgr.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "..\Demo1\fe_Globals.hpp"

//=============================================================================

enum
{
    PICKUP_REPAIR_PATCH,
    DEPLOY_FRAG_GRENADE,
    DEPLOY_FLARE_GRENADE,
    DEPLOY_CONCUSSION_GRENADE,
    DROP_MINES,
    FIRE_MORTAR,
    MISSILE_DELAY,
    CHANGE_TO_MISSILES,
    FIRE_MISSILES,
    COMPLETE,
};

static s32 s_FirstState = PICKUP_REPAIR_PATCH;

const  f32 GrenadeDelayTime = 4.0f;
const  f32 WeaponDelay      = 4.0f;

//=============================================================================
//  VARIABLES
//=============================================================================

ct3_logic CT3_Logic;

//=============================================================================

void ct3_logic::BeginGame( void )
{
    ASSERT( m_RepairPatch != obj_mgr::NullID );
    ASSERT( m_RepairKit   != obj_mgr::NullID );
    ASSERT( m_Inven       != obj_mgr::NullID );
    ASSERT( m_Turret1     != obj_mgr::NullID );
    ASSERT( m_Turret2     != obj_mgr::NullID );
    ASSERT( m_Turret3     != obj_mgr::NullID );
    ASSERT( m_Turret4     != obj_mgr::NullID );
    ASSERT( m_Turret5     != obj_mgr::NullID );
    ASSERT( m_TurretGen   != obj_mgr::NullID );

    campaign_logic::BeginGame();

    SetState( STATE_MISSION );
    
    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( m_PlayerID );
    DEMAND( pPlayer );
    
    PowerLoss( m_Inven );
    PowerLoss( m_TurretGen );
    
    fegl.PlayerInvulnerable = TRUE;
}

//=============================================================================

void ct3_logic::AdvanceTime( f32 DeltaTime )
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

void ct3_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "PATCH"     )) m_RepairPatch = ItemID;
    if( !x_strcmp( pTag, "KIT"       )) m_RepairKit   = ItemID;
    if( !x_strcmp( pTag, "INVEN"     )) m_Inven       = ItemID;
    if( !x_strcmp( pTag, "TURRET1"   )) m_Turret1     = ItemID;
    if( !x_strcmp( pTag, "TURRET2"   )) m_Turret2     = ItemID;
    if( !x_strcmp( pTag, "TURRET3"   )) m_Turret3     = ItemID;
    if( !x_strcmp( pTag, "TURRET4"   )) m_Turret4     = ItemID;
    if( !x_strcmp( pTag, "TURRET5"   )) m_Turret5     = ItemID;
    if( !x_strcmp( pTag, "TURRETGEN" )) m_TurretGen   = ItemID;
}

//=============================================================================

void ct3_logic::ItemDestroyed( object::id ItemID, object::id OriginID )
{
    (void)OriginID;

    if( ItemID == m_Turret1 ) ClearWaypoint( m_Turret1 );
    if( ItemID == m_Turret2 ) ClearWaypoint( m_Turret2 );
    if( ItemID == m_Turret3 ) ClearWaypoint( m_Turret3 );
    if( ItemID == m_Turret4 ) ClearWaypoint( m_Turret4 );
    if( ItemID == m_Turret5 ) ClearWaypoint( m_Turret5 );
}

//=============================================================================

void ct3_logic::AdvanceStates( void )
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

void ct3_logic::WeaponFired( object::id ItemID )
{
    (void)ItemID;
    m_bWeaponFired = TRUE;
}

//=============================================================================

void ct3_logic::NextState( void )
{
    m_NewState++;
    ClearText();
}

//=============================================================================

void ct3_logic::Mission( void )
{
    if( !m_bInitialized )
    {
        m_bInitialized = TRUE;
        m_bWeaponFired = FALSE;
        m_bGrenadeUsed = FALSE;
        m_NumPickups   = 0;
        m_NewState     = s_FirstState;
        m_State        = -1;

        SetHealth( 0.5f );
        
        SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MORTAR,  3 );
        SetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MISSILE, 3 );
        
        EnablePadButtons( 0 );
        EnablePadButtons( PAD_RIGHT_STICK | PAD_LEFT_STICK | PAD_JUMP_BUTTON | PAD_JET_BUTTON );
        EnablePadButtons( PAD_ZOOM_BUTTON | PAD_ZOOM_IN_BUTTON | PAD_ZOOM_OUT_BUTTON );
    }

    AdvanceStates();

    //
    // Pickup Repair Patch
    //
    
    if( m_State == PICKUP_REPAIR_PATCH )
    {
        if( m_bInitState == FALSE )
        {
            m_bObjective = FALSE;
            AddWaypoint( m_RepairPatch );
            AddWaypoint( m_RepairKit   );
            CreatePickup( pickup::KIND_HEALTH_PATCH, m_RepairPatch );
            CreatePickup( pickup::KIND_HEALTH_KIT,   m_RepairKit   );
            GameAudio( T3_REPAIR_PATCH );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_REPAIR_PATCH );
            }

            if( m_bPickupTouched == TRUE )
            {
                m_bPickupTouched  = FALSE;
                
                if( m_PickupType == pickup::KIND_HEALTH_PATCH )
                {
                    m_NumPickups++;
                    ClearWaypoint( m_RepairPatch );
                }
                
                if( m_PickupType == pickup::KIND_HEALTH_KIT   )
                {
                    m_NumPickups++;
                    ClearWaypoint( m_RepairKit );
                }
            }
            
            if( m_bObjective )
            {
                // Wait until repair patch and kit are picked up
                if( m_NumPickups == 2 )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Drop Mines
    //
    
    if( m_State == DROP_MINES )
    {
        if( m_bInitState == FALSE )
        {
            SetInventCount( player_object::INVENT_TYPE_MINE, 1 );
            GameAudio( T3_MINE );
            m_bObjective   = FALSE;
            m_bGrenadeUsed = FALSE;
            DisablePadButtons( PAD_DROP_GRENADE_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_MINE );
                EnablePadButtons( PAD_DROP_GRENADE_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( m_bGrenadeUsed == TRUE )
                {
                    if( m_Time > GrenadeDelayTime )
                    {
                        NextState();
                    }
                }
                else
                {
                    if( HasWeaponFired( player_object::INVENT_TYPE_MINE ) == TRUE )
                    {
                        m_bGrenadeUsed = TRUE;
                        m_Time = 0;
                    }
                }
            }
        }
    }

    //
    // Deploy Frag Grenade
    //
    
    if( m_State == DEPLOY_FRAG_GRENADE )
    {
        if( m_bInitState == FALSE )
        {
            SetInventCount( player_object::INVENT_TYPE_GRENADE_BASIC, 1 );
            GameAudio( T3_FRAG_GRENADE );
            m_bObjective   = FALSE;
            m_bGrenadeUsed = FALSE;
            DisablePadButtons( PAD_DROP_GRENADE_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_FRAG_GRENADE );
                EnablePadButtons( PAD_DROP_GRENADE_BUTTON );
            }

            if( m_bObjective )
            {
                if( m_bGrenadeUsed == TRUE )
                {
                    if( m_Time > GrenadeDelayTime )
                    {
                        NextState();
                    }
                }
                else
                {
                    if( HasWeaponFired( player_object::INVENT_TYPE_GRENADE_BASIC ) == TRUE )
                    {
                        m_bGrenadeUsed = TRUE;
                        m_Time = 0;
                    }
                }
            }
        }
    }

    //
    // Deploy Flare Grenade
    //
    
    if( m_State == DEPLOY_FLARE_GRENADE )
    {
        if( m_bInitState == FALSE )
        {
            SetInventCount( player_object::INVENT_TYPE_GRENADE_FLARE, 1 );
            GameAudio( T3_FLARE_GRENADE );
            m_bObjective   = FALSE;
            m_bGrenadeUsed = FALSE;
            DisablePadButtons( PAD_DROP_GRENADE_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_FLARE_GRENADE );
                EnablePadButtons( PAD_DROP_GRENADE_BUTTON );
            }

            if( m_bObjective )
            {
                if( m_bGrenadeUsed == TRUE )
                {
                    if( m_Time > GrenadeDelayTime )
                    {
                        NextState();
                    }
                }
                else
                {
                    if( HasWeaponFired( player_object::INVENT_TYPE_GRENADE_FLARE ) == TRUE )
                    {
                        m_bGrenadeUsed = TRUE;
                        m_Time = 0;
                    }
                }
            }
        }
    }

    //
    // Deploy Concussion Grenade
    //
    
    if( m_State == DEPLOY_CONCUSSION_GRENADE )
    {
        if( m_bInitState == FALSE )
        {
            SetInventCount( player_object::INVENT_TYPE_GRENADE_CONCUSSION, 1 );
            GameAudio( T3_CONCUSSION_GRENADE );
            m_bObjective   = FALSE;
            m_bGrenadeUsed = FALSE;
            DisablePadButtons( PAD_DROP_GRENADE_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_CONCUSSION_GRENADE );
                EnablePadButtons( PAD_DROP_GRENADE_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( m_bGrenadeUsed == TRUE )
                {
                    if( m_Time > GrenadeDelayTime )
                    {
                        NextState();
                    }
                }
                else
                {
                    if( HasWeaponFired( player_object::INVENT_TYPE_GRENADE_CONCUSSION ) == TRUE )
                    {
                        m_bGrenadeUsed = TRUE;
                        m_Time = 0;
                    }
                }
            }
        }
    }
    
    //
    // Fire Mortar
    //
    
    if( m_State == FIRE_MORTAR )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T3_MORTAR );
            m_bObjective = FALSE;
            AddWaypoint( m_Turret1 );
            AddWaypoint( m_Turret2 );
            AddWaypoint( m_Turret3 );
            AddWaypoint( m_Turret4 );
            AddWaypoint( m_Turret5 );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_MORTAR );
                EnablePadButtons( PAD_FIRE_BUTTON | PAD_NEXT_WEAPON_BUTTON | PAD_AUTOLOCK_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( m_bWeaponFired == TRUE )
                {
                    m_bWeaponFired = FALSE;
                    NextState();
                }
            }
            
            if( GetCurrentWeapon() == player_object::INVENT_TYPE_WEAPON_MORTAR_GUN )
            {
                DisablePadButtons( PAD_NEXT_WEAPON_BUTTON );
            }
        }
    }

    //
    // Missile Delay
    //
    
    if( m_State == MISSILE_DELAY )
    {
        if( m_Time > WeaponDelay )
            NextState();
    }

    //
    // Switch to Missile Launcher
    //
    
    if( m_State == CHANGE_TO_MISSILES )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T3_SWITCH_TO_MISSILE );
            m_bObjective = FALSE;
            DisablePadButtons( PAD_NEXT_WEAPON_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_SWITCH_TO_MISSILE );
                EnablePadButtons( PAD_NEXT_WEAPON_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( GetCurrentWeapon() == player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Fire Missiles
    //
    
    if( m_State == FIRE_MISSILES )
    {
        if( m_bInitState == FALSE )
        {
            GameAudio( T3_MISSILELAUNCHER );
            m_bObjective = FALSE;
            DisablePadButtons( PAD_FIRE_BUTTON | PAD_NEXT_WEAPON_BUTTON );
        }
        else
        {
            if( !m_bObjective && (audio_IsPlaying( m_ChannelID ) == FALSE) )
            {
                m_bObjective = TRUE;
                PrintObjective( TEXT_T3_MISSILELAUNCHER );
                EnablePadButtons( PAD_FIRE_BUTTON | PAD_NEXT_WEAPON_BUTTON | PAD_AUTOLOCK_BUTTON );
            }
            
            if( m_bObjective )
            {
                if( GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MISSILE ) == 0 )
                {
                    NextState();
                }
            }
        }
    }

    //
    // Training Complete
    //

    if( m_State == COMPLETE )
    {
        Success( T3_COMPLETE );
    }
}

//==============================================================================

void ct3_logic::EnforceBounds( f32 DeltaTime )
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


