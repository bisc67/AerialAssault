//=========================================================================
//
//  ui_controllist.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "AudioMgr\audio.hpp"
#include "LabelSets\Tribes2Types.hpp"

#include "dlg_controlconfig.hpp"
#include "ui_controllist.hpp"
#include "ui/ui_manager.hpp"
#include "ui/ui_font.hpp"
#include "ui/ui_dlg_list.hpp"
//#include "ui_dlg_combolist.hpp"
#include "Objects/Player/PlayerObject.hpp"

#include "FrontEnd.hpp"

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

#define CONTROL_FLAG_ANALOG     0x01        // Analog control
#define CONTROL_FLAG_VOICE      0x02        // Voice Menu control
#define CONTROL_FLAG_FIXED      0x04        // Cannot be remapped
#define CONTROL_FLAG_SHIFTKEY   0x08        // Shift key definition
#define CONTROL_FLAG_SHIFTED    0x10        // Shift activates

static const char* ControlNames[] =
{
    { "Shift Button"        },
    { "Move"                },
    { "Look"                },
    { "Free Look"           },
    { "Jump"                },
    { "Fire"                },
    { "Jet"                 },
    { "Zoom"                },
    { "Next Weapon",        },
    { "Grenade",            },
    { "Voice Menu",         },
    { "Voice Chat",         },
    { "Mine",               },
    { "Use Pack",           },
    { "Change Zoom",        },
    { "Use Health",         },
    { "Targeting Laser",    },
    { "Drop Weapon",        },
    { "Drop Pack",          },
    { "Deploy Beacon",      },
    { "Drop Flag",          },
    { "Suicide",            },
    { "Options Menu",       }
};

struct key_data
{
    s32             GadgetID;
    const char*     KeyName;
};

#define INPUT_NULL              32768
#define INPUT_VOICE_CHAT_RIGHT  32769
#define INPUT_VOICE_CHAT_LEFT   32770

static key_data KeyData[] =
{
    { INPUT_PS2_STICK_LEFT_X,   "Left Analog" },
    { INPUT_PS2_STICK_RIGHT_X,  "Right Analog" },
    { INPUT_PS2_BTN_R_STICK,    "Right Stick" },
    { INPUT_PS2_BTN_L_STICK,    "Left Stick" },
    { INPUT_PS2_BTN_L1,         "L1" },
    { INPUT_PS2_BTN_L2,         "L2" },
    { INPUT_PS2_BTN_R1,         "R1" },
    { INPUT_PS2_BTN_R2,         "R2" },
    { INPUT_PS2_BTN_TRIANGLE,   "Triangle" },
    { INPUT_PS2_BTN_CIRCLE,     "Circle" },
    { INPUT_PS2_BTN_CROSS,      "Cross" },
    { INPUT_PS2_BTN_SQUARE,     "Square" },
    { INPUT_PS2_BTN_L_LEFT,     "Left" },
    { INPUT_PS2_BTN_L_RIGHT,    "Right" },
    { INPUT_PS2_BTN_L_UP,       "Up" },
    { INPUT_PS2_BTN_L_DOWN,     "Down" },
    { INPUT_PS2_BTN_SELECT,     "Select" },
    { INPUT_PS2_BTN_START,      "Start" },
    { INPUT_VOICE_CHAT_RIGHT,   "Action Cluster" },
    { INPUT_VOICE_CHAT_LEFT,    "D-Pad" },
    { INPUT_UNDEFINED,          "<undefined>" }
};

#define NUM_KEYS    ((s32)(sizeof(KeyData)/sizeof(key_data)))

static warrior_control_data ControlData[WARRIOR_CONTROL_SIZEOF];

//=========================================================================
//  Data
//=========================================================================

//=========================================================================
//  Registration function
//=========================================================================

