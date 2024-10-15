//=========================================================================
//
//  hud.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "aux_bitmap.hpp"
#include "hud.hpp"
#include "Objects/Player/PlayerObject.hpp"
#include "demo1/Globals.hpp"
#include "demo1/GameClient.hpp"

#include "VoiceLayout.hpp"

//=========================================================================

#define HUD_CHAT_WINDOW_X   16
#define HUD_CHAT_WINDOW_Y   16
#define HUD_CHAT_WINDOW_W   384 //424
#define HUD_CHAT_WINDOW_H   56

#define HUD_PAUSE_WINDOW_X  16
#define HUD_PAUSE_WINDOW_Y  80
#define HUD_PAUSE_WINDOW_W  440
#define HUD_PAUSE_WINDOW_H  350

#define HUD_METER_HEALTH_X  (HUD_CHAT_WINDOW_X+HUD_CHAT_WINDOW_W+4)
#define HUD_METER_HEALTH_Y  (HUD_CHAT_WINDOW_Y)

#define HUD_METER_ENERGY_X  (HUD_METER_HEALTH_X+16)
#define HUD_METER_ENERGY_Y  (HUD_CHAT_WINDOW_Y)

#define HUD_WEAPONS_X       (m_Width-16-27)
#define HUD_WEAPONS_Y       (m_Height-16-40*6)

#define HUD_VOICE_WINDOW_W  (192-24)
#define HUD_VOICE_WINDOW_H  102
#define HUD_VOICE_WINDOW1_X (HUD_PAUSE_WINDOW_X+16)
#define HUD_VOICE_WINDOW1_Y (HUD_PAUSE_WINDOW_Y+HUD_PAUSE_WINDOW_H-16-HUD_VOICE_WINDOW_H)
#define HUD_VOICE_WINDOW2_X (HUD_PAUSE_WINDOW_X+HUD_PAUSE_WINDOW_W-HUD_VOICE_WINDOW_W-16)
#define HUD_VOICE_WINDOW2_Y (HUD_PAUSE_WINDOW_Y+HUD_PAUSE_WINDOW_H-16-HUD_VOICE_WINDOW_H)
#define HUD_VOICE_WINDOW3_X (HUD_PAUSE_WINDOW_X+HUD_PAUSE_WINDOW_W-HUD_VOICE_WINDOW_W-16)
#define HUD_VOICE_WINDOW3_Y (HUD_PAUSE_WINDOW_Y+56)

//=========================================================================

/*
#ifdef TARGET_PS2
    #define RES_X 512
    #define RES_Y 448
#endif
#ifdef TARGET_PC
    #define RES_X 800 // 512
    #define RES_Y 600 // 448
#endif
*/

//=========================================================================
//  For debugging
//=========================================================================

hud::voice_table* pVoiceTables[] =
{
    &hud_vc_Root,
    &hud_vc_Self,
    &hud_vc_Global,
    &hud_vc_Animations,
    &hud_vc_Compliment,
    &hud_vc_Respond,
    &hud_vc_Taunt,
    &hud_vc_Attack,
    &hud_vc_Defend,
    &hud_vc_Repair,
    &hud_vc_Base,
    &hud_vc_Enemy,
    &hud_vc_Flag,
    &hud_vc_Need,
    &hud_vc_Attack_Self,
    &hud_vc_Defend_Self,
    &hud_vc_Repair_Self,
    &hud_vc_Task_Self,
    &hud_vc_Target,
    &hud_vc_Warning,
    &hud_vc_Quick
};
s32 VoiceTableIndex = 0;

//=========================================================================

static xbool LoadTexture( xbitmap& BMP, const char* pPathName )
{
    char    Path[X_MAX_PATH];
    char    Drive[X_MAX_DRIVE];
    char    Dir[X_MAX_DIR];
    char    FName[X_MAX_FNAME];
    char    Ext[X_MAX_EXT];

    xbool   Success = FALSE;

    x_splitpath( pPathName, Drive, Dir, FName, Ext );
#if defined TARGET_PS2
    x_strcat( Dir, "PS2/" );
#elif defined TARGET_PC
    x_strcat( Dir, "PC/" );
#else
    #error NO LOAD TEXTURE FUNCTION FOR TARGET
#endif
    x_makepath( Path, Drive, Dir, FName, Ext );

    if( !(Success = BMP.Load( xfs("%s.xbmp",Path) )) )
    {
        if( (Success = auxbmp_Load( BMP, xfs("%s.tga",pPathName) )) )
        {
#if defined TARGET_PS2
            BMP.ConvertFormat( xbitmap::FMT_P8_RGBA_8888 );
            auxbmp_ConvertToPS2( BMP );
#elif defined TARGET_PC
            auxbmp_ConvertToD3D( BMP );
#else
    #error NO LOAD TEXTURE FUNCTION FOR TARGET
#endif
            BMP.Save( xfs("%s.xbmp",Path) );
        }
    }

    if( Success )
        vram_Register( BMP );

    return Success;
}

//=========================================================================

hud::hud( void )
{
    m_Initialized           = FALSE;

    m_MenuMode              = menu_null;

    m_pVoiceTable           = NULL;
    m_MenuLX                = FALSE;
    m_MenuLY                = FALSE;
    m_MenuRX                = FALSE;
    m_MenuRY                = FALSE;

    m_InventorySelection    = 0;

    m_PopupTimer            = 0.0f;
    m_PopupMessage[0]       = 0;
}

