//=========================================================================
//
//  event_Input.cpp
//
//=========================================================================

#include "Entropy.hpp"
#include "Globals.hpp"
#include "Objects/Player/PlayerObject.hpp"
#include "ObjectMgr\ColliderCannon.hpp"
#include "Building/BuildingObj.hpp"
#include "sm_common.hpp"

//=========================================================================

extern xbool g_RunningPathEditor;

void event_UpdateManualView( void )
{
#ifdef TARGET_PC
    // Move view using keyboard and mouse
    // WASD - forward, left, back, right
    // RF   - up, down
    // MouseRightButton - 4X speed

    radian Pitch;
    radian Yaw;
    f32    S = 0.500f;
    f32    R = 0.005f;

    if( input_IsPressed( INPUT_MOUSE_BTN_R ) )  S *= 8.0f;
    if( input_IsPressed( INPUT_KBD_E       ) )  tgl.FreeCam[0].Translate( vector3( 0, 0, S), view::VIEW );
    if( input_IsPressed( INPUT_KBD_D       ) )  tgl.FreeCam[0].Translate( vector3( 0, 0,-S), view::VIEW );
    if( input_IsPressed( INPUT_KBD_S       ) )  tgl.FreeCam[0].Translate( vector3( S, 0, 0), view::VIEW );
    if( input_IsPressed( INPUT_KBD_F       ) )  tgl.FreeCam[0].Translate( vector3(-S, 0, 0), view::VIEW );
    if( input_IsPressed( INPUT_KBD_T       ) )  tgl.FreeCam[0].Translate( vector3( 0, S, 0), view::VIEW );
    if( input_IsPressed( INPUT_KBD_G       ) )  tgl.FreeCam[0].Translate( vector3( 0,-S, 0), view::VIEW );

    tgl.FreeCam[0].GetPitchYaw( Pitch, Yaw );       
    Pitch += input_GetValue( INPUT_MOUSE_Y_REL ) * R;
    Yaw   -= input_GetValue( INPUT_MOUSE_X_REL ) * R;
    Pitch  = MINMAX( -R_89, Pitch, R_89 );
    tgl.FreeCam[0].SetRotation( radian3(Pitch,Yaw,R_0) );

    if( input_WasPressed( INPUT_KBD_F12 ) ) tgl.DoRender           = !tgl.DoRender;
    if( input_WasPressed( INPUT_KBD_F11 ) ) tgl.DisplayStats       = !tgl.DisplayStats;
    if( input_WasPressed( INPUT_KBD_F10 ) ) tgl.RenderBuildingGrid = !tgl.RenderBuildingGrid;
    if( input_WasPressed( INPUT_KBD_F9 ) )  tgl.DisplayObjectStats = !tgl.DisplayObjectStats;
    if( input_WasPressed( INPUT_KBD_F1  ) ) tgl.ShowColliderCannon = !tgl.ShowColliderCannon;

    if( input_WasPressed( INPUT_KBD_F11 ) &&
        (input_IsPressed( INPUT_KBD_LCONTROL ) || input_IsPressed( INPUT_KBD_RCONTROL )) )
    {
        tgl.LogStats = ~tgl.LogStats;
        if( tgl.LogStats ) x_printf("Stats logging on\n");
        else               x_printf("Stats logging off\n");
    }

    if( input_IsPressed( INPUT_KBD_1 ) && tgl.UseFreeCam[0] )
    {
        tgl.ShowColliderCannon = TRUE;
        cc_Aim( tgl.FreeCam[0].GetPosition(), tgl.FreeCam[0].GetViewZ() );
    }

    if( tgl.ShowColliderCannon )
    {
        cc_Fire();
    }

    if( input_WasPressed( INPUT_KBD_F3 ) )
    {
	    player_object *pPlayer ;
        ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
        while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
        {
            pPlayer->Respawn() ;
        }
        ObjMgr.EndTypeLoop();
    }
    
    if( input_WasPressed( INPUT_KBD_F5 ) &&
        (input_IsPressed( INPUT_KBD_LCONTROL ) || input_IsPressed( INPUT_KBD_RCONTROL )) )
    {
        g_BldManager.SetLightDir( -tgl.Sun );
        
#if 0
        LightBuildings();
#endif
    }
    

#endif

#ifdef TARGET_PS2

    radian Pitch;
    radian Yaw;
    f32    S   = 0.50f*2;
    f32    R   = 0.025f*2;
    f32    Lateral;
    f32    Vertical;

    if( input_IsPressed( INPUT_PS2_BTN_L1 ) )  S *= 4.0f;
    if( input_IsPressed( INPUT_PS2_BTN_L2 ) )  S *= 0.025f;
    if( input_IsPressed( INPUT_PS2_BTN_R1 ) )  tgl.FreeCam[0].Translate( vector3( 0, S, 0), view::VIEW );
    if( input_IsPressed( INPUT_PS2_BTN_R2 ) )  tgl.FreeCam[0].Translate( vector3( 0,-S, 0), view::VIEW );

    Lateral  = S * input_GetValue( INPUT_PS2_STICK_LEFT_X );
    Vertical = S * input_GetValue( INPUT_PS2_STICK_LEFT_Y );
    tgl.FreeCam[0].Translate( vector3(0,0,Vertical), view::VIEW );
    tgl.FreeCam[0].Translate( vector3(-Lateral,0,0), view::VIEW );

    tgl.FreeCam[0].GetPitchYaw( Pitch, Yaw );
    Pitch += input_GetValue( INPUT_PS2_STICK_RIGHT_Y ) * R;
    Yaw   -= input_GetValue( INPUT_PS2_STICK_RIGHT_X ) * R;
    Pitch  = MINMAX( -R_89, Pitch, R_89 );
    tgl.FreeCam[0].SetRotation( radian3(Pitch,Yaw,R_0) );

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) &&
        input_WasPressed( INPUT_PS2_BTN_L1 ) ) 
    {
        tgl.DisplayStats = !tgl.DisplayStats;
    }

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) &&
        input_WasPressed( INPUT_PS2_BTN_R1 ) )
    {
        tgl.LogStats = ~tgl.LogStats;
        if( tgl.LogStats ) x_printf("Stats logging on\n");
        else               x_printf("Stats logging off\n");
    }

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) &&
        input_WasPressed( INPUT_PS2_BTN_L2 ) )
    {
        tgl.DisplayObjectStats = !tgl.DisplayObjectStats;
    }

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) && 
        input_WasPressed( INPUT_PS2_BTN_R2 ) )
    {
        eng_ScreenShot();
    }

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) && 
        input_WasPressed( INPUT_PS2_BTN_TRIANGLE ) )
    {
        g_BldManager.SetLightDir( -tgl.Sun );
        
#if 0
        LightBuildings();
#endif
    }

    if( input_IsPressed( INPUT_PS2_BTN_SELECT ) && 
        input_WasPressed( INPUT_PS2_BTN_CIRCLE ) )
    {
        tgl.ShowColliderCannon ^= 1;;
    }

    if( tgl.ShowColliderCannon )
    {
        cc_Aim( tgl.FreeCam[0].GetPosition(), tgl.FreeCam[0].GetViewZ() );
        cc_Fire();
    }

