//==============================================================================
//
//  PlayerNet.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "NetLib/BitStream.hpp"
#include "AADS/Globals.hpp"
#include "AADS/MoveManager.hpp"
#include "AADS/GameClient.hpp"
#include "Objects/Vehicles/GndEffect.hpp"
#include "AudioMgr/Audio.hpp"
#include "LabelSets/Tribes2Types.hpp"

//==============================================================================
// DEFINES
//==============================================================================


//#define PLAYER_TIMERS

// Blending defines
xbool   player_object::PLAYER_CLIENT_BLENDING                       = TRUE ;
f32     player_object::PLAYER_CLIENT_BLEND_TIME_SCALER              = 1.5f ;    // The bigger, the smoother, but the more out of date!
f32     player_object::PLAYER_CLIENT_BLEND_TIME_MAX                 = 0.5f ;    // Max secs to blend to server pos

xbool   player_object::PLAYER_SERVER_BLENDING                       = TRUE ;
f32     player_object::PLAYER_SERVER_BLEND_TIME_SCALER              = 1.5f ;    // The bigger, the smoother, but the more out of date!
f32     player_object::PLAYER_SERVER_BLEND_TIME_MAX                 = 0.5f ;    // Max secs to blend to server pos

f32     player_object::PLAYER_BLEND_DIST_MAX                        = 20.0f ;   // Max dist before snapping to server pos
                                                                    
// Max values                                                       
f32     player_object::PLAYER_VEL_MAX                               = 165.0f ;
f32     player_object::PLAYER_ROT_MAX                               = R_180 ;
f32     player_object::PLAYER_ENERGY_MAX                            = 1.0f ;
f32     player_object::PLAYER_HEALTH_MAX                            = 100 ;
                                                        
// Bits to use when sending to an input player machine  
s32     player_object::INPUT_PLAYER_VEL_BITS                        = 16 ;
s32     player_object::INPUT_PLAYER_ROT_BITS                        = 16 ;
s32     player_object::INPUT_PLAYER_ENERGY_BITS                     = 18 ;
s32     player_object::INPUT_PLAYER_HEALTH_BITS                     = 7 ;
s32     player_object::PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_BITS   = 8 ;
s32     player_object::PLAYER_LAND_RECOVER_TIME_BITS                = 8 ;

// Bits to use when sending to a ghost player machine
s32     player_object::GHOST_PLAYER_COMP_POS_BITS                   = 16 ;      // 8bit exp, 8bit mantissa
s32     player_object::GHOST_PLAYER_POS_BITS                        = 16 ;      // 1bit sign, 7bits frac, 8bits int
f32     player_object::GHOST_PLAYER_POS_PRECISION                   = 0.01f ;   // 0.01=7bits
s32     player_object::GHOST_PLAYER_VEL_BITS                        = 16 ;      // Per component (any less makes it jerky)
s32     player_object::GHOST_PLAYER_ROT_BITS                        = 6 ;
s32     player_object::GHOST_PLAYER_ENERGY_BITS                     = 6 ;
s32     player_object::GHOST_PLAYER_HEALTH_BITS                     = 6 ;

// Prediction vars
f32     player_object::PLAYER_TIME_STEP_MIN                         = 0.005f ;
f32     player_object::GHOST_PLAYER_LATENCY_TIME_MAX                = 0.5f ;
f32     player_object::GHOST_PLAYER_LATENCY_TIME_STEP               = 1.0f / 30.0f ;
f32     player_object::GHOST_PLAYER_PREDICT_TIME_MAX                = 0.5f ;
                                                                    
// misc                                                             
xbool   player_object::PLAYER_NO_PRIORITY                           = TRUE;
xbool   player_object::SHOW_NET_STATS                               = FALSE;


//=========================================================================
// player_object::inventory FUNCTIONS
//=========================================================================

// Functions
void player_object::inventory::Clear ( void )
{
    // Clear
    x_memset((void*)this, 0, sizeof(player_object::inventory)) ;
}

// Net functions

//=========================================================================

// Writes complete inventory
void player_object::inventory::Write (       bitstream& BitStream ) const
{
    s32 Count, i, Total, TotalCursor, EndCursor ;

    // Write out dummy total for now
    Total       = 0 ;
    TotalCursor = BitStream.GetCursor() ;
    BitStream.WriteRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;

    // Write out all inventory counts
    for (i = INVENT_TYPE_START ; i <= INVENT_TYPE_END ; i++)
    {
        // Any inventory for this object?
        Count = Counts[i] ;
        
        // Make sure it's valid!
        if (Count > s_InventInfo[i].MaxAmount)
            Count = s_InventInfo[i].MaxAmount ;

        // Any?
        if (Count > 0)
        {
            BitStream.WriteRangedS32(i, INVENT_TYPE_START, INVENT_TYPE_END) ;
            BitStream.WriteRangedS32(Count, 0, s_InventInfo[i].MaxAmount) ;
            Total++ ;
        }
    }

    // Write out total
    EndCursor = BitStream.GetCursor() ;
    BitStream.SetCursor(TotalCursor) ;
    BitStream.WriteRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;
    BitStream.SetCursor(EndCursor) ;
}

// Reads complete inventory
void player_object::inventory::Read  ( const bitstream& BitStream )
{
    s32 i, Total, Index ;

    // Reset all counts to zero
    Clear() ;

    // Inventory counts
    BitStream.ReadRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;
    for (i = 0 ; i < Total ; i++)
    {
        BitStream.ReadRangedS32(Index, INVENT_TYPE_START, INVENT_TYPE_END) ;
        BitStream.ReadRangedS32(Counts[Index], 0, s_InventInfo[Index].MaxAmount) ;
    }
}

//=========================================================================

// Only writes what the client needs updating
void player_object::inventory::ProvideUpdate ( bitstream& BitStream, inventory& ClientInventory ) const
{
    s32 Count, i, Total, TotalCursor, EndCursor ;

    // Write out dummy total for now
    Total       = 0 ;
    TotalCursor = BitStream.GetCursor() ;
    BitStream.WriteRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;

    // Only need to write out what the client needs to update
    for (i = INVENT_TYPE_START ; i <= INVENT_TYPE_END ; i++)
    {
        // Any inventory for this object?
        Count = Counts[i] ;
        
        // Make sure it's valid!
        if (Count > s_InventInfo[i].MaxAmount)
            Count = s_InventInfo[i].MaxAmount ;

        // Update the client?
        if (Count != ClientInventory.Counts[i])
        {
            // Send item
            BitStream.WriteRangedS32(i, INVENT_TYPE_START, INVENT_TYPE_END) ;
            BitStream.WriteRangedS32(Count, 0, s_InventInfo[i].MaxAmount) ;
            Total++ ;

            // Flag client item is up-to-date
            ClientInventory.Counts[i] = Count ;
        }
    }

    // Write out total
    EndCursor = BitStream.GetCursor() ;
    BitStream.SetCursor(TotalCursor) ;
    BitStream.WriteRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;
    BitStream.SetCursor(EndCursor) ;
}

// Only reads what needs updating
void player_object::inventory::AcceptUpdate( const bitstream& BitStream )
{
    s32 i, Total, Index ;

    // Get total to update
    BitStream.ReadRangedS32(Total, 0, INVENT_TYPE_TOTAL) ;

    // Update counts
    for (i = 0 ; i < Total ; i++)
    {
        BitStream.ReadRangedS32(Index, INVENT_TYPE_START, INVENT_TYPE_END) ;
        BitStream.ReadRangedS32(Counts[Index], 0, s_InventInfo[Index].MaxAmount) ;
    }
}


//=========================================================================
// player_object::loadout FUNCTIONS
//=========================================================================


// Functions
void player_object::loadout::Clear ( void )
{
    // Clear
    x_memset((void*)this, 0, sizeof(player_object::loadout)) ;
}

// Net functions

//=========================================================================

// Writes complete loadout
void player_object::loadout::Write (       bitstream& BitStream ) const
{
    s32 i ;

    // Armor
    BitStream.WriteRangedU32(Armor, ARMOR_TYPE_START, ARMOR_TYPE_END) ;

    // Weapon slots
    BitStream.WriteRangedU32(NumWeapons, 0, 5) ;
    for (i = 0 ; i < NumWeapons ; i++)
        BitStream.WriteRangedU32((u32)Weapons[i], INVENT_TYPE_WEAPON_START, INVENT_TYPE_WEAPON_END) ;

    // Vehicle
    BitStream.WriteRangedU32(VehicleType, object::TYPE_START_OF_LIST, object::TYPE_END_OF_LIST-1) ;

    // Write out all inventory counts
    Inventory.Write(BitStream) ;
}

// Reads complete loadout
void player_object::loadout::Read  ( const bitstream& BitStream )
{
    s32 i ;
    u32 TempU32 ;

    // Clear everything
    Clear() ;

    // Armor
    BitStream.ReadRangedU32(TempU32, player_object::ARMOR_TYPE_START, player_object::ARMOR_TYPE_END) ;
    Armor = (armor_type)TempU32 ;

    // Weapon slots
    BitStream.ReadRangedU32(TempU32, 0, 5) ;
    NumWeapons = (s32)TempU32 ;
    for (i = 0 ; i < NumWeapons ; i++)
    {
        BitStream.ReadRangedU32(TempU32, player_object::INVENT_TYPE_WEAPON_START, player_object::INVENT_TYPE_WEAPON_END) ;
        Weapons[i] = (invent_type)TempU32 ;
    }

    // Vehicle
    BitStream.ReadRangedU32(TempU32, object::TYPE_START_OF_LIST, object::TYPE_END_OF_LIST-1) ;
    VehicleType = (object::type)TempU32 ;

    // Inventory counts
    Inventory.Read(BitStream) ;
}

//=========================================================================

// Only writes what the client needs updating
void player_object::loadout::ProvideUpdate ( bitstream& BitStream, loadout& ClientLoadout ) const
{
    s32     i ;
    xbool   Update ;

    // Armor?
    Update = (Armor != ClientLoadout.Armor) ;
    if (BitStream.WriteFlag(Update))
    {
        // Send item
        BitStream.WriteRangedU32(Armor, ARMOR_TYPE_START, ARMOR_TYPE_END) ;

        // Flag client item is up-to-date
        ClientLoadout.Armor = Armor ;
    }
    
    // Weapon slots?
    Update = (NumWeapons != ClientLoadout.NumWeapons) ;
    for (i = 0 ; i < NumWeapons ; i++)
        Update |= Weapons[i] != ClientLoadout.Weapons[i] ;
    if (BitStream.WriteFlag(Update))
    {
        // Send item
        BitStream.WriteRangedU32(NumWeapons, 0, 5) ;

        // Flag client item is up-to-date
        ClientLoadout.NumWeapons = NumWeapons ;

        // Send weapons
        for (i = 0 ; i < NumWeapons ; i++)
        {
            // Send item
            BitStream.WriteRangedU32((u32)Weapons[i], INVENT_TYPE_WEAPON_START, INVENT_TYPE_WEAPON_END) ;

            // Flag client item is up-to-date
            ClientLoadout.Weapons[i] = Weapons[i] ;
        }
    }

    // Vehicle?
    Update = (VehicleType != ClientLoadout.VehicleType) ;
    if (BitStream.WriteFlag(Update))
    {
        // Send item
        BitStream.WriteRangedU32(VehicleType, object::TYPE_START_OF_LIST, object::TYPE_END_OF_LIST-1) ;

        // Flag client item is up-to-date
        ClientLoadout.VehicleType = VehicleType ;
    }

    // Inventory counts
    Inventory.ProvideUpdate(BitStream, ClientLoadout.Inventory) ;
}