hud::~hud( void )
{
    if( m_Initialized )
        Kill();
}

//=========================================================================

void hud::Init( void )
{
    // Load font
    VERIFY( m_Font.Load( "Data/HUD/Font/hud_font" ) );

    // Load Window texture
    VERIFY( LoadTexture( m_Window, "Data/HUD/Window/hud_window" ) );

    // Load Weapons textures
    VERIFY( LoadTexture( m_Weapons[disc], "Data/HUD/Weapons/hud_disc" ) );
    VERIFY( LoadTexture( m_Weapons[plasma], "Data/HUD/Weapons/hud_plasma" ) );
    VERIFY( LoadTexture( m_Weapons[sniper], "Data/HUD/Weapons/hud_sniper" ) );
    VERIFY( LoadTexture( m_Weapons[mortar], "Data/HUD/Weapons/hud_mortar" ) );
    VERIFY( LoadTexture( m_Weapons[grenade], "Data/HUD/Weapons/hud_grenade" ) );
    VERIFY( LoadTexture( m_Weapons[chaingun], "Data/HUD/Weapons/hud_chaingun" ) );
    VERIFY( LoadTexture( m_Weapons[blaster], "Data/HUD/Weapons/hud_blaster" ) );
    VERIFY( LoadTexture( m_Weapons[elf], "Data/HUD/Weapons/hud_elf" ) );
    VERIFY( LoadTexture( m_Weapons[missile], "Data/HUD/Weapons/hud_missile" ) );
    VERIFY( LoadTexture( m_Weapons[targetlaser], "Data/HUD/Weapons/hud_targetlaser" ) );

    // Load Reticle textures
    VERIFY( LoadTexture( m_ReticleBase, "Data/HUD/Reticles/hud_ret_base" ) );
    VERIFY( LoadTexture( m_Reticles[disc], "Data/HUD/Reticles/hud_ret_disc" ) );
    VERIFY( LoadTexture( m_Reticles[plasma], "Data/HUD/Reticles/hud_ret_plasma" ) );
    VERIFY( LoadTexture( m_Reticles[sniper], "Data/HUD/Reticles/hud_ret_sniper" ) );
    VERIFY( LoadTexture( m_Reticles[mortar], "Data/HUD/Reticles/hud_ret_mortar" ) );
    VERIFY( LoadTexture( m_Reticles[grenade], "Data/HUD/Reticles/hud_ret_grenade" ) );
    VERIFY( LoadTexture( m_Reticles[chaingun], "Data/HUD/Reticles/hud_ret_chaingun" ) );
    VERIFY( LoadTexture( m_Reticles[blaster], "Data/HUD/Reticles/hud_ret_blaster" ) );
    VERIFY( LoadTexture( m_Reticles[elf], "Data/HUD/Reticles/hud_ret_elf" ) );
    VERIFY( LoadTexture( m_Reticles[missile], "Data/HUD/Reticles/hud_ret_missile" ) );
    VERIFY( LoadTexture( m_Reticles[targetlaser], "Data/HUD/Reticles/hud_ret_targetlaser" ) );

    // Load compass
    VERIFY( LoadTexture( m_Compass, "Data/HUD/Compass/hud_compass" ) );
    VERIFY( LoadTexture( m_NESW, "Data/HUD/Compass/hud_nesw" ) );

    // Load Voice Chat controller
    VERIFY( LoadTexture( m_VoiceChatController, "Data/HUD/VoiceChat/hud_controller" ) );

    // Initialize chat
    m_pChatMemory = (char*)x_malloc( (HUD_CHAT_LINE_LEN+1) * HUD_CHAT_LINES );
    ASSERT( m_pChatMemory );
    char* pChatMemory = m_pChatMemory;
    for( s32 i=0 ; i<HUD_CHAT_LINES ; i++ )
    {
        m_pChatLine[i] = pChatMemory;
        m_pChatLine[i][0] = 0;
        pChatMemory += HUD_CHAT_LINE_LEN+1;
    }
    m_iChatLine = 0;

    // We are initialized
    m_Initialized = TRUE;

    // TODO: Remove
    //AddChatLine( xcolor(128,255,255,255), "You are in mission Sun Dried (Rabbit)." );
    //AddChatLine( xcolor(128,255,128,255), "Ricochet has joined the hunt." );
    //AddChatLine( xcolor(128,255,128,255), "Mumm-Rajoinedthegame.asdfjhasdkfjhaskjdfhaksjdfhksajhdfkjsahfkjash" );
    //AddChatLine( xcolor(128,255,255,255), "buttcandy gooses Sadismo with an extra-friendly burst of plasma." );
}

//=========================================================================

void hud::Kill( void )
{
    m_Font.Kill();

    vram_Unregister( m_Window );
    m_Window.Kill();

    for( s32 i=0 ; i<num_weapons ; i++ )
    {
        if (m_Weapons[i].GetVRAMID() != 0)
        {
            vram_Unregister( m_Weapons[i] );
            m_Weapons[i].Kill();
        }

        if (m_Reticles[i].GetVRAMID() != 0)
        {
            vram_Unregister( m_Reticles[i] );
            m_Reticles[i].Kill();   
        }
    }

    vram_Unregister( m_ReticleBase );
    m_ReticleBase.Kill();

    vram_Unregister( m_VoiceChatController );
    m_VoiceChatController.Kill();

    vram_Unregister( m_Compass );
    vram_Unregister( m_NESW );
    m_Compass.Kill();
    m_NESW.Kill();

    x_free( m_pChatMemory );

    m_Initialized = FALSE;
}

