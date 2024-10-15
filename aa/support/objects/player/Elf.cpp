//==============================================================================
//
//  Elf.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "Demo1/Globals.hpp"
#include "ObjectMgr\Object.hpp"
#include "GameMgr/GameMgr.hpp"

//==============================================================================
// DATA
//==============================================================================

static xbool    s_TexInit = FALSE ;
static s32      s_ElfTexIndex=-1 ;
static s32      s_ElfTexBeam ;
static s32      s_PointCore;


//==============================================================================
// DEFINES
//==============================================================================

#define NUM_BEAM_SEGS            16
#define RELOAD_SFX_DELAY_TIME    0.5f

#define ELF_RANGE                37.0f        // from the T2 scripts, not BS!
#define ELF_TARGET_ANGLE         R_45

//==============================================================================
// LOCAL FUNCTIONS
//==============================================================================

void CalcSplinePts( const matrix4& Start, const vector3& End, vector3* pPoints, s32 NumPoints );
xbool CheckSplineCollision( collider& Collider, vector3* pPts, s32 NumPts, vector3& ColPt, s32 Exclude );
void RenderELFBeam( vector3* pBeam );

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::Elf_AdvanceState(f32 DeltaTime)
{
	// Call advance function
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		Elf_Advance_ACTIVATE	(DeltaTime) ; break ; 
		case WEAPON_STATE_IDLE:			Elf_Advance_IDLE		(DeltaTime) ; break ; 
		case WEAPON_STATE_FIRE:			Elf_Advance_FIRE		(DeltaTime) ; break ; 
		case WEAPON_STATE_RELOAD:		Elf_Advance_RELOAD		(DeltaTime) ; break ; 
		case WEAPON_STATE_DEACTIVATE:	Elf_Advance_DEACTIVATE	(DeltaTime) ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::Elf_ExitState()
{
	// Call exit state functions of current state
	switch(m_WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:										   ; break ; 
		case WEAPON_STATE_IDLE:			         Elf_Exit_IDLE()           ; break ; 
		case WEAPON_STATE_FIRE:					 Elf_Exit_FIRE();		   ; break ; 
		case WEAPON_STATE_RELOAD:										   ; break ; 
		case WEAPON_STATE_DEACTIVATE:	                                   ; break ; 
	}
}

//==============================================================================

// Sets up a new state
void player_object::Elf_SetupState(weapon_state WeaponState)
{
	// Call setup function
	switch(WeaponState)
	{
		case WEAPON_STATE_ACTIVATE:		Elf_Setup_ACTIVATE	    () ; break ; 
		case WEAPON_STATE_IDLE:			Elf_Setup_IDLE		    () ; break ; 
		case WEAPON_STATE_FIRE:			Elf_Setup_FIRE		    () ; break ; 
		case WEAPON_STATE_RELOAD:		Elf_Setup_RELOAD		() ; break ; 
		case WEAPON_STATE_DEACTIVATE:	Elf_Setup_DEACTIVATE	() ; break ; 
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

void player_object::Elf_Setup_ACTIVATE()
{
	// Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_ELF_ACTIVATE) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_ELF_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::Elf_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::Elf_Setup_IDLE()
{
    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::Elf_Exit_IDLE()
{
}

void player_object::Elf_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::Elf_Setup_FIRE()
{
    // Out of ammo?
    ASSERT(m_WeaponInfo) ;

    if ( m_Energy < 0.03f )      // 3.0 is from the script(minEnergy = 3)
    {
        if ( tgl.ServerPresent )
        {
            Weapon_SetupState( WEAPON_STATE_IDLE );
            return;
        }
    }
    else
    {
	    // Only create the elf beam on the server
	    if (tgl.ServerPresent)
	    {
            // find the target
            object*    pObj;
            object::id TargetID = obj_mgr::NullID ;

            // what direction are we aiming?
            vector3 FirePos = GetWeaponFirePos() ;
            vector3 AimDir  = GetWeaponFireDir() ;

            // step one: collect nearby targetable objects within a radius

            ObjMgr.Select( object::ATTR_ELFABLE, m_WorldPos, ELF_RANGE );

            // step two: iterate through the collection weeding out non-targetables

            f32 Closest = F32_MAX ;

            while( (pObj = ObjMgr.GetNextSelected()) )
            {
                // Don't elf self!
                if ((pObj != this) && (pObj->GetHealth() > 0))
                {
                    vector3 TarDir = ( pObj->GetPosition() - FirePos );

                    // don't bother computing ATAN if outside of range
                    if ( TarDir.LengthSquared() < Closest )
                    {
                        radian Dif = v3_AngleBetween( AimDir, TarDir);

                        // is this target in angular range
                        if ( Dif < ELF_TARGET_ANGLE )
                        {
                            Closest  = TarDir.LengthSquared();
                            TargetID = pObj->GetObjectID() ;
                        }
                    }
                }
            }
            ObjMgr.ClearSelection();

            // Set new target
            SetWeaponTargetID(TargetID) ;

            // decrease shooter's energy, 1 unit per second
            // (actually I set it to 5...but needs tuning because player recharges too fast right now.)
            if ( TargetID != obj_mgr::NullID )
                AddEnergy( -0.05f * tgl.DeltaLogicTime ) ;

            // Could not fire, revert to idle mode and return
            if ( TargetID == obj_mgr::NullID )
            {
                Weapon_SetupState( WEAPON_STATE_IDLE );
                return;
            }
        }
    }

    // start the anim playing
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE, 0.1f) ;
    m_WeaponInstance.GetCurrentAnimState().SetSpeed( 1.0f );  
    
    // don't start the sound playing until we know we can draw the beam
    // audio_StopLooping(m_WeaponSfxID) ;
    m_WeaponSfxID = -1;
}

void player_object::Elf_Advance_FIRE(f32 DeltaTime)
{
    (void)DeltaTime;
    // continually seek valid targets
	if (tgl.ServerPresent)
	{

        if ( m_Energy < 0.03f )
        {
            Weapon_SetupState( WEAPON_STATE_IDLE );
            return;
        }

        object* pObj;
        object::id TargetID = obj_mgr::NullID ;

        // what direction are we aiming?
        vector3 FirePos = GetWeaponFirePos() ;
        vector3 AimDir  = GetWeaponFireDir() ;

        // step one: collect nearby targetable objects within a radius

        ObjMgr.Select( object::ATTR_ELFABLE, m_WorldPos, ELF_RANGE );

        // step two: iterate through the collection weeding out non-targetables

        f32 Closest = F32_MAX ;

        while( (pObj = ObjMgr.GetNextSelected()) )
        {
            // Don't elf self!
            if ((pObj != this) && (pObj->GetHealth() > 0))
            {
                vector3 TarDir = ( pObj->GetPosition() - FirePos );

                // don't bother computing ATAN if outside of range
                if ( TarDir.LengthSquared() < Closest )
                {
                    radian Dif = v3_AngleBetween( AimDir, TarDir);

                    // is this target in angular range
                    if ( Dif < ELF_TARGET_ANGLE )
                    {
                        Closest = TarDir.LengthSquared();

                        TargetID = pObj->GetObjectID() ;
                    }
                }
            }           
        }
        ObjMgr.ClearSelection();

        // Set new target
        SetWeaponTargetID(TargetID) ;

        // decrease shooter's energy, 1 unit per second
        // (actually I set it to 5...but needs tuning because player recharges too fast right now.)
        AddEnergy( -0.05f * tgl.DeltaLogicTime ) ;

        // Calculate the Beam...
        vector3 Beam[NUM_BEAM_SEGS];

        if ( Elf_CalculateBeam( Beam, FALSE ) == FALSE )
        {
            // revert to no target
            SetWeaponTargetID(obj_mgr::NullID) ;
            Weapon_SetupState( WEAPON_STATE_IDLE );
            return;
        }
        else
        {
            // apply ELF to the target object
            object* pObject = ObjMgr.GetObjectFromID( TargetID );

            if ( ( pObject ) && ( pObject->GetHealth() != 0.0f ) )
            {
                ObjMgr.InflictPain( pain::WEAPON_ELF, 
                                    pObject->GetPosition(),
                                    m_ObjectID,
                                    pObject->GetObjectID(),
                                    DeltaTime );

                pGameLogic->WeaponFired();
            }
            else
            {
                Weapon_SetupState( WEAPON_STATE_IDLE );
                return ;
            }
        }

        // Released fire?
        if (!m_Move.FireKey)
        {
            Weapon_SetupState( WEAPON_STATE_IDLE );
            return ;
        }
    }

    // play the audio in the world for all to hear
    if ( !audio_IsPlaying( m_WeaponSfxID ) )
        audio_PlayLooping( m_WeaponSfxID, SFX_WEAPONS_REPAIR_USE, &m_WorldPos ) ;

    if ( m_ProcessInput )
    {
        input_Feedback(0.25f,0.25f,m_ControllerID);
    }
}

void player_object::Elf_Exit_FIRE( )
{
    // Stop sound
    audio_StopLooping( m_WeaponSfxID );
}

//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::Elf_Setup_RELOAD()
{
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::Elf_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::Elf_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::Elf_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================

void player_object::Elf_RenderFx( void )
{
    // Only draw when firing and you have a valid target
    if ( (m_WeaponState == WEAPON_STATE_FIRE) && ( m_WeaponTargetID != obj_mgr::NullID ) )
    {
        // Draw beam...
        vector3 Beam[NUM_BEAM_SEGS];

        if ( Elf_CalculateBeam( Beam, TRUE ) == TRUE )
        {
            // render it
            RenderELFBeam( Beam );
        }
    }

    // This function has now been exorcised.
}

//==============================================================================

void RenderELFBeam( vector3* pBeam )
{   
    // A pseudo-delta time
    f32 TheTime = (f32)(tgl.LogicTimeMs % 4000) / 1000.0f;
    f32 RenderTime = (tgl.LogicTimeMs % 1000) / 1000.0f ; //1.0f - ( TheTime - (u32)TheTime );
    f32 RenderTime2 = TheTime / 4.0f; //(f32)( TheTime - (((u32)TheTime / 4) * 4.0f) ) / 4.0f;

    // Lookup textures
    if( !s_TexInit )
    {
	    s_TexInit     = TRUE;
	    s_ElfTexIndex = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]elf_beam") ;  // not extension not needed!
	    s_ElfTexBeam  = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]Bolt1") ;  // not extension not needed!
        s_PointCore   = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]HotCore") ;  // not extension not needed! 
    }

    // First, draw the hot points at the beginning and end
    draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_SetClamping( FALSE );
    gsreg_End();
#endif

    // render the bright spot at the target
    draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_PointCore).GetXBitmap() );
    //draw_SpriteUV( pBeam[0], vector2(0.30f, 0.25f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
    //draw_SpriteUV( pBeam[0], vector2(0.30f, 0.25f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360 );
    draw_SpriteUV( pBeam[NUM_BEAM_SEGS-1], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
    draw_SpriteUV( pBeam[NUM_BEAM_SEGS-1], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360 );
    draw_SpriteUV( pBeam[0], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
    draw_SpriteUV( pBeam[0], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360 );

    tgl.PostFX.AddFX( pBeam[NUM_BEAM_SEGS-1], vector2(2.0f, 2.0f), xcolor(225,225,255,38) );
    tgl.PostFX.AddFX( pBeam[0], vector2(1.0f, 1.0f), xcolor(225,225,255,38) );

    draw_End();

// turn the settings back on because draw_End resets them!!
    draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_SetClamping( FALSE );
    gsreg_End();
#endif

    // draw the core
    // find the segment length
    vector3 tmp = (pBeam[1] - pBeam[0]);
    f32 SegLen = tmp.Length() * 0.25f;

    // Activate the core texture 
    draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_ElfTexIndex).GetXBitmap() );
    draw_OrientedStrand (pBeam,
                         NUM_BEAM_SEGS,
                         vector2(RenderTime,0), vector2( SegLen + RenderTime, 1),                                 
                         XCOLOR_WHITE, XCOLOR_WHITE,                                       
                         0.125f );

    // draw the lightning
    draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_ElfTexBeam).GetXBitmap() );
    f32 U1, Rad;
    U1 = x_frand(0.0f, 1.0f);
    Rad = x_frand( 0.125f, 0.160f );

    SegLen *= x_frand( 0.1f, 0.25f) ;

    s32 upside_down = x_irand(0, 1);

    draw_OrientedStrand( pBeam,
                         NUM_BEAM_SEGS,
                         vector2(U1, (f32)upside_down), vector2( U1 + SegLen, (f32)(!upside_down)),                                 
                         XCOLOR_WHITE, XCOLOR_WHITE,                                       
                         Rad );
    // draw the lightning again, upside-down, longer/shorter, smaller and dimmer
    U1  = x_frand(0.0f, 1.0f);
    Rad = x_frand( 0.125f, 0.135f );
    xcolor a;
    a.Set( 128, 128, 128, 128 );

    SegLen *= x_frand( 0.8f, 1.6f) ;
    draw_OrientedStrand( pBeam,
                         NUM_BEAM_SEGS,
                         vector2(U1,1), vector2( U1 + SegLen, 0),                                 
                         a, a,                                       
                         Rad );

    draw_End();