void ui_controllist_register( ui_manager* pManager )
{
    pManager->RegisterWinClass( "controllist", &ui_controllist_factory  );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* ui_controllist_factory( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    ui_controllist* pcombo = new ui_controllist;
    pcombo->Create( UserID, pManager, Position, pParent, Flags );

    return (ui_win*)pcombo;
}

//=========================================================================
//  ui_controllist
//=========================================================================

ui_controllist::ui_controllist( void )
{
}

//=========================================================================

ui_controllist::~ui_controllist( void )
{
    Destroy();
}

//=========================================================================

xbool ui_controllist::Create( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags )
{
    xbool   Success;
    s32     i;

    Success = ui_listbox::Create( UserID, pManager, Position, pParent, Flags );

    // Set pointer to warrior setup
    dlg_controlconfig* pControlConfig = (dlg_controlconfig*)pParent;
    m_pWarriorSetup = pControlConfig->m_pWarriorSetup;
    ASSERT( m_pWarriorSetup );

    m_NewKeyBinding = INPUT_NULL;

    // Setup control data
    for( i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
        ControlData[i] = m_pWarriorSetup->ControlEditData[i];

    // Add all controls to list
    for( i=0 ; i<s32(sizeof(ControlData)/sizeof(warrior_control_data)) ; i++ )
    {
        AddItem( "", (s32)&ControlData[i] );
    }
    SetSelection( 0 );

    return Success;
}

//=========================================================================

void ui_controllist::SetDefault( s32 iDefault )
{
    SetWarriorControlDefault( &ControlData[0], iDefault );
}

//=========================================================================

warrior_control_data* ui_controllist::GetControlData( void )
{
    return &ControlData[0];
}

//=========================================================================

static const char* KeyNameFromGadgetID( s32 GadgetID )
{
    for( s32 i=0 ; i<(s32)(sizeof(KeyData)/sizeof(key_data)) ; i++ )
    {
        if( KeyData[i].GadgetID == GadgetID )
            return KeyData[i].KeyName;
    }

    return "<undefined>";
}

//=========================================================================

void ui_controllist::Render( s32 ox, s32 oy )
{
    // Check for updated key binding
    if( m_NewKeyBinding != INPUT_NULL )
    {
        if( m_iSelection == WARRIOR_CONTROL_MOVE )
        {
            s32                     GadgetID           = m_NewKeyBinding & 0x7fffffff;
            ControlData[WARRIOR_CONTROL_MOVE].GadgetID = GadgetID;
            ControlData[WARRIOR_CONTROL_LOOK].GadgetID = (GadgetID == INPUT_PS2_STICK_LEFT_X)?INPUT_PS2_STICK_RIGHT_X:INPUT_PS2_STICK_LEFT_X;
        }
        else if( m_iSelection == WARRIOR_CONTROL_LOOK )
        {
            s32                     GadgetID           = m_NewKeyBinding & 0x7fffffff;
            ControlData[WARRIOR_CONTROL_LOOK].GadgetID = GadgetID;
            ControlData[WARRIOR_CONTROL_MOVE].GadgetID = (GadgetID == INPUT_PS2_STICK_LEFT_X)?INPUT_PS2_STICK_RIGHT_X:INPUT_PS2_STICK_LEFT_X;
        }
        else if( m_iSelection != -1 )
        {
            warrior_control_data*   pControlData = (warrior_control_data*)GetSelectedItemData();
            s32                     GadgetID     = m_NewKeyBinding & 0x7fffffff;
            xbool                   IsShifted    = ((m_NewKeyBinding & 0x80000000) != 0);

            // Clear out bad bindings
            for( s32 i=0 ; i<WARRIOR_CONTROL_SIZEOF ; i++ )
            {
                if( (ControlData[i].GadgetID == GadgetID) && (ControlData[i].IsShifted == IsShifted) )
                {
                    ControlData[i].GadgetID  = INPUT_UNDEFINED;
                    ControlData[i].IsShifted = 0;
                }

                if( ControlData[i].IsShifted && (pControlData->ControlID == WARRIOR_CONTROL_SHIFT) && (GadgetID == INPUT_UNDEFINED) )
                {
                    ControlData[i].GadgetID  = INPUT_UNDEFINED;
                    ControlData[i].IsShifted = 0;
                }
            }

            // Bind to new key
            pControlData->GadgetID  = GadgetID;
            pControlData->IsShifted = IsShifted;
        }
        m_NewKeyBinding = INPUT_NULL;
    }

    // Check to enable/disable Voice Chat binding if Voice Menu is not bound
    xbool EnableVoiceChat = (ControlData[WARRIOR_CONTROL_VOICE_MENU].GadgetID != INPUT_UNDEFINED);
    EnableItem( WARRIOR_CONTROL_VOICE_CHAT, EnableVoiceChat );

    ui_listbox::Render( ox, oy );
}

//=========================================================================

void ui_controllist::RenderItem( irect r, const item& Item, const xcolor& c1, const xcolor& c2 )
{
    irect                       r2              = r;
    warrior_control_data*       pControlData    = (warrior_control_data*)Item.Data;
    xbool                       IsShifted;
    xstring                     ControlName;
    xstring                     KeyName;

    ControlName = ControlNames[pControlData->ControlID];

    IsShifted  = pControlData->IsShifted;
    if( IsShifted )
        KeyName = "Shift + ";
    KeyName   += KeyNameFromGadgetID( pControlData->GadgetID );

    if( pControlData )
    {
        r.Deflate( 4, 0 );

        r2 = r;
        r2.Translate( 1, -2 );
        m_pManager->RenderText( m_Font, r2, ui_font::h_left|ui_font::v_center, c2, (const char*)ControlName );
        r2.Translate( -1, -1 );
        m_pManager->RenderText( m_Font, r2, ui_font::h_left|ui_font::v_center, c1, (const char*)ControlName );

        r2 = r;
        r2.Translate( 1, -2 );
        m_pManager->RenderText( m_Font, r2, ui_font::h_right|ui_font::v_center, c2, (const char*)KeyName );
        r2.Translate( -1, -1 );
        m_pManager->RenderText( m_Font, r2, ui_font::h_right|ui_font::v_center, c1, (const char*)KeyName );
    }
}

//=========================================================================

void AddKey( ui_listbox* pList, warrior_control_data* pControlData, s32 GadgetID, xbool Shifted )
{
    xstring ShiftString;
    s32     ShiftMask = 0;

    if( Shifted ) ShiftString = "Shift + ";
    if( Shifted ) ShiftMask   = 0x80000000;

    if( !( (pControlData->ControlID == WARRIOR_CONTROL_SHIFT) && (Shifted) ) &&
        !( (GadgetID == INPUT_UNDEFINED) && (Shifted) ) )
    {
        pList->AddItem( ShiftString + xstring(KeyNameFromGadgetID(GadgetID)), ShiftMask|GadgetID );
    }
}

//=========================================================================

void ui_controllist::OnPadSelect( ui_win* pWin )
{
    (void)pWin;

    // If already selected
    if(m_Flags & WF_SELECTED )
    {
        if( m_iSelection != -1 )
        {
            s32 nRows = 8;
            warrior_control_data*   pControlData = (warrior_control_data*)GetSelectedItemData();

            // Determine number of options to show in list
            if( (pControlData->ControlID == WARRIOR_CONTROL_VOICE_CHAT) ||
                (pControlData->ControlID == WARRIOR_CONTROL_MOVE) ||
                (pControlData->ControlID == WARRIOR_CONTROL_LOOK) )
            {
                nRows = 2;
            }

            // Open list dialog to select new key
            m_NewKeyBinding = INPUT_NULL;
            irect r = m_Position;
            r.Translate( -r.l, -r.t );
            r.r -= 19;
            r.Deflate( 2, 0 );
            if( m_iSelection > -1 )
                r.Translate( 0, 4+(m_iSelection-m_iFirstVisibleItem)*18 );
            r.SetHeight( 24+18*nRows );
            r.Translate( 0, -(r.GetHeight()-18)/2 );
            LocalToScreen( r );
            ui_dlg_list* pListDialog = (ui_dlg_list*)m_pManager->OpenDialog( m_UserID, "ui_list", r, NULL, ui_win::WF_VISIBLE|ui_win::WF_INPUTMODAL );
            ui_listbox* pList = pListDialog->GetListBox();
            pListDialog->SetResultPtr( &m_NewKeyBinding );

            // Add all possible keys for this control
            if( pControlData->ControlID == WARRIOR_CONTROL_VOICE_CHAT )
            {
                pList->AddItem( KeyNameFromGadgetID(INPUT_VOICE_CHAT_RIGHT), INPUT_VOICE_CHAT_RIGHT);
                pList->AddItem( KeyNameFromGadgetID(INPUT_VOICE_CHAT_LEFT ), INPUT_VOICE_CHAT_LEFT );
                pList->SetSelection( pList->FindItemByData( ControlData[pControlData->ControlID].GadgetID ) );
            }
            else if( (pControlData->ControlID == WARRIOR_CONTROL_MOVE) || (pControlData->ControlID == WARRIOR_CONTROL_LOOK) )
            {
                pList->AddItem( KeyNameFromGadgetID(INPUT_PS2_STICK_LEFT_X ), INPUT_PS2_STICK_LEFT_X  );
                pList->AddItem( KeyNameFromGadgetID(INPUT_PS2_STICK_RIGHT_X), INPUT_PS2_STICK_RIGHT_X );
                pList->SetSelection( pList->FindItemByData( ControlData[pControlData->ControlID].GadgetID ) );
            }
            else
            {
                s32 Max = (ControlData[WARRIOR_CONTROL_SHIFT].GadgetID == INPUT_UNDEFINED) ? 1 : 2;
                if( pControlData->ControlID == WARRIOR_CONTROL_SHIFT )
                    Max = 1;
                for( s32 i=0 ; i<Max ; i++ )
                {
                    AddKey( pList, pControlData, INPUT_UNDEFINED,           (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_R_STICK,     (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L_STICK,     (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L1,          (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L2,          (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_R1,          (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_R2,          (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_TRIANGLE,    (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_CIRCLE,      (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_CROSS,       (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_SQUARE,      (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L_LEFT,      (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L_RIGHT,     (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L_UP,        (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_L_DOWN,      (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_SELECT,      (i == 1) );
                    AddKey( pList, pControlData, INPUT_PS2_BTN_START,       (i == 1) );
                }

                s32 Selection = pList->FindItemByData( pControlData->GadgetID | (pControlData->IsShifted?0x80000000:0) );
                if( Selection == -1 ) Selection = 0;
                pList->SetSelection( Selection );
            }
        }
    }
    else
    {
        // Set Selected
        m_Flags |= WF_SELECTED;

        audio_Play( SFX_FRONTEND_SELECT_02 );
        m_iSelectionBackup = m_iSelection;
    }
}

//=========================================================================

void ui_controllist::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    if( m_Flags & WF_SELECTED )
    {
        // Clear selected
        m_Flags &= ~WF_SELECTED;
        audio_Play( SFX_FRONTEND_CANCEL_02 );

        if( m_pParent )
            m_pParent->OnNotify( m_pParent, this, WN_LIST_CANCELLED, (void*)m_iSelection );
    }
    else
    {
        if( m_pParent )
            m_pParent->OnPadBack( pWin );
    }
}

//=========================================================================
