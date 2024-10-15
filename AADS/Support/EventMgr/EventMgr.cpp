//==============================================================================
//
//  EventMgr.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "EventMgr.hpp"
#include "GameMgr\GameMgr.hpp"

//==============================================================================
//  STORAGE
//==============================================================================

event_mgr EventMgr;             // Looks rather innocent, eh?  Don't be fooled!

//------------------------------------------------------------------------------

xbool   event_mgr::m_Initialized = FALSE;

//==============================================================================
//  FUNCTIONS
//==============================================================================

event_mgr::event_mgr( void )
{
    ASSERT( !m_Initialized );
    m_Initialized = TRUE;
}

//==============================================================================

event_mgr::~event_mgr( void )
{
    ASSERT( m_Initialized );
    m_Initialized = FALSE;
}

//==============================================================================
//==============================================================================

void event::EncodeFlagTouched( object::id FlagID, object::id PlayerID )
{                                                                    
    m_Code  = FLAG_TOUCHED;
    m_ID[0] = FlagID;
    m_ID[1] = PlayerID;
}

//==============================================================================

void event::DecodeFlagTouched( object::id& FlagID, object::id& PlayerID )
{
    ASSERT( m_Code == FLAG_TOUCHED );
    FlagID   = m_ID[0];
    PlayerID = m_ID[1];
}

//==============================================================================

void event_mgr::FlagTouched( object::id FlagID, object::id PlayerID )
{
    event Event;
    Event.EncodeFlagTouched( FlagID, PlayerID );
//  GameMgr.FlagTouched( FlagID, PlayerID );
}

//==============================================================================
