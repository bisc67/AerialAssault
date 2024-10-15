//==============================================================================
//
//  ChainGun.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "Objects/Projectiles/bullet.hpp"
#include "pointlight\pointlight.hpp"
#include "GameMgr/GameMgr.hpp"

//==============================================================================
// LOCALS
//==============================================================================

static s32  s_FlashTexture                  = -1;
f32         CHAINGUN_RAMP_SPEED_UP_TIME     =  1.0f;   // Secs

//==============================================================================
// DEFINES
//==============================================================================

#define RELOAD_SFX_DELAY_TIME               0.5f
#define CHAINGUN_DELAY                      (1.0f / 8.0f)       //  8 shots/sec
#define CHAINGUN_AUDIO_DELAY                (1.0f / 10.0f)      // 10 shots/sec
#define CHAINGUN_RAND_ANGLE                 0.03f
#define CHAINGUN_BOT_RAND_ANGLE             0.03f
#define CHAINGUN_RAMP_SPEED_DOWN_TIME       2.0f        // Secs
#define CHAINGUN_SPIN_SPEED                 (2.00f)     // 2 Revs per sec

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::ChainGun_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		ChainGun_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			ChainGun_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			ChainGun_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		ChainGun_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	ChainGun_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}

    // Set the spin (always use the fire anim)
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE) ;
    anim* Anim = m_WeaponInstance.GetCurrentAnimState().GetAnim() ;
    ASSERT(Anim) ;
    f32   Frames = (f32)Anim->GetFrameCount() ;
    f32   Frame = (m_WeaponSpin/R_360) * (Frames-1.0f) ;    // First and last frames are the same!
    
    m_WeaponInstance.GetCurrentAnimState().SetFrame(Frame) ;
    m_WeaponInstance.GetCurrentAnimState().SetSpeed( 0.0f ) ;

    // Update the spin
    if (m_WeaponState == WEAPON_STATE_FIRE)
    {
        // but only if we have bullets
        if ( GetWeaponAmmoCount(m_WeaponCurrentType) >= m_WeaponInfo->AmmoNeededToAllowFire )
        {
            // Increase speed
            m_WeaponSpinSpeed += (1.0f / CHAINGUN_RAMP_SPEED_UP_TIME) * m_FrameRateDeltaTime ;
            if (m_WeaponSpinSpeed > 1)
                m_WeaponSpinSpeed = 1 ;
        }
    }
    else
    {
        // Decrease speed
        m_WeaponSpinSpeed -= (1.0f / CHAINGUN_RAMP_SPEED_DOWN_TIME) * m_FrameRateDeltaTime ;
        if (m_WeaponSpinSpeed < 0)
            m_WeaponSpinSpeed = 0 ;
    }

    // Goto next rotation
    m_WeaponSpin += m_WeaponSpinSpeed * CHAINGUN_SPIN_SPEED * R_360 * m_FrameRateDeltaTime ;
    m_WeaponSpin = x_ModAngle(m_WeaponSpin) ; // Keep 0->360
    ASSERT(m_WeaponSpin >= 0) ;
    ASSERT(m_WeaponSpin < R_360) ;
}

//==============================================================================

// Exit states
void player_object::ChainGun_ExitState()
{
	// Call exit state functions of current state
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:										   ; break ; 
		case WEAPON_STATE_IDLE:			    ChainGun_Exit_IDLE()           ; break ; 
		case WEAPON_STATE_FIRE:				ChainGun_Exit_FIRE()		   ; break ; 
		case WEAPON_STATE_RELOAD:										   ; break ; 
		case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
	}
}

// Sets up a new state
void player_object::ChainGun_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		ChainGun_Setup_ACTIVATE	    () ; break ; 
		case WEAPON_STATE_IDLE:			ChainGun_Setup_IDLE		    () ; break ; 
		case WEAPON_STATE_FIRE:			ChainGun_Setup_FIRE		    () ; break ; 
		case WEAPON_STATE_RELOAD:		ChainGun_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	ChainGun_Setup_DEACTIVATE	() ; break ; 
	}
}

//==============================================================================
//
// STATE FUNCTIONS
//
//==============================================================================

//==============================================================================
// WEAPON_STATE_ACTIVATE
//==============================================================================

