//==============================================================================
//  
//  dlg_warrior_setup.hpp
//  
//==============================================================================

#ifndef DLG_WARRIOR_SETUP_HPP
#define DLG_WARRIOR_SETUP_HPP

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "cleantext/cleantext.hpp"
#include "fe_globals.hpp"

//==============================================================================
//  dlg_warrior_setup
//==============================================================================

class ui_edit;
class ui_combo;
class ui_frame;
class ui_button;

extern void     dlg_warrior_setup_register  ( ui_manager* pManager );
extern ui_win*  dlg_warrior_setup_factory   ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class dlg_warrior_setup : public ui_dialog
{
public:
                    dlg_warrior_setup        ( void );
    virtual        ~dlg_warrior_setup        ( void );
                                            
    xbool           Create                   ( s32                       UserID,
                                               ui_manager*               pManager,
                                               ui_manager::dialog_tem*   pDialogTem,
                                               const irect&              Position,
                                               ui_win*                   pParent,
                                               s32                       Flags,
                                               void*                     pUserData );
    virtual void    Destroy                  ( void );
                                            
    virtual void    Render                   ( s32 ox=0, s32 oy=0 );
    void            RenderWarrior            ( const irect& Window, s32 Gender, s32 Skin, s32 Armor );
                                            
    virtual void    OnUpdate                 ( ui_win* pWin, f32 DeltaTime );
    virtual void    OnPadSelect              ( ui_win* pWin );
    virtual void    OnPadBack                ( ui_win* pWin );
    virtual void    OnNotify                 ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );

    void            SetupWeaponCombo         ( ui_combo* pCombo, const player_object::move_info& MoveInfo );
    void            SetupPackCombo           ( ui_combo* pCombo, const player_object::move_info& MoveInfo );
    void            AddWeaponToCombo         ( ui_combo* pCombo, const char* Name, s32 Type, const player_object::move_info& MoveInfo );


protected:
    void            WarriorToControls   ( void );
    void            ControlsToWarrior   ( xbool CheckDirty=FALSE );
    shape*          GetShape            ( s32 Data );
	void			UpdatePreview		( s32 Armor, s32 Weapon, s32 Pack );

protected:
    s32             m_iWarrior;
    s32             m_InUpdate;
    s32             m_SelectionID;
    s32             m_CharSelectionID;
    s32             m_SoundID;
    s32             m_PrevArmor;
    f32             m_StickX;
    f32             m_StickY;

    ui_edit*        m_pName;
    ui_combo*       m_pGender;
    ui_combo*       m_pSkin;
    ui_combo*       m_pVoice;

    ui_combo*       m_pArmor;
    ui_combo*       m_pWeapon;
    ui_combo*       m_pPack;

    ui_frame*       m_pFrame;
    ui_frame*       m_pFrame2;
    ui_frame*       m_pFrame3;
    ui_button*      m_pBack;
    ui_button*      m_pContinue;
    ui_button*      m_pControls;
    ui_button*      m_pBuddies;
//    ui_button*      m_pLoad;
//    ui_button*      m_pSave;
    ui_button*      m_pLoad1;
    ui_button*      m_pSave1;
	xbool			m_WaitingOnSave;
	xbool			m_DontPlayVoice;
//    ui_button*      m_pTest;

    CleanLanguage   m_Language;
};

//==============================================================================
#endif // DLG_WARRIOR_SETUP_HPP
//==============================================================================