//=========================================================================

void hud::AddChatLine( xcolor Color, const char* pString )
{
    s32 x           = 0;
    s32 iString     = 0;
    s32 iLine       = 0;
    s32 iStringWrap = -1;
    s32 iLineWrap   = 0;
    s32 cPrev       = 0;
    s32 c;
    s32 w;

    // Advance to next line
    m_iChatLine = (m_iChatLine+1) % HUD_CHAT_LINES;

    // Set color of line
    m_ChatColor[m_iChatLine] = Color;

    // Word Wrap Text
    while( pString[iString] )
    {
        // Get Character
        c = pString[iString++];

        // Check for beginning of word
        if( x_isspace(cPrev) && !x_isspace(c) )
        {
            iStringWrap = iString-1;
            iLineWrap   = iLine;
        }

        // Update previous character
        cPrev = c;

        // Advance cursor before checking wrap
        w = m_Font.m_Characters[c].W;
        x += w+1;

        // If bounds exceeded then wrap string or if line length exceeded, wrap string
        if( (x > HUD_CHAT_WINDOW_W-8) || (iLine == HUD_CHAT_LINE_LEN) )
        {
            // Check if there is a word wrap point
            if( iStringWrap == -1 )
            {
                // Wrap to next line
                m_pChatLine[m_iChatLine][iLine] = 0;
                m_iChatLine = (m_iChatLine+1) % HUD_CHAT_LINES;
                m_ChatColor[m_iChatLine] = Color;
                iLine = 0;
                x     = w;
                iStringWrap = -1;
            }
            else
            {
                // Terminate previous line
                m_pChatLine[m_iChatLine][iLineWrap] = 0;

                // Reset String scanner
                iString = iStringWrap;
                iStringWrap = -1;
                c = pString[iString++];
                m_iChatLine = (m_iChatLine+1) % HUD_CHAT_LINES;
                m_ChatColor[m_iChatLine] = Color;
                iLine = 0;
                x     = w;
            }
        }
        m_pChatLine[m_iChatLine][iLine++] = c;
    }

    // Terminate string
    m_pChatLine[m_iChatLine][iLine] = 0;
}

//=========================================================================

void hud::RenderWindow( const irect& R, f32 Alpha )
{
    f32     w = (f32)R.GetWidth();
    f32     h = (f32)R.GetHeight();
    xcolor  c = XCOLOR_WHITE;

    ASSERT( R.GetWidth() >= 16 );
    ASSERT( R.GetHeight() >= 16 );

    c.A = (u8)(255*Alpha);

    draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );

    draw_SetTexture( m_Window );

    vector3 p( (f32)R.l, (f32)R.t, 0.0f );

    vector2 wh1( 8.0f      , 8.0f      );
    vector2 wh2( w - 16.0f , 8.0f      );
    vector2 wh3( 8.0f      , h - 16.0f );
    vector2 wh4( w - 16.0f , h - 16.0f );

    vector2 uv[16];
    for( s32 i=0 ; i<16 ; i++ )
    {
        uv[i].X = ((i%4)*8.0f)/32.0f;
        uv[i].Y = ((i/4)*8.0f)/32.0f;
    }

    // Top
    draw_SpriteUV( p+vector3(  0.0f,0.0f,0.0f), wh1, uv[ 0], uv[ 5], c );
    draw_SpriteUV( p+vector3(  8.0f,0.0f,0.0f), wh2, uv[ 1], uv[ 6], c );
    draw_SpriteUV( p+vector3(w-8.0f,0.0f,0.0f), wh1, uv[ 2], uv[ 7], c );

    // Mid
    draw_SpriteUV( p+vector3(  0.0f,8.0f,0.0f), wh3, uv[ 4], uv[ 9], c );
    draw_SpriteUV( p+vector3(  8.0f,8.0f,0.0f), wh4, uv[ 5], uv[10], c );
    draw_SpriteUV( p+vector3(w-8.0f,8.0f,0.0f), wh3, uv[ 6], uv[11], c );

    // Bot
    draw_SpriteUV( p+vector3(  0.0f,h-8.0f,0.0f), wh1, uv[ 8], uv[13], c );
    draw_SpriteUV( p+vector3(  8.0f,h-8.0f,0.0f), wh2, uv[ 9], uv[14], c );
    draw_SpriteUV( p+vector3(w-8.0f,h-8.0f,0.0f), wh1, uv[10], uv[15], c );

    draw_End();
}

//=========================================================================

