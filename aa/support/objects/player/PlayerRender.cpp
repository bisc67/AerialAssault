//==============================================================================
//
//  PlayerRender.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#include "PlayerObject.hpp"
#include "Entropy.hpp"
#include "Demo1/Globals.hpp"
#include "Demo1/fe_globals.hpp"

#include "Demo1/GameClient.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Objects/Terrain/Terrain.hpp"
#include "Objects/Bot/BotObject.hpp"
#include "Building/BuildingObj.hpp"
#include "Objects/Projectiles/Flag.hpp"
#include "Objects/Projectiles/AutoAim.hpp"
#include "Damage/Damage.hpp"
#include "HUD/hud_Icons.hpp"
#include "UI/ui_manager.hpp"
#include "ui/ui_font.hpp"

//#define DEBUG_TURN_STATS_ON
#include "Shape/DebugUtils.hpp"
#include "ObjectMgr/ColliderCannon.hpp"

//==============================================================================
// DEBUG COLLISION DEFINES
//==============================================================================

//#define DEBUG_COLLISION       // Draws ngon points (setup by player collision) + bbox
//#define DRAW_VIEW_AND_WEAPON_FIRE_POS
//#define DRAW_WEAPON_LOS   // draws weapon line of sight
//#define DRAW_DIRECTION      // draws direction arrow

//#define PLAYER_TIMERS

#ifdef PLAYER_TIMERS
xtimer PlayerTimer[10];
#endif

#ifdef DRAW_VIEW_AND_WEAPON_FIRE_POS

bbox PlayerWeaponStartBBox ;
bbox PlayerWeaponEndBBox ;

#endif


//==============================================================================
// DEFINES FOR DROPPING GEOMETRY
//==============================================================================

static f32 MIN_PIXEL_SIZE_PLAYER_RADIUS     = 1.0f;    // Radius around player
static f32 MIN_PIXEL_SIZE_PLAYER            = 1.5f;
static f32 MIN_PIXEL_SIZE_DUMMY             = 6.0f;
static f32 MIN_PIXEL_SIZE_PARTICLES         = 10 ;
static f32 MIN_PIXEL_SIZE_WEAPON            = 10 ;
static f32 MIN_PIXEL_SIZE_BACKPACK          = 10 ;
static f32 MIN_PIXEL_SIZE_FLAG              = 10 ;

extern xbool   HUD_HideHUD;

//==============================================================================
// DEFINES
//==============================================================================

#define COLLIDER_LOS_MASK   ATTR_SOLID_STATIC

extern xbool DO_SCREEN_SHOTS;

f32 FadeDistanceT      = 0.5f;
f32 Flag3rdPersonAlpha = 0.1f;

//==============================================================================
// UTIL DRAW FUNCTIONS
//==============================================================================

xcolor Blend(const xcolor& ColA, const xcolor& ColB, f32 t)
{
    s32 T = (s32)(t * 256.0f) ;

    return xcolor((u8)(ColA.R + ((T * (ColB.R - ColA.R)) >> 8)),
                  (u8)(ColA.G + ((T * (ColB.G - ColA.G)) >> 8)),
                  (u8)(ColA.B + ((T * (ColB.B - ColA.B)) >> 8)),
                  (u8)(ColA.A + ((T * (ColB.A - ColA.A)) >> 8)) ) ;
}

//==============================================================================


