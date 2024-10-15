//=========================================================================
//
//  dlg_vehicle.cpp
//
//=========================================================================

#include "entropy.hpp"

#include "globals.hpp"
#include "fe_globals.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_tabbed_dialog.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_check.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_font.hpp"

#include "GameMgr\GameMgr.hpp"

#include "dlg_vehicle.hpp"

#include "fe_colors.hpp"

#include "data\ui\ui_strings.h"
#include "StringMgr\StringMgr.hpp"

//=========================================================================
//  Vehicle Dialog
//=========================================================================

enum controls
{
    IDC_WILDCAT,
    IDC_BEOWULF,
    IDC_JERICHO,
    IDC_SHRIKE,
    IDC_THUNDERSWORD,
    IDC_HAVOC,
    IDC_FRAME,
    IDC_FRAME2
};

ui_manager::control_tem VehicleControls[] =
{
    { IDC_WILDCAT,      IDS_WILDCAT,        "check",  13,  10, 140,  24, 0, 0, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SHRIKE,       IDS_SHRIKE,         "check",  13,  37, 140,  24, 0, 3, 1, 1, ui_win::WF_VISIBLE },
    { IDC_THUNDERSWORD, IDS_THUNDERSWORD,   "check",  13,  64, 140,  24, 0, 4, 1, 1, ui_win::WF_VISIBLE },
    { IDC_HAVOC,        IDS_HAVOC,          "check",  13,  91, 140,  24, 0, 5, 1, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME,        IDS_NULL,           "frame", 165,   4, 309, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_FRAME2,       IDS_NULL,           "frame",   6,   4, 154, 172, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
};

ui_manager::dialog_tem VehicleDialog =
{
    IDS_NULL,
    2, 6,
    sizeof(VehicleControls)/sizeof(ui_manager::control_tem),
    &VehicleControls[0],
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

//=========================================================================
//  Registration function
//=========================================================================

void dlg_vehicle_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "vehicle", &VehicleDialog, &dlg_vehicle_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_vehicle_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_vehicle* pDialog = new dlg_vehicle;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_vehicle
//=========================================================================

dlg_vehicle::dlg_vehicle( void )
{
}

//=========================================================================

dlg_vehicle::~dlg_vehicle( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_vehicle::Create( s32                        UserID,
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
    m_pVehicle[0] = (ui_check*)FindChildByID( IDC_WILDCAT );
    m_pVehicle[1] = (ui_check*)FindChildByID( IDC_SHRIKE );
    m_pVehicle[2] = (ui_check*)FindChildByID( IDC_THUNDERSWORD );
    m_pVehicle[3] = (ui_check*)FindChildByID( IDC_HAVOC );

    m_pFrame2    = (ui_frame* )FindChildByID( IDC_FRAME );

    // Get pointer to warrior setup
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( UserID );
    ASSERT( pPlayer );
    s32 WarriorID = pPlayer->GetWarriorID();
    ASSERT( (WarriorID == 0) || (WarriorID == 1) || (WarriorID == 2) );
    m_pWarriorSetup = &fegl.WarriorSetups[WarriorID];

    // Move the 1st favorite to the controls
    VehicleToControls();

    m_BlockNotify = 0;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_vehicle::Destroy( void )
{
/*
    // If the loadout has changed then update it in the player
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );
    {
        // Set the loadout to the player
//        pPlayer->SetInventoryLoadout( *m_pLoadout );
    }
*/
    // Destroy the dialog
    ui_dialog::Destroy();
}

//=========================================================================

void dlg_vehicle::Render( s32 ox, s32 oy )
{
    // We need some custom code here to render the vehicle

    // Exit if not visible
    if( !(m_Flags & WF_VISIBLE) )
        return;

    // Render eveything
    ui_dialog::Render( ox, oy );
    
    // The location of the frame.
    irect r = m_pFrame2->GetPosition();
    
    // Render the units count.
    irect UnitsRect = m_pVehicle[3]->GetPosition();
    UnitsRect.t = UnitsRect.b;
    UnitsRect.b = r.b;
    LocalToScreen( UnitsRect );

    // Render Vehicle
    r.Deflate( 4, 4 );
    LocalToScreen( r );

#ifdef TARGET_PC
/*    // Adjust the vehicles position according to the resolution.
    s32 XRes, YRes;
    eng_GetRes( XRes, YRes );
    s32 midX = XRes>>1;
    s32 midY = YRes>>1;

    s32 dx = midX - 256;
    s32 dy = midY - 256;
    r.Translate( dx, dy );
*/
#endif
    // Turn on Z Buffer
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBuffer(TRUE);
    gsreg_End();
#endif
#ifdef TARGET_PC
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
#endif

    // Render Shape
    object::type VehicleType = m_pWarriorSetup->Favorites[0].VehicleType;
    if( (VehicleType >= object::TYPE_VEHICLE_WILDCAT) && (VehicleType <= object::TYPE_VEHICLE_HAVOC) )
    {
        RenderVehicle( r, VehicleType );

        // Check the bounds for the text.
        irect bounds =  m_pManager->GetUserBounds( m_UserID );
        r.t = r.b - m_pManager->GetLineHeight( 1 );
        if( bounds.t > r.t )
        {
            r.Translate( 0, bounds.t );
            UnitsRect.Translate( 0, bounds.t );
        }

        // Get pointer to warrior setup
        player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
        ASSERT( pPlayer );
        
        s32 amount, current, max;
        GameMgr.GetDeployStats( VehicleType, pPlayer->GetTeamBits() , current, max );
        amount = max - current;

        // Don't let it go below zero.
        if( amount < 0 )
            amount = 0;
        
        byte buff[20];
        x_sprintf((char *)buff, "Units Available: %d", amount );
        xstring label = (char *)buff;
        switch( VehicleType )
        {
        case object::TYPE_VEHICLE_WILDCAT:
            m_pManager->RenderText( 1, r, ui_font::h_left, XCOLOR_WHITE, StringMgr( "ui", IDS_MONGOOSE ) );
            m_pManager->RenderText( 1, UnitsRect, ui_font::h_center | ui_font::v_center, XCOLOR_WHITE, label );
            break;
        case object::TYPE_VEHICLE_SHRIKE:
            m_pManager->RenderText( 1, r, ui_font::h_left, XCOLOR_WHITE, StringMgr( "ui", IDS_DINGO ) );
            m_pManager->RenderText( 1, UnitsRect, ui_font::h_center | ui_font::v_center, XCOLOR_WHITE, label );
            break;
        case object::TYPE_VEHICLE_THUNDERSWORD:
            m_pManager->RenderText( 1, r, ui_font::h_left, XCOLOR_WHITE, StringMgr( "ui", IDS_HAMMER ) );
            m_pManager->RenderText( 1, UnitsRect, ui_font::h_center | ui_font::v_center, XCOLOR_WHITE, label );
            break;
        case object::TYPE_VEHICLE_HAVOC:
            m_pManager->RenderText( 1, r, ui_font::h_left, XCOLOR_WHITE, StringMgr( "ui", IDS_PILTDOWN ) );
            m_pManager->RenderText( 1, UnitsRect, ui_font::h_center | ui_font::v_center, XCOLOR_WHITE, label );
            break;
        }
    }

}

//=========================================================================

void dlg_vehicle::RenderVehicle( irect Window, s32 VehicleType )
{
    // Preserve old view and setup warrior view
    view OldView = *eng_GetView( 0 );
    view WarriorView;
    WarriorView = OldView;

    const irect& br = m_pManager->GetUserBounds( m_UserID );
    Window.Translate( br.l, br.t );

    WarriorView.SetViewport( Window.l, Window.t, Window.r, Window.b );
    WarriorView.SetRotation( radian3( R_0, R_0, R_0 ) );
	WarriorView.SetXFOV( R_70 );
    eng_SetView( WarriorView, 0 );
    eng_ActivateView( 0 );

    // Setup Scissoring
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetScissor(Window.l,Window.t,Window.r,Window.b);
    eng_PushGSContext(1);
    gsreg_SetScissor(Window.l,Window.t,Window.r,Window.b);
    eng_PopGSContext();
    gsreg_End();
#endif

    draw_ClearZBuffer( Window );

    struct pos{ f32 x; f32 y; f32 z; };

    static shape_instance   Inst;
    static shape_instance   TurretInst;
    s32    ShapeType;
    static radian3          Rot(0.0f,0.0f,0.0f);
    static vector3          Pos;
    shape* Turret ;
    static pos              Positions[] = {
                                              { 0.0f, 0.0f,  5.0f },      // Wildcat
                                              { 0.0f, 1.0f, 12.0f },      // Beowulf
                                              { 0.0f, 1.0f, 16.0f },      // Jericho
                                              { 0.0f, 1.0f,  8.0f },      // Shrike
                                              { 0.0f, 1.0f, 10.0f },      // Thundersword
                                              { 0.0f, 0.5f, 12.0f },      // Havoc
                                          };

    // Setup instance
    {
        ShapeType = SHAPE_TYPE_VEHICLE_WILDCAT;

        // Get Shape Type to render vehicle
        switch( VehicleType )
        {
        case object::TYPE_VEHICLE_WILDCAT:
            ShapeType = SHAPE_TYPE_VEHICLE_WILDCAT;
            break;
        case object::TYPE_VEHICLE_BEOWULF:
            ShapeType = SHAPE_TYPE_VEHICLE_BEOWULF;
            break;
        case object::TYPE_VEHICLE_JERICHO2:
            ShapeType = SHAPE_TYPE_VEHICLE_JERICHO2;
            break;
        case object::TYPE_VEHICLE_SHRIKE:
            ShapeType = SHAPE_TYPE_VEHICLE_SHRIKE;
            break;
        case object::TYPE_VEHICLE_THUNDERSWORD:
            ShapeType = SHAPE_TYPE_VEHICLE_THUNDERSWORD;
            break;
        case object::TYPE_VEHICLE_HAVOC:
            ShapeType = SHAPE_TYPE_VEHICLE_HAVOC;
            break;
        }

        // Lookup useful info...
        shape* Shape = tgl.GameShapeManager.GetShapeByType( ShapeType );

        // Shapes not loaded yet?
        if( Shape == NULL )
            return;

        // Initialize
        if( Inst.GetShape() != Shape )
        {
            // Set shape
            Inst.SetShape( Shape );

            // Set light
            Inst.SetLightAmbientColor( FECOL_VEHICLE_LIGHT );
            Inst.SetLightColor( XCOLOR_WHITE );
            Inst.SetLightDirection( vector3(0,0,1), TRUE );

            // Set color
            Inst.SetColor( XCOLOR_WHITE );

            // Make the geometry use frame 0 reflection map
            Inst.SetTextureAnimFPS(0) ;
            Inst.SetTextureFrame(0) ;
        }

        // Transport needs turret and turret barrel to be setup
        if( ShapeType == SHAPE_TYPE_VEHICLE_HAVOC )
        {
            // Get turret
            Turret = tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_TURRET_TRANSPORT );
            if( Turret == NULL )
                return;

            // Initialize turret
            if( TurretInst.GetShape() != Turret )
            {
                // Set shape
                TurretInst.SetShape( Turret );

                // Set light
                TurretInst.SetLightAmbientColor( FECOL_VEHICLE_LIGHT );
                TurretInst.SetLightColor( XCOLOR_WHITE );
                TurretInst.SetLightDirection( vector3(0,0,1), TRUE );

                // Set color
                TurretInst.SetColor( XCOLOR_WHITE );

                // Make the geometry use frame 0 reflection map
                TurretInst.SetTextureAnimFPS(0);
                TurretInst.SetTextureFrame(0);

                // Set anim
                TurretInst.SetAnimByType( ANIM_TYPE_TURRET_ACTIVATE ) ;
            }
        }
    }

    // Put infront of camera
    const view  *View   = eng_GetView();
    view v = *View;
    v.SetRotation(radian3(R_20,0,0));
    eng_SetView( v, 0 );
    pos& p = Positions[VehicleType-object::TYPE_VEHICLE_WILDCAT];
    Pos.Set( p.x, p.y, p.z );
    vector3 ViewPos = v.ConvertV2W(Pos);
    Inst.SetPos(ViewPos);
    Inst.SetRot(Rot);

    // Spin
    Rot.Yaw += tgl.DeltaLogicTime * R_20;

    // Advance anim
    Inst.Advance(tgl.DeltaLogicTime);

    // Update turret position on havoc
    if( ShapeType == SHAPE_TYPE_VEHICLE_HAVOC )
    {
        //hot_point *HotPoint = TurretInst.GetHotPointByIndex( 0 );
        matrix4 Mat = Inst.GetHotPointL2W( HOT_POINT_TYPE_TURRET_MOUNT );
        TurretInst.SetRot(Mat.GetRotation()) ;
        TurretInst.SetPos(Mat.GetTranslation());
    }

    // Finally draw it
    shape::BeginDraw();
    Inst.Draw(shape::DRAW_FLAG_CLIP);
    
    // Draw turret?
    if( ShapeType == SHAPE_TYPE_VEHICLE_HAVOC )
        TurretInst.Draw(shape::DRAW_FLAG_CLIP);

    shape::EndDraw();

    // Restore old view
    eng_SetView( OldView, 0 );
    eng_ActivateView( 0 );

    // Setup Scissoring
#ifdef TARGET_PS2
    s32 X0,Y0,X1,Y1;
    OldView.GetViewport( X0,Y0,X1,Y1 );
    gsreg_Begin();
    gsreg_SetScissor(X0,Y0,X1,Y1);
    eng_PushGSContext(1);
    gsreg_SetScissor(X0,Y0,X1,Y1);
    eng_PopGSContext();
    gsreg_End();
#endif

}

//=========================================================================

void dlg_vehicle::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    // Convert controls to selected vehicle
    if( m_BlockNotify == 0 )
    {
        // Enable only the check that sent a message
        for( s32 i=0 ; i<4; i++ )
        {
            xbool Selected = (m_pVehicle[i] == pSender);
            m_pVehicle[i]->SetFlags( (m_pVehicle[i]->GetFlags() & ~WF_SELECTED) | (Selected ? WF_SELECTED : 0) );
        }

        m_BlockNotify++;
        ControlsToVehicle();
        VehicleToControls();
        m_BlockNotify--;
    }
}

//=========================================================================

void dlg_vehicle::OnPadNavigate( ui_win* pWin, s32 Code, s32 Presses, s32 Repeats )
{
    // Only allow up and down navigation
//    if( (Code == ui_manager::NAV_UP) || (Code == ui_manager::NAV_DOWN) )
//    {
        // If at the top of the dialog then a move up will move back to the tabbed dialog
        if( (Code == ui_manager::NAV_UP) && (m_NavX == 0) && (m_NavY == 0) )
        {
            if( m_pParent )
            {
                ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
                audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
            }
        }
        else
        {
            // Call the navigation function
            ui_dialog::OnPadNavigate( pWin, Code, Presses, Repeats );
        }
//    }
}

//=========================================================================

void dlg_vehicle::OnPadSelect( ui_win* pWin )
{
    ui_dialog::OnPadSelect( pWin );
}

//=========================================================================

void dlg_vehicle::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Go back to Tabbed Dialog parent
    if( m_pParent )
    {
        ((ui_tabbed_dialog*)m_pParent)->ActivateTab( this );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_vehicle::VehicleToControls( void )
{
    s32 Index=-1;
    s32 i;

/*
    // Get player info
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );
*/

    // Get control index for vehicle
    switch( m_pWarriorSetup->Favorites[0].VehicleType )
    {
    case object::TYPE_VEHICLE_WILDCAT:
        Index = 0; break;
    case object::TYPE_VEHICLE_SHRIKE:
        Index = 1; break;
    case object::TYPE_VEHICLE_THUNDERSWORD:
        Index = 2; break;
    case object::TYPE_VEHICLE_HAVOC:
        Index = 3; break;
    }

    // Setup controls
    for( i=0 ; i<4; i++ )
    {
        xbool Selected = (i == Index);
        m_pVehicle[i]->SetFlags( (m_pVehicle[i]->GetFlags() & ~WF_SELECTED) | (Selected ? WF_SELECTED : 0) );
    }
}

//=========================================================================

void dlg_vehicle::ControlsToVehicle( void )
{
    s32             Index = 0;
    s32             i;
    object::type    VehicleType = object::TYPE_VEHICLE_WILDCAT;

    // Get the move info for this character type
    player_object* pPlayer = (player_object*)m_pManager->GetUserData( m_UserID );
    ASSERT( pPlayer );

    // Find selected control
    for( i=0 ; i<4 ; i++ )
    {
        if( m_pVehicle[i]->GetFlags() & WF_SELECTED )
        {
            Index = i;
            break;
        }
    }

    // Get Object Type from control index
    switch( Index )
    {
    case 0:
        VehicleType = object::TYPE_VEHICLE_WILDCAT; break;
    case 1:
        VehicleType = object::TYPE_VEHICLE_SHRIKE; break;
    case 2:
        VehicleType = object::TYPE_VEHICLE_THUNDERSWORD; break;
    case 3:
        VehicleType = object::TYPE_VEHICLE_HAVOC; break;
    }

    // Set into all favorites
    for( i=0 ; i<FE_TOTAL_FAVORITES ; i++ )
    {
        m_pWarriorSetup->Favorites[i].VehicleType = VehicleType;
    }

    // Send the inventory update to the server
    player_object::loadout Loadout = pPlayer->GetInventoryLoadout();
    Loadout.VehicleType = VehicleType;
    pPlayer->SetInventoryLoadout( Loadout );
}

//=========================================================================

void SetVehicleButtonState( s32 StringID, xbool Enabled )
{
    s32 NumButtons = sizeof( VehicleControls ) / sizeof( VehicleControls[0] );
    
    for( s32 i=0; i<NumButtons; i++ )
    {
        if( VehicleControls[i].StringID == StringID )
        {
            if( Enabled == TRUE )
            {
                VehicleControls[i].Flags &= ~ui_win::WF_DISABLED;
            }
            else
            {
                VehicleControls[i].Flags |=  ui_win::WF_DISABLED;
            }
        }
    }
}

//=========================================================================

