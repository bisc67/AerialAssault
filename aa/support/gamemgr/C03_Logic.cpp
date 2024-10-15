//==============================================================================
//
//  C03_Logic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "C03_Logic.hpp"
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

c03_logic   C03_Logic;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void c03_logic::AdvanceTime( f32 DeltaTime )
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
        if( GameMgr.m_Score.Player[0].Score >= 250 )
        {
            Victorious = TRUE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M03_SUCCESS );
            tgl.MissionSuccess = TRUE;
        }

        if( GameMgr.m_TimeRemaining <= 0.0f )
        {
            Victorious = FALSE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M03_FAIL );
            tgl.MissionSuccess = FALSE;
        }

        if( GameOver )
        {
            GameMgr.m_TimeLimit = 0;
        }
    }
}

//==============================================================================

void c03_logic::BeginGame( void )
{
    Victorious = FALSE;
    GameOver   = FALSE;

    GameMgr.SetPlayerLimits( 32, 6, TRUE );

    audio_Play( M03_INTRO );

    tgl.MissionSuccess = FALSE;

    hunt_logic::BeginGame();
    campaign_logic::SetCampaignObjectives( 23 );
}

//==============================================================================
