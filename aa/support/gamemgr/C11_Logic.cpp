//========================================================================================================================================================
//
//  Campaign 11 by JP
//
//========================================================================================================================================================

#include "c11_logic.hpp"
#include "LabelSets\Tribes2Types.hpp"

//========================================================================================================================================================
//  TYPES
//========================================================================================================================================================

struct c11_data
{
    s32     BASE_RADIUS;
    s32     GENERATOR_RADIUS;
    s32     GENERATOR_TIME;
    s32     MIN_ENEMY_COUNT_WAVE1;
    s32     MIN_ENEMY_COUNT_WAVE2;
    s32     WAVE1_ATTACK_TIME;
    s32     WAVE2_ATTACK_TIME;
    s32     WAVE3_ATTACK_TIME;
};

//========================================================================================================================================================
//  VARIABLES
//========================================================================================================================================================

c11_data  C11_Data = { 100, 100, 10, 1, 5, 90, 45, 45 };

c11_logic C11_Logic;

//========================================================================================================================================================

void c11_logic::BeginGame( void )
{
    ASSERT( m_Generator1 != obj_mgr::NullID );
    ASSERT( m_Generator2 != obj_mgr::NullID );

    m_TargetGenerator = m_Generator1;

    campaign_logic::BeginGame();
    
    SetState( STATE_PREPARATION );
    SetCampaignObjectives( 30 );
}

//========================================================================================================================================================

void c11_logic::EndGame( void )
{
}

//========================================================================================================================================================

void c11_logic::AdvanceTime( f32 DeltaTime )
{
    campaign_logic::AdvanceTime( DeltaTime );
    
    switch( m_GameState )
    {
        case STATE_IDLE        : return;
        case STATE_PREPARATION : Preparation(); break;
        case STATE_FIGHT       : Fight();       break;
        default                :                break;
    }

    if( m_bPlayerDied == TRUE )
    {
        Failed( M11_FAIL_GENS_LOST );
    }

    //
    // check for both generators going off
    //
    
    if( m_bGenerator1Disabled && m_bGenerator2Disabled )
    {
        Failed( M11_FAIL );
    }

    m_Time  += DeltaTime;
    m_Time2 += DeltaTime;
}

//========================================================================================================================================================

void c11_logic::RegisterItem( object::id ItemID, const char* pTag )
{
    if( AddSpawnPoint( ItemID, pTag )) return;
    
    if( !x_strcmp( pTag, "GEN1" )) m_Generator1 = ItemID;
    if( !x_strcmp( pTag, "GEN2" )) m_Generator2 = ItemID;
}

//========================================================================================================================================================

void c11_logic::ItemDisabled( object::id ItemID,  object::id OriginID )
{
    (void)OriginID;
    
    if( ItemID == m_Generator1 )
    {
        m_bGenerator1Disabled = TRUE;
        m_bGenerator1Warning  = TRUE;
        m_TargetGenerator     = m_Generator2;
    }
    
    if( ItemID == m_Generator2 )
    {
        m_bGenerator2Disabled = TRUE;
        m_bGenerator2Warning  = TRUE;
        m_TargetGenerator     = m_Generator1;
    }

    if( m_GameState == STATE_FIGHT )
    {
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
    }
}

//========================================================================================================================================================

void c11_logic::ItemEnabled( object::id ItemID,  object::id OriginID )
{
    (void)OriginID;

    if( ItemID == m_Generator1 )
    {
        m_bGenerator1Online   = TRUE;
        m_bGenerator1Disabled = FALSE;
//      m_TargetGenerator     = m_Generator1;
    }
    
    if( ItemID == m_Generator2 )
    {
        m_bGenerator2Online   = TRUE;
        m_bGenerator2Disabled = FALSE;
//      m_TargetGenerator     = m_Generator2;
    }
}

//========================================================================================================================================================
    
void c11_logic::Preparation( void )
{
    c11_data& D = C11_Data;

    //
    // initialize Preparation state
    //

    if( !m_bInitialized )
    {
        m_bInitialized         = TRUE;
        m_bWarning1            = FALSE;
        m_bWarning2            = FALSE;
        m_bGenerator1Disabled  = FALSE;
        m_bGenerator2Disabled  = FALSE;
        m_bGenerator1Warning   = FALSE;
        m_bGenerator2Warning   = FALSE;
        m_bGenerator1Online    = FALSE;
        m_bGenerator2Online    = FALSE;
        m_bAttackWave1         = FALSE;
        m_Time2                = 0;

        SpawnAllies();
        OrderAllies( TASK_PATROL, m_PlayerID );

        GamePrompt( M11_INTRO );
    }

    //
    // time to spawn the first wave?
    //
    
    if( !m_bAttackWave1 && ( m_Time > D.WAVE1_ATTACK_TIME ))
    {
        m_bAttackWave1 = TRUE;
        
        SpawnEnemies( ATTACK_WAVE_1 );
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        
        GamePrompt( M11_ENEMY_COMING );
    }

    //
    // check if enemy has reached our base
    //

    if( DistanceToClosestPlayer( m_Generator1, TEAM_ENEMY ) < D.BASE_RADIUS )
    {
        SetState( STATE_FIGHT );
    }
    
    //
    // constrain player to generator area
    //

    StayNearGens();
}

