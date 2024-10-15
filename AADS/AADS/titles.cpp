#include "x_files.hpp"
#include "entropy.hpp"
#include "titles.hpp"
#include "support\movieplayer\movieplayer.hpp"
#include "cardmgr/cardmgr.hpp"
#include "savedgame.hpp"
#include "globals.hpp"
#include "ui/ui_manager.hpp"
#include "ui/ui_font.hpp"
#include "audiomgr/audio.hpp"
#include "LabelSets\Tribes2Types.hpp"
#include "serverman.hpp"
#include "masterserver/masterserver.hpp"
#include "sm_common.hpp"
#include "ui\ui_win.hpp"
#include "dlg_message.hpp"
#include "dlg_WarriorSetup.hpp"
#include "x_threads.hpp"
#include "dlg_DeBrief.hpp"
#include "Demo1/SpecialVersion.hpp"

xbitmap         s_TitleBitmap;
xbool           s_TitleLoaded = FALSE;
//extern s32 LastSaved;

#ifdef bwatson
#ifndef DEDICATED_SERVER
#define ENABLE_MOVIES
#endif
#endif

enum memcard_errors
{
    TS_ERR_OK=0,
    TS_ERR_NOTFOUND,
    TS_ERR_UNFORMATTED,
    TS_ERR_FULL,
    TS_ERR_CORRUPT,
    TS_ERR_ATTACH_FAILED,
	TS_ERR_ATTACH_ABORTED,
	TS_ERR_ATTACH_TIMEOUT,
    TS_ERR_NO_NET_CONFIG,
	TS_ERR_INVALID_NET_CONFIG,
    TS_ERR_UNKNOWN,
    TS_ERR_WRONGTYPE,
    TS_ERR_BAD_NET_CONFIG,
    TS_ERR_ATTACH_OK,
};

static f32              s_Delay;
static s32              s_LastError;
static s32              s_LastErrorSlot;
static dlg_message*     s_pMessage=NULL;
static s32				s_Result;
static xbool            s_InitialCheckDone=FALSE;
#ifdef ENABLE_MOVIES
static xbool            s_MoviePlaying=FALSE;
#endif

extern xbool UseAutoAimHelp;
extern xbool UseAutoAimHint;

xbool           titlecheck_Advance(f32 DeltaTime);
extern server_manager*  pSvrMgr;

xbool UpdateFrontEnd( f32 DeltaSec );

void LoadLogo(const char *pName)
{
    if (s_TitleLoaded)
    {
        title_ClearLogo();
    }

#ifdef TARGET_PS2
    auxbmp_LoadNative(s_TitleBitmap,xfs("data/titles/ps2/%s",pName));
#else
    auxbmp_LoadNative(s_TitleBitmap,xfs("data/titles/pc/%s",pName));
#endif

    vram_Register(s_TitleBitmap);
    s_TitleLoaded = TRUE;
}

xbool s_OptionsWereLoaded;

