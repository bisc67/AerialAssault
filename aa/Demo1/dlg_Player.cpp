//=========================================================================
//
//  dlg_player.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "FrontEnd.hpp"

#include "GameMgr\GameMgr.hpp"
#include "serverman.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_dlg_list.hpp"
#include "ui\ui_listbox.hpp"

#include "dlg_player.hpp"
#include "dlg_message.hpp"
#include "sm_common.hpp"

#include "objects\bot\botobject.hpp"

#include "data\ui\ui_strings.h"
#include "fe_colors.hpp"
#include "fe_globals.hpp"
#include "StringMgr\StringMgr.hpp"

#include "Demo1/SpecialVersion.hpp"
extern bot_object::bot_debug g_BotDebug;
extern xbool CTF_LOGIC_HUMANS_ON_SAME_TEAM;
extern server_manager* pSvrMgr;

//=========================================================================
//  Player Dialog
//=========================================================================

enum controls
{
    IDC_INVERT_PLAYER_Y,
    IDC_INVERT_VEHICLE_Y,
    IDC_DUALSHOCK,
    IDC_3RD_PLAYER,
    IDC_3RD_VEHICLE,
    IDC_EXIT,

    IDC_X_SENS_TEXT,
    IDC_X_SENS_SLIDER,

    IDC_CHANGE_TEAM,
    IDC_Y_SENS_TEXT,
    IDC_Y_SENS_SLIDER,

    IDC_VOTE_MAP,
    IDC_VOTE_KICK,
    IDC_VOTE_YES,
    IDC_VOTE_NO,
    IDC_SUICIDE,

    IDC_FRAME,
    IDC_FRAME2,
};

