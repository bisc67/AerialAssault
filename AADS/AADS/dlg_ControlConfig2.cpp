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
#include "ui\ui_frame.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_slider.hpp"
#include "ui\ui_dlg_list.hpp"
#include "ui\ui_listbox.hpp"
#include "dlg_ControlConfig2.hpp"
//#include "ui_controllist.hpp"
#include "dlg_util_rendercontroller.hpp"

#include "StringMgr\StringMgr.hpp"

#include "fe_colors.hpp"
#include "FrontEnd.hpp"

#include "data\ui\ui_strings.h"

extern xbool UseAutoAimHelp;
extern xbool UseAutoAimHint;

//==============================================================================
// ControlConfig Dialog
//==============================================================================

enum controls
{
    IDC_CANCEL,
    IDC_OK,
    IDC_CONFIGURATION,
    IDC_FRAME2,
    IDC_FRAME1,
    IDC_X_SENS_TEXT,
    IDC_X_SENS_SLIDER,
    IDC_Y_SENS_TEXT,
    IDC_Y_SENS_SLIDER,
    IDC_INVERT_PLAYER_Y,
    IDC_INVERT_VEHICLE_Y,
//    IDC_AUTO_AIM,
    IDC_DUALSHOCK
};

enum
{
    AIM_OFF,
    AIM_HINT,
    AIM_HELP,
    AIM_HINT_HELP,
};