// Updates all the instances ready for drawing
void player_object::UpdateInstanceOrient()
{
    DEBUG_BEGIN_TIMER(UpdateInstanceOrient)

    s32 i ;

    //==========================================================================
    // Setup correct body look pose
    //==========================================================================

    // Get animation state
    anim_state* BodyLookAnimState = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;
    if (BodyLookAnimState)
    {
        // Set body lookup anim
        s32 BodyLookAnimType = ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH ;
        if (m_Armed) 
        {
            switch(m_WeaponCurrentType)
            {
                case INVENT_TYPE_WEAPON_MISSILE_LAUNCHER:
                    BodyLookAnimType = ANIM_TYPE_CHARACTER_ROCKET_LOOK_SOUTH_NORTH ;
                    break ;

                case INVENT_TYPE_WEAPON_SNIPER_RIFLE:
                    BodyLookAnimType = ANIM_TYPE_CHARACTER_SNIPER_LOOK_SOUTH_NORTH ;
                    break ;
            }
        }

        // Swap to it?
        ASSERT(BodyLookAnimState->GetAnim()) ;
        if (BodyLookAnimState->GetAnim()->GetType() != BodyLookAnimType)
        {
            // Set new anim if it's there...
            anim* BodyLookAnim = m_PlayerInstance.GetShape()->GetAnimByType(BodyLookAnimType) ;
            if (BodyLookAnim)
                BodyLookAnimState->SetAnim(BodyLookAnim) ;
        }
    }

    //==========================================================================
    // Setup additive look animations
    //==========================================================================

    // Setup anim weight for additive anims (if dead, blend out)
    f32 AnimWeight ;
    if (m_Health > 0)
        AnimWeight = 1 ;
    else
        AnimWeight = 1.0f - m_GroundOrientBlend ; // will be blending towards ground when dead

    // Get pitch of player (without ground rot)

    // Calculate pitch as a value from 0 (up - VIEW_MIN_PITCH) to 1 (down - VIEW_MAX_PITCH)
    f32 Pitch = (m_DrawRot.Pitch - s_CommonInfo.VIEW_MIN_PITCH) / (s_CommonInfo.VIEW_MAX_PITCH - s_CommonInfo.VIEW_MIN_PITCH) ;
    if (Pitch < 0)
        Pitch = 0 ;
    else
    if (Pitch > 1)
        Pitch = 1 ;

    // Calculate yaw as a value from 0 (-R_180) to 1 (R_180)
    //f32 Yaw = (R_360 - (m_DrawViewFreeLookYaw + R_180)) / R_360 ;
    //if (Yaw < 0)
        //Yaw = 0 ;
    //else
    //if (Yaw > 1)
        //Yaw = 1 ;
    f32 Yaw = m_DrawViewFreeLookYaw ;


    // Update body look up/down additive anim
    anim_state *BodyUD = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;
    if ( (BodyUD) && (BodyUD->GetAnim()) )
    {
        // No body look when in a vehicle
        if (IsSatDownInVehicle())
            BodyUD->SetWeight(0) ;
        else
        {
            BodyUD->SetWeight(AnimWeight) ;
            BodyUD->SetFrameParametric(Pitch) ;
        }
    }

    // Update head look up/down additive anim
    anim_state *HeadUD = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_HEAD_LOOK_UD) ;
    if ( (HeadUD) && (HeadUD->GetAnim()) )
    {
        HeadUD->SetWeight(AnimWeight) ;
        HeadUD->SetFrameParametric(Pitch) ;
    }

    // Update head look left/right additive anim
    anim_state *HeadLR = m_PlayerInstance.GetAdditiveAnimState(ADDITIVE_ANIM_ID_HEAD_LOOK_LR) ;
    if ( (HeadLR) && (HeadLR->GetAnim()) )
    {
        // Turn off?
        if (m_DrawViewFreeLookYaw == 0.5f)
            HeadLR->SetWeight(0) ;
        else
        {
            // Set left/right look
            HeadLR->SetWeight(AnimWeight) ;
            HeadLR->SetFrameParametric(Yaw) ;
        }
    }

    //==========================================================================
    // Mask off upper body with idle animation when moving
    // so that the weapon does not move all over the place
    //==========================================================================
    anim_state *IdleAnimState = m_PlayerInstance.GetMaskedAnimState(MASKED_ANIM_ID_IDLE) ;
    if (        (m_Health > 0)
            &&  (m_ProcessInput) 
            &&  (m_PlayerState >= PLAYER_STATE_MOVE_START)
            &&  (m_PlayerState <= PLAYER_STATE_MOVE_END) )
    {
        // Add state if not there
        if (!IdleAnimState)
            IdleAnimState = m_PlayerInstance.AddMaskedAnim(MASKED_ANIM_ID_IDLE) ;

        // Set masked anim
        ASSERT(IdleAnimState) ;
        m_PlayerInstance.SetMaskedAnimByType(MASKED_ANIM_ID_IDLE, ANIM_TYPE_CHARACTER_IDLE) ;
        if (GetPlayerViewType() == VIEW_TYPE_1ST_PERSON) 
            IdleAnimState->SetWeight(0.95f) ;   // Make upper half of body be 95% idle
        else
            IdleAnimState->SetWeight(0.25f) ;   // Make upper half of body be 25% idle
    }
    else
    {
        // Remove masked anim
        if (IdleAnimState)
            m_PlayerInstance.DeleteMaskedAnim(MASKED_ANIM_ID_IDLE) ;
    }


    //==========================================================================
    // Setup weapon orientation
    //==========================================================================
    if (m_WeaponInstance.GetShape())
    {
        DEBUG_BEGIN_TIMER(CalcWeaponPos)

        DEBUG_BEGIN_TIMER(WeapGetWeaponMount)
        hot_point *MountHotPoint = m_WeaponInstance.GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
        DEBUG_END_TIMER(WeapGetWeaponMount)

        if (MountHotPoint)
        {
            // All forced aim?
            if (m_WeaponOrientBlend == 0)
            {
                // Get hand orientation
                DEBUG_BEGIN_TIMER(PlyrGetWeaponMount)
                vector3 AlivePos = m_PlayerInstance.GetHotPointWorldPos(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                radian3 AliveRot = m_WeaponDrawRot ;
                DEBUG_END_TIMER(PlyrGetWeaponMount)

                DEBUG_BEGIN_TIMER(SetWeaponPos)

                // Setup mount offset from hand
                vector3 MountOffset = MountHotPoint->GetPos() ;
                MountOffset.Rotate(AliveRot) ;

                // Take into account mount offset and pull back offset
                AlivePos -= MountOffset ;
                AlivePos += m_WeaponPullBackOffset ;

                // Set the weapon instance
                m_WeaponInstance.SetPos(AlivePos) ;
                m_WeaponInstance.SetRot(AliveRot) ;

                DEBUG_END_TIMER(SetWeaponPos)

            }
            else
            // All anim?
            if (m_WeaponOrientBlend == 1)
            {
                // Get hand orientation
                DEBUG_BEGIN_TIMER(PlyrGetWeaponMount)
                matrix4 mHand = m_PlayerInstance.GetHotPointL2W(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                DEBUG_END_TIMER(PlyrGetWeaponMount)

                DEBUG_BEGIN_TIMER(SetWeaponPos)

                // Calculate dead orientation
                radian3 DeadRot = mHand.GetRotation() ;
                vector3 DeadPos = mHand.GetTranslation() ;

                // Setup mount offset from hand
                vector3 MountOffset = MountHotPoint->GetPos() ;
                MountOffset.Rotate(DeadRot) ;

                // Take into account mount offset
                DeadPos -= MountOffset ;

                // Set the weapon position
                m_WeaponInstance.SetPos(DeadPos) ;
                m_WeaponInstance.SetRot(DeadRot) ;

                DEBUG_END_TIMER(SetWeaponPos)
            }
            else
            {
                // Blend between forced aim and anim
                vector3    AlivePos, DeadPos ;
                quaternion AliveRot, DeadRot ;

                // Get hand orientation
                DEBUG_BEGIN_TIMER(PlyrGetWeaponMount)
                matrix4 mHand = m_PlayerInstance.GetHotPointL2W(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                DEBUG_END_TIMER(PlyrGetWeaponMount)

                DEBUG_BEGIN_TIMER(SetWeaponPos)

                vector3 HandPos = mHand.GetTranslation() ;
                radian3 HandRot = mHand.GetRotation() ;

                // Calculate alive orientation
                AliveRot  = m_DrawRot ;
                AlivePos  = HandPos - (AliveRot * MountHotPoint->GetPos()) ;
                AlivePos += m_WeaponPullBackOffset ;

                // Calculate dead orientation
                DeadRot.Setup(HandRot) ;
                DeadPos = HandPos - (DeadRot * MountHotPoint->GetPos()) ;

                // Blend between alive(0) and death(1) orientation
                vector3 WeaponPos ;
                BlendLinear(AlivePos, DeadPos, m_WeaponOrientBlend, WeaponPos) ;

                quaternion WeaponRot ;
                WeaponRot = Blend(AliveRot, DeadRot, m_WeaponOrientBlend) ;

                // Set the weapon position
                m_WeaponInstance.SetPos(WeaponPos) ;
                m_WeaponInstance.SetRot(WeaponRot.GetRotation()) ;

                DEBUG_END_TIMER(SetWeaponPos)
            }
        }

        DEBUG_END_TIMER(CalcWeaponPos)
    }

    //==========================================================================
    // Setup backpack instance
    //==========================================================================

    DEBUG_BEGIN_TIMER(SetupBackpack)

    // Lookup current backpack shape 
    ASSERT(m_PackCurrentType >= INVENT_TYPE_START) ;
    ASSERT(m_PackCurrentType <= INVENT_TYPE_END) ;
    s32 BackpackShapeType = s_InventInfo[m_PackCurrentType].ShapeType ;
    
    // If no backpack, clear the shape
    if (BackpackShapeType == SHAPE_TYPE_NONE)
        m_BackpackInstance.SetShape(NULL) ;
    else
    {
        // Set shape if not already using this...
        if ( (!m_BackpackInstance.GetShape()) || (m_BackpackInstance.GetShape()->GetType() != BackpackShapeType) )
        {
            // Link the backpack to the player
            m_BackpackInstance.SetShape(tgl.GameShapeManager.GetShapeByType(BackpackShapeType)) ;
            if (m_BackpackInstance.GetShape())
            {
                ASSERTS(m_BackpackInstance.GetShape(), "shape not present yet - remove from invent!") ;
                m_BackpackInstance.SetParentInstance(&m_PlayerInstance, HOT_POINT_TYPE_BACKPACK_MOUNT) ;

                // Put back edge of backpack on player
                bbox    BBox = m_BackpackInstance.GetModel()->GetNode(0).GetBounds() ;
                vector3 Pos ;
                Pos.X = -(BBox.Min.X + BBox.Max.X) * 0.5f ;
                Pos.Y = -(BBox.Min.Y + BBox.Max.Y) * 0.5f ;
                Pos.Z = - BBox.Max.Z ;

                // Set pos
                m_BackpackInstance.SetPos(Pos) ;
            }
        }
    }

    DEBUG_END_TIMER(SetupBackpack)

   
    //==========================================================================
    // Update particle positions
    //==========================================================================

    // Update jet particles
    DEBUG_BEGIN_TIMER(UpdateJetParticles)
    for (i = 0 ; i < m_nJets ; i++)
    {
#if defined jnagle && 0
        // Reload particles.txt (just for use when I'm tweaking particles)
        if ( input_IsPressed( INPUT_PS2_BTN_R2, m_ControllerID ) )
        {
            m_JetPfx[i].Reset();
            tgl.ParticleLibrary.Kill();
            tgl.ParticleLibrary.Create( "data/particles/particles" );
            m_JetPfx[i].Create(&tgl.ParticlePool, &tgl.ParticleLibrary, PARTICLE_TYPE_JETPACK, m_WorldPos, vector3(0,1,0)) ;
        }
#endif

        // Start/stop jet particles
        m_JetPfx[i].SetEnabled(m_Jetting) ;
        
        // Update particle?
        if (m_Jetting)
        {
            // Update position
            ASSERT(m_JetHotPoint[i]) ;
            vector3 JetPos = m_PlayerInstance.GetHotPointWorldPos(m_JetHotPoint[i]) ;
            m_JetPfx[i].SetPosition(JetPos) ;

            // Setup default pitch from player look pitch -
            // Of course it is dampened and no -ve pitch is allowed (stops jet going into butt - ouch!)
            radian3 JetRot = m_PlayerDrawRot ;
            JetRot.Roll = 0 ;
            if (JetRot.Pitch > 0)
                JetRot.Pitch *= 0.2f ;
            else
            if (JetRot.Pitch < 0)
                JetRot.Pitch *= -0.07f ;

            // Always jet backwards slightly
            JetRot.Pitch += R_5 ;

            // Tweak the pitch for light and medium armor types so that the jet doesn't go through the running anims
            if (m_ArmorType != ARMOR_TYPE_HEAVY)
            {
                // Make the jet point away from the player when running
                f32 RunPitch = R_25 ;

                // Blending or playing a run?
                anim* CurAnim = m_PlayerInstance.GetCurrentAnimState().GetAnim() ;
                ASSERT(CurAnim) ;
                switch(CurAnim->GetType())
                {
                    case ANIM_TYPE_CHARACTER_FORWARD:
                    case ANIM_TYPE_CHARACTER_BACKWARD:
                    case ANIM_TYPE_CHARACTER_SIDE:
                    case ANIM_TYPE_CHARACTER_JUMP_UP:
                        JetRot.Pitch += RunPitch * (1.0f - m_PlayerInstance.GetBlend()) ;   // 0=all current
                        break ;
                }

                // Blending from a run?
                if (m_PlayerInstance.IsBlending())
                {
                    anim* BlendAnim = m_PlayerInstance.GetBlendAnimState().GetAnim() ;
                    switch(BlendAnim->GetType())
                    {
                        case ANIM_TYPE_CHARACTER_FORWARD:
                        case ANIM_TYPE_CHARACTER_BACKWARD:
                        case ANIM_TYPE_CHARACTER_SIDE:
                        case ANIM_TYPE_CHARACTER_JUMP_UP:
                            JetRot.Pitch += RunPitch * m_PlayerInstance.GetBlend() ;    // 1=all blend
                            break ;
                    }
                }
            }

            // Finally update the bloody emitter axis
            vector3 JetAxis = vector3(0,-1,0) ;
            JetAxis.Rotate(JetRot) ;
            m_JetPfx[i].GetEmitter(0).SetAxis( JetAxis );
            m_JetPfx[i].GetEmitter(0).UseAxis( TRUE );
        }
    }
    DEBUG_END_TIMER(UpdateJetParticles)

    DEBUG_END_TIMER(UpdateInstanceOrient)
}


//==============================================================================
// PLAYER RENDER FUNCTIONS
//==============================================================================

//==============================================================================

void player_object::PreRender( void )
{
    //DEBUG_BEGIN_STATS(1, 11)
    DEBUG_BEGIN_STATS(1, 1)

    //x_printfxy(10,m_ObjectID,"%f %f %f\n", m_WorldPos.X, m_WorldPos.Y, m_WorldPos.Z);
    //x_printf("Render %1d\n",m_ObjectID);
    //draw_BBox(m_WorldBBox, XCOLOR_RED );
    //bbox Box = m_WorldBBox ;
    //Box.Translate(vector3(0.1f,0,0)) ;
    //draw_BBox(Box, XCOLOR_GREEN );
    //draw_Label( m_WorldPos, XCOLOR_WHITE, "%d", m_ObjectID) ;

    // Clear screen size incase it's not rendered
    m_ScreenPixelSize = 0 ;

    // Hidden?
    if(m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    DEBUG_BEGIN_TIMER(PreRender)

    // Get current view
    const view *View = &tgl.GetView();
    ASSERT(View) ;

    // Calculate world bounds for drawing (includes all added objects)
    if (m_MaxScreenPixelSize == 0)  // First time rendered this frame?
    {
        DEBUG_BEGIN_TIMER(SetupDrawBBox)
        m_PlayerInstance.SetPos(m_DrawPos) ;
        m_PlayerInstance.SetRot(m_PlayerDrawRot) ;
        m_DrawBBox = m_PlayerInstance.GetWorldBounds() ;
        DEBUG_END_TIMER(SetupDrawBBox)
    }

    // Drawing view of player?
    if (m_View == View)
    {
        // Always make visible
        m_PlayerInView          = view::VISIBLE_PARTIAL ; // 2=Clip!
        m_ScreenPixelSize = 500 ;
    }
    else
    {
#ifdef PLAYER_TIMERS
        PlayerTimer[0].Start();
#endif
        // Draw?
        DEBUG_BEGIN_TIMER(IsVisibleCheck)
        {
            zone_set ZSet;
            ComputeZoneSet( ZSet, m_DrawBBox );
            m_PlayerInView = IsVisibleInZone( ZSet, m_DrawBBox ) ;
        }
        DEBUG_END_TIMER(IsVisibleCheck)
#ifdef PLAYER_TIMERS
        PlayerTimer[0].Stop();
#endif

        // Setup screen pixel size
        DEBUG_BEGIN_TIMER(CalcScreenSize)
        if (m_PlayerInView)
            m_ScreenPixelSize = View->CalcScreenSize(m_DrawPos, MIN_PIXEL_SIZE_PLAYER_RADIUS) ;
        else
            m_ScreenPixelSize = 0 ;
        DEBUG_END_TIMER(CalcScreenSize)
    }

    DEBUG_END_TIMER(PreRender)

    // Jet particle out of sync bug fix -

    // Decide on whether to update the player instances (slow) since this must
    // happen before the jet particles are drawn if the player is draw last
    // when transparent due to camera angle

    // Hidden?
    if (m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    // Player not in view volume?
    if (!m_PlayerInView)
        return;

    // Player too small to draw?
    if (m_ScreenPixelSize < MIN_PIXEL_SIZE_PLAYER)
        return ;

    // Just add a dummy sprite?
    if (m_ScreenPixelSize < MIN_PIXEL_SIZE_DUMMY)
        return ;

    // Make sure instances are in sync ready for drawing
    if (m_MaxScreenPixelSize == 0)  // First time rendered this frame?
    {
        // Update instances
#ifdef PLAYER_TIMERS
        PlayerTimer[1].Start();
#endif
        UpdateInstanceOrient() ;
#ifdef PLAYER_TIMERS
        PlayerTimer[1].Stop();
#endif
    }

    // Update max screen pixel size so logic can use it
    m_MaxScreenPixelSize = MAX(m_MaxScreenPixelSize, m_ScreenPixelSize) ;
}

//==============================================================================

volatile xbool ALWAYS_SHOW_TRIANGLES = FALSE;

void player_object::PostRender( void )
{
    s32 i ;
    
    // Show net stats?
    if (SHOW_NET_STATS)
    {
        irect Rect;
        Rect.l = 10 ;
        Rect.t = 70 + (m_ObjectID.Slot * 14) ;
        Rect.r = Rect.l + 512 ;
        Rect.b = Rect.t + 14 ;
        tgl.pUIManager->RenderText( 0, 
                                    Rect, 
                                    ui_font::h_left|ui_font::v_top, 
                                    XCOLOR_WHITE, 
                                    xfs( "P%d R:%3.3f L:%3.3f", 
                                         m_ObjectID.Slot, 
                                         m_NetSampleAverage.RecieveDeltaTime, 
                                         m_NetSampleAverage.SecsInPast ) );
    }
            //m = (s32)x_TicksToSec(x_GetTime()) / 60;
            //s = (s32)x_TicksToSec(x_GetTime()) % 60;
            //xstring Buildstring(xfs("%s - %d:%02d",BUILD_STRING,m,s) );


            //tgl.pUIManager->RenderText( 0, irect(0, 8,512,24), ui_font::v_center|ui_font::h_center, xcolor(255,255,255,192), Buildstring );
            //tgl.pUIManager->RenderText( 0, irect(0,24,512,40), ui_font::v_center|ui_font::h_center, xcolor(255,255,255,192), xfs("%d fps",(s32)(1000.0f/x_TicksToMs(DeltaTime)+0.5f)) );

    // TEMP - Death birds fly - arcer predict hit pos assert
    //bbox BotBBox ;
    //BotBBox.Min = vector3(1113.42f, 89.0776f, 503.334f) ;
    //BotBBox.Max = vector3(1114.42f, 91.6776f, 504.334f) ;
    //draw_ClearL2W() ;
    //draw_BBox(BotBBox, XCOLOR_RED) ;
    //vector3 FirePos = vector3(1114.66f, 89.9744f, 504.353f) ;
    //draw_Marker(FirePos, XCOLOR_YELLOW) ;


    // Hidden?
    if(m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    // Is player in the view?
    if( !m_PlayerInView )
        return;

    DEBUG_BEGIN_TIMER(PostRender)

    // Is player too far away to be rendered?
    if( (m_ScreenPixelSize >= MIN_PIXEL_SIZE_PLAYER) &&
        (m_ScreenPixelSize >= MIN_PIXEL_SIZE_DUMMY) )
    {
        // Render any weapon special fx (before culling player)?
        if (m_Armed)
        {
            DEBUG_BEGIN_TIMER(WeaponRenderFx)
            Weapon_RenderFx() ;
            DEBUG_END_TIMER(WeaponRenderFx)
        }

        // Draw particles?
        if ((m_PlayerInstance.GetColor().A != 0) && (m_ScreenPixelSize >= MIN_PIXEL_SIZE_PARTICLES))
        {
            DEBUG_BEGIN_TIMER(RenderParticles)

            // Draw jet particles
            for (i = 0 ; i < m_nJets ; i++)
            {
                if (m_JetPfx[i].IsEnabled())
                    m_JetPfx[i].RenderEffect() ;
            }

            DEBUG_END_TIMER(RenderParticles)
        }
    }

#ifdef PLAYER_TIMERS
PlayerTimer[2].Start();
#endif
    // Get current view
    const view *View = &tgl.GetView();//eng_GetView() ;
    ASSERT(View) ;

    // Get 2d screen size
    s32 X0,Y0,X1,Y1 ;
    View->GetViewport( X0, Y0, X1, Y1 ) ;
    s32 CX = (X0 + X1) >> 1 ;
    s32 CY = (Y0 + Y1) >> 1 ;
    s32 Width   = X1 - X0 ;
    s32 Height  = Y1 - Y0 ;

    // Do we have a line of sight to player's head?
    xbool LOS = FALSE;
    if (m_View != View) // Skip if drawing players own view
    {
        DEBUG_BEGIN_TIMER(LineOfSightTest)

        vector3  Start = View->GetPosition() ;
        vector3  End   = m_DrawPos + vector3(0, m_MoveInfo->BOUNDING_BOX.Max.Y, 0) ;
        
        // If really close, simply draw player!
        if ((End - Start).LengthSquared() < (0.001f * 0.001f))
            LOS = TRUE ;
        else
        {
            // Do ray line of sight check...
            s_Collider.ClearExcludeList() ;
            s_Collider.RaySetup( NULL, Start, End, m_ObjectID.GetAsS32(), TRUE) ;
            ObjMgr.Collide( COLLIDER_LOS_MASK, s_Collider ) ;
            if( (!s_Collider.HasCollided()) || ALWAYS_SHOW_TRIANGLES)
                LOS = TRUE;
        }
        DEBUG_END_TIMER(LineOfSightTest)
    }
    else
        LOS = TRUE ;    // Always draw in players own view
#ifdef PLAYER_TIMERS
PlayerTimer[2].Stop();
#endif

    // Draw health and energy bars
    DEBUG_BEGIN_TIMER(DrawHUD)
    if (m_ProcessInput && (m_WarriorID == tgl.WC))
    {
        // Show zoom info?
        if (m_ViewShowZoomTime > 0)
        {
            // Show view area in 3rd person
            if (m_ViewZoomState == VIEW_ZOOM_STATE_OFF)
            {
                f32 S   = GetViewZoomFactor() ;
                f32 fCX = (f32)CX ;
                f32 fCY = (f32)CY ;
                f32 W   = (f32)(Width  - 16) / (2.0f*S) ;
                f32 H   = (f32)(Height - 16) / (2.0f*S) ;
                f32 L   = 128.0f / (2.0f*S);

                // Draw outline
                draw_Begin(DRAW_LINES, DRAW_2D | DRAW_NO_ZBUFFER) ;
                draw_Color(xcolor(192,255,255,255)) ;

                // Top left
                draw_Vertex(fCX - W, fCY - H, 0) ;
                draw_Vertex(fCX - W + L, fCY - H, 0) ;
            
                draw_Vertex(fCX - W, fCY - H, 0) ;
                draw_Vertex(fCX - W, fCY - H + L, 0) ;
                            
                // Top right
                draw_Vertex(fCX + W, fCY - H, 0) ;
                draw_Vertex(fCX + W -L, fCY - H, 0) ;
            
                draw_Vertex(fCX + W, fCY - H, 0) ;
                draw_Vertex(fCX + W, fCY - H+L, 0) ;

                // Bot right
                draw_Vertex(fCX + W, fCY + H, 0) ;
                draw_Vertex(fCX + W-L, fCY + H, 0) ;

                draw_Vertex(fCX + W, fCY + H, 0) ;
                draw_Vertex(fCX + W, fCY + H-L, 0) ;

                // Bot left
                draw_Vertex(fCX - W, fCY + H, 0) ;
                draw_Vertex(fCX - W+L, fCY + H, 0) ;

                draw_Vertex(fCX - W, fCY + H, 0) ;
                draw_Vertex(fCX - W, fCY + H-L, 0) ;

                draw_End() ;
            }
        }
    }
    else
    {
        // Check line of sight with view
        if( (LOS) && (m_Health > 0) && (!DO_SCREEN_SHOTS))
        {
            vector3 Position = m_DrawPos;
            Position.Y += 3.0f;
            HudIcons.Add( hud_icons::PLAYER_INDICATOR, Position, m_TeamBits );
        }
/*    
        // Render the auto-aim hints
        if(( m_Health > 0 ) && ( !HUD_HideHUD ))
        {
            if( UseAutoAimHint )
            {
                RenderAutoAimHint( this );
            }
        }
*/
    }
    DEBUG_END_TIMER(DrawHUD)

    // Screen flash? (only on own view)
    if ( (m_View == View) && (m_ProcessInput) && (m_ScreenFlashCount > 0) )
    {
        // Draw alpha rect
        draw_Begin(DRAW_RECTS, DRAW_2D | DRAW_USE_ALPHA | DRAW_NO_ZBUFFER) ;
        draw_Color(xcolor(m_ScreenFlashColor.R, m_ScreenFlashColor.G, m_ScreenFlashColor.B, (u8)((f32)m_ScreenFlashColor.A * MIN(m_ScreenFlashCount,1.0f) ))) ;
        draw_Vertex((f32)X0, (f32)Y0, 0) ;
        draw_Vertex((f32)X1, (f32)Y1, 0) ;
        draw_End() ;
    }

    // Render weapon line of sight
    #ifdef DRAW_WEAPON_LOS
    {
        draw_Begin(DRAW_LINES, DRAW_USE_ALPHA) ;
        draw_Color(XCOLOR_WHITE) ;

        // If this asserts, you need to add a fire weapon hot point info to the max
        // weapon model file and re-export the .skel
        ASSERT(m_WeaponInstance.GetHotPointByType(HOT_POINT_TYPE_FIRE_WEAPON)) ;
        vector3 A = m_WeaponInstance.GetHotPointWorldPos(HOT_POINT_TYPE_FIRE_WEAPON) ;

        matrix4 Rot ;
        Rot.Identity() ;
        Rot = m_WeaponInstance.GetRot() ;
        vector3 B = A + (Rot * vector3(0,0,3)) ;

        draw_Vertex(A) ;
        draw_Vertex(B) ;

        draw_End() ;
    }
    #endif

    // draws direction arrows that the player wants to face
    #ifdef DRAW_DIRECTION
    {
        vector3 Start = m_WorldPos ;
        vector3 End   = vector3(0,0,1) ;
        End.Rotate(m_Rot) ;
        End += Start ;
        draw_Line(Start, End, XCOLOR_YELLOW) ;

        // Draw bot aim direction?
        if (GetType() == TYPE_BOT)
        {
            bot_object* pBot = (bot_object*)this ;
            End = Start + pBot->m_AimDirection ;
            draw_Line(Start, End, XCOLOR_RED) ;
        }
    }
    #endif


    #ifdef DRAW_VIEW_AND_WEAPON_FIRE_POS
        // Render view pos and weapon fire pos
        draw_Marker(m_ViewPos) ;
        draw_Marker(GetWeaponFirePos()) ;

        // Draw weapon collision
        draw_BBox(PlayerWeaponStartBBox, XCOLOR_BLUE) ;
        draw_BBox(PlayerWeaponEndBBox, XCOLOR_RED) ;

    #endif

    //draw_Marker( m_WorldPos );

    DEBUG_END_TIMER(PostRender)

    DEBUG_END_STATS
}

//==============================================================================

volatile xbool SHOW_AMBIENT_LIGHTING = FALSE;
volatile f32   PLAYER_AMBIENT_HUE    = 1.0f;
volatile f32   PLAYER_AMBIENT_INT    = 0.65f;
xcolor         PLAYER_ENV_COLOR_DIR  = xcolor( 180, 180, 180, 255 );

void player_object::UpdateLighting( void ) 
{
    xcolor Color = GetAmbientLighting( m_WorldPos, vector3( 0, -100.0f, 0 ), PLAYER_AMBIENT_HUE, PLAYER_AMBIENT_INT );

    // We now have ambient.  Blend into old ambient
    m_EnvAmbientColor.R = (u8)(( Color.R * 0.1f ) + ( m_EnvAmbientColor.R * 0.9f ));
    m_EnvAmbientColor.G = (u8)(( Color.G * 0.1f ) + ( m_EnvAmbientColor.G * 0.9f ));
    m_EnvAmbientColor.B = (u8)(( Color.B * 0.1f ) + ( m_EnvAmbientColor.B * 0.9f ));

    // Setup directional
    m_EnvDirColor = (xcolor)PLAYER_ENV_COLOR_DIR;
    m_EnvLightingDir = -tgl.Sun;

    // Render global lighting color swatch
    if( SHOW_AMBIENT_LIGHTING && m_HasInputFocus )
    {
        rect Rect( vector2(  20, 200 ), vector2(  70, 250 ));
        draw_Rect( Rect, Color,        FALSE );
        draw_Rect( Rect, XCOLOR_WHITE, TRUE  );
    }
}


//==============================================================================

void AddDummy( const vector3& Pos );

#ifdef DEBUG_COLLISION

vector3 PlayerCollPoints0[64] ;
s32     NPlayerCollPoints0 = 0 ;

vector3 PlayerCollPoints1[64] ;
s32     NPlayerCollPoints1 = 0 ;

vector3 PlayerCollPoints2[64] ;
s32     NPlayerCollPoints2 = 0 ;

#endif  // #ifdef DEBUG_COLLISION

void player_object::Render( void )
{
    // Draw debug points?
    #ifdef DEBUG_COLLISION

        // Begin draw?
        xbool InBegin = eng_InBeginEnd() ;
        if (!InBegin)
            eng_Begin() ;

        // Draw collision polys
        if (NPlayerCollPoints0)
            draw_NGon( PlayerCollPoints0, NPlayerCollPoints0, XCOLOR_BLUE, TRUE) ;
        if (NPlayerCollPoints1)
            draw_NGon( PlayerCollPoints1, NPlayerCollPoints1, XCOLOR_RED, TRUE) ;
        if (NPlayerCollPoints2)
            draw_NGon( PlayerCollPoints2, NPlayerCollPoints2, XCOLOR_GREEN, TRUE) ;

        // End draw?
        if (!InBegin)
            eng_End() ;

    #endif  // #ifdef DEBUG_COLLISION

    // Hidden?
    if(m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    // Player not in view volume?
    if (!m_PlayerInView)
        return;

    // Player too small to draw?
    if (m_ScreenPixelSize < MIN_PIXEL_SIZE_PLAYER)
        return ;

    // Just add a dummy sprite?
    if (m_ScreenPixelSize < MIN_PIXEL_SIZE_DUMMY)
    {
        AddDummy( m_DrawPos );
        return ;
    }

    DEBUG_BEGIN_TIMER(Render)


/*
    x_printfxy(0,m_ObjectID*2+0,"P(%f,%f,%f)",
        m_WorldPos.X,
        m_WorldPos.Y,
        m_WorldPos.Z);
    x_printfxy(0,m_ObjectID*2+1,"R(%f,%f,%f)",
        m_Rot.Pitch,
        m_Rot.Yaw,
        m_Rot.Roll);
*/


    // Uses shape library
    ASSERT(shape::InBeginDraw()) ;

    // Lookup damage texture + clut
    texture* DamageTexture ;
    s32      DamageClutHandle ;
    tgl.Damage.GetTexture( GetHealth(), DamageTexture, DamageClutHandle ) ;

    // Draw the player instance?
    if (m_PlayerInstance.GetShape())
    {
        xcolor  FogColor ;
        vector4 Color ;

        m_PlayerInstance.SetLightAmbientColor( m_EnvAmbientColor );
        m_PlayerInstance.SetLightColor( m_EnvDirColor );
        m_PlayerInstance.SetLightDirection( m_EnvLightingDir );

        // Set skin color
        m_PlayerInstance.SetTextureAnimFPS(0) ; // No texture anim
        m_PlayerInstance.SetTextureFrame((f32)m_SkinType) ;

        // Calc fog color
        FogColor = tgl.Fog.ComputeFog(m_WorldPos) ;

        // Spawning in?
        if (m_SpawnTime > 0)
        {
            // Get spawn ratio from 1->0
            f32 SpawnRatio        = m_SpawnTime / SPAWN_TIME ;
            
            // Get spawn fade out ratio from 1->0
            f32 SpawnFadeOutRatio = SpawnRatio * 3 ;
            if (SpawnFadeOutRatio > 1)
                SpawnFadeOutRatio = 1 ;

            // Calc sine speed
            f32 SineSpeed = 4 ;

            // Calc C to sine wave between 1 (normal) and 2 (bright)
            f32 C = 1.0f + (0.5f * x_sin(R_360 * SineSpeed * SpawnRatio)) ;
            
            // Calc C to be C when SpawnFadeOutRatio = 1, and 1 when SpawnFadeOutRatio=0
            C = (C * SpawnFadeOutRatio) + (1-SpawnFadeOutRatio) ;

            // Set color channels
            Color.X = C ;
            Color.Y = C ;
            Color.Z = C ;
            
            // Alpha fade in
            Color.W = 1 - SpawnRatio ;
        }
        else
        {
            // Normal color
            Color    = vector4(1,1,1,1) ;
        }

        if( (GetPlayerViewType() == VIEW_TYPE_3RD_PERSON) &&
            (m_HasInputFocus     == TRUE)                 &&
            (tgl.WC              == m_WarriorID) )
        {
            // Fade out player based on distance from camera position to look at point
            f32 T = 1.0f;
            
            if( m_CamDistanceT < FadeDistanceT )
            {
                T = m_CamDistanceT / FadeDistanceT;
            }
            
            Color.W *= T;
        }

        // Setup draw flags
        u32 PlayerDrawFlags = shape::DRAW_FLAG_FOG ; // environ map
        u32 WeaponDrawFlags = shape::DRAW_FLAG_FOG | shape::DRAW_FLAG_REF_WORLD_SPACE  ; // spec

        // Clip?
        if (m_PlayerInView == view::VISIBLE_PARTIAL)
        {
            PlayerDrawFlags |= shape::DRAW_FLAG_CLIP ;
            WeaponDrawFlags |= shape::DRAW_FLAG_CLIP ;
        }

        // Draw weapon unless dead or not armed
        if (        (m_ScreenPixelSize >= MIN_PIXEL_SIZE_WEAPON)
                &&  (m_WeaponInstance.GetShape())
                &&  (m_Health > 0)
                &&  (m_Armed) )
        {
            DEBUG_BEGIN_TIMER(DrawWeapon)

            // Prime z buffer if transparent
            if (Color.W != 1)
                WeaponDrawFlags |= shape::DRAW_FLAG_PRIME_Z_BUFFER | shape::DRAW_FLAG_SORT ;

            m_WeaponInstance.SetLightAmbientColor( m_EnvAmbientColor );
            m_WeaponInstance.SetLightColor( m_EnvDirColor );
            m_WeaponInstance.SetLightDirection( m_EnvLightingDir );

            m_WeaponInstance.SetColor   (Color) ;
            m_WeaponInstance.SetFogColor(FogColor) ;
            m_WeaponInstance.Draw( WeaponDrawFlags ) ;

            DEBUG_END_TIMER(DrawWeapon)
        }

        // Fade out player if not in 3rd person view
        // (all 3rd person m_ViewBlend == 1, all 1st person = m_ViewBlend=0)
        if( m_HasInputFocus && (tgl.WC==m_WarriorID) )
        {
            if ((m_ViewBlend != 1) && (m_Health > 0) )
                Color.W *= m_ViewBlend ;
        }
        
        // Store colors even though we might not draw
        // (so the post render functions can test color)
        m_PlayerInstance.SetColor    (Color) ;
        m_PlayerInstance.SetFogColor (FogColor) ;

        // Draw player and attached instances?
        if (Color.W > 0)
        {
            // Prime z buffer if transparent
            if (Color.W != 1)
                PlayerDrawFlags |= shape::DRAW_FLAG_PRIME_Z_BUFFER | shape::DRAW_FLAG_SORT ;

            // Draw player
            DEBUG_BEGIN_TIMER(DrawPlayer)
            ASSERT(m_PlayerInstance.GetShape()) ;
            m_PlayerInstance.Draw( PlayerDrawFlags | shape::DRAW_FLAG_TURN_ON_ANIM_MAT_DETAIL,
                                   DamageTexture, 
                                   DamageClutHandle ) ;
            DEBUG_END_TIMER(DrawPlayer)

            // Draw backpack?
            if (        (m_ScreenPixelSize >= MIN_PIXEL_SIZE_BACKPACK)
                    &&  (m_BackpackInstance.GetShape()) )
            {
                DEBUG_BEGIN_TIMER(DrawBackpack)
                m_BackpackInstance.SetLightAmbientColor( m_EnvAmbientColor );
                m_BackpackInstance.SetLightColor( m_EnvDirColor );
                m_BackpackInstance.SetLightDirection( m_EnvLightingDir );

                m_BackpackInstance.SetColor(Color) ;
                m_BackpackInstance.SetFogColor(FogColor) ;
                m_BackpackInstance.Draw( PlayerDrawFlags ) ;
                DEBUG_END_TIMER(DrawBackpack)
            }

            // Draw flag?
            if (        (m_ScreenPixelSize >= MIN_PIXEL_SIZE_FLAG)
                    &&  (m_FlagInstance.GetShape()) )
            {
                m_FlagInstance.SetModel( AnimateFlagShape( m_FlagInstance.GetShape(), tgl.NRenders, tgl.DeltaLogicTime ) ) ;
                m_FlagInstance.SetLightAmbientColor( m_EnvAmbientColor );
                m_FlagInstance.SetLightColor( m_EnvDirColor );
                m_FlagInstance.SetLightDirection( m_EnvLightingDir );

                vector4 FlagColor( Color );

                if( (GetPlayerViewType() == VIEW_TYPE_3RD_PERSON) &&
                    (m_HasInputFocus     == TRUE)                 &&
                    (tgl.WC              == m_WarriorID) )
                    FlagColor.W *= Flag3rdPersonAlpha;

                m_FlagInstance.SetColor   (FlagColor) ;
                m_FlagInstance.SetFogColor(FogColor) ;
                m_FlagInstance.Draw( PlayerDrawFlags ) ;
            }
        }

        // Draw bounds
        #ifdef DEBUG_COLLISION
        {
            draw_ClearL2W() ;
            draw_BBox(m_WorldBBox, xcolor(255,255,255,255)) ;
            //draw_BBox(bbox(m_WorldPos,0.05f),XCOLOR_YELLOW);
        }
        #endif
    }

    DEBUG_END_TIMER(Render)
}

//==============================================================================

void player_object::DebugRender( void ) 
{
    // Hidden?
    if(m_PlayerState == PLAYER_STATE_CONNECTING)
        return;

    // Draw debug info
    object::DebugRender();
    draw_Marker( m_DrawPos );
    draw_BBox( m_DrawBBox, XCOLOR_WHITE ) ;
}

//==============================================================================

void test_draw_player()
{
    static vector3          Pos(0,0,5) ;
    static radian3          Rot(0,0,0) ;
    static shape_instance   Inst ;     

    // Setup instance? (just do once)
    if (Inst.GetShape() == NULL)
    {
        // Setup character to draw
        player_object::character_type CharType  = player_object::CHARACTER_TYPE_BIO ;
        player_object::armor_type     ArmorType = player_object::ARMOR_TYPE_LIGHT ;

        // Lookup useful info...
        player_object::character_info Info = player_object::GetCharacterInfo(CharType, ArmorType) ;
        shape* Shape = tgl.GameShapeManager.GetShapeByType(Info.ShapeType) ;

        // Shapes not loaded yet?
        if (Shape == NULL)
            return ;

        // Set shape
        Inst.SetShape(Shape) ;

        // Set light
        Inst.SetLightAmbientColor(xcolor(30,30,30)) ;
        Inst.SetLightColor(XCOLOR_WHITE) ;
        Inst.SetLightDirection(vector3(0,0,1), TRUE) ;

        // Set color
        Inst.SetColor(XCOLOR_WHITE) ;

        // Setup anim
        Inst.SetAnimByType(ANIM_TYPE_CHARACTER_IDLE) ;
    }

    // Put infront of camera
    const view  *View   = eng_GetView() ;
    vector3     ViewPos = View->ConvertV2W(Pos) ;
    Inst.SetPos(ViewPos) ;
    Inst.SetRot(Rot) ;

    // Spin
    Rot.Yaw += tgl.DeltaLogicTime * R_10 ;

    // Advance anim
    Inst.Advance(tgl.DeltaLogicTime) ;

    // Finally draw it
    shape::BeginDraw() ;
    Inst.Draw(shape::DRAW_FLAG_CLIP) ;
    shape::EndDraw() ;
}

//==============================================================================