#endif
}

//=========================================================================

void event_Input( void )
{
    // Watch for shutdown request
    #ifdef TARGET_PC
    {
        if( input_WasPressed( INPUT_KBD_ESCAPE ) && !g_RunningPathEditor)
        {
            // To exit in a safe manner please use this function.
            //DestroyWindow( d3deng_GetWindowHandle() );

            // SB - Changed to this so full screen exit correctly
            x_DebugMsg("*-*-*-* Shutdown has been requested\n");
            g_SM.ServerShutdown = TRUE;
            g_SM.MissionRunning = FALSE;
            return;
        }

        if( input_IsPressed( INPUT_MSG_EXIT ) )
        {
            x_DebugMsg("*-*-*-* Shutdown has been requested\n");
            g_SM.ServerShutdown = TRUE;
            g_SM.MissionRunning = FALSE;
            return;
        }
    }
    #endif
/*
	// Is player controlling the view?
	xbool PlayerControllingView = FALSE ;
	if (tgl.ControllingPlayerID != obj_mgr::NullID)
	{
		player_object *pPlayerObj = (player_object *)ObjMgr.GetObjectFromID(tgl.ControllingPlayerID) ;
		PlayerControllingView = pPlayerObj->GetHasInputFocus() ;
	}

	// Update manual view
    if( (!PlayerControllingView) && ( tgl.UseManualView ) )
    {
        event_UpdateManualView();
    }
*/
    if( tgl.UseFreeCam[0] )
    {
        event_UpdateManualView();
    }
}

//=========================================================================