ui_manager::control_tem PlayerControls[] =
{
    { IDC_INVERT_PLAYER_Y,  IDS_INVERT_PLAYER_Y,    "check",   13,  10, 184,  24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_INVERT_VEHICLE_Y, IDS_INVERT_VEHICLE_Y,   "check",   13,  37, 184,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DUALSHOCK,        IDS_DUALSHOCK,          "check",   13,  64, 184,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_3RD_PLAYER,       IDS_3RD_PLAYER,         "check",   13,  91, 184,  24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_3RD_VEHICLE,      IDS_3RD_VEHICLE,        "check",   13, 118, 184,  24, 0, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_EXIT,             IDS_EXIT_GAME,          "button",  13, 145, 184,  24, 0, 5, 1, 1, ui_win::WF_VISIBLE },

    { IDC_SUICIDE,          IDS_SUICIDE,            "button", 202,  10, 131,  24, 1, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_CHANGE_TEAM,      IDS_CHANGE_TEAM,        "button", 338,  10, 131,  24, 2, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_VOTE_YES,         IDS_VOTE_YES,           "button", 202,  37, 131,  24, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_VOTE_NO,          IDS_VOTE_NO,            "button", 338,  37, 131,  24, 2, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_VOTE_KICK,        IDS_VOTE_KICK,          "button", 202,  64, 131,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_VOTE_MAP,         IDS_VOTE_MAP,           "button", 338,  64, 131,  24, 2, 2, 1, 1, ui_win::WF_VISIBLE },

    { IDC_X_SENS_TEXT,      IDS_X_SENSITIVITY,      "text",   198, 104, 130,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_X_SENS_SLIDER,    IDS_NULL,               "slider", 322, 104, 138,  24, 1, 3, 2, 1, ui_win::WF_VISIBLE },
    { IDC_Y_SENS_TEXT,      IDS_Y_SENSITIVITY,      "text",   198, 135, 130,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_Y_SENS_SLIDER,    IDS_NULL,               "slider", 322, 135, 138,  24, 1, 4, 2, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME2,           IDS_NULL,               "frame",  202,  93, 265,  76, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_FRAME,            IDS_NULL,               "frame",    6,   4, 468, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem PlayerDialog =
{
    IDS_NULL,
    3, 6,
    sizeof(PlayerControls)/sizeof(ui_manager::control_tem),
    &PlayerControls[0],
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

extern xbool s_InventoryDirty;
extern xbool s_Inventory2Dirty;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_player_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "player", &PlayerDialog, &dlg_player_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_player_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_player* pDialog = new dlg_player;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_player
//=========================================================================

dlg_player::dlg_player( void )
{
}

//=========================================================================

dlg_player::~dlg_player( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_player::Create( s32                        UserID,
                          ui_manager*                pManager,
                          ui_manager::dialog_tem*    pDialogTem,
                          const irect&               Position,
                          ui_win*                    pParent,
                          s32                        Flags,
                          void*                      pUserData )
{
    xbool   Success = FALSE;
    m_NoUpdate = TRUE;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_pInvertPlayerY  = (ui_check* )FindChildByID( IDC_INVERT_PLAYER_Y  );
    m_pInvertVehicleY = (ui_check* )FindChildByID( IDC_INVERT_VEHICLE_Y );
    m_p3rdPlayer      = (ui_check* )FindChildByID( IDC_3RD_PLAYER       );
    m_p3rdVehicle     = (ui_check* )FindChildByID( IDC_3RD_VEHICLE      );
    m_pDualshock      = (ui_check* )FindChildByID( IDC_DUALSHOCK        );
    m_pExit           = (ui_button*)FindChildByID( IDC_EXIT             );
    m_pChangeTeam     = (ui_button*)FindChildByID( IDC_CHANGE_TEAM      );
    m_pVoteMap        = (ui_button*)FindChildByID( IDC_VOTE_MAP         );
    m_pVoteKick       = (ui_button*)FindChildByID( IDC_VOTE_KICK        );
    m_pVoteYes        = (ui_button*)FindChildByID( IDC_VOTE_YES         );
    m_pVoteNo         = (ui_button*)FindChildByID( IDC_VOTE_NO          );
    m_pSensX          = (ui_slider*)FindChildByID( IDC_X_SENS_SLIDER    );
    m_pSensY          = (ui_slider*)FindChildByID( IDC_Y_SENS_SLIDER    );
    m_pSuicide        = (ui_button*)FindChildByID( IDC_SUICIDE          );
    m_iChangeMap    = -1;
    m_iKickPlayer   = -1;
    m_DoChangeTeam  = FALSE;
    m_DoSuicide     = FALSE;
    m_DoExitGame    = FALSE;

    // Disable change team if not a team based game
    if( (!GameMgr.IsTeamBasedGame()) || (GameMgr.GetCampaignMission() != -1) )
        m_pChangeTeam->SetFlag( WF_DISABLED, TRUE );

    // Get Player Object
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );
    m_WarriorID = pPlayer->GetWarriorID();

    if( !pPlayer->GetVoteCanStart() )
    {
        m_pVoteMap ->SetFlag( WF_DISABLED, 1 );
        m_pVoteKick->SetFlag( WF_DISABLED, 1 );
    }
    
    if( fegl.State != FE_STATE_ONLINE )
    {
        m_pVoteMap ->SetFlag( WF_DISABLED, 1 );
        m_pVoteKick->SetFlag( WF_DISABLED, 1 );
    }

    #ifdef DEMO_DISK_HARNESS
    m_pVoteMap ->SetFlag( WF_DISABLED, 1 );
    #endif
    if( tgl.ServerPresent && (fegl.GameMode != FE_MODE_CAMPAIGN) && fegl.GameModeOnline )
    {
        m_pExit->SetFlag( WF_DISABLED, TRUE );
    }

    if( !pPlayer->GetVoteCanCast() )
    {
        m_pVoteYes->SetFlag( WF_DISABLED, 1 );
        m_pVoteNo ->SetFlag( WF_DISABLED, 1 );
    }

    m_pSensX->SetRange( 1, 10 );
    m_pSensY->SetRange( 1, 10 );
    m_pSensX->SetStep ( 1 );
    m_pSensY->SetStep ( 1 );
    m_pSensX->SetValue( fegl.WarriorSetups[m_WarriorID].SensitivityX );
    m_pSensY->SetValue( fegl.WarriorSetups[m_WarriorID].SensitivityY );

    m_p3rdPlayer ->SetSelected( fegl.WarriorSetups[m_WarriorID].View3rdPlayer  );
    m_p3rdVehicle->SetSelected( fegl.WarriorSetups[m_WarriorID].View3rdVehicle );

    m_NoUpdate = FALSE;
    
    WarriorToControls();
    
    if( GameMgr.IsCampaignMission() )
    {
        m_pChangeTeam->SetFlag( WF_DISABLED, 1 );
        m_pSuicide->SetFlag( WF_DISABLED, 1 );
    }

    // Return success code
    return Success;
}

//=========================================================================

void dlg_player::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_player::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_player::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    ui_dialog::OnUpdate( pWin, DeltaTime );

    xbool Flag;
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );

    // Kick the player
    if( m_iKickPlayer != -1 )
    {
        if( pPlayer )
            pPlayer->VoteKickPlayer( m_iKickPlayer );

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        return;
    }

    // Change the map
    if( m_iChangeMap != -1 )
    {
        if( pPlayer )
            pPlayer->VoteChangeMap( m_iChangeMap );

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        return;
    }

    // Change Team
    if( m_DoChangeTeam == DLG_MESSAGE_YES )
    {
        // Change Team
        pPlayer->RequestChangeTeam();

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        return;
    }

    // Suicide
    if( m_DoSuicide == DLG_MESSAGE_YES )
    {
        // Suicide
        pPlayer->CommitSuicide();

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        return;
    }

    // Exit Gmae
    if( m_DoExitGame == DLG_MESSAGE_YES )
    {
        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );

        tgl.Playing = FALSE;
        
        if( tgl.ServerPresent )
            g_SM.ServerShutdown = TRUE;
        else
            g_SM.ClientQuit = TRUE;

        GameMgr.EndGame();
        return;
    }

    // Dynamically disable some buttons based on vote circumstances...
    ui_win* pActiveWin = m_pManager->GetWindowUnderCursor( m_UserID );

    // Disable the Change Team button when there is a vote in progress
    // and when not a team based game and in campaign missions.
    Flag = pPlayer->GetVoteCanStart() &&
           GameMgr.IsTeamBasedGame() &&
           (fegl.State != FE_STATE_ENTER_CAMPAIGN);

    // Apply the flag to Change Team, and move the focus if needed.
    m_pChangeTeam->SetFlag( WF_DISABLED, !Flag );
    if( (!Flag) && ((ui_win*)m_pChangeTeam == pActiveWin) )
        OnPadBack( this );

    // Disable the Vote Kick button when there is a vote in progress
    // and when nobody to kick and when offline and during campaign missions.
    Flag = pPlayer->GetVoteCanStart() && 
           GetKickMask() &&
           (fegl.State != FE_STATE_OFFLINE) && 
           (fegl.State != FE_STATE_ENTER_CAMPAIGN);
    
    // Apply the flag to the Vote Kick buttons, move focus if needed.
    m_pVoteKick->SetFlag( WF_DISABLED, !Flag );
    if( (!Flag) && ((ui_win*)m_pVoteKick == pActiveWin) )
        OnPadBack( this );

    // Disable the Vote Map button when there is a vote in progress
    // and when offline and during campaign missions.
    Flag = pPlayer->GetVoteCanStart() && 
           (fegl.State != FE_STATE_OFFLINE) && 
           (fegl.State != FE_STATE_ENTER_CAMPAIGN);
    
    // Apply the flag to the Vote Map buttons, move focus if needed.
    #ifndef DEMO_DISK_HARNESS
    m_pVoteMap->SetFlag( WF_DISABLED, !Flag );
    if( (!Flag) && ((ui_win*)m_pVoteMap == pActiveWin) )
        OnPadBack( this );
    #endif

    // Disable the Vote Yes and Vote No buttons when there is no vote in progress.
    Flag = pPlayer->GetVoteCanCast() && 
           (fegl.State != FE_STATE_OFFLINE);

    // Apply the flag to the Vote Yes and No buttons, move focus if needed.
    m_pVoteYes->SetFlag( WF_DISABLED, !Flag );
    m_pVoteNo ->SetFlag( WF_DISABLED, !Flag );
    if( (!Flag) && 
        (((ui_win*)m_pVoteYes == pActiveWin) || ((ui_win*)m_pVoteNo == pActiveWin)) )
        OnPadBack( this );

    // Check if the voting selection list is active and we are no longer allowed
    // to vote, then end the selection list
    ui_win* pTopDialog = m_pManager->GetTopmostDialog( m_UserID );
    if( pTopDialog )
    {
        if( pTopDialog->GetLabel() == xwstring("VOTE SELECTION") )
        {
            if( !pPlayer->GetVoteCanStart() )
            {
                m_pManager->EndDialog( m_UserID, TRUE );

                if( m_pParent )
                {
                    ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
                    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
                }
            }
        }
    }
}

