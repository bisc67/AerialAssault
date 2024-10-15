//==============================================================================
//  
//  dlg_player.hpp
//  
//==============================================================================

#ifndef DLG_PLAYER_HPP
#define DLG_PLAYER_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"

class ui_button;
class ui_check;
class ui_slider;

//==============================================================================
//  dlg_player
//==============================================================================

extern void     dlg_player_register  ( ui_manager* pManager );
extern ui_win*  dlg_player_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_player : public ui_dialog
{
public:
                    dlg_player          ( void );
    virtual        ~dlg_player          ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );
    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );

    virtual void    OnPadNavigate       ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );
    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

    void            ControlsToWarrior   ( void );
    void            WarriorToControls   ( void );

protected:
    u32             GetKickMask         ( void );   // Bit for each player which can be kicked.

    ui_check*       m_pInvertPlayerY;
    ui_check*       m_pInvertVehicleY;
    ui_check*       m_p3rdPlayer;
    ui_check*       m_p3rdVehicle;
    ui_check*       m_pDualshock;
    ui_button*      m_pExit;
    ui_button*      m_pChangeTeam;
    ui_button*      m_pSuicide;

    ui_button*      m_pVoteMap;
    ui_button*      m_pVoteKick;
    ui_button*      m_pVoteYes;
    ui_button*      m_pVoteNo;

    ui_slider*      m_pSensX;
    ui_slider*      m_pSensY;

    xbool           m_NoUpdate;
    s32             m_iChangeMap;
    s32             m_iKickPlayer;
    xbool           m_DoChangeTeam;
    xbool           m_DoSuicide;
    xbool           m_DoExitGame;

    s32             m_WarriorID;
};

//==============================================================================
#endif // DLG_PLAYER_HPP
//==============================================================================