#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
#endif
}

//==============================================================================

void CalcSplinePts( const matrix4& Start, const vector3& End, vector3* pPoints, s32 NumPoints )
{
    vector3 StartPos, EndSeg1, EndSeg2;
    vector3 tmp;
    f32     Mult, CurMult;
    vector3 DeltSeg1, DeltSeg2;
    vector3 CurPosSeg1, CurPosSeg2;
    radian3 StartRot = Start.GetRotation();

    StartPos = Start.GetTranslation();
    tmp = End - StartPos;

    // EndSeg1 is out along the firing axis, 3/4 the distance to the target
    // EndSeg2 is midway between EndSeg1 and the target
    
    // calculate the end segments of the spline
    EndSeg1.Set( 0.0f, 0.0f, tmp.Length() * 0.9f );
    EndSeg1.Rotate(StartRot);
    EndSeg1 += StartPos;

    EndSeg2 = ( EndSeg1 + End ) * 0.5f;

    // now interpolate the spline over 'NumPoints' steps
    Mult = 1.0f / ( NumPoints - 1 );

    // compute the delta segments
    DeltSeg1 = ( EndSeg1 - StartPos ) * Mult;
    DeltSeg2 = ( End - EndSeg2 ) * Mult;

    // stuff in the points we already know
    CurPosSeg1 = StartPos;
    CurPosSeg2 = EndSeg2;
    pPoints[0] = CurPosSeg1;
    pPoints[NumPoints - 1] = End;

    CurMult = 0.0f;

    // calc the rest
    for ( s32 i = 1; i < (NumPoints-1); i++ )
    {
        CurMult += Mult;
        CurPosSeg1 += DeltSeg1;
        CurPosSeg2 += DeltSeg2;

        // interpolate along the CurPosSegs
        pPoints[i] = CurPosSeg1 + ( (CurPosSeg2 - CurPosSeg1) * CurMult );
    }       
}