void DrawLogo(s32 transparency)
{
    s32 width,height,starty;

    eng_GetRes(width,height);

    eng_Begin();
    draw_Begin( DRAW_SPRITES, DRAW_2D|DRAW_TEXTURED|DRAW_NO_ZBUFFER );
    draw_SetTexture( s_TitleBitmap );
    if (height > 448)
    {
        starty = 0;
    }
    else
    {
        starty = -32;
    }
    draw_Sprite( vector3(0,(f32)starty,0),vector2(512,512), xcolor(transparency,transparency,transparency,transparency) );

    draw_End();
    eng_End();
    eng_PageFlip();
}
//=================================================================================================
s32 title_MemcardCheck(s32 cardnum,xbool &LoadOptions)
{
    card_info   Info;
    s32         status;
    char        name[128];
    card_file   *pFile;
    s32         i;
    s32         CurrentState;

    CurrentState = CARD_ERR_NOTFOUND;

    x_sprintf(name,"card0%d:",cardnum);
    status = card_Check(name,&Info);        // We want to ignore the changed check
    status = card_Check(name,&Info);        // We want to ignore the changed check

    if ( (status == CARD_ERR_OK) || (status==CARD_ERR_CHANGED) )
    {
        x_sprintf(name,"card0%d:/%s/%s",cardnum,SAVE_DIRECTORY,SAVE_FILENAME);
        pFile = card_Open(name,"rb");
        // If the file exists, we don't have to worry about the remaining
        // amount of space since it's already been created.
        if ((s32)pFile>0)
        {
            saved_game *pSavedData;
            s32 checksum,SavedChecksum;

            CurrentState = CARD_ERR_OK;

            pSavedData = (saved_game *)x_malloc(sizeof(saved_game));
            ASSERT(pSavedData);
            card_Read(pFile,pSavedData,sizeof(saved_game));
            SavedChecksum = pSavedData->Checksum;
            pSavedData->Checksum = 0;
            checksum = card_Checksum((u8 *)pSavedData,sizeof(saved_game),SAVED_CHECKSUM_SEED);
            if (checksum != SavedChecksum)
            {
                CurrentState = CARD_ERR_CORRUPT;
            }
            else
            {
				if (LoadOptions)
				{
					s_OptionsWereLoaded = TRUE;
					title_SetOptions(&pSavedData->Options,ALL_OPTIONS);
					LoadOptions = FALSE;
					// if the autoconnect option is set, we try and activate the network
					// configuration now. We don't really care right here if it succeeds 
					// or not.
					// Restore the last saved warrior for player 1 and player 2
					// Only restore if we have a valid saved warrior in that slot
					for (i=0;i<2;i++)
					{
						if ( (pSavedData->LastWarriorIndex[i] < pSavedData->WarriorCount) &&
							 (pSavedData->LastWarriorIndex[i] >=0 ) &&
							 (pSavedData->LastWarriorValid & (1<<i)) )
						{
							fegl.WarriorSetups[i] = pSavedData->Warrior[pSavedData->LastWarriorIndex[i]];
							fegl.WarriorSetups[i].LastPauseTab = 0;
						}
					}
				}
            }

            x_free(pSavedData);
            card_Close(pFile);
        }
        else
        {
            if ((s32)pFile != CARD_ERR_NOTFOUND)
            {
                CurrentState = (s32)pFile;
            }
            else
            {
                if (Info.Free * 1024 >= SAVE_FILE_SIZE)
                {
                    CurrentState = CARD_ERR_OK;
                }
                else
                {
                    CurrentState = CARD_ERR_FULL;
                }
            }
        }
    }
    else
    {
        CurrentState = status;
    }

    return CurrentState;
}

//=================================================================================================
void title_InevitableLogo(xbool WaitForEnd)
{
    (void)WaitForEnd;

    title_ClearLogo();
#ifdef ENABLE_MOVIES
    if (WaitForEnd)
    {
        if (!s_MoviePlaying)
            return;
        movie_WaitAsync(FALSE);
        s_MoviePlaying=FALSE;

    }
    else
    {
		s32 x,y;
		eng_GetRes(x,y);
		if (y>448)
		{
			movie_PlayAsync("data/movies/pal/inevitable.pss",512,448);
		}
		else
		{
			movie_PlayAsync("data/movies/inevitable.pss",512,448);
		}
        s_MoviePlaying=TRUE;
    }
#else
#endif
	tgl.ActualTime = x_GetTime();

}

//=================================================================================================
void title_ClearLogo(void)
{
    s32 i;

    if (s_TitleLoaded)
    {
        for (i=7;i>=0;i--)
        {
            DrawLogo(i*32);
        }
        DrawLogo(0);

        vram_Unregister(s_TitleBitmap);
        s_TitleBitmap.Kill();
        s_TitleLoaded = FALSE;
    }
}

//=================================================================================================
void title_SierraLogo(xbool WaitForEnd)
{
    (void)WaitForEnd;

    title_ClearLogo();
#ifdef ENABLE_MOVIES
    if (WaitForEnd)
    {
        if (!s_MoviePlaying)
            return;
        movie_WaitAsync(FALSE);
        s_MoviePlaying=FALSE;

    }
    else
    {
        s_MoviePlaying=TRUE;
        title_InitialCheck();
		s32 x,y;
		eng_GetRes(x,y);
		if (y>448)
		{
			movie_PlayAsync("data/movies/pal/sierra.pss",512,448);
		}
		else
		{
			movie_PlayAsync("data/movies/sierra.pss",512,448);
		}
    }
#else
    if (!WaitForEnd)
        title_InitialCheck();
#endif
	tgl.ActualTime = x_GetTime();
}

