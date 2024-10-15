#ifndef _DLG_LOADSAVE_HPP
#define _DLG_LOADSAVE_HPP

//==============================================================================
//  
//  dlg_loadsave.hpp
//  
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "ui\ui_dialog.hpp"
#include "savedgame.hpp"
#include "dlg_message.hpp"
#include "support/cardmgr/cardmgr.hpp"

//==============================================================================
//  dlg_online
//==============================================================================

extern void     dlg_load_save_register( ui_manager* pManager );
extern ui_win*  dlg_load_save_factory ( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData );

class ui_edit;
class ui_combo;
class ui_listbox;
class ui_button;
class ui_check;
class ui_listbox;
class ui_slider;
class ui_text;

#define LS_STATE_STACK_SIZE 10

enum load_save_mode
{
    LOAD_WARRIOR,
    SAVE_WARRIOR,
    LOAD_NETWORK_OPTIONS,
	LOAD_AUDIO_OPTIONS,
	LOAD_CAMPAIGN_OPTIONS,
    SAVE_NETWORK_OPTIONS,
    SAVE_AUDIO_OPTIONS,
	SAVE_CAMPAIGN_OPTIONS,
	SAVE_CAMPAIGN_ENDOFGAME,
    LOAD_FILTER,
    SAVE_FILTER,
	ASK_FIRST = 1<<16,
};

#define MODE_OPTION_MASK (ASK_FIRST)

enum ls_errorcode
{
	LS_ERR_NONE = 0,
	LS_ERR_NO_SAVED_WARRIORS,
	LS_ERR_NO_CARD,
	LS_ERR_TOO_MANY_WARRIORS,
	LS_ERR_SAVE_NOT_FORMATTED,
	LS_ERR_LOAD_NOT_FORMATTED,
	LS_ERR_UNABLE_TO_FORMAT,
	LS_ERR_NO_SAVED_DATA,
	LS_ERR_WRONG_TYPE,
	LS_ERR_FULL,
	LS_ERR_SAVE_FAILED,
	LS_ERR_NO_OPTIONS,
	LS_ERR_TOO_MANY_FILTERS,
	LS_ERR_CORRUPT,
	LS_ERR_CARD_REMOVED,

};

enum load_save_state
{
    LS_STATE_IDLE = 0,
    LS_STATE_CHECKING_CARD,
    LS_STATE_WAIT_CHECK,
    LS_STATE_SELECT_CARD,
    LS_STATE_WAIT_CARD,
    LS_STATE_MESSAGE_DELAY,
    LS_STATE_MESSAGE_DELAY_EXIT,
    LS_STATE_MESSAGE_DELAY_POP,
    LS_STATE_START_SAVE,
    LS_STATE_WAIT_SAVE,
    LS_STATE_WAIT_DELETE,
    LS_STATE_SAVE,
    LS_STATE_CONFIRM_OVERWRITE,
    LS_STATE_CONFIRM_DELETE,
    LS_STATE_SAVING_DATA,
    LS_STATE_SAVE_COMPLETE,
    LS_STATE_SAVE_FAILED,
    LS_STATE_ASK_FORMAT,
    LS_STATE_FORMAT_DELAY,
    LS_STATE_START_DELETE,
    LS_STATE_DELETE,
    LS_STATE_LOAD,
	LS_STATE_ASK_SAVE,
	LS_STATE_WAIT_ASK_SAVE,
    LS_STATE_ERR_FULL,
    LS_STATE_ERR_CORRUPT,
    LS_STATE_ERR_NOTFORMATTED,
    LS_STATE_ERR_NODATA,
	LS_STATE_MESSAGE_CORRUPT_WAIT,
};

class load_save_params
{
public:
    load_save_params(s32 PlayerIndex, load_save_mode Mode,ui_dialog *pDialog=NULL,void *pData=NULL);

    ui_dialog*      GetDialog(void) { return m_pDialog;};
    load_save_mode  GetMode(void)   { return m_Mode;};
    void*           GetData(void)   { return m_pData;};
    s32             GetPlayerIndex(void) { return m_PlayerIndex;};

protected:
    ui_dialog*      m_pDialog;
    load_save_mode  m_Mode;
    void*           m_pData;
    s32             m_PlayerIndex;
};

struct dlg_stack_entry
{
    s32             State;
    dlg_message*    pMessage;
};

class dlg_load_save : public ui_dialog
{
public:
                    dlg_load_save       ( void );
    virtual        ~dlg_load_save       ( void );

    xbool           Create              ( s32                       UserID,
                                          ui_manager*               pManager,
                                          ui_manager::dialog_tem*   pDialogTem,
                                          const irect&              Position,
                                          ui_win*                   pParent,
                                          s32                       Flags,
                                          void*                     pUserData );

    virtual void    Destroy             ( void );

    virtual void    Render              ( s32 ox=0, s32 oy=0 );

    virtual void    OnPadNavigate       ( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats );
    virtual void    OnPadSelect         ( ui_win* pWin );
    virtual void    OnPadBack           ( ui_win* pWin );

    virtual void    OnNotify            ( ui_win* pWin, ui_win* pSender, s32 Command, void* pData );
    virtual void    OnUpdate            ( ui_win* pWin, f32 DeltaTime );


protected:
    void            UpdateControls      ( void );
    void            WarningBox          (const char *pTitle,const char *pMessage,xbool AllowPrematureExit=TRUE);
    void            OptionBox           (const char *pTitle,const char *pMessage,const xwchar* pYes = NULL,const xwchar* pNo = NULL,const xbool DefaultToNo=TRUE,const xbool AllowCancel=FALSE);
    void            LoadCardData        (void);
    s32             GetCardCount        (void);
    void            PickCardBox         (const char *pTitle,const char *pMessage);
    void            PushState           (load_save_state NewState);
    void            PopState            (void);
    void            SetState            (load_save_state NewState);
	void			ResetToTop			(void);
    load_save_state GetState            (void)      { return m_DialogState; };

    xbool           ValidateSavedData   (void);
    void            PrepareSavedData    (void);
	void			ErrorReset			(ls_errorcode Error);
	xbool			CheckCardForErrors	(void);

protected:
    ui_manager*     m_pManager;
    load_save_mode  m_Mode;             
    xbool           m_ForceReload;
    xbool           m_IsSaving;
    ui_listbox*     m_pDirectory;
    ui_button*      m_pCancel;
    ui_button*      m_pDelete;
    ui_button*      m_pSave;
    ui_dialog*      m_pParent;
    ui_combo*       m_pSlot;
    f32             m_CardScanDelay;
    s32             m_LastCardStatus;
    s32             m_CurrentCard;
    saved_game      m_SavedData;
    load_save_state m_DialogState;
    f32             m_MessageDelay;
    s32             m_MessageResult;
    s32             m_PlayerIndex;
    char            m_Filename[64];
    warrior_setup*  m_pWarrior;
//    saved_filter*   m_pFilter;
    dlg_message*    m_pMessage;
    load_save_state m_StateStack[LS_STATE_STACK_SIZE];
    s32             m_StateStackTop;
    s32             m_CardCount;
    s32             m_LastCardCount;
    s32             m_SelectedCard;
    xbool           m_Exit;
    s32				m_DataError;
    s16             m_DialogsAllocated;
    card_info		m_CardInfo[2];

};

#endif // _DLG_LOADSAVE_HPP
