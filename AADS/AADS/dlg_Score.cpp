//=========================================================================
//
//  dlg_score.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_frame.hpp"

#include "dlg_score.hpp"

#include "GameMgr/GameMgr.hpp"
#include "StringMgr\StringMgr.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"

//=========================================================================
//  Score Dialog
//=========================================================================

enum controls
{
    IDC_FRAME1,
    IDC_FRAME2,
    IDC_FRAME3
};

ui_manager::control_tem ScoreControls[] =
{
    { IDC_FRAME1,   IDS_NULL,   "frame",   6,   4, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_FRAME2,   IDS_NULL,   "frame",   6,   4, 232, 172, 0, 0, 0, 0, ui_win::WF_STATIC },
    { IDC_FRAME3,   IDS_NULL,   "frame", 242,   4, 232, 172, 0, 0, 0, 0, ui_win::WF_STATIC },
};

ui_manager::dialog_tem ScoreDialog =
{
    IDS_NULL,
    0, 0,
    sizeof(ScoreControls)/sizeof(ui_manager::control_tem),
    &ScoreControls[0],
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

//=========================================================================
//  Registration function
//=========================================================================

void dlg_score_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "score", &ScoreDialog, &dlg_score_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_score_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_score* pDialog = new dlg_score;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_score
//=========================================================================

dlg_score::dlg_score( void )
{
}

//=========================================================================

dlg_score::~dlg_score( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_score::Create( s32                        UserID,
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

    m_pFrame  = (ui_frame*)FindChildByID( IDC_FRAME1 );
    m_pFrame2 = (ui_frame*)FindChildByID( IDC_FRAME2 );
    m_pFrame3 = (ui_frame*)FindChildByID( IDC_FRAME3 );
    ASSERT( m_pFrame  );
    ASSERT( m_pFrame2 );
    ASSERT( m_pFrame3 );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_score::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

struct score_sorter
{
    s32             Position;
    s32             PlayerIndex;
    game_score*     pGameScore;
};

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

void dlg_score::Render( s32 ox, s32 oy )
{
    s32     i;

    if( m_Flags & WF_VISIBLE )
    {
        // Get Rectangle
        irect   r = m_pFrame->GetPosition();
        r.Deflate( 4, 2 );
        LocalToScreen( r );


#ifdef TARGET_PC
/*        // Change where the vehicles get drawn according to the resolution.
        s32 XRes, YRes;
        eng_GetRes( XRes, YRes );
        s32 midX = XRes>>1;
        s32 midY = YRes>>1;

        s32 dx = midX - 256;
        s32 dy = midY - 256;
        r.Translate( dx, dy );
*/
#endif
        const irect& br = m_pManager->GetUserBounds( m_UserID );
        r.Translate( br.l, br.t );

        // Get Score Info from Game Manager
        game_score GmScore = GameMgr.GetScore();

        // Get Player Object & Team
        player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
        s32 MyPlayerIndex = pPlayer->GetPlayerIndex();
        s32 MyTeam = GmScore.Player[MyPlayerIndex].Team;
        
        // Draw a highlight around the player.
        xbool   DrawHighlight = TRUE;

        // Setup score sort structs
        score_sorter ScoreSorter[32];
        for( i=0 ; i<32 ; i++ )
        {
            ScoreSorter[i].PlayerIndex = i;
            ScoreSorter[i].pGameScore  = &GmScore;
        }

        // Sort players by score
//        x_qsort( GmScore.Player, 32, sizeof(player_score), &score_sorter );
        x_qsort( ScoreSorter, 32, sizeof(score_sorter), &fn_score_sorter );

        m_pFrame ->SetFlag( ui_win::WF_VISIBLE, 0 );//!GmScore.IsTeamBased );
        m_pFrame2->SetFlag( ui_win::WF_VISIBLE, 1 );// GmScore.IsTeamBased );
        m_pFrame3->SetFlag( ui_win::WF_VISIBLE, 1 );// GmScore.IsTeamBased );

        ui_dialog::Render( ox, oy );

        // Get rectangles ready to render scores
        irect   rl[2];

        rl[0] = r;
        rl[1] = r;
        rl[0].SetHeight( 16 );
        rl[1].SetHeight( 16 );
        rl[1].l = rl[1].l + 236;
        rl[0].SetWidth( 232-8 );
        rl[1].SetWidth( 232-8 );

        const xwchar* t1 = StringMgr( "ui", IDS_PLAYER );
        const xwchar* t2 = StringMgr( "ui", IDS_SCORE  );
        const xwchar* t3 = StringMgr( "ui", IDS_KILLS  );

        xcolor  c[3];
        c[0] = FECOL_HOME;       // Home
        c[1] = FECOL_AWAY;       // Away
        c[2] = FECOL_NEUTRAL;    // Neutral

        // Set Label 3 to Flags in HUNTER
        if( GmScore.GameType == GAME_HUNTER )
            t3 = StringMgr( "ui", IDS_FLAGS );

        // Render Team Scores if team based game
        if( GmScore.IsTeamBased )
        {
            s32     Team;

            ASSERT( (MyTeam == 0) || (MyTeam == 1) );

            // Render Player Score info
            for( Team=0 ; Team<2 ; Team++ )
            {
                // Render Team Score
                irect   r = rl[Team];
                r.Deflate( 4, 0 );
                m_pManager->RenderText( 0, r, ui_font::h_left|ui_font::v_center, c[Team], xwstring(GmScore.Team[MyTeam^Team].Name) );
                m_pManager->RenderText( 0, r, ui_font::h_right|ui_font::v_center, c[Team], xwstring(xfs("%d",GmScore.Team[MyTeam^Team].Score)) );
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
                for( i=0 ; (i<8) && (ScoreIndex < 32) && (GmScore.Player[ScoreSorter[ScoreIndex].PlayerIndex].IsInGame) ; i++ )
                {
                    // Find next player on the team we are looking at
                    while( (ScoreIndex < 32) && ((GmScore.Player[ScoreSorter[ScoreIndex].PlayerIndex].Team ^ Team) != MyTeam) )
                        ScoreIndex++;

                    // If a player found then print the score details and advance ScoreIndex
                    if( (ScoreIndex < 32) && (GmScore.Player[ScoreSorter[ScoreIndex].PlayerIndex].IsInGame) )
                    {
                        player_score& Player = GmScore.Player[ScoreSorter[ScoreIndex].PlayerIndex];
                        
                        if( DrawHighlight )
                        {
                            // Change the player color.
                            xcolor color;
                            color.Set( XCOLOR_YELLOW );
 
                            // This is the local player so change the color of his score, so its more visible.
                            if( MyPlayerIndex == ScoreSorter[ScoreIndex].PlayerIndex )
                            {
                                DrawHighlight = FALSE;
                                RenderPlayerScore( rl[Team], color, i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                            }
                            else
                            {
                                RenderPlayerScore( rl[Team], c[Team], i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                            }
                        }
                        else
                        {
                            RenderPlayerScore( rl[Team], c[Team], i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                        }

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
            rl[0].Translate( 0, rl[0].GetHeight() + 2 );
            rl[1].Translate( 0, rl[1].GetHeight() + 2 );

            // Render Player Score info
            for( i=0 ; (i<16) && (GmScore.Player[ScoreSorter[i].PlayerIndex].IsInGame) ; i++ )
            {
                s32 iRect = i/8;
                player_score& Player = GmScore.Player[ScoreSorter[i].PlayerIndex];

                // Only one highlight can be drawn.
                if( DrawHighlight )
                {
                    // Change the player color.
                    xcolor color;
                    color.Set( XCOLOR_YELLOW );

                    // This is the local player so change the color of his score, so its more visible.
                    if( MyPlayerIndex == ScoreSorter[i].PlayerIndex )
                    {
                        DrawHighlight = FALSE;
                        
                        if( GmScore.GameType == GAME_HUNTER )
                            RenderPlayerScore( rl[iRect], color, i+1, xwstring(Player.Name), Player.Score, Player.Deaths );
                        else
                            RenderPlayerScore( rl[iRect], color, i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                    }
                    else
                    {
                        if( GmScore.GameType == GAME_HUNTER )
                            RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Deaths );
                        else
                            RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                    }
                }
                else
                {                                    
                    if( GmScore.GameType == GAME_HUNTER )
                        RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Deaths );
                    else
                        RenderPlayerScore( rl[iRect], c[2], i+1, xwstring(Player.Name), Player.Score, Player.Kills );
                }

                rl[iRect].Translate( 0, rl[iRect].GetHeight() );
            }
        }
    }
}

//=========================================================================

void dlg_score::RenderScoreTitle( const irect& rl, const xcolor& Color, const xwchar* Title1, const xwchar* Title2, const xwchar* Title3 )
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

void dlg_score::RenderPlayerScore( const irect& rl, const xcolor& Color, s32 Pos, const xwchar* Name, s32 Score, s32 Kills )
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

//    m_pManager->RenderRect( rp, XCOLOR_RED  , FALSE );
//    m_pManager->RenderRect( rn, XCOLOR_GREEN, FALSE );
//    m_pManager->RenderRect( rs, XCOLOR_BLUE , FALSE );
//    m_pManager->RenderRect( rk, XCOLOR_BLACK, FALSE );

    m_pManager->RenderText( 0, rp, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Pos)) );
    m_pManager->RenderText( 0, rn, ui_font::h_left |ui_font::v_center|ui_font::clip_ellipsis, Color, Name );
    m_pManager->RenderText( 0, rs, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Score)) );
    m_pManager->RenderText( 0, rk, ui_font::h_right|ui_font::v_center, Color, xwstring(xfs("%d",Kills)) );
}
//=========================================================================