//==============================================================================

xbool CheckSplineCollision( collider& Collider, vector3* pPts, s32 NumPts, vector3& ColPt, s32 Exclude )
{
    collider::collision     Collision;

    // quick safety check
    // vector3 dist = ( pPts[0] - pPts[NumPts - 1] );

    // if ( dist.LengthSquared() < ( 0.5f * 0.5f ) )
        // return FALSE;

    // walk the spline checking for collisions (all but last seg, since sometimes its under floor)
    for ( s32 i = 0; i < NumPts-1; i++ )
    {
        // Quick safety check
        vector3 dist = ( pPts[i] - pPts[i+1] );

        if ( dist.LengthSquared() < ( 0.5f * 0.5f ) )
            continue;

        Collider.ClearExcludeList() ;
        Collider.RaySetup( NULL, pPts[i], pPts[i+1] );

        ObjMgr.Collide( object::ATTR_SOLID_STATIC | object::ATTR_DAMAGEABLE | object::ATTR_SOLID_DYNAMIC, Collider );

        if ( Collider.HasCollided() )
        {
            Collider.GetCollision( Collision );
            object* pObject = (object*)(Collision.pObjectHit);
            
            if ( pObject->GetObjectID().GetAsS32() == Exclude )
            {
                // we've hit the target object, so adjust the last point on the spline
                // to stop on the collision point
                for ( s32 j = i+1; j < NumPts; j++ )
                {
                    pPts[j] = Collision.Point;
                }
                // return false, so it thinks there wasn't a collision anywhere
                return FALSE;
            }
            else
            {
                ColPt = Collision.Point;
                return TRUE;
            }
        }
    }

    return FALSE;       
}

