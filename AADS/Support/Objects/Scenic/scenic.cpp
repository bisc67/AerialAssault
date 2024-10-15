//==============================================================================
//
//  Scenic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "Scenic.hpp"
#include "Entropy.hpp"
#include "..\AADS\Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"


//==============================================================================
//  DEFINES
//==============================================================================


//==============================================================================
//  STRUCTURES
//==============================================================================

//==============================================================================
//  DEBUG DATA
//==============================================================================

//==============================================================================
//  DEFINES
//==============================================================================

struct scenic_name
{
    s32             Label;
    const char*     pName;
};

static scenic_name ScenicName[] =
{
    {SHAPE_TYPE_SCENERY_BORG1,          "borg1"},                          
    {SHAPE_TYPE_SCENERY_BORG5,          "borg5"},                          
    {SHAPE_TYPE_SCENERY_BORG13,         "borg13"},                         
    {SHAPE_TYPE_SCENERY_BORG16,         "borg16"},                         
    {SHAPE_TYPE_SCENERY_BORG17,         "borg17"},                         
    {SHAPE_TYPE_SCENERY_BORG18,         "borg18"},                         
    {SHAPE_TYPE_SCENERY_BORG19,         "borg19"},                         
    {SHAPE_TYPE_SCENERY_BORG20,         "borg20"},                         
    {SHAPE_TYPE_SCENERY_BORG33,         "borg33"},                         
    {SHAPE_TYPE_SCENERY_BORG34,         "borg34"},                         
                                        
    {SHAPE_TYPE_SCENERY_BROCK6,         "brock6"},                         
    {SHAPE_TYPE_SCENERY_BROCK7,         "brock7"},                         
    {SHAPE_TYPE_SCENERY_BROCK8,         "brock8"},                         
    {SHAPE_TYPE_SCENERY_BROCKA,         "brocka"},                         
    {SHAPE_TYPE_SCENERY_BROCKC,         "brockc"},                         
                                        
    {SHAPE_TYPE_SCENERY_BSPIR1,         "bspir1"},                         
    {SHAPE_TYPE_SCENERY_BSPIR2,         "bspir2"},                         
    {SHAPE_TYPE_SCENERY_BSPIR3,         "bspir3"},                         
    {SHAPE_TYPE_SCENERY_BSPIR4,         "bspir4"},                         
    {SHAPE_TYPE_SCENERY_BSPIR5,         "bspir5"}, 
    
    {SHAPE_TYPE_SCENERY_BMISCF,         "bmiscf"},
                                        
    {SHAPE_TYPE_SCENERY_DORG16,         "dorg16"},                         
    {SHAPE_TYPE_SCENERY_DORG18,         "dorg18"},                         
    {SHAPE_TYPE_SCENERY_DORG19,         "dorg19"},                         
                                        
    {SHAPE_TYPE_SCENERY_DROCK8,         "drock8"},                         
                                        
    {SHAPE_TYPE_SCENERY_PORG1,          "porg1"},                          
    {SHAPE_TYPE_SCENERY_PORG2,          "porg2"},                          
    {SHAPE_TYPE_SCENERY_PORG3,          "porg3"},                          
    {SHAPE_TYPE_SCENERY_PORG5,          "porg5"},                          
    {SHAPE_TYPE_SCENERY_PORG6,          "porg6"},                          
    {SHAPE_TYPE_SCENERY_PORG20,         "porg20"},                         
                                        
    {SHAPE_TYPE_SCENERY_PROCK6,         "prock6"},                         
    {SHAPE_TYPE_SCENERY_PROCK7,         "prock7"},                         
    {SHAPE_TYPE_SCENERY_PROCK8,         "prock8"},                         
    {SHAPE_TYPE_SCENERY_PROCKA,         "procka"},                         
    {SHAPE_TYPE_SCENERY_PROCKB,         "prockb"},                         
    {SHAPE_TYPE_SCENERY_PROCKC,         "prockc"},                         
                                        
    {SHAPE_TYPE_SCENERY_PSPIR1,         "pspir1"},                         
    {SHAPE_TYPE_SCENERY_PSPIR2,         "pspir2"},                         
    {SHAPE_TYPE_SCENERY_PSPIR3,         "pspir3"},                         
    {SHAPE_TYPE_SCENERY_PSPIR4,         "pspir4"},                         
    {SHAPE_TYPE_SCENERY_PSPIR5,         "pspir5"},                         

    {SHAPE_TYPE_SCENERY_PMISCF,         "pmiscf"},                         
                                        
    {SHAPE_TYPE_SCENERY_SORG20,         "sorg20"},                         
    {SHAPE_TYPE_SCENERY_SORG22,         "sorg22"},                         
                                        
    {SHAPE_TYPE_SCENERY_SROCK6,         "srock6"},                         
    {SHAPE_TYPE_SCENERY_SROCK7,         "srock7"},                         
    {SHAPE_TYPE_SCENERY_SROCK8,         "srock8"},                         
                                        
    {SHAPE_TYPE_SCENERY_SSPIR1,         "sspir1"},                         
    {SHAPE_TYPE_SCENERY_SSPIR2,         "sspir2"},                         
    {SHAPE_TYPE_SCENERY_SSPIR3,         "sspir3"},                         
    {SHAPE_TYPE_SCENERY_SSPIR4,         "sspir4"}, 
                                                        // above: environmental
    { -123456,                          "------"},      // --------------------
                                                        // below: generic
    {SHAPE_TYPE_SCENERY_STACKABLE1L,    "stackable1l"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE2L,    "stackable2l"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE3L,    "stackable3l"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE4L,    "stackable4l"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE5L,    "stackable5l"},                    
                                        
    {SHAPE_TYPE_SCENERY_STACKABLE1M,    "stackable1m"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE2M,    "stackable2m"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE3M,    "stackable3m"},                    
                                        
    {SHAPE_TYPE_SCENERY_STACKABLE1S,    "stackable1s"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE2S,    "stackable2s"},                    
    {SHAPE_TYPE_SCENERY_STACKABLE3S,    "stackable3s"},                    
                                        
    {SHAPE_TYPE_SCENERY_STATUE_BASE,    "statue_base"},                    
    {SHAPE_TYPE_SCENERY_STATUE_HMALE,   "statue_hmale"},                   
    
    {SHAPE_TYPE_SCENERY_VEHICLE_LAND_ASSAULT_WRECK, "vehicle_land_assault_wreck"},
    {SHAPE_TYPE_SCENERY_VEHICLE_AIR_SCOUT_WRECK,    "vehicle_air_scout_wreck"},   

    {SHAPE_TYPE_SCENERY_NEXUS_BASE,     "nexusbase"},
    {SHAPE_TYPE_SCENERY_NEXUS_CAP,      "nexuscap"},

    {SHAPE_TYPE_MISC_FLAG_STAND,        "flagstand"},
};