//=========================================================================

void dlg_player::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at the top of the dialog then a move up will move back to the tabbed dialog
    if( (Code == ui_manager::NAV_UP) &&
        ( (m_NavY == 0) ||
          (GameMgr.IsCampaignMission() && (m_NavX > 0) && (m_NavY == 3)) ) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
            return;
        }
    }

    // Call the navigation function
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

void dlg_player::OnPadSelect( ui_win* pWin )
{
    // Get Player Object & Change Team
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );

    if( pWin == (ui_win*)m_pVoteMap )
    {
        // Throw up default config selection dialog
        irect r = GetPosition();
        LocalToScreen( r );
        r.t -= 14;
        r.b = r.t + 26+18*7;
        r.l += 64;
        r.r -= 64;
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        pListDialog->SetLabel( xwstring("VOTE SELECTION") );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_iChangeMap );

        // Add all the maps to the list
        xwstring    MissionName;
        for( s32 i=0 ; ; i++ )
        {
            if( fe_Missions[i].GameType == GAME_NONE )
                break;

            if( fegl.GameType == fe_Missions[i].GameType )
            {
                MissionName = StringMgr( "MissionName", fe_Missions[i].DisplayNameString );
                pList->AddItem( MissionName, i );
            }
        }

        pList->SetSelection( 0 );
    }

    u32 KickMask = GetKickMask();

    if( (pWin == (ui_win*)m_pVoteKick) && KickMask )
    {
        // Throw up default config selection dialog
        irect r = GetPosition();
        LocalToScreen( r );
        r.t -= 14;
        r.b  = r.t + 26+18*7;
        r.l += 64;
        r.r -= 64;
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        pListDialog->SetLabel( xwstring("VOTE SELECTION") );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_iKickPlayer );

        const game_score& Score = GameMgr.GetScore();

        for( s32 i = 0; i < 32; i++ )
        {
            if( KickMask & (1 << i) )
            {
                s32 Item = pList->AddItem( Score.Player[i].Name, i );
                pList->SetItemColor( Item, HUDCOL_TEXT_BLUE );
            }
        }

        pList->SetSelection( 0 );
    }

    if( pWin == (ui_win*)m_pVoteYes )
    {
        pPlayer->VoteYes();

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

    if( pWin == (ui_win*)m_pVoteNo )
    {
        pPlayer->VoteNo();

        tgl.pUIManager->EndUsersDialogs( m_UserID );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

    if( pWin == (ui_win*)m_pChangeTeam )
    {
        dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                 "message",
                                                                 irect(0,0,250,120),
                                                                 NULL,
                                                                 WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        pMD->Configure( StringMgr("ui", IDS_CHANGE_TEAM),
                        StringMgr("ui", IDS_YES),
                        StringMgr("ui", IDS_NO),
                        StringMgr("ui", IDS_ARE_YOU_SURE),
                        HUDCOL_TEXT_WHITE,
                        &m_DoChangeTeam );
    }

    if( pWin == (ui_win*)m_pSuicide )
    {
        dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                 "message",
                                                                 irect(0,0,250,120),
                                                                 NULL,
                                                                 WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        pMD->Configure( StringMgr("ui", IDS_SUICIDE),
                        StringMgr("ui", IDS_YES),
                        StringMgr("ui", IDS_NO),
                        StringMgr("ui", IDS_ARE_YOU_SURE),
                        HUDCOL_TEXT_WHITE,
                        &m_DoSuicide );
    }

    if( pWin == (ui_win*)m_pExit )
    {
        dlg_message* pMD = (dlg_message*)m_pManager->OpenDialog( m_UserID,
                                                                 "message",
                                                                 irect(0,0,250,120),
                                                                 NULL,
                                                                 WF_VISIBLE|WF_BORDER|WF_DLG_CENTER );
        pMD->Configure( StringMgr("ui", IDS_EXIT_GAME),
                        StringMgr("ui", IDS_YES),
                        StringMgr("ui", IDS_NO),
                        StringMgr("ui", IDS_ARE_YOU_SURE),
                        HUDCOL_TEXT_WHITE,
                        &m_DoExitGame );
    }
}

//=========================================================================

void dlg_player::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_player::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    ControlsToWarrior();
}

