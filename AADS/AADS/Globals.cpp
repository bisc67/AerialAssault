//=========================================================================
//
//  Globals.cpp
//
//=========================================================================
#include "globals.hpp"
#include "Building\BldManager.hpp"

t2_globals  tgl;

//=========================================================================

// Returns TRUE if object is occluded by the terrain (ie. do not draw)
xbool IsOccludedByTerrain( const bbox& BBox, const vector3& EyePos )
{
    ASSERT( tgl.pTerr );
    return tgl.pTerr->IsOccluded(BBox,EyePos);
}

//=========================================================================

// Performs view volume test only
// Returns:
//  view::VISIBLE_NONE    (0) = Not visible
//  view::VISIBLE_FULL    (1) = Visible (no clipping needed)
//  view::VISIBLE_PARTIAL (2) = Partially visible (ie. check for clipping)
s32 IsInViewVolume( const bbox& BBox )
{
    // Get current view
    const view* pView = eng_GetActiveView(0);
    ASSERT(pView) ;

    // If sky is fogged then check if we are completely fogged
    //if(tgl.Fog.GetVisDistance()<300.0f)
    {
        f32 D2 = (pView->GetPosition() - BBox.GetCenter()).LengthSquared();
        f32 D = (tgl.Fog.GetVisDistance()+20.0f);
        if( D2 > D*D )
        {
            //draw_Point( m_WorldPos, XCOLOR_YELLOW );
            return view::VISIBLE_NONE ;
        }
    }

    // In view volume?
    return pView->BBoxInView( BBox ) ;
}

//=========================================================================

// Performs view volume and occlusion test
// Returns:
//  view::VISIBLE_NONE    (0) = Not visible
//  view::VISIBLE_FULL    (1) = Visible (no clipping needed)
//  view::VISIBLE_PARTIAL (2) = Partially visible (ie. check for clipping)
s32 IsVisible( const bbox& BBox )
{
    s32 Visible = view::VISIBLE_NONE ;

    // Get current view
    const view* pView = eng_GetActiveView(0);
    ASSERT(pView) ;

    // If sky is fogged then check if we are completely fogged
    //if(tgl.Fog.GetVisDistance()<300.0f)
    {
        f32 D2 = (pView->GetPosition() - BBox.GetCenter()).LengthSquared();
        f32 D = (tgl.Fog.GetVisDistance()+20.0f);
        if( D2 > D*D )
        {
            //draw_Point( m_WorldPos, XCOLOR_YELLOW );
            return view::VISIBLE_NONE ;
        }
    }

    // In view volume?
    Visible = pView->BBoxInView( BBox ) ;

    // Do terrain visiblity check?
    if (Visible)
    {
        // Occluded?
        ASSERT( tgl.pTerr );
        if (tgl.pTerr->IsOccluded(BBox, pView->GetPosition()))
            return view::VISIBLE_NONE ;
    }

/*
    if( Visible )
    {
        draw_Point( BBox.GetCenter(), XCOLOR_BLUE );
    }
    else
    {
        draw_Point( BBox.GetCenter(), XCOLOR_YELLOW );
    }
*/
    return Visible;
}
//=========================================================================
xbool UseZoneVis = TRUE;
xbool RenderZoneVis = FALSE;

s32 IsVisibleInZone( const zone_set& ZSet, const bbox& BBox )
{
    s32 Visible = view::VISIBLE_NONE ;

    // Get current view
    const view* pView = eng_GetActiveView(0);
    ASSERT(pView) ;

    // If sky is fogged then check if we are completely fogged
    //if(tgl.Fog.GetVisDistance()<300.0f)
    {
        f32 D2 = (pView->GetPosition() - BBox.GetCenter()).LengthSquared();
        f32 D = (tgl.Fog.GetVisDistance()+20.0f);
        if( D2 > D*D )
        {
            Visible = view::VISIBLE_NONE;
            goto Exit;
        }
    }

    // In view volume?
    Visible = pView->BBoxInView( BBox ) ;

    // Do building zone visiblity check?
    if (Visible && UseZoneVis)
    {
        if( !g_BldManager.IsZoneSetVisible( *((bld_manager::zone_set*)&ZSet) ) )
        {
            Visible = view::VISIBLE_NONE;
            if( RenderZoneVis )
                draw_Point( BBox.GetCenter(), XCOLOR_BLUE );
            return Visible;
        }
    }

    // Do terrain visiblity check?
    if( Visible && g_BldManager.IsViewInZoneZero(0) )
    {
        // Occluded?
        ASSERT( tgl.pTerr );
        if (tgl.pTerr->IsOccluded(BBox, pView->GetPosition()))
        {
            Visible = view::VISIBLE_NONE;
            goto Exit;
        }
    }

Exit:

    if( RenderZoneVis )
    {
        if( Visible )
        {
            draw_Point( BBox.GetCenter(), XCOLOR_GREEN );
        }
        else
        {
            draw_Point( BBox.GetCenter(), XCOLOR_RED );
        }
    }
    return Visible;
}

//=========================================================================

void ComputeZoneSet( zone_set& ZSet, const bbox& BBox, xbool JustCenter )
{
    g_BldManager.GetZoneSet( *((bld_manager::zone_set*)&ZSet), BBox, JustCenter );
}

//=========================================================================
