//==============================================================================
//  
//  dlg_score.hpp
//  
//==============================================================================

#ifndef DLG_SCORE_HPP
#define dlg_score_hpp

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_score
//==============================================================================

class ui_frame;

extern void     dlg_score_register  ( ui_manager* pManager );
extern ui_win*  dlg_score_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_score : public ui_dialog
{
public:
                    dlg_score           ( void );
    virtual        ~dlg_score           ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );
    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

protected:
    void            RenderScoreTitle    ( const irect& rl, const xcolor& Color, const xwchar* Title1, const xwchar* Title2, const xwchar* Title3 );
    void            RenderPlayerScore   ( const irect& rl, const xcolor& Color, s32 Pos, const xwchar* Name, s32 Score, s32 Kills );

protected:
    ui_frame*       m_pFrame;
    ui_frame*       m_pFrame2;
    ui_frame*       m_pFrame3;
};

//==============================================================================
#endif // DLG_SCORE_HPP
//==============================================================================