//=========================================================================

void dlg_player::ControlsToWarrior( void )
{
    if( !m_NoUpdate )
    {
        m_NoUpdate = TRUE;

        // Check to see if something has changed.
        if( fegl.WarriorSetups[m_WarriorID].InvertPlayerY != m_pInvertPlayerY->GetSelected() )
        {
            fegl.WarriorSetups[m_WarriorID].InvertPlayerY = m_pInvertPlayerY->GetSelected();
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        if( fegl.WarriorSetups[m_WarriorID].InvertVehicleY != m_pInvertVehicleY->GetSelected() )
        {
            fegl.WarriorSetups[m_WarriorID].InvertVehicleY = m_pInvertVehicleY->GetSelected();
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        if( fegl.WarriorSetups[m_WarriorID].View3rdPlayer != m_p3rdPlayer->GetSelected() )
        {
            fegl.WarriorSetups[m_WarriorID].View3rdPlayer = m_p3rdPlayer->GetSelected();
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        if( fegl.WarriorSetups[m_WarriorID].View3rdVehicle != m_p3rdVehicle->GetSelected() )
        {
            fegl.WarriorSetups[m_WarriorID].View3rdVehicle = m_p3rdVehicle->GetSelected();
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        if( fegl.WarriorSetups[m_WarriorID].DualshockEnabled != m_pDualshock->GetSelected() )
        {
            fegl.WarriorSetups[m_WarriorID].DualshockEnabled = m_pDualshock->GetSelected();
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        fegl.WarriorSetups[m_WarriorID].SensitivityX     = m_pSensX->GetValue();
        fegl.WarriorSetups[m_WarriorID].SensitivityY     = m_pSensY->GetValue();

        // Useable range of 19 to 76
        fegl.WarriorSetups[m_WarriorID].ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED = 0.2f + 0.008f * ( 19 + ((f32)fegl.WarriorSetups[m_WarriorID].SensitivityX/10.0f) * (76-19) );

        // Useable range of 6 to 24
        fegl.WarriorSetups[m_WarriorID].ControlInfo.PS2_LOOK_UP_DOWN_SPEED    = 0.2f + 0.008f * (  6 + ((f32)fegl.WarriorSetups[m_WarriorID].SensitivityY/10.0f) * (24- 6) );

        // Get Player Object & Change Team
        player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );

        // Set players view
        if( fegl.WarriorSetups[m_WarriorID].View3rdPlayer )
            pPlayer->SetPlayerViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            pPlayer->SetPlayerViewType( player_object::VIEW_TYPE_1ST_PERSON );

        // Set vehicle view
        if( fegl.WarriorSetups[m_WarriorID].View3rdVehicle )
            pPlayer->SetVehicleViewType( player_object::VIEW_TYPE_3RD_PERSON );
        else
            pPlayer->SetVehicleViewType( player_object::VIEW_TYPE_1ST_PERSON );

        m_NoUpdate = FALSE;
    }
}

//=========================================================================

void dlg_player::WarriorToControls( void )
{
    if( !m_NoUpdate )
    {
        m_NoUpdate = TRUE;

        // Get Player Object & Change Team
        player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
        ASSERT( pPlayer );
        s32 WarriorID = pPlayer->GetWarriorID();

        m_pInvertPlayerY ->SetSelected( fegl.WarriorSetups[WarriorID].InvertPlayerY    );
        m_pInvertVehicleY->SetSelected( fegl.WarriorSetups[WarriorID].InvertVehicleY   );
        m_p3rdPlayer     ->SetSelected( fegl.WarriorSetups[WarriorID].View3rdPlayer    );
        m_p3rdVehicle    ->SetSelected( fegl.WarriorSetups[WarriorID].View3rdVehicle   );
        m_pDualshock     ->SetSelected( fegl.WarriorSetups[WarriorID].DualshockEnabled );

        m_NoUpdate = FALSE;
    }
}

//=========================================================================

u32 dlg_player::GetKickMask( void )
{
    const game_score& Score      = GameMgr.GetScore();
    player_object*    pPlayer    = (player_object*)m_pManager->GetUserData( m_UserID );
    u32               MyTeamBits = pPlayer->GetTeamBits();
    s32               MyIndex    = pPlayer->GetPlayerIndex();
    u32               KickMask   = 0;

    for( s32 i=0; i<32; i++ )
    {
        // To be able to kick, the victim must be:
        //  - Not yourself
        //  - In the game
        //  - Not a bot
        //  - Not on machine 0 (the server)
        //  - Not on the same machine as yourself (split screen protection)
        if( (i != MyIndex) &&
            (Score.Player[i].IsInGame) &&
            (Score.Player[i].IsBot == FALSE) &&
            (Score.Player[i].Machine != 0) && 
            (Score.Player[i].Machine != Score.Player[MyIndex].Machine) )
        {
            if( GameMgr.IsTeamBasedGame() )
            {
                // For team based games, you can only kick within own team.
                if( MyTeamBits & GameMgr.GetTeamBits( i ) )
                {
                    KickMask |= (1 << i);
                }
            }
            else
            {
                KickMask |= (1 << i);
            }
        }
    }

    return( KickMask );
}

//=========================================================================
