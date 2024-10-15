//==============================================================================
//
//  Contrail.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================
#include "..\Demo1\Globals.hpp"
#include "Entropy.hpp"

#include "contrail.hpp"

// our texture 
static s32 s_SmokeIdx = -1;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void contrail::AddNode ( const vector3& NewNode, f32 Alpha )
{
    // only add the node if either:
    // a) If there are no active nodes or
    // b) the length between this node and the last node is > SEGLEN and THEN
    //    create the new node based on SEGLEN, and repeat if dist still > SEGLEN
/*    if ( AnyActiveNodes() )
    {
        s32 i = GetPrevNodeIdx( m_CurNode );
        f32 LenSq = (NewNode - m_Nodes[i].m_Node).LengthSquared();

        // add as many nodes as required to fill the gap
        while ( LenSq > SEGLEN2 )
        {
            // find the delta vector
            vector3 Delta = NewNode - m_Nodes[i].m_Node;
            
            // normalize it in preparation for scaling
            Delta.Normalize();
            
            // add the new node
            m_Nodes[ m_CurNode ].m_Node = m_Nodes[i].m_Node + ( Delta * SEGLEN );
            m_Nodes[ m_CurNode ].m_Age = 0.0f;
            m_Nodes[ m_CurNode ].m_Alive = TRUE;

            // decrement the length, and fix i
            LenSq -= SEGLEN2;
            i = m_CurNode;

            // prepare to add a new node
            NextNode();
        }
    }
    else*/   // Screw it, just add the node, it looks fine.
    {
        // no active nodes, so add it
        m_Nodes[ m_CurNode ].m_Node = NewNode;
        m_Nodes[ m_CurNode ].m_Age = 0.0f;
        m_Nodes[ m_CurNode ].m_Alive = TRUE ;        
        m_Nodes[ m_CurNode ].m_MaxAlpha = Alpha ;
        NextNode();
    }
}

//==============================================================================

void contrail::Update ( f32 DeltaTime )
{
    // loop thru all nodes, aging them and destroying them as necessary
    for ( s32 i = 0; i < MAXNODES; i++ )
    {
        // age the node
        m_Nodes[ i ].m_Age += DeltaTime;

        // kill the node if it's too old
        if ( m_Nodes[ i ].m_Age > MAXLIFE )
            m_Nodes[ i ].m_Alive = FALSE;
    }
}

//==============================================================================

void contrail::Render ( const vector3& CurPos, xbool Connect )
{
    // we render the trail, then connect the newest point to the CurPos, in
    // order to keep the trail constantly connected to the wing
    // render from the newest node back, until we hit a dead one
    s32         n1 = GetPrevNodeIdx( m_CurNode );
    s32         n2;
    vector3     Pos1, Pos2;
    s32         Op1, Op2;
    f32         W1, W2;
    xcolor      C1, C2;

    // initialize Op1 and W1
    Op1 = MAX_OPACITY;
    W1 = MIN_WIDTH;


    // init our texture index
    if ( s_SmokeIdx == -1 )
	    s_SmokeIdx = tgl.GameShapeManager.GetTextureManager().GetTextureIndexByName("[A]ShrikeBolt.BMP") ;  // not extension not needed!

    // Initialize the hardware
    draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | DRAW_USE_ALPHA );
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
    gsreg_SetZBufferUpdate(FALSE);
    gsreg_SetClamping( FALSE );
    gsreg_End();
#endif
    draw_SetTexture( tgl.GameShapeManager.GetTextureManager().GetTexture(s_SmokeIdx).GetXBitmap() );

    for ( s32 i = 0; i < MAXNODES-1; i++ )
    {
        // travel backwards in time
        n2 = GetPrevNodeIdx( n1 );

        // again exit at first opportunity
        if ( m_Nodes[ n2 ].m_Alive == FALSE )
            break;


        // if this is the first iteration, draw between newest point and CurPos
        if ( i == 0 )
        {
            Pos2 = m_Nodes[ n1 ].m_Node;
            Pos1 = CurPos;

            // figure out opacity and width
            f32 AgeFac = 0.0f;

            Op1 = 0;
            Op2 = (s32)( m_Nodes[ n1 ].m_MaxAlpha * MAX_OPACITY ) ;
            W2 = MIN_WIDTH + ( AgeFac * (MAX_WIDTH - MIN_WIDTH) );
        }
        else
        {
            // get our endpoints
            Pos2 = m_Nodes[ n2 ].m_Node;

            // figure out opacity and width
            f32 AgeFac = ( m_Nodes[ n2 ].m_Age / MAXLIFE );

            Op2 = (s32)((1.0f - AgeFac) * m_Nodes[ n2 ].m_MaxAlpha * MAX_OPACITY) ;
            W2 = MIN_WIDTH + ( AgeFac * (MAX_WIDTH - MIN_WIDTH) );

            n1 = n2;
        }

        // setup the colors
        C1.Set( 255, 255, 255, Op1 );
        C2.Set( 255, 255, 255, Op2 );

        // render the quad
        if ( (i != 0) || Connect )
        {
            draw_OrientedQuad(   Pos1 ,
                                 Pos2 ,
                                 vector2(0,0),
                                 vector2(1,1),
                                 C1,
                                 C2,
                                 W1,
                                 W2 );
        }

        // fix the variables for stepping
        W1 = W2;
        Op1 = Op2;
        Pos1 = Pos2;
    }

    
    draw_End();

#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBufferUpdate(TRUE);
    gsreg_End();
#endif
}

//==============================================================================

void contrail::NextNode    ( void )
{
    m_CurNode++;

    if ( m_CurNode == MAXNODES )
        m_CurNode = 0;
}

//==============================================================================

s32 contrail::GetPrevNodeIdx( s32 Idx )
{
    if ( Idx == 0 )
        return MAXNODES - 1;
    else
        return Idx - 1;
}

//==============================================================================

xbool contrail::AnyActiveNodes( void )
{
    for ( s32 i = 0; i < MAXNODES; i++ )
    {
        if ( m_Nodes[i].m_Alive == TRUE )
            return TRUE;
    }

    return FALSE;
}

//==============================================================================
