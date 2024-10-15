//==============================================================================
//
//  Laser.cpp
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
#include "netlib/bitstream.hpp"


#include "Objects\Player\PlayerObject.hpp"
#include "PointLight\PointLight.hpp"

#include "Laser.hpp"
#include "Textures.hpp"

//==============================================================================
//  DEFINES
//==============================================================================

#define ANIM_TEXTURE_LEN        32.0f
#define LASER_LIFE              0.25f


//==============================================================================
//  STATICS
//==============================================================================

//static    s32       CoreTex;        // VRAM texture ID for the core
//static    xbitmap   CoreBmp;

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   LaserCreator;

obj_type_info   LaserClassInfo( object::TYPE_LASER, 
                                "Laser", 
                                LaserCreator,
                                object::ATTR_GLOBAL );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* LaserCreator( void )
{
    return( (object*)(new laser) );
}

//==============================================================================

void laser::Init( void )
{
    // Load the textures.
    //CoreTex = LoadProjTexture( CoreBmp, "LaserRifle/[A]Laser" );
    // CoreBmp.BuildMips();
}

//==============================================================================

void laser::Kill( void )
{
    //vram_Unregister( CoreBmp );
    //CoreBmp.Kill();
}

//==============================================================================

void laser::Initialize( const vector3&   Start,    
                              f32        Energy,    
                              u32        TeamBits,  
                              object::id OriginID )
{
    radian3         VRot;
    f32             VFOV;
    vector3         VZ, VP;

    player_object* pPlayer = (player_object*)ObjMgr.GetObjectFromID( OriginID );
    ASSERT( pPlayer );

    pPlayer->GetView( VP, VRot, VFOV );
    VZ.Set( VRot.Pitch, VRot.Yaw );

    m_Zoom = (s32)pPlayer->GetCurrentZoomFactor() * 10;

    m_End = VP + (VZ * 800.0f);

    // Look for collision.  Test everything the laser could possibly hit.
    collider            Ray;
    collider::collision Collision; 
    Ray.RaySetup    ( this, VP, m_End, OriginID.GetAsS32() );
    ObjMgr.Collide  ( object::ATTR_SOLID | object::ATTR_DAMAGEABLE, Ray );
    Ray.GetCollision( Collision );

    m_Hit = Collision.Collided;

    m_ColNorm(0,1,0);

    // Collided?
    if( m_Hit )
    {
        m_End      = Collision.Point;
        m_VictimID = ((object*)(Collision.pObjectHit))->GetObjectID();
        m_ColNorm  = Collision.Plane.Normal;
        m_Length   = 800.0f * Collision.T;

        // check to see if it was a head shot
        if ( Collision.NodeType == NODE_TYPE_CHARACTER_HEAD )
        {
            m_WasHeadShot = TRUE;
            //x_printf("Head Shot!\n");
        }
    }
    else
    {
        m_Length = 800.0f;
    }

    // shoot another ray from the gun barrel to see if it hits anything closer
    Ray.RaySetup    ( this, Start, m_End, OriginID.GetAsS32() );
    ObjMgr.Collide  ( object::ATTR_SOLID, Ray );
    Ray.GetCollision( Collision );

    if( Collision.Collided )
    {
        f32 Len = m_Length * Collision.T;

        if( Len < m_Length )
        {
            // hit the closer object
            m_End      = Collision.Point;
            m_VictimID = ((object*)(Collision.pObjectHit))->GetObjectID();
            m_ColNorm  = Collision.Plane.Normal;
            m_Length   = Len;
        }
    }

    m_Start = Start;

    m_Dir = (m_End - m_Start);
    m_Dir.Normalize();

    m_WorldPos  = Start;
    m_WorldBBox = Start;
    m_OriginID  = OriginID;
    m_ExcludeID = OriginID;
    m_DirtyBits = 0x00;
    m_Age       = 0.0f;
    m_TeamBits  = TeamBits;

    // Setup energy.
    m_Energy = Energy / 100.0f;
}

//==============================================================================

void laser::OnAcceptInit( const bitstream& BitStream )
{
    BitStream.ReadVector( m_Start  );
    BitStream.ReadVector( m_End    );
    BitStream.ReadUnitVector( m_ColNorm, 28 );
    BitStream.ReadF32   ( m_Energy );

    m_WorldPos  = m_Start;
    m_WorldBBox = m_Start;
    m_Age       = 0.0f;
    m_Length    = (m_End - m_Start).Length();
    m_Zoom      = 100;

    m_Dir = (m_End - m_Start);
    m_Dir.Normalize();
}

//==============================================================================

void laser::OnProvideInit( bitstream& BitStream )
{
    BitStream.WriteVector( m_Start  );
    BitStream.WriteVector( m_End    );
    BitStream.WriteUnitVector( m_ColNorm, 28 );
    BitStream.WriteF32   ( m_Energy );
}

//==============================================================================

