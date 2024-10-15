//==============================================================================
//
//  Projector.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "Projector.hpp"
#include "Entropy.hpp"
#include "..\AADS\Globals.hpp"
#include "GameMgr\GameMgr.hpp"
#include "NetLib\BitStream.hpp"
#include "LabelSets/Tribes2Types.hpp"

//==============================================================================
//  DEFINES
//==============================================================================


//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   ProjectorCreator;

obj_type_info   ProjectorTypeInfo( object::TYPE_PROJECTOR,
                                   "Projector", 
                                   ProjectorCreator, 
                                   object::ATTR_SOLID_DYNAMIC );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* ProjectorCreator( void )
{
    return( (object*)(new projector) );
}

//==============================================================================

object::update_code projector::OnAdvanceLogic( f32 DeltaTime )
{    
    m_Shape.Advance( DeltaTime );
    m_Logo. Advance( DeltaTime );

    m_LogoRot.Yaw += DeltaTime;

    m_TeamBits = GameMgr.GetSwitchBits( m_Switch );
    return( NO_UPDATE );
}

//==============================================================================

void projector::Initialize( const vector3& WorldPos,
                            const radian3& WorldRot,
                                  s32      Switch )
{   
    // Get the position and orientation.
    m_WorldPos = WorldPos;
    m_WorldRot = WorldRot;

    // Set the logo orientation.
    m_LogoRot.Pitch = R_0;
    m_LogoRot.Yaw   = R_0;
    m_LogoRot.Roll  = R_0;

    // Set the switch circuit.
    m_Switch = Switch;

    // Setup the shape instance.
    m_Shape.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_MISC_LOGO_PROJECTOR ) );
    m_Shape.SetPos( m_WorldPos );
    m_Shape.SetRot( m_WorldRot );

    // Setup the logo shape instance.
    m_Logo.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_MISC_LOGO_NEUTRAL ) );
    m_Logo.SetScale( vector3( 2.5f, 2.5f, 2.5f ) );
    m_Logo.SetPos( m_WorldPos + vector3(0,50,0) );
    m_Logo.SetRot( m_LogoRot );

    // Set bbox.
    m_WorldBBox = m_Shape.GetWorldBounds() + m_Logo.GetWorldBounds();
}

//==============================================================================

void projector::DebugRender( void )
{
    u32 ContextBits = 0x00;
    object::id PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );

    // Get the rendering context team bits.
    object* pObject = ObjMgr.GetObjectFromID( PlayerID );
    if( pObject )
        ContextBits = pObject->GetTeamBits();

    xcolor Color = (ContextBits & m_TeamBits) ? XCOLOR_GREEN : XCOLOR_RED;

    draw_Point( m_WorldPos, Color );
}

//==============================================================================

//                                 NEUTRAL                    RED                     GREEN
static xcolor Lo[3] = { xcolor(127,127,191,191), xcolor(191,127,127,191), xcolor(127,191,127,191) };
static xcolor Hi[3] = { xcolor(255,255,255,255), xcolor(255,191,191,255), xcolor(191,255,191,255) };

void projector::Render( void )
{
    s32 LogoShapeIndex[3] = 
    {
        SHAPE_TYPE_MISC_LOGO_NEUTRAL,
        SHAPE_TYPE_MISC_LOGO_STORM,
        SHAPE_TYPE_MISC_LOGO_INFERNO,
    };

    // Visible?
    s32 Visible = IsVisible( m_WorldBBox );
    if( !Visible )
        return;

    s32 ColorIndex = 0;

    // Update the TeamBits from the switch settings.
    m_TeamBits = GameMgr.GetSwitchBits( m_Switch );

    // Set the logo image.
    s32 Logo = (s32)m_TeamBits;
    if( m_TeamBits == 0xFFFFFFFF )
        Logo = 0;
    m_Logo.SetShape( tgl.GameShapeManager.GetShapeByType( LogoShapeIndex[Logo] ) );
    m_Logo.SetRot( m_LogoRot );

    // Get the fog color.
    xcolor FogColor = tgl.Fog.ComputeFog( m_WorldPos );
    m_Shape.SetFogColor( FogColor );
    m_Logo .SetFogColor( FogColor );

    // Decide on the color.
    xcolor Color;
    if( (m_TeamBits != 0x00000000) && 
        (m_TeamBits != 0xFFFFFFFF) )        
    {
        u32         ContextBits = 0x00000000;
        object::id  PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );
        object*     pObject = ObjMgr.GetObjectFromID( PlayerID );
        if( pObject )
        {
            ContextBits = pObject->GetTeamBits();
            if( ContextBits & m_TeamBits )    ColorIndex = 2;
            else                              ColorIndex = 1;
        }
    }   

    Color.R = x_irand( Lo[ColorIndex].R, Hi[ColorIndex].R );
    Color.G = x_irand( Lo[ColorIndex].G, Hi[ColorIndex].G );
    Color.B = x_irand( Lo[ColorIndex].B, Hi[ColorIndex].B );
    Color.A = x_irand( Lo[ColorIndex].A, Hi[ColorIndex].A );

#ifdef TARGET_PS2
    m_Logo.SetColor( Color );
#endif
#ifdef TARGET_PC
    m_Logo.SetLightAmbientColor( Color );
#endif

    // Setup draw flags.
    u32 DrawFlags = shape::DRAW_FLAG_FOG;
    if( Visible == view::VISIBLE_PARTIAL )
        DrawFlags |= shape::DRAW_FLAG_CLIP;

    // Draw.    
    m_Shape.Draw( DrawFlags );
    m_Logo.Draw ( DrawFlags );
}

//==============================================================================

void projector::OnCollide( u32 AttrBits, collider& Collider )
{
    (void)AttrBits;
    (void)Collider;
    return;
}

//==============================================================================
