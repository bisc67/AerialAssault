//==============================================================================
//
//  C04_Logic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "C04_Logic.hpp"
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

c04_logic   C04_Logic;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void c04_logic::AdvanceTime( f32 DeltaTime )
{
    cnh_logic::AdvanceTime( DeltaTime );

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
        if( GameMgr.m_Score.Team[0].Score >= 1000 )
        {
            Victorious = TRUE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M04_SUCCESS );
            tgl.MissionSuccess = TRUE;
        }

        if( (GameMgr.m_TimeRemaining <= 0.0f) || 
            (GameMgr.m_Score.Team[1].Score >= 1000) )
        {
            Victorious = FALSE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M04_FAIL );
            tgl.MissionSuccess = FALSE;
        }

        if( GameOver )
        {
            GameMgr.m_TimeLimit = 0;
        }
    }
}

//==============================================================================

void c04_logic::BeginGame( void )
{
    Victorious = FALSE;
    GameOver   = FALSE;

    GameMgr.SetPlayerLimits( 32, 6, TRUE );

    audio_Play( M04_INTRO );

    tgl.MissionSuccess = FALSE;

    cnh_logic::BeginGame();
    campaign_logic::SetCampaignObjectives( 24 );
}

//==============================================================================