ui_manager::control_tem ControlConfigControls[] =
{
    { IDC_CANCEL,           IDS_CANCEL,           "button",   8, 368,  80,  24, 0, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_OK,               IDS_OK,               "button", 376, 368,  80,  24, 1, 5, 1, 1, ui_win::WF_VISIBLE },
                                                
    { IDC_CONFIGURATION,    IDS_CONFIGURATION,    "combo",    8,  48, 448,  19, 0, 1, 2, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME2,           IDS_NULL,             "frame",    8,  76, 448, 184, 0, 0, 0, 0, ui_win::WF_VISIBLE },
    { IDC_FRAME1,           IDS_NULL,             "frame",    8, 264, 448,  96, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_X_SENS_TEXT,      IDS_X_SENSITIVITY,    "text",    16, 272+14-4, 154,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_X_SENS_SLIDER,    IDS_NULL,             "slider", 136, 272+14-4, 120,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_Y_SENS_TEXT,      IDS_Y_SENSITIVITY,    "text",    16, 300+14+4, 154,  24, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_Y_SENS_SLIDER,    IDS_NULL,             "slider", 136, 300+14+4, 120,  24, 0, 3, 1, 2, ui_win::WF_VISIBLE },
//    { IDC_AUTO_AIM,         IDS_AUTO_AIM,         "combo",  16, 328, 250,  19, 0, 4, 1, 1, ui_win::WF_VISIBLE },

    { IDC_INVERT_PLAYER_Y,  IDS_INVERT_PLAYER_Y,  "check",  284, 272, 168,  24, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_INVERT_VEHICLE_Y, IDS_INVERT_VEHICLE_Y, "check",  284, 300, 168,  24, 1, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_DUALSHOCK,        IDS_DUALSHOCK,        "check",  284, 328, 168,  24, 1, 4, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem ControlConfigDialog =
{
    IDS_CONTROLS_TITLE,
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
xbool g_ControlModified;
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

#if 0
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
#endif

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
//    irect   NewPos( 0, 0, 464, 400 );   // 108,32 

    (void)pUserData;
    (void)Position;

    ASSERT( pManager );

    // Set pointer to warrior setup to edit
    m_pWarriorSetup = (warrior_setup*)pUserData;
    ASSERT( m_pWarriorSetup );

    // Backup the warrior config
    x_memcpy( &m_WarriorBackup, m_pWarriorSetup, sizeof(m_WarriorBackup ) );

    // Center Dialog
//    const irect&    b = pManager->GetUserBounds( UserID );
//    NewPos.Translate( b.l + (b.GetWidth ()-NewPos.GetWidth ())/2 - NewPos.l,
//                      b.t + (b.GetHeight()-NewPos.GetHeight())/2 - NewPos.t );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_BackgroundColor   = FECOL_DIALOG2;
    m_SetDefaultConfig  = -1;
    m_LastDefaultConfig = 0;

    m_pControllerConfig = (ui_combo*       ) FindChildByID( IDC_CONFIGURATION );
    m_pControllerFrame  = (ui_frame*       ) FindChildByID( IDC_FRAME2 );
    m_pOk               = (ui_button*      ) FindChildByID( IDC_OK );
    m_pCancel           = (ui_button*      ) FindChildByID( IDC_CANCEL  );
    m_pTextSensX        = (ui_text*        ) FindChildByID( IDC_X_SENS_TEXT );
    m_pTextSensY        = (ui_text*        ) FindChildByID( IDC_Y_SENS_TEXT );
    m_pSensX            = (ui_slider*      ) FindChildByID( IDC_X_SENS_SLIDER );
    m_pSensY            = (ui_slider*      ) FindChildByID( IDC_Y_SENS_SLIDER );
    m_pInvertPlayerY    = (ui_button*      ) FindChildByID( IDC_INVERT_PLAYER_Y );
    m_pInvertVehicleY   = (ui_button*      ) FindChildByID( IDC_INVERT_VEHICLE_Y );
//    m_pAutoAim          = (ui_combo*      ) FindChildByID( IDC_AUTO_AIM );
    m_pDualshock        = (ui_check*       ) FindChildByID( IDC_DUALSHOCK );

    m_pControllerConfig->SetLabelWidth( 200 );
    m_pControllerConfig->AddItem( "CONFIG A", WARRIOR_CONTROL_CONFIG_A  );
    m_pControllerConfig->AddItem( "CONFIG B", WARRIOR_CONTROL_CONFIG_B  );
    m_pControllerConfig->AddItem( "CONFIG C", WARRIOR_CONTROL_CONFIG_C  );
    m_pControllerConfig->AddItem( "CONFIG D", WARRIOR_CONTROL_CONFIG_D  );

    //m_pControllerConfig->AddItem( StringMgr( "ui", IDS_CUSTOM )  , WARRIOR_CONTROL_CUSTOM   );

    m_pControllerConfig->SetSelection( m_pWarriorSetup->ControlConfigID );

    m_DualshockEnabled  = m_pWarriorSetup->DualshockEnabled;
    m_SensitivityX      = m_pWarriorSetup->SensitivityX;
    m_SensitivityY      = m_pWarriorSetup->SensitivityY;

    m_InvertPlayerY     = m_pWarriorSetup->InvertPlayerY;
    m_InvertVehicleY    = m_pWarriorSetup->InvertVehicleY;

//    m_pAutoAim->SetLabelWidth( 94 );
//    m_pAutoAim->AddItem( StringMgr( "ui", IDS_AUTO_AIM_OFF ), AIM_OFF );
//    m_pAutoAim->AddItem( StringMgr( "ui", IDS_AUTO_AIM_HINT ), AIM_HINT );
//    m_pAutoAim->AddItem( StringMgr( "ui", IDS_AUTO_AIM_HELP ), AIM_HELP );
//    m_pAutoAim->AddItem( StringMgr( "ui", IDS_AUTO_AIM_HINT_HELP ), AIM_HINT_HELP ) ;

/*
    switch( (UseAutoAimHelp?2:0)|(UseAutoAimHint?1:0) )
    {
    case 0:
        m_pAutoAim->SetSelection( AIM_OFF ); break;
    case 1:
        m_pAutoAim->SetSelection( AIM_HINT ); break;
    case 2:
        m_pAutoAim->SetSelection( AIM_HELP ); break;
    case 3:
        m_pAutoAim->SetSelection( AIM_HINT_HELP ); break;
    default:
        ASSERT( 0 );
    }    
*/
    
//    m_pAutoAim->SetLabel( xwstring("AUTOAIM: "+GetAutoAimType()) );

    m_pDualshock->SetFlags( m_pDualshock->GetFlags() | (m_DualshockEnabled?WF_SELECTED:0) );
    m_pTextSensX->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pTextSensY->SetLabelFlags( ui_font::v_center|ui_font::h_left );
    m_pSensX->SetRange( 0, 10 );
    s32 tempX = m_SensitivityX;
    // Force the notify event.
    if( m_SensitivityX < 10 )
        m_pSensX->SetValue( 10 );
    else
        m_pSensX->SetValue( 0 );

    m_pSensX->SetValue( tempX );
    m_pSensX->SetStep ( 1, 1 );
    m_pSensY->SetRange( 0, 10 );
    s32 tempY = m_SensitivityY;
    // Force the notify event.
    if( m_SensitivityY < 10 )
        m_pSensY->SetValue( 10 );
    else
        m_pSensY->SetValue( 0 );

    m_pSensY->SetValue( tempY );
    m_pSensY->SetStep ( 1, 1 );
//    m_pTextSensX->SetLabel( xwstring(xfs("X SENSITIVITY (%d)", m_pSensX->GetValue())) );
//    m_pTextSensY->SetLabel( xwstring(xfs("Y SENSITIVITY (%d)", m_pSensY->GetValue())) );

    m_pInvertPlayerY ->SetFlag( WF_SELECTED, m_InvertPlayerY  );
    m_pInvertVehicleY->SetFlag( WF_SELECTED, m_InvertVehicleY );

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
/*
    if( m_SetDefaultConfig != -1 )
    {
        m_pControlList->SetDefault( m_SetDefaultConfig );
        m_LastDefaultConfig = m_SetDefaultConfig;
        m_SetDefaultConfig = -1;
    }
*/

    // Only render is visible
    if( m_Flags & WF_VISIBLE )
    {
        // Fade out background
//        const irect& ru = m_pManager->GetUserBounds( m_UserID );
//        m_pManager->RenderRect( ru, xcolor(0,0,0,128), FALSE );

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
            m_pManager->RenderText( 2, rb, ui_font::v_center, XCOLOR_BLACK, m_Label );
            rb.Translate( -1, -1 );
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

        // Get Bounding rectangle for controller & Render
        irect Rect = m_pControllerFrame->GetPosition();
        Rect.Deflate( 4, 4 );
        Rect.Translate( 0, 10 );
        LocalToScreen( Rect );
        const irect& br = m_pManager->GetUserBounds( m_UserID );
        Rect.Translate( br.l, br.t );
        RenderController( Rect, m_pWarriorSetup );
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
	if ( (pGameKey->MainGadget  != (input_gadget)MainGadget) ||
		 (pGameKey->ShiftGadget != (input_gadget)ShiftGadget) ||
		 (pGameKey->ShiftState  != (input_gadget)ShiftState) )
	{
		g_ControlModified = TRUE;
	}
    pGameKey->MainGadget  = (input_gadget)MainGadget;
    pGameKey->ShiftGadget = (input_gadget)ShiftGadget;
    pGameKey->ShiftState  = ShiftState;
}

void dlg_controlconfig::OnPadSelect( ui_win* pWin )
{
    if( pWin == (ui_win*)m_pOk )
    {
		// Check for control data modified
        if ( (m_pWarriorSetup->DualshockEnabled != m_DualshockEnabled) ||
			 (m_pWarriorSetup->SensitivityX     != m_SensitivityX) ||
			 (m_pWarriorSetup->SensitivityY     != m_SensitivityY) ||
			 (m_pWarriorSetup->InvertPlayerY    != m_InvertPlayerY) ||
			 (m_pWarriorSetup->InvertVehicleY   != m_InvertVehicleY))
		{
			g_ControlModified = TRUE;
		}

        // Set Control Data
        m_pWarriorSetup->DualshockEnabled = m_DualshockEnabled;
        m_pWarriorSetup->SensitivityX     = m_SensitivityX;
        m_pWarriorSetup->SensitivityY     = m_SensitivityY;
        m_pWarriorSetup->InvertPlayerY    = m_InvertPlayerY;
        m_pWarriorSetup->InvertVehicleY   = m_InvertVehicleY;

        ASSERT( INPUT_PS2_STICK_LEFT_X  == (INPUT_PS2_STICK_LEFT_Y-1) );
        ASSERT( INPUT_PS2_STICK_RIGHT_X == (INPUT_PS2_STICK_RIGHT_Y-1) );

		// Check for control modification
        if ( (m_pWarriorSetup->ControlInfo.PS2_MOVE_LEFT_RIGHT.      MainGadget != (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID  ))||
			 (m_pWarriorSetup->ControlInfo.PS2_MOVE_FORWARD_BACKWARD.MainGadget != (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_MOVE].GadgetID+1))||
			 (m_pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT.      MainGadget != (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID  ))||
			 (m_pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN.         MainGadget != (input_gadget)(m_pWarriorSetup->ControlEditData[WARRIOR_CONTROL_LOOK].GadgetID+1)) )
		{
			g_ControlModified = TRUE;
		}

        
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
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_ZOOM_IN,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM_IN       );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_ZOOM_OUT,         &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_ZOOM_OUT      );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_HEALTH_KIT,       &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_USE_HEALTH    );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_TARGETING_LASER,  &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TARGET_LASER  );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_WEAPON,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_WEAPON   );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_PACK,        &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_PACK     );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_BEACON,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DEPLOY_BEACON );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_DROP_FLAG,        &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_DROP_FLAG     );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_SUICIDE,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_SUICIDE       );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_OPTIONS,          &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_OPTIONS       );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_TARGET_LOCK,      &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TARGET_LOCK   );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_COMPLIMENT,       &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_COMPLIMENT    );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_TAUNT,            &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_TAUNT         );
        SetControl( &m_pWarriorSetup->ControlInfo.PS2_EXCHANGE,         &m_pWarriorSetup->ControlEditData[0], WARRIOR_CONTROL_EXCHANGE      );

        // Useable range of 19 to 76
		f32 hspeed,vspeed;

		hspeed = 0.2f + 0.008f * ( 19 + ((f32)m_pWarriorSetup->SensitivityX/10.0f) * (76-19) );
		vspeed = 0.2f + 0.008f * (  6 + ((f32)m_pWarriorSetup->SensitivityY/10.0f) * (24- 6) );

        if ( (m_pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED != hspeed) ||
			 (m_pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN_SPEED    != vspeed) )
		{
			g_ControlModified = TRUE;
		}

        m_pWarriorSetup->ControlInfo.PS2_LOOK_LEFT_RIGHT_SPEED = hspeed;

        // Useable range of 6 to 24
        m_pWarriorSetup->ControlInfo.PS2_LOOK_UP_DOWN_SPEED    = vspeed;

