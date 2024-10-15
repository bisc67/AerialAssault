//=========================================================================
//
//  dlg_controlconfig.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "AudioMgr\audio.hpp"
#include "LabelSets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_button.hpp"
#include "ui\ui_text.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_font.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_dlg_list.hpp"
#include "ui\ui_listbox.hpp"
#include "dlg_ControlConfig.hpp"
#include "ui_controllist.hpp"

#include "fe_colors.hpp"
 
extern xbool UseAutoAimHelp;
extern xbool UseAutoAimHint;

//==============================================================================
// ControlConfig Dialog
//==============================================================================

enum controls
{
    IDC_CANCEL,
    IDC_OK,
    IDC_RESTORE_DEFAULTS,
    IDC_CONTROLS,
    IDC_FRAME2,
    IDC_FRAME1,
    IDC_X_SENS_TEXT,
    IDC_X_SENS_SLIDER,
    IDC_Y_SENS_TEXT,
    IDC_Y_SENS_SLIDER,
    IDC_AUTO_AIM,
    IDC_DUALSHOCK
};

ui_manager::control_tem ControlConfigControls[] =
{
    { IDC_CANCEL,           IDS_CANCEL,             "button",        8,      368-32,     80,  24, 0, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OK,               IDS_OK,                 "button",        340-88, 368-32,     80,  24, 1, 5, 1, 1, ui_win::WF_VISIBLE },

    { IDC_RESTORE_DEAFULTS, IDS_RESTORE_DEFAULTS,   "button",        8,          48, 340-16,  24, 0, 0, 2, 1, ui_win::WF_VISIBLE },

    { IDC_CONTROLS,         IDS_NULL,               "controllist",   8,          76, 340-16, 152, 0, 1, 2, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME1,           IDS_NULL,               "frame",         8,         232, 340-16,  68, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_X_SENS_TEXT,      IDS_X_SENSITIVITY,      "text",         16,         240,    154,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_X_SENS_SLIDER,    IDS_NULL,               "slider",      170,         240,    154,  24, 0, 2, 2, 1, ui_win::WF_VISIBLE },
    { IDC_Y_SENS_TEXT,      IDS_Y_SENSITIVITY,      "text",         16,         268,    154,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_Y_SENS_SLIDER,    IDS_NULL,               "slider",      170,         268,    154,  24, 0, 3, 2, 1, ui_win::WF_VISIBLE },

    { IDC_AUTO_AIM,         IDS_AUTO_AIM,           "button",        8,         304, 180-8,   24, 0, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DUALSHOCK,        IDS_DUALSHOCK,          "check",         200,       304, 140-8,   24, 1, 4, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem ControlConfigDialog =
{
    IDS_CONTROLS,
    2, 6,
    sizeof(ControlConfigControls)/sizeof(ui_manager::control_tem),
    &ControlConfigControls[0],
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

void dlg_controlconfig_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "controlconfig", &ControlConfigDialog, &dlg_controlconfig_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_controlconfig_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_controlconfig* pDialog = new dlg_controlconfig;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_controlconfig
//=========================================================================

dlg_controlconfig::dlg_controlconfig( void )
{
}

//=========================================================================

dlg_controlconfig::~dlg_controlconfig( void )
{
    Destroy();
}

//=========================================================================

static xstring GetAutoAimType( void )
{
    xstring String;

    switch( (UseAutoAimHelp?2:0)|(UseAutoAimHint?1:0) )
    {
    case 0:
        String = "OFF"; break;
    case 1:
        String = "HINT"; break;
    case 2:
        String = "HELP"; break;
    case 3:
        String = "HINT+HELP"; break;
    default:
        ASSERT( 0 );
    }

    return String;
}

static void NextAutoAimType( void )
{
    s32 Code = (UseAutoAimHelp?2:0)|(UseAutoAimHint?1:0);
    Code = (Code+1)&3;
    UseAutoAimHint = (Code&1);
    UseAutoAimHelp = (Code&2);
}

//=========================================================================

xbool dlg_controlconfig::Create( s32                        UserID,
                                 ui_manager*                pManager,
                                 ui_manager::dialog_tem*    pDialogTem,
                                 const irect&               Position,
                                 ui_win*                    pParent,
                                 s32                        Flags,
                                 void*                      pUserData)
{
    xbool   Success = FALSE;
//    irect   NewPos( 0, 0, 480, 416 );
    irect   NewPos( 0, 0, 340, 368 );

    (void)pUserData;
    (void)Position;

    ASSERT( pManager );

    // Set pointer to warrior setup to edit
    m_pWarriorSetup = (warrior_setup*)pUserData;
    ASSERT( m_pWarriorSetup );

    // Center Dialog
    const irect&    b = pManager->GetUserBounds( UserID );
    NewPos.Translate( b.l + (b.GetWidth ()-NewPos.GetWidth ())/2 - NewPos.l,
                      b.t + (b.GetHeight()-NewPos.GetHeight())/2 - NewPos.t );

    // Make it input modal
    Flags |= WF_INPUTMODAL;

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, NewPos, pParent, Flags );

    m_BackgroundColor   = FECOL_DIALOG2;
    m_SetDefaultConfig  = -1;
    m_LastDefaultConfig = 0;

    m_pOk          = (ui_button*      ) FindChildByID( IDC_OK );
    m_pCancel      = (ui_button*      ) FindChildByID( IDC_CANCEL  );
    m_pTextSensX   = (ui_text*        ) FindChildByID( IDC_X_SENS_TEXT );
    m_pTextSensY   = (ui_text*        ) FindChildByID( IDC_Y_SENS_TEXT );
    m_pSensX       = (ui_slider*      ) FindChildByID( IDC_X_SENS_SLIDER );
    m_pSensY       = (ui_slider*      ) FindChildByID( IDC_Y_SENS_SLIDER );
    m_pRestore     = (ui_button*      ) FindChildByID( IDC_RESTORE_DEFAULTS );
    m_pAutoAim     = (ui_button*      ) FindChildByID( IDC_AUTO_AIM );
    m_pDualshock   = (ui_check*       ) FindChildByID( IDC_DUALSHOCK );
    m_pControlList = (ui_controllist* ) FindChildByID( IDC_CONTROLS );

    m_DualshockEnabled  = m_pWarriorSetup->DualshockEnabled;
    m_SensitivityX      = m_pWarriorSetup->SensitivityX;
    m_SensitivityY      = m_pWarriorSetup->SensitivityY;

    m_pAutoAim->SetLabel( "AUTOAIM: "+GetAutoAimType() );

    m_pDualshock->SetFlags( m_pDualshock->GetFlags() | (m_DualshockEnabled?WF_SELECTED:0) );
    m_pTextSensX->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextSensY->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pSensX->SetRange( 0, 100 );
    m_pSensX->SetValue( m_SensitivityX );
    m_pSensX->SetStep ( 1, 5 );
    m_pSensY->SetRange( 0, 100 );
    m_pSensY->SetValue( m_SensitivityY );
    m_pSensY->SetStep ( 1, 5 );
    m_pTextSensX->SetLabel( xfs("X SENSITIVITY (%d)", m_pSensX->GetValue()) );
    m_pTextSensY->SetLabel( xfs("Y SENSITIVITY (%d)", m_pSensY->GetValue()) );

    // Return success code
    return Success;
}

//=========================================================================

void dlg_controlconfig::Destroy( void )
{
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_controlconfig::Render( s32 ox, s32 oy )
{
    // Check for setting default config
    if( m_SetDefaultConfig != -1 )
    {
        m_pControlList->SetDefault( m_SetDefaultConfig );
        m_LastDefaultConfig = m_SetDefaultConfig;
        m_SetDefaultConfig = -1;
    }

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Fade out background
        const irect& ru = m_pManager->GetUserBounds( m_UserID );
        m_pManager->RenderRect( ru, xcolor(0,0,0,128), FALSE );

        // Get window rectangle
        irect   r;
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        irect   rb = r;
        rb.Deflate( 1, 1 );

        // Render frame
        if( m_Flags & WF_BORDER )
        {
            // Render background color
            if( m_BackgroundColor.A > 0 )
            {
                m_pManager->RenderRect( rb, m_BackgroundColor, FALSE );
            }

            // Render Title Bar Gradient
            rb.SetHeight( 40 );
            xcolor c1 = FECOL_TITLE1;
            xcolor c2 = FECOL_TITLE2;
            m_pManager->RenderGouraudRect( rb, c1, c1, c2, c2, FALSE );

            // Render the Frame
            m_pManager->RenderElement( m_iElement, r, 0 );

            // Render Title
            rb.Deflate( 16, 0 );
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_WHITE, m_Label );
        }

/*
        // Render Message
        r.Set( m_Position.l+ox, m_Position.t+oy, m_Position.r+ox, m_Position.b+oy );
        r.Deflate( 8, 40 );
        r.t += 9;
        m_pManager->RenderElement( m_iElement, r, 0 );
        r.Deflate( 4, 4 );
        m_pManager->RenderTextFormatted( 0, r, ui_font::h_center|ui_font::v_center, m_MessageColor, m_Message );
*/

        // Render children
        for( s32 i=0 ; i<m_Children.GetCount() ; i++ )
        {
            m_Children[i]->Render( m_Position.l+ox, m_Position.t+oy );
        }
    }

}

//=========================================================================

void dlg_controlconfig::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
}