//=================================================================================================
void title_TribesLogo(void)
{
    s32 i;

    LoadLogo("tribeslogo.xbmp");

    for (i=1;i<8;i++)
    {
        DrawLogo(i*32);
    }
    DrawLogo(255);
    DrawLogo(255);
}

//=================================================================================================
void title_IntroMovie(void)
{
    #ifndef jcossigny
	s32 x,y;
	eng_GetRes(x,y);
	if (y>448)
	{
		movie_Play("data/movies/pal/intro.pss",512,448,tgl.pUIManager);
	}
	else
	{
		movie_Play("data/movies/intro.pss",512,448,tgl.pUIManager);
	}
    #endif
	tgl.ActualTime = x_GetTime();
}

//=================================================================================================
void title_Credits(void)
{
    audio_Stop(tgl.MusicId);
	while( audio_IsPlaying( tgl.MusicId ) )
	{
		audio_Update();
		x_DelayThread(10);
	}
    tgl.MusicId = 0;
	s32 x,y;
	eng_GetRes(x,y);
	if (y>448)
	{
		movie_Play("data/movies/pal/credits.pss",512,448,tgl.pUIManager);
	}
	else
	{
	    movie_Play("data/movies/credits.pss",512,448,tgl.pUIManager);
	}
	for (s32 i=0;i<15;i++)
	{
		audio_Update();
		x_DelayThread(10);
	}
	tgl.ActualTime = x_GetTime();
}

//=================================================================================================
void title_SetOptions(saved_options *pSavedData,s32 SetMask)
{

    if (SetMask & AUDIO_OPTIONS)
    {
        tgl.MasterVolume    = pSavedData->MasterVolume;
        tgl.VoiceVolume     = pSavedData->VoiceVolume;
        tgl.EffectsVolume   = pSavedData->EffectsVolume;
        tgl.MusicVolume     = pSavedData->MusicVolume;
        fegl.AudioSurround  = pSavedData->SurroundEnabled;

        audio_SetMasterVolume   (tgl.MasterVolume,tgl.MasterVolume);
        if (tgl.EffectsPackageId) audio_SetContainerVolume(tgl.EffectsPackageId,tgl.EffectsVolume);
        if (tgl.MusicPackageId)   audio_SetContainerVolume(tgl.MusicPackageId,tgl.MusicVolume);
        if (tgl.VoicePackageId)   audio_SetContainerVolume(tgl.VoicePackageId,tgl.VoiceVolume);
        audio_SetSurround       (fegl.AudioSurround);
    }

	if (SetMask & NETWORK_OPTIONS)
	{
		fegl.ServerSettings = pSavedData->ServerSettings;
		fegl.AutoConnect    = pSavedData->AutoConnect;
		fegl.LastNetConfig  = pSavedData->LastNetConfig;
		fegl.SortKey        = pSavedData->SortKey;

		// This will force a port re-open. This will make sure we
		// get the port defined within the saved data file.
		if ( (tgl.UserInfo.MyAddr.IP) && (tgl.UserInfo.MyAddr.Port != MIN_USER_PORT) )
		{
			net_Unbind(tgl.UserInfo.MyAddr);
			tgl.UserInfo.MyAddr.Clear();
			if (pSvrMgr)
				pSvrMgr->SetLocalAddr(tgl.UserInfo.MyAddr);
			if (tgl.pMasterServer)
				tgl.pMasterServer->ClearAddress();
		}
	}

	if (SetMask & CAMPAIGN_OPTIONS)
	{
		fegl.ServerSettings = pSavedData->ServerSettings;
		if( pSavedData->HighestCampaign < 6 )
			tgl.HighestCampaignMission = 6;
		else if( pSavedData->HighestCampaign > 16 )
			tgl.HighestCampaignMission = 16;
		else
			tgl.HighestCampaignMission = pSavedData->HighestCampaign;

        #ifdef DEMO_DISK_HARNESS
        tgl.HighestCampaignMission = 5;
        #endif

	    LastSaved = tgl.HighestCampaignMission;
	}

    UseAutoAimHint		= pSavedData->UseAutoAimHint;
	UseAutoAimHelp		= pSavedData->UseAutoAimHelp;

//    fegl.CurrentFilter  = pSavedData->LastFilter;
}

