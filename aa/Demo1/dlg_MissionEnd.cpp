//=========================================================================
//
//  dlg_mission_end.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "fe_Globals.hpp"
#include "serverman.hpp"
#include "GameMgr\GameMgr.hpp"

#include "GameClient.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_listbox.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_button.hpp"

#include "dlg_MissionEnd.hpp"
#include "dlg_LoadSave.hpp"

#include "fe_colors.hpp"
#include "sm_common.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"

void ShowMissionLoadDialog( void );

//==============================================================================
// Mission Load Dialog
//==============================================================================

enum controls
{
    IDC_CONTINUE,
    IDC_FRAME1,
    IDC_FRAME2
};

ui_manager::control_tem MissionEndControls[] =
{
    { IDC_CONTINUE, IDS_CONTINUE,   "button", 376, 384,  96,  24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_FRAME1,   IDS_NULL,       "frame",    6,  44, 232, 334, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_FRAME2,   IDS_NULL,       "frame",  242,  44, 232, 334, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem MissionEndDialog =
{
    IDS_MISSION_END,
    1, 1,
    sizeof(MissionEndControls)/sizeof(ui_manager::control_tem),
    &MissionEndControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

struct score_sorter
{
    s32             Position;
    s32             PlayerIndex;
    game_score*     pGameScore;
};

//=========================================================================
//  Data
//=========================================================================

score_sorter    s_ScoreSorter[32];
extern xbool s_InventoryDirty;
extern xbool s_Inventory2Dirty;
extern warrior_setup   g_WarriorBackup[2];

//=========================================================================
//  Registration function
//=========================================================================

void dlg_mission_end_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "mission end", &MissionEndDialog, &dlg_mission_end_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_mission_end_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_mission_end* pDialog = new dlg_mission_end;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_mission_end
//=========================================================================

dlg_mission_end::dlg_mission_end( void )
{
}

//=========================================================================

dlg_mission_end::~dlg_mission_end( void )
{
    Destroy();

    // Clear exists flag
    fegl.MissionEndDialogActive = FALSE;

    if( g_SM.MissionLoading )
    {
        ShowMissionLoadDialog();
    }
}

//=========================================================================

static s32 fn_score_sorter( const void* p1, const void* p2 )
{
    score_sorter*   s1 = (score_sorter*)p1;
    score_sorter*   s2 = (score_sorter*)p2;

    player_score*   ps1 = &(s1->pGameScore->Player[s1->PlayerIndex]);
    player_score*   ps2 = &(s2->pGameScore->Player[s2->PlayerIndex]);

    if( !ps1->IsInGame && !ps2->IsInGame )
        return 0;

    if( !ps1->IsInGame )
        return 1;

    if( !ps2->IsInGame )
        return -1;

    if( ps1->Score < ps2->Score )
        return 1;
    else if( ps1->Score > ps2->Score )
        return -1;
    else
        return 0;
}

//=========================================================================

xbool dlg_mission_end::Create( s32                        UserID,
                               ui_manager*                pManager,
                               ui_manager::dialog_tem*    pDialogTem,
                               const irect&               Position,
                               ui_win*                    pParent,
                               s32                        Flags,
                               void*                      pUserData )
{
    xbool   Success = FALSE;
    s32     i;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_pContinue = (ui_button*)FindChildByID( IDC_CONTINUE );
    m_pFrame1   = (ui_frame*) FindChildByID( IDC_FRAME1 );
    m_pFrame2   = (ui_frame*) FindChildByID( IDC_FRAME2 );

    if( !*(xbool*)pUserData )
    {
        m_pContinue->SetFlag( WF_DISABLED, 1 );
        m_pContinue->SetFlag( WF_VISIBLE,  0 );
    }

    // Get Score & Sort
    m_GmScore = GameMgr.GetScore();

    // Setup Score Sorter and Sort
    for( i=0 ; i<32 ; i++ )
    {
        s_ScoreSorter[i].PlayerIndex = i;
        s_ScoreSorter[i].pGameScore  = &m_GmScore;
    }

    x_qsort( s_ScoreSorter, 32, sizeof(score_sorter), &fn_score_sorter );

    // Set Title from winning team
    m_WinningTeam = -1;
    xwchar Message[64];
    if( m_GmScore.IsTeamBased )
    {
        if( m_GmScore.Team[0].Score == m_GmScore.Team[1].Score )
        {
            x_wstrcpy( Message, StringMgr( "ui", IDS_GAME_OVER ) );
        }
        else
        {
            if( m_GmScore.Team[0].Score > m_GmScore.Team[1].Score )
            {
                m_WinningTeam = 0;
            }
            else
            {
                m_WinningTeam = 1;
            }
            x_wstrcpy( Message, StringMgr( "ui", IDS_TEAM_WINS_FRONT ) );
            x_wstrcat( Message, m_GmScore.Team[m_WinningTeam].Name );
            x_wstrcat( Message, StringMgr( "ui", IDS_TEAM_WINS_BACK ) );
        }
    }
    else
    {
        x_wstrcpy( Message, StringMgr( "ui", IDS_GAME_OVER ) );
    }

    // Set exists flag
    fegl.MissionEndDialogActive = TRUE;

    // Don't draw the highlight, unless you were a valid client or the server.
    m_Player1Index = -1;
    m_Player2Index = -1;

    if( tgl.NLocalPlayers==2 )
    {
        m_Player1Index = tgl.PC[0].PlayerIndex;
        m_Player2Index = tgl.PC[1].PlayerIndex;
    }
    else
    {
        m_Player1Index = tgl.PC[0].PlayerIndex;
    }

    // Clear completion
    tgl.MissionLoadCompletion = 0.0f;

    // If we are saving don't destroy the dialog till we are done.
    m_WaitOnSave = FALSE;

    // Disable the continue button, this might work because the MissionLoading flag might not be set and this point.
    if( g_SM.MissionLoading )
    {
        m_pContinue->SetFlag( WF_DISABLED, 1 );
    }

    // Backup the warrior configs
    x_memcpy( &g_WarriorBackup[0], &fegl.WarriorSetups[0], sizeof(g_WarriorBackup[0]) );
    x_memcpy( &g_WarriorBackup[1], &fegl.WarriorSetups[1], sizeof(g_WarriorBackup[1]) );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_mission_end::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_mission_end::Render( s32 ox, s32 oy )
{
    s32     i;
    irect       rl[2];

    ui_dialog::Render( ox, oy );

    // Get Score Rectangles
    rl[0] = m_pFrame1->GetPosition( );
    rl[1] = m_pFrame2->GetPosition( );
#if defined(PAL_RELEASE)
    rl[0].t += 32;
    rl[1].t += 32;
#endif

    LocalToScreen( rl[0] );
    LocalToScreen( rl[1] );
    rl[0].Deflate( 4, 4 );
    rl[1].Deflate( 4, 4 );
    rl[0].SetHeight( 16 );
    rl[1].SetHeight( 16 );

    const xwchar* t1 = StringMgr( "ui", IDS_PLAYER );
    const xwchar* t2 = StringMgr( "ui", IDS_SCORE  );
    const xwchar* t3 = StringMgr( "ui", IDS_KILLS  );

    xcolor  c[3];
    c[0] = FECOL_HOME;
    c[1] = FECOL_AWAY;
    c[2] = FECOL_NEUTRAL;

    // Set Label 3 to Flags in HUNTER
    if( m_GmScore.GameType == GAME_HUNTER )
        t3 = StringMgr( "ui", IDS_FLAGS );

/*    if( tgl.ClientPresent )
    {
        object::id ID0 = tgl.pClient->GetPlayerObjID(0);
        object::id ID1 = tgl.pClient->GetPlayerObjID(1);

        m_Player1Index = (ID0.Seq == -1) ? -1 : ID0.Slot;
        m_Player2Index = (ID1.Seq == -1) ? -1 : ID1.Slot;
    }
    else
    {
*/
//    }

    // Render Team Scores if team based game
    if( m_GmScore.IsTeamBased )
    {
        s32     MyTeam = 0;
        s32     Team;

        ASSERT( (MyTeam == 0) || (MyTeam == 1) );

        // Render Player Score info
        for( Team=0 ; Team<2 ; Team++ )
        {
            // Render Team Score
            irect   r = rl[Team];
            r.Deflate( 4, 0 );
            m_pManager->RenderText( 0, r, ui_font::h_left|ui_font::v_center, c[2], xwstring(m_GmScore.Team[Team].Name) );
            m_pManager->RenderText( 0, r, ui_font::h_right|ui_font::v_center, c[2], xwstring(xfs("%d",m_GmScore.Team[Team].Score)) );
            draw_Begin( DRAW_LINES, DRAW_2D|DRAW_USE_ALPHA|DRAW_NO_ZBUFFER );
            draw_Color( xcolor(0,0,0,64) );
            r.l -= 4;
            r.r += 4;
            draw_Vertex( vector3( (f32)r.l, (f32)r.b+1, 0.0f) );
            draw_Vertex( vector3( (f32)r.r, (f32)r.b+1, 0.0f) );
            draw_End();
            rl[Team].Translate( 0, rl[Team].GetHeight() + 2 );

            // Render Score Titles
            RenderScoreTitle( rl[Team], XCOLOR_WHITE, t1, t2, t3 );
            rl[Team].Translate( 0, rl[Team].GetHeight() + 2 );

            // Loop through teams players
            s32 ScoreIndex = 0;
            for( i=0 ; (i<19) && (ScoreIndex < 32) && (m_GmScore.Player[s_ScoreSorter[ScoreIndex].PlayerIndex].IsInGame) ; i++ )
            {
                // Find next player on the team we are looking at
                while( (ScoreIndex < 32) && ((m_GmScore.Player[s_ScoreSorter[ScoreIndex].PlayerIndex].Team ^ Team) != MyTeam) )
                    ScoreIndex++;

                // If a player found then print the score details and advance ScoreIndex
                if( (ScoreIndex < 32) && (m_GmScore.Player[s_ScoreSorter[ScoreIndex].PlayerIndex].IsInGame) )
                {
                    player_score& Player = m_GmScore.Player[s_ScoreSorter[ScoreIndex].PlayerIndex];

                    // Change the player color.
                    xcolor color;
                    color.Set( XCOLOR_YELLOW );

                    // If its a local player then render their strings
                    if( (s_ScoreSorter[ScoreIndex].PlayerIndex == m_Player1Index) || (s_ScoreSorter[ScoreIndex].PlayerIndex == m_Player2Index) )
                        RenderPlayerScore( rl[Team], color, i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                    else
                        RenderPlayerScore( rl[Team], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Kills );

                    rl[Team].Translate( 0, rl[Team].GetHeight() );
                    ScoreIndex++;
                }
            }
        }
    }
    else
    {
        RenderScoreTitle( rl[0], XCOLOR_WHITE, t1, t2, t3 );
        RenderScoreTitle( rl[1], XCOLOR_WHITE, t1, t2, t3 );
#if defined(PAL_RELEASE)
        // AHHH! What we have to do to maintain code alignment between PAL/NTSC!
        // Some funky hacking here.............
        //s32 t;
        //t = rl[0].GetHeight()+2;
        rl[0].Translate( 0, 18 );
        rl[1].Translate( 0, 18 );
#else
        rl[0].Translate( 0, rl[0].GetHeight() + 2 );
        rl[1].Translate( 0, rl[1].GetHeight() + 2 );
#endif

        // Render Player Score info
        for( i=0 ; (i<32) && (m_GmScore.Player[s_ScoreSorter[i].PlayerIndex].IsInGame) ; i++ )
        {
            s32 iRect = i/19;
            player_score& Player = m_GmScore.Player[s_ScoreSorter[i].PlayerIndex];

            // Change the player color.
            xcolor color;
            color.Set( XCOLOR_YELLOW );

            if( (s_ScoreSorter[i].PlayerIndex == m_Player1Index) || (s_ScoreSorter[i].PlayerIndex == m_Player2Index) )            
            {
                if( m_GmScore.GameType == GAME_HUNTER )
                    RenderPlayerScore( rl[iRect], color, i+1, xwstring(Player.Name), Player.Score, Player.Deaths );
                else
                    RenderPlayerScore( rl[iRect], color, i+1, xwstring(Player.Name), Player.Score, Player.Kills );
            }
            else
            {
                if( m_GmScore.GameType == GAME_HUNTER )
                    RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Deaths );
                else
                    RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Kills );
            }

            rl[iRect].Translate( 0, rl[iRect].GetHeight() );
        }
    }

    // Get Enclosing Rect
#if !defined(PAL_RELEASE)
    irect br;
    br.Set( m_Position.l + ox, m_Position.t + oy, m_Position.r + ox, m_Position.b + oy );
#endif

    // Render loading completion
    if( g_SM.MissionLoading )
    {
//        m_pContinue->SetFlag( WF_DISABLED, 1 );
//        m_pContinue->SetFlag( WF_VISIBLE,  0 );
#if !defined(PAL_RELEASE)
        irect r = br;
#else
        irect r;
        r.Set( m_Position.l + ox, m_Position.t + oy, m_Position.r + ox, m_Position.b + oy );
#endif
        r. Deflate( 8, 8 );
        r.SetWidth( 256 );
        r.t = r.b - 22;
        irect r2 = r;
        r2.Deflate( 2, 2 );
        f32 LoadCompletion = tgl.MissionLoadCompletion;
        if( LoadCompletion < 0.0f ) LoadCompletion = 0.0f;
        if( LoadCompletion > 1.0f ) LoadCompletion = 1.0f;
        r2.r = r2.l + (s32)(r2.GetWidth() * LoadCompletion);
        m_pManager->RenderRect( r2, FECOL_LOADING_BAR, FALSE );
        m_pManager->RenderElement( m_iElement, r, 0 );
        r.Translate( 0, -2 );
        const xwchar* pName = NULL;
        if( fegl.pMissionDef )
            pName = StringMgr( "MissionName", fegl.pMissionDef->DisplayNameString );
        if( fegl.pMissionDef && pName )
            m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, xcolor(192,192,192,255), xwstring( "Loading " ) + pName );
        else
            m_pManager->RenderText( 0, r, ui_font::h_center|ui_font::v_center, xcolor(192,192,192,255), xwstring(xfs("Loading Mission")) );
    }
}

//=========================================================================

void dlg_mission_end::RenderScoreTitle( const irect& rl, const xcolor& Color, const xwchar* Title1, const xwchar* Title2, const xwchar* Title3 )
{
    irect   rp = rl;
    irect   rn = rl;
    irect   rs = rl;
    irect   rk = rl;

    rp.r = rn.l + 20;

    rn.l = rp.r + 8;
    rn.r = rn.l + 104;

    rs.l = rn.r;
    rs.r = rs.l + 45;

    rk.l = rs.r;
    rk.r = rk.l + 44;

    m_pManager->RenderText( 0, rn, ui_font::h_left |ui_font::v_center, Color, Title1 );
    m_pManager->RenderText( 0, rs, ui_font::h_right|ui_font::v_center, Color, Title2 );
    m_pManager->RenderText( 0, rk, ui_font::h_right|ui_font::v_center, Color, Title3 );
}

//=========================================================================

void dlg_mission_end::RenderPlayerScore( const irect& rl, const xcolor& Color, s32 Pos, const xwchar* Name, s32 Score, s32 Kills )
{
    irect   rp = rl;
    irect   rn = rl;
    irect   rs = rl;
    irect   rk = rl;

    rp.r = rn.l + 20;

    rn.l = rp.r + 8;
    rn.r = rn.l + 112;

    rs.l = rn.r - 8;
    rs.r = rs.l + 45;

    rk.l = rs.r;
    rk.r = rk.l + 44;

    draw_Begin( DRAW_LINES, DRAW_2D|DRAW_USE_ALPHA|DRAW_NO_ZBUFFER );
    draw_Color( xcolor(0,0,0,64) );
    draw_Vertex( vector3( (f32)rl.l, (f32)rl.t+2, 0.0f) );
    draw_Vertex( vector3( (f32)rl.r, (f32)rl.t+2, 0.0f) );
    draw_End();

    m_pManager->RenderText( 0, rp, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Pos)) );
    m_pManager->RenderText( 0, rn, ui_font::h_left |ui_font::v_center|ui_font::clip_ellipsis, Color, Name );
    m_pManager->RenderText( 0, rs, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Score)) );
    m_pManager->RenderText( 0, rk, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Kills)) );
}