// Only reads what needs updating
void player_object::loadout::AcceptUpdate ( const bitstream& BitStream )
{
    s32 i ;
    u32 TempU32 ;

    // Armor
    if (BitStream.ReadFlag())
    {
        BitStream.ReadRangedU32(TempU32, player_object::ARMOR_TYPE_START, player_object::ARMOR_TYPE_END) ;
        Armor = (armor_type)TempU32 ;
    }

    // Weapon slots
    if (BitStream.ReadFlag())
    {
        BitStream.ReadRangedU32(TempU32, 0, 5) ;
        NumWeapons = (s32)TempU32 ;
        for (i = 0 ; i < NumWeapons ; i++)
        {
            BitStream.ReadRangedU32(TempU32, player_object::INVENT_TYPE_WEAPON_START, player_object::INVENT_TYPE_WEAPON_END) ;
            Weapons[i] = (invent_type)TempU32 ;
        }
    }

    // Vehicle
    if (BitStream.ReadFlag())
    {
        BitStream.ReadRangedU32(TempU32, object::TYPE_START_OF_LIST, object::TYPE_END_OF_LIST-1) ;
        VehicleType = (object::type)TempU32 ;
    }

    // Inventory counts
    Inventory.AcceptUpdate(BitStream) ;
}

//=========================================================================




//=========================================================================
// player_object::move FUNCTIONS
//=========================================================================

void player_object::move::Clear()
{
    // Set to zero
    x_memset((void*)this, 0, sizeof(player_object::move)) ;
}

//=========================================================================

#define MOVE_BITS               4
#define LOOK_BITS               8

// Util functions to write a f32 that has the range -1 to 1
void player_object::move::TruncateF32( f32& Value, s32 Bits )
{
    #ifndef PLAYER_DEBUG_NO_TRUNCATE

        f32 Scalar ;
        u32 IntValue ;

        // Needs at least one bit for the sign!
        ASSERT(Bits > 1) ;
        ASSERT((Value >= -1) && (Value <= 1)) ;

        // Only truncate if non zero
        if (Value != 0)
        {
            Value   += 1.0f;
            Scalar   = (f32)((1<<Bits)-1) ;
            IntValue = (u32)((Value * Scalar * 0.5f)+0.5f) ;

            Value    = ((f32)IntValue * (2.0f / Scalar)) - 1.0f ;
        }
    #endif

    ASSERT((Value >= -1) && (Value <= 1)) ;
}

//=========================================================================

void player_object::move::WriteF32( bitstream& BitStream, f32 Value, s32 Bits )
{
    #ifndef PLAYER_DEBUG_NO_TRUNCATE
        f32 Scalar ;
        u32 IntValue ;

        // Needs at least one bit for the sign!
        ASSERT(Bits > 1) ;
        ASSERT((Value >= -1) && (Value <= 1)) ;

        // Only write if non zero
        if (BitStream.WriteFlag(Value != 0))
        {
            Value   += 1.0f;
            Scalar   = (f32)((1<<Bits)-1) ;
            IntValue = (u32)((Value * Scalar * 0.5f)+0.5f) ;
            BitStream.WriteU32(IntValue, Bits) ;
        }
    #else
        BitStream.WriteF32(Value) ;
    #endif
}

//=========================================================================

void player_object::move::ReadF32( const bitstream& BitStream, f32& Value, s32 Bits )
{
    #ifndef PLAYER_DEBUG_NO_TRUNCATE

        f32 Scalar ;
        u32 IntValue ;

        // Needs at least one bit for the sign!
        ASSERT(Bits > 1) ;

        // Only read if non zero
        if (BitStream.ReadFlag())
        {
            Scalar   = (f32)((1<<Bits)-1) ;
            BitStream.ReadU32(IntValue, Bits) ;

            Value    = ((f32)IntValue * (2.0f / Scalar)) - 1.0f ;
        }
        else
            Value = 0 ;

    #else
        BitStream.ReadF32(Value) ;
    #endif

    ASSERT((Value >= -1) && (Value <= 1)) ;
}

//=========================================================================


void player_object::move::Truncate( f32 TVRefreshRate )
{
    #ifndef PLAYER_DEBUG_NO_TRUNCATE
        
        // Truncate the move + look input
        TruncateF32(MoveX, MOVE_BITS) ;
        TruncateF32(MoveY, MOVE_BITS) ;
        TruncateF32(LookX, LOOK_BITS) ;
        TruncateF32(LookY, LOOK_BITS) ; 

        // Convert delta time to nearest delta vblank
        DeltaVBlanks = (s32)(0.5f + (DeltaTime * TVRefreshRate)) ;

        // Cap for 3 bits
        if (DeltaVBlanks > 7)
            DeltaVBlanks = 7 ;

        // Convert delta vblanks to nearest ms
        s32 MS = (s32)(0.5f + (((f32)DeltaVBlanks * 1000.0f) / TVRefreshRate)) ;

        // Convert to delta time (Sec)
        DeltaTime = (f32)MS / 1000.0f ;
    #endif
}

//=========================================================================

void player_object::move::Write ( bitstream& BitStream )
{
    // Write delta vblanks (3 bits)
    BitStream.WriteRangedU32(DeltaVBlanks, 0,7) ;

    // Write move
    WriteF32( BitStream, MoveX, MOVE_BITS ) ;
    WriteF32( BitStream, MoveY, MOVE_BITS ) ;
    
    // Write look
    WriteF32( BitStream, LookX, LOOK_BITS ) ;
    WriteF32( BitStream, LookY, LOOK_BITS ) ;

    // Write zoom factor (==0: 1 bit, !=0: 3 bits)
    if (BitStream.WriteFlag(ViewZoomFactor != 0))
        BitStream.WriteRangedU32(ViewZoomFactor,1,4) ;

    // Movement keys
    BitStream.WriteFlag( FireKey ) ;
    BitStream.WriteFlag( JetKey ) ;
    BitStream.WriteFlag( JumpKey ) ;
    BitStream.WriteFlag( FreeLookKey ) ;

    // Weapon change keys
    if (BitStream.WriteFlag( (NextWeaponKey) || (PrevWeaponKey) ))
        BitStream.WriteFlag( NextWeaponKey ) ;

    // Use/throw/drop keys
    if (BitStream.WriteFlag( (TargetingLaserKey) ||
                             (MineKey)           ||
                             (PackKey)           ||
                             (GrenadeKey)        ||
                             (HealthKitKey)      ||
                             (TargetLockKey)     ||
							 (ExchangeKey)		 ||
							 (TauntKey)		     ||
							 (ComplimentKey)	 ||
                             (DropWeaponKey)     ||
                             (DropPackKey)       ||
                             (DropBeaconKey)     ||
                             (DropFlagKey) ))
    {
        BitStream.WriteFlag( TargetingLaserKey ) ;
        BitStream.WriteFlag( MineKey ) ;
        BitStream.WriteFlag( PackKey ) ;
        BitStream.WriteFlag( GrenadeKey ) ;
        BitStream.WriteFlag( HealthKitKey ) ;
        BitStream.WriteFlag( TargetLockKey ) ;
        BitStream.WriteFlag( ExchangeKey ) ;
        BitStream.WriteFlag( TauntKey ) ;
        BitStream.WriteFlag( ComplimentKey ) ;
        BitStream.WriteFlag( DropWeaponKey ) ;
        BitStream.WriteFlag( DropPackKey ) ;
        BitStream.WriteFlag( DropBeaconKey ) ;
        BitStream.WriteFlag( DropFlagKey ) ;
    }

    // Special events that don't occur very often
    if (BitStream.WriteFlag( (SuicideKey)               ||
							 (NetSfxID)                 || 
							 (InventoryLoadoutChanged)  || 
                             (ViewChanged)              ||
							 (MonkeyMode)               ||
							 (ChangeTeam)               ||
                             (VoteType) ) )
    {
        // Suicide key?
        BitStream.WriteFlag( SuicideKey ) ;

        // Net sfx?
        if (BitStream.WriteFlag(NetSfxID != 0))
        {
            BitStream.WriteS32(NetSfxID) ;
            BitStream.WriteU32(NetSfxTeamBits) ;
            if (BitStream.WriteFlag(NetSfxTauntAnimType != 0))
                BitStream.WriteS32(NetSfxTauntAnimType) ;
        }

        // Inventory loadout?
        if (BitStream.WriteFlag( InventoryLoadoutChanged ) )
            InventoryLoadout.Write( BitStream ) ;

        // View setting change?
        if( BitStream.WriteFlag( ViewChanged ) )
        {
            BitStream.WriteFlag( View3rdPlayer  );
            BitStream.WriteFlag( View3rdVehicle );
        }

		// Team change?
		BitStream.WriteFlag( ChangeTeam ) ;

        // Monkey mode
        BitStream.WriteFlag( MonkeyMode ) ;

        // Vote type?
        if (BitStream.WriteFlag(VoteType != VOTE_TYPE_NULL))
        {
            // Send vote type
            BitStream.WriteRangedS32(VoteType, VOTE_TYPE_START, VOTE_TYPE_END) ;

            // Send approriate data (if any) with vote type
            switch(VoteType)
            {
                case VOTE_TYPE_CHANGE_MAP:
                    BitStream.WriteRangedS32(VoteData, 0, 127) ; // Max of 127 maps
                    break ;

                case VOTE_TYPE_KICK_PLAYER:
                    BitStream.WriteRangedS32(VoteData, 0, 31) ; // Max of 31 players
                    break ;
            }
        }
    }
}

//=========================================================================

void player_object::move::Read  ( const bitstream& BitStream, f32 TVRefreshRate )
{
    // Clear incase any info (eg. MonkeyMode) isn't read/written
    Clear() ;

    // Read delta vblanks (3 bits)
    BitStream.ReadRangedU32(DeltaVBlanks, 0,7) ;

    // Convert delta vblanks to nearest ms
    s32 MS = (s32)(0.5f + (((f32)DeltaVBlanks * 1000.0f) / TVRefreshRate)) ;

    // Convert to delta time (Sec)
    DeltaTime = (f32)MS / 1000.0f ;

    // Read move
    ReadF32( BitStream, MoveX, MOVE_BITS ) ;
    ReadF32( BitStream, MoveY, MOVE_BITS ) ;
    
    // Read look
    ReadF32( BitStream, LookX, LOOK_BITS ) ;
    ReadF32( BitStream, LookY, LOOK_BITS ) ;

    // Read zoom factor
    if (BitStream.ReadFlag())
        BitStream.ReadRangedU32(ViewZoomFactor,1,4) ;

    // Movement keys
    FireKey             = BitStream.ReadFlag() ; 
    JetKey              = BitStream.ReadFlag() ;  
    JumpKey             = BitStream.ReadFlag() ;  
    FreeLookKey         = BitStream.ReadFlag() ;  

    // Weapon change keys
    if (BitStream.ReadFlag())
    {
        NextWeaponKey = BitStream.ReadFlag() ;
        PrevWeaponKey = NextWeaponKey ^ 1 ;
    }
    
    // Use/throw/drop keys
    if (BitStream.ReadFlag())
    {
        TargetingLaserKey   = BitStream.ReadFlag() ;  
        MineKey             = BitStream.ReadFlag() ;  
        PackKey             = BitStream.ReadFlag() ;  
        GrenadeKey          = BitStream.ReadFlag() ;  
        HealthKitKey        = BitStream.ReadFlag() ;  
        TargetLockKey       = BitStream.ReadFlag() ;  
        ExchangeKey			= BitStream.ReadFlag() ;  
        TauntKey			= BitStream.ReadFlag() ;  
        ComplimentKey		= BitStream.ReadFlag() ;  
        DropWeaponKey       = BitStream.ReadFlag() ;  
        DropPackKey         = BitStream.ReadFlag() ;  
        DropBeaconKey       = BitStream.ReadFlag() ;  
        DropFlagKey         = BitStream.ReadFlag() ;  
    }

    // Special events that don't occur very often
    if (BitStream.ReadFlag())
    {
        // Suicide key?
        SuicideKey = BitStream.ReadFlag() ;  

        // Net sfx?
        if (BitStream.ReadFlag())
        {
            BitStream.ReadS32(NetSfxID) ;
            BitStream.ReadU32(NetSfxTeamBits) ;
            if (BitStream.ReadFlag())
                BitStream.ReadS32(NetSfxTauntAnimType) ;
        }

        // Inventory loadout?
        if(BitStream.ReadFlag())
        {
            InventoryLoadoutChanged = TRUE ;
            InventoryLoadout.Read( BitStream ) ;
        }

        if( (ViewChanged = BitStream.ReadFlag()) )
        {
            View3rdPlayer  = BitStream.ReadFlag();
            View3rdVehicle = BitStream.ReadFlag();
        }

		// Change team?
		ChangeTeam = BitStream.ReadFlag() ;

        // Monkey mode
        MonkeyMode = BitStream.ReadFlag() ;

        // Vote type?
        if (BitStream.ReadFlag())
        {
            // Read vote type
            s32 Type = VOTE_TYPE_NULL ;
            BitStream.ReadRangedS32(Type, VOTE_TYPE_START, VOTE_TYPE_END) ;
            VoteType = (s8)Type ;

            // Read approriate data (if any) with vote type
            s32 Data = 0 ;
            switch(VoteType)
            {
                case VOTE_TYPE_CHANGE_MAP:
                    BitStream.ReadRangedS32(Data, 0, 127) ; // Max of 127 maps
                    break ;

                case VOTE_TYPE_KICK_PLAYER:
                    BitStream.ReadRangedS32(Data, 0, 31) ; // Max of 31 players
                    break ;
            }
            VoteData = (s8)Data ;
        }
    }
}

