//==============================================================================
//
//  RepairGun.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"
#include "AADS/Globals.hpp"
#include "GameMgr/GameMgr.hpp"

//==============================================================================
// DATA
//==============================================================================

static xbool    s_TexInit = FALSE ;
static s32      s_RepairGunTexIndex=-1 ;
static s32      s_PointCore1;


//==============================================================================
// DEFINES
//==============================================================================

#define NUM_BEAM_SEGS            16
#define RELOAD_SFX_DELAY_TIME    0.5f

#define REPAIRGUN_RANGE                10.0f
#define REPAIRGUN_RANGE_SQ             ( REPAIRGUN_RANGE * REPAIRGUN_RANGE )
#define REPAIRGUN_TARGET_ANGLE         R_25

//==============================================================================
// USEFUL STATE FUNCTIONS
//==============================================================================

// Advance current state
void player_object::RepairGun_AdvanceState(f32 DeltaTime)
{
    // Call advance function
    switch(m_WeaponState)
    {
        case WEAPON_STATE_ACTIVATE:     RepairGun_Advance_ACTIVATE  (DeltaTime) ; break ; 
        case WEAPON_STATE_IDLE:         RepairGun_Advance_IDLE      (DeltaTime) ; break ; 
        case WEAPON_STATE_FIRE:         RepairGun_Advance_FIRE      (DeltaTime) ; break ; 
        case WEAPON_STATE_RELOAD:       RepairGun_Advance_RELOAD        (DeltaTime) ; break ; 
        case WEAPON_STATE_DEACTIVATE:   RepairGun_Advance_DEACTIVATE    (DeltaTime) ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::RepairGun_ExitState()
{
    // Call exit state functions of current state
    switch(m_WeaponState)
    {
        case WEAPON_STATE_ACTIVATE:                                        ; break ; 
        case WEAPON_STATE_IDLE:                  RepairGun_Exit_IDLE()     ; break ; 
        case WEAPON_STATE_FIRE:                  RepairGun_Exit_FIRE()     ; break ; 
        case WEAPON_STATE_RELOAD:                                          ; break ; 
        case WEAPON_STATE_DEACTIVATE:                                      ; break ; 
    }
}

//==============================================================================

// Sets up a new state
void player_object::RepairGun_SetupState(weapon_state WeaponState)
{
    // Call setup function
    switch(WeaponState)
    {
        case WEAPON_STATE_ACTIVATE:     RepairGun_Setup_ACTIVATE        () ; break ; 
        case WEAPON_STATE_IDLE:         RepairGun_Setup_IDLE            () ; break ; 
        case WEAPON_STATE_FIRE:         RepairGun_Setup_FIRE            () ; break ; 
        case WEAPON_STATE_RELOAD:       RepairGun_Setup_RELOAD      () ; break ; 
        case WEAPON_STATE_DEACTIVATE:   RepairGun_Setup_DEACTIVATE  () ; break ; 
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

void player_object::RepairGun_Setup_ACTIVATE()
{
    // Play sound!
    if (m_ProcessInput)
    {
        audio_Play(SFX_WEAPONS_REPAIR_ACTIVATE) ;
    }
    else
    {
        audio_Play(SFX_WEAPONS_REPAIR_ACTIVATE, &m_WorldPos) ;
    }

    // Call default
    Weapon_Setup_ACTIVATE() ;
}

void player_object::RepairGun_Advance_ACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_ACTIVATE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_IDLE
//==============================================================================

void player_object::RepairGun_Setup_IDLE()
{

    // Call default
    Weapon_Setup_IDLE() ;
}

void player_object::RepairGun_Exit_IDLE()
{
}

void player_object::RepairGun_Advance_IDLE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_IDLE(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_FIRE
//==============================================================================

void player_object::RepairGun_Setup_FIRE()
{
    // Out of ammo?
    ASSERT(m_WeaponInfo) ;
    if (GetWeaponAmmoCount(m_WeaponCurrentType) < m_WeaponInfo->AmmoNeededToAllowFire)
    {
        // Play sound only for controlling player
        //if (m_ProcessInput)
            //audio_Play(SFX_WEAPONS_OUT_OF_AMMO_ALL, &m_WorldPos) ;
    }
    else
    {
        // Only create the repair_gun beam on the server
        if( tgl.ServerPresent && (m_Energy >= 0.1f) )
        {
            // find the target

            // what direction are we aiming?
            vector3 FirePos = GetWeaponFirePos() ;
            vector3 AimDir  = GetWeaponFireDir() ;

            object::id TargetID = obj_mgr::NullID;

            collider Ray;            
            Ray.RaySetup  ( this, FirePos, FirePos + (AimDir * REPAIRGUN_RANGE), m_ObjectID.GetAsS32() );
            ObjMgr.Collide( ATTR_REPAIRABLE | ATTR_SOLID_STATIC | ATTR_SOLID_DYNAMIC, Ray );
            if( Ray.HasCollided() )
            {
                object*             pObj = NULL;
                collider::collision Collision;

                Ray.GetCollision( Collision );
                pObj = (object*)Collision.pObjectHit;

                // The target must meet several requirements.

                if(     (pObj->GetAttrBits() & ATTR_REPAIRABLE)
                    &&  (pObj->GetTeamBits() & m_TeamBits)
                    &&  (pObj->GetHealth() < 1.0f)
                    &&  ((!(pObj->GetAttrBits() & ATTR_PLAYER)) || (pObj->GetHealth() > 0.0f)) )
                {
                    // If we got here, then the target is good.
                    TargetID = pObj->GetObjectID();
                }
            }

            /*
            // step one: collect nearby targetable objects within a radius
            object* pObj;

            ObjMgr.Select( object::ATTR_REPAIRABLE, m_WorldPos, REPAIRGUN_RANGE );

            // step two: iterate through the collection weeding out non-targetables
            object::id TargetID = obj_mgr::NullID;

            f32 Closest = F32_MAX ;
            
            while( (pObj = ObjMgr.GetNextSelected()) )
            {
                // Skip player if he's dead
                if ((pObj->GetAttrBits() & object::ATTR_PLAYER) && (pObj->GetHealth() <= 0))
                    continue ;

                // Skip enemy items
                if( (pObj->GetTeamBits() & m_TeamBits) == 0x00 )
                    continue;

                // don't bother computing ATAN if outside of range
                vector3 TarDir = ( pObj->GetPosition() - FirePos );
                if ( TarDir.LengthSquared() < Closest )
                {
                    radian Dif = v3_AngleBetween( AimDir, TarDir );

                    // is this target in angular range
                    if ( Dif < REPAIRGUN_TARGET_ANGLE )
                    {
                        Closest  = TarDir.LengthSquared();                        
                        TargetID = pObj->GetObjectID() ;
                    }
                }
            }
            ObjMgr.ClearSelection();
            */

            // Repair self?
            if ((TargetID == obj_mgr::NullID) && (m_Health < 100) && (m_Health > 0))
                TargetID = m_ObjectID ;

            // Set new target
            SetWeaponTargetID(TargetID) ;

            // Could not fire, revert to idle mode and return
            if ( TargetID == obj_mgr::NullID )
            {
                Weapon_SetupState( WEAPON_STATE_IDLE );
                return;
            }
            else
            {
                // apply soothing energy to the target object
                object* pObject = ObjMgr.GetObjectFromID( TargetID );

                if ( pObject )
                {
                    f32 Health = pObject->GetHealth();
                    if( Health >= 1.0f )
                    {
                        // all done repairing?
                        Weapon_SetupState( WEAPON_STATE_IDLE );
                        SetWeaponTargetID( obj_mgr::NullID ) ;
                        return;
                    }
                    else
                    {
                        pGameLogic->ItemRepairing( TargetID, m_ObjectID );
                    }
                }
            }
        }
    }

    // start the anim playing
    m_WeaponInstance.SetAnimByType(ANIM_TYPE_WEAPON_FIRE, 0.1f) ;
    m_WeaponInstance.GetCurrentAnimState().SetSpeed( 1.0f );

    // don't start the sound playing until we know we can draw the beam
    m_WeaponSfxID = -1;
}

void player_object::RepairGun_Advance_FIRE(f32 DeltaTime)
{
    (void)DeltaTime;

    vector3 FirePos = GetWeaponFirePos() ;
    vector3 AimDir  = GetWeaponFireDir() ;
    object* pObject = NULL;

    if( m_WeaponTargetID == m_ObjectID )
    {
        if( (m_Health >= 0.0f) && (m_Health < 100.0f) )
        {
            AddEnergy( m_MoveInfo->RECHARGE_RATE * DeltaTime * -2.0f );

            ObjMgr.InflictPain( pain::REPAIR_VIA_PACK,  // PainType
                                GetPosition(),          // Position of pain
                                m_ObjectID,             // OriginID of pain
                                m_WeaponTargetID,       // VictimID of pain
                                DeltaTime );            // Scalar (time)
        }
        else
        {
            goto exit_and_clear_target;
        }
    }
    else
    if( pObject = ObjMgr.GetObjectFromID( m_WeaponTargetID ) )
    {
        // Revalidate target.

        // what direction are we aiming?
        vector3 FirePos = GetWeaponFirePos() ;
        vector3 AimDir  = GetWeaponFireDir() ;

        object::id TargetID = obj_mgr::NullID;

        collider Ray;            
        Ray.RaySetup  ( this, FirePos, FirePos + (AimDir * REPAIRGUN_RANGE), m_ObjectID.GetAsS32() );
        ObjMgr.Collide( ATTR_REPAIRABLE | ATTR_SOLID_STATIC | ATTR_SOLID_DYNAMIC, Ray );
        if( Ray.HasCollided() )
        {
            object*             pObj = NULL;
            collider::collision Collision;

            Ray.GetCollision( Collision );
            pObj = (object*)Collision.pObjectHit;

            // The target must meet several requirements.

            if(     (pObj->GetAttrBits() & ATTR_REPAIRABLE)
                &&  (pObj->GetTeamBits() & m_TeamBits)
                &&  (pObj->GetHealth() < 1.0f)
                &&  ((!(pObj->GetAttrBits() & ATTR_PLAYER)) || (pObj->GetHealth() > 0.0f)) )
            {
                // If we got here, then the target is good.
                TargetID = pObj->GetObjectID();
            }
            else
            {
                TargetID = obj_mgr::NullID;
            }
        }

        if( m_WeaponTargetID == TargetID )
        {
            // Target is still good.
            AddEnergy( m_MoveInfo->RECHARGE_RATE * DeltaTime * -1.2f );
            ObjMgr.InflictPain( pain::REPAIR_VIA_PACK,  // PainType
                                pObject->GetPosition(), // Position of pain
                                m_ObjectID,             // OriginID of pain
                                m_WeaponTargetID,       // VictimID of pain
                                DeltaTime );            // Scalar (time)
        }
        else
            goto exit_and_clear_target;

        /*
        vector3 TarDir = ( pObject->GetPosition() - FirePos );
        //radian Dif = v3_AngleBetween( AimDir, TarDir);

        if ( pObject != this )
        {
        //  if ( ( Dif > REPAIRGUN_TARGET_ANGLE ) || ( TarDir.LengthSquared() > REPAIRGUN_RANGE_SQ ) )
            if ( ( TarDir.LengthSquared() > REPAIRGUN_RANGE_SQ ) )
                goto exit_and_clear_target;
        }

        // Calculate the Beam...
        if ( Elf_CalculateBeam( Beam, FALSE ) == TRUE )
        {
            // apply soothing energy to the target object
            ObjMgr.InflictPain( pain::REPAIR_VIA_PACK,  // PainType
                                pObject->GetPosition(), // Position of pain
                                m_ObjectID,             // OriginID of pain
                                m_WeaponTargetID,       // VictimID of pain
                                DeltaTime );            // Scalar (time)
            f32 Health = pObject->GetHealth();

            if( (Health < 0.0f) || (Health == 1.0f) )
                goto exit_and_clear_target;
        }
        else
            goto exit_and_clear_target;
        */
    }
    else
    {
        // revert to no target
        goto exit_and_clear_target;
    }

    // Released fire?
    if (!m_Move.FireKey)
    {
        /*
        Weapon_SetupState( WEAPON_STATE_IDLE );
        return ;
        */
        goto exit_and_clear_target;
    }

    if( m_Energy <= 0.0f )
    {
        goto exit_and_clear_target;
    }

    /*
    // play the audio in the world for all to hear
    if ( !audio_IsPlaying( m_WeaponSfxID ) )
        audio_PlayLooping( m_WeaponSfxID, SFX_WEAPONS_REPAIR_USE, &m_WorldPos ) ;
    */

    return;

exit_and_clear_target:
	m_DisabledInput.FireKey = TRUE ;
	m_Move.FireKey		    = FALSE ;
    SetWeaponTargetID(obj_mgr::NullID) ;
    Weapon_SetupState( WEAPON_STATE_RELOAD );
}

void player_object::RepairGun_Exit_FIRE( )
{
    // Stop sound
    audio_StopLooping( m_WeaponSfxID );
}

//==============================================================================
// WEAPON_STATE_RELOAD
//==============================================================================

void player_object::RepairGun_Setup_RELOAD()
{
    // Call default
    Weapon_Setup_RELOAD() ;
}

void player_object::RepairGun_Advance_RELOAD(f32 DeltaTime)
{

    // Call default
    Weapon_Advance_RELOAD(DeltaTime) ;
}

//==============================================================================
// WEAPON_STATE_DEACTIVATE
//==============================================================================

void player_object::RepairGun_Setup_DEACTIVATE()
{
    // Call default
    Weapon_Setup_DEACTIVATE() ;
}

void player_object::RepairGun_Advance_DEACTIVATE(f32 DeltaTime)
{
    // Call default
    Weapon_Advance_DEACTIVATE(DeltaTime) ;
}

//==============================================================================
void CalcSplinePts( const matrix4& Start, const vector3& End, vector3* pPoints, s32 NumPoints );
xbool CheckSplineCollision( vector3* pPts, s32 NumPts, vector3& ColPt, s32 Exclude );

void player_object::RepairGun_RenderFx( void )
{
    // A pseudo-delta time
    f32 TheTime = (f32)(tgl.LogicTimeMs % 4000) / 1000.0f;
    f32 RenderTime = (tgl.LogicTimeMs % 1000) / 1000.0f ; //1.0f - ( TheTime - (u32)TheTime );
    f32 RenderTime2 = TheTime / 4.0f; //(f32)( TheTime - (((u32)TheTime / 4) * 4.0f) ) / 4.0f;

    // Lookup textures
    if (!s_TexInit)
    {
        s_TexInit = TRUE ;
        s_RepairGunTexIndex = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]Beam") ;  // not extension not needed!
        s_PointCore1 = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]Spot") ;  // not extension not needed! 
    }

    // Only draw when firing and you have a valid target
    if ( (m_WeaponState == WEAPON_STATE_FIRE) && ( m_WeaponTargetID != obj_mgr::NullID ) )
    {
        // Draw beam...
        vector3 Beam[NUM_BEAM_SEGS];

        // if we can't draw it, just return
        if ( Elf_CalculateBeam( Beam, TRUE ) == FALSE )
            return;
        
        // First, draw the hot points at the beginning and end
        draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );
#ifdef TARGET_PS2
        gsreg_Begin();
        gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
        gsreg_SetZBufferUpdate(FALSE);
        gsreg_SetClamping( FALSE );
        gsreg_End();
#endif

        draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_PointCore1).GetXBitmap() );

        draw_SpriteUV( Beam[0], vector2(0.25f, 0.25f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
        draw_SpriteUV( Beam[0], vector2(0.25f, 0.25f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360 );

        tgl.PostFX.AddFX( Beam[0], vector2(1.0f, 1.0f), xcolor(255,225,225,38) );

        if ( m_WeaponTargetID != GetObjectID() )
        {
            draw_SpriteUV( Beam[NUM_BEAM_SEGS-1], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, RenderTime2 * R_360 );
            draw_SpriteUV( Beam[NUM_BEAM_SEGS-1], vector2(0.5f, 0.5f), vector2(0,0), vector2(1,1), XCOLOR_WHITE, -RenderTime2 * R_360 );
        }

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
        vector3 tmp = (Beam[1] - Beam[0]);
        f32 SegLen = tmp.Length() * 0.25f;

        // Activate the core texture 
        if ( m_WeaponTargetID != GetObjectID() )
        {
            draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_RepairGunTexIndex).GetXBitmap() );
            draw_OrientedStrand (Beam,
                                 NUM_BEAM_SEGS,
                                 vector2(RenderTime,0), vector2( SegLen+RenderTime, 1),                                 
                                 XCOLOR_WHITE, XCOLOR_WHITE,                                       
                                 0.10f );
        }

        draw_End();

#ifdef TARGET_PS2
        gsreg_Begin();
        gsreg_SetZBufferUpdate(TRUE);
        gsreg_End();
#endif
    }
}

//==============================================================================
