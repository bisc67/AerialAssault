//=========================================================================
//
//  dlg_mission_load.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_Globals.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"

#include "dlg_MissionLoad.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"

//==============================================================================
// Mission Load Dialog
//==============================================================================

enum controls
{
    IDC_FRAME
};

ui_manager::control_tem MissionLoadControls[] =
{
    { IDC_FRAME,    IDS_NULL,   "frame",   6,   4, 468, 172, 0, 0, 0, 0, ui_win::WF_STATIC },
};

ui_manager::dialog_tem MissionLoadDialog =
{
    IDS_MISSION_LOAD,
    1, 1,
    sizeof(MissionLoadControls)/sizeof(ui_manager::control_tem),
    &MissionLoadControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

s16 s_RenderCalls = 0;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_mission_load_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "mission load", &MissionLoadDialog, &dlg_mission_load_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_mission_load_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_mission_load* pDialog = new dlg_mission_load;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_mission_load
//=========================================================================

dlg_mission_load::dlg_mission_load( void )
{
}

//=========================================================================

dlg_mission_load::~dlg_mission_load( void )
{
    // Clear exists flag
    fegl.MissionLoadDialogActive = FALSE;

    Destroy();
}

//=========================================================================

xbool dlg_mission_load::Create( s32                        UserID,
                                ui_manager*                pManager,
                                ui_manager::dialog_tem*    pDialogTem,
                                const irect&               Position,
                                ui_win*                    pParent,
                                s32                        Flags,
                                void*                      pUserData )
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    // Set Title
    if( fegl.pMissionDef )
        m_Label = xwstring("LOADING:");

    // Set exists flag
    fegl.MissionLoadDialogActive = TRUE;

    // Clear completion
    tgl.MissionLoadCompletion = 0.0f;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_mission_load::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

extern void GetMissionDefFromNameAndType( void );

void dlg_mission_load::Render( s32 ox, s32 oy )
{
    ui_manager* pUIManager = tgl.pUIManager;
    irect       br;
    irect       r;

    ui_dialog::Render( ox, oy );

    if( s_RenderCalls < 2 )
        tgl.JumpTime = TRUE;

    s_RenderCalls++;

#ifdef TARGET_PC
/*    // Adjust the text for the loading screen.
    s32 XRes, YRes;
    eng_GetRes( XRes, YRes );
    s32 midX = XRes>>1;
    s32 midY = YRes>>1;

    ox = midX - 256;
    oy = midY - 256;
*/
#endif
        
    GetMissionDefFromNameAndType();
    if( fegl.pMissionDef )
    {
        xcolor Color( 192,192,192,224 ); //128,224,255,255 );
        xcolor Color2( 224,224,224,224 ); //128,224,255,255 );
        const xwchar* pGameName = StringMgr( "ui", IDS_NULL );
        const xwchar* pGameDesc = StringMgr( "ui", IDS_NULL );

        m_Label = xwstring("LOADING: ") + StringMgr( "MissionName", fegl.pMissionDef->DisplayNameString );
        // TODO: Render Level Bitmap

        // Get String for GameType
        switch( fegl.pMissionDef->GameType )
        {
        case GAME_CTF:
            pGameName = StringMgr( "ui", IDS_CAPTURE_THE_FLAG );
            pGameDesc = StringMgr( "ui", IDS_RULES_CTF );
            break;
        case GAME_CNH:
            pGameName = StringMgr( "ui", IDS_CAPTURE_AND_HOLD );
            pGameDesc = StringMgr( "ui", IDS_RULES_CNH );       
            break;
        case GAME_TDM:
            pGameName = StringMgr( "ui", IDS_TEAM_DEATH_MATCH );
            pGameDesc = StringMgr( "ui", IDS_RULES_TDM );       
            break;
        case GAME_DM:
            pGameName = StringMgr( "ui", IDS_DEATH_MATCH );
            pGameDesc = StringMgr( "ui", IDS_RULES_DM );       
            break;
        case GAME_HUNTER:
            pGameName = StringMgr( "ui", IDS_HUNTER );
            pGameDesc = StringMgr( "ui", IDS_RULES_HUNT );       
            break;
        }

        // Get Enclosing Rect
        br.Set( m_Position.l + ox, m_Position.t + oy, m_Position.r + ox, m_Position.b + oy );

        r = br;
        r.Deflate( 8, 48 );

        // Render GameType
        pUIManager->RenderText( 2, r, ui_font::h_left|ui_font::v_top, Color2, pGameName );
        r.Translate( 0, 40 );

        // Get size of game type description
        irect rs;
        pUIManager->TextSize( 0, rs, pGameDesc, -1 );

        // Render game type description
        pUIManager->RenderText( 0, r, 0, Color, pGameDesc );
        r.Translate( 0, rs.GetHeight() + 16 );

        // Render Game Name
        pUIManager->RenderText( 2, r, ui_font::h_left|ui_font::v_top, Color2, StringMgr( "MissionName", fegl.pMissionDef->DisplayNameString ) );
        r.Translate( 0, 40 );

        // Render mission specifics
        pUIManager->RenderText( 0, r, 0, Color, StringMgr( "Objective", fegl.pMissionDef - fe_Missions ) );

        // Render loading completion
        r = br;
        r. Deflate( 8, 8 );
        r.SetWidth( 256 );
        r.t = r.b - 22;
        irect r2 = r;
        r2.Deflate( 2, 2 );
        f32 LoadCompletion = tgl.MissionLoadCompletion;
        if( LoadCompletion < 0.0f ) LoadCompletion = 0.0f;
        if( LoadCompletion > 1.0f ) LoadCompletion = 1.0f;
        r2.r = r2.l + (s32)(r2.GetWidth() * LoadCompletion);
        pUIManager->RenderRect( r2, FECOL_LOADING_BAR, FALSE );
        pUIManager->RenderElement( m_iElement, r, 0 );
        r.Translate( 0, -2 );
        pUIManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, xcolor(192,192,192,255), xwstring("Loading Mission") );
    }
}

//=========================================================================

void dlg_mission_load::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    (void)pWin;
    (void)Presses;
    (void)Repeats;
    (void)Code;
}

//=========================================================================

void dlg_mission_load::OnPadSelect( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_mission_load::OnPadBack( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_mission_load::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;
}

//=========================================================================

void dlg_mission_load::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
}

//=========================================================================