void player_object::ChainGun_Setup_ACTIVATE()
{
    // Stop texture animation
    m_WeaponInstance.SetTexturePlaybackType( material::PLAYBACK_TYPE_ONCE_ONLY );
    m_WeaponInstance.SetTextureAnimFPS(0);
    m_WeaponInstance.SetTextureFrame(100) ;

	// Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_CHAINGUN_ACTIVATE) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_CHAINGUN_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;

    // Reset spin
    m_WeaponSpin      = 0.0f ;
    m_WeaponSpinSpeed = 0.0f ;
}

void player_object::ChainGun_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::ChainGun_Setup_IDLE()
{

    Weapon_Setup_IDLE() ;
}

void player_object::ChainGun_Exit_IDLE()
{
}

void player_object::ChainGun_Exit_FIRE()
{
    // Stop sound
    audio_StopLooping(m_WeaponSfxID) ;
    audio_StopLooping(m_WeaponSfxID2) ;

    // Play wind down
	// Play sound!
    if ( m_WeaponSpinSpeed == 1.0f )
    {
        if (m_ProcessInput)
        {
            audio_Play(SFX_WEAPONS_CHAINGUN_SPINDOWN) ;
        }
        else
        {
            audio_Play(SFX_WEAPONS_CHAINGUN_SPINDOWN, &m_WorldPos) ;
        }
    }
}

void player_object::ChainGun_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::ChainGun_Setup_FIRE()
{
    // Out of ammo?
    ASSERT(m_WeaponInfo) ;
    if (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire)
    {
	    // Play sound only for controlling player
        if ( ( m_ProcessInput ) && ( m_WeaponSpinSpeed == 0.0f ) )
	        audio_Play(SFX_WEAPONS_OUT_OF_AMMO_ALL) ;

        // Clear fire press
        m_Move.FireKey = FALSE ;

        // reset the spin
        m_WeaponSpinSpeed = 0.1f ;
    }
    else
    {
        // Start anim
        m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE, 0.25f) ;
        m_WeaponInstance.GetCurrentAnimState().SetSpeed( 0.0f );
        
        // Start the spinup loop
        if (m_ProcessInput)
            audio_PlayLooping(m_WeaponSfxID, SFX_WEAPONS_CHAINGUN_SPINUP_LOOP) ;
        else
            audio_PlayLooping(m_WeaponSfxID, SFX_WEAPONS_CHAINGUN_SPINUP_LOOP, &m_WorldPos) ;

        m_FrameWeaponStateTime = 0.01f;

        // Play Turn off flash
        m_WeaponInstance.SetTexturePlaybackType(material::PLAYBACK_TYPE_LOOP) ;

        // Stop looping fire sound
        audio_StopLooping(m_WeaponSfxID2) ;
    }
}

