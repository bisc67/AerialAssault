//==============================================================================
//  
//  dlg_campaign.hpp
//  
//==============================================================================

#ifndef DLG_CAMPAIGN_HPP
#define DLG_CAMPAIGN_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

//==============================================================================
//  dlg_campaign
//==============================================================================

class ui_combo;
class ui_listbox;
class ui_button;
class ui_frame;
class ui_textbox;

extern void     dlg_campaign_register  ( ui_manager* pManager );
extern ui_win*  dlg_campaign_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags,void *pUserData );

class dlg_campaign : public ui_dialog
{
public:
                    dlg_campaign        ( void );
    virtual        ~dlg_campaign        ( void );

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
    void            CampaignToControls  ( void );
    void            ControlsToCampaign  ( void );

protected:
    s32             m_InUpdate;
    s32             m_OldMission;

//    ui_combo*       m_pDifficulty;
    ui_listbox*     m_pMissions;
    ui_textbox*     m_pCommunique;
    ui_button*      m_pBack;
//    ui_button*      m_pLoad;
//    ui_button*      m_pSave;
    ui_button*      m_pStart;
};

//==============================================================================
#endif // DLG_CAMPAIGN_HPP
//==============================================================================