//=========================================================================

void dlg_mission_end::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_mission_end::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    if( pWin == (ui_win*)m_pContinue )
    {
        if( s_InventoryDirty || s_Inventory2Dirty )
        {
            if( tgl.NLocalPlayers == 2 )
            {
                
                if( s_InventoryDirty  )
                {
                    load_save_params options(0,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[0]);
                    m_pManager->OpenDialog( m_UserID, 
                                          "warriorloadsave", 
                                          irect(0,0,300,264), 
                                          NULL, 
                                          ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                          (void *)&options);
                    s_InventoryDirty = FALSE;
                }
                else if( s_Inventory2Dirty )
                {
                    m_WaitOnSave = TRUE;
                    load_save_params options(1,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[1]);
                    m_pManager->OpenDialog( m_UserID, 
                                          "warriorloadsave", 
                                          irect(0,0,300,264), 
                                          NULL, 
                                          ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                          (void *)&options);
            
                    s_Inventory2Dirty = FALSE;

                }
            }
            else
            {
                m_WaitOnSave = TRUE;

                load_save_params options(0,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[0]);
                m_pManager->OpenDialog( m_UserID, 
                                      "warriorloadsave", 
                                      irect(0,0,300,264), 
                                      NULL, 
                                      ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                      (void *)&options);
                s_InventoryDirty = FALSE;
            }
        }
        else
        {
            m_pManager->EndDialog( m_UserID, TRUE );
        }
    }
}