//=================================================================================================
void title_GetOptions(saved_options *pSavedData,s32 GetMask)
{
	if (GetMask & AUDIO_OPTIONS)
	{
		pSavedData->MasterVolume    = tgl.MasterVolume;
		pSavedData->VoiceVolume     = tgl.VoiceVolume;
		pSavedData->EffectsVolume   = tgl.EffectsVolume;
		pSavedData->MusicVolume     = tgl.MusicVolume;
		pSavedData->SurroundEnabled = fegl.AudioSurround;
	}

	if (GetMask & NETWORK_OPTIONS)
	{
	    pSavedData->AutoConnect     = fegl.AutoConnect;
	    pSavedData->ServerSettings  = fegl.ServerSettings;
		pSavedData->LastNetConfig   = fegl.LastNetConfig;
	}

	if (GetMask & CAMPAIGN_OPTIONS)
	{
		pSavedData->HighestCampaign = tgl.HighestCampaignMission;
	    pSavedData->ServerSettings  = fegl.ServerSettings;
	}

//	pSavedData->LastFilter      = fegl.CurrentFilter;
    pSavedData->SortKey         = fegl.SortKey;
	pSavedData->UseAutoAimHint	= UseAutoAimHint;
	pSavedData->UseAutoAimHelp  = UseAutoAimHelp;


    // We need to zero the checksum field since it will be included in the checksummed data
    // and so it needs to be constant to prevent anomalous checksum failures.
    pSavedData->Checksum = 0;
    pSavedData->Checksum = card_Checksum((u8 *)pSavedData,sizeof(saved_options),SAVED_CHECKSUM_SEED);
}


//=================================================================================================
void title_Init_Begin(void)
{
    // Play the intro movie
    title_IntroMovie();

    // If we've done the initial check already, we don't need to enter the 
    // title init stage at all. We just go straight to the front end.
    if (s_InitialCheckDone)
    {
        sm_SetState(STATE_COMMON_INIT_FRONTEND);
		return;
    }
    // We know that we've already consumed tons of time while
    // the intro movies are playing so we can make this timeout
    // very short. This is used when we need to verify that the
    // network connection has been established.
    s_Delay = 90.0f;
	switch (s_LastError)
	{
	case TS_ERR_CORRUPT:
	case TS_ERR_FULL:
	case TS_ERR_UNFORMATTED:
	case TS_ERR_NOTFOUND:
	case TS_ERR_WRONGTYPE:
		char ErrorString[64];

        if (!s_pMessage)
        {
			tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
            // Open dialog box containing message that we're waiting
            // for the connection to complete.
            s_pMessage = (dlg_message *)tgl.pUIManager->OpenDialog(fegl.UIUserID,
                            "message",
                            irect(0,0,300,200),
                            NULL,
                            ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER);
        }
        x_sprintf(ErrorString,"Checking for a memory card(8MB)\n"
							  "(for PlayStation""\x10""2)...\n");
        s_pMessage->Configure(
			xwstring("PLEASE WAIT..."), 
                       NULL,
                       NULL, 
                       xwstring(ErrorString), 
                       XCOLOR_WHITE,
                       &s_Result
                       );
        {
		    for (s32 i=0;i<20;i++)
		    {
			    tgl.pUIManager->Render();
			    eng_PageFlip();
		    }
        }
		title_InitialCheck();
        tgl.pUIManager->EndDialog(fegl.UIUserID,TRUE);
		s_pMessage = NULL;
	default:
		break;
	}
}

//=================================================================================================
// This is where we would wait for the connect to complete