void hud::RenderCompass( s32 x, s32 y )
{
    // Get Heading angle
    const view* pView = eng_GetActiveView( 0 );
    ASSERT( pView );
    vector3 v = pView->GetViewZ();
    f32 a = x_atan2( v.X, v.Z );
    
    x += m_XOffset;
    y += m_YOffset;

    // Draw Compass frame
    {
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
        draw_SetTexture( m_Compass );
        vector3 p( (f32)x, (f32)y, 0.0f );
        vector2 wh( 64.0f, 64.0f );
        vector2 uv1( 0.0f, 0.0f );
        vector2 uv2( 1.0f, 1.0f );
        draw_SpriteUV( p, wh, uv1, uv2, XCOLOR_WHITE );
        draw_End();
    }

    // Draw Compass NESW
    {
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
        draw_SetTexture( m_NESW );
        vector3 p( (f32)(x+31), (f32)(y+31), 0.0f );
        vector2 wh(  12.0f, 12.0f );
        vector2 uv1( 0.0f, 0.0f );
        vector2 uv2( 1.0f/2, 0.0f/2 );
        vector2 uv4( 0.0f/2, 1.0f/2 );
        vector2 uv5( 1.0f/2, 1.0f/2 );
        vector2 uv6( 2.0f/2, 1.0f/2 );
        vector2 uv8( 1.0f/2, 2.0f/2 );
        vector2 uv9( 2.0f/2, 2.0f/2 );

        vector3 o( 0.0f, -31+5.0f, 0.0f );
        vector3 o2;

        xcolor Color = xcolor(128,255,128,255);

        o.RotateZ( a );
        o2.X = x_floor(o.X); o2.Y = x_floor(o.Y); o2.Z = o.Z;
        o2 = o;
        draw_SpriteUV( p+o2, wh, uv1, uv5, Color, -a );
        
        o.RotateZ( R_90 );
        o2.X = x_floor(o.X); o2.Y = x_floor(o.Y); o2.Z = o.Z;
        o2 = o;
        draw_SpriteUV( p+o2, wh, uv2, uv6, Color, -a-RADIAN(90) );
        
        o.RotateZ( R_90 );
        o2.X = x_floor(o.X); o2.Y = x_floor(o.Y); o2.Z = o.Z;
        o2 = o;
        draw_SpriteUV( p+o2, wh, uv4, uv8, Color, -a-RADIAN(180) );
        
        o.RotateZ( R_90 );
        o2.X = x_floor(o.X); o2.Y = x_floor(o.Y); o2.Z = o.Z;
        o2 = o;
        draw_SpriteUV( p+o2, wh, uv5, uv9, Color, -a-RADIAN(270) );

        draw_End();
    }
}

//=========================================================================

void hud::RenderMeter( s32 x, s32 y )
{
    // Draw Meter frame
    if( 0 )
    {
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
//        draw_SetTexture( m_Meter );
        vector3 p( (f32)x, (f32)y, 0.0f );
        vector2 wh( 32.0f, 64.0f );
        vector2 uv1( 0.0f, 0.0f );
        vector2 uv2( 1.0f, 1.0f );
        draw_SpriteUV( p, wh, uv1, uv2, XCOLOR_WHITE );
        draw_End();
    }
}

//=========================================================================

void hud::RenderChat( s32 x, s32 y )
{
    // Render Text Window
    irect R( x, 
             y, 
             (x+HUD_CHAT_WINDOW_W), 
             (y+HUD_CHAT_WINDOW_H) );
    RenderWindow( R );

    R.Deflate( 4, 3 );
    R.SetHeight( 13 );
    R.Translate( 0, -3 );
    for( s32 i=0 ; i<HUD_CHAT_LINES ; i++ )
    {
        s32 iLine = (m_iChatLine+1+i) % HUD_CHAT_LINES;
        m_Font.DrawText( R, 0, m_ChatColor[iLine], m_pChatLine[iLine] );
        R.Translate( 0, 13 );
    }
}

//=========================================================================

void hud::RenderWeapons( s32 x, s32 y )
{
//  vector3 pos( (f32)x, (f32)y, 0.0f );
//  vector2 wh( 27.0f, 38.0f );
//  vector2 uv0;
//  vector2 uv1;
//  s32     i;

    x += m_XOffset;
    y += m_YOffset;

    // Only is we have a player pointer
    if( m_pPlayer )
    {
        // Get Loadout
        const player_object::loadout& Loadout = m_pPlayer->GetLoadout();

        // Get current weapon type
        s32 WeaponCurrentSlot = m_pPlayer->GetWeaponCurrentSlot();
        s32 WeaponCurrentType = Loadout.Weapons[WeaponCurrentSlot];
/*
        // Render Weapon Icons
        pos.Y += (6-Loadout.NumWeapons) * 40.0f;
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
        for( i=0 ; i < Loadout.NumWeapons ; i++ )
        {
            s32 WeaponType = Loadout.Weapons[i];
            uv0.X = ( 0.0f) / m_Weapons[WeaponType].GetWidth();
            uv0.Y = ( 0.0f) / m_Weapons[WeaponType].GetHeight();
            uv1.X = (27.0f) / m_Weapons[WeaponType].GetWidth();
            uv1.Y = (38.0f) / m_Weapons[WeaponType].GetHeight();

            draw_SetTexture( m_Weapons[WeaponType] );
            draw_SpriteUV( pos, wh, uv0, uv1, xcolor(255,255,255,(i==WeaponCurrentSlot)?255:100) );

            pos.Y += 40.0f;
        }
        draw_End( );

        // Render Ammo Counts
        irect R( x, y+25, x+27, y+25 );
        R.Translate( 0, (6-Loadout.NumWeapons)*40 );
        for( i=0 ; i < Loadout.NumWeapons ; i++ )
        {
            s32 WeaponType = Loadout.Weapons[i];
            if( m_Ammo[WeaponType] == -1 )
                m_Font.DrawText( R, hud_font::h_center, xcolor(255,255,255,(i==WeaponCurrentSlot)?224:100), "%" );
            else
                m_Font.DrawText( R, hud_font::h_center, xcolor(255,255,255,(i==WeaponCurrentSlot)?224:100), xfs("%d",m_Ammo[WeaponType]) );
            R.Translate( 0, 40 );
        }
*/
        // Render Reticle for Weapon
        if((WeaponCurrentType != 0) && ( m_pPlayer->GetViewType() == player_object::VIEW_TYPE_1ST_PERSON ))
        {
            if( (WeaponCurrentType>=0) && (WeaponCurrentType<num_weapons) )
            {
                f32 bw = (f32)m_Reticles[WeaponCurrentType].GetWidth();
                f32 bh = (f32)m_Reticles[WeaponCurrentType].GetHeight();

                if( (WeaponCurrentType != sniper) &&
                    (WeaponCurrentType != targetlaser) )
                {
                    draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D|DRAW_NO_ZBUFFER );
                    draw_SetTexture( m_ReticleBase );
                    draw_Sprite( vector3((f32)m_XOffset+(m_Width-64)/2.0f,(f32)m_YOffset+(m_Height-64)/2.0f, 0.0f), vector2(64.0f,64.0f), XCOLOR_WHITE );
                    draw_End( );
                }

                draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D|DRAW_NO_ZBUFFER );
                draw_SetTexture( m_Reticles[WeaponCurrentType] );
                draw_Sprite( vector3((f32)m_XOffset+(m_Width-bw)/2.0f,(f32)m_YOffset+(m_Height-bh)/2.0f, 0.0f), vector2((f32)bw,(f32)bh), XCOLOR_WHITE );
                draw_End( );
            }
        }
    }
}

