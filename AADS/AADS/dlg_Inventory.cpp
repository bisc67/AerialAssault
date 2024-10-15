//=========================================================================
//
//  dlg_inventory.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "globals.hpp"
#include "fe_globals.hpp"
#include "GameMgr\GameMgr.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_frame.hpp"

#include "dlg_Inventory.hpp"

#include "StringMgr\StringMgr.hpp"
#include "data\ui\ui_strings.h"

//=========================================================================
//  Inventory Dialog
//=========================================================================

enum controls
{
    IDC_ARMOR,
    IDC_1,
    IDC_2,
    IDC_3,
    IDC_4,
    IDC_5,
    IDC_FRAME1,
    IDC_FRAME2,
    IDC_WEAPON1,
    IDC_WEAPON2,
    IDC_WEAPON3,
    IDC_WEAPON4,
    IDC_WEAPON5,
    IDC_PACK,
    IDC_GRENADE
};

ui_manager::control_tem InventoryControls[] =
{
    { IDC_ARMOR,    IDS_ARMOR,      "combo",  13,  12, 140,  19, 0, 0, 1, 1, ui_win::WF_VISIBLE },

    { IDC_1,        IDS_NULL,       "check",  13,  37, 140,  24, 0, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_2,        IDS_NULL,       "check",  13,  64, 140,  24, 0, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_3,        IDS_NULL,       "check",  13,  91, 140,  24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_4,        IDS_NULL,       "check",  13, 118, 140,  24, 0, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_5,        IDS_NULL,       "check",  13, 145, 140,  24, 0, 5, 1, 2, ui_win::WF_VISIBLE },

    { IDC_FRAME1,   IDS_NULL,       "frame",   6,   4, 154, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_FRAME2,   IDS_NULL,       "frame", 165,   4, 309, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_WEAPON1,  IDS_WEAPON1,    "combo", 180,   8, 281,  19, 1, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON2,  IDS_WEAPON2,    "combo", 180,  32, 281,  19, 1, 1, 1, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON3,  IDS_WEAPON3,    "combo", 180,  56, 281,  19, 1, 2, 1, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON4,  IDS_WEAPON4,    "combo", 180,  80, 281,  19, 1, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON5,  IDS_WEAPON5,    "combo", 180, 104, 281,  19, 1, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_PACK,     IDS_PACK,       "combo", 180, 128, 281,  19, 1, 5, 1, 1, ui_win::WF_VISIBLE },
    { IDC_GRENADE,  IDS_GRENADE,    "combo", 180, 152, 281,  19, 1, 6, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem InventoryDialog =
{
    IDS_NULL,
    2, 7,
    sizeof(InventoryControls)/sizeof(ui_manager::control_tem),
    &InventoryControls[0],
    0
};

//=========================================================================
//  Defines
//=========================================================================

//=========================================================================
//  Structs
//=========================================================================

//=========================================================================
//  Data
//=========================================================================

xbool s_InventoryDirty = FALSE;
xbool s_Inventory2Dirty = FALSE;

//=========================================================================
//  Registration function
//=========================================================================

void dlg_inventory_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "inventory", &InventoryDialog, &dlg_inventory_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_inventory_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_inventory* pDialog = new dlg_inventory;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_inventory
//=========================================================================

dlg_inventory::dlg_inventory( void )
{
}

//=========================================================================

dlg_inventory::~dlg_inventory( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_inventory::Create( s32                        UserID,
                             ui_manager*                pManager,
                             ui_manager::dialog_tem*    pDialogTem,
                             const irect&               Position,
                             ui_win*                    pParent,
                             s32                        Flags,
                             void*                      pUserData )
{
    xbool   Success = FALSE;

    (void)pUserData;

    ASSERT( pManager );

    // Do dialog creation
    Success = ui_dialog::Create( UserID, pManager, pDialogTem, Position, pParent, Flags );

    m_BlockNotify = 1;

    // Get Pointers to all controls
    m_pFavArmor  = (ui_combo*)FindChildByID( IDC_ARMOR );
    m_pFav[0]    = (ui_check*)FindChildByID( IDC_1 );
    m_pFav[1]    = (ui_check*)FindChildByID( IDC_2 );
    m_pFav[2]    = (ui_check*)FindChildByID( IDC_3 );
    m_pFav[3]    = (ui_check*)FindChildByID( IDC_4 );
    m_pFav[4]    = (ui_check*)FindChildByID( IDC_5 );

    m_pFrame2    = (ui_frame*)FindChildByID( IDC_FRAME2 );

    m_pWeapon[0] = (ui_combo*)FindChildByID( IDC_WEAPON1 );
    m_pWeapon[1] = (ui_combo*)FindChildByID( IDC_WEAPON2 );
    m_pWeapon[2] = (ui_combo*)FindChildByID( IDC_WEAPON3 );
    m_pWeapon[3] = (ui_combo*)FindChildByID( IDC_WEAPON4 );
    m_pWeapon[4] = (ui_combo*)FindChildByID( IDC_WEAPON5 );
    m_pPack      = (ui_combo*)FindChildByID( IDC_PACK );
    m_pGrenade   = (ui_combo*)FindChildByID( IDC_GRENADE );

    // Get the Loadout for the first light favorite
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( UserID );
    ASSERT( pPlayer );
    m_WarriorID = pPlayer->GetWarriorID();
    ASSERT( (m_WarriorID == 0) || (m_WarriorID == 1) || (m_WarriorID == 2) );
    m_pWarriorSetup = &fegl.WarriorSetups[m_WarriorID];

    // Initialize Controls
    m_pFavArmor->AddItem( "Light" );
    m_pFavArmor->AddItem( "Medium" );
    m_pFavArmor->AddItem( "Heavy" );

    m_pWeapon[0]->SetLabelWidth( 90 );
    m_pWeapon[1]->SetLabelWidth( 90 );
    m_pWeapon[2]->SetLabelWidth( 90 );
    m_pWeapon[3]->SetLabelWidth( 90 );
    m_pWeapon[4]->SetLabelWidth( 90 );
    m_pPack     ->SetLabelWidth( 90 );
    m_pGrenade  ->SetLabelWidth( 90 );

    // Move the 1st favorite to the controls
    m_pFavArmor->SetSelection( m_pWarriorSetup->ActiveFavoriteArmor );
    m_pLoadout = &m_pWarriorSetup->Favorites[m_pWarriorSetup->ActiveFavorite[m_pWarriorSetup->ActiveFavoriteArmor]];
    LoadoutToControls();

    m_BlockNotify = 0;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_inventory::Destroy( void )
{
    // If the loadout has changed then update it in the player
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );
//TODO    if( pPlayer->GetInventoryLoadout() != m_Loadout )
    {
        // Set the loadout to the player
        pPlayer->SetInventoryLoadout( *m_pLoadout );
    }

    // Destroy the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_inventory::Render( s32 ox, s32 oy )
{
    // Enable weapon controls
    m_pWeapon[0]->SetFlags( m_pWeapon[0]->GetFlags() | ui_win::WF_VISIBLE );
    m_pWeapon[1]->SetFlags( m_pWeapon[1]->GetFlags() | ui_win::WF_VISIBLE );
    m_pWeapon[2]->SetFlags( m_pWeapon[2]->GetFlags() | ui_win::WF_VISIBLE );
    m_pWeapon[3]->SetFlags( m_pWeapon[3]->GetFlags() | ui_win::WF_VISIBLE );
    m_pWeapon[4]->SetFlags( m_pWeapon[4]->GetFlags() | ui_win::WF_VISIBLE );
    
    // HACK - SULTAN.
    if( m_pPack->GetSelection() == -1 )
    {
        m_pPack->SetSelection( 1 );
        ControlsToLoadout();
    }    
    m_pPack     ->SetFlags( m_pPack     ->GetFlags() | ui_win::WF_VISIBLE );
    m_pGrenade  ->SetFlags( m_pGrenade  ->GetFlags() | ui_win::WF_VISIBLE );

    // Render eveything
    ui_dialog::Render( ox, oy );
}

//=========================================================================

void dlg_inventory::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    if( m_BlockNotify == 0 )
    {
        m_BlockNotify++;
        s32 Index = -1;
        
        // Get the latest controls from the fegl.
        if( m_pFavArmor->GetSelection() != m_pWarriorSetup->ActiveFavoriteArmor )
        {
            Index = m_pWarriorSetup->ActiveFavorite[m_pFavArmor->GetSelection()];
            
            // Make sure that is index is between 0 - 4.
            if( (Index >= 5) && (Index <= 9) )
                Index -= FE_ARMOR_FAVORITES;
            else if( (Index >= 10) && (Index <= 14) )
                Index -= (FE_ARMOR_FAVORITES*2);
            
            ASSERT( (Index >= 0) && (Index <= 4) );
        }

        // Was this from one of the favorites, if so, switch favorite
        if( (s32)pSender == (s32)m_pFav[0] )
        {
            Index = 0;
            
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
        else if( (s32)pSender == (s32)m_pFav[1] )
        {            
            Index = 1;

            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
        else if( (s32)pSender == (s32)m_pFav[2] )
        {
            Index = 2;

            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
        else if( (s32)pSender == (s32)m_pFav[3] )
        {
            Index = 3;

            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
        else if( (s32)pSender == (s32)m_pFav[4] )
        {
            Index = 4;

            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }

        // New loadout slection?
        if( Index != -1 )
        {
            // Get Pointer
            switch( m_pFavArmor->GetSelection() )
            {
            case 0:
                m_pLoadout = &m_pWarriorSetup->Favorites[Index];
                m_pWarriorSetup->ActiveFavorite[0] = Index;
                break;
            case 1:
                m_pLoadout = &m_pWarriorSetup->Favorites[FE_ARMOR_FAVORITES+Index];
                m_pWarriorSetup->ActiveFavorite[1] = FE_ARMOR_FAVORITES+Index;
                break;
            case 2:
                m_pLoadout = &m_pWarriorSetup->Favorites[FE_ARMOR_FAVORITES*2+Index];
                m_pWarriorSetup->ActiveFavorite[2] = FE_ARMOR_FAVORITES*2+Index;
                break;
            default:
                ASSERT( 0 );
            }

        }
        else
        {
            // Convert controls to loadout structure
            ControlsToLoadout();
            LoadoutToControls();
        }

        // Convert loadout to controls
        LoadoutToControls();
        
        m_BlockNotify--;
    }
}

//=========================================================================

void dlg_inventory::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // If at the top of the dialog then a move up will move back to the tabbed dialog
    if( (Code == ui_manager::NAV_UP) && (m_NavY == 0) )
    {
        if( m_pParent )
        {
            ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
            audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
        }
    }
    else
    {
        // Check for special cases of navigating to disabled controls
        if( Code == ui_manager::NAV_RIGHT )
        {
            // Get Current Control
            ui_manager::user*   pUser = m_pManager->GetUser( m_UserID );
            ASSERT( pUser );
            ui_win* pWin = pUser->pLastWindowUnderCursor;
            if( (pWin == m_pFav[2]) && (m_pWeapon[3]->GetFlags() & WF_DISABLED) )
            {
                GotoControl( m_pWeapon[2] );
            }
            else
            if( (pWin == m_pFav[3]) && (m_pWeapon[4]->GetFlags() & WF_DISABLED) )
            {
                GotoControl( m_pPack );
            }
            else
            {
                // Call the navigation function
                ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
            }
        }
        else
        {
            // Call the navigation function
            ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
        }
    }
}

//=========================================================================

void dlg_inventory::OnPadSelect( ui_win* pWin )
{
    (void)pWin;
}

//=========================================================================

void dlg_inventory::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }

        // Call default handler
//        ui_dialog::OnPadBack( pWin );
}

//=========================================================================

void dlg_inventory::SetupWeaponCombo( ui_combo* pCombo, const player_object::move_info& MoveInfo )
{
    AddWeaponToCombo( pCombo, "Plasma Rifle"     ,(s32)player_object::INVENT_TYPE_WEAPON_PLASMA_GUN        ,MoveInfo );
    AddWeaponToCombo( pCombo, "Spinfusor"        ,(s32)player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER     ,MoveInfo );
    AddWeaponToCombo( pCombo, "Grenade Launcher" ,(s32)player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER  ,MoveInfo );
    AddWeaponToCombo( pCombo, "Fusion Mortar"    ,(s32)player_object::INVENT_TYPE_WEAPON_MORTAR_GUN        ,MoveInfo );
    AddWeaponToCombo( pCombo, "Missile Launcher" ,(s32)player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER  ,MoveInfo );
    AddWeaponToCombo( pCombo, "Blaster"          ,(s32)player_object::INVENT_TYPE_WEAPON_BLASTER           ,MoveInfo );
    AddWeaponToCombo( pCombo, "Chaingun"         ,(s32)player_object::INVENT_TYPE_WEAPON_CHAIN_GUN         ,MoveInfo );
    AddWeaponToCombo( pCombo, "Laser Rifle"      ,(s32)player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE      ,MoveInfo );
}

//=========================================================================

void dlg_inventory::AddWeaponToCombo( ui_combo* pCombo, const char* Name, s32 Type, const player_object::move_info& MoveInfo )
{
    if( MoveInfo.INVENT_MAX[Type] > 0 )
    {
//        s32 iItem = pCombo->AddItem( Name, Type );
        pCombo->AddItem( Name, Type );
    }
//    pCombo->SetItemEnabled( iItem, MoveInfo.INVENT_MAX[Type] > 0 );
}

//=========================================================================

void dlg_inventory::RemoveWeaponFromCombo( ui_combo* pCombo, s32 Type )
{
    s32 Index = pCombo->FindItemByData( Type );
    if( Index != -1 )
        pCombo->DeleteItem( Index );
}

//=========================================================================

void dlg_inventory::SetupPackCombo( ui_combo* pCombo, const player_object::move_info& MoveInfo )
{
    struct pack_info
    {
        const char* Name;
        s32         Type;
        s32         BanList;
    };

    pack_info Packs[] =
    {
        { "Ammo Pack",         player_object::INVENT_TYPE_ARMOR_PACK_AMMO,              0 },
        { "Energy Pack",       player_object::INVENT_TYPE_ARMOR_PACK_ENERGY,            0 },
        { "Repair Pack",       player_object::INVENT_TYPE_ARMOR_PACK_REPAIR,            0 },
        { "Shield Pack",       player_object::INVENT_TYPE_ARMOR_PACK_SHIELD,            0 },

        { "Satchel Charge",    player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE,   0 },
        { "Remote Station",    player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION,   0 },

        { "Remote Turret",     player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR,    1 },
        { "Remote Sensors",    player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR,     1 },

        { "AA Barrel",         player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_AA,        1 },
        { "Mortar Barrel",     player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR,    1 },
        { "Missile Barrel",    player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE,   1 },
        { "Plasma Barrel",     player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA,    1 },
//      { "ELF Barrel",        player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_ELF,       1 },
    };

    xbool UseBan = FALSE;

    game_type GameType = GameMgr.GetGameType();
    if( (GameType == GAME_DM)  ||
        (GameType == GAME_HUNTER) )
    {
        UseBan = TRUE;
    }

    // Add all the packs
    for( s32 i=0 ; i<(s32)(sizeof(Packs)/sizeof(pack_info)) ; i++ )
    {
        if( MoveInfo.INVENT_MAX[Packs[i].Type] > 0 )
        {
//            if( (Packs[i].BanList == 0) || !UseBan )
            {
                // Add pack
                s32 iItem = pCombo->AddItem( Packs[i].Name, Packs[i].Type );
                pCombo->SetItemEnabled( iItem, (Packs[i].BanList == 0) || !UseBan );

                // Select it if the loadout has one of these packs
                if( m_pLoadout->Inventory.Counts[Packs[i].Type] > 0 )
                    pCombo->SetSelection( pCombo->GetItemCount()-1 );
            }
        }
    }
}

//=========================================================================

void dlg_inventory::LoadoutToControls( void )
{
    s32     i;
    s32     j;

    // Get player info
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );

    // Get the moveinfo for the character type and armor type we have
    player_object::armor_type   ArmorType = m_pLoadout->Armor;
    const player_object::move_info& MoveInfo = pPlayer->GetMoveInfo( pPlayer->GetCharacterType(), ArmorType );

    // Set Armor
    xwchar ArmorString[32];
    s32 iArmor = m_pFavArmor->GetSelection();
    
    // Set the armor index.
    if( iArmor != -1 )
        m_pWarriorSetup->ActiveFavoriteArmor = iArmor;

    player_object::loadout* pFavArray = m_pWarriorSetup->Favorites;
    m_pFavArmor->SetSelection( iArmor );
    if( iArmor == 0 )
    {
        x_wstrcpy( ArmorString, StringMgr( "ui", IDS_FAVORITE_LIGHT ) );
        pFavArray = &m_pWarriorSetup->Favorites[0];
    }
    else if( iArmor == 1 )
    {
        x_wstrcpy( ArmorString, StringMgr( "ui", IDS_FAVORITE_MEDIUM ) );
        pFavArray = &m_pWarriorSetup->Favorites[FE_ARMOR_FAVORITES];
    }
    else if( iArmor == 2 )
    {
        x_wstrcpy( ArmorString, StringMgr( "ui", IDS_FAVORITE_HEAVY ) );
        pFavArray = &m_pWarriorSetup->Favorites[FE_ARMOR_FAVORITES*2];
    }

    // Relabel all the favorite buttons
    for( i=0 ; i<5 ; i++ )
    {
        xwchar Number = xwchar( '1' + i );
        s32    Len    = x_wstrlen( ArmorString );
        ArmorString[Len-1] = Number;
        switch( iArmor )
        {
            case 0:
                if( m_pWarriorSetup->ActiveFavorite[0] == i )
                {
                    m_pFav[i]->SetSelected( TRUE );
                }
                else
                {
                    m_pFav[i]->SetSelected( FALSE );
                }           
            break;
            case 1:
                if( (m_pWarriorSetup->ActiveFavorite[1]-FE_ARMOR_FAVORITES) == i )
                {
                    m_pFav[i]->SetSelected( TRUE );
                }
                // This is only going to happen when this dialog box is opened for the first time.
                else if( m_pWarriorSetup->ActiveFavorite[1] == i )
                {   
                    m_pFav[i]->SetSelected( TRUE );
                }   
                else
                {
                    m_pFav[i]->SetSelected( FALSE );
                }           

            break;
            case 2:
                s32 sub = FE_ARMOR_FAVORITES*2;
                if( (m_pWarriorSetup->ActiveFavorite[2] - sub) == i )
                {
                    m_pFav[i]->SetSelected( TRUE );
                }
                // This is only going to happen when this dialog box is opened for the first time.
                else if( m_pWarriorSetup->ActiveFavorite[2] == i )
                {   
                    m_pFav[i]->SetSelected( TRUE );
                }   
                else
                {
                    m_pFav[i]->SetSelected( FALSE );
                }           
            break;
        };
        
        m_pFav[i]->SetLabel( ArmorString );
    }

    // Select weapons currently in use
    for( i=0 ; i<5 ; i++ )
    {
        // Clear out the slot
        m_pWeapon[i]->DeleteAllItems();

        // Check if this is a valid slot for this configuration
        if( i < m_pLoadout->NumWeapons )
        {
            // Enable weapon slot
            m_pWeapon[i]->SetFlags( m_pWeapon[i]->GetFlags() & ~ui_win::WF_DISABLED );

            // Fill with Weapons
            SetupWeaponCombo( m_pWeapon[i], MoveInfo );

            // Remove the weapons in use by other slots
            for( j=0 ; j<m_pLoadout->NumWeapons ; j++ )
            {
                if( (j!=i) )
                    RemoveWeaponFromCombo( m_pWeapon[i], m_pLoadout->Weapons[j] );
            }

            // Select weapon for slot
            m_pWeapon[i]->SetSelection( m_pWeapon[i]->FindItemByData( (s32)m_pLoadout->Weapons[i] ) );
        }
        else
        {
            // Disable weapon slot
            m_pWeapon[i]->SetFlags( m_pWeapon[i]->GetFlags() | ui_win::WF_DISABLED );
        }
    }

    // Set Packs
    m_pPack->DeleteAllItems();
    SetupPackCombo( m_pPack, MoveInfo );

    // Set Grenades
    m_pGrenade->DeleteAllItems();
    m_pGrenade->AddItem( "Basic Grenade",      (s32)player_object::INVENT_TYPE_GRENADE_BASIC      );
//  m_pGrenade->AddItem( "Flash Grenade",      (s32)player_object::INVENT_TYPE_GRENADE_FLASH      );
    m_pGrenade->AddItem( "Concussion Grenade", (s32)player_object::INVENT_TYPE_GRENADE_CONCUSSION );
    m_pGrenade->AddItem( "Flare Grenade",      (s32)player_object::INVENT_TYPE_GRENADE_FLARE      );
    m_pGrenade->AddItem( "Mine Grenade",       (s32)player_object::INVENT_TYPE_MINE               );

    if( m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_GRENADE_BASIC     ] > 0 ) m_pGrenade->SetSelection( 0 );
//  if( m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_GRENADE_FLASH     ] > 0 ) m_pGrenade->SetSelection( 1 );
    if( m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_GRENADE_CONCUSSION] > 0 ) m_pGrenade->SetSelection( 1 );
    if( m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_GRENADE_FLARE     ] > 0 ) m_pGrenade->SetSelection( 2 );
    if( m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_MINE              ] > 0 ) m_pGrenade->SetSelection( 3 );
}

