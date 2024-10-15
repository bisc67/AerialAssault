//==============================================================================
//
//  hud_Icons.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "hud_Icons.hpp"
#include "Entropy.hpp"
#include "Demo1\Globals.hpp"

//==============================================================================
//  STORAGE
//==============================================================================

hud_icons   HudIcons;
xbool       hud_icons::m_Initialized = FALSE;

//==============================================================================
//  FUNCTIONS
//==============================================================================

hud_icons::hud_icons( void )
{
    ASSERT( !m_Initialized );
    m_Initialized = TRUE;
}

//==============================================================================

hud_icons::~hud_icons( void ) 
{
    ASSERT( m_Initialized );
    m_Initialized = FALSE;
}

//==============================================================================

void hud_icons::Init( void )
{
    // Load the xbitmap.

    #ifdef TARGET_PC
    VERIFY( m_Icons.Load( "Data\\HUD\\PC\\hud_Icons.xbmp" ) );
    #endif

    #ifdef TARGET_PS2
    VERIFY( m_Icons.Load( "Data\\HUD\\PS2\\hud_Icons.xbmp" ) );
    #endif

    vram_Register( m_Icons );
}

//==============================================================================

void hud_icons::Kill( void )
{
    // Unload the xbitmap.
    vram_Unregister( m_Icons );
    m_Icons.Kill();
}

//==============================================================================

void hud_icons::Add( icon Icon, const vector3& Position, u32 TeamBits )
{
    if( m_NEntries == MAX_ENTRIES )
        return;

    m_Entry[m_NEntries].Icon       = Icon;
    m_Entry[m_NEntries].Position   = Position;
    m_Entry[m_NEntries].TeamBits   = TeamBits;
    m_Entry[m_NEntries].ColorIndex = USE_BITS;
    m_Entry[m_NEntries].Alpha      = 127;

    m_NEntries++;
}

//==============================================================================

void hud_icons::Add( icon Icon, const vector3& Position, hud_icons::color ColorIndex )
{
    if( m_NEntries == MAX_ENTRIES )
        return;

    m_Entry[m_NEntries].Icon       = Icon;
    m_Entry[m_NEntries].Position   = Position;
    m_Entry[m_NEntries].TeamBits   = 0x00;
    m_Entry[m_NEntries].ColorIndex = ColorIndex;
    m_Entry[m_NEntries].Alpha      = 127;

    m_NEntries++;
}

//==============================================================================

void hud_icons::Add( icon Icon, const vector3& Position, hud_icons::color ColorIndex, s32 Alpha )
{
    if( m_NEntries == MAX_ENTRIES )
        return;

    m_Entry[m_NEntries].Icon       = Icon;
    m_Entry[m_NEntries].Position   = Position;
    m_Entry[m_NEntries].TeamBits   = 0x00;
    m_Entry[m_NEntries].ColorIndex = ColorIndex;
    m_Entry[m_NEntries].Alpha      = Alpha;

    m_NEntries++;
}

//==============================================================================

void hud_icons::Add( icon Icon, const vector3& Position )
{
    if( m_NEntries == MAX_ENTRIES )
        return;

    m_Entry[m_NEntries].Icon       = Icon;
    m_Entry[m_NEntries].Position   = Position;
    m_Entry[m_NEntries].TeamBits   = 0x00;
    m_Entry[m_NEntries].ColorIndex = HARD_CODED;

    m_NEntries++;
}

//==============================================================================

void hud_icons::Add( icon Icon )
{
    (void)Icon;
    /*
    ASSERT( Icon == BEACON_TARGET_LINE );

    if( m_NEntries == MAX_ENTRIES )
        return;

    m_Entry[m_NEntries].Icon       = Icon;
    m_Entry[m_NEntries].TeamBits   = 0x00;
    m_Entry[m_NEntries].ColorIndex = HARD_CODED;

    m_NEntries++;
    */
}

//==============================================================================