// Read/write move to net bitstream (used when writing/reading from server->client)
void player_object::move::AcceptUpdate( const bitstream& BitStream )
{
    Clear() ;

    // Motion key presses
    JetKey = BitStream.ReadFlag() ;

    // Read move
    ReadF32( BitStream, MoveX, MOVE_BITS ) ;
    ReadF32( BitStream, MoveY, MOVE_BITS ) ;
}

//==============================================================================

void player_object::move::ProvideUpdate ( bitstream& BitStream )
{
    // Motion key presses
    BitStream.WriteFlag(JetKey) ;

    // Write move
    WriteF32( BitStream, MoveX, MOVE_BITS ) ;
    WriteF32( BitStream, MoveY, MOVE_BITS ) ;
}


//==============================================================================
//  GHOST PLAYER NET FUNCTIONS
//==============================================================================

// Updates ghost compression pos
void player_object::UpdateGhostCompPos( void )
{
    // Only change on the server
    if (!tgl.ServerPresent)
        return ;

    // Skip?
    if (GHOST_PLAYER_POS_BITS == 32)
        return ;

    // Get range of floats to write
    f32 Range           = (1 << (GHOST_PLAYER_POS_BITS-1)) * GHOST_PLAYER_POS_PRECISION ;
    f32 Min             = -Range ;
    f32 Max             = +Range ;
    u32 CompPosDirtyBit = PLAYER_DIRTY_BIT_GHOST_COMP_POS_X ;

    // Check all components
    for (s32 i = 0 ; i < 3 ; i++)
    {
        // Get position relate to compression position
        f32 RelPos = m_WorldPos[i] - m_GhostCompPos[i] ;
        u32 Data ;

        // Out of range?
        if ( (RelPos < Min) || (RelPos > Max) )
        {
            // Flag dirty
            m_DirtyBits |= CompPosDirtyBit ;
            
            // Update
            Data = *(u32*)&m_WorldPos[i] ;

            // Truncate
            Data >>= (32 - GHOST_PLAYER_COMP_POS_BITS) ;
            Data <<= GHOST_PLAYER_COMP_POS_BITS ;

            // Finally set new compression pos
            *(u32*)&m_GhostCompPos[i] = Data ;

            // Make sure new relative pos is valid
            RelPos = m_WorldPos[i] - m_GhostCompPos[i] ;
            ASSERT(RelPos >= Min) ;
            ASSERT(RelPos <= Max) ;

            // TEMP
            //x_printf("m_GhostCompPos[%d] updated\n", i) ;
        }

        // Next component
        CompPosDirtyBit <<= 1 ;
    }
}

//==============================================================================

void player_object::WriteGhostCompPos( bitstream& BitStream, const f32 Value )
{
    u32 Data = *(u32*)&Value ;
    Data >>= (32 - GHOST_PLAYER_COMP_POS_BITS) ;
    BitStream.WriteU32(Data, GHOST_PLAYER_COMP_POS_BITS) ;
}

//==============================================================================

void player_object::ReadGhostCompPos( const bitstream& BitStream, f32& Value )
{
    u32 Data ;
    BitStream.ReadU32(Data, GHOST_PLAYER_COMP_POS_BITS) ;
    Data <<= (32 - GHOST_PLAYER_COMP_POS_BITS) ;
    *(u32*)&Value = Data ;
}

//==============================================================================

void player_object::WriteGhostPos( bitstream& BitStream, const vector3& Value, u32& DirtyBits )
{
    // Full precision?
    if (GHOST_PLAYER_POS_BITS == 32)
    {
        BitStream.WriteVector(Value) ;
        DirtyBits &= ~PLAYER_DIRTY_BIT_GHOST_COMP_POS ;
        return ;
    }

    // Write new ghost position?
    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_GHOST_COMP_POS) != 0))
    {
        // Check all components
        u32 CompPosDirtyBit = PLAYER_DIRTY_BIT_GHOST_COMP_POS_X ;
        for (s32 i = 0 ; i < 3 ; i++)
        {
            // Update component?
            if (BitStream.WriteFlag((DirtyBits & CompPosDirtyBit) != 0))
            {
                DirtyBits &= ~CompPosDirtyBit ;
                WriteGhostCompPos( BitStream, m_GhostCompPos[i] ) ;
            }

            // Next component
            CompPosDirtyBit <<= 1 ;
        }
    }
        
    // Get range of floats to write
    f32 Range  = (1 << (GHOST_PLAYER_POS_BITS-1)) * GHOST_PLAYER_POS_PRECISION ;
    f32 Min    = -Range ;
    f32 Max    = +Range ;

    // Write all components
    for (s32 i = 0 ; i < 3 ; i++)
    {
        // Get position relate to compression position
        f32 RelPos = Value[i] - m_GhostCompPos[i] ;
        
        // Make sure the value is valid
        ASSERT(RelPos >= Min) ;
        ASSERT(RelPos <= Max) ;
    
        // Write data
        BitStream.WriteRangedF32(RelPos, GHOST_PLAYER_POS_BITS, Min, Max) ;
    }
}

//==============================================================================

void player_object::ReadGhostPos( const bitstream& BitStream, vector3& Value )
{
    // Full precision?
    if (GHOST_PLAYER_POS_BITS == 32)
    {
        BitStream.ReadVector(Value) ;
        return ;
    }

    // Read new ghost position?
    if (BitStream.ReadFlag())
    {
        // Check all components
        for (s32 i = 0 ; i < 3 ; i++)
        {
            // Update component?
            if (BitStream.ReadFlag())
                ReadGhostCompPos( BitStream, m_GhostCompPos[i] ) ;
        }
    }
        
    // Get range of floats to read
    f32 Range  = (1 << (GHOST_PLAYER_POS_BITS-1)) * GHOST_PLAYER_POS_PRECISION ;
    f32 Min    = -Range ;
    f32 Max    = +Range ;

    // Read all components
    for (s32 i = 0 ; i < 3 ; i++)
    {
        // Read rel pos
        BitStream.ReadRangedF32(Value[i], GHOST_PLAYER_POS_BITS, Min, Max) ;

        // Put back to world space
        Value[i] += m_GhostCompPos[i] ;
    }
}

//==============================================================================

void player_object::WriteGhostVel( bitstream& BitStream, const vector3& Value )
{
    WriteVel( BitStream, Value, GHOST_PLAYER_VEL_BITS ) ;
}

//==============================================================================

void player_object::ReadGhostVel( const bitstream& BitStream, vector3& Value )
{
    ReadVel( BitStream, Value, GHOST_PLAYER_VEL_BITS ) ;
}


//==============================================================================
//  INPUT PLAYER NET FUNCTIONS
//==============================================================================

// Truncate as clients will recieve player data
void player_object::Truncate()
{
    TruncateVel(m_Vel, INPUT_PLAYER_VEL_BITS) ;
    TruncateRot(m_Rot, INPUT_PLAYER_ROT_BITS) ;
    TruncateRot(m_ViewFreeLookYaw, INPUT_PLAYER_ROT_BITS) ;

    bitstream::TruncateRangedF32(m_Energy, INPUT_PLAYER_ENERGY_BITS, 0.0f, PLAYER_ENERGY_MAX) ;

    if (m_JumpSurfaceLastContactTime > PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX)
        m_JumpSurfaceLastContactTime = PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX ;

    if (m_LandRecoverTime > PLAYER_LAND_RECOVER_TIME_MAX)
        m_LandRecoverTime = PLAYER_LAND_RECOVER_TIME_MAX ;

    if (m_JumpSurfaceLastContactTime != PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX)
        bitstream::TruncateRangedF32(m_JumpSurfaceLastContactTime,  PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_BITS, 0, PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX) ;

    bitstream::TruncateRangedF32(m_LandRecoverTime, PLAYER_LAND_RECOVER_TIME_BITS, 0, PLAYER_LAND_RECOVER_TIME_MAX) ;
}

//==============================================================================

void player_object::TruncateVel( f32& Value, s32 Bits )
{
    // Keep within range
    if (Value < -PLAYER_VEL_MAX)
        Value = -PLAYER_VEL_MAX ;
    else
    if (Value > PLAYER_VEL_MAX)
        Value = PLAYER_VEL_MAX ;

    // Truncate
	if (Value != 0)
        bitstream::TruncateRangedF32(Value, Bits, -PLAYER_VEL_MAX, PLAYER_VEL_MAX) ;

    ASSERT(Value >= -PLAYER_VEL_MAX) ;
    ASSERT(Value <=  PLAYER_VEL_MAX) ;
}

//==============================================================================

void player_object::TruncateVel( vector3& Value, s32 Bits )
{
    TruncateVel(Value.X, Bits) ;
    TruncateVel(Value.Y, Bits) ;
    TruncateVel(Value.Z, Bits) ;
}

//==============================================================================

void player_object::WriteVel( bitstream& BitStream, const f32 Value, s32 Bits )
{
    if (BitStream.WriteFlag(Value != 0))
		BitStream.WriteRangedF32(Value, Bits, -PLAYER_VEL_MAX, PLAYER_VEL_MAX) ;
}

//==============================================================================

void player_object::WriteVel( bitstream& BitStream, const vector3& Value, s32 Bits )
{
    WriteVel(BitStream, Value.X, Bits) ;
    WriteVel(BitStream, Value.Y, Bits) ;
    WriteVel(BitStream, Value.Z, Bits) ;
}

//==============================================================================

void player_object::ReadVel( const bitstream& BitStream, f32& Value, s32 Bits )
{
	if (BitStream.ReadFlag())
		BitStream.ReadRangedF32(Value, Bits, -PLAYER_VEL_MAX, PLAYER_VEL_MAX) ;
    else
        Value = 0 ;
}

//==============================================================================

void player_object::ReadVel( const bitstream& BitStream, vector3& Value, s32 Bits )
{
    ReadVel(BitStream, Value.X, Bits) ;
    ReadVel(BitStream, Value.Y, Bits) ;
    ReadVel(BitStream, Value.Z, Bits) ;
}

//==============================================================================

void player_object::TruncateRot( radian& Value, s32 Bits )
{
    // Keep in -R_180 to R_180
    if ((Value < -R_180) || (Value > R_180))
        Value = x_ModAngle2(Value) ;

    // Keep within range
    if (Value < -PLAYER_ROT_MAX)
        Value = -PLAYER_ROT_MAX ;
    else
    if (Value > PLAYER_ROT_MAX)
        Value = PLAYER_ROT_MAX ;

    // Truncate
	if (Value != 0)
        bitstream::TruncateRangedF32(Value, Bits, -PLAYER_ROT_MAX, PLAYER_ROT_MAX) ;

    ASSERT(Value >= -PLAYER_ROT_MAX) ;
    ASSERT(Value <=  PLAYER_ROT_MAX) ;
}

//==============================================================================

void player_object::TruncateRot( radian3& Value, s32 Bits )
{
    TruncateRot(Value.Pitch, Bits) ;
    TruncateRot(Value.Yaw,   Bits) ;
    TruncateRot(Value.Roll,  Bits) ;
}

//==============================================================================

