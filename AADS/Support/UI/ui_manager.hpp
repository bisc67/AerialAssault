//==============================================================================
//  
//  ui_manager.hpp
//  
//==============================================================================

#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_TYPES_HPP
#include "x_math.hpp"
#include "x_color.hpp"
#endif

//==============================================================================
//  ui_manager
//==============================================================================

class ui_win;
class ui_dialog;

class ui_manager
{
public:

    //==========================================================================
    //  Templates for dialogs and controls
    //==========================================================================

    struct control_tem
    {
    };

    struct dialog_tem
    {
    };

    //==========================================================================
    //  Typedefs for window and dialog factories
    //==========================================================================

    typedef ui_win* (*ui_pfn_winfact)( s32 UserID, ui_manager* pManager, const irect& Position, ui_win* pParent, s32 Flags );
    typedef ui_win* (*ui_pfn_dlgfact)( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

    //==========================================================================
    //  Input Button Data
    //==========================================================================

    class button
    {
    };

    //==========================================================================
    //  Control States
    //==========================================================================

    enum control_state
    {
        CS_NORMAL               = 0,
        CS_HIGHLIGHT,
        CS_SELECTED,
        CS_HIGHLIGHT_SELECTED,
        CS_DISABLED
    };

    //==========================================================================
    //  User Data
    //==========================================================================

    enum user_navigate
    {
        NAV_UP,
        NAV_DOWN,
        NAV_LEFT,
        NAV_RIGHT,
    };

    struct user
    {
    };

    //==========================================================================
    //  Window Class
    //==========================================================================

    struct winclass
    {
    };

    //==========================================================================
    //  Graphic Element for UI
    //==========================================================================

    struct element
    {
    };

    //==========================================================================
    //  Background
    //==========================================================================

    struct background
    {
    };

    //==========================================================================
    //  Font
    //==========================================================================

    struct font
    {
    };

    //==========================================================================
    //  Dialog Class
    //==========================================================================

    struct dialogclass
    {
    };

    //==========================================================================
    //  Clip Record
    //==========================================================================

    struct cliprecord
    {
    };

    //==========================================================================
    //  Highlight
    //==========================================================================

    struct highlight
    {
    };

//==============================================================================
//  Functions
//==============================================================================

protected:
    void            UpdateButton        ( ui_manager::button& Button, xbool State, f32 DeltaTime );
    void            UpdateAnalog        ( ui_manager::button& Button, f32 Value, f32 DeltaTime );

public:
                    ui_manager          ( void );
                   ~ui_manager          ( void );

    void            Init                ( void );
    void            Kill                ( void );

    s32             LoadBackground      ( const char* pName, const char* pPathName );
    s32             FindBackground      ( const char* pName );
    void            RenderBackground    ( s32 iBackground ) const;

    s32             LoadElement         ( const char* pName, const char* pPathName, s32 nStates, s32 cx, s32 cy );
    s32             FindElement         ( const char* pName ) const;
    void            RenderElement       ( s32 iElement, const irect& Position,       s32 State, const xcolor& Color = XCOLOR_WHITE, xbool IsAdditive = FALSE ) const;
    void            RenderElementUV     ( s32 iElement, const irect& Position, const irect& UV, const xcolor& Color = XCOLOR_WHITE, xbool IsAdditive = FALSE ) const;
    const element*  GetElement          ( s32 iElement ) const;

    s32             LoadFont            ( const char* pName, const char* pPathName );
    s32             FindFont            ( const char* pName ) const;
    void            RenderText          ( s32 iFont, const irect& Position, s32 Flags, const xcolor& Color, const   char* pString ) const;
    void            RenderText          ( s32 iFont, const irect& Position, s32 Flags, const xcolor& Color, const xwchar* pString ) const;
    void            RenderText          ( s32 iFont, const irect& Position, s32 Flags,       s32     Alpha, const xwchar* pString ) const;
    void            TextSize            ( s32 iFont, irect& Rect, const xwchar* pString, s32 Count ) const;
    s32             GetLineHeight       ( s32 iFont ) const;

    void            RenderRect          ( const irect& r, const xcolor& Color, xbool IsWire=TRUE ) const;
    void            RenderGouraudRect   ( const irect& r, const xcolor& c1, const xcolor& c2, const xcolor& c3, const xcolor& c4, xbool IsWire=TRUE ) const;

    xbool           RegisterWinClass    ( const char* ClassName, ui_pfn_winfact pFactory );
    ui_win*         CreateWin           ( s32 UserID, const char* ClassName, const irect& Position, ui_win* pParent, s32 Flags );

    xbool           RegisterDialogClass ( const char* ClassName, dialog_tem* pDialogTem, ui_pfn_dlgfact pFactory );
    ui_dialog*      OpenDialog          ( s32 UserID, const char* ClassName, irect Position, ui_win* pParent, s32 Flags, void* pUserData = NULL );
    void            EndDialog           ( s32 UserID, xbool ResetCursor = FALSE );
    void            EndUsersDialogs     ( s32 UserID );
    s32             GetNumUserDialogs   ( s32 UserID );
    ui_dialog*      GetTopmostDialog    ( s32 UserID );

    s32             CreateUser          ( s32 ControllerID, const irect& Bounds, s32 Data = 0 );
    void            DeleteUser          ( s32 UserID );
    void            DeleteAllUsers      ( void );
    user*           GetUser             ( s32 UserID ) const;
    s32             GetUserData         ( s32 UserID ) const;
    ui_win*         GetWindowUnderCursor( s32 UserID ) const;
    void            SetCursorVisible    ( s32 UserID, xbool State );
    xbool           GetCursorVisible    ( s32 UserID ) const;
    void            SetCursorPos        ( s32 UserID, s32  x, s32  y );
    void            GetCursorPos        ( s32 UserID, s32& x, s32& y ) const;
    ui_win*         SetCapture          ( s32 UserID, ui_win* pWin );
    void            ReleaseCapture      ( s32 UserID );
    void            SetUserBackground   ( s32 UserID, s32 iBackground );
    const irect&    GetUserBounds       ( s32 UserID ) const;
    void            EnableUser          ( s32 UserID, xbool State );
    xbool           IsUserEnabled       ( s32 UserID ) const;
    void            AddHighlight        ( s32 UserID, irect& r, xbool Flash = TRUE );

    void            PushClipWindow      ( const irect &r );
    void            PopClipWindow       ( void );

    ui_win*         GetWindowAtXY       ( user* pUser, s32 x, s32 y );
    xbool           ProcessInput        ( f32 DeltaTime );
    xbool           ProcessInput        ( f32 DeltaTime, s32 UserID );

    void            EnableUserInput     ( void );
    void            DisableUserInput    ( void );

    void            Update              ( f32 DeltaTime );
    void            Render              ( void );

//  const xwstring& WordWrapString      ( s32 iFont, const irect& r, const char* pString );
//  const xwstring& WordWrapString      ( s32 iFont, const irect& r, const xwstring& String );

    void            SetRes              ( void );

//==============================================================================
//  Data
//==============================================================================

protected:

};

//==============================================================================
#endif // UI_MANAGER_HPP
//==============================================================================