void title_Init_Advance(f32 DeltaTime)
{
    xbool           message;
    char            ErrorString[512];
    char            *pHeader;
    f32             MessageDuration;
    s32             soundeffect;

    s_InitialCheckDone = TRUE;

    pHeader = "ERROR!";

    UpdateFrontEnd(DeltaTime);
    if (fegl.AutoConnect)
    {
        interface_info info;

        net_GetInterfaceInfo(-1,info);
        if (!info.Address)
        {
            xbool error;
            s32 id,state;

            s_Delay -= DeltaTime;

            state = net_GetAttachStatus(id);
            error = (s_Delay <= 0.0f) || 
                    (s_Result != DLG_MESSAGE_IDLE) ||
                    (state == ATTACH_STATUS_ERROR);

            if ( !error )
            {
                if (!s_pMessage)
                {
			        tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
                    // Open dialog box containing message that we're waiting
                    // for the connection to complete.
                    s_pMessage = (dlg_message *)tgl.pUIManager->OpenDialog(fegl.UIUserID,
                                    "message",
                                    irect(0,0,300,200),
                                    NULL,
                                    ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER);
                }
                x_sprintf(ErrorString,"Waiting for network autoconnect to\n"
                                      "complete. This will take a moment.\n"
                                      "Timeout remaining: %d seconds.",(s32)s_Delay);

                s_pMessage->Configure( xwstring("PLEASE WAIT..."), 
                               NULL,
                               xwstring("CANCEL"), 
                               xwstring(ErrorString), 
                               XCOLOR_WHITE,
                               &s_Result
                               );


                return;
            }
            else
            {
				fegl.DeliberateDisconnect = TRUE;
                net_ActivateConfig(FALSE);

				if ( (s_Delay <= 0.0f) || (state == ATTACH_STATUS_ERROR) )
				{
					s_LastError = TS_ERR_ATTACH_TIMEOUT;
				}
				else
				{
					s_LastError = TS_ERR_ATTACH_ABORTED;
				}
            }
        }
        else
        {
            if (s_LastError == TS_ERR_OK)
                s_LastError = TS_ERR_ATTACH_OK;
        }
        if (s_pMessage)
        {
            if (s_Result == DLG_MESSAGE_IDLE)
            {
                // Close the dialog containing the message. We can only do this if
                // the message state is idle since the message dialog box code will
                // close itself if a button option has been selected
                tgl.pUIManager->EndDialog(fegl.UIUserID,TRUE);
                s_pMessage = NULL;
            }
        }
    }

    message = TRUE;
    MessageDuration = -1.0f;
    soundeffect = SFX_FRONTEND_ERROR;
    switch (s_LastError)
    {
        case    TS_ERR_OK:
            message=FALSE;
            break;
        case TS_ERR_NOTFOUND:
            x_sprintf(ErrorString,"No memory card (8MB)(for PlayStation""\x10""2)\n"
								  "was found. You will not be able to \n"
								  "save your game unless you have a\n"
								  "memory card (8MB)(for PlayStation""\x10""2)\n"
								  "with %dKB of space available.",SAVE_FILE_SIZE / 1024);
            break;
        case TS_ERR_UNFORMATTED:
            x_sprintf(ErrorString,"The memory card (8MB)(for PlayStation""\x10""2)\n"
								  "in MEMORY CARD slot %d is not formatted.\n"
                                  "You will need to format it before use.",s_LastErrorSlot+1);
            break;
        case TS_ERR_FULL:
            x_sprintf(ErrorString,"The memory card (8MB)(for PlayStation""\x10""2)\n"
								  "in MEMORY CARD slot %d is full.\n"
								  "%dKB of space is needed. You\n"
								  "will need to have a memory card (8MB)\n"
								  "(for PlayStation""\x10""2) with sufficient space\n"
								  "available to save.",s_LastErrorSlot+1,SAVE_FILE_SIZE / 1024);
            break;
        case TS_ERR_CORRUPT:
            x_sprintf(ErrorString,"The saved game on memory card (8MB)\n"
                                  "(for PlayStation""\x10""2) in MEMORY\n"
								  "CARD slot %d appears to be damaged.\n"
								  "It cannot be used.",s_LastErrorSlot+1);
            break;
        case TS_ERR_ATTACH_FAILED:
            x_sprintf(ErrorString,"An error occurred while connecting.\n"
                                  "Please check Your Network Configuration\n"
                                  "file with the MODIFY button on the\n"
								  "Network Settings main menu option.\n");
            break;
        case TS_ERR_ATTACH_ABORTED:
            pHeader = "CANCELLED!";
             x_sprintf(ErrorString,"Network connect aborted.\n"
								   "Please try again. If you still have\n"
								   "trouble connecting, please check\n"
								   "Your Network Configuration file by\n"
								   "selecting the MODIFY button on the\n"
								   "Network Settings main menu option.\n");
            break;
        case TS_ERR_ATTACH_TIMEOUT:
             x_sprintf(ErrorString,"Timed out while attempting to connect.\n"
								   "Please try again. If you still have\n"
								   "trouble connecting, please check\n"
								   "Your Network Configuration file by\n"
								   "selecting the MODIFY button on the\n"
								   "Network Settings main menu option.\n");
            break;
        case TS_ERR_NO_NET_CONFIG:
            x_sprintf(ErrorString,"Auto-connect was specified but there\n"
                                  "is no Network Configuration file\n"
                                  "available. Please create Your Network\n"
                                  "file using the MODIFY button on the\n"
								  "Network Settings main menu option.");
            break;
		case TS_ERR_INVALID_NET_CONFIG:
			x_sprintf(ErrorString,"The supported network hardware is not\n"
								  "correctly connected to your PlayStation""\x10""2\n"
								  "console. Please turn off your console\n"
								  "and check your hardware connection. If\n"
								  "you continue to have trouble, please use\n"
								  "the MODIFY button on the Network Settings\n"
								  "main menu option to re-configure and\n"
								  "check Your Network Configuration file.");
			break;
        case TS_ERR_BAD_NET_CONFIG:
            x_sprintf(ErrorString,"The Network Configuration file on\n"
                                  "the memory card (8MB)(for PlayStation""\x10""2)\n"
								  "in MEMORY CARD slot %d is for another\n"
								  "PlayStation""\x10""2 console and cannot\n"
								  "be used. Please use the MODIFY button on\n"
								  "the Network Settings main menu option to\n"
								  "configure Your Network Configuration file.",s_LastErrorSlot+1);
            break;
        case TS_ERR_ATTACH_OK:
            char temp[64];
			interface_info info;

			net_GetInterfaceInfo(-1,info);
            net_IPToStr(info.Address,temp);
            x_sprintf(ErrorString,"Network connection successfully\n"
                                  "established. Your network address\n"
                                  "is %s.\n",temp);
			connect_status status;

			net_GetConnectStatus(status);
			if (status.ConnectSpeed > 1)
			{
				x_strcat(ErrorString,xfs("Connected at %2.1fK.",(f32)status.ConnectSpeed/1000.0f));
				fegl.ModemConnection = TRUE;
			}
			else
			{
				fegl.ModemConnection = (status.ConnectSpeed!=0);
			}
            pHeader = "SUCCESS!";
            soundeffect = SFX_MISC_RESPAWN;
            MessageDuration = 8.0f;
            break;
        case TS_ERR_UNKNOWN:
            x_sprintf(ErrorString,"An unknown error occurred during\n"
                                  "the initial MEMORY CARD(PS2) checks.");
            break;
        case TS_ERR_WRONGTYPE:
            x_sprintf(ErrorString,"The memory card in MEMORY CARD slot\n"
								"%d is not a memory card(8MB)(for\n"
								"PlayStation""\x10""2).\n"
								"This memory card cannot be used\n"
								"with this game. Please insert a\n"
								"memory card (8MB)(for PlayStation""\x10""2).",s_LastErrorSlot+1);
            break;
        default:
            ASSERT(FALSE);
    }

    if (message)
    {
        dlg_message *   pMessage;
        tgl.pUIManager->EnableUser( fegl.UIUserID, TRUE );
        pMessage = (dlg_message *)tgl.pUIManager->OpenDialog(fegl.UIUserID,
                                    "message",
                                    irect(0,0,320,200),
                                    NULL,
                                    ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER);
        pMessage->Configure( xwstring(pHeader), 
                               NULL,
                               xwstring("DONE"),
                               xwstring(ErrorString), 
                               XCOLOR_WHITE,
                               NULL
                               );
        pMessage->SetTimeout(MessageDuration);
        audio_Play(soundeffect, AUDFLAG_CHANNELSAVER);
    }

    sm_SetState(STATE_COMMON_INIT_FRONTEND);
}

