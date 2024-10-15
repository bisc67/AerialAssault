//==============================================================================
//
//  TargetingLaser.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "ObjectMgr\Object.hpp"
#include "Objects/Projectiles/Beacon.hpp"

//==============================================================================
// DATA
//==============================================================================

static s32      s_TargetingLaserTexBeam = -1 ;
static s32      s_TargetingLaserTexSpot = -1 ;

#define NUM_BEAM_PTS    10

//==============================================================================
// DEFINES
//==============================================================================

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::TargetingLaser_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		TargetingLaser_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			TargetingLaser_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			TargetingLaser_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		TargetingLaser_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	TargetingLaser_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::TargetingLaser_ExitState()
{
	// Call exit state functions of current state
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:										   ; break ; 
		case WEAPON_STATE_IDLE:			         TargetingLaser_Exit_IDLE()           ; break ; 
		case WEAPON_STATE_FIRE:					 TargetingLaser_Exit_FIRE();		   ; break ; 
		case WEAPON_STATE_RELOAD:										   ; break ; 
		case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::TargetingLaser_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		TargetingLaser_Setup_ACTIVATE	    () ; break ; 
		case WEAPON_STATE_IDLE:			TargetingLaser_Setup_IDLE		    () ; break ; 
		case WEAPON_STATE_FIRE:			TargetingLaser_Setup_FIRE		    () ; break ; 
		case WEAPON_STATE_RELOAD:		TargetingLaser_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	TargetingLaser_Setup_DEACTIVATE	() ; break ; 
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

void player_object::TargetingLaser_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
	    audio_Play(SFX_WEAPONS_TARGETLASER_ACTIVATE) ;
    }
    else
    {
	    audio_Play(SFX_WEAPONS_TARGETLASER_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::TargetingLaser_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::TargetingLaser_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::TargetingLaser_Exit_IDLE()
{
}

void player_object::TargetingLaser_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::TargetingLaser_Setup_FIRE()
{
    // Out of ammo?
    ASSERT(m_WeaponInfo) ;

    // start the anim playing
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE, 0.1f) ;
    m_WeaponInstance.GetCurrentAnimState().SetSpeed( 1.0f );  
    
    // don't start the sound playing until we know we can draw the beam
    audio_StopLooping(m_WeaponSfxID) ;

    // clear our target variable
    SetWeaponTargetID(obj_mgr::NullID) ;

    // no stray rendering until we're sure we can render a valid beam
    m_TargetingLaserIsValid = FALSE;
}

void player_object::TargetingLaser_Advance_FIRE(f32 DeltaTime)
{
    (void)DeltaTime;

    // Released fire?
    if ( ( tgl.ServerPresent || m_ProcessInput ) && (!m_Move.FireKey) )
    {
            Weapon_SetupState( WEAPON_STATE_IDLE );
    }
    else
    {
        // figure out where we're aiming
        vector3 WeaponPos = GetWeaponFirePos() ;

        // find the target
        vector3     ColPt;
        radian3     VRot;
        f32         VFOV;
        vector3     VZ, VP, End;

        collider::collision Collision;

        GetView( VP, VRot, VFOV );
        VZ.Set( VRot.Pitch, VRot.Yaw );
    
        End = VP+(VZ*1000);

        // set up the ray
        s_Collider.ClearExcludeList() ;
        s_Collider.RaySetup( this, VP, End, GetObjectID().GetAsS32() );

        // try to hit something with it
        ObjMgr.Collide  ( ATTR_SOLID, s_Collider );

        if ( s_Collider.HasCollided() )
        {
            s_Collider.GetCollision( Collision );
            ColPt = Collision.Point;
        }
        else
            ColPt = End;

        // validate this ray from the weapon's point of view
        s_Collider.RaySetup( this, WeaponPos, ColPt, GetObjectID().GetAsS32() );

        // try to hit something with it
        ObjMgr.Collide  ( ATTR_SOLID, s_Collider );

        // move the collision point back?
        if ( s_Collider.HasCollided() )
        {
            s_Collider.GetCollision( Collision );

            // raise the point up just a hair...otherwise will be below LOS much of the time
            ColPt = Collision.Point + ( Collision.Plane.Normal * 0.1f );
        }

        m_TargetingLaserIsValid = TRUE;
        m_LocalWeaponTargetPt = ColPt;

        // update the beacon, or create it if it doesn't yet exist
        if ( tgl.ServerPresent )
        {
            if ( m_WeaponTargetID == obj_mgr::NullID )
            {
                beacon* pBeacon = (beacon*)ObjMgr.CreateObject( object::TYPE_BEACON );
                pBeacon->Initialize( ColPt, m_TeamBits, this->GetObjectID() );
                ObjMgr.AddObject( pBeacon, obj_mgr::AVAILABLE_SERVER_SLOT );

                // this is now our "target"
                SetWeaponTargetID(pBeacon->GetObjectID()) ;
            }
            else
            {
                // it exists, so just update it's position
                beacon* pBeacon = (beacon*)ObjMgr.GetObjectFromID( m_WeaponTargetID );

                ASSERT( pBeacon );
                pBeacon->SetPosition( ColPt );
            }
        }
    }
}

void player_object::TargetingLaser_Exit_FIRE( )
{
    // Stop sound
    audio_StopLooping( m_WeaponSfxID ) ;

    // destroy the beacon object when not needed (it's local, no tgl.serverpresent needed)
    if ( m_WeaponTargetID != obj_mgr::NullID )
    {
        beacon* pBeacon = (beacon*)ObjMgr.GetObjectFromID( m_WeaponTargetID );

        ASSERT( pBeacon );
        ObjMgr.RemoveObject( pBeacon );
    }

    SetWeaponTargetID(obj_mgr::NullID) ;

    m_TargetingLaserIsValid = FALSE;
}

//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::TargetingLaser_Setup_RELOAD()
{
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::TargetingLaser_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::TargetingLaser_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::TargetingLaser_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
f32 CalcLinePts( const vector3& Start, const vector3& End, vector3* pPoints, s32 NumPoints );

void player_object::TargetingLaser_RenderFx( void )
{
    vector3 ColPt;

    // A pseudo-delta time
    f32 TheTime = (f32)(tgl.LogicTimeMs % 4000) / 1000.0f;

    // Timer 1 : one cycle every 1/4 second
    f32 RenderTime = (tgl.LogicTimeMs % 250) / 250.0f ; 
    // Timer 2 : one cycle every 4 seconds
    f32 RenderTime2 = TheTime / 4.0f; 

    // Lookup textures
    if (s_TargetingLaserTexBeam == -1)
    {
	    s_TargetingLaserTexBeam = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]TargetBeam") ;  // not extension not needed!
	    s_TargetingLaserTexSpot = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]TargetSpot") ;  // not extension not needed!
    }

    // Only draw when firing and you have a valid target
    if ( (m_WeaponState == WEAPON_STATE_FIRE) )
    {
        // did we?
        if ( m_TargetingLaserIsValid )
        {
            vector3 BeamPts[ NUM_BEAM_PTS ];

            // find the firing point of the weapon
            vector3 WeaponPos = m_WeaponInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON) ;
            matrix4 L2W;
            L2W.SetTranslation(WeaponPos) ;
   		    L2W.SetRotation(radian3(m_DrawRot.Pitch, m_DrawRot.Yaw, 0)) ;

            // grab our target point (we calculated it during the logic phase)
            ColPt = m_LocalWeaponTargetPt;

            // play the sound if we haven't started already
            audio_PlayLooping(m_WeaponSfxID, SFX_WEAPONS_REPAIR_USE, &m_WorldPos) ;

            // prepare to draw the beam
            f32 Len = CalcLinePts( WeaponPos, ColPt, BeamPts, NUM_BEAM_PTS );

            // if zoomed in, adjust UV's to avoid piling
            Len /= GetCurrentZoomFactor();

            // render the effect
            draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            #ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
            gsreg_SetZBufferUpdate(FALSE);
            gsreg_SetClamping( FALSE );
            gsreg_End();
            #endif
            // render the bright spot at the barrel and the target
            draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_TargetingLaserTexBeam).GetXBitmap() );
            draw_OrientedStrand( BeamPts, NUM_BEAM_PTS, vector2(-RenderTime,0), vector2(Len-RenderTime,1), XCOLOR_WHITE, XCOLOR_WHITE, 0.10f );
            draw_End();

            // render the spots
            // First, draw the hot points at the beginning and end
            draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            
            #ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
            gsreg_SetZBufferUpdate(FALSE);
            gsreg_SetZBufferTest( ZBUFFER_TEST_GEQUAL );
            gsreg_SetClamping( FALSE );
            gsreg_End();
            #endif

            // render the bright spot at the end of the gun
            draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_TargetingLaserTexSpot).GetXBitmap() );
            draw_SpriteUV( WeaponPos, vector2(0.27f, 0.27f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360  );

            tgl.PostFX.AddFX( WeaponPos, vector2(1.0f, 1.0f), xcolor(225,255,225,38) );

            // turn off z-buffer for shooter to render nice glowy spot (guaranteed to be in line-of-sight anyway)
            if ( m_ProcessInput )
            {
                draw_End();
                draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );

                #ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
                gsreg_SetZBufferUpdate(FALSE);
                gsreg_SetZBufferTest( ZBUFFER_TEST_ALWAYS );
                gsreg_SetClamping( FALSE );
                gsreg_End();
                #endif
            }

            // render the spot on the ground
            draw_SpriteUV( ColPt, vector2(1.0f, 1.0f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
            tgl.PostFX.AddFX( ColPt, vector2(2.0f, 2.0f), xcolor(225,255,225,38) );

            draw_End();

#ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetZBufferUpdate(TRUE);
            gsreg_SetZBufferTest( ZBUFFER_TEST_GEQUAL );
            gsreg_End();
#endif
        }
    }
}

f32 CalcLinePts( const vector3& Start, const vector3& End, vector3* pPoints, s32 NumPoints )
{
    // real simple...divide the line into (NumPoints-1) segments
    s32 i;
    vector3 Delta;
    vector3 Tmp = Start;

    Delta.X = ( End.X - Start.X ) / (NumPoints-1);
    Delta.Y = ( End.Y - Start.Y ) / (NumPoints-1);
    Delta.Z = ( End.Z - Start.Z ) / (NumPoints-1);

    for ( i = 0; i < NumPoints; i++ )
    {
        pPoints[i] = Tmp;
        Tmp += Delta;
    }

    // return the length of each segment, for use in UV
    return Delta.Length();
}