void player_object::WriteRot( bitstream& BitStream, const radian Value, s32 Bits )
{
	if (BitStream.WriteFlag(Value != 0))
		BitStream.WriteRangedF32(Value, Bits, -PLAYER_ROT_MAX, PLAYER_ROT_MAX) ;
}

//==============================================================================

void player_object::WriteRot( bitstream& BitStream, const radian3& Value, s32 Bits )
{
    WriteRot( BitStream, Value.Pitch, Bits ) ;
    WriteRot( BitStream, Value.Yaw,   Bits ) ;
    WriteRot( BitStream, Value.Roll,  Bits ) ;
}

//==============================================================================

void player_object::ReadRot( const bitstream& BitStream, radian& Value, s32 Bits )
{
	if (BitStream.ReadFlag())
		BitStream.ReadRangedF32(Value, Bits, -PLAYER_ROT_MAX, PLAYER_ROT_MAX) ;
    else
        Value = 0 ;
}

//==============================================================================

void player_object::ReadRot( const bitstream& BitStream, radian3& Value, s32 Bits )
{
    ReadRot( BitStream, Value.Pitch, Bits ) ;
    ReadRot( BitStream, Value.Yaw,   Bits ) ;
    ReadRot( BitStream, Value.Roll,  Bits ) ;
}

//==============================================================================

// TO DO - SKIP POS + VELS IF IN A VEHICLE!!!

void player_object::WriteDirtyBitsData( bitstream& BitStream, u32& DirtyBits, f32 Priority, xbool MoveUpdate )
{
    // Incase all of this data does not fit in this packet
    u32     OriginalDirtyBits       = DirtyBits ;
    loadout OriginalClientLoadout   = m_ClientLoadout ;

    // 1st bit means there is some data...
    BitStream.WriteFlag(TRUE) ;

    // Setup priority levels
    xbool Level0;
    xbool Level1;
    xbool Level2;
    if (PLAYER_NO_PRIORITY)
    {
        Level0 = TRUE;
        Level1 = TRUE;
        Level2 = TRUE;
    }
    else
    {
        if( Priority > 0.5f )
        {
            Level0 = TRUE;
            Level1 = TRUE;
            Level2 = TRUE;
        }
        else
        if( Priority > 0.3f )
        {
            Level0 = TRUE;
            Level1 = ((tgl.NRenders%2)==0);
            Level2 = ((tgl.NRenders%3)==0);
        }
        else
        {
            Level0 = ((tgl.NRenders%2)==0);
            Level1 = ((tgl.NRenders%3)==0);
            Level2 = ((tgl.NRenders%4)==0);
        }
    }
//x_printf("Provide update %1d\n",m_ObjectID);

    // Levels
    BitStream.WriteFlag( Level0 ) ;
    BitStream.WriteFlag( Level1 ) ;
    BitStream.WriteFlag( Level2 ) ;

    // Create corpse - done before sending position update
    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_CREATE_CORPSE) != 0))
    {
        // Clear diry bit
        DirtyBits &= ~PLAYER_DIRTY_BIT_CREATE_CORPSE ;

        // Send death anim
        BitStream.WriteRangedS32(m_DeathAnimIndex, -1, 62) ;
    }

    // Lookup vehicle info
    vehicle*        Vehicle       = IsAttachedToVehicle() ;
    s32             VehicleStatus = GetVehicleStatus(Vehicle) ;

    // There should be a vehicle if we are attached to one!
    if (VehicleStatus != VEHICLE_STATUS_NULL)
        ASSERT(Vehicle) ;

    // Write vehicle info
    BitStream.WriteRangedS32(VehicleStatus, VEHICLE_STATUS_START, VEHICLE_STATUS_END) ;

    // Providing a move update?
    // (ie - this data is going to a controlling client player)
    if (BitStream.WriteFlag(MoveUpdate))
    {
        // Input player update...

        // Clear dirty bits that don't need to go to input players
        DirtyBits &= ~PLAYER_DIRTY_BIT_GHOST_COMP_POS ;

        // Tell client the last move that has been received
        BitStream.WriteS32( m_LastMoveReceived );

        // If piloting a vehicle, write the vehicle dirty bits too
        if (VehicleStatus == VEHICLE_STATUS_PILOT)
        {
            ASSERT(Vehicle) ;

            // Write vehicle ID
            BitStream.WriteObjectID(Vehicle->GetObjectID()) ;

            // Write dummy of total bits written
            // (there shouldn't be more than 127 bits!!)
            s32 StartCursor = BitStream.GetCursor() ;
            BitStream.WriteU32(0, 7) ; 

            // Write vehicle data
            Vehicle->WriteDirtyBitsData( BitStream, DirtyBits, Priority, MoveUpdate ) ;

            // We can now fill out how many bits were written
            s32 EndCursor = BitStream.GetCursor() ;             // Get current pos
            BitStream.SetCursor(StartCursor) ;                  // Put back to start
            BitStream.WriteU32(EndCursor - StartCursor, 7) ;    // Write length
            BitStream.SetCursor(EndCursor) ;                    // Put back to correct pos
        }
        else
        {
            // Clear vehicle dirty bits
            DirtyBits &= ~PLAYER_DIRTY_BIT_VEHICLE_USER_BITS ;
        }

        // Movement
        if (VehicleStatus == VEHICLE_STATUS_NULL)
        {
            // If the player is already dead, just leave the position so that the client
            // doesn't jump all over the place!
            // (NOTE: When the player is first put into dead state, his position will be
            //        set so this should be okay, beside, as soon as the player becomes a
            //        corpse, there is no net data sent so it's consitent)
            DirtyBits &= ~PLAYER_DIRTY_BIT_POS;
            if (BitStream.WriteFlag(m_PlayerState != PLAYER_STATE_DEAD))
            {
                // Write pos
	    	    BitStream.WriteVector(m_WorldPos) ;
            }

            // Vel
            DirtyBits &= ~PLAYER_DIRTY_BIT_VEL ;
            WriteVel( BitStream, m_Vel, INPUT_PLAYER_VEL_BITS ) ;

            // Jump surface last contact time (0=on jump surface, max=flying) 
            if (BitStream.WriteFlag(m_JumpSurfaceLastContactTime > 0))   // Non-min?
            {
                // Non-max?
                if (BitStream.WriteFlag(m_JumpSurfaceLastContactTime < PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX))
                    BitStream.WriteRangedF32(m_JumpSurfaceLastContactTime, PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_BITS, 0, PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX) ;
            }

            // Land recover time
            if (BitStream.WriteFlag(m_LandRecoverTime > 0))
                BitStream.WriteRangedF32(m_LandRecoverTime, PLAYER_LAND_RECOVER_TIME_BITS, 0, PLAYER_LAND_RECOVER_TIME_MAX) ;
        }
        else
        {
            // Clear dirty bits that aren't needed
            DirtyBits &= ~PLAYER_DIRTY_BIT_POS;
            DirtyBits &= ~PLAYER_DIRTY_BIT_VEL ;
        }

        // Rot (skip if player is dead since no prediction happens which keeps the pos + cam in sync!)
        DirtyBits &= ~PLAYER_DIRTY_BIT_ROT ;
        if (BitStream.WriteFlag(m_PlayerState != PLAYER_STATE_DEAD))
        {
		    WriteRot( BitStream, m_ViewFreeLookYaw, INPUT_PLAYER_ROT_BITS ) ;
            if ((VehicleStatus == VEHICLE_STATUS_NULL) || (VehicleStatus == VEHICLE_STATUS_MOVING_PASSENGER))
	    	    WriteRot( BitStream, m_Rot, INPUT_PLAYER_ROT_BITS ) ;
        }

        // Energy
        DirtyBits &= ~PLAYER_DIRTY_BIT_ENERGY;
        BitStream.WriteRangedF32( m_Energy, INPUT_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX );

        // Health (NOTE - THIS MUST ALWAYS BE SENT WITH THE STATE!!!)
	    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_HEALTH) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_HEALTH;

            // Non-zero?
            if (BitStream.WriteFlag(m_Health > 0))
                BitStream.WriteRangedF32( m_Health, INPUT_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );
        }

        // Inventory loadout
        if( BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_LOADOUT) != 0) )
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_LOADOUT ;

            // Just send whats changed
            m_Loadout.ProvideUpdate(BitStream, m_ClientLoadout) ;
        }

        // Wiggle
	    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_WIGGLE) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_WIGGLE;
		    BitStream.WriteRangedF32(m_DamageWiggleRadius,      8, 0, 0.5f) ;
		    BitStream.WriteRangedF32(m_DamageTotalWiggleTime,   8, 0, 2.0f) ;
		    BitStream.WriteRangedF32(m_DamageWiggleTime,        8, 0, 2.0f) ;
        }

        // Screen flash?
        if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_SCREEN_FLASH) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_SCREEN_FLASH ;

            BitStream.WriteF32(m_ScreenFlashSpeed) ;
            BitStream.WriteColor(m_ScreenFlashColor) ;
        }

        // Play sound?
        if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_PLAY_SOUND) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_PLAY_SOUND ;

            BitStream.WriteS32(m_SoundType) ;
        }

		// Exchange vars?
		if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_EXCHANGE_PICKUP) != 0))
		{
			DirtyBits &= ~PLAYER_DIRTY_BIT_EXCHANGE_PICKUP ;

			// Pickup kind there?
			if (BitStream.WriteFlag(m_ExchangePickupKind != pickup::KIND_NONE))
				BitStream.WriteRangedS32(m_ExchangePickupKind, pickup::KIND_NONE+1, pickup::KIND_TOTAL-1) ;
		}

        // Vote vars - didn't bother with a dirty bit since there's only 2 bits to go over
        BitStream.WriteFlag(m_VoteCanStart) ;   // Flags that player can start a vote
        BitStream.WriteFlag(m_VoteCanCast) ;    // Flags that player can cast a vote

        // send target lock data here
        // and clear the dirty bit
        if( BitStream.WriteFlag( DirtyBits & PLAYER_DIRTY_BIT_TARGET_LOCK ) )
        {   
            DirtyBits &= ~PLAYER_DIRTY_BIT_TARGET_LOCK;
            if( BitStream.WriteFlag( m_TargetIsLocked ) )
                BitStream.WriteObjectID( m_LockedTarget );
        }
    }
    else
    {
        // Ghost player update...

        // Clear dirty bits that don't need to go to ghost players
        DirtyBits &= ~(     PLAYER_DIRTY_BIT_LOADOUT
                        |   PLAYER_DIRTY_BIT_WIGGLE
                        |   PLAYER_DIRTY_BIT_TARGET_LOCK
                        |   PLAYER_DIRTY_BIT_SCREEN_FLASH
                        |   PLAYER_DIRTY_BIT_PLAY_SOUND 
                        |   PLAYER_DIRTY_BIT_VEHICLE_USER_BITS
                        |   PLAYER_DIRTY_BIT_EXCHANGE_PICKUP ) ;

        // Move
        m_Move.ProvideUpdate(BitStream) ;

        // Movement
        if (VehicleStatus == VEHICLE_STATUS_NULL)
        {
	        // Pos
            if( Level0 )
            {
	            if (BitStream.WriteFlag((DirtyBits & (PLAYER_DIRTY_BIT_POS | PLAYER_DIRTY_BIT_GHOST_COMP_POS)) != 0))
                {
                    DirtyBits &= ~PLAYER_DIRTY_BIT_POS;
                    WriteGhostPos( BitStream, m_WorldPos, DirtyBits ) ;
                }
            }
        
            // Vel
            if( Level1 )
            {
	            if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_VEL) != 0))
                {
                    DirtyBits &= ~PLAYER_DIRTY_BIT_VEL ;
                    WriteGhostVel( BitStream, m_Vel ) ;
                }
            }
        }
        else
        {
            // Clear dirty bits that aren't needed
            DirtyBits &= ~PLAYER_DIRTY_BIT_POS;
            DirtyBits &= ~PLAYER_DIRTY_BIT_VEL ;
        }

        // Rot
        if( Level2 )
        {
            // Rotation?
	        if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_ROT) != 0))
            {
                DirtyBits &= ~PLAYER_DIRTY_BIT_ROT ;
		        WriteRot( BitStream, m_ViewFreeLookYaw, GHOST_PLAYER_ROT_BITS ) ;
                if ((VehicleStatus == VEHICLE_STATUS_NULL) || (VehicleStatus == VEHICLE_STATUS_MOVING_PASSENGER))
		            WriteRot( BitStream, m_Rot, GHOST_PLAYER_ROT_BITS ) ;
            }
        }

        // Energy
        if( Level0 )
        {
            if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_ENERGY) != 0))
            {
                DirtyBits &= ~PLAYER_DIRTY_BIT_ENERGY;
                BitStream.WriteRangedF32( m_Energy, GHOST_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX );
            }
        }

        // Health (NOTE - THIS MUST ALWAYS BE SENT WITH THE STATE!!!)
	    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_HEALTH) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_HEALTH;

            // Non-zero?
            if (BitStream.WriteFlag(m_Health > 0))
                BitStream.WriteRangedF32( m_Health, GHOST_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );
        }
    }

    // Weapon lock info
    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_TARGET_DATA) != 0))
    {
        DirtyBits &= ~PLAYER_DIRTY_BIT_TARGET_DATA ;

        // Write missile vars
        BitStream.WriteFlag( m_MissileLock ) ;
        BitStream.WriteFlag( m_MissileTargetTrackCount > 0 ) ;
        BitStream.WriteFlag( m_MissileTargetLockCount > 0 ) ;

        // Write target info
        BitStream.WriteObjectID(m_WeaponTargetID) ;
    }

    // Pack
	if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_PACK_TYPE) != 0))
    {
        DirtyBits &= ~PLAYER_DIRTY_BIT_PACK_TYPE ;

        // Status
        BitStream.WriteFlag(m_PackActive) ;

        // Type
        BitStream.WriteRangedU32((u32)m_PackCurrentType, INVENT_TYPE_START, INVENT_TYPE_END) ;
    }

    // Vehicle status
    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_VEHICLE_STATUS) != 0))
    {
        DirtyBits &= ~PLAYER_DIRTY_BIT_VEHICLE_STATUS ;

        // Attaching to vehicle?
        if (BitStream.WriteFlag(m_VehicleID != obj_mgr::NullID))
        {
            BitStream.WriteObjectID(m_VehicleID) ;
            BitStream.WriteRangedS32(m_VehicleSeat, 0, vehicle::MAX_SEATS-1) ;
        }    
    }

    // Flag status
    if ( Level0 )
    {
        if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_FLAG_STATUS) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_FLAG_STATUS ;

            // Changed flag count and/or attached flag?
            BitStream.WriteS32( m_FlagCount, 10 );
            if (BitStream.WriteFlag(m_FlagInstance.GetShape() != NULL))
                BitStream.WriteRangedU32((s32)m_FlagInstance.GetTextureFrame(), PLAYER_MIN_FLAG_TEXTURE, PLAYER_MAX_FLAG_TEXTURE) ;  // Write texture #
        }
    }

    // Glow status
    if( Level0 )
    {
        if( BitStream.WriteFlag( m_Glow ) )
            BitStream.WriteRangedF32( m_GlowRadius, 8, 0.0f, 10.0f );
        DirtyBits &= ~PLAYER_DIRTY_BIT_GLOW_STATUS ;
    }

    // Weapon/character info
    if( Level2 )
    {
        // Weapon type
	    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_WEAPON_TYPE) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_WEAPON_TYPE;
            BitStream.WriteRangedU32((u32)m_WeaponCurrentType, (u32)INVENT_TYPE_WEAPON_START, (u32)INVENT_TYPE_WEAPON_END) ;
            BitStream.WriteRangedU32((u32)m_WeaponCurrentSlot, 0, 5) ;
        }

        // Weapon state
	    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_WEAPON_STATE) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_WEAPON_STATE;
            BitStream.WriteRangedU32((u32)m_WeaponState, (u32)WEAPON_STATE_START, (u32)WEAPON_STATE_END) ;
        }
       
        // Character type?
        if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_CHARACTER) != 0))
        {
            DirtyBits &= ~PLAYER_DIRTY_BIT_CHARACTER ;
            BitStream.WriteRangedU32((u32)m_CharacterType, (u32)CHARACTER_TYPE_START, (u32)CHARACTER_TYPE_END) ;
            BitStream.WriteRangedU32((u32)m_ArmorType,     (u32)ARMOR_TYPE_START,     (u32)ARMOR_TYPE_END) ;
            BitStream.WriteRangedU32((u32)m_SkinType,      (u32)SKIN_TYPE_START,      (u32)SKIN_TYPE_END) ;
            BitStream.WriteRangedU32((u32)m_VoiceType,     (u32)VOICE_TYPE_START,     (u32)VOICE_TYPE_END) ;
        }
    }

    // Player state
	if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_PLAYER_STATE) != 0))
    {
        DirtyBits &= ~PLAYER_DIRTY_BIT_PLAYER_STATE;
		BitStream.WriteRangedU32((u32)m_PlayerState, (u32)PLAYER_STATE_START, (u32)PLAYER_STATE_END) ;

        // Send anim index too?
        // (only needed for states that use a random anim. ie. several anims have the same type)
        switch(m_PlayerState)
        {
            case PLAYER_STATE_TAUNT:
            case PLAYER_STATE_DEAD:
                {
                    ASSERT(m_PlayerInstance.GetCurrentAnimState().GetAnim()) ;
                    u32 AnimIndex = (u32)m_PlayerInstance.GetCurrentAnimState().GetAnim()->GetIndex() ;
                    ASSERT(AnimIndex != 0xFFFFFFFF) ;

            		BitStream.WriteRangedU32(AnimIndex, 0, 63) ;
                }
                break ;
        }
    }

    // Armed
    if (Level2)
    {
        BitStream.WriteFlag(m_Armed) ;
    }

    // Spawn?
    if (BitStream.WriteFlag((DirtyBits & PLAYER_DIRTY_BIT_SPAWN) != 0))
    {
        DirtyBits &= ~PLAYER_DIRTY_BIT_SPAWN ;

        // Send team bits incase the player has swapped teams
		BitStream.WriteTeamBits(m_TeamBits) ;
    }

    // Make sure any unused bits are cleared so the server stops sending when it doesn't need to!
    DirtyBits &= ~PLAYER_DIRTY_BIT_UNUSED ;

    //==========================================================================
    // If we couldn't fit all the player data in the packet, then rewind and
    // write a FALSE to flag there is no data for the player...
    //==========================================================================

    // Failed to fit everything?
    if (BitStream.Overwrite())
    {
        // Restore dirty bits and client loadout so everything is sent again next time
        DirtyBits       = OriginalDirtyBits ;
        m_ClientLoadout = OriginalClientLoadout ;

		// If there is an overrun, the parent routine will detect it so we don't need
		// to rewind or write a flag saying it wasn't written

    }
    else
    {
        // Check to make sure all the dirty bits have been cleared as expected!
        if (Priority == 1)
        {
            // If not attached to a vehicle then all the dirty bits should be 0
            // (when attached, the player position is not sent, and hence the ghost
            //  pos dirty bits will be left as is)
            if (VehicleStatus == VEHICLE_STATUS_NULL)
            {
                ASSERT(DirtyBits == 0) ;
            }
            else
            {
                ASSERT((DirtyBits & (~PLAYER_DIRTY_BIT_GHOST_COMP_POS)) == 0) ;
            }
        }
    }
}

