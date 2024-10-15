//==============================================================================
//  
//  dlg_mission_end.hpp
//  
//==============================================================================

#ifndef DLG_MISSION_END_HPP
#define DLG_MISSION_END_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "GameMgr\GameMgr.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_mission_end_register( ui_manager* pManager );
extern ui_win*  dlg_mission_end_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_frame;
class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

class dlg_mission_end : public ui_dialog
{
public:
                    dlg_mission_end     ( void );
    virtual        ~dlg_mission_end     ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );

    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );
    void            RenderScoreTitle    ( const irect& rl, const xcolor& Color, const xwchar* Title1, const xwchar* Title2, const xwchar* Title3 );
    void            RenderPlayerScore   ( const irect& rl, const xcolor& Color, s32 Pos, const xwchar* Name, s32 Score, s32 Kills );

    virtual void    OnPadNavigate       ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );

    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );

protected:
    game_score      m_GmScore;
    s32             m_WinningTeam;
    s32             m_Player1Index;
    s32             m_Player2Index;
    xbool           m_WaitOnSave;

    ui_frame*       m_pFrame1;
    ui_frame*       m_pFrame2;
    ui_button*      m_pContinue;
};

//==============================================================================
#endif // DLG_MISSION_END_HPP
//==============================================================================