//=========================================================================

static void SetControl( player_object::game_key* pGameKey, warrior_control_data* pControlData, s32 ControlIndex )
{
    s32 MainGadget  = pControlData[ControlIndex].GadgetID;
    s32 ShiftGadget = pControlData[WARRIOR_CONTROL_SHIFT].GadgetID;
    s32 ShiftState  = pControlData[ControlIndex].IsShifted;

    ASSERT( !ShiftState || (ShiftGadget != INPUT_UNDEFINED) );

    // Check if we should set shift gadget to undefined
    if( !ShiftState && (ShiftGadget != INPUT_UNDEFINED) )
    {
        xbool   HasShiftedCounterpart = 0;

        for( s32 i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
        {
            if( (pControlData[i].GadgetID == MainGadget) &&
                (pControlData[i].IsShifted) )
            {
                HasShiftedCounterpart = 1;
                break;
            }
        }

        // If no shifted version then make shift gadget undefined
        if( !HasShiftedCounterpart )
            ShiftGadget = INPUT_UNDEFINED;
    }

    // Set GameKey
    pGameKey->MainGadget  = (input_gadget)MainGadget;
    pGameKey->ShiftGadget = (input_gadget)ShiftGadget;
    pGameKey->ShiftState  = ShiftState;
}

void dlg_controlconfig::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pOk )
    {
        // Set Control Data
        warrior_control_data* pControlData = m_pControlList->GetControlData();
        ASSERT( pControlData );
        for( s32 i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
            m_pWarriorSetup->ControlEditData[i] = pControlData[i];
        m_pWarriorSetup->DualshockEnabled = m_DualshockEnabled;
        m_pWarriorSetup->SensitivityX    = m_SensitivityX;
        m_pWarriorSetup->SensitivityY    = m_SensitivityY;

        ASSERT( INPUT_PS2_STICK_LEFT_X  == (INPUT_PS2_STICK_LEFT_Y-1) );
        ASSERT( INPUT_PS2_STICK_RIGHT_X == (INPUT_PS2_STICK_RIGHT_Y-1) );

        // Set into actual control structure
        m_pWarriorSetup->ControlInfo.PS2_MOVE_LEFT_RIGHT.      MainGadget = (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID  );
        m_pWarriorSetup->ControlInfo.PS2_MOVE_FORWARD_BACKWARD.MainGadget = (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID+1);
        m_pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT.      MainGadget = (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID  );
        m_pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN.         MainGadget = (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID+1);
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_FREE_LOOK,        &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_FREELOOK      );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_JUMP,             &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_JUMP          );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_FIRE,             &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_FIRE          );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_JET,              &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_JET           );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_ZOOM,             &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM          );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_NEXT_WEAPON,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_NEXT_WEAPON   );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_GRENADE,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_GRENADE       );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_VOICE_MENU,       &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_VOICE_MENU    );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_MINE,             &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_MINE          );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_USE_PACK,         &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_USE_PACK      );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_ZOOM_CHANGE,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_CHANGE_ZOOM   );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_HEALTH_KIT,       &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_USE_HEALTH    );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_TARGETING_LASER,  &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TARGET_LASER  );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_WEAPON,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_WEAPON   );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_PACK,        &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_PACK     );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_BEACON,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DEPLOY_BEACON );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_FLAG,        &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_FLAG     );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_SUICIDE,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_SUICIDE       );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_OPTIONS,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_OPTIONS       );
        m_pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED = 0.2f + (0.8f * m_pWarriorSetup->SensitivityX / 100.0f);
        m_pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN_SPEED    = 0.2f + (0.8f * m_pWarriorSetup->SensitivityY / 100.0f);

        // Close dialog and step back
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
        m_pManager->EndDialog( m_UserID, TRUE );
    }
    else if( pWin == (ui_win*)m_pCancel )
    {
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
        m_pManager->EndDialog(m_UserID,TRUE);
    }
    else if( pWin == (ui_win*)m_pRestore )
    {
        // Throw up default config selection dialog
        irect r = m_pRestore->GetPosition();
        LocalToScreen( r );
        r.SetHeight(24+18*3);
        ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
        ui_listbox* pList = pListDialog->GetListBox();
        pListDialog->SetResultPtr( &m_SetDefaultConfig );
        pList->AddItem( "NewBlood" , 0 );
        pList->AddItem( "Officer"  , 1 );
        pList->AddItem( "Commander", 2 );
        pList->SetSelection( m_LastDefaultConfig );
    }
    else if( pWin == (ui_win*)m_pAutoAim )
    {
        // Toggle State
        NextAutoAimType();
        m_pAutoAim->SetLabel( "AUTOAIM: "+GetAutoAimType() );
    }

}

//=========================================================================

void dlg_controlconfig::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Close dialog and step back
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE );
    m_pManager->EndDialog( m_UserID, TRUE );
}

//=========================================================================

void dlg_controlconfig::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    if( pSender == (ui_win*)m_pDualshock )
    {
        m_DualshockEnabled = (m_pDualshock->GetFlags() & WF_SELECTED);
    }

    if( pSender == (ui_win*)m_pSensX )
    {
        s32 Min, Max;
        m_pSensX->GetRange( Min, Max );
        m_SensitivityX = m_pSensX->GetValue();
        m_pTextSensX->SetLabel( xfs("X SENSITIVITY (%d)", m_pSensX->GetValue()) );
    }

    if( pSender == (ui_win*)m_pSensY )
    {
        s32 Min, Max;
        m_pSensX->GetRange( Min, Max );
        m_SensitivityY = m_pSensY->GetValue();
        m_pTextSensY->SetLabel( xfs("Y SENSITIVITY (%d)", m_pSensY->GetValue()) );
    }
}

//=========================================================================

void dlg_controlconfig::UpdateControls( void )
{
}

//=========================================================================
