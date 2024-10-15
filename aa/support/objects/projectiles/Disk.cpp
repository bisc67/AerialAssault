//==============================================================================
//
//  Disk.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "Disk.hpp"
#include "ParticleObject.hpp"
#include "Entropy.hpp"
#include "..\Demo1\Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "AudioMgr/Audio.hpp"

#include "objects\player\playerobject.hpp"
#include "pointlight\pointlight.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

#define POINT_LIGHT_RADIUS  4.0f
#define POINT_LIGHT_COLOR   xcolor(63,63,255,128)

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   DiskCreator;

obj_type_info   DiskTypeInfo( object::TYPE_DISK, 
                              "Disk", 
                              DiskCreator, 
                              0 );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* DiskCreator( void )
{
    return( (object*)(new disk) );
}

//==============================================================================

void disk::Setup( void )
{
    const player_object::weapon_info& WeaponInfo
        = player_object::GetWeaponInfo(
            player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER );
    m_Speed   = WeaponInfo.MuzzleSpeed;
    m_Life    = WeaponInfo.MaxAge;

    m_SoundID = SFX_WEAPONS_SPINFUSOR_PROJECTILE_LP;

    m_Core   .SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_PROJ_DISC_CORE   ) );
    m_Effects.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_PROJ_DISC_EFFECT ) );
}

//==============================================================================

void disk::Initialize( const matrix4& L2W, u32 TeamBits, object::id OriginID )
{
    Setup();

    // call default initializer
    linear::Initialize( L2W, TeamBits, OriginID );
}

//==============================================================================

void disk::OnAdd( void )
{
    linear::OnAdd();
    vector3 Pos = GetRenderPos();

    // Determine if a local player fired this blaster
    xbool Local = FALSE;
    object* pCreator = ObjMgr.GetObjectFromID( m_OriginID );
    if( pCreator && (pCreator->GetType() == object::TYPE_PLAYER) )
    {
        player_object* pPlayer = (player_object*)pCreator;
        Local = pPlayer->IsLocal();
    }

    // play firing sound
    if( Local )
        audio_Play( SFX_WEAPONS_SPINFUSORFIRE02MIX01 );
    else
        audio_Play( SFX_WEAPONS_SPINFUSORFIRE02MIX01, &Pos, AUDFLAG_LOCALIZED );

    // Setup point light
    m_PointLightID = ptlight_Create( Pos, POINT_LIGHT_RADIUS, POINT_LIGHT_COLOR );
}

//==============================================================================

void disk::OnAcceptInit( const bitstream& BitStream )
{
    Setup();

    // call default OnAcceptInit
    linear::OnAcceptInit( BitStream );
}

//==============================================================================

object::update_code disk::Impact( vector3& Point, vector3& Normal )
{
    SpawnExplosion( pain::WEAPON_DISC,
                    Point, 
                    Normal,
                    m_OriginID,
                    m_ImpactObjectID );

    // Remove disk from world.
    return( DESTROY );
}

//==============================================================================

void disk::RenderCore( void )
{
    if( m_Hidden )
        return;

    vector3 Pos = GetRenderPos();

    m_Core.SetPos( Pos );
    m_Core.SetRot( m_Orient.GetRotation() );
    m_Core.SetFogColor( tgl.Fog.ComputeFog(Pos) );
    m_Core.Draw( shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_TURN_OFF_LIGHTING ) ;
    tgl.PostFX.AddFX( Pos, vector2(2.0f, 2.0f), xcolor(225,225,255,38) );
}

//==============================================================================

xbool OLD_DISC_RENDER = FALSE;

void disk::RenderEffects( void )
{
    if( m_Hidden )
        return;

    vector3 Pos = GetRenderPos();

    if( !OLD_DISC_RENDER )
    {
        f32 Scale = 1.0f;

        if( m_Age == 0.0f )
            return;            

        if( m_Age > 4.0f )
            Scale = MAX( 5.0f - m_Age, 0.0f );

        vector3 Pos = GetRenderPos();

        m_Effects.SetScale( vector3(Scale,Scale,Scale) );
        m_Effects.SetPos( Pos );
        m_Effects.SetRot( m_Orient.GetRotation() );
        m_Effects.SetFogColor( tgl.Fog.ComputeFog(Pos) );

        m_Effects.Draw( shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_TURN_OFF_LIGHTING );
    }
    else
    {
        vector3 Point[6] = { vector3(  0.0f,  0.0f,  0.2f ),
                             vector3(  0.0f,  0.1f,  0.0f ),
                             vector3( -0.2f,  0.0f,  0.0f ),
                             vector3(  0.0f, -0.1f,  0.0f ),
                             vector3(  0.2f,  0.0f,  0.0f ),
                             vector3(  0.0f,  0.0f,-20.0f ) };

        xcolor  Color[6] = { xcolor(  63,  63, 255, 127 ), 
                             xcolor( 127, 127, 255,  63 ), 
                             xcolor( 127, 127, 255,  63 ), 
                             xcolor( 127, 127, 255,  63 ), 
                             xcolor( 127, 127, 255,  63 ), 
                             xcolor( 255, 255, 255,   0 ) };

        s16 Index[24]    = { 0,1,2,  0,2,3,  0,3,4,  0,4,1,  
                             5,2,1,  5,3,2,  5,4,3,  5,1,4 };

        // Set the tail length.  Max is 10.
        if( m_Age < 0.1f )
        {
            Point[5].Z = -(m_Age * 200.0f);
        }

        matrix4 M = m_Orient;
        M.SetTranslation( Pos );

        draw_Begin   ( DRAW_TRIANGLES, DRAW_USE_ALPHA );
        draw_SetL2W  ( M ); 
        draw_Verts   ( Point,  6 );
        draw_Colors  ( Color,  6 );
        draw_Execute ( Index, 24 );
        draw_End     ();
        draw_ClearL2W();
    }
}

//==============================================================================
