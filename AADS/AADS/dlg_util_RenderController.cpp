//=========================================================================
//
//  dlg_util_rendercontroller.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "FrontEnd.hpp"
#include "fe_globals.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_font.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

//=========================================================================
//  Controller Element Elements
//=========================================================================

enum Elements
{
    CE_A,
    CE_B,
    CE_C,
    CE_D,
    CE_E,
    CE_F,
    CE_G,
    CE_H,
    CE_I,
    CE_J,
    CE_K
};

struct cpos
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

static const cpos ElementPositions[] =
{
    {   0,   0,  22,  14 },
    {  23,   0,  22,   9 },
    {  47,   0,  10,   8 },
    {  59,   0,  11,   9 },
    {  96,   0,  32,  31 },
    { 113,  34,  15,  16 },
    { 113,  51,  15,  15 },
    { 113,  67,  15,  16 },
    { 113,  84,  15,  15 },
    {   0,  22,  93, 106 },
    {  93,  22, -94, 106 },
};

struct crender
{
    s32 Flags;
    s32 ID;
    s32 x;
    s32 y;
};

static const crender RenderElements[] =
{
    { 0, CE_J,   1,  29 },
    { 0, CE_K,  94,  29 },

    { 0, CE_A,  32,   0 },
    { 0, CE_A, 135,   0 },
    { 0, CE_B,  32,  20 },
    { 0, CE_B, 135,  20 },

    { 0, CE_C,  76,  65 },
//    { 0, CE_C,  89,  81 },
    { 0, CE_D, 103,  64 },

    { 0, CE_E,  27,  54 },

    { 0, CE_F, 138,  47 },
    { 1, CE_G, 152,  62 },
    { 0, CE_H, 138,  75 },
    { 0, CE_I, 124,  62 },
};

struct ccallout
{
    s32 x1,y1;
    s32 x2,y2;
    s32 GadgetID;
    s16 FunctionID;
    s16 Shifted;
};

#define Q 16

static ccallout Callouts[] =
{
    {    -Q,   5,  31,   5, INPUT_PS2_BTN_L2,        -1, 0 },         // L2
    {    -Q,  25,  31,  25, INPUT_PS2_BTN_L1,        -1, 0 },         // L1
    {    -Q,  47,  42,  52, INPUT_PS2_BTN_L_UP,      -1, 0 },         // DPAD UP
    {    -Q,  69,  25,  69, INPUT_PS2_BTN_L_LEFT,    -1, 0 },         // DPAD LEFT
    {    -Q,  91,  42,  88, INPUT_PS2_BTN_L_DOWN,    -1, 0 },         // DPAD DOWN
    {    -Q, 111,  54,  80, INPUT_PS2_BTN_L_RIGHT,   -1, 0 },         // DPAD RIGHT
    {    -Q, 130,  69, 114, INPUT_PS2_STICK_LEFT_X,  -1, 0 },         // LEFT ANALOG
    {    -Q, -17,  80,  63, INPUT_PS2_BTN_SELECT,    -1, 0 },         // SELECT
    { 186+Q, -17, 109,  63, INPUT_PS2_BTN_START,     -1, 0 },         // START
    { 186+Q,   5, 160,   5, INPUT_PS2_BTN_R2,        -1, 0 },         // R2
    { 186+Q,  25, 160,  25, INPUT_PS2_BTN_R1,        -1, 0 },         // R1
    { 186+Q,  47, 145,  52, INPUT_PS2_BTN_TRIANGLE,  -1, 0 },         // TRIANGLE
    { 186+Q,  69, 171,  69, INPUT_PS2_BTN_CIRCLE,    -1, 0 },         // CIRCLE
    { 186+Q,  91, 145,  86, INPUT_PS2_BTN_CROSS,     -1, 0 },         // CROSS
    { 186+Q, 111, 132,  80, INPUT_PS2_BTN_SQUARE,    -1, 0 },         // SQUARE
    { 186+Q, 130, 119, 114, INPUT_PS2_STICK_RIGHT_X, -1, 0 },         // RIGHT ANALOG
};