//=========================================================================

void dlg_inventory::ControlsToLoadout( void )
{
    s32     i;

    // Get the move info for this character type
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );

    // Get the moveinfo for the character type and armor type we have
    player_object::armor_type   ArmorType = m_pLoadout->Armor;
    const player_object::move_info& MoveInfo = pPlayer->GetMoveInfo( pPlayer->GetCharacterType(), ArmorType );

    // Set Pack
    if( m_pPack->GetSelection() != -1 )
    {
        s32 Type = m_pPack->GetSelectedItemData();
        if( m_pLoadout->Inventory.Counts[Type] != MoveInfo.INVENT_MAX[Type] )
        {
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
    }

    // Set Grenades
    if( m_pGrenade->GetSelection() != -1 )
    {
        s32 Type = m_pGrenade->GetSelectedItemData();
        if( m_pLoadout->Inventory.Counts[Type] != MoveInfo.INVENT_MAX[Type] )
        {
            if( m_WarriorID == 0 )
                s_InventoryDirty = TRUE;
            else
                s_Inventory2Dirty = TRUE;
        }
    }

    // Clear the inventory counts
    m_pLoadout->Inventory.Clear() ;

    // Read out weapons
    for( i=0 ; i<5 ; i++ )
    {
        // If the control has a weapon selected
        if( m_pWeapon[i]->GetSelection() != -1 )
        {
            // Set the weapon in the loadout
            player_object::invent_type WeaponType = (player_object::invent_type)m_pWeapon[i]->GetSelectedItemData();

            // Only setup the weapon if it's allowed
            if (MoveInfo.INVENT_MAX[WeaponType] > 0)
            {
                // Fill slot
                if( m_pLoadout->Weapons[i] != WeaponType )
                {
                    m_pLoadout->Weapons[i]                   = WeaponType;
                    if( m_WarriorID == 0 )
                        s_InventoryDirty = TRUE;
                    else
                        s_Inventory2Dirty = TRUE;
                }

                m_pLoadout->Inventory.Counts[WeaponType] = 1 ;

                // Check for sniper rifle to force an energy pack
                // JP: removed restriction that says you must equip an energy pack when using the sniper rifle
                //if( WeaponType == player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE )
                //{
                //    s32 iPack = m_pPack->FindItemByData( player_object::INVENT_TYPE_ARMOR_PACK_ENERGY );
                //    if( iPack != -1 )
                //        m_pPack->SetSelection( iPack );
                //}

                // Set the Ammo for the weapon
                const player_object::weapon_info& WeaponInfo = pPlayer->GetWeaponInfo( WeaponType );
                player_object::invent_type AmmoType   = WeaponInfo.AmmoType;
                m_pLoadout->Inventory.Counts[ AmmoType] = MoveInfo.INVENT_MAX[AmmoType] ;
            }
            else
            {
                // Weapon is not allowed, so clear the weapon slot
                m_pLoadout->Weapons[i] = player_object::INVENT_TYPE_NONE ;
            }
        }
    }

    // Set Pack
    if( m_pPack->GetSelection() != -1 )
    {
        s32 Type = m_pPack->GetSelectedItemData();
        m_pLoadout->Inventory.Counts[Type] = MoveInfo.INVENT_MAX[Type];
    }

    // Set Grenades
    if( m_pGrenade->GetSelection() != -1 )
    {
        s32 Type = m_pGrenade->GetSelectedItemData();
        m_pLoadout->Inventory.Counts[Type] = MoveInfo.INVENT_MAX[Type];
    }

    // Set Mines
//    m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_MINE] = MoveInfo.INVENT_MAX[player_object::INVENT_TYPE_MINE];

    // Set Healthkit
//    m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_BELT_GEAR_HEALTH_KIT] = 1;

    // Set Beacons
//    m_pLoadout->Inventory.Counts[player_object::INVENT_TYPE_BELT_GEAR_BEACON] = MoveInfo.INVENT_MAX[player_object::INVENT_TYPE_BELT_GEAR_BEACON];
}

//=========================================================================