//=================================================================================================
void title_InitialCheck( void )
{
    s32     cardnum;
    xtimer  t;
    s32     error;
	s32		LoadOptions;

    s_LastErrorSlot = -1;
    s_LastError = TS_ERR_NOTFOUND;
    t.Reset();
    t.Start();
	LoadOptions = TRUE;

    for (cardnum=0;cardnum<2;cardnum++)
    {
        error = title_MemcardCheck(cardnum,LoadOptions);

        switch(error)
        {
        case 0:
            if (s_LastError != TS_ERR_OK)
            {
                s_LastError = TS_ERR_OK;
                s_LastErrorSlot = cardnum;
            }
            break;
        case CARD_ERR_NOTFOUND:
        case CARD_ERR_UNPLUGGED:
            if ( s_LastErrorSlot < 0 )
            {
                s_LastError = TS_ERR_NOTFOUND;
                s_LastErrorSlot = cardnum;
            }
            break;
        case CARD_ERR_UNFORMATTED:
            if ( s_LastError == TS_ERR_NOTFOUND )
            {
                s_LastError = TS_ERR_UNFORMATTED;
                s_LastErrorSlot = cardnum;
            }
            break;
        case CARD_ERR_FULL:
            if ( s_LastError == TS_ERR_NOTFOUND  )
            {
                s_LastError = TS_ERR_FULL;
                s_LastErrorSlot = cardnum;
            }
            break;
        case CARD_ERR_CORRUPT:
            if ( s_LastError == TS_ERR_NOTFOUND )
            {
                s_LastError = TS_ERR_CORRUPT;
                s_LastErrorSlot = cardnum;
            }
            break;
        case CARD_ERR_WRONGTYPE:
            if ( s_LastError == TS_ERR_NOTFOUND )
            {
                s_LastError = TS_ERR_WRONGTYPE;
                s_LastErrorSlot = cardnum;
            }
            break;
        default:
            s_LastError = TS_ERR_UNKNOWN;
            s_LastErrorSlot = cardnum;
            break;
        }
    }

    // If any error occurs, the fegl.AutoConnect would never get set anyway.
    interface_info info;
    net_GetInterfaceInfo(-1,info);
    if (fegl.AutoConnect && (info.Address == 0))
    {
        char path[64];
        s32 status;
        xtimer t;
		net_config_list ConfigList;

        x_sprintf(path,"mc%d:BWNETCNF/BWNETCNF",s_LastErrorSlot);
        t.Reset();
        t.Start();
		status = net_GetConfigList(path,&ConfigList);
		if ( (status < 0) || (ConfigList.Count <= 0) )
		{
	        x_sprintf(path,"mc%d:BWNETCNF/BWNETCNF",(s_LastErrorSlot+1)&1);
			status = net_GetConfigList(path,&ConfigList);
			if ( (status < 0) || (ConfigList.Count <= 0) )
			{
		        x_sprintf(path,"mc%d:BWNETCNF/BWNETCNF",s_LastErrorSlot);
			}
		}

        status = net_SetConfiguration(path,fegl.LastNetConfig);
		x_DebugMsg("SetConfiguration returned status %d\n",status);

        t.Stop();
        if (status >=0 )
        {
            t.Reset();
            t.Start();
            s_LastError = TS_ERR_ATTACH_FAILED;
            while (t.ReadSec() < 5.0f)
            {
                error = 0;
                error = net_GetAttachStatus(error);
                if ( (error==ATTACH_STATUS_CONFIGURED) ||
                     (error==ATTACH_STATUS_ATTACHED) )
                {
                    t.Reset();
                    t.Start();
                    net_ActivateConfig(TRUE);
                    t.Stop();
                    s_LastError = TS_ERR_OK;
                    break;
                }
                else
                {
                    s_LastError = TS_ERR_BAD_NET_CONFIG;
                    fegl.AutoConnect = FALSE;
                }
            }
        }
        else
        {
			if (status==-2)	// Error -2, configuration if for another system
            {
                s_LastError = TS_ERR_BAD_NET_CONFIG;
            }
			else if (status==-3) // Error -3, no hardware available
			{
				s_LastError = TS_ERR_INVALID_NET_CONFIG;
			}
            else			// Error -1, no network configurations
            {
                s_LastError = TS_ERR_NO_NET_CONFIG;
            }
            fegl.AutoConnect = FALSE;
        }
    }
}