//=========================================================================

void hud::RenderInventory( s32 x, s32 y )
{
    irect R;
    irect R2;

    // Render frame
    R.Set(  x, y, HUD_PAUSE_WINDOW_X+HUD_PAUSE_WINDOW_W, HUD_PAUSE_WINDOW_Y+HUD_PAUSE_WINDOW_H );
    RenderWindow( R );
    R.SetHeight( 40 );
    R.Deflate( 8, 8 );
    RenderWindow( R );
    R2 = R; R2.Deflate( 8, 0 );
    m_Font.DrawText( R2, hud_font::v_center, XCOLOR_WHITE, "INVENTORY SELECTION" );

    if( m_InventorySelection == 0 )
    {
        // Render options
        R2 = R; R2.t += 32; R2.SetHeight(20); R2.SetWidth(160);
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, xcolor(240,240,100,255), "SCOUT SNIPER" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "SCOUT OFFENSE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "JUGGERNAUT OFFENSE" );

        // Render loadout
        R2 = R; R2.l += 168; R2.t += 32; R2.SetHeight(20); R2.SetWidth(256);
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "ARMOR TYPE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SCOUT" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 1:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SPINFUSOR" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 2:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "GRENADE LAUNCHER" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 3:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SNIPER RIFLE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "PACK:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "ENERGY PACK" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "GRENADE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "GRENADE" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "MINE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "MINE" );
    }

    if( m_InventorySelection == 1 )
    {
        // Render options
        R2 = R; R2.t += 32; R2.SetHeight(20); R2.SetWidth(160);
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "SCOUT SNIPER" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, xcolor(240,240,100,255), "SCOUT OFFENSE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "JUGGERNAUT OFFENSE" );

        // Render loadout
        R2 = R; R2.l += 168; R2.t += 32; R2.SetHeight(20); R2.SetWidth(256);
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "ARMOR TYPE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SCOUT" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 1:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SPINFUSOR" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 2:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "PLASMA RIFLE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 3:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "CHAIN GUN" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "PACK:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "ENERGY PACK" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "GRENADE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "GRENADE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "MINE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "MINE" );
    }

    if( m_InventorySelection == 2 )
    {
        // Render options
        R2 = R; R2.t += 32; R2.SetHeight(20); R2.SetWidth(160);
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "SCOUT SNIPER" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, XCOLOR_WHITE, "SCOUT OFFENSE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        m_Font.DrawText( R2, hud_font::h_center|hud_font::v_center, xcolor(240,240,100,255), "JUGGERNAUT OFFENSE" );

        // Render loadout
        R2 = R; R2.l += 168; R2.t += 32; R2.SetHeight(20); R2.SetWidth(256);
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "ARMOR TYPE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "JUGGERNAUT" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 1:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "CHAIN GUN" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 2:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "SPINFUSOR" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 3:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "PLASMA RIFLE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 4:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "GRENADE LAUNCHER" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "WEAPON 5:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "MORTAR LAUNCHER" );

        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "PACK:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "ENERGY PACK" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "GRENADE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "GRENADE" );
    
        R2.Translate( 0, 24 );
        RenderWindow( R2 );
        R = R2; R.Deflate( 6, 0 );
        m_Font.DrawText( R, hud_font::v_center, XCOLOR_GREY, "MINE:" );
        m_Font.DrawText( R, hud_font::h_right|hud_font::v_center, XCOLOR_GREY, "MINE" );
    }
}

//=========================================================================

#define HUD_VOICE_SEL_W     80
#define HUD_VOICE_SEL_H     32