void laser::OnAdd( void )
{
    // Setup pointlight.
    vector3 Flash;
    Flash = m_End + ( m_ColNorm * 0.03f );
    ptlight_Create( m_Start, 2.00f, xcolor(255,0,0,64), 0.0f, 0.2f, 0.0f );
    ptlight_Create( Flash,   2.00f, xcolor(255,0,0,(u8)(m_Energy * 64.0f)), 0.0f, 0.2f, 0.0f );
    ptlight_Create( Flash,   0.25f, xcolor(255,0,0,(u8)(m_Energy * 192.0f)), 0.10f, 0.2f, m_Energy * 4.0f );

    // Determine if a local player fired this blaster
    xbool Local = FALSE;
    object* pCreator = ObjMgr.GetObjectFromID( m_OriginID );
    if( pCreator && (pCreator->GetType() == object::TYPE_PLAYER) )
    {
        player_object* pPlayer = (player_object*)pCreator;
        Local = pPlayer->IsLocal();
    }

    // play the firing sound
    if( Local )
        audio_Play( SFX_WEAPONS_SNIPERRIFLE_FIRE );
    else
        audio_Play( SFX_WEAPONS_SNIPERRIFLE_FIRE, &m_Start );

    if( m_Hit )
    {
        SpawnExplosion( m_WasHeadShot 
                            ? pain::WEAPON_LASER_HEAD_SHOT 
                            : pain::WEAPON_LASER,
                        m_End, 
                        m_ColNorm,
                        m_OriginID, 
                        m_VictimID,
                        m_Energy );
    }
}

//==============================================================================

void laser::BeginRender( void )
{
    draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_SetClamping( FALSE );
    gsreg_End();
#endif
//    draw_SetTexture( CoreBmp );
}

//==============================================================================

void laser::EndRender( void )
{
    draw_End();

#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
#endif
}

//==============================================================================

void laser::Render( void )
{
    if( m_Age < LASER_LIFE )
    {
        f32     u1;
        f32     Alpha;
        xcolor  Color = XCOLOR_WHITE;

        // calculate the UVStart based on age
        u1 = 1.0f - (m_Age / LASER_LIFE);
        
        // calc the alpha
        Alpha = MIN( (u1 * 1.5f), 1.0f );
        Color.A = (u8)(255 * Alpha * m_Energy);

        // speed up the texture crawl
        u1 *= 2.0f;

        // draw the beam
        DrawBeam( u1, m_Zoom, Color, Color, 0.05f, 64 );

        // draw the glow (slightly infront of the collision so it always appears)
        tgl.PostFX.AddFX( m_End + ( m_ColNorm * 0.03f ), vector2(2.0f, 2.0f), xcolor(255,255,255,38), 0.25f );
    }
}

//==============================================================================

object::update_code laser::OnAdvanceLogic( f32 DeltaTime )
{
    m_Age += DeltaTime;
    if( m_Age > LASER_LIFE )
        return( DESTROY );
    else
        return( NO_UPDATE );
}

//==============================================================================

void laser::DrawBeam    (  f32      UVStart,
                           s32      UVLen, 
                           xcolor   Color0,
                           xcolor   Color1,
                           f32      Radius,
                           s32      TWidth )
{
    s32         SegLen, UMax, nLegs;
    vector3     SegVec;
    f32         UStart, UEnd;

    ASSERT( TWidth > 0 );

    vector3 CrossDir = m_Dir.Cross( eng_GetActiveView(0)->GetPosition() - m_Start );
    CrossDir.Normalize();
    vector3 Cross = CrossDir * Radius;

    // calc the segment length
    UMax = 2048 / TWidth;

    SegLen = (s32)(UVLen * UMax);

    // SegLen *= UVLen;

    if ( SegLen > 0 )
        nLegs = (s32)(( m_Length / SegLen ) + 1);
    else
        nLegs = 1;

    // setup tmp vectors
    vector3 TmpStart, TmpEnd;
    f32     TmpLen;

    TmpStart = m_Start;
    TmpLen = m_Length;
    SegVec = m_Dir * (f32)SegLen;

    UStart = UVStart;

    for ( s32 i = 0; i < nLegs; i++ )
    {
        if ( TmpLen > SegLen )
        {
            TmpEnd = TmpStart + SegVec;
            UEnd = (f32)UMax + UVStart;
        }
        else
        {
            TmpEnd = m_End;
            UEnd = (( TmpLen / SegLen ) * UMax ) + UVStart;
        }

        draw_Color( Color1 );
        draw_UV( UEnd, 1.0f );    draw_Vertex( TmpEnd + Cross );
        draw_UV( UEnd, 0.0f );    draw_Vertex( TmpEnd - Cross );
        draw_Color( Color0 );
        draw_UV( UStart, 0.0f );  draw_Vertex( TmpStart - Cross );
        draw_UV( UStart, 0.0f );  draw_Vertex( TmpStart - Cross );
        draw_UV( UStart, 1.0f );  draw_Vertex( TmpStart + Cross );
        draw_Color( Color1 );
        draw_UV( UEnd, 1.0f );    draw_Vertex( TmpEnd + Cross );

        // adjust the length for the next seg
        TmpLen -= SegLen;
        TmpStart = TmpEnd;
    }
}

//==============================================================================