//==============================================================================

xbool player_object::Elf_CalculateBeam( vector3* Beam, xbool FromRender )
{
    // find the firing point of the weapn
    matrix4 L2W;
    if (FromRender)
    {
        // Use exact draw position
        L2W.Zero();
        L2W.SetTranslation(m_WeaponInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON)) ;
   	    L2W.SetRotation(radian3(m_DrawRot.Pitch, m_DrawRot.Yaw, 0)) ;
    }
    else
    {
        // Use logic fire pos
        L2W = GetWeaponFireL2W() ;
    }

    // Get fire point of weapon
    vector3 WeaponPos;
    WeaponPos = L2W.GetTranslation();

    // find the target
    object* pObject;
    pObject = ObjMgr.GetObjectFromID( m_WeaponTargetID );

    if ( !pObject )
        return FALSE;

    // fix ID if necessary (per DMT)
    if( m_WeaponTargetID.Seq == -1 )
        m_WeaponTargetID.Seq = pObject->GetObjectID().Seq;

    vector3 TargetPos = pObject->GetBlendPainPoint();

    CalcSplinePts( L2W, TargetPos, Beam, NUM_BEAM_SEGS );

    // don't draw it if it's colliding with something
    vector3 ColPt;
    if ( CheckSplineCollision( s_Collider, Beam, NUM_BEAM_SEGS, ColPt, m_WeaponTargetID.GetAsS32() ) == TRUE )
    {
        return FALSE;
    }

    // success
    return TRUE;
}
