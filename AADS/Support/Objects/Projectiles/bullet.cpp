//==============================================================================
//
//  Bullet.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ParticleObject.hpp"
#include "Entropy.hpp"
#include "..\AADS\Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"

#include "Objects\Player\PlayerObject.hpp"
#include "pointlight\pointlight.hpp"

#include "Bullet.hpp"
#include "textures.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

#define TWEAK   1.25f

//==============================================================================
//  STATICS
//==============================================================================

//static    s32       BulletTex;        // VRAM texture ID for the core
//static    xbitmap   BulletBmp;

//==============================================================================
//  GLOBAL INITIALIZATION (CALLED ONLY ONCE EVER)
//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   BulletCreator;

obj_type_info   BulletClassInfo( object::TYPE_BULLET, 
                                 "Bullet", 
                                 BulletCreator, 
                                 0 );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* BulletCreator( void )
{
    return( (object*)(new bullet) );
}

//==============================================================================

void bullet::Init( void )
{
    // load the textures
    //BulletTex = LoadProjTexture( BulletBmp, "Bullet/[A]Bullet" );
}

//==============================================================================

void bullet::Kill( void )
{
    //vram_Unregister( BulletBmp );
    //BulletBmp.Kill();
}

//==============================================================================

void bullet::Setup( void )
{
    const player_object::weapon_info& WeaponInfo
        = player_object::GetWeaponInfo(
            player_object::INVENT_TYPE_WEAPON_CHAIN_GUN );
    m_Speed = WeaponInfo.MuzzleSpeed;
    m_Life  = WeaponInfo.MaxAge;
    
    m_SoundID = SOUNDTYPE_NONE;
}

//==============================================================================

f32 BULLET_AIMING_TWEAK = 7.5f;

void bullet::Initialize( const matrix4&   L2W, 
                               u32        TeamBits, 
                               object::id OriginID,
                               object::id ExcludeID )
{
    Setup();

    // call default initializer
    linear::Initialize( L2W, TeamBits, OriginID, ExcludeID );

    // add in randomness to angle
    m_Movement.X += x_frand( -BULLET_AIMING_TWEAK, +BULLET_AIMING_TWEAK );
    m_Movement.Y += x_frand( -BULLET_AIMING_TWEAK, +BULLET_AIMING_TWEAK );
    m_Movement.Z += x_frand( -BULLET_AIMING_TWEAK, +BULLET_AIMING_TWEAK );

    m_Tracer = m_Movement;
    m_Tracer.Normalize();
}

//==============================================================================

void bullet::OnAcceptInit( const bitstream& BitStream )
{
    Setup();

    // call default OnAcceptInit
    linear::OnAcceptInit( BitStream );

    m_Tracer = m_Movement;
    m_Tracer.Normalize();
}

//==============================================================================

void bullet::BeginRender( s32 Stage )
{
    /*
    switch( Stage )
    {
        case 0:
            draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            break;

        case 1:
            draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
            break;

        default:
            ASSERT( FALSE );
    }
    
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend(ALPHA_BLEND_ADD, 64);
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_End();
#endif
    draw_SetTexture( BulletBmp );
    */
}

//==============================================================================

void bullet::EndRender( void )
{
    /*
    draw_End();

#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
#endif
    */
}

//==============================================================================

void bullet::Render( s32 Stage )
{
    /*
    vector3 Pos = GetRenderPos();

    xcolor C0 = XCOLOR_WHITE;
    xcolor C1 = XCOLOR_WHITE;

    switch( Stage )
    {
        case 0:
            // render core
            draw_SpriteUV( Pos, 
                           vector2( 0.10f, 0.1f ), 
                           vector2( 0.66f, 0.0f ), 
                           vector2( 1.00f, 1.0f ), 
                           XCOLOR_WHITE );
            break;

        case 1:

            // render tracer once we've reached a certain distance
            if( m_Age > 0.01f )
            {
                f32 Len;

                if( m_Age < 0.15f )
                    Len = (1.0f - (m_Age / 0.15f)) * 5.0f + 1.0f;
                else
                    Len = 1.0f;

                draw_OrientedQuad( Pos - (m_Tracer * Len), 
                                   Pos + (m_Tracer * Len), 
                                   vector2(0.66f,0), 
                                   vector2(0.0f,1.0f), 
                                   C0, C1, 0.06f );
            }
            break;
        
        default:
            ASSERT( FALSE );
    }
    */
}

//==============================================================================

object::update_code bullet::Impact( vector3& Point, vector3& Normal )
{
    SpawnExplosion( pain::WEAPON_CHAINGUN,
                    Point + (Normal * 0.01f), 
                    Normal,
                    m_OriginID, 
                    m_ImpactObjectID );
    return( DESTROY );
}

//==============================================================================