//========================================================================================================================================================

void c11_logic::Fight( void )
{
    c11_data& D = C11_Data;

    //
    // initialize Fight state
    //

    if( !m_bInitialized )
    {
        m_bInitialized  = TRUE;
        m_bWave2Warning = FALSE;
        m_bWave3Warning = FALSE;
        m_bAttackWave2  = FALSE;
        m_bAttackWave3  = FALSE;

        GamePrompt( M11_BATTLE_STATIONS );
    }

    //
    // check for generators going down
    //

    if( m_bGenerator1Warning )
    {
        m_bGenerator1Warning = FALSE;
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        if( !(m_bGenerator1Disabled && m_bGenerator2Disabled) )
            GamePrompt( M11_GEN_LOST );
    }

    if( m_bGenerator2Warning )
    {
        m_bGenerator2Warning = FALSE;
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        if( !(m_bGenerator1Disabled && m_bGenerator2Disabled) )
            GamePrompt( M11_GEN_LOST );
    }
    
    //
    // check for generators coming back online
    //
    
    if( m_bGenerator1Online )
    {
        m_bGenerator1Online = FALSE;
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        GamePrompt( M11_GEN_ONLINE );
    }

    if( m_bGenerator2Online )
    {
        m_bGenerator2Online = FALSE;
        OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        GamePrompt( M11_GEN_ONLINE );
    }

    //
    // attack wave 2
    //

    if( !m_bWave2Warning && m_nEnemies <= D.MIN_ENEMY_COUNT_WAVE1 )
    {
        m_bWave2Warning = TRUE;
        GamePrompt( M11_MORE_ENEMY );
        m_Time = 0;
    }
    else
    {
        if( !m_bAttackWave2 && m_bWave2Warning && ( m_Time > D.WAVE2_ATTACK_TIME ))
        {
            m_bAttackWave2 = TRUE;
            SpawnEnemies( ATTACK_WAVE_2 );
            OrderEnemies( TASK_DESTROY, m_TargetGenerator );
        }
    }
    
    //
    // attack wave 3
    //

    if( m_bAttackWave2 )
    {
        if( !m_bWave3Warning && ( m_nEnemies <= D.MIN_ENEMY_COUNT_WAVE2 ))
        {
            m_bWave3Warning = TRUE;
            GamePrompt( M11_EVEN_MORE_ENEMY );
            m_Time = 0;
        }
        else
        {
            if( !m_bAttackWave3 && m_bWave3Warning && ( m_Time > D.WAVE3_ATTACK_TIME ))
            {
                m_bAttackWave3 = TRUE;
            
                SpawnEnemies( ATTACK_WAVE_3 );
                OrderEnemies( TASK_DESTROY, m_TargetGenerator );
            }
        }
    }

    //
    // constrain player to generator area
    //

    StayNearGens();

    //
    // wait until all enemies dead
    //
    
    if( m_bAttackWave3 && ( m_nEnemies == 0 ))
    {
        Success( M11_SUCCESS );
    }
}

//========================================================================================================================================================

void c11_logic::StayNearGens( void )
{
    c11_data& D = C11_Data;

    if(( DistanceToObject( m_Generator1 ) > D.GENERATOR_RADIUS ) ||
       ( DistanceToObject( m_Generator2 ) > D.GENERATOR_RADIUS ))
    {
        if( !m_bWarning1 && m_Time2 > D.GENERATOR_TIME )
        {
            m_bWarning1 = TRUE;
            m_Time2     = 0;
            
            GamePrompt( M11_GO_TO_GEN );
        }
        
        if( !m_bWarning2 && m_Time2 > D.GENERATOR_TIME )
        {
            m_bWarning2 = TRUE;
            m_Time2     = 0;
        
            GamePrompt( M11_STAY_AT_GEN );
        }
        
        if( m_Time2 > D.GENERATOR_TIME )
        {
            Failed( M11_FAIL );
        }
    }
    else
    {
        m_Time2 = 0;
    }
}

//========================================================================================================================================================