static const char* ControlNames[] =
{
    "Shift",            // WARRIOR_CONTROL_SHIFT,
    "Move",             // WARRIOR_CONTROL_MOVE,
    "Look",             // WARRIOR_CONTROL_LOOK,
    "Free Look",        // WARRIOR_CONTROL_FREELOOK,
    "Jump",             // WARRIOR_CONTROL_JUMP,
    "Fire",             // WARRIOR_CONTROL_FIRE,
    "Jet",              // WARRIOR_CONTROL_JET,
    "Zoom",             // WARRIOR_CONTROL_ZOOM,
    "Cycle Weapon",     // WARRIOR_CONTROL_NEXT_WEAPON,
    "Grenade",          // WARRIOR_CONTROL_GRENADE,
    "Voice Chat",       // WARRIOR_CONTROL_VOICE_MENU,
    "Voice Chat",       // WARRIOR_CONTROL_VOICE_CHAT,
    "Mine",             // WARRIOR_CONTROL_MINE,
    "Use Pack",         // WARRIOR_CONTROL_USE_PACK,
    "Zoom In",          // WARRIOR_CONTROL_ZOOM_IN,
    "Zoom Out",         // WARRIOR_CONTROL_ZOOM_OUT,
    "Use Health",       // WARRIOR_CONTROL_USE_HEALTH,
    "Target Laser",     // WARRIOR_CONTROL_TARGET_LASER,
    "Drop Weapon",      // WARRIOR_CONTROL_DROP_WEAPON,
    "Drop Pack",        // WARRIOR_CONTROL_DROP_PACK,
    "Deploy Beacon",    // WARRIOR_CONTROL_DEPLOY_BEACON,
    "Drop Flag",        // WARRIOR_CONTROL_DROP_FLAG,
    "Suicide",          // WARRIOR_CONTROL_SUICIDE,
    "Options Menu",     // WARRIOR_CONTROL_OPTIONS,
    "Target Lock",      // WARRIOR_CONTROL_TARGET_LOCK,
    "\"Positive\"",     // WARRIOR_CONTROL_COMPLIMENT,
    "\"Negative\"",     // WARRIOR_CONTROL_TAUNT,
    "Exchange Item",    // WARRIOR_CONTROL_EXCHANGE,
};

//=========================================================================