//==============================================================================

xbool player_object::ReadDirtyBitsData( const bitstream& BitStream, f32 SecInPast )
{
    xbool           Level0;
    xbool           Level1;
    xbool           Level2;
    xbool           MoveUpdate ;
    xbool           DoTargetDeniedSound = FALSE;

    // Update next recieve data sample time
    UpdateNetSamples( SecInPast ) ;

    // If there is no data present, just exit
    if (!BitStream.ReadFlag())
        return FALSE ;

    // Read levels
    Level0 = BitStream.ReadFlag() ;
    Level1 = BitStream.ReadFlag() ;
    Level2 = BitStream.ReadFlag() ;

    // Create corpse - done here before position is updated
    if (BitStream.ReadFlag())
    {
        // Read death anim
        BitStream.ReadRangedS32(m_DeathAnimIndex, -1, 62) ;

        // Create a corpse on this client
        CreateCorpse() ;
    }

    // Read vehicle info
    s32  VehicleStatus ;
    BitStream.ReadRangedS32(VehicleStatus, VEHICLE_STATUS_START, VEHICLE_STATUS_END) ;

    // Is this a move update?
    // ie. is going to a controlled client player
    if ((MoveUpdate = BitStream.ReadFlag()))
    {
        // Flag client that it is recieving a move update so that weapon ammo isn't updated
        m_NetMoveUpdate = TRUE ;

        // Get the last move that the server been recveived and applied
        BitStream.ReadS32( m_LastMoveReceived );

        // If piloting a vehicle, read the vehicle dirty bits too!
        if (VehicleStatus == VEHICLE_STATUS_PILOT)
        {
            // Get vehicle ID
            object::id VehicleID ;
            BitStream.ReadObjectID(VehicleID) ;

            // Get bits to read incase vehicle is not there
            u32 BitsToRead ;
            BitStream.ReadU32(BitsToRead, 7) ; 

            // Is the vehicle created yet?
            vehicle* Vehicle = (vehicle*)ObjMgr.GetObjectFromID(VehicleID) ;
            if ((Vehicle) && (Vehicle->GetAttrBits() & object::ATTR_VEHICLE))
            {
                // Update the vehicle
                Vehicle->ReadDirtyBitsData( BitStream, SecInPast ) ;
            }
            else
            {
                // Skip the vehicle data!
                while(BitsToRead--)
                    BitStream.ReadFlag() ;
            }
        }

        // Movement
        if (VehicleStatus == VEHICLE_STATUS_NULL)
        {
            // Get new pos?
            if (BitStream.ReadFlag())
            {
                // Read pos
		        BitStream.ReadVector(m_WorldPos) ;

			    // Update bounds
                CalcBounds() ;
			    
			    // Setup contact info ready for move/physics
			    SetupContactInfo() ;
            }
            
            // Vel
            ReadVel( BitStream, m_Vel, INPUT_PLAYER_VEL_BITS ) ;

            // Jump surface last contact time (0=on jump surface, max=flying) 
            if (BitStream.ReadFlag())   // Non-min?
            {
                // Non-max?
                if (BitStream.ReadFlag())
                    BitStream.ReadRangedF32(m_JumpSurfaceLastContactTime, PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_BITS, 0, PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX) ;
                else
                    m_JumpSurfaceLastContactTime = PLAYER_JUMP_SURFACE_LAST_CONTACT_TIME_MAX ;
            }
            else
                m_JumpSurfaceLastContactTime = 0 ;

            // Land recover time
            if (BitStream.ReadFlag())   // Non-min?
                BitStream.ReadRangedF32(m_LandRecoverTime, PLAYER_LAND_RECOVER_TIME_BITS, 0, PLAYER_LAND_RECOVER_TIME_MAX) ;
            else
                m_LandRecoverTime = 0 ;
        }

        // Rot
        if (BitStream.ReadFlag())
        {
		    ReadRot( BitStream, m_ViewFreeLookYaw, INPUT_PLAYER_ROT_BITS ) ;
            if ((VehicleStatus == VEHICLE_STATUS_NULL) || (VehicleStatus == VEHICLE_STATUS_MOVING_PASSENGER))
		        ReadRot( BitStream, m_Rot, INPUT_PLAYER_ROT_BITS ) ;
        }

        // Energy
        BitStream.ReadRangedF32( m_Energy, INPUT_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX );

	    // Health
	    if (BitStream.ReadFlag())
        {
            // Non-zero?
            if (BitStream.ReadFlag())
            {
                // Read health
                BitStream.ReadRangedF32( m_Health, INPUT_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );

                // Incase health is truncated to zero
                if (m_Health == 0)
                    m_Health = 1 ;
            }
            else
                m_Health = 0 ;
        }
        
        // Inventory loadout
        if( BitStream.ReadFlag() )
        {
            // Just read whats changed
            m_Loadout.AcceptUpdate(BitStream) ;
        }

        // Wiggle
	    if (BitStream.ReadFlag())
        {
		    BitStream.ReadRangedF32(m_DamageWiggleRadius,      8, 0, 0.5f) ;
		    BitStream.ReadRangedF32(m_DamageTotalWiggleTime,   8, 0, 2.0f) ;
		    BitStream.ReadRangedF32(m_DamageWiggleTime,        8, 0, 2.0f) ;
        }

        // Screen flash?
        if (BitStream.ReadFlag())
        {
            BitStream.ReadF32(m_ScreenFlashSpeed) ;
            BitStream.ReadColor(m_ScreenFlashColor) ;
            if (m_ScreenFlashSpeed != 0)
                FlashScreen(1.0f / m_ScreenFlashSpeed, m_ScreenFlashColor) ;
        }

        // Play sound?
        if (BitStream.ReadFlag())
        {
            BitStream.ReadS32(m_SoundType) ;
            PlaySound(m_SoundType) ;
        }

		// Exchange vars?
		if (BitStream.ReadFlag())
		{
			// Pickup kind there?
			if (BitStream.ReadFlag())
			{
				s32 PickupKind ;
				BitStream.ReadRangedS32(PickupKind, pickup::KIND_NONE+1, pickup::KIND_TOTAL-1) ;
				m_ExchangePickupKind = (pickup::kind)PickupKind ;
			}
			else
				m_ExchangePickupKind = pickup::KIND_NONE ;
		}

        // Vote vars - didn't bother with a dirty bit since there's only 2 bits to go over
        m_VoteCanStart = BitStream.ReadFlag() ;     // Flags that player can start a vote
        m_VoteCanCast  = BitStream.ReadFlag() ;     // Flags that player can cast a vote

        // Read target lock data here
        if( BitStream.ReadFlag() )
        {
            xbool Locked = BitStream.ReadFlag();
            if( Locked )
                BitStream.ReadObjectID( m_LockedTarget );
            else
                m_LockedTarget = obj_mgr::NullID;

            // Audio for the client.
            if( m_Health > 0.0f )
            {
                if( Locked && !m_TargetIsLocked )
                    audio_Play( SFX_TARGET_LOCK_AQUIRED );

                if( !Locked && m_TargetIsLocked )
                    audio_Play( SFX_TARGET_LOCK_LOST );

                if( !Locked && !m_TargetIsLocked )
                    DoTargetDeniedSound = TRUE;
                    
            }

            // Keep the value.
            m_TargetIsLocked = Locked;
        }
    }
    else
    {
        // Ghost accept...

        // Move
        m_Move.AcceptUpdate(BitStream) ;

        // Movement
        if (VehicleStatus == VEHICLE_STATUS_NULL)
        {
	        // Pos
            if( Level0 )
            {
                // Pos?
	            if (BitStream.ReadFlag())
                {
		            ReadGhostPos(BitStream, m_WorldPos) ;
					
					// Update bounds
                    CalcBounds() ;

					// Setup contact info ready for move/physics
					SetupContactInfo() ;
                }
            }

            // Vel
            if( Level1 )
            {
	            // Vel?
	            if (BitStream.ReadFlag())
                    ReadGhostVel( BitStream, m_Vel ) ;
            }
        }

        // Rot
        if( Level2 )
        {
	        // Rotation?
	        if (BitStream.ReadFlag())
            {
		        ReadRot( BitStream, m_ViewFreeLookYaw, GHOST_PLAYER_ROT_BITS ) ;
                if ((VehicleStatus == VEHICLE_STATUS_NULL) || (VehicleStatus == VEHICLE_STATUS_MOVING_PASSENGER))
		            ReadRot( BitStream, m_Rot, GHOST_PLAYER_ROT_BITS ) ;
            }
        }

        // Energy
        if( Level0 )
        {
            if (BitStream.ReadFlag())
                BitStream.ReadRangedF32( m_Energy, GHOST_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX ) ;
        }

	    // Health
	    if (BitStream.ReadFlag())
        {
            // Non-zero?
            if (BitStream.ReadFlag())
            {
                // Read health
                BitStream.ReadRangedF32( m_Health, GHOST_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );

                // Incase health is truncated to zero
                if (m_Health == 0)
                    m_Health = 1 ;
            }
            else
                m_Health = 0 ;
        }
    }

    // Weapon lock info
    if (BitStream.ReadFlag())
    {
        // Read missile vars
        m_MissileLock             = BitStream.ReadFlag() ;
        m_MissileTargetTrackCount = BitStream.ReadFlag() ;
        m_MissileTargetLockCount  = BitStream.ReadFlag() ;

        // Read target 
        BitStream.ReadObjectID(m_WeaponTargetID) ;
    }
    
    // Pack
	if (BitStream.ReadFlag())
    {
        // Status
        xbool PackActive = BitStream.ReadFlag() ;

        // Type
        s32 PackType ;
        BitStream.ReadRangedS32(PackType, INVENT_TYPE_START, INVENT_TYPE_END) ;

        // Update player vars ( call functions so that sounds get updated etc)
        SetPackCurrentType((invent_type)PackType) ;
        SetPackActive(PackActive) ;
    }

    // Vehicle status
    if (BitStream.ReadFlag())
    {
        // Attaching to vehicle?
        if (BitStream.ReadFlag())
        {
            BitStream.ReadObjectID(m_VehicleID) ;
            BitStream.ReadRangedS32(m_VehicleSeat, 0, vehicle::MAX_SEATS-1) ;
            
            // Initialize spring camera variables on client controlled vehicles
            if( m_VehicleID != obj_mgr::NullID )
            {
                // Only setup camera if we are controlling the vehicle
                if( m_HasInputFocus == TRUE )
                {
                    vehicle* pVehicle = (vehicle*)ObjMgr.GetObjectFromID( m_VehicleID );
                    InitSpringCamera( pVehicle );                    
                }
            }
        }    
        else
        {
            // Clear vehicle vars
            m_VehicleID   = obj_mgr::NullID ;             // ID of vehicle (if in one)
            m_VehicleSeat = -1 ;                          // Vehicle seat number (if in one)
        }
    }

    // Flag status
    if ( Level0 )
    {
        if (BitStream.ReadFlag())
        {
            BitStream.ReadS32( m_FlagCount, 10 );

            // Just attached flag?
            if (BitStream.ReadFlag())
            {
                // Get flag texture
                s32 FlagTexture ;
                BitStream.ReadRangedS32(FlagTexture, PLAYER_MIN_FLAG_TEXTURE, PLAYER_MAX_FLAG_TEXTURE) ;

                // Attach flag
                AttachFlag(FlagTexture) ;
            }
            else
                DetachFlag() ;  // Remove flag
        }
    }

    // Glow status
    if ( Level0 )
    {
        if( BitStream.ReadFlag() )
        {
            f32 Radius;
            BitStream.ReadRangedF32( Radius, 8, 0.0f, 10.0f );
            SetGlow( Radius );
        }
        else
        {
            ClearGlow();
        }
    }

    // Weapon/character info
    if( Level2 )
    {
        // Weapon type
	    if (BitStream.ReadFlag())
        {
            // Update weapon
            s32 WeaponRequestedType ;
            BitStream.ReadRangedS32(WeaponRequestedType, INVENT_TYPE_WEAPON_START, INVENT_TYPE_WEAPON_END) ;
            m_WeaponRequestedType = (invent_type)WeaponRequestedType ;

            // Call this incase the weapon has changed so that the weapon state machine does it's stuff
            // (turns off sounds, plays new activate sounds etc)
            Weapon_CheckForUpdatingShape() ;

            // Update slot
            BitStream.ReadRangedS32(m_WeaponCurrentSlot, 0, 5) ;
        }

        // Weapon state
	    if (BitStream.ReadFlag())
        {
            // Update state
            s32 WeaponState ;
            BitStream.ReadRangedS32(WeaponState, WEAPON_STATE_START, WEAPON_STATE_END) ;

            // Update states
            Weapon_SetupState((weapon_state)WeaponState) ;
        }

        // Character type?
        if (BitStream.ReadFlag())
        {
            s32 CharType, ArmorType, SkinType, VoiceType ;

            BitStream.ReadRangedS32(CharType,  CHARACTER_TYPE_START, CHARACTER_TYPE_END) ;
            BitStream.ReadRangedS32(ArmorType, ARMOR_TYPE_START,     ARMOR_TYPE_END) ;
            BitStream.ReadRangedS32(SkinType,  SKIN_TYPE_START,      SKIN_TYPE_END) ;
            BitStream.ReadRangedS32(VoiceType, VOICE_TYPE_START,     VOICE_TYPE_END) ;

            // Set new character and armor
            SetCharacter( (character_type)CharType, (armor_type)ArmorType, (skin_type)SkinType, (voice_type)VoiceType ) ;
        }
    }

    // Player state
	if (BitStream.ReadFlag())
    {
        s32 PlayerState ;
        s32 AnimIndex = -1 ;

        // Read state
		BitStream.ReadRangedS32(PlayerState, PLAYER_STATE_START, PLAYER_STATE_END) ;

        // Read anim index too?
        switch(PlayerState)
        {
            case PLAYER_STATE_TAUNT:
            case PLAYER_STATE_DEAD:
            	BitStream.ReadRangedS32(AnimIndex, 0, 63) ;
                break ;
        }

        // Setup the state
        Player_SetupState((player_state)PlayerState, FALSE) ;

        // Set anim too?
        if (AnimIndex != -1)
        {
            f32 BlendTime = 0.25f ;
            switch(PlayerState)
            {
                case PLAYER_STATE_TAUNT:    BlendTime = 0.25f ;
                case PLAYER_STATE_DEAD:     BlendTime = 0.25f ;
            }
            m_PlayerInstance.SetAnimByIndex((s32)AnimIndex, BlendTime) ;
        }
    }

    // Armed
    if (Level2)
    {
        m_Armed = BitStream.ReadFlag() ;
    }

    // Spawn?
    if (BitStream.ReadFlag())
    {
        // Get team bits incase the player has swapped teams
		BitStream.ReadTeamBits(m_TeamBits) ;

        // Do respawn effects
        Respawn() ;
    }

    //==========================================================================
    // Ghost player prediction
    //==========================================================================

    // Ghost player?        
    if (!m_ProcessInput)
    {
        // Cap latency time
        if (SecInPast > GHOST_PLAYER_LATENCY_TIME_MAX)
        {
            // Add rest of latency time to prediction so it's spread over the logic loop
            m_PredictionTime = (SecInPast - GHOST_PLAYER_LATENCY_TIME_MAX) ;

            // Cap latency time
            SecInPast = GHOST_PLAYER_LATENCY_TIME_MAX ;
        }

        // Put ghost client at predicted pos
        while(SecInPast >= PLAYER_TIME_STEP_MIN)
        {
            // Apply prediction with max of 30fps
            m_Move.DeltaTime = MIN(SecInPast, GHOST_PLAYER_LATENCY_TIME_STEP) ;
            ApplyMove() ;
	        ApplyPhysics() ;

            // Next time
            SecInPast -= GHOST_PLAYER_LATENCY_TIME_STEP ;
        }

        // Add any remaining time to the prediction
        m_PredictionTime += SecInPast ;

        // Cap prediction time
        if (m_PredictionTime > GHOST_PLAYER_PREDICT_TIME_MAX)
            m_PredictionTime = GHOST_PLAYER_PREDICT_TIME_MAX ;

        // Blend to new info if not too far away
        if(     (PLAYER_CLIENT_BLENDING)
            &&  (!IsVehiclePilot())     // Pilots are stuck to vehicles like glue!
            &&  (m_PlayerState != PLAYER_STATE_CONNECTING)
            &&  (m_PlayerState != PLAYER_STATE_WAIT_FOR_GAME)
            &&  (m_NetSampleAverage.RecieveDeltaTime != 0)
            &&  ((m_WorldPos - m_DrawPos).LengthSquared() <= (PLAYER_BLEND_DIST_MAX * PLAYER_BLEND_DIST_MAX)) )
	    {
            f32 Time ;

            // Blend from last pos and rot
            m_BlendMove.DeltaPos = m_DrawPos - m_WorldPos ;
            m_BlendMove.DeltaRot = m_DrawRot - m_Rot ;
            m_BlendMove.DeltaRot.ModAngle2() ;
            m_BlendMove.DeltaViewFreeLookYaw = x_ModAngle2(m_DrawViewFreeLookYaw - m_ViewFreeLookYaw) ;

#if 0
#define MIN_BLEND_TIME      (0.2f)
#define MAX_BLEND_TIME      (0.6f)
#define MID_BLEND_TIME      ((MIN_BLEND_TIME + MAX_BLEND_TIME)/2.0f)
            // Calc average speed of client and server
            f32 AverageSpeed = (Speed + m_Vel.Length()) * 0.5f ;

            // Calc time to get to new pos, given average speed
            if (AverageSpeed > 0.00001f)
                Time = (m_BlendMove.DeltaPos.Length() / AverageSpeed) * MID_BLEND_TIME ;
            else
                Time = MID_BLEND_TIME ;

            // Cap blend time
            if (Time < MIN_BLEND_TIME)
                Time = MIN_BLEND_TIME ;
            else
            if (Time > MAX_BLEND_TIME)
                Time = MAX_BLEND_TIME ;
#endif

            // Blend using just a tad more then the average recieve time
            // so that the ghost always has something to aim for 
            Time = m_NetSampleAverage.RecieveDeltaTime * PLAYER_CLIENT_BLEND_TIME_SCALER ;
            if (Time > PLAYER_CLIENT_BLEND_TIME_MAX)
                Time = PLAYER_CLIENT_BLEND_TIME_MAX ;

            // Start the blend from the last pos
            m_BlendMove.Blend    = 1;
            m_BlendMove.BlendInc = -1.0f / Time ;

            // Predict for a tad...
            //m_PredictionTime = m_NetSampleAverage.RecieveDeltaTime ;
        }
        else
        {
            m_BlendMove.Blend = 0.0f ;  // No blend
            m_BlendMove.BlendInc = 0.0f ;
        }

        // On vehicle?
        vehicle* Vehicle = IsAttachedToVehicle() ;
        if (Vehicle)
        {   
            // Don't blend rotation if player is fixed to vehicle
            const vehicle::seat& Seat = Vehicle->GetSeat(m_VehicleSeat) ;
            if (!Seat.CanLookAndShoot)
                m_BlendMove.DeltaRot.Zero() ;

            // Never blend pos since vehicle does it's own blending
            m_BlendMove.DeltaPos.Zero() ;
        }
    }

    if( m_Armed && DoTargetDeniedSound )
        audio_Play( SFX_TARGET_LOCK_DENIED );

    return MoveUpdate ;
}

