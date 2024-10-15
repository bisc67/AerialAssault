//=========================================================================
//
//  ui_manager.cpp
//
//=========================================================================

#include "ui_manager.hpp"

//=========================================================================
//  Defines
//=========================================================================

#define ENABLE_SCREENSHOTS  0

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================


//=========================================================================
//  Helpers
//=========================================================================

void ui_manager::UpdateButton( ui_manager::button& Button, xbool State, f32 DeltaTime )
{
}

//=========================================================================

void ui_manager::UpdateAnalog( ui_manager::button& Button, f32 Value, f32 DeltaTime )
{
}

//=========================================================================
//  ui_manager
//=========================================================================

ui_manager::ui_manager( void )
{
}

//=========================================================================

ui_manager::~ui_manager( void )
{
}

//=========================================================================

void ui_manager::Init( void )
{
}

//=========================================================================

void ui_manager::Kill( void )
{
}

//=========================================================================

s32 ui_manager::LoadBackground( const char* pName, const char* pPathName )
{
    return( 1 );
}

//=========================================================================

s32 ui_manager::FindBackground( const char* pName )
{
    return( 1 );
}

//=========================================================================

void ui_manager::RenderBackground( s32 iBackground ) const
{
}

//=========================================================================

s32 ui_manager::LoadElement( const char* pName, const char* pPathName, s32 nStates, s32 cx, s32 cy )
{
    return( 1 );
}

//=========================================================================

s32 ui_manager::FindElement( const char* pName ) const
{
    return( 1 );
}

//=========================================================================

void ui_manager::RenderElement( s32 iElement, const irect& Position, s32 State, const xcolor& Color, xbool IsAdditive ) const
{
}

//=========================================================================

void ui_manager::RenderElementUV( s32 iElement, const irect& Position, const irect& UV, const xcolor& Color, xbool IsAdditive ) const
{
}

//=========================================================================

const ui_manager::element* ui_manager::GetElement( s32 iElement ) const
{
    return( NULL );
}

//=========================================================================

s32 ui_manager::LoadFont( const char* pName, const char* pPathName )
{
    return( 1 );
}

//=========================================================================

s32 ui_manager::FindFont( const char* pName ) const
{
    return( 1 );
}

//=========================================================================

void ui_manager::RenderText( s32 iFont, const irect& Position, s32 Flags, const xcolor& Color, const char* pString ) const
{
}

//=========================================================================

void ui_manager::RenderText( s32 iFont, const irect& Position, s32 Flags, const xcolor& Color, const xwchar* pString ) const
{
}

//=========================================================================

void ui_manager::RenderText( s32 iFont, const irect& Position, s32 Flags, s32 Alpha, const xwchar* pString ) const
{
}

//=========================================================================

void ui_manager::TextSize( s32 iFont, irect& Rect, const xwchar* pString, s32 Count ) const
{
}

//=========================================================================

s32 ui_manager::GetLineHeight( s32 iFont ) const
{
    return( 10 );
}

//=========================================================================

void ui_manager::RenderRect( const irect& r, const xcolor& Color, xbool IsWire ) const
{
}

//=========================================================================

void ui_manager::RenderGouraudRect( const irect& r, const xcolor& c1, const xcolor& c2, const xcolor& c3, const xcolor& c4, xbool IsWire ) const
{
}

//=========================================================================

xbool ui_manager::RegisterWinClass ( const char* ClassName, ui_pfn_winfact pFactory )
{
    return( TRUE );
}

//=========================================================================

ui_win* ui_manager::CreateWin( s32 UserID, const char* ClassName, const irect& Position, ui_win* pParent, s32 Flags )
{
    return( NULL );
}

//=========================================================================

s32 ui_manager::CreateUser( s32 ControllerID, const irect& Bounds, s32 Data )
{
    return( 1 );
}

//=========================================================================

void ui_manager::DeleteAllUsers( void )
{
}

//=========================================================================

void ui_manager::DeleteUser( s32 UserID )
{
}

//=========================================================================

ui_manager::user* ui_manager::GetUser( s32 UserID ) const
{
    return( NULL );
}

//=========================================================================

s32 ui_manager::GetUserData( s32 UserID ) const
{
    return( 1 );
}

//=========================================================================
ui_win* ui_manager::GetWindowUnderCursor( s32 UserID ) const
{
    return( NULL );
}

//=========================================================================

void ui_manager::SetCursorVisible( s32 UserID, xbool State )
{
}

//=========================================================================

xbool ui_manager::GetCursorVisible( s32 UserID ) const
{
    return( TRUE );
}

//=========================================================================

void ui_manager::SetCursorPos( s32 UserID, s32 x, s32 y )
{
}

//=========================================================================

void ui_manager::GetCursorPos( s32 UserID, s32& x, s32& y ) const
{
}

//=========================================================================

ui_win* ui_manager::SetCapture( s32 UserID, ui_win* pWin )
{
    return( NULL );
}

//=========================================================================

void ui_manager::ReleaseCapture( s32 UserID )
{
}

//=========================================================================

void ui_manager::SetUserBackground( s32 UserID, s32 iBackground )
{
}

//=========================================================================

static irect Rect( 0, 0, 100, 100 );

const irect& ui_manager::GetUserBounds( s32 UserID ) const
{
    return( Rect );
}

//=========================================================================

void ui_manager::EnableUser( s32 UserID, xbool State )
{
}

//=========================================================================

xbool ui_manager::IsUserEnabled( s32 UserID ) const
{
    return( TRUE );
}

//=========================================================================

void ui_manager::AddHighlight( s32 UserID, irect& r, xbool Flash )
{
}

//=========================================================================

xbool ui_manager::ProcessInput( f32 DeltaTime )
{
    return( FALSE );
}

//=========================================================================

ui_win* ui_manager::GetWindowAtXY( user* pUser, s32 x, s32 y )
{
    return( NULL );
}

//=========================================================================

xbool ui_manager::ProcessInput( f32 DeltaTime, s32 UserID )
{
    return( FALSE );
}

//=========================================================================

void ui_manager::EnableUserInput( void )
{
}

//=========================================================================

void ui_manager::DisableUserInput( void )
{
}

//=========================================================================

void ui_manager::Update( f32 DeltaTime )
{
}

//=========================================================================

void ui_manager::Render( void )
{
}

//=========================================================================

xbool ui_manager::RegisterDialogClass( const char* ClassName, dialog_tem* pDialogTem, ui_pfn_dlgfact pFactory )
{
    return( TRUE );
}

//=========================================================================

ui_dialog* ui_manager::OpenDialog( s32 UserID, const char* ClassName, irect Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    return( NULL );
}

//=========================================================================

void ui_manager::EndDialog( s32 UserID, xbool ResetCursor )
{
}

//=========================================================================

void ui_manager::EndUsersDialogs( s32 UserID )
{
}

//=========================================================================

s32 ui_manager::GetNumUserDialogs( s32 UserID )
{
    return( 0 );
}

//=========================================================================

ui_dialog* ui_manager::GetTopmostDialog( s32 UserID )
{
    return( NULL );
}

//=========================================================================

void ui_manager::PushClipWindow( const irect &r )
{
}

//=========================================================================

void ui_manager::PopClipWindow( void )
{
}

//=========================================================================

void ui_manager::SetRes( void )
{
}

//=========================================================================