void hud_icons::Render( void )
{
    s32    i;
    xcolor Color;
    xcolor ColorList[] = 
    {                       
        xcolor(  0,255,  0,127),    // GREEN
        xcolor(191,  0,  0,127),    // RED
        xcolor(255,255,255,127),    // WHITE
        xcolor(191,191,  0,127),    // YELLOW
    };

    u32 ContextBits = 0x00;
    {
        object::id  PlayerID( tgl.PC[tgl.WC].PlayerIndex, -1 );
        object*     pObject = ObjMgr.GetObjectFromID( PlayerID );
        if( pObject )
            ContextBits = pObject->GetTeamBits();
    }

    eng_Begin( "HudIcons" );
    draw_Begin( DRAW_SPRITES, DRAW_2D | DRAW_TEXTURED | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER );
    draw_SetTexture( m_Icons );

    const view& View = tgl.GetView();

    for( i = 0; i < m_NEntries; i++ )
    {
        vector3 ScreenPos = View.PointToScreen( m_Entry[i].Position );

        // If behind camera, just forget about it.
        if( ScreenPos.Z < 0 )
            continue;

        if( m_Entry[i].ColorIndex == USE_BITS )
        {
            if( (m_Entry[i].TeamBits == 0xFFFFFFFF) || 
                (m_Entry[i].TeamBits == 0x00000000) || 
                (ContextBits         == 0x00000000) )
                Color = ColorList[2];       // WHITE
            else
                Color = (ContextBits & m_Entry[i].TeamBits) 
                        ? ColorList[0]      // GREEN
                        : ColorList[1];     // RED
        }
        else if( m_Entry[i].ColorIndex != HARD_CODED )
        {
            ASSERT( IN_RANGE( 0, m_Entry[i].ColorIndex, 3 ) );
            Color = ColorList[ m_Entry[i].ColorIndex ];
        }

        Color.A = m_Entry[i].Alpha;

        switch( m_Entry[i].Icon )
        {
        //----------------------------------------------------------------------
        case FLAG:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.00f,0.00f), 
                           vector2(0.25f,0.25f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        case FLAG_HALO:         
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.25f,0.00f), 
                           vector2(0.50f,0.25f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        case WAYPOINT:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.50f,0.00f), 
                           vector2(0.75f,0.25f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        case SWITCHPOINT:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.75f,0.25f), 
                           vector2(1.00f,0.50f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        case PLAYER_INDICATOR:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.75f,0.00f), 
                           vector2(1.00f,0.25f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        /*
        case BEACON_TARGET_BASE:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.25f,0.25f), 
                           vector2(0.50f,0.50f), 
                           xcolor(  0,255,  0,191) );
            break;
            */
        //----------------------------------------------------------------------
        /*  
        case BEACON_TARGET_AIM:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.00f,0.25f), 
                           vector2(0.25f,0.50f), 
                           xcolor(  0,255,  0,191) );
            break;
            */
        //----------------------------------------------------------------------
        case BEACON_MARK:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.50f,0.25f), 
                           vector2(0.75f,0.50f), 
                           xcolor(  0,255,  0,191) );
            break;

        //----------------------------------------------------------------------
        case MISSILE_TRACK:
            ScreenPos.X -= 16;
            ScreenPos.Y -= 16;
            draw_SpriteUV( ScreenPos, 
                           vector2( 32, 32 ), 
                           vector2(0.50f,0.50f), 
                           vector2(1.00f,1.00f), 
                           xcolor(255,255,  0,191) );
            break;

        //----------------------------------------------------------------------
        case MISSILE_LOCK:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            if( tgl.ActualTime & 0x00002000 )
                draw_SpriteUV( ScreenPos, 
                               vector2( 16, 16 ), 
                               vector2(0.00f,0.50f), 
                               vector2(0.25f,0.75f), 
                               xcolor(191,191,  0,191) );
            else
                draw_SpriteUV( ScreenPos, 
                               vector2( 16, 16 ), 
                               vector2(0.00f,0.50f), 
                               vector2(0.25f,0.75f), 
                               xcolor(191,  0,  0,191) );
            break;

        //----------------------------------------------------------------------
        case AUTOAIM_TARGET:
            ScreenPos.X -= 8;
            ScreenPos.Y -= 8;
            draw_SpriteUV( ScreenPos, 
                           vector2( 16, 16 ), 
                           vector2(0.25f,0.25f), 
                           vector2(0.50f,0.50f), 
                           Color );
            break;

        //----------------------------------------------------------------------
        default:
            break;
        }
    }
    draw_End();

    /*
    draw_Begin( DRAW_LINES, DRAW_2D | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER );
    draw_Color( xcolor(0,255,0,191) );
    for( i = 2; i < m_NEntries; i++ )
    {
        if( (m_Entry[i-0].Icon != BEACON_TARGET_LINE) ||
            (m_Entry[i-1].Icon != BEACON_TARGET_AIM ) ||
            (m_Entry[i-2].Icon != BEACON_TARGET_BASE) )
            continue;
        vector3 From = View.PointToScreen( m_Entry[i-1].Position );
        vector3 To   = View.PointToScreen( m_Entry[i-2].Position );

        // If behind camera, just forget about it.
        if( (From.Z < 0) || (To.Z < 0) )
            continue;

        draw_Vertex( From );
        draw_Vertex( To   );
    }
    draw_End();
    */

    m_NEntries = 0;
    eng_End();
}

//==============================================================================

void hud_icons::Clear( void )
{
    m_NEntries = 0;
}

//==============================================================================