//==============================================================================

void player_object::UpdateNetSamples( f32 SecsInPast )
{
    s32 i ;

    // Skip if already done this frame
    // (the server receives 5-7 moves in the same frame!)
    if (tgl.NRenders == m_NetSampleFrame)
        return ;

    // Keep frame
    m_NetSampleFrame = tgl.NRenders ;

	// Keep stat
	m_NetSampleCurrent.SecsInPast = SecsInPast ;

	// Store info
    m_NetSample[m_NetSampleIndex] = m_NetSampleCurrent ;
    
	// Goto next sample slot
	if (++m_NetSampleIndex == NET_SAMPLES)
        m_NetSampleIndex = 0 ;

    // Clear current stats
	m_NetSampleCurrent.Clear() ;

    // Calculate average of all samples
    m_NetSampleAverage.Clear() ;
    for (i = 0 ; i < NET_SAMPLES ; i++)
        m_NetSampleAverage += m_NetSample[i] ;
    m_NetSampleAverage /= NET_SAMPLES ;
}

//==============================================================================

#ifdef PLAYER_TIMERS
xtimer UpdatePlayerTimer;
#endif

// On client...
object::update_code player_object::OnAcceptUpdate( const bitstream& BitStream, f32 SecInPast )
{

#ifdef PLAYER_TIMERS
UpdatePlayerTimer.Start();
#endif

