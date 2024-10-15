//=========================================================================
//
//  dlg_warrior_setup.cpp
//
//=========================================================================

#include "entropy.hpp"
#include "globals.hpp"
#include "objects\player\PlayerObject.hpp"

#include "AudioMgr\audio.hpp"
#include "labelsets\Tribes2Types.hpp"

#include "ui\ui_manager.hpp"
#include "ui\ui_control.hpp"
#include "ui\ui_combo.hpp"
#include "ui\ui_edit.hpp"
#include "ui\ui_frame.hpp"
#include "ui\ui_button.hpp"

#include "dlg_WarriorSetup.hpp"
#include "dlg_Loadsave.hpp"
#include "fe_globals.hpp"

#include "fe_colors.hpp"

#include "StringMgr/StringMgr.hpp"

#include "data\ui\ui_strings.h"
#include "demo1/specialversion.hpp"

//=========================================================================
//  Warrior Setup Dialog
//=========================================================================

enum controls
{
    IDC_CONTINUE,
    IDC_FRAME1,
    IDC_FRAME2,
    IDC_FRAME3,
    IDC_NAME,
    IDC_GENDER,
    IDC_SKIN,
    IDC_VOICE,
    IDC_ARMOR,
    IDC_WEAPON,
    IDC_PACK,
    IDC_CONTROLS,
    IDC_BUDDY_LIST,
    IDC_LOAD1,
    IDC_SAVE1,
    IDC_BACK
};