//=========================================================================

void dlg_mission_end::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    if( s_InventoryDirty || s_Inventory2Dirty )
    {
        if( tgl.NLocalPlayers == 2 )
        {
            if( s_InventoryDirty  )
            {
                load_save_params options(0,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[0]);
                m_pManager->OpenDialog( m_UserID, 
                                      "warriorloadsave", 
                                      irect(0,0,300,264), 
                                      NULL, 
                                      ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                      (void *)&options);
                s_InventoryDirty = FALSE;
            }
            else if( s_Inventory2Dirty )
            {
                m_WaitOnSave = TRUE;
                load_save_params options(1,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[1]);
                m_pManager->OpenDialog( m_UserID, 
                                      "warriorloadsave", 
                                      irect(0,0,300,264), 
                                      NULL, 
                                      ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                      (void *)&options);
        
                s_Inventory2Dirty = FALSE;
            }
        }
        else
        {
            m_WaitOnSave = TRUE;

            load_save_params options(0,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[0]);
            m_pManager->OpenDialog( m_UserID, 
                                  "warriorloadsave", 
                                  irect(0,0,300,264), 
                                  NULL, 
                                  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                  (void *)&options);
            s_InventoryDirty = FALSE;
        }
    }
    else
    {
        m_pManager->EndDialog( m_UserID, TRUE );
    }

//    m_pManager->EndDialog( m_UserID, TRUE );
}

//=========================================================================

void dlg_mission_end::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    // The save dialog will return this command.
    if (Command == WN_USER)
    {
        if( tgl.NLocalPlayers == 2 && s_Inventory2Dirty )
        {
            m_WaitOnSave = TRUE;
            load_save_params options(1,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[1]);
            
            m_pManager->OpenDialog( m_UserID, 
                                  "warriorloadsave", 
                                  irect(0,0,300,264), 
                                  NULL, 
                                  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                                  (void *)&options);
            
            s_Inventory2Dirty = FALSE;
        }

        if( !m_WaitOnSave )
        {
            m_pManager->EndDialog( m_UserID, TRUE );
        }
    }

}

//=========================================================================

void dlg_mission_end::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
}

//=========================================================================