void hud::Render4Way( s32 x, s32 y, s32 w, s32 h, voice_command* pCommands )
{
    s32     ox = 0;
    s32     oy = 0;
    s32     x2, y2;
    irect   R( x, y, x+w, y+h );
    xcolor  c1(128,255,128,255);
    xcolor  c2(192,192,192,255);

//    RenderWindow( R );

    // Left
    x2 = x+ox;
    y2 = y+(h-HUD_VOICE_SEL_H)/2;
    R.Set( x2, y2, x2+HUD_VOICE_SEL_W, y2+HUD_VOICE_SEL_H );
    RenderWindow( R );
    if( pCommands[3].pLabel != NULL )
    {
        m_Font.DrawFormattedText( R, hud_font::h_center|hud_font::v_center, pCommands[3].ID?c1:c2, pCommands[3].pLabel );
    }

    // Right
    x2 = x+w-HUD_VOICE_SEL_W-ox;
    y2 = y+(h-HUD_VOICE_SEL_H)/2;
    R.Set( x2, y2, x2+HUD_VOICE_SEL_W, y2+HUD_VOICE_SEL_H );
    RenderWindow( R );
    if( pCommands[1].pLabel != NULL )
    {
        m_Font.DrawFormattedText( R, hud_font::h_center|hud_font::v_center, pCommands[1].ID?c1:c2, pCommands[1].pLabel );
    }

    // Top
    x2 = x+(w-HUD_VOICE_SEL_W)/2;
    y2 = y+oy;
    R.Set( x2, y2, x2+HUD_VOICE_SEL_W, y2+HUD_VOICE_SEL_H );
    RenderWindow( R );
    if( pCommands[0].pLabel != NULL )
    {
        m_Font.DrawFormattedText( R, hud_font::h_center|hud_font::v_center, pCommands[0].ID?c1:c2, pCommands[0].pLabel );
    }

    // Bottom
    x2 = x+(w-HUD_VOICE_SEL_W)/2;
    y2 = y+h-HUD_VOICE_SEL_H-oy;
    R.Set( x2, y2, x2+HUD_VOICE_SEL_W, y2+HUD_VOICE_SEL_H );
    RenderWindow( R );
    if( pCommands[2].pLabel != NULL )
    {
        m_Font.DrawFormattedText( R, hud_font::h_center|hud_font::v_center, pCommands[2].ID?c1:c2, pCommands[2].pLabel );
    }
}

//=========================================================================

void hud::RenderVoiceMenu( s32 x, s32 y, voice_table* pVoiceTable )
{
    irect R;
    irect R2;

    // Render frame
    R.Set(  x, y, HUD_PAUSE_WINDOW_X+HUD_PAUSE_WINDOW_W, HUD_PAUSE_WINDOW_Y+HUD_PAUSE_WINDOW_H );
    RenderWindow( R );
    R.SetHeight( 40 );
    R. Deflate( 8, 8 );
    RenderWindow( R );
    R2 = R ; R2.Deflate( 8, 0 );
    m_Font.DrawText( R2, hud_font::v_center, XCOLOR_WHITE, xfs("VOICE COMMAND: %s", pVoiceTable->pTitle) );

    R.Set(  x, y, HUD_PAUSE_WINDOW_X+HUD_PAUSE_WINDOW_W, HUD_PAUSE_WINDOW_Y+HUD_PAUSE_WINDOW_H );
    R.Deflate( 8, 8 );
    R.t = R.b - 120;
    RenderWindow( R );

    R.Set(  HUD_VOICE_WINDOW3_X,
            HUD_VOICE_WINDOW3_Y,
            HUD_VOICE_WINDOW3_X+HUD_VOICE_WINDOW_W,
            HUD_VOICE_WINDOW3_Y+HUD_VOICE_WINDOW_H );
    R.Inflate( 8, 8 );
    RenderWindow( R );

    // Draw controller graphic
    {
        draw_Begin( DRAW_SPRITES, DRAW_USE_ALPHA|DRAW_TEXTURED|DRAW_2D );
        draw_SetTexture( m_VoiceChatController );
        vector3 p( (f32)x-8, (f32)y+72, 0.0f );
        vector2 wh( 256, 128 );
        vector2 uv1( 0.0f, 0.0f );
        vector2 uv2( 1.0f, 1.0f );
        draw_SpriteUV( p, wh, uv1, uv2, XCOLOR_WHITE );
        draw_End();
    }

    //Render Left Stick
    Render4Way( HUD_VOICE_WINDOW1_X, HUD_VOICE_WINDOW1_Y, HUD_VOICE_WINDOW_W, HUD_VOICE_WINDOW_H, &pVoiceTable->Commands[0] );

    //Render Right Stick
    Render4Way( HUD_VOICE_WINDOW2_X, HUD_VOICE_WINDOW2_Y, HUD_VOICE_WINDOW_W, HUD_VOICE_WINDOW_H, &pVoiceTable->Commands[4] );

    //Render Buttons
    Render4Way( HUD_VOICE_WINDOW3_X, HUD_VOICE_WINDOW3_Y, HUD_VOICE_WINDOW_W, HUD_VOICE_WINDOW_H, &pVoiceTable->Commands[8] );
}

//=========================================================================

#define POPUP_W     320
#define POPUP_H     64