ui_manager::control_tem WarriorSetupControls[] =
{
    { IDC_CONTINUE,     IDS_CONTINUE,           "button", 376, 384,  96,  24, 3, 9, 1, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME1,       IDS_NULL,               "frame",  265,  56, 207, 312, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },

    { IDC_FRAME2,       IDS_NULL,               "frame",    8,  56, 247, 197, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_NAME,         IDS_VK_WARRIOR_NAME,    "edit",    16,  86, 231,  24, 0, 0, 4, 1, ui_win::WF_VISIBLE },
    { IDC_GENDER,       IDS_GENDER,             "combo",   16, 122, 231,  19, 0, 1, 4, 1, ui_win::WF_VISIBLE },
    { IDC_SKIN,         IDS_SKIN,               "combo",   16, 144, 231,  19, 0, 2, 4, 1, ui_win::WF_VISIBLE },
    { IDC_VOICE,        IDS_VOICE,              "combo",   16, 166, 231,  19, 0, 3, 4, 1, ui_win::WF_VISIBLE },

    { IDC_CONTROLS,     IDS_WS_CONTROLS,        "button",  16, 191, 231,  24, 0, 4, 4, 1, ui_win::WF_VISIBLE },
    { IDC_BUDDY_LIST,   IDS_WS_BUDDY_LIST,      "button",  16, 219, 231,  24, 0, 5, 4, 1, ui_win::WF_VISIBLE },

    { IDC_FRAME3,       IDS_NULL,               "frame",    8, 263, 247, 105, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_ARMOR,        IDS_WS_ARMOR,           "combo",   16, 294, 231,  19, 0, 6, 4, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON,       IDS_WS_WEAPON,          "combo",   16, 316, 231,  19, 0, 7, 4, 1, ui_win::WF_VISIBLE },
    { IDC_PACK,         IDS_WS_PACK,            "combo",   16, 338, 231,  19, 0, 8, 4, 1, ui_win::WF_VISIBLE },

    { IDC_BACK,         IDS_BACK,               "button",   8, 384,  96,  24, 0, 9, 1, 1, ui_win::WF_VISIBLE },
    { IDC_LOAD1,        IDS_LOAD,               "button", 131, 384,  96,  24, 1, 9, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SAVE1,        IDS_SAVE,               "button", 253, 384,  96,  24, 2, 9, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::control_tem Warrior2SetupControls[] =
{
    { IDC_CONTINUE,     IDS_CONTINUE,           "button", 376, 384,  96,  24, 3, 9, 1, 1, ui_win::WF_VISIBLE },
                                                
    { IDC_FRAME1,       IDS_NULL,               "frame",    8,  56, 207, 312, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
                                                
    { IDC_FRAME2,       IDS_NULL,               "frame",  225,  56, 247, 197, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_NAME,         IDS_VK_WARRIOR_NAME,    "edit",   233,  86, 231,  24, 0, 0, 4, 1, ui_win::WF_VISIBLE },
    { IDC_GENDER,       IDS_GENDER,             "combo",  233, 122, 231,  19, 0, 1, 4, 1, ui_win::WF_VISIBLE },
    { IDC_SKIN,         IDS_SKIN,               "combo",  233, 144, 231,  19, 0, 2, 4, 1, ui_win::WF_VISIBLE },
    { IDC_VOICE,        IDS_VOICE,              "combo",  233, 166, 231,  19, 0, 3, 4, 1, ui_win::WF_VISIBLE },
                                                
    { IDC_CONTROLS,     IDS_WS_CONTROLS,        "button", 233, 191, 231,  24, 0, 4, 4, 1, ui_win::WF_VISIBLE },
    { IDC_BUDDY_LIST,   IDS_WS_BUDDY_LIST,      "button", 233, 219, 231,  24, 0, 5, 4, 1, ui_win::WF_VISIBLE },
                                                
    { IDC_FRAME3,       IDS_NULL,               "frame",  225, 263, 247, 105, 0, 0, 0, 0, ui_win::WF_VISIBLE|ui_win::WF_STATIC },
    { IDC_ARMOR,        IDS_WS_ARMOR,           "combo",  233, 294, 231,  19, 0, 6, 4, 1, ui_win::WF_VISIBLE },
    { IDC_WEAPON,       IDS_WS_WEAPON,          "combo",  233, 316, 231,  19, 0, 7, 4, 1, ui_win::WF_VISIBLE },
    { IDC_PACK,         IDS_WS_PACK,            "combo",  233, 338, 231,  19, 0, 8, 4, 1, ui_win::WF_VISIBLE },
                                                
    { IDC_BACK,         IDS_BACK,               "button",   8, 384,  96,  24, 0, 9, 1, 1, ui_win::WF_VISIBLE },
    { IDC_LOAD1,        IDS_LOAD,               "button", 131, 384,  96,  24, 1, 9, 1, 1, ui_win::WF_VISIBLE },
    { IDC_SAVE1,        IDS_SAVE,               "button", 253, 384,  96,  24, 2, 9, 1, 1, ui_win::WF_VISIBLE },
};

ui_manager::dialog_tem WarriorSetupDialog =
{
    IDS_WARRIOR_SETUP,
    4, 10,
    sizeof(WarriorSetupControls)/sizeof(ui_manager::control_tem),
    &WarriorSetupControls[0],
    0
};

ui_manager::dialog_tem Warrior2SetupDialog =
{
    IDS_WARRIOR_SETUP,
    4, 11,
    sizeof(Warrior2SetupControls)/sizeof(ui_manager::control_tem),
    &Warrior2SetupControls[0],
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

static xbool WarriorSave[2] = {0};
xbool  InitWarrior = TRUE;
extern xbool g_ControlModified;

warrior_setup   g_WarriorBackup[2];

//=========================================================================
//  Registration function
//=========================================================================

void dlg_warrior_setup_register( ui_manager* pManager )
{
    pManager->RegisterDialogClass( "warrior setup",  &WarriorSetupDialog,  &dlg_warrior_setup_factory );
    pManager->RegisterDialogClass( "warrior2 setup", &Warrior2SetupDialog, &dlg_warrior_setup_factory );
}

//=========================================================================
//  Factory function
//=========================================================================

ui_win* dlg_warrior_setup_factory( s32 UserID, ui_manager* pManager, ui_manager::dialog_tem* pDialogTem, const irect& Position, ui_win* pParent, s32 Flags, void* pUserData )
{
    dlg_warrior_setup* pDialog = new dlg_warrior_setup;
    pDialog->Create( UserID, pManager, pDialogTem, Position, pParent, Flags, pUserData );

    return (ui_win*)pDialog;
}

//=========================================================================
//  dlg_warrior_setup
//=========================================================================

dlg_warrior_setup::dlg_warrior_setup( void )
{
}

//=========================================================================

dlg_warrior_setup::~dlg_warrior_setup( void )
{
    Destroy();
}

//=========================================================================

xbool dlg_warrior_setup::Create( s32                        UserID,
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

    // Initialize Data
    m_iWarrior = fegl.iWarriorSetup;
    m_InUpdate = 0;
    WarriorSave[m_iWarrior] = FALSE;
    g_ControlModified = FALSE;

    // Backup the warrior config
    x_memcpy( &g_WarriorBackup[m_iWarrior], &fegl.WarriorSetups[m_iWarrior], sizeof(g_WarriorBackup[m_iWarrior]) );

    m_SoundID = -1;

    m_pName     = (ui_edit*)  FindChildByID( IDC_NAME );
    m_pGender   = (ui_combo*) FindChildByID( IDC_GENDER );
    m_pSkin     = (ui_combo*) FindChildByID( IDC_SKIN );
    m_pArmor    = (ui_combo*) FindChildByID( IDC_ARMOR );
    m_pWeapon   = (ui_combo*) FindChildByID( IDC_WEAPON );
    m_pPack     = (ui_combo*) FindChildByID( IDC_PACK );
    m_pVoice    = (ui_combo*) FindChildByID( IDC_VOICE );
    m_pFrame    = (ui_frame*) FindChildByID( IDC_FRAME1 );
    m_pFrame2   = (ui_frame*) FindChildByID( IDC_FRAME2 );
    m_pFrame3   = (ui_frame*) FindChildByID( IDC_FRAME3 );
    m_pLoad1    = (ui_button*)FindChildByID( IDC_LOAD1 );
    m_pSave1    = (ui_button*)FindChildByID( IDC_SAVE1 );
    m_pBack     = (ui_button*)FindChildByID( IDC_BACK );
    m_pContinue = (ui_button*)FindChildByID( IDC_CONTINUE );
    m_pControls = (ui_button*)FindChildByID( IDC_CONTROLS );
    m_pBuddies  = (ui_button*)FindChildByID( IDC_BUDDY_LIST );
//    m_pLoad     = (ui_button*)FindChildByID( IDC_LOAD );
//    m_pSave     = (ui_button*)FindChildByID( IDC_SAVE );

    m_pName->SetLabelWidth( 76 );
    m_pName->SetMaxCharacters( FE_MAX_WARRIOR_NAME-1 );

    m_pGender->SetLabelWidth( 76 );
    m_pGender->AddItem( "Female" , player_object::CHARACTER_TYPE_FEMALE );
    m_pGender->AddItem( "Male"   , player_object::CHARACTER_TYPE_MALE   );
    m_pGender->AddItem( "BioDerm", player_object::CHARACTER_TYPE_BIO    );

    m_pSkin->SetLabelWidth( 76 );

    m_pArmor->SetLabelWidth( 76 );
    m_pArmor->AddItem( "Light" , player_object::ARMOR_TYPE_LIGHT  );
    m_pArmor->AddItem( "Medium", player_object::ARMOR_TYPE_MEDIUM );
    m_pArmor->AddItem( "Heavy" , player_object::ARMOR_TYPE_HEAVY  );

    m_pWeapon->SetLabelWidth( 76 );
    m_pPack  ->SetLabelWidth( 76 );

    m_pVoice->SetLabelWidth( 76 );

    m_pFrame ->SetBackgroundColor( FECOL_WARRIOR_FRAME1 );
    m_pFrame2->SetBackgroundColor( FECOL_WARRIOR_FRAME2 );
    m_pFrame3->SetBackgroundColor( FECOL_WARRIOR_FRAME2 );
   
    // Set Initial Warrior Setup
	m_DontPlayVoice = TRUE;
    WarriorToControls();
	ControlsToWarrior(FALSE);
    m_DontPlayVoice = FALSE;

    // Select Armor
    m_pArmor->SetSelection( 0 );
	UpdatePreview(m_pArmor->GetSelection(),-1,-1);

//    m_pFrame->EnableTitle( "Setup" );
    m_pFrame2->EnableTitle( "SETUP" );
    m_pFrame3->EnableTitle( "PREVIEW" );
    
    m_StickX = m_StickY = 0; 

    m_PrevArmor = -1;
    m_CharSelectionID = m_pGender->GetSelection();
    m_SelectionID = m_pVoice->GetSelection();
	m_WaitingOnSave = FALSE;
    InitWarrior = FALSE;
	g_ControlModified = FALSE;

    // Return success code
    return Success;
}

//=========================================================================

void dlg_warrior_setup::Destroy( void )
{
    // Make sure that all the sounds are stopped when we leave the dialog.
    audio_Stop( m_SoundID );
    
    // Destory the dialog
    ui_dialog::Destroy();

    InitWarrior = TRUE;
}

//=========================================================================

void dlg_warrior_setup::Render( s32 ox, s32 oy )
{
    ui_dialog::Render( ox, oy );

    // Turn on Z Buffer
#ifdef TARGET_PS2
    gsreg_Begin();
    gsreg_SetZBuffer(TRUE);
    gsreg_End();
#endif
#ifdef TARGET_PC
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
#endif

    // Get window to render warrior to in screen coordinates.
#if defined(PAL_RELEASE)
    irect   Window( 0, 32, m_pFrame->GetWidth(), m_pFrame->GetHeight()+32 );
#else
    irect   Window( 0, 0, m_pFrame->GetWidth(), m_pFrame->GetHeight() );
#endif
    Window.Deflate( 4, 4 );
    m_pFrame->LocalToScreen( Window );
    
#ifdef TARGET_PC
/*    // Adjust where the character gets drawn according to the resulotion.
    s32 XRes, YRes;
    eng_GetRes( XRes, YRes );
    s32 midX = XRes>>1;
    s32 midY = YRes>>1;

    s32 dx = midX - 256;
    s32 dy = midY - 256;

    Window.Translate( dx, dy );
*/
#endif

    // Get Warrior Parameters
    s32 Gender = m_pGender->GetSelection();
    s32 Skin   = m_pSkin->GetSelection();
    s32 Armor  = m_pArmor->GetSelection();
    ASSERT( Gender != -1 );
    ASSERT( Skin != -1 );

    // Render the warrior
    RenderWarrior( Window, Gender, Skin, Armor );
}

//=========================================================================

void dlg_warrior_setup::RenderWarrior( const irect& Window, s32 Gender, s32 Skin, s32 Armor )
{
    static vector3          Pos(0,-1.3f,3.1f);
    static radian3          Rot(0,R_90,0);
    static shape_instance   Inst;
    static shape_instance   WeaponInst;
    static shape_instance   PackInst;

    // Preserve old view and setup warrior view
    view OldView;// = *eng_GetView( 0 );
#ifdef PAL_RELEASE
    OldView.SetViewport( 0, 0, 512, 512 );
#else
    OldView.SetViewport( 0, 0, 512, 448 );
#endif
    view WarriorView;
    WarriorView = OldView;
    
/*    static f32 dy = 0.0f;
    static f32 dz = 0.0f;
    if( dz < 2.5f )
    {
        dy += (m_StickY*0.5) * tgl.DeltaLogicTime;
        dz += dy*dy;
    }
*/
    WarriorView.SetViewport( Window.l, Window.t, Window.r, Window.b );
    WarriorView.SetRotation( radian3( R_0, R_0, R_0 ) );
//    WarriorView.SetPosition(vector3(0.0f, dy, dz) );
    WarriorView.LookAtPoint( Pos );
    WarriorView.SetXFOV( 0.75f );
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


    // Setup instance
    {
        // Setup character to draw
        player_object::character_type CharType;
        player_object::armor_type     ArmorType;
        player_object::invent_type    InventType;

        // Set Gender
        if( Gender == 0 )
            CharType = player_object::CHARACTER_TYPE_FEMALE;
        else if( Gender == 1 )
            CharType = player_object::CHARACTER_TYPE_MALE;
        else
            CharType = player_object::CHARACTER_TYPE_BIO;

        // Set Armor
        if( Armor == 0 )
            ArmorType = player_object::ARMOR_TYPE_LIGHT;
        else if( Armor == 1 )
            ArmorType = player_object::ARMOR_TYPE_MEDIUM;
        else
            ArmorType = player_object::ARMOR_TYPE_HEAVY;

        // Lookup useful info...
        const player_object::character_info& CharInfo = player_object::GetCharacterInfo( CharType, ArmorType );
        const player_object::move_info&     MoveInfo  = player_object::GetMoveInfo( CharType, ArmorType ) ;

        InventType = (player_object::invent_type)m_pWeapon->GetSelectedItemData();

//        const player_object::weapon_info&   WeaponInfo = player_object::GetWeaponInfo( InventType );
//        WeaponInfo
        shape* weapon = GetShape( m_pWeapon->GetSelectedItemData() );
        if( weapon == NULL )
            return;

        // Initialize
        if (WeaponInst.GetShape() != weapon)
        {
            // Set shape and scale
            WeaponInst.SetShape( weapon );

            // Set light
            WeaponInst.SetLightAmbientColor( xcolor(60,60,60) );
            WeaponInst.SetLightColor( XCOLOR_WHITE );
            WeaponInst.SetLightDirection( vector3(0,-1,1), TRUE );

            // Set color
            WeaponInst.SetColor( XCOLOR_WHITE );

            // Setup anim
//            WeaponInst.SetAnimByType( ANIM_TYPE_CHARACTER_IDLE );
        }

        shape* pack = tgl.GameShapeManager.GetShapeByType( m_pPack->GetSelectedItemData() );
        if( pack == NULL )
            return;

        // Initialize
        if (PackInst.GetShape() != pack)
        {
            // Set shape and scale
            PackInst.SetShape( pack );

            // Set light
            PackInst.SetLightAmbientColor( xcolor(60,60,60) );
            PackInst.SetLightColor( XCOLOR_WHITE );
            PackInst.SetLightDirection( vector3(0,-1,1), TRUE );
            PackInst.SetTextureAnimFPS( 15.0f );
            PackInst.SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG);
            // Set color
            PackInst.SetColor( XCOLOR_WHITE );

            // Setup anim
//            WeaponInst.SetAnimByType( ANIM_TYPE_CHARACTER_IDLE );
        }

        // Shapes not loaded yet?
        shape* Shape = tgl.GameShapeManager.GetShapeByType( CharInfo.ShapeType );
        if( Shape == NULL )
            return;

        // Initialize
        if (Inst.GetShape() != Shape)
        {
            // Set shape and scale
            Inst.SetShape( Shape );
            Inst.SetScale( MoveInfo.SCALE );

            // Set light
            Inst.SetLightAmbientColor( xcolor(60,60,60) );
            Inst.SetLightColor( XCOLOR_WHITE );
            Inst.SetLightDirection( vector3(0,-1,1), TRUE );

            // Set color
            Inst.SetColor( XCOLOR_WHITE );

            // Setup anim
            Inst.SetAnimByType( ANIM_TYPE_CHARACTER_IDLE );
            
        }

        // Set 
        Inst.SetTextureAnimFPS( 0 );
        Inst.SetTextureFrame( (f32)Skin );

        // Lookup anim type
        s32 AnimType  = ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH ;
        switch( m_pWeapon->GetSelectedItemData() )
        {
            case (s32)player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:
                AnimType = ANIM_TYPE_CHARACTER_ROCKET_LOOK_SOUTH_NORTH;
                break ;

            case (s32)player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE:
                AnimType = ANIM_TYPE_CHARACTER_SNIPER_LOOK_SOUTH_NORTH;
                break ;
        }
        
        // Lookup anim
        anim* pAnim = Shape->GetAnimByType(AnimType) ;
        if (!pAnim)
            pAnim = Shape->GetAnimByType(ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH) ;
        ASSERT(pAnim) ;

        // Setup anim
        anim_state*	pAnimState = Inst.AddAdditiveAnim(0) ;
        ASSERT(pAnimState) ;
        pAnimState->SetAnim(pAnim) ;
        pAnimState->SetSpeed(0) ; // Stop animation
        pAnimState->SetFrameParametric(0.5f) ;
    }

    // Put infront of camera
    const view  *View   = eng_GetView();
    vector3     ViewPos = View->ConvertV2W(Pos);
    Inst.SetPos(ViewPos);
    Inst.SetRot(Rot);

    hot_point *WeaponMountHotPoint = WeaponInst.GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT);
    
    // Get hand orientation
    matrix4 mHand = Inst.GetHotPointL2W(HOT_POINT_TYPE_WEAPON_MOUNT);

    // Calculate dead orientation
    radian3 DeadRot = Rot ;//mHand.GetRotation();
    vector3 DeadPos = mHand.GetTranslation();

    // Setup mount offset from hand
    vector3 WeaponMountOffset = WeaponMountHotPoint->GetPos();
    WeaponMountOffset.Rotate(DeadRot);

    // Take into account mount offset
    DeadPos -= WeaponMountOffset ;

    WeaponInst.SetPos( DeadPos );
    WeaponInst.SetRot( DeadRot );

    {
        PackInst.SetParentInstance( &Inst, HOT_POINT_TYPE_BACKPACK_MOUNT );

        // Put back edge of backpack on player
        bbox    BBox = PackInst.GetModel()->GetNode(0).GetBounds();
        vector3 Pos;
        Pos.X = -(BBox.Min.X + BBox.Max.X) * 0.5f;
        Pos.Y = -(BBox.Min.Y + BBox.Max.Y) * 0.5f;
        Pos.Z = - BBox.Max.Z;

        // Set pos
        PackInst.SetPos(Pos);
    }
    
    static f32 Time = 4.0;

    if( m_StickX == 0.0f )
        Time += tgl.DeltaLogicTime;
    else
        Time = 0.0f;

    // Spin
    if( Time > 4.0f )
    {
        Rot.Yaw += tgl.DeltaLogicTime * R_20;
    }
    else
    {
        Rot.Yaw += tgl.DeltaLogicTime * (m_StickX*2);//R_20;
    }

    // Advance anim
    Inst.Advance( tgl.DeltaLogicTime );
    WeaponInst.Advance( tgl.DeltaLogicTime );
    PackInst.Advance( tgl.DeltaLogicTime );

    // Finally draw it
    shape::BeginDraw();
    Inst.Draw(shape::DRAW_FLAG_CLIP);
    WeaponInst.Draw( shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_REF_WORLD_SPACE );
    PackInst.Draw( shape::DRAW_FLAG_CLIP );
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

void dlg_warrior_setup::OnUpdate( ui_win* pWin, f32 DeltaTime )
{
    (void)pWin;
    (void)DeltaTime;
    
	if ( (m_pWeapon->GetSelection() == -1) ||
		 (m_pPack->GetSelection()==-1) )
	{
		UpdatePreview(m_pArmor->GetSelection(),-1,-1);
	}

    f32 Stick1X = input_GetValue( INPUT_PS2_STICK_RIGHT_X, 0);
    f32 Stick1Y = input_GetValue( INPUT_PS2_STICK_RIGHT_Y, 0);
    f32 Stick2X = input_GetValue( INPUT_PS2_STICK_RIGHT_X, 1);
    f32 Stick2Y = input_GetValue( INPUT_PS2_STICK_RIGHT_Y, 1);

    if( x_abs(Stick1X) > x_abs(Stick2X) )
        m_StickX = Stick1X;
    else
        m_StickX = Stick2X;

    if( x_abs(Stick1Y) > x_abs(Stick2Y) )
        m_StickY = Stick1Y;
    else
        m_StickY = Stick2Y;
}

//=========================================================================

void dlg_warrior_setup::OnPadSelect( ui_win* pWin )
{
    s32 i = m_iWarrior;

/*
    // Check for TEST button
    if( pWin == (ui_win*)m_pVoice )
    {
        static s32 ID = 0;
        audio_Stop( ID );
        ID = audio_Play( m_pVoice->GetSelectedItemData( 1 ) + x_irand( 56, 59 ) );
    }
*/

    // Check for LOAD button
    if( pWin == (ui_win*)m_pLoad1 )
    {
        load_save_params options(m_iWarrior,LOAD_WARRIOR,this,&fegl.WarriorSetups[m_iWarrior]);
        fegl.iWarriorSetup = m_iWarrior;
		m_DontPlayVoice = TRUE;
        m_pManager->OpenDialog( m_UserID, 
                              "warriorloadsave", 
                              irect(0,0,300,264), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options );
    }

    // Check for SAVE button
    else if( pWin == (ui_win*)m_pSave1 )
    {
        load_save_params options(m_iWarrior,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),NULL,&fegl.WarriorSetups[m_iWarrior]);
        // Save the Warrior setup
        fegl.iWarriorSetup = m_iWarrior;
        x_wstrncpy( fegl.WarriorSetups[i].Name, m_pName->GetLabel(), FE_MAX_WARRIOR_NAME-1 );
        fegl.WarriorSetups[i].CharacterType = (player_object::character_type)m_pGender->GetSelectedItemData();
        fegl.WarriorSetups[i].SkinType      = (player_object::skin_type)m_pSkin->GetSelectedItemData();
        fegl.WarriorSetups[i].VoiceType     = (player_object::voice_type)m_pVoice->GetSelectedItemData();
        fegl.WarriorSetups[i].ArmorType     = (player_object::armor_type)m_pArmor->GetSelectedItemData();

        m_pManager->OpenDialog( m_UserID, 
                              "warriorloadsave", 
                              irect(0,0,300,264), 
                              NULL, 
                              ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
                              (void *)&options);
        
        WarriorSave[m_iWarrior] = FALSE;
        g_ControlModified = FALSE;
        // Backup the warrior config
        x_memcpy( &g_WarriorBackup[m_iWarrior], &fegl.WarriorSetups[m_iWarrior], sizeof(g_WarriorBackup[m_iWarrior]) );
    }

    // Check for CONTINUE button
    else if( pWin == (ui_win*)m_pContinue )
    {
		m_DontPlayVoice=TRUE;
        // Save the Warrior setup
        ControlsToWarrior(TRUE);

        // Backup the new warrior config
        x_memcpy( &g_WarriorBackup[m_iWarrior], &fegl.WarriorSetups[m_iWarrior], sizeof(g_WarriorBackup[m_iWarrior]) );

        // Increment Warrior Number
        fegl.iWarriorSetup = m_iWarrior + 1;

        // Set State
		if (WarriorSave[m_iWarrior] || g_ControlModified)
		{
			load_save_params options(m_iWarrior,(load_save_mode)(SAVE_WARRIOR|ASK_FIRST),this,&fegl.WarriorSetups[m_iWarrior]);

			m_WaitingOnSave = TRUE;
			m_pManager->OpenDialog( m_UserID, 
								  "warriorloadsave", 
								  irect(0,0,300,150), 
								  NULL, 
								  ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL,
								  (void *)&options );
			WarriorSave[m_iWarrior] = FALSE;
		}
		else
		{
			fegl.State = FE_STATE_GOTO_WARRIOR_SETUP;
			audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
		}

    }

    // Check for CONTROLS button
    else if( pWin == (ui_win*)m_pControls )
    {
        fegl.iWarriorSetup = m_iWarrior;
        m_pManager->OpenDialog( m_UserID, "controlconfig", irect( 0, 0, 464, 400 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL, &fegl.WarriorSetups[m_iWarrior] );
    }

    // Check for BUDDIES button
    else if( pWin == (ui_win*)m_pBuddies )
    {
        fegl.iWarriorSetup = m_iWarrior;
        m_pManager->OpenDialog( m_UserID, "buddies", irect( 0, 0, 320, 283 ), NULL, ui_win::WF_VISIBLE|ui_win::WF_BORDER|ui_win::WF_DLG_CENTER|ui_win::WF_INPUTMODAL, &fegl.WarriorSetups[m_iWarrior] );
    }

    // Chech for BACK button
    else if( pWin == (ui_win*)m_pBack )
    {
        // Restore the warrior config
        x_memcpy( &fegl.WarriorSetups[m_iWarrior], &g_WarriorBackup[m_iWarrior], sizeof(fegl.WarriorSetups[m_iWarrior]) );

	    m_DontPlayVoice = TRUE;
        m_pManager->EndDialog( m_UserID, TRUE );
        audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
    }
}

//=========================================================================

void dlg_warrior_setup::OnPadBack( ui_win* pWin )
{
    (void)pWin;

    // Restore the warrior config
    x_memcpy( &fegl.WarriorSetups[m_iWarrior], &g_WarriorBackup[m_iWarrior], sizeof(fegl.WarriorSetups[m_iWarrior]) );

	m_DontPlayVoice = TRUE;
    m_pManager->EndDialog( m_UserID, TRUE );
    audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
}

//=========================================================================
void dlg_warrior_setup::UpdatePreview(s32 Armor,s32 Weapon,s32 Pack)
{
	s32 Gender;

	Gender = m_pGender->GetSelection();

	m_PrevArmor = Armor;

    // Setup character to draw
    player_object::character_type CharType;
    player_object::armor_type     ArmorType;

    // Set Gender
    if( Gender == 0 )
        CharType = player_object::CHARACTER_TYPE_FEMALE;
    else if( Gender == 1 )
        CharType = player_object::CHARACTER_TYPE_MALE;
    else
        CharType = player_object::CHARACTER_TYPE_BIO;

    // Set Armor
    if( Armor == 0 )
        ArmorType = player_object::ARMOR_TYPE_LIGHT;
    else if( Armor == 1 )
        ArmorType = player_object::ARMOR_TYPE_MEDIUM;
    else
        ArmorType = player_object::ARMOR_TYPE_HEAVY;

    // Lookup useful info...
//        const player_object::character_info& CharInfo = player_object::GetCharacterInfo( CharType, ArmorType );
    const player_object::move_info&     MoveInfo  = player_object::GetMoveInfo( CharType, ArmorType ) ;

    // Setup the weapon combo
    SetupWeaponCombo( m_pWeapon, MoveInfo );

    // Setup the pack combo
    SetupPackCombo( m_pPack, MoveInfo );

    // The default weapons for the armors.
	if (Weapon == -1)
	{
		if( Armor == 0 )
			m_pWeapon->SetSelection( 1 );
		else if( Armor == 1 )
			m_pWeapon->SetSelection( 0 );
		else
			m_pWeapon->SetSelection( 3 );
	}
	else
	{
		m_pWeapon->SetSelection(Weapon);
	}

    // The default packs for the armors.
	if (Pack == -1)
	{
		if( Armor == 0 )
			m_pPack->SetSelection( 1 );
		else if( Armor == 1 )
			m_pPack->SetSelection( 1 );
		else
			m_pPack->SetSelection( 1 );
	}
	else
	{
		m_pPack->SetSelection(Pack);
	}
}

void dlg_warrior_setup::OnNotify( ui_win* pWin, ui_win* pSender, s32 Command, void* pData )
{
    (void)pWin;
    (void)pSender;
    (void)Command;
    (void)pData;

    // Update weapon if armor changes
    if( pSender == (ui_win*)m_pArmor )
    {
        ASSERT( m_pGender->GetSelection() != -1);
        ASSERT( m_pSkin->GetSelection() != -1);

		if (m_pArmor->GetSelection() == m_PrevArmor)
			return;

		UpdatePreview(m_pArmor->GetSelection(),-1,-1);
    }
    else if (Command != WN_USER)
    {
        ControlsToWarrior(TRUE);
    }
    else
    {
		WarriorToControls();
		WarriorSave[m_iWarrior]=FALSE;
		UpdatePreview(fegl.WarriorSetups[m_iWarrior].ArmorType,
					fegl.WarriorSetups[m_iWarrior].WeaponType,
					fegl.WarriorSetups[m_iWarrior].PackType);

		if (m_WaitingOnSave)
		{
			fegl.State = FE_STATE_GOTO_WARRIOR_SETUP;
			audio_Play( SFX_FRONTEND_SELECT_01_CLOSE, AUDFLAG_CHANNELSAVER );
			m_WaitingOnSave = FALSE;
			g_ControlModified = FALSE;
			return;
		}
		else
		{
//			GotoControl( (ui_control*)m_pContinue );
		}

    }

    WarriorToControls();
}

//=========================================================================

void dlg_warrior_setup::WarriorToControls( void )
{
    // Only enter if not already updating
    if( !m_InUpdate )
    {
        // Set updating flag
        m_InUpdate++;

        // Set Name
        m_pName->SetLabel( fegl.WarriorSetups[m_iWarrior].Name );

        // Locate and select Gender
        s32 iGender = m_pGender->FindItemByData( (s32)fegl.WarriorSetups[m_iWarrior].CharacterType );
        if( iGender == -1 ) iGender = 0;
        m_pGender->SetSelection( iGender );

        // Setup Skins & Voices from Gender
        m_pSkin->DeleteAllItems();
        m_pVoice->DeleteAllItems();
        switch( m_pGender->GetSelectedItemData() )
        {
        case player_object::CHARACTER_TYPE_FEMALE:
            m_pVoice->AddItem( "Heroine"      , player_object::VOICE_TYPE_FEMALE_HEROINE,      SFX_VOICE_FEMALE_1 );
            m_pVoice->AddItem( "Professional" , player_object::VOICE_TYPE_FEMALE_PROFESSIONAL, SFX_VOICE_FEMALE_2 );
            m_pVoice->AddItem( "Cadet"        , player_object::VOICE_TYPE_FEMALE_CADET,        SFX_VOICE_FEMALE_3 );
            m_pVoice->AddItem( "Veteran"      , player_object::VOICE_TYPE_FEMALE_VETERAN,      SFX_VOICE_FEMALE_4 );
            m_pVoice->AddItem( "Amazon"       , player_object::VOICE_TYPE_FEMALE_AMAZON,       SFX_VOICE_FEMALE_5 );
            m_pSkin ->AddItem( "Blood Eagle"  , player_object::SKIN_TYPE_BEAGLE         );
            m_pSkin ->AddItem( "Phoenix"      , player_object::SKIN_TYPE_COTP           );
            m_pSkin ->AddItem( "Diamond Sword", player_object::SKIN_TYPE_DSWORD         );
            m_pSkin ->AddItem( "Starwolf"     , player_object::SKIN_TYPE_SWOLF          );
            break;

        case player_object::CHARACTER_TYPE_MALE:
            m_pVoice->AddItem( "Hero"         , player_object::VOICE_TYPE_MALE_HERO,     SFX_VOICE_MALE_1 );
            m_pVoice->AddItem( "Iceman"       , player_object::VOICE_TYPE_MALE_ICEMAN,   SFX_VOICE_MALE_2 );
            m_pVoice->AddItem( "Rogue"        , player_object::VOICE_TYPE_MALE_ROGUE,    SFX_VOICE_MALE_3 );
            m_pVoice->AddItem( "Hardcase"     , player_object::VOICE_TYPE_MALE_HARDCASE, SFX_VOICE_MALE_4 );
            m_pVoice->AddItem( "Psycho"       , player_object::VOICE_TYPE_MALE_PSYCHO,   SFX_VOICE_MALE_5 );
            m_pSkin ->AddItem( "Blood Eagle"  , player_object::SKIN_TYPE_BEAGLE         );
            m_pSkin ->AddItem( "Phoenix"      , player_object::SKIN_TYPE_COTP           );
            m_pSkin ->AddItem( "Diamond Sword", player_object::SKIN_TYPE_DSWORD         );
            m_pSkin ->AddItem( "Starwolf"     , player_object::SKIN_TYPE_SWOLF          );
            break;

        case player_object::CHARACTER_TYPE_BIO:
            m_pVoice->AddItem( "Warrior" ,     player_object::VOICE_TYPE_BIO_WARRIOR,  SFX_VOICE_DERM_1 );
            m_pVoice->AddItem( "Monster" ,     player_object::VOICE_TYPE_BIO_MONSTER,  SFX_VOICE_DERM_2 );
            m_pVoice->AddItem( "Predator",     player_object::VOICE_TYPE_BIO_PREDATOR, SFX_VOICE_DERM_3 );
            m_pSkin ->AddItem( "Skin 1"  ,     player_object::SKIN_TYPE_BEAGLE        );
            m_pSkin ->AddItem( "Skin 2"  ,     player_object::SKIN_TYPE_COTP          );
            m_pSkin ->AddItem( "Skin 3"  ,     player_object::SKIN_TYPE_DSWORD        );
            break;
        }

        // Set Skin
        s32 iSkin = m_pSkin->FindItemByData( (s32)fegl.WarriorSetups[m_iWarrior].SkinType );
        if( iSkin == -1 ) iSkin = 0;
        m_pSkin->SetSelection( iSkin );

        // Set Voice
        s32 iVoice = (s32)fegl.WarriorSetups[m_iWarrior].VoiceType;
        if( iVoice <  0 ) iVoice = 0;
        if( iVoice > player_object::VOICE_TYPE_END ) iVoice = player_object::VOICE_TYPE_END;
        if( iVoice >= player_object::VOICE_TYPE_BIO_WARRIOR ) iVoice -= player_object::VOICE_TYPE_BIO_WARRIOR;
        if( iVoice >= player_object::VOICE_TYPE_MALE_HERO   ) iVoice -= player_object::VOICE_TYPE_MALE_HERO;
        if( iVoice >= m_pVoice->GetItemCount() ) iVoice = m_pVoice->GetItemCount() - 1;
        m_pVoice->SetSelection( iVoice );
        
        if( m_CharSelectionID != m_pGender->GetSelection() )
        {
            m_CharSelectionID = m_pGender->GetSelection();
            audio_Stop( m_SoundID );
            m_SelectionID = m_pVoice->GetSelection();
        }

        m_pArmor->SetSelection( fegl.WarriorSetups[m_iWarrior].ArmorType);
        // Clear updating flag
        m_InUpdate--;
    }
}

//=========================================================================
// If CheckDirty is true, then the warrior is checked to see if any
// values have changed and a global flag is set if this is so.
void dlg_warrior_setup::ControlsToWarrior( xbool CheckDirty )
{
    if( (m_pVoice->GetSelection() != -1) && (m_pGender->GetSelection() != -1) )
    {
        if( m_SelectionID > m_pVoice->GetItemCount() )
        {
            audio_Stop( m_SoundID );
            m_SelectionID = m_pVoice->GetSelection();
        }

        // Play the voice as soon as the type changes.
        if( (m_SelectionID != m_pVoice->GetSelection()) &&  (m_CharSelectionID == m_pGender->GetSelection()) && (!InitWarrior))
        {
            audio_Stop( m_SoundID );
			if (m_DontPlayVoice)
			{
				m_DontPlayVoice = FALSE;
			}
			else
			{
				m_SoundID = audio_Play( m_pVoice->GetSelectedItemData( 1 ) + x_irand( 56, 59 ), AUDFLAG_CHANNELSAVER );
			}

            m_SelectionID = m_pVoice->GetSelection();   
        }        
    }

    if( !m_InUpdate )
    {
        xstring ok;

		player_object::character_type	gendertype;
		player_object::skin_type		skintype;
		player_object::voice_type		voicetype;
		player_object::armor_type		armortype;

		s32								weapontype;
		s32								packtype;

        if( m_Language.CheckWord( xstring(m_pName->GetLabel()) ) )
        {
            // Reset the original name if they entered a bad word
            m_pName->SetLabel( fegl.WarriorSetups[m_iWarrior].Name );
        }
        // Copy Name Back
		if ( CheckDirty && (x_wstrcmp(&fegl.WarriorSetups[m_iWarrior].Name[0],m_pName->GetLabel())!=0) )
		{
			WarriorSave[m_iWarrior] = TRUE;
		}
        x_wstrncpy( &fegl.WarriorSetups[m_iWarrior].Name[0], m_pName->GetLabel(), FE_MAX_WARRIOR_NAME-1 );
        fegl.WarriorSetups[m_iWarrior].Name[FE_MAX_WARRIOR_NAME-1] = 0;

		gendertype = (player_object::character_type)m_pGender->GetSelectedItemData();
		skintype   = (player_object::skin_type)m_pSkin->GetSelectedItemData();

		armortype  = (player_object::armor_type)m_pArmor->GetSelectedItemData();
		weapontype = m_pWeapon->GetSelection();
		packtype   = m_pPack->GetSelection();

		voicetype  = (player_object::voice_type)m_pVoice->GetSelectedItemData();
        // Copy Gender Back
        if( gendertype != -1 )
		{
			if ( CheckDirty && (fegl.WarriorSetups[m_iWarrior].CharacterType != gendertype) )
			{
				WarriorSave[m_iWarrior] = TRUE;
			}
            fegl.WarriorSetups[m_iWarrior].CharacterType = gendertype;
		}


        // Copy Skin Back
        if( skintype != -1 )
		{
			if ( CheckDirty && (fegl.WarriorSetups[m_iWarrior].SkinType != skintype) )
			{
				WarriorSave[m_iWarrior] = TRUE;
			}

            fegl.WarriorSetups[m_iWarrior].SkinType = skintype;
		}

        // Copy Voice Back
        if( voicetype != -1 )
		{
			if ( CheckDirty && (fegl.WarriorSetups[m_iWarrior].VoiceType != voicetype) )
			{
				WarriorSave[m_iWarrior] = TRUE;
			}
            fegl.WarriorSetups[m_iWarrior].VoiceType = voicetype;
		}

        // Copy Armor back and preview information
        fegl.WarriorSetups[m_iWarrior].ArmorType = armortype;
		fegl.WarriorSetups[m_iWarrior].WeaponType = weapontype;
		fegl.WarriorSetups[m_iWarrior].PackType = packtype;
    }
}

//=========================================================================

void dlg_warrior_setup::SetupWeaponCombo( ui_combo* pCombo, const player_object::move_info& MoveInfo )
{
    pCombo->DeleteAllItems();

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

struct pack_info
{
    s32 InvenType;
    s32 ShapeType;
    s32 StringID;
};

pack_info PackInfo[] =
{
    { player_object::INVENT_TYPE_ARMOR_PACK_AMMO              , SHAPE_TYPE_ARMOR_PACK_AMMO            , IDS_PACK_AMMO           },
    { player_object::INVENT_TYPE_ARMOR_PACK_ENERGY            , SHAPE_TYPE_ARMOR_PACK_ENERGY          , IDS_PACK_ENERGY         },
    { player_object::INVENT_TYPE_ARMOR_PACK_REPAIR            , SHAPE_TYPE_ARMOR_PACK_REPAIR          , IDS_PACK_REPAIR         },
    { player_object::INVENT_TYPE_ARMOR_PACK_SHIELD            , SHAPE_TYPE_ARMOR_PACK_SHIELD          , IDS_PACK_SHIELD         },
    { player_object::INVENT_TYPE_DEPLOY_PACK_SATCHEL_CHARGE   , SHAPE_TYPE_DEPLOY_PACK_SATCHEL_CHARGE , IDS_PACK_DEPLOY_SATCHEL },
    { player_object::INVENT_TYPE_DEPLOY_PACK_INVENT_STATION   , SHAPE_TYPE_DEPLOY_PACK_INVENT_STATION , IDS_PACK_DEPLOY_INVEN   },
    { player_object::INVENT_TYPE_DEPLOY_PACK_TURRET_INDOOR    , SHAPE_TYPE_DEPLOY_PACK_TURRET_INDOOR  , IDS_PACK_DEPLOY_TURRET  },
    { player_object::INVENT_TYPE_DEPLOY_PACK_PULSE_SENSOR     , SHAPE_TYPE_DEPLOY_PACK_PULSE_SENSOR   , IDS_PACK_DEPLOY_SENSOR  },
    { player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_AA        , SHAPE_TYPE_DEPLOY_PACK_BARREL_AA      , IDS_PACK_BARREL_AA      },
    { player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MORTAR    , SHAPE_TYPE_DEPLOY_PACK_BARREL_MORTAR  , IDS_PACK_BARREL_MORTAR  },
    { player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_MISSILE   , SHAPE_TYPE_DEPLOY_PACK_BARREL_MISSILE , IDS_PACK_BARREL_MISSILE },
    { player_object::INVENT_TYPE_DEPLOY_PACK_BARREL_PLASMA    , SHAPE_TYPE_DEPLOY_PACK_BARREL_PLASMA  , IDS_PACK_BARREL_PLASMA  }
};

void dlg_warrior_setup::SetupPackCombo( ui_combo* pCombo, const player_object::move_info& MoveInfo )
{
    pCombo->DeleteAllItems();

    for( u32 i=0 ; i<(sizeof(PackInfo)/sizeof(pack_info)) ; i++ )
    {
        s32 iItem = pCombo->AddItem( StringMgr( "ui", PackInfo[i].StringID ), PackInfo[i].ShapeType );
        pCombo->SetItemEnabled( iItem, MoveInfo.INVENT_MAX[PackInfo[i].InvenType] > 0 );
    }
}

//=========================================================================

void dlg_warrior_setup::AddWeaponToCombo( ui_combo* pCombo, const char* Name, s32 Type, const player_object::move_info& MoveInfo )
{
    s32 iItem = pCombo->AddItem( Name, Type );
    pCombo->SetItemEnabled( iItem, MoveInfo.INVENT_MAX[Type] > 0 );
}

//=========================================================================

shape* dlg_warrior_setup::GetShape( s32 Data )
{
    // Return the correct shape from the shape manager. 
    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_PLASMA_GUN )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_PLASMA_GUN );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_DISK_LAUNCHER )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_DISC_LAUNCHER );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_GRENADE_LAUNCHER )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_GRENADE_LAUNCHER );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_MORTAR_GUN )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_MORTAR_LAUNCHER );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_MISSILE_LAUNCHER )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_MISSILE_LAUNCHER );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_BLASTER )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_BLASTER );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_CHAIN_GUN )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_CHAIN_GUN );

    if( Data == (s32)player_object::INVENT_TYPE_WEAPON_SNIPER_RIFLE )
        return tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_WEAPON_SNIPER_RIFLE );

    return NULL;
}

//=========================================================================