//==============================================================================
//  STORAGE
//==============================================================================

obj_create_fn   ScenicCreator;
    
obj_type_info   ScenicTypeInfo( object::TYPE_SCENIC, 
                                "Scenic", 
                                ScenicCreator, 
                                object::ATTR_SOLID_STATIC |
                                object::ATTR_PERMANENT );

//==============================================================================
//  FUNCTIONS
//==============================================================================

object* ScenicCreator( void )
{
    return( (object*)(new scenic) );
}

//==============================================================================

scenic::scenic( void )
{
    // Put into valid state
    m_WorldPos.Zero();
    m_WorldRot.Zero();
    m_HasShape = FALSE;
    m_CollBBox.Clear() ;
}

//==============================================================================

void scenic::Initialize( const vector3& Pos,                                
                         const radian3& Rot,
                         const vector3& Scale,
                         const char*    pName )
{
    s32    i ;
    shape* pShape;
    s32    Label  = -1;
    xbool  Global = FALSE;

    for( i = 0; i < (s32)(sizeof(ScenicName) / sizeof(scenic_name)); i++ )
    {
        if( ScenicName[i].Label == -123456 )
            Global = TRUE;

        if( x_stricmp( ScenicName[i].pName, pName ) == 0 )
        {
            Label = ScenicName[i].Label;
            break;
        }
    }
    ASSERT( Label != -1 );

    // Keep orientation
    m_WorldPos = Pos;
    m_WorldRot = Rot;

    // Setup shape instance
    if( Global )    pShape = tgl.GameShapeManager.GetShapeByType(Label);
    else            pShape = tgl.EnvShapeManager. GetShapeByType(Label);

    if( pShape )
    {
        m_ShapeInstance.SetShape( pShape );
        ASSERT( m_ShapeInstance.GetShape() );

        m_ShapeInstance.SetScale( Scale );
        m_ShapeInstance.SetPos  ( m_WorldPos );
        m_ShapeInstance.SetRot  ( m_WorldRot );
        m_ShapeInstance.SetTextureAnimFPS( 0 );
        m_ShapeInstance.SetTextureFrame  ( 0 );

        // Since we are never going to move this instance anymore, declare it 
        // static for some speedup.
        m_ShapeInstance.SetFlag( shape_instance::FLAG_IS_STATIC, TRUE );

        // This code only works for 2 models max
        ASSERT(pShape->GetModelCount() <= 2) ;

        // Does shape have a collision model?
        if (pShape->GetModelCount() == 2)
        {
            // Grab tight collision bbox
            m_ShapeInstance.SetModelByIndex( 1 );
            m_CollBBox = m_ShapeInstance.GetTightWorldBounds();

            // Grab tight world bbox
            m_ShapeInstance.SetModelByIndex( 0 );
            m_WorldBBox = m_ShapeInstance.GetTightWorldBounds();

            // Make sure world bbox encloses collision bbox or object manager may incorrectly skip it
            // during collision!
            m_WorldBBox += m_CollBBox ;
        }
        else
        {
            // Use tight geometry bbox as world + collision bbox
            m_ShapeInstance.SetModelByIndex( 0 );
            m_CollBBox = m_WorldBBox = m_ShapeInstance.GetTightWorldBounds();
        }

        // Flag this scenic has a shape...
        m_HasShape = TRUE;
    }

    ComputeZoneSet( m_ZoneSet, m_WorldBBox );
}

