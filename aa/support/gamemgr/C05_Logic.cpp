//==============================================================================
//
//  C05_Logic.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "C05_Logic.hpp"
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

c05_logic   C05_Logic;

//==============================================================================
//  FUNCTIONS
//==============================================================================

void c05_logic::AdvanceTime( f32 DeltaTime )
{
    ctf_logic::AdvanceTime( DeltaTime );

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
        if( GameMgr.m_Score.Team[0].Score >= 600 )
        {
            Victorious = TRUE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M05_SUCCESS );
            tgl.MissionSuccess = TRUE;
        }

        if( (GameMgr.m_TimeRemaining <= 0.0f) || 
            (GameMgr.m_Score.Team[1].Score >= 600) )
        {
            Victorious = FALSE;
            GameOver   = TRUE;
            DelayTime  = 2.0f;
            ChannelID  = audio_Play( M05_FAIL );
            tgl.MissionSuccess = FALSE;
        }

        if( GameOver )
        {
            GameMgr.m_TimeLimit = 0;
        }
    }
}

//==============================================================================

void c05_logic::BeginGame( void )
{
    Victorious = FALSE;
    GameOver   = FALSE;

    GameMgr.SetPlayerLimits( 32, 6, TRUE );
    GameMgr.SetVehicleLimits( 2, 2, 0, 0 );

    audio_Play( M05_INTRO );

    tgl.MissionSuccess = FALSE;

    ctf_logic::BeginGame();
    campaign_logic::SetCampaignObjectives( 25 );
}

//==============================================================================