void RenderController( const irect& Rect, warrior_setup* pWarriorSetup )
{
    s32 i;

    irect newRect = Rect;

#ifdef TARGET_PC
/*    // Change the position where the controller get drawn.
    s32 XRes, YRes;
    eng_GetRes( XRes, YRes );
    s32 midX = XRes>>1;
    s32 midY = YRes>>1;

    s32 dx = midX - 256;
    s32 dy = midY - 256;
    newRect.Translate( dx, dy );
*/
#endif
    // Is control shifted?
    xbool IsShifted = input_IsPressed( (input_gadget)pWarriorSetup->ControlEditData[WARRIOR_CONTROL_SHIFT].GadgetID );

    // Clear callouts
    for( i=0 ; i<(s32)(sizeof(Callouts)/sizeof(ccallout)) ; i++ )
    {
        Callouts[i].FunctionID = -1;
    }

    // Setup callouts from controller
    for( i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
    {
        s32 GadgetID = pWarriorSetup->ControlEditData[i].GadgetID;
        s32 Shift    = pWarriorSetup->ControlEditData[i].IsShifted;
        if( GadgetID != INPUT_UNDEFINED )
        {
            for( s32 j=0 ; j<(s32)(sizeof(Callouts)/sizeof(ccallout)) ; j++ )
            {
                if( (Callouts[j].GadgetID == GadgetID) )
                {
                    if( (Callouts[j].FunctionID == -1) || (IsShifted == Shift) )
                    {
                        Callouts[j].FunctionID = i;
                        Callouts[j].Shifted    = Shift;
                    }
                }
            }
        }
    }

    // Does this control config have a shift key?
    xbool HasShiftKey = (pWarriorSetup->ControlEditData[0].GadgetID != INPUT_UNDEFINED);

    // Get Alpha value
    xtick Ticks = x_GetTime();
    f32 Time = (f32)x_TicksToSec( Ticks );
    f32 A = x_sin( Time * PI * 2 ) * 0.5f + 0.5f;

    // Get frame position
    irect rf = newRect;
    rf.Translate( (rf.GetWidth() - 186) / 2, (rf.GetHeight() - 135) / 2 );

    s32 iController = tgl.pUIManager->FindElement( "controller" );
    ASSERT( iController != -1 );

    // Render the controller
    for( i=0 ; i<(s32)(sizeof(RenderElements)/sizeof(crender)) ; i++ )
    {
        s32     ID;
        irect   Pos;
        irect   UV;
        xcolor  Color = XCOLOR_WHITE;

        ID    = RenderElements[i].ID;
        Pos.l = RenderElements[i].x;
        Pos.t = RenderElements[i].y;
        Pos.r = Pos.l + x_abs(ElementPositions[ID].w);
        Pos.b = Pos.t + x_abs(ElementPositions[ID].h);
        UV.l  = ElementPositions[ID].x;
        UV.t  = ElementPositions[ID].y;
        UV.r  = ElementPositions[ID].x + ElementPositions[ID].w;
        UV.b  = ElementPositions[ID].y + ElementPositions[ID].h;

        Pos.Translate( rf.l, rf.t );

        if( RenderElements[i].Flags && HasShiftKey )
        {
            Color.A = (u8)(255.0f * A);
        }

        tgl.pUIManager->RenderElementUV( iController, Pos, UV, Color );
    }

    // Render the callouts
    for( i=0 ; i<(s32)(sizeof(Callouts)/sizeof(ccallout)) ; i++ )
    {
        if( Callouts[i].FunctionID != -1 )
        {
            f32     tx      = (f32)rf.l;
            f32     ty      = (f32)rf.t;
            vector2 t( tx,ty );
            f32     x1      = (f32)Callouts[i].x1;
            f32     y1      = (f32)Callouts[i].y1;
            f32     x2      = (f32)Callouts[i].x2;
            f32     y2      = (f32)Callouts[i].y2;
            xcolor  Color   = HUDCOL_TEXT_WHITE;

            // Build text rectangle & alignment
            irect TextRect;
            s32   Alignment;
            if( x1 < 0 )
            {
                TextRect.Set( (s32)x1-100, (s32)y1-8, (s32)x1-8, (s32)y1+4 );
                Alignment = ui_font::h_right|ui_font::v_center;
            }
            else
            {
                TextRect.Set( (s32)x1+8, (s32)y1-8, (s32)x1+100, (s32)y1+4 );
                Alignment = ui_font::h_left|ui_font::v_center;
            }
            TextRect.Translate( rf.l, rf.t );

            // Change color if shifted
            if( IsShifted )
            {
                if( Callouts[i].Shifted || (Callouts[i].FunctionID == WARRIOR_CONTROL_SHIFT) )
                    Color.A = 255;
                else
                    Color.A = 96;
            }

            // Render Text
            if( Callouts[i].FunctionID == WARRIOR_CONTROL_SHIFT )
                Color.A = (u8)(Color.A*A);

            tgl.pUIManager->RenderText( 0, TextRect, Alignment, Color, xwstring(ControlNames[Callouts[i].FunctionID]) );
            Color.A = Color.A / 2;

            // Render Line
            if( y1 == y2 )
            {
                // Straight Line
                rect r;
                r.Set( x1, y1, x2, y1+1 );
                r.Translate( t );
                draw_Rect( r, Color, TRUE );
            }
            else
            {
                // Need horizontal + vertical
                rect r;
                if( x1 < x2 )
                    r.Set( x1, y1, x2, y1+1 );
                else
                    r.Set( x2, y1, x1, y1+1 );
                r.Translate( t );
                draw_Rect( r, Color, TRUE );
                r.Set( x2, y1, x2+1, y2 );
                r.Translate( t );
                draw_Rect( r, Color, TRUE );
            }
        }
    }
}

//=========================================================================