    // Keep a copy of the players current anim status incase we need to restore it
    model*                          PlayerModel = m_PlayerInstance.GetModel() ;
    player_state                    PlayerState = m_PlayerState ;
    shape_instance::anim_status     PlayerAnimStatus ;
    m_PlayerInstance.GetAnimStatus(PlayerAnimStatus) ;

    // Read dirty bits
    xbool MoveUpdate = ReadDirtyBitsData(BitStream, SecInPast) ;

    // Is this a move update?
    if (MoveUpdate)
    {
        // Skip the prediction if dead to keep the client player from wobbling all over the place!
        // Beside - nothing can move the player anymore - simple!
        if (m_PlayerState != PLAYER_STATE_DEAD)
        {
            // Apply history
            s32 LastMoveSeq = m_LastMoveReceived ;
            if( LastMoveSeq != -1 )
            {
                if( (m_pMM) && (m_pMM->FindMoveInPast(LastMoveSeq)) )
                {
                    s32 C = 0;
                    s32 MoveSeq = LastMoveSeq ;
                    s32 Slot;

                    //x_DebugMsg("Applying old moves...\n") ;

                    while( m_pMM->GetNextMove(m_Move, Slot) )
                    {
                        MoveSeq++ ;

                        if( m_ObjectID.Slot == Slot )
                        {
                            // Update player
                            ApplyMove();
                            ApplyPhysics() ;
                            C++;
                        }
                    }

                    //x_printf("Applied multi-move update to %1d\n",m_ObjectID);
                    //x_DebugMsg("Updated and applied %1d moves\n",C);
                }
            }
        }
    }

    // If the old client predicted state is the same as the predicted servers
    // state, then restore the animation vars so the client player looks smooth
    if (        (m_PlayerState               != PLAYER_STATE_DEAD)
            &&  (m_PlayerInstance.GetModel() == PlayerModel)
            &&  (m_PlayerState               == PlayerState) )
    {
        // Set instance anim to where is was
        m_PlayerInstance.SetAnimStatus(PlayerAnimStatus) ;
    }

    // Back to normal
    m_NetMoveUpdate = FALSE ;

#ifdef PLAYER_TIMERS
UpdatePlayerTimer.Stop();
#endif

    // Update
    return( UPDATE );
}

//==============================================================================

// On server...
void player_object::OnProvideUpdate( bitstream& BitStream, u32& DirtyBits, f32 Priority )
{
    // Send info
    WriteDirtyBitsData(BitStream, DirtyBits, Priority, FALSE ) ;

    //x_printfxy( 10, 12+(m_ObjectID.Slot%5), "%4.2f", Priority );
}

//==============================================================================

// On server...
void player_object::OnProvideMoveUpdate( bitstream& BitStream, u32& DirtyBits )
{
    // Send info
    WriteDirtyBitsData( BitStream, DirtyBits, 1.0f, TRUE ) ;

    //x_printf("Provide move update %1d %08X\n",m_ObjectID,DirtyBits);
}

//==============================================================================

// On client...
void player_object::OnAcceptInit( const bitstream& BitStream )
{
    //x_DebugMsg("On Accept Init %1d\n",m_ObjectID);

	// Put in valid state
	Reset() ;

    // Attributes
    BitStream.ReadU32(m_AttrBits) ;

    // Energy
    BitStream.ReadRangedF32( m_Energy, INPUT_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX );

    // Health
    if (BitStream.ReadFlag())
        BitStream.ReadRangedF32( m_Health, INPUT_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );
    else
        m_Health = 0 ;

	// World info
	BitStream.ReadVector(m_WorldPos) ;
	BitStream.ReadVector(m_GhostCompPos) ;
	BitStream.ReadRadian3(m_Rot) ;
	
	// Character info
	s32 CharacterType, ArmorType, SkinType, VoiceType ;
	BitStream.ReadS32(CharacterType) ;
	BitStream.ReadS32(ArmorType) ;
	BitStream.ReadS32(SkinType) ;
	BitStream.ReadS32(VoiceType) ;
	SetCharacter((character_type)CharacterType,
				 (armor_type)    ArmorType,
                 (skin_type)     SkinType,
                 (voice_type)    VoiceType) ;
    BitStream.ReadS32(m_PlayerIndex) ;

    // Loadouts
    m_Loadout.Read(BitStream) ;
    BitStream.ReadS32(m_DefaultLoadoutType) ;
    m_InventoryLoadout.Read(BitStream) ;

    // Weapon info
    s32 WeaponCurrentType ;
    BitStream.ReadRangedS32(WeaponCurrentType, INVENT_TYPE_WEAPON_START, INVENT_TYPE_WEAPON_END) ;
    m_WeaponCurrentType   = (invent_type)WeaponCurrentType ;
    m_WeaponRequestedType = m_WeaponCurrentType ;
    m_WeaponInfo          = &s_WeaponInfo[m_WeaponCurrentType] ;

    BitStream.ReadRangedS32(m_WeaponCurrentSlot, 0, 5);
    
    s32 WeaponState ;
    BitStream.ReadRangedS32(WeaponState, WEAPON_STATE_START, WEAPON_STATE_END) ;
    Weapon_SetupState((weapon_state)WeaponState) ;

    // Vehicle info
    BitStream.ReadObjectID(m_VehicleID) ;
    BitStream.ReadS32(m_VehicleSeat) ;

	// Player state
    s32 Value;
	BitStream.ReadRangedS32(Value, PLAYER_STATE_START, PLAYER_STATE_END) ;
	Player_SetupState((player_state)Value, FALSE) ;
    BitStream.ReadWString( m_Name );
    BitStream.ReadU32( m_TeamBits );

    // Flag attached?
    BitStream.ReadS32( m_FlagCount, 10 );
    if (BitStream.ReadFlag())
    {
        // Get flag texture
        s32 FlagTexture ;
        BitStream.ReadRangedS32(FlagTexture, PLAYER_MIN_FLAG_TEXTURE, PLAYER_MAX_FLAG_TEXTURE) ;

        // Attach flag
        AttachFlag(FlagTexture) ;
    }
    else
        DetachFlag() ;  // Remove flag

    // Glowing?
    if( BitStream.ReadFlag() )
    {
        f32 Radius;
        BitStream.ReadRangedF32( Radius, 8, 0.0f, 10.0f );
        SetGlow( Radius );
    }
    else
    {
        ClearGlow();
    }

    // Pack Status
    xbool PackActive = BitStream.ReadFlag() ;

    // Type
    s32 PackType ;
    BitStream.ReadRangedS32(PackType, INVENT_TYPE_START, INVENT_TYPE_END) ;

    // Update player vars ( call functions so that sounds get updated etc)
    SetPackCurrentType((invent_type)PackType) ;
    SetPackActive(PackActive) ;

    // Weapon targetID
    BitStream.ReadObjectID(m_WeaponTargetID) ;

    // Read missile vars
    m_MissileLock             = BitStream.ReadFlag() ;
    m_MissileTargetTrackCount = BitStream.ReadFlag() ;
    m_MissileTargetLockCount  = BitStream.ReadFlag() ;

    // Armed 
    m_Armed = BitStream.ReadFlag() ;

    // Animation
    s32 AnimIndex ;
    f32 AnimFrame ;
    BitStream.ReadRangedS32 (AnimIndex, -1, 60) ;
    BitStream.ReadRangedF32 (AnimFrame, 16, 0, 400) ;

    // Set the anim
    m_PlayerInstance.SetAnimByIndex(AnimIndex, 0.0f) ;  // No blend!
    m_PlayerInstance.GetCurrentAnimState().SetFrame(AnimFrame) ;

	// Exchange vars
	s32 PickupKind ;
	BitStream.ReadRangedS32(PickupKind, pickup::KIND_NONE, pickup::KIND_TOTAL-1) ;
	m_ExchangePickupKind = (pickup::kind)PickupKind ;

    // Vote vars
    m_VoteCanStart = BitStream.ReadFlag() ;     // Flags that player can start a vote
    m_VoteCanCast  = BitStream.ReadFlag() ;     // Flags that player can cast a vote

    // Misc
    BitStream.ReadF32(m_TVRefreshRate) ;

    // Grab info from player and update
	CalcBounds() ;

	// Setup contact info ready move/physics
	SetupContactInfo() ;
}

//==============================================================================