void hud::RenderPopup( void )
{
    // If timer has not expired render the popup
    if( m_PopupTimer > 0.0f )
    {
        f32     Alpha = m_PopupAlpha;
        irect   R;
        xcolor  c = m_PopupColor;

        if( m_PopupTimer < 0.25f )
            Alpha *= m_PopupTimer * 4.0f;
        c.A = (u8)(255*Alpha);

        // Render frame
        m_Font.FormattedTextSize( R, m_PopupMessage );
        R.Translate( (m_Width - R.GetWidth()) / 2, (m_Height - R.GetHeight() - 100) );
        R.Inflate( 50, 20 );
        RenderWindow( R, Alpha );
        R.Deflate( 8, 8 );
        R.Translate( 0, 2 );
        m_Font.DrawFormattedText( R, hud_font::h_center|hud_font::v_center, c, m_PopupMessage );
    }
}

//=========================================================================

void hud::Render( void )
{
    // Get out if we are not yet initialized
    if( !m_Initialized )
        return;

    // Get window dimensions
    s32 X0,Y0,X1,Y1;
    eng_GetActiveView(0)->GetViewport( X0, Y0, X1, Y1 );
    m_XOffset = X0;
    m_YOffset = Y0;
    m_Width  = X1-X0;
    m_Height = Y1-Y0;

    // Clear Player Pointer
    m_pPlayer = NULL;

    // Get Pointer to Player Object
    m_pPlayer = (player_object*)ObjMgr.GetObjectFromID( object::id( tgl.PC[tgl.WC].PlayerIndex, -1 ) );

    if( m_pPlayer )
    {
        m_Ammo[0] = m_pPlayer->GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_DISK    );
        m_Ammo[1] = m_pPlayer->GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_PLASMA  );
        m_Ammo[2] = -1;
        m_Ammo[3] = m_pPlayer->GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_MORTAR  );
        m_Ammo[4] = m_pPlayer->GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_GRENADE );
        m_Ammo[5] = m_pPlayer->GetInventCount( player_object::INVENT_TYPE_WEAPON_AMMO_BULLET  );
        m_Ammo[6] = -1;
        m_Ammo[7] = -1;
        m_Ammo[8] = -1;
        m_Ammo[9] = -1;

        // Begin rendering
        eng_Begin();

#ifdef TARGET_PS2
        gsreg_Begin();
        gsreg_SetClamping( TRUE );
//        gsreg_SetMipEquation( FALSE, 1.0f, 0, MIP_MAG_POINT, MIP_MIN_POINT );
        gsreg_End();
#endif
/*
        // Render Text Window
        if( tgl.WC == 0 )
        {
            if( tgl.NLocalPlayers==2 )
                RenderChat( HUD_CHAT_WINDOW_X, m_YOffset+m_Height-32 );
            else
                RenderChat( HUD_CHAT_WINDOW_X, HUD_CHAT_WINDOW_Y );
        }

        // Render Compass
        RenderCompass( m_Width - 56 - 16, 16 );
*/
        // Render Weapons & Reticle
        RenderWeapons( HUD_WEAPONS_X, HUD_WEAPONS_Y );

        // Render Inventory Menu
        if( tgl.WC == 0 )
        if( m_MenuMode == menu_inventory )
        {
            RenderInventory( HUD_PAUSE_WINDOW_X, HUD_PAUSE_WINDOW_Y );
        }

        // Render Voice Menu
        if( tgl.WC == 0 )
        if( m_MenuMode == menu_voice )
        {
            RenderVoiceMenu( HUD_PAUSE_WINDOW_X, HUD_PAUSE_WINDOW_Y, m_pVoiceTable );
        }

        // Render Popup
        if( tgl.WC == 0 )
        if( m_PopupTimer > 0.0f )
        {
            RenderPopup();
        }

#ifdef TARGET_PS2
        gsreg_Begin();
        gsreg_SetClamping( FALSE );
        gsreg_End();
#endif

        eng_End();
    }
}

//=========================================================================
//  MENU SYSTEM
//=========================================================================

s32 hud::VoiceCommandSelect( s32 Selection )
{
    s32     ID = 0;
    ASSERT( Selection >=  0 );
    ASSERT( Selection <  12 );

    if( m_pVoiceTable )
    {
        // Get ID
        ID = m_pVoiceTable->Commands[Selection].ID;

        // Check for terminal node or interior node
        if( ID != 0 )
        {
            // Terminal node, clear menu mode
            m_pVoiceCommand = &m_pVoiceTable->Commands[Selection];
            m_pVoiceTable   = NULL;
            m_MenuMode      = menu_null;
        }
        else
        {
            // Follow tree to next menu
            if( m_pVoiceTable->Commands[Selection].pText )
                m_pVoiceTable = (hud::voice_table*)m_pVoiceTable->Commands[Selection].pText;
        }
    }

    // Return ID read
    return ID;
}

//=========================================================================

s32 hud::GetVoiceCommand( void )
{
    s32 Command = 0;

    if( m_pVoiceCommand )
        Command = m_pVoiceCommand->ID;

    return Command;
}

//=========================================================================

s32 hud::GetInventorySelection( void )
{
    return m_InventorySelection;
}

//=========================================================================

#define DEADZONE    0.4f
#define ACTIVEZONE  0.6f

