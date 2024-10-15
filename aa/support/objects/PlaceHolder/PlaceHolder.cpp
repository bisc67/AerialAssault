//==============================================================================
//
//  PlaceHolder.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlaceHolder.hpp"
#include "Entropy.hpp"
#include "NetLib\BitStream.hpp"
#include "..\Demo1\Globals.hpp"

//==============================================================================
//  DEFINES
//==============================================================================


//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   PlaceHolderCreator;

obj_type_info   PlaceHolderTypeInfo( object::TYPE_PLACE_HOLDER, 
                                     "PlaceHolder", 
                                     PlaceHolderCreator, 
                                     object::ATTR_SOLID_STATIC );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* PlaceHolderCreator( void )
{
    return( (object*)(new place_holder) );
}

//==============================================================================

object::update_code place_holder::OnAdvanceLogic( f32 DeltaTime )
{    
    m_LabelTimer += DeltaTime;
    return( NO_UPDATE );
}

//==============================================================================

void place_holder::Initialize( const vector3& WorldPos,
                               const radian3& WorldRot,
                               const char*    pLabel )
{   
    // Set the L2W.
    m_L2W.Identity();
    m_L2W.SetRotation   ( WorldRot );
    m_L2W.SetTranslation( WorldPos );

    // Set position and bbox.
    m_WorldPos  = WorldPos;
    m_WorldBBox( vector3(-1,0,-1), vector3(1,2,1) );
    m_WorldBBox.Transform( m_L2W );

    // Set the label.
    if( pLabel )
        x_strncpy( m_Label, pLabel, 63 );
    else
        x_strcpy( m_Label, "PlaceHolder" );
    m_LabelTimer = 0.0f;
}

//==============================================================================

void place_holder::DebugRender( void )
{
    Render();
}

//==============================================================================

void place_holder::Render( void )
{
    // Is bbox in view?
    {
        const view *pView = &tgl.GetView();
        if( pView && (pView->BBoxInView( m_WorldBBox ) == FALSE) )
            return;
    }

    bbox BBox( vector3(-1,0,-1), vector3(1,2,1) );

    draw_ClearL2W();

    if( m_LabelTimer < 5.0f )
    {
        draw_BBox ( m_WorldBBox, XCOLOR_RED );
        draw_Label( m_WorldPos,  XCOLOR_YELLOW, m_Label );
    }
    else
    {
        draw_Point( m_WorldPos,  XCOLOR_BLUE   );
        draw_BBox ( m_WorldBBox, XCOLOR_YELLOW );
    }

    draw_SetL2W( m_L2W );
    draw_BBox  ( BBox, XCOLOR_BLUE );
    draw_Axis  ( 1.0f );
}

//==============================================================================

object::update_code place_holder::OnAcceptUpdate( const bitstream& BitStream, f32 SecInPast )
{
    (void)BitStream;
    (void)SecInPast;
    return( NO_UPDATE );
}

//==============================================================================

void place_holder::OnProvideUpdate( bitstream& BitStream, u32& DirtyBits, f32 Priority )
{
    (void)BitStream;
    (void)Priority;

    DirtyBits = 0;
}

//==============================================================================

void place_holder::OnAcceptInit( const bitstream& BitStream )
{
    BitStream.ReadMatrix4( m_L2W );
    BitStream.ReadString( m_Label );

    m_WorldPos = m_L2W.GetTranslation();
    m_WorldBBox( vector3(-1,0,-1), vector3(1,2,1) );
    m_WorldBBox.Transform( m_L2W );

    m_LabelTimer = 0.0f;
}

//==============================================================================

void place_holder::OnProvideInit( bitstream& BitStream )
{
    BitStream.WriteMatrix4( m_L2W );
    BitStream.WriteString( m_Label );
}

//==============================================================================

void place_holder::OnCollide( u32 AttrBits, collider& Collider )
{
    object::OnCollide( AttrBits, Collider );
    m_LabelTimer = 0.0f;
}

//==============================================================================