// On server...
void player_object::OnProvideInit( bitstream& BitStream )
{
    //x_DebugMsg("On Provide Init %1d\n",m_ObjectID);
	
    // Attributes
    BitStream.WriteU32(m_AttrBits) ;

    // Energy
    BitStream.WriteRangedF32( m_Energy, INPUT_PLAYER_ENERGY_BITS, 0, PLAYER_ENERGY_MAX );

    // Health
    if (BitStream.WriteFlag(m_Health > 0))
        BitStream.WriteRangedF32( m_Health, INPUT_PLAYER_HEALTH_BITS, 0, PLAYER_HEALTH_MAX );

    // World info
    BitStream.WriteVector(m_WorldPos) ;
	BitStream.WriteVector(m_GhostCompPos) ;
	BitStream.WriteRadian3(m_Rot) ;
	
    // Character info
    BitStream.WriteS32(m_CharacterType) ;
	BitStream.WriteS32(m_ArmorType) ;
	BitStream.WriteS32(m_SkinType) ;
	BitStream.WriteS32(m_VoiceType) ;
    BitStream.WriteS32(m_PlayerIndex) ;

    // Loadouts
    m_Loadout.Write(BitStream) ;
    BitStream.WriteS32(m_DefaultLoadoutType) ;
    m_InventoryLoadout.Write(BitStream) ;

    // This is what the will have after reading the above data
    m_ClientLoadout = m_Loadout ;

    // Weapon info
    BitStream.WriteRangedS32(m_WeaponCurrentType, INVENT_TYPE_WEAPON_START, INVENT_TYPE_WEAPON_END) ;
    BitStream.WriteRangedS32(m_WeaponCurrentSlot, 0, 5) ;
    BitStream.WriteRangedS32(m_WeaponState, WEAPON_STATE_START, WEAPON_STATE_END) ;

    // Vehicle info
    BitStream.WriteObjectID(m_VehicleID) ;
    BitStream.WriteS32(m_VehicleSeat) ;

    // Player info
	BitStream.WriteRangedS32(m_PlayerState, PLAYER_STATE_START, PLAYER_STATE_END) ;
    BitStream.WriteWString( m_Name );
    BitStream.WriteU32( m_TeamBits );

    // Send flag status
    BitStream.WriteS32( m_FlagCount, 10 );
    if (BitStream.WriteFlag(m_FlagInstance.GetShape() != NULL))
        BitStream.WriteRangedS32((s32)m_FlagInstance.GetTextureFrame(), PLAYER_MIN_FLAG_TEXTURE, PLAYER_MAX_FLAG_TEXTURE) ;  // Write texture #

    // Send glow status
    if( BitStream.WriteFlag( m_Glow ) )
        BitStream.WriteRangedF32( m_GlowRadius, 8, 0.0f, 10.0f );

    // Pack
    BitStream.WriteFlag(m_PackActive) ;
    BitStream.WriteRangedS32(m_PackCurrentType, INVENT_TYPE_START, INVENT_TYPE_END) ;

    // Weapon targetID
    BitStream.WriteObjectID(m_WeaponTargetID) ;

    // Write missile vars
    BitStream.WriteFlag( m_MissileLock ) ;
    BitStream.WriteFlag( m_MissileTargetTrackCount > 0 ) ;
    BitStream.WriteFlag( m_MissileTargetLockCount > 0 ) ;

    // Armed
    BitStream.WriteFlag(m_Armed) ;

    // Animation
    BitStream.WriteRangedS32(m_PlayerInstance.GetCurrentAnimState().GetAnim()->GetIndex(), -1, 60) ;
    BitStream.WriteRangedF32(m_PlayerInstance.GetCurrentAnimState().GetFrame(), 16, 0, 400) ;

	// Exchange vars
	BitStream.WriteRangedS32(m_ExchangePickupKind, pickup::KIND_NONE, pickup::KIND_TOTAL-1) ;

    // Vote vars
    BitStream.WriteFlag(m_VoteCanStart) ;   // Flags that player can start a vote
    BitStream.WriteFlag(m_VoteCanCast) ;    // Flags that player can cast a vote

    // Misc
    BitStream.WriteF32(m_TVRefreshRate) ;
}

//==============================================================================

void player_object::SetMoveManager( move_manager* pMM )
{
    m_pMM = pMM;
}

//==============================================================================

// On server...
void player_object::ReceiveMove( player_object::move& Move, s32 MoveSeq)
{
    UpdateNetSamples(0) ;

    // Calculate current draw pos incase of blending...
    vector3 DrawPos               = m_WorldPos        + (m_BlendMove.Blend * m_BlendMove.DeltaPos) ;
    radian3 DrawRot               = m_Rot             + (m_BlendMove.Blend * m_BlendMove.DeltaRot) ;
    radian  DrawViewFreeLookYaw   = m_ViewFreeLookYaw + (m_BlendMove.Blend * m_BlendMove.DeltaViewFreeLookYaw) ;

    // Monkey mode test....
    if( Move.MonkeyMode && (GetInventCount(INVENT_TYPE_WEAPON_AMMO_DISK)==0))
        AddInvent(INVENT_TYPE_WEAPON_AMMO_DISK, 15) ;

    //=================================================================
    // Clear disable keys
    //=================================================================

    // Movement keys
    m_DisabledInput.FireKey             = (m_DisabledInput.FireKey)     && (Move.FireKey) ;
    m_DisabledInput.JetKey              = (m_DisabledInput.JetKey)      && (Move.JetKey) ;
    m_DisabledInput.JumpKey             = (m_DisabledInput.JumpKey)     && (Move.JumpKey) ;
    m_DisabledInput.FreeLookKey         = (m_DisabledInput.FreeLookKey) && (Move.FreeLookKey) ;
                                            
    // Change keys                      
    m_DisabledInput.NextWeaponKey       = (m_DisabledInput.NextWeaponKey) && (Move.NextWeaponKey) ;
    m_DisabledInput.PrevWeaponKey       = (m_DisabledInput.PrevWeaponKey) && (Move.PrevWeaponKey) ;
                                            
    // Use keys                         
    m_DisabledInput.MineKey             = (m_DisabledInput.MineKey)      && (Move.MineKey) ;
    m_DisabledInput.PackKey             = (m_DisabledInput.PackKey)      && (Move.PackKey) ;
    m_DisabledInput.GrenadeKey          = (m_DisabledInput.GrenadeKey)   && (Move.GrenadeKey) ;
    m_DisabledInput.HealthKitKey        = (m_DisabledInput.HealthKitKey) && (Move.HealthKitKey) ;
                                            
    // Drop keys                        
    m_DisabledInput.DropWeaponKey       = (m_DisabledInput.DropWeaponKey) && (Move.DropWeaponKey) ;
    m_DisabledInput.DropPackKey         = (m_DisabledInput.DropPackKey)   && (Move.DropPackKey) ;
    m_DisabledInput.DropBeaconKey       = (m_DisabledInput.DropBeaconKey) && (Move.DropBeaconKey) ;
    m_DisabledInput.DropFlagKey         = (m_DisabledInput.DropFlagKey)   && (Move.DropFlagKey) ;
                                        
    // Special keys                 
    m_DisabledInput.SuicideKey          = (m_DisabledInput.SuicideKey)        && (Move.SuicideKey) ;
    m_DisabledInput.TargetingLaserKey   = (m_DisabledInput.TargetingLaserKey) && (Move.TargetingLaserKey) ;

    //=================================================================
    // Clear keys that are disabled
    //=================================================================

    // Movement keys
    Move.FireKey                        = (!m_DisabledInput.FireKey)            && (Move.FireKey) ;
    Move.JetKey                         = (!m_DisabledInput.JetKey)             && (Move.JetKey) ;
    Move.JumpKey                        = (!m_DisabledInput.JumpKey)            && (Move.JumpKey) ;
    Move.FreeLookKey                    = (!m_DisabledInput.FreeLookKey)        && (Move.FreeLookKey) ;
                                                                                   
    // Change keys                                                                 
    Move.NextWeaponKey                  = (!m_DisabledInput.NextWeaponKey)      && (Move.NextWeaponKey) ;
    Move.PrevWeaponKey                  = (!m_DisabledInput.PrevWeaponKey)      && (Move.PrevWeaponKey) ;
                                                                                   
    // Use keys                                                                    
    Move.MineKey                        = (!m_DisabledInput.MineKey)            && (Move.MineKey) ;
    Move.PackKey                        = (!m_DisabledInput.PackKey)            && (Move.PackKey) ;
    Move.GrenadeKey                     = (!m_DisabledInput.GrenadeKey)         && (Move.GrenadeKey) ;
    Move.HealthKitKey                   = (!m_DisabledInput.HealthKitKey)       && (Move.HealthKitKey) ;
                                                                                   
    // Drop keys                                                                   
    Move.DropWeaponKey                  = (!m_DisabledInput.DropWeaponKey)      && (Move.DropWeaponKey) ;
    Move.DropPackKey                    = (!m_DisabledInput.DropPackKey)        && (Move.DropPackKey) ;
    Move.DropBeaconKey                  = (!m_DisabledInput.DropBeaconKey)      && (Move.DropBeaconKey) ;
    Move.DropFlagKey                    = (!m_DisabledInput.DropFlagKey)        && (Move.DropFlagKey) ;
                                                   
    // Special keys                            
    Move.SuicideKey                     = (!m_DisabledInput.SuicideKey)         && (Move.SuicideKey) ;
    Move.TargetingLaserKey              = (!m_DisabledInput.TargetingLaserKey)  && (Move.TargetingLaserKey) ;

    // Always free looking when dead
    if (IsDead())
        Move.FreeLookKey = TRUE ;

    // Use move
    m_Move = Move ;
    m_LastMoveReceived = MoveSeq;

	// Apply move
    m_FrameRateDeltaTime = m_Move.DeltaTime ;
    ApplyMove();
    ApplyPhysics() ;
    m_FrameRateDeltaTime = 0.0f ;

    // Update
    ObjMgr.UpdateObject( this );

    // Blend to new info if not too far away
    if(     (PLAYER_SERVER_BLENDING)
        &&  (m_PlayerState != PLAYER_STATE_CONNECTING)
        &&  (m_PlayerState != PLAYER_STATE_WAIT_FOR_GAME)
        &&  (m_NetSampleAverage.RecieveDeltaTime != 0)
        &&  ((m_WorldPos - DrawPos).LengthSquared() <= (PLAYER_BLEND_DIST_MAX * PLAYER_BLEND_DIST_MAX)) )
	{
        // Blend from last pos and rot
        m_BlendMove.DeltaPos = DrawPos - m_WorldPos ;
        m_BlendMove.DeltaRot = DrawRot - m_Rot ;
        m_BlendMove.DeltaRot.ModAngle2() ;

        m_BlendMove.DeltaViewFreeLookYaw = x_ModAngle2(DrawViewFreeLookYaw - m_ViewFreeLookYaw) ;


        // Blend using just a tad more then the average recieve time
        // so that the server player always has something to aim for 
        f32 Time = m_NetSampleAverage.RecieveDeltaTime * PLAYER_SERVER_BLEND_TIME_SCALER ;
        if (Time > PLAYER_SERVER_BLEND_TIME_MAX)
            Time = PLAYER_SERVER_BLEND_TIME_MAX ;

        // Start the blend from the last pos
        m_BlendMove.Blend    = 1;
        m_BlendMove.BlendInc = -1.0f / Time ;

        // On vehicle?
        vehicle* Vehicle = IsAttachedToVehicle() ;
        if (Vehicle)
        {   
            // Don't blend if player is fixed to vehicle
            const vehicle::seat& Seat = Vehicle->GetSeat(m_VehicleSeat) ;
            if (!Seat.CanLookAndShoot)
                m_BlendMove.DeltaRot.Zero() ;

            // Never blend pos since vehicle does it's own blending
            m_BlendMove.DeltaPos.Zero() ;
        }
    }
    else
    {
        m_BlendMove.Blend = 0.0f ;  // No blend
        m_BlendMove.BlendInc = 0.0f ;
    }

    // Make sure ghost compression pos is up to date
    UpdateGhostCompPos() ;
}

//==============================================================================