void player_object::ChainGun_Advance_FIRE(f32 DeltaTime)
{
    (void)DeltaTime ;

    // Release fire or out of ammo?
    ASSERT(m_WeaponInfo) ;

    xbool Stop = IsDead() ;

    // Out of ammo?
    Stop |= (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire) ;

    // Released fire? (leave if it's a ghost)
    Stop |= (!m_Move.FireKey) && ((tgl.ServerPresent) || (m_ProcessInput)) ;

    // Stop?
    if (Stop)
    {
        // Back to idle mr. weapon
        Weapon_SetupState( WEAPON_STATE_IDLE );
        
        // play one more if we were firing
        if (m_WeaponSfxID2)
            audio_Play( SFX_WEAPONS_CHAINGUN_FIRE, &m_WorldPos );

        // Stop firing sound
        audio_StopLooping(m_WeaponSfxID2) ;
    }
    else
    // Fire weapon only when running at top speed
    if ( m_WeaponSpinSpeed == 1.0f )
    {
        // Fire again?
        m_FrameWeaponStateTime += m_FrameRateDeltaTime; // local framerate version for smoothness
        if ( m_FrameWeaponStateTime > CHAINGUN_DELAY )
        {
            m_FrameWeaponStateTime -= CHAINGUN_DELAY;

            // set the flag
            m_WeaponHasFired = TRUE;

	        // Only create the bullet on the server
	        if (tgl.ServerPresent)
	        {
		        matrix4 L2W ;

                // Set pos
                L2W.SetTranslation(GetWeaponFirePos()) ;
                
                // Set rot
                radian3 WeaponRot = GetWeaponFireRot() ;
                const f32 RandAngle
                    = (GetType() == TYPE_BOT)
                    ? CHAINGUN_BOT_RAND_ANGLE
                    : CHAINGUN_RAND_ANGLE;
                
		        L2W.SetRotation(radian3(WeaponRot.Pitch + x_frand( -RandAngle, RandAngle ),
                                        WeaponRot.Yaw + x_frand( -RandAngle, RandAngle ), WeaponRot.Roll)) ;

		        // Fire!
		        bullet* pBullet = (bullet*)ObjMgr.CreateObject( object::TYPE_BULLET );
    		    pBullet->Initialize( GetTweakedWeaponFireL2W(), m_TeamBits, m_ObjectID );
                ObjMgr.AddObject( pBullet, obj_mgr::AVAILABLE_SERVER_SLOT );

                // Update ammo
                AddInvent(m_WeaponInfo->AmmoType, (s32)-m_WeaponInfo->AmmoUsedPerFire) ;
                
                pGameLogic->WeaponFired();
            }

            // Determine if a local player fired this blaster
            xbool Local = FALSE;
            if( GetWarriorID() != -1 )
            {
                Local = TRUE;
            }

  	        // Play sound looping firing sound!
            if( Local )
                audio_PlayLooping(m_WeaponSfxID2, SFX_WEAPONS_CHAINGUN_FIRE_LOOP );
            else
                audio_PlayLooping(m_WeaponSfxID2, SFX_WEAPONS_CHAINGUN_FIRE_LOOP, &m_WorldPos );

            if (m_ProcessInput)
                input_Feedback(0.25f,0.3f,m_ControllerID);

            m_WeaponInstance.SetTextureFrame(0) ;

            // Setup pointlight
            ptlight_Create( m_WorldPos, 4.0f, xcolor(255,200,0,64), 0.0f, 0.1f, 0.0f );

            // Start the recoil anim
            m_PlayerInstance.SetAdditiveAnimByType(ADDITIVE_ANIM_ID_RECOIL, ANIM_TYPE_CHARACTER_RECOIL) ;

            // Set to frame zero incase it's already playing
            anim_state *Recoil = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_RECOIL) ;
            ASSERT(Recoil) ;
            if (Recoil->GetAnim())
            {
                Recoil->SetFrame(0.0f) ;
                Recoil->SetAnimPlayed(0) ;
            }
        }
    }    
}


//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::ChainGun_Setup_RELOAD()
{
    // Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_CHAINGUN_SPINDOWN) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_CHAINGUN_SPINDOWN, &m_WorldPos) ;
    }
}

void player_object::ChainGun_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::ChainGun_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::ChainGun_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================

void player_object::ChainGun_RenderFx( void )
{
    if ( ( m_WeaponState == WEAPON_STATE_FIRE ) && ( m_WeaponSpinSpeed == 1.0f ) && ( m_WeaponHasFired == TRUE ) )
    {

        // clear the flag
        m_WeaponHasFired = FALSE;

        // find the firing point of the weapon
        vector3 StartPos = m_WeaponInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON) ;

        // end point of the flame
        vector3 EndPos( 0.0f, 0.0f, x_frand(0.6f, 1.2f) );

        radian3 StartRot = m_WeaponInstance.GetRot() ;
        EndPos.Rotate( StartRot );
        EndPos.RotateY( x_frand(-R_10, R_10) );
        EndPos.RotateZ( x_frand(-R_10, R_10) );

        EndPos += StartPos;

        // start drawing
        draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
        #ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
            gsreg_SetZBufferUpdate(FALSE);
            gsreg_SetClamping( FALSE );
            gsreg_End();
        #endif

        // find the texture
        if ( s_FlashTexture == -1 )
        {
            s_FlashTexture = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]Flash") ;
        }

        // activate the texture
        draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_FlashTexture).GetXBitmap() );

        // draw the oriented quad
        draw_OrientedQuad( StartPos, EndPos, vector2(0,0), vector2(1,1), XCOLOR_WHITE, XCOLOR_WHITE, 0.15f );

        // finished
        draw_End();

        #ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetZBufferUpdate(TRUE);
            gsreg_End();
        #endif
    }
}