/*
        switch( m_pAutoAim->GetSelection() )
        {
            case AIM_OFF:
                UseAutoAimHelp = 0;
                UseAutoAimHint = 0;
            break;
            case AIM_HINT:
                UseAutoAimHint = 1;
                UseAutoAimHelp = 0;
            break;
            case AIM_HELP:
                UseAutoAimHelp = 2;
                UseAutoAimHint = 0;
            break;
            case AIM_HINT_HELP:
                UseAutoAimHelp = 2;
                UseAutoAimHint = 1;
            break;
        }

		if ( m_pAutoAim->GetSelection() != s_OriginalAim )
		{
			g_ControlModified = TRUE;
		}
*/

        // Close dialog and step back
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog( m_UserID, TRUE );
    }
    else if( pWin == (ui_win*)m_pCancel )
    {
        // Restore the warrior
        x_memcpy( m_pWarriorSetup, &m_WarriorBackup, sizeof(m_WarriorBackup ) );

        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        m_pManager->EndDialog(m_UserID,TRUE);
    }
/*
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
*/
//    else if( pWin == (ui_win*)m_pAutoAim )
//    {
//        // Toggle State
//        NextAutoAimType();
//        m_pAutoAim->SetLabel( xwstring("AUTOAIM: "+GetAutoAimType()) );
//    }

}