//==============================================================================

void scenic::Render( void )
{
    // Visible?
    s32 InView = IsVisibleInZone( m_ZoneSet, m_WorldBBox ) ;
    if( !InView )
        return;

    // Setup draw flags
    u32 DrawFlags = shape::DRAW_FLAG_FOG ;
    if (InView == view::VISIBLE_PARTIAL)
        DrawFlags |= shape::DRAW_FLAG_CLIP ;

    // Is shape there?
    if( m_HasShape )
    {
		// Set self illum for environment shapes
		shape* pShape = m_ShapeInstance.GetShape() ;
		ASSERT(pShape) ;

		// Leave nexus as is
		if (		(pShape->GetType() < SHAPE_TYPE_SCENERY_NEXUS_BASE)
				||	(pShape->GetType() > SHAPE_TYPE_SCENERY_NEXUS_EFFECT) )
		{
			// Set self illum color
			static f32 s_ScenicSIA = 0.6f ;
			static f32 s_ScenicSID = 0.6f ;
			xcolor Col ;
			Col.R = (u8) MIN(255, ((f32)tgl.Direct.R * s_ScenicSID) + ((f32)tgl.Ambient.R * s_ScenicSIA) ) ;
			Col.G = (u8) MIN(255, ((f32)tgl.Direct.G * s_ScenicSID) + ((f32)tgl.Ambient.G * s_ScenicSIA) ) ; 
			Col.B = (u8) MIN(255, ((f32)tgl.Direct.B * s_ScenicSID) + ((f32)tgl.Ambient.B * s_ScenicSIA) ) ; 
			Col.A = 255 ;
			m_ShapeInstance.SetSelfIllumColor(Col) ;
		}

	    // Draw it
        vector3 Pos = m_WorldBBox.GetCenter();
        xcolor  F   = tgl.Fog.ComputeFog( Pos );
        //draw_Label( Pos, XCOLOR_WHITE, "%1d",(s32)F.A);
        m_ShapeInstance.SetFogColor(F) ;
        m_ShapeInstance.Draw(DrawFlags) ;
    }
    else
    {
        draw_BBox( m_WorldBBox, XCOLOR_YELLOW );
    }
}

//==============================================================================

void scenic::OnCollide( u32 AttrBits, collider& Collider )
{
    (void)AttrBits;

    if( m_HasShape == FALSE )
    {
        Collider.ApplyBBox( m_WorldBBox );
        return;
    }

    // SB: ANOTHER CHANGE OF PLAN
    // Use collision model if present for ray + extrusion
    // Now only the trees have collision models (without branches)
    if( m_ShapeInstance.GetShape()->GetModelCount() == 2 )
    {
        // Use collision model
        m_ShapeInstance.SetCollisionModelByIndex( 1 );

        // Collision bbox is smaller than geometry bbox so do another trivial reject
        if (!Collider.GetMovementBBox().Intersect(m_CollBBox))
            return ;
    }        
    else
        m_ShapeInstance.SetCollisionModelByIndex( 0 );


    /*
    if( Collider.GetType() == collider::RAY )
    {
        m_ShapeInstance.SetCollisionModelByIndex( 0 );
    }
    else
    {
        // SB: TEMP FOR NOW UNTIL COLLISION MODELS ARE SORTED!
        //     (SOME OF THE SPIRES COLLISION MODELS ARE TOO BIG)
        //
        //if( m_ShapeInstance.GetShape()->GetModelCount() == 2 )
        //    m_ShapeInstance.SetCollisionModelByIndex( 1 );
        //else
            m_ShapeInstance.SetCollisionModelByIndex( 0 );
    }
    */

    m_ShapeInstance.ApplyCollision( Collider );
}

//==============================================================================

object::update_code scenic::OnAdvanceLogic( f32 DeltaTime )
{
    m_ShapeInstance.Advance( DeltaTime );
    return( NO_UPDATE );
}

//==============================================================================

s32 scenic::GetShapeType( void )
{
    shape* Shape = m_ShapeInstance.GetShape() ;
    if (Shape)
        return Shape->GetType() ;
    else
        return SHAPE_TYPE_NONE ;
}

//==============================================================================


