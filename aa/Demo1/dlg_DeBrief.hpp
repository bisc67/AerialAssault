//==============================================================================
//  
//  dlg_debrief.hpp
//  
//==============================================================================

#ifndef DLG_DEBRIEF_HPP
#define DLG_DEBRIEF_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_debrief
//==============================================================================

class ui_combo;
class ui_listbox;
class ui_button;
class ui_frame;
class ui_textbox;
extern s32 LastSaved;
extern void     dlg_debrief_register  ( ui_manager* pManager );
extern ui_win*  dlg_debrief_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData );

class dlg_debrief : public ui_dialog
{
public:
                    dlg_debrief         ( void );
    virtual        ~dlg_debrief         ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData);
    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );
    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

protected:
    s32             m_InUpdate;
    
    s32             m_SoundID;
    xbitmap         m_Image;
    ui_frame*       m_pImage;
    ui_textbox*     m_pBrief;
    ui_button*      m_pBack;
    ui_button*      m_pStart;
};

//==============================================================================
#endif // DLG_DEBRIEF_HPP
//==============================================================================