//=========================================================================

void dlg_controlconfig::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Restore the warrior
    x_memcpy( m_pWarriorSetup, &m_WarriorBackup, sizeof(m_WarriorBackup ) );

    // Close dialog and step back
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
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
        m_pTextSensX->SetLabel( StringMgr( "ui", IDS_X_SENSITIVITY ) );
    }

    if( pSender == (ui_win*)m_pSensY )
    {
        s32 Min, Max;
        m_pSensX->GetRange( Min, Max );
        m_SensitivityY = m_pSensY->GetValue();
        m_pTextSensY->SetLabel( StringMgr( "ui", IDS_Y_SENSITIVITY ) );
    }

    if( pSender == m_pControllerConfig )
    {
        s32 ControlID = m_pControllerConfig->GetSelectedItemData();
        if( (ControlID >= WARRIOR_CONTROL_CONFIG_A) && (ControlID <= WARRIOR_CONTROL_CONFIG_D) )
        {
            SetWarriorControlDefault( m_pWarriorSetup, ControlID );
        }
    }
/*
    if( pSender == m_pAutoAim )
    {
        s32 Aim = m_pAutoAim->GetSelectedItemData();
        switch( Aim )
        {
            case AIM_OFF:
                UseAutoAimHelp = 0;
                UseAutoAimHint = 0;
            break;
            case AIM_HINT:
                UseAutoAimHint = 1;
            break;
            case AIM_HELP:
                UseAutoAimHelp = 2;
            break;
            case AIM_HINT_HELP:
                UseAutoAimHelp = 2;
                UseAutoAimHint = 1;
            break;
        }
    }
*/
    if( pSender == m_pInvertPlayerY )
    {
        m_InvertPlayerY  = m_pInvertPlayerY ->GetFlags() & WF_SELECTED;
    }

    if( pSender == m_pInvertVehicleY )
    {
        m_InvertVehicleY = m_pInvertVehicleY->GetFlags() & WF_SELECTED;
    }
}

//=========================================================================

void dlg_controlconfig::UpdateControls( void )
{
}

//=========================================================================
