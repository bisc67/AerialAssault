//==============================================================================
//
//  C01_Logic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "C01_Logic.hpp"
#include "Campaign_Logic.hpp"
#include "GameMgr.hpp"

#include "Objects\Player\PlayerObject.hpp"
#include "Objects\Projectiles\Flag.hpp"
#include "Demo1\Globals.hpp"
#include "Demo1\fe_Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"

//==============================================================================
//  DEFINES
//==============================================================================


//==============================================================================
//  STORAGE
//==============================================================================

c01_logic   C01_Logic;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void c01_logic::AdvanceTime( f32 DeltaTime )
{
    if( GameOver )
    {
        if( audio_IsPlaying( ChannelID ) == FALSE )
        {
            DelayTime -= DeltaTime;
            if( DelayTime <= 0.0f )
            {
                GameMgr.EndGame();
            }
        }
    }
    else
    {
        if( GameMgr.m_Score.Player[0].Score >= 15 )
        {
            Victorious = TRUE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M01_SUCCESS );
            tgl.MissionSuccess = TRUE;
        }

        if( GameMgr.m_TimeRemaining <= 0.0f )
        {
            Victorious = FALSE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M01_FAIL );
            tgl.MissionSuccess = FALSE;
        }

        if( GameOver )
        {
            GameMgr.m_TimeLimit = 0;
        }
    }
}

//==============================================================================

void c01_logic::BeginGame( void )
{
    Victorious = FALSE;
    GameOver   = FALSE;

    GameMgr.SetPlayerLimits( 32, 6, TRUE );

    audio_Play( M01_INTRO );
    tgl.MissionSuccess = FALSE;

    dm_logic::BeginGame();
    campaign_logic::SetCampaignObjectives( 21 );
}

//==============================================================================