void hud::MenuInputInit( f32 LX, f32 LY, f32 RX, f32 RY )
{
    m_MenuLX = ( fabs(LX) <= DEADZONE );
    m_MenuLY = ( fabs(LY) <= DEADZONE );
    m_MenuRX = ( fabs(RX) <= DEADZONE );
    m_MenuRY = ( fabs(RY) <= DEADZONE );
}

//=========================================================================

void hud::MenuInput( f32 LX, f32 LY, f32 RX, f32 RY,
                     xbool DU, xbool DD, xbool DL, xbool DR,
                     xbool B1, xbool B2, xbool B3, xbool B4 )
{
    xbool   LU = FALSE;
    xbool   LD = FALSE;
    xbool   LR = FALSE;
    xbool   LL = FALSE;
    xbool   RU = FALSE;
    xbool   RD = FALSE;
    xbool   RR = FALSE;
    xbool   RL = FALSE;

    (void)DL;
    (void)DR;

    // Check for entering deadzones to enable sticks
    if( fabs(LX) <= DEADZONE ) m_MenuLX = TRUE;
    if( fabs(LY) <= DEADZONE ) m_MenuLY = TRUE;
    if( fabs(RX) <= DEADZONE ) m_MenuRX = TRUE;
    if( fabs(RY) <= DEADZONE ) m_MenuRY = TRUE;

    // Check for activations
    if( m_MenuLX )
    {
        if( LX < - ACTIVEZONE )
        {
            LL = TRUE;
            m_MenuLX = FALSE;
        }
        if( LX >  ACTIVEZONE )
        {
            LR = TRUE;
            m_MenuLX = FALSE;
        }
    }

    // Check for activations
    if( m_MenuLY )
    {
        if( LY < -ACTIVEZONE )
        {
            LD = TRUE;
            m_MenuLY = FALSE;
        }
        if( LY >  ACTIVEZONE )
        {
            LU = TRUE;
            m_MenuLY = FALSE;
        }
    }

    // Check for activations
    if( m_MenuRX )
    {
        if( RX < -ACTIVEZONE )
        {
            RL = TRUE;
            m_MenuRX = FALSE;
        }
        if( RX >  ACTIVEZONE )
        {
            RR = TRUE;
            m_MenuRX = FALSE;
        }
    }

    // Check for activations
    if( m_MenuRY )
    {
        if( RY < -ACTIVEZONE )
        {
            RD = TRUE;
            m_MenuRY = FALSE;
        }
        if( RY >  ACTIVEZONE )
        {
            RU = TRUE;
            m_MenuRY = FALSE;
        }
    }

    // CHeck which menu is active
    if( m_MenuMode == menu_voice )
    {
        s32 Selection   = -1;
        s32 Command     = 0;

        // Check buttons
        if( LU )
            Selection = 0;
        if( LR )
            Selection = 1;
        if( LD )        
            Selection = 2;
        if( LL )
            Selection = 3;
        if( RU )
            Selection = 4;
        if( RR )
            Selection = 5;
        if( RD )
            Selection = 6;
        if( RL )
            Selection = 7;
        if( B1 )
            Selection = 8;
        if( B2 )
            Selection = 9;
        if( B3 )
            Selection = 10;
        if( B4 )
            Selection = 11;

        // Return the selected command
        if( Selection != -1 )
            Command = VoiceCommandSelect( Selection );
//        return Command&0x7fff;
    }
    else if( m_MenuMode == menu_inventory )
    {
        if( LU || RU || DU )
        {
            m_InventorySelection--;
            if( m_InventorySelection < 0 )
                m_InventorySelection = 2;
        }

        if( LD || RD || DD )
        {
            m_InventorySelection++;
            if( m_InventorySelection > 2 )
                m_InventorySelection = 0;
        }
    }
}

//=========================================================================

xbool hud::MenuIsActive( void )
{
    // Set voice table pointer
    return (m_MenuMode != menu_null);
}

//=========================================================================

xbool hud::VoiceMenuIsActive( void )
{
    // Set voice table pointer
    return (m_MenuMode == menu_voice);
}

//=========================================================================

void hud::ToggleVoiceMenu( void )
{
    // Clear menu mode if in voice command mode already, else goto voice command mode
    if( m_MenuMode == menu_voice )
        m_MenuMode = menu_null;
    else
        m_MenuMode = menu_voice;

    // Clear Voice Command
    m_pVoiceCommand = NULL;

    // Set voice table pointer
    if( m_MenuMode == menu_voice )
        m_pVoiceTable = &hud_vc_Root;
    else
        m_pVoiceTable = NULL;
}

//=========================================================================

void hud::ToggleInventoryMenu( void )
{
    // Clear menu mode if in inventory mode already, else goto inventory mode
    if( m_MenuMode == menu_inventory )
        m_MenuMode = menu_null;
    else
        m_MenuMode = menu_inventory;

    m_pVoiceCommand = NULL;
}

//=========================================================================

void hud::DisplayMessage( const char* pString, xcolor TextColor, f32 Time, f32 Alpha )
{
    // Reset popup timer
    m_PopupTimer   = Time;
    m_PopupColor   = TextColor;
    m_PopupAlpha   = Alpha;

    // Copy string
    x_strncpy( m_PopupMessage, pString, HUD_POPUP_LEN-1 );
}

//=========================================================================

void hud::ProcessTime( f32 Delta )
{
    // Decrement popup timer
    if( m_PopupTimer > 0.0f )
        m_PopupTimer -= Delta;
}

//=========================================================================
