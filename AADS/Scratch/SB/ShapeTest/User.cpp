#include "Entropy.hpp"
#include "Clipper.hpp"

#include "Shape/ShapeInstance.hpp"
#include "shape/ShapeManager.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Damage/Damage.hpp"

//efine DEBUG_TURN_STATS_ON
#include "Shape/DebugUtils.hpp"




//=========================================================================
// Defines
//=========================================================================

#ifdef TARGET_PC

#define HIT_LEFT            INPUT_KBD_LEFT
#define HIT_RIGHT           INPUT_KBD_RIGHT
#define HIT_UP              INPUT_KBD_UP
#define HIT_DOWN            INPUT_KBD_DOWN
#define HIT_QUIT            INPUT_KBD_ESCAPE
#define HIT_NEXT_ANIM       INPUT_KBD_SPACE
#define HIT_TOGGLE_VIEW     INPUT_KBD_TAB
#define HIT_SHIFT           INPUT_KBD_LSHIFT
#define HIT_NEXT_SHAPE      INPUT_KBD_N
#define HIT_NEXT_MODEL      INPUT_KBD_M
#define HIT_WIREFRAME       INPUT_KBD_P
#define HIT_TOGGLE_TEXT     INPUT_KBD_T

#endif

#ifdef TARGET_PS2

#define HIT_LEFT            INPUT_PS2_BTN_L_LEFT 
#define HIT_RIGHT           INPUT_PS2_BTN_L_RIGHT 
#define HIT_UP              INPUT_PS2_BTN_L_UP 
#define HIT_DOWN            INPUT_PS2_BTN_L_DOWN 
#define HIT_QUIT            INPUT_PS2_BTN_START
#define HIT_NEXT_ANIM       INPUT_PS2_BTN_SELECT
#define HIT_TOGGLE_VIEW     INPUT_PS2_BTN_TRIANGLE
#define HIT_SHIFT           INPUT_PS2_BTN_CIRCLE
#define HIT_NEXT_SHAPE      INPUT_PS2_BTN_L1
#define HIT_NEXT_MODEL      INPUT_PS2_BTN_L2
#define HIT_WIREFRAME       INPUT_PS2_BTN_CROSS
#define HIT_TOGGLE_TEXT     INPUT_PS2_BTN_SQUARE


#endif



//=========================================================================
// Data
//=========================================================================

//#define X_TOTAL	6
//#define Z_TOTAL	16
//#define X_TOTAL	4
//#define Z_TOTAL	4
#define X_TOTAL	1
#define Z_TOTAL	1
#define SPACE	2.0f


#define MASKED_ANIM_ID_IDLE             0

#define ADDITIVE_ANIM_ID_BODY_LOOK_UD   0
#define ADDITIVE_ANIM_ID_HEAD_LOOK_LR   1
#define ADDITIVE_ANIM_ID_HEAD_LOOK_UD   2

#define VIEW_MIN_PITCH                  -R_80
#define VIEW_MAX_PITCH                  R_80
static radian Pitch=0 ;
static xbool  In1stPerson=FALSE, Triggered1stPerson=FALSE ;



static view View, EyeView ;
static s32  s_Anim=0;
static shape_instance *s_InstanceArray[X_TOTAL][Z_TOTAL] ;

shape_manager   s_ShapeManager ;
damage          s_Damage ;


// Search for a player instance
shape_instance* s_PlayerInstance = NULL ;
shape_instance* s_WeaponInstance = NULL ;
shape_instance* s_VehicleInstance = NULL ;
shape_instance* s_PackInstance   = NULL ;
shape_instance* s_FlagInstance   = NULL ;
shape_instance* s_TurretInstance = NULL ;
shape_instance* s_BarrelInstance = NULL ;
shape_instance* s_HumanInventInstance = NULL ;
shape_instance* s_Projector = NULL ;
shape*          s_FlagShape      = NULL ;
s32             s_ShapeIndex     = 0 ;    

f32 s_Health=1 ;

f32 s_FOV  = 70.0f ;
f32 s_Far  = 1000.0f ;
f32 s_Near = 0.1f ;
xbool s_Clipping=TRUE;//FALSE;
xbool s_Cull=TRUE;//FALSE;//TRUE;



xbool s_DrawDebugText=FALSE;
xbool s_WireFrame=FALSE ;
xbool s_DrawOrigin=TRUE;//FALSE ;

xbool s_IncRot=FALSE ;


#undef BEGIN_LABEL_SET
#undef LABEL
#undef END_LABEL_SET


// Begins enum set
#define BEGIN_LABEL_SET(__name__)   const char* __name__##Strings[] = {

// Adds a label to enum set
#define LABEL(__name__, __desc__)   __desc__,

// Ends enum set
#define END_LABEL_SET(__name__) } ;


#undef __ANIM_TYPES_H__
#include "LabelSets/AnimTypes.hpp"


#undef __HOT_POINT_TYPES_H__
#include "LabelSets/HotPointTypes.hpp"

#undef __SHAPE_TYPES_H__
#include "LabelSets/ShapeTypes.hpp"

#undef __MODEL_TYPES_H__
#include "LabelSets/ModelTypes.hpp"



//=========================================================================
// Functions
//=========================================================================

xbool CollectInput ( void )
{
    s32 x,z,i ;
    s32 Quit=FALSE ;
    static s32 Debounce=0;
    while( input_UpdateState() )
    {
        //s32 i;

        Quit |= input_WasPressed(HIT_QUIT) ;

        // Change view?
        if (input_WasPressed(HIT_TOGGLE_VIEW))
        {
            In1stPerson ^= TRUE ;

            // Clear rotations?
            if (In1stPerson)
            {
                Triggered1stPerson=TRUE ;
                Pitch=0;
			    for (x = 0 ; x < X_TOTAL ; x++)
			    {
				    for (z = 0 ; z < Z_TOTAL ; z++)
				    {
                        shape_instance* Instance = s_InstanceArray[x][z] ;
                        if (!Instance->GetParentInstance())
                        {
                            radian3 Rot ;
                            Rot.Zero() ;
                            vector3 Pos ;
                            Pos.Zero() ;
                            Instance->SetRot(Rot) ;
                            Instance->SetPos(Pos) ;
				        }
			        }
                }
            }
        }

        // Next anim?
        if (input_WasPressed(HIT_NEXT_ANIM))
        {
            s_Anim++ ;
        }

        #ifdef TARGET_PC
        if (!In1stPerson)
        {
             // Move view using keyboard and mouse
            // WASD - forward, left, back, right
            // RF - up, down
            // MouseRightButton - 4X speed
            f32 S   = 0.125f;
            if( input_IsPressed(INPUT_MOUSE_BTN_R) ) S *= 4.0f;
            if( input_IsPressed(INPUT_KBD_W) ) View.Translate( vector3(0,0,S),  view::VIEW );
            if( input_IsPressed(INPUT_KBD_S) ) View.Translate( vector3(0,0,-S), view::VIEW );
            if( input_IsPressed(INPUT_KBD_A) ) View.Translate( vector3(S,0,0),  view::VIEW );
            if( input_IsPressed(INPUT_KBD_D) ) View.Translate( vector3(-S,0,0), view::VIEW );
            if( input_IsPressed(INPUT_KBD_R) ) View.Translate( vector3(0,S,0),  view::VIEW );
            if( input_IsPressed(INPUT_KBD_F) ) View.Translate( vector3(0,-S,0), view::VIEW );
            //radian Pitch;
            //radian Yaw;
            //View.GetPitchYaw( Pitch, Yaw );
            //Pitch += input_GetValue(INPUT_MOUSE_Y_REL)*0.005f;
            //Yaw   -= input_GetValue(INPUT_MOUSE_X_REL)*0.005f;
            //View.SetRotation( radian3(Pitch,Yaw,0) );

			for (x = 0 ; x < X_TOTAL ; x++)
			{
				for (z = 0 ; z < Z_TOTAL ; z++)
				{
                    shape_instance* Instance = s_InstanceArray[x][z] ;

                    if (!Instance->GetParentInstance())
                    {
                        // Skip?
                        if ((Triggered1stPerson) && (Instance == s_WeaponInstance))
                            continue ;

    					radian3 Rot = Instance->GetRot() ;
					    Rot.Pitch += input_GetValue(INPUT_MOUSE_Y_REL)*0.005f;
					    Rot.Yaw   += input_GetValue(INPUT_MOUSE_X_REL)*0.005f;
					    Instance->SetRot(Rot) ;
                    }
				}
			}
        }

        // Look up and down
		Pitch += input_GetValue(INPUT_MOUSE_Y_REL)*0.01f;
        if (Pitch < VIEW_MIN_PITCH)
            Pitch = VIEW_MIN_PITCH ;
        else
        if (Pitch > VIEW_MAX_PITCH)
            Pitch = VIEW_MAX_PITCH ;

        #endif

        #ifdef TARGET_PS2
        if (!In1stPerson)
        {
            vector3 Translate = vector3(0,0,0) ;

            #if 0
                // Move view using keyboard and mouse
                // WASD - forward, left, back, right
                // RF - up, down
                // MouseRightButton - 4X speed
                f32 S   = 0.125f;
                if( input_IsPressed(INPUT_PS2_BTN_CROSS) ) S *= 4.0f;
            
			    if( input_IsPressed(INPUT_PS2_BTN_L_UP) )    View.Translate( vector3(0,0,S),  view::VIEW );
                if( input_IsPressed(INPUT_PS2_BTN_L_DOWN) ) View.Translate( vector3(0,0,-S), view::VIEW );
                if( input_IsPressed(INPUT_PS2_BTN_L_LEFT) ) View.Translate( vector3(S,0,0),  view::VIEW );
                if( input_IsPressed(INPUT_PS2_BTN_L_RIGHT) ) View.Translate( vector3(-S,0,0), view::VIEW );
            
			    if( input_IsPressed(INPUT_PS2_BTN_R1) ) View.Translate( vector3(0,S,0),  view::VIEW );
                if( input_IsPressed(INPUT_PS2_BTN_R2) ) View.Translate( vector3(0,-S,0), view::VIEW );
                //radian Pitch;
                //radian Yaw;
                //View.GetPitchYaw( Pitch, Yaw );
                //Pitch += input_GetValue(INPUT_MOUSE_Y_REL)*0.005f;
                //Yaw   -= input_GetValue(INPUT_MOUSE_X_REL)*0.005f;
                //View.SetRotation( radian3(Pitch,Yaw,0) );
            #else
                 // Move view using keyboard and mouse
                // WASD - forward, left, back, right
                // RF - up, down
                // MouseRightButton - 4X speed
                f32 S   = 0.125f;

                if( input_IsPressed(INPUT_PS2_BTN_CROSS) ) S *= 4.0f;
            
			    if( input_IsPressed(INPUT_PS2_BTN_L_UP) )    Translate  += vector3(0,0,S) ;
                if( input_IsPressed(INPUT_PS2_BTN_L_DOWN) ) Translate  += vector3(0,0,-S) ;
                if( input_IsPressed(INPUT_PS2_BTN_L_LEFT) ) Translate  += vector3(S,0,0) ;
                if( input_IsPressed(INPUT_PS2_BTN_L_RIGHT) ) Translate  += vector3(-S,0,0) ;
            
			    if( input_IsPressed(INPUT_PS2_BTN_R1) ) Translate  += vector3(0,S,0) ;
                if( input_IsPressed(INPUT_PS2_BTN_R2) ) Translate  += vector3(0,-S,0) ;
                //radian Pitch;
                //radian Yaw;
                //View.GetPitchYaw( Pitch, Yaw );
                //Pitch += input_GetValue(INPUT_MOUSE_Y_REL)*0.005f;
                //Yaw   -= input_GetValue(INPUT_MOUSE_X_REL)*0.005f;
                //View.SetRotation( radian3(Pitch,Yaw,0) );
        #endif

			for (x = 0 ; x < X_TOTAL ; x++)
			{
				for (z = 0 ; z < Z_TOTAL ; z++)
				{
                    shape_instance* Instance = s_InstanceArray[x][z] ;
    
                    if (!Instance->GetParentInstance())
                    {
                        // Skip?
                        if ((Triggered1stPerson) && (Instance == s_WeaponInstance))
                            continue ;
                        
                        // Translate
                        vector3 Pos = Instance->GetPos() ;
                        Pos += Translate ;
                        
                        // Rotate
                        f32 RotSpeed = 0.1f ;

					    radian3 Rot = Instance->GetRot() ;
					    Rot.Yaw   += input_GetValue(INPUT_PS2_STICK_RIGHT_X)*RotSpeed;
					    Rot.Pitch += input_GetValue(INPUT_PS2_STICK_RIGHT_Y)*RotSpeed;
					    Rot.Yaw   += input_GetValue(INPUT_PS2_STICK_LEFT_X)*RotSpeed;
					    Rot.Pitch += input_GetValue(INPUT_PS2_STICK_LEFT_Y)*RotSpeed;

                        if (s_IncRot)
                        {
                            Rot.Yaw   += R_3/10.0f ;
                            Rot.Pitch += R_4/10.0f ;
                            Rot.Roll  += R_5/10.0f ;
                        }

                        // Set new orient
                        Instance->SetPos(Pos) ;
					    Instance->SetRot(Rot) ;
                    }
				}
			}
        }

        // Look up and down
		Pitch += input_GetValue(INPUT_PS2_STICK_RIGHT_Y)*0.1f;
        if (Pitch < VIEW_MIN_PITCH)
            Pitch = VIEW_MIN_PITCH ;
        else
        if (Pitch > VIEW_MAX_PITCH)
            Pitch = VIEW_MAX_PITCH ;

        #endif


    }
    
    int ModelDir=0 ;
    int ShapeDir=0 ;

    // Prev shape/model?
    if (input_IsPressed(HIT_SHIFT))
    {
        // Prev shape?
        if (input_WasPressed(HIT_NEXT_SHAPE))
            ShapeDir = -1 ;
        
        // Prev model?
        if (input_WasPressed(HIT_NEXT_MODEL))
            ModelDir = -1 ;
    }
    else
    {
        // Next shape?
        if (input_WasPressed(HIT_NEXT_SHAPE))
            ShapeDir = 1 ;
        
        // Next model?
        if (input_WasPressed(HIT_NEXT_MODEL))
            ModelDir = 1 ;
    }

    // Next/prev shape?
    if (ShapeDir != 0)
    {
        // Update shape index
        s_ShapeIndex += ShapeDir ;
        if (s_ShapeIndex < 0)
            s_ShapeIndex = s_ShapeManager.GetShapeCount()-1 ;
        else
        if (s_ShapeIndex >= s_ShapeManager.GetShapeCount())
            s_ShapeIndex = 0 ;

        // Update instances
		for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
                // Lookup current info
                shape_instance* Instance = s_InstanceArray[x][z] ;
                shape* Shape = Instance->GetShape() ;
                model* Model = Instance->GetModel() ;
                
                // Find current model index
                s32    ModelIndex=0 ;
                for (i = 0 ; i < Shape->GetModelCount() ; i++)
                {
                    if (Shape->GetModelByIndex(i) == Model)
                    {
                        ModelIndex = i ;
                        break ;
                    }
                }

                // Get new shape
                Shape = s_ShapeManager.GetShapeByIndex(s_ShapeIndex) ;
                ASSERT(Shape) ;
                Instance->SetShape(Shape) ;

                // Try set the same model as last time
                if (ModelIndex < 0)
                    ModelIndex = Shape->GetModelCount() - 1 ;
                else
                if (ModelIndex >= Shape->GetModelCount())
                    ModelIndex = 0 ;
                Instance->SetModel(Shape->GetModelByIndex(ModelIndex)) ;
            }
        }
    }

    // Next/prev shape?
    if (ModelDir != 0)
    {
        // Update instances
		for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
                // Lookup current info
                shape_instance* Instance = s_InstanceArray[x][z] ;
                shape* Shape = Instance->GetShape() ;
                model* Model = Instance->GetModel() ;
                
                // Find current model index
                s32    ModelIndex=0 ;
                for (i = 0 ; i < Shape->GetModelCount() ; i++)
                {
                    if (Shape->GetModelByIndex(i) == Model)
                    {
                        ModelIndex = i ;
                        break ;
                    }
                }

                // Update index
                ModelIndex += ModelDir ;
                if (ModelIndex < 0)
                    ModelIndex = Shape->GetModelCount() - 1 ;
                else
                if (ModelIndex >= Shape->GetModelCount())
                    ModelIndex = 0 ;

                // Finally, set new model
                Instance->SetModel(Shape->GetModelByIndex(ModelIndex)) ;
            }
        }
    }

    // Toggle wire frame?
    if (input_WasPressed(HIT_WIREFRAME))
        s_WireFrame ^= TRUE ;
    shape::SetWireFrame(s_WireFrame) ;

    // Toggle text?
    if (input_WasPressed(HIT_TOGGLE_TEXT))
        s_DrawDebugText ^= TRUE ;

    // Quit?
    return ((input_IsPressed( HIT_QUIT ) == FALSE) && (!Quit) ) ;
}

//=========================================================================


void    eng_GetStats(s32 &Count, f32 &CPU, f32 &GS, f32 &INT, f32 &FPS) ;



void AppMain( s32 argc, char* argv[] )
{
	s32				x,z,i,j,v,h ;

#if 0
    for (s32 p = 0 ; p < 2 ; p++)
    {
        f32 dt ;
        if (p == 0)
            dt=0.5f ;
        else
            dt=0.125f ;

        f32 Time=0 ;
        f32 Pos=0;
        f32 Vel=0;
        f32 Accel=0.5f ;

        x_DebugMsg("/nDeltaTime:%f/n", dt) ;
        while(Time <= 10.0f)
        {
            x_DebugMsg("Time:%f Pos:%f Vel:%f Accel:%f/n", Time, Pos, Vel, Accel) ;
            Time += dt ;
            Vel += Accel*dt;
            Pos += Vel*dt ;
        }
    }
#endif


#if 1

    f32 fTest ;


    *(u32*)&fTest = 0 ;
#endif

#if 0

// CONVERTING FLOAT TO INT (SCREWS UP TOP 10 BITS OF int THOUGH...)

    f32 f ;
    s32 i ;

    // convert float to int (screws up top 10 bits of int)
    f = 100.0f ;
    f += (3<<22) ;
    i = *(s32 *)&f ;



// CONVERTING INT TO FLOAT!!!
/*

//You want a biased offset of something like ((23-exponent)+0x7f)<<23 +
//0x400000

//Where exponent =
//0 for 0-bit signed fixed point
//4 for 4-bit signed fixed point etc

//Then in your transformation matrix you need to subtract 1.5 *
//(2^(23-exponent) to normalise.

//Mark

    u32 FracBits=0 ;
    s32 iV = -32769 ;  // 0-511 (9bits)
    u32 iT = iV + ((((23-FracBits) + 0x7f)<<23) + 0x400000) ;   // USE VIF TO ADD THIS!!!
    f32 fV ;
    fV = *(f32*)&iT ;
    fV -= 1.5f * (1 << (23-FracBits)) ;     // Do this in the matrix!

*/

#endif



    f32 f ;
    //s32 i ;

    // convert float to int (screws up top 10 bits of int)
    f = 100.0f ;
    f += (3<<22) ;
    //i = *(s32 *)&f ;

    //f = *(f32*)&i ;
    f += (3<<22) ;









    // Init engine
    eng_Init();

#define PRINT_STRUC_SIZE(__s__) x_DebugMsg("sizeof(%s) = %d bytes\n", #__s__, sizeof(__s__)) ;

    PRINT_STRUC_SIZE(shape_bin_file_class) ;
    PRINT_STRUC_SIZE(anim) ;
    PRINT_STRUC_SIZE(key_set) ;
    PRINT_STRUC_SIZE(hot_point) ;
    PRINT_STRUC_SIZE(material) ;
    PRINT_STRUC_SIZE(material_dlist) ;
    PRINT_STRUC_SIZE(model) ;
    PRINT_STRUC_SIZE(node) ;
    PRINT_STRUC_SIZE(shape) ;
    PRINT_STRUC_SIZE(shape_texture_ref) ;
    PRINT_STRUC_SIZE(shape_instance) ;

    shape::SetPrintfFunc(shape::DefaultPrintf) ;

    //shape_manager::mode   Mode = shape_manager::MODE_LOAD_ASCII ;
    shape_manager::mode   Mode = shape_manager::MODE_LOAD_BINARY ;

    // Load the shapes
    s_ShapeManager.AddAndLoadShapes("Data/Shapes.txt", shape_manager::MODE_LOAD_ASCII) ;
    //s_ShapeManager.AddAndLoadShapes("C:/Projects/T2/Demo1/Data/Shapes/GameShapes.txt", Mode) ;
    //s_ShapeManager.AddAndLoadShapes("C:/Projects/T2/Demo1/Data/Shapes/IceShapes.txt", Mode) ;
    //s_ShapeManager.AddAndLoadShapes("C:/Projects/T2/Demo1/Data/Shapes/DesertShapes.txt", Mode) ;
    //s_ShapeManager.AddAndLoadShapes("C:/Projects/T2/Demo1/Data/Shapes/LavaShapes.txt", Mode) ;
    //s_ShapeManager.AddAndLoadShapes("C:/Projects/T2/Demo1/Data/Shapes/LushShapes.txt", Mode) ;
    


    //eng_SetBackColor(XCOLOR_WHITE) ;
    eng_SetBackColor(xcolor(0,63,0));


    // Setup global light
    shape::SetLightAmbientColor (vector4(0.2f, 0.2f, 0.2f, 1.0f)) ;
    shape::SetLightColor        (vector4(1.0f, 1.0f, 1.0f, 1.0f)) ;
    shape::SetLightDirection    (vector3(0.0f, 0.0f, -1.0f), TRUE) ;
    shape::SetFogColor          (xcolor(0,0,0)) ;

    // Test self illum materials
    //shape::SetLightAmbientColor (vector4(0.1f, 0.1f, 0.1f, 1.0f)) ;
    //shape::SetLightColor        (vector4(0.1f, 0.1f, 0.1f, 1.0f)) ;

                                     
	// Set start anim
	//s_Anim = s_ShapeManager.GetShapeByIndex(0)->GetAnimCount() -1 ;

    // Create instance of the shapes
    s32 ShapeIndex = 0 ;
    s32 Col = 0 ;
	for (z = 0 ; z < Z_TOTAL ; z++)
	{
    	for (x = 0 ; x < X_TOTAL ; x++)
		{
			shape           *Shape = s_ShapeManager.GetShapeByIndex(ShapeIndex) ;
			shape_instance  *Instance = new shape_instance(Shape) ;

            s_InstanceArray[x][z] = Instance ;
			
            Instance->SetPos( vector3(0,0,0) );

            // breaks strike
            vector3 Pos ;
            radian3 Rot ;

            // set1
            *(u32*)&Pos.X = 0x3ec00000 ;
            *(u32*)&Pos.Y = 0xbe000000 ;
            *(u32*)&Pos.Z = 0x40ac0000 ;

            *(u32*)&Rot.Pitch = 0xbef4b052 ;
            *(u32*)&Rot.Yaw   = 0x40484369 ;
            *(u32*)&Rot.Roll  = 0x00000000 ;

            //set2
            *(u32*)&Pos.X = 0x3ec00000 ;
            *(u32*)&Pos.Y = 0xbe000000 ;
            *(u32*)&Pos.Z = 0x40c40000 ;

            *(u32*)&Rot.Pitch = 0xbeed9899 ;
            *(u32*)&Rot.Yaw   = 0x4043ff69 ;
            *(u32*)&Rot.Roll  = 0x00000000 ;

            //set3
            *(u32*)&Pos.X = 0x3f200000 ;
            *(u32*)&Pos.Y = 0xbe800000 ;
            *(u32*)&Pos.Z = 0x40700000 ;

            *(u32*)&Rot.Pitch = 0xbe82e34f ;
            *(u32*)&Rot.Yaw   = 0x403d6d28 ;
            *(u32*)&Rot.Roll  = 0x00000000 ;

            //cracks in ass tank
            *(u32*)&Pos.X = 0x3e000000 ;
            *(u32*)&Pos.Y = 0x00000000 ;
            *(u32*)&Pos.Z = 0x40ac0000 ;

            *(u32*)&Rot.Pitch = 0x42035924 ;
            *(u32*)&Rot.Yaw   = 0x424ac331 ;
            *(u32*)&Rot.Roll  = 0x00000000 ;

            //Instance->SetPos(Pos) ;
            //Instance->SetRot(Rot) ;


            //Instance->SetRot(radian3(R_180,0,0)) ;

			f32 Scale = 2.0f / Instance->GetWorldBounds().GetRadius() ;
			//f32 Scale = 1.0f ;
            //Instance->SetScale(vector3(Scale, Scale, Scale)) ;

            if (Shape->GetAnimCount())
                Instance->SetAnimByIndex(Shape->GetAnimCount()-1, 1.0f) ;

            xcolor Color ;
            switch(Col)
            {
                case 0: Color = XCOLOR_RED ;    break ;
                case 1: Color = XCOLOR_GREEN ;  break ;
                case 2: Color = XCOLOR_BLUE ;   break ;
            }
            //Instance->SetColor(Color) ;
            //Instance->SetColor(xcolor(255,255,255,2)) ;
            
            if ((x == 0) && (z == 0))
            {
                //Instance->SetColor(vector4(2,2,2,2)) ;

                //Color = XCOLOR_GREY;
                //Color.R = 0 ;
                //Color.G = 0 ;
                //Color.B = 0 ;
                //Color.A = 255 ;
                //Instance->SetColor(Color) ;
            }

            //Instance->SetColor(XCOLOR_RED) ;

            // Next shape
            if (++ShapeIndex == s_ShapeManager.GetShapeCount())
                ShapeIndex = 0 ;

            // Next color
            Col++ ;
            if (Col == 3)
                Col = 0 ;

		}
	}    

    // Do 2 passes, since instances may in wrong order
    for (s32 p = 0 ; p < 2 ; p++)
    {
        // Search and attach instances
        for (z = 0 ; z < Z_TOTAL ; z++)
	    {
    	    for (x = 0 ; x < X_TOTAL ; x++)
		    {
                shape_instance* Instance = s_InstanceArray[x][z] ;
                ASSERT(Instance) ;

                shape*          Shape    = Instance->GetShape() ;
                ASSERT(Shape) ;

                // Temp
                //Instance->SetTextureAnimFPS(0) ;
                //Instance->SetTextureFrame(0) ;

                // Scale down some scenery
                if (Shape->GetType() >= SHAPE_TYPE_SCENERY_BORG1)
                    Instance->SetScale(vector3(0.125f, 0.125f, 0.125f)) ;
            
                // Look for player
                if (        (Shape->GetType() == SHAPE_TYPE_CHARACTER_LIGHT_FEMALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_MEDIUM_FEMALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_HEAVY_FEMALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_LIGHT_MALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_MEDIUM_MALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_HEAVY_MALE)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_LIGHT_BIO)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_MEDIUM_BIO)
                        ||  (Shape->GetType() == SHAPE_TYPE_CHARACTER_HEAVY_BIO) )
                {
                    s_PlayerInstance = Instance ;

                    // Keep on first skin
                    s_PlayerInstance->SetTextureAnimFPS(0) ;
                    s_PlayerInstance->SetTextureFrame(0) ;

                    // Add layered anims
                    anim_state *BodyUD   = NULL ;
                    anim_state *HeadLR   = NULL ;
                    anim_state *HeadUD   = NULL ;
			        anim_state *IdleAnimState = NULL ;
                
                    BodyUD   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;
                    //HeadLR   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_LR) ;
                    //HeadUD   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_UD) ;
			        //IdleAnimState = s_PlayerInstance->AddMaskedAnim(MASKED_ANIM_ID_IDLE) ;

                    if (BodyUD)
                    {
                        BodyUD->SetSpeed(0.1f) ;

                        if (Shape->GetAnimByType(ANIM_TYPE_CHARACTER_ROCKET_LOOK_SOUTH_NORTH))
                        {
                            s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_BODY_LOOK_UD, ANIM_TYPE_CHARACTER_ROCKET_LOOK_SOUTH_NORTH) ;
                            //s_PlayerInstance->SetAnimByType(ANIM_TYPE_CHARACTER_ROCKET_POSE) ;
                        }
                        else
                        if (Shape->GetAnimByType(ANIM_TYPE_CHARACTER_SNIPER_LOOK_SOUTH_NORTH))
                        {
                            s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_BODY_LOOK_UD, ANIM_TYPE_CHARACTER_SNIPER_LOOK_SOUTH_NORTH) ;
                            //s_PlayerInstance->SetAnimByType(ANIM_TYPE_CHARACTER_SNIPER_POSE) ;
                        }
                        else
                            s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_BODY_LOOK_UD, ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH) ;
                    }

                    if (HeadLR)
                    {
                        HeadLR->SetSpeed(0.1f) ;
                        s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_LR, ANIM_TYPE_CHARACTER_HEAD_LEFT_RIGHT) ;
                    }

                    if (HeadUD)
                    {
                        HeadUD->SetSpeed(0.1f) ;
                        s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_UD, ANIM_TYPE_CHARACTER_HEAD_UP_DOWN) ;
                    }

		            // Set masked anim
		            if (IdleAnimState)
                    {
		                s_PlayerInstance->SetMaskedAnimByType(MASKED_ANIM_ID_IDLE, ANIM_TYPE_CHARACTER_IDLE) ;
			            IdleAnimState->SetWeight(0.95f) ;
                    }
                }

                // Look for vehicle
                if (        (Shape->GetType() == SHAPE_TYPE_VEHICLE_SHRIKE)
                        ||  (Shape->GetType() == SHAPE_TYPE_VEHICLE_THUNDERSWORD)
                        ||  (Shape->GetType() == SHAPE_TYPE_VEHICLE_HAVOC)
                        ||  (Shape->GetType() == SHAPE_TYPE_VEHICLE_WILDCAT)
                        ||  (Shape->GetType() == SHAPE_TYPE_VEHICLE_BEOWULF)
                        ||  (Shape->GetType() == SHAPE_TYPE_VEHICLE_JERICHO2) )
                {
                    s_VehicleInstance = Instance ;
                }

                // Look for projector
                if (Shape->GetType() == SHAPE_TYPE_MISC_LOGO_PROJECTOR)
                {
                    s_Projector = Instance ;

                    s_Projector->SetTextureAnimFPS(15) ;
                    s_Projector->SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG) ;
                }


                // Look for human invent station
                if (Shape->GetType() == SHAPE_TYPE_INVENT_STATION_HUMAN)
                {
                    s_HumanInventInstance = Instance ;

                    // Attach to a vehicle?
                    if ((s_VehicleInstance) && (s_VehicleInstance->GetHotPointByType(HOT_POINT_TYPE_INVENT_MOUNT)))
                    {
                        // Attach to vehicle
                        s_HumanInventInstance->SetParentInstance(s_VehicleInstance, HOT_POINT_TYPE_INVENT_MOUNT) ;
                        s_HumanInventInstance->SetPos(vector3(0,0,0)) ;
                        s_HumanInventInstance->SetScale(vector3(1,1,1)) ;
                    }
                }

                // Mount weapon to player
                if (        (Shape->GetType() == SHAPE_TYPE_WEAPON_CHAIN_GUN)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_DISC_LAUNCHER)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_ELF)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_ENERGY)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_GRENADE_LAUNCHER)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_MISSILE_LAUNCHER)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_MORTAR_LAUNCHER)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_PLASMA_GUN)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_PLASMA_THROWER)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_REPAIR)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_SHOCKLANCE)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_SNIPER_RIFLE)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_TARGETING)
                        ||  (Shape->GetType() == SHAPE_TYPE_WEAPON_BLASTER) )
                {
                    s_WeaponInstance = Instance ;
                
                    // Make weapon relative to mount position
                    if (s_PlayerInstance)
                    {
                        hot_point *WeaponHotPoint = s_WeaponInstance->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                        hot_point *PlayerHotPoint = s_PlayerInstance->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                        if ((WeaponHotPoint) && (PlayerHotPoint))
                        {
                            s_WeaponInstance->SetParentInstance(s_PlayerInstance, HOT_POINT_TYPE_WEAPON_MOUNT) ;
                            s_WeaponInstance->SetPos(-WeaponHotPoint->GetPos()) ;
                        }
                    }
                }

                // Mount flag to player
                if (Shape->GetType() == SHAPE_TYPE_MISC_FLAG)
                {
                    s_FlagInstance = Instance ;
                    s_FlagShape    = Instance->GetShape() ;

                    // Make object relative to mount position
                    if (s_PlayerInstance)
                    {
                        hot_point *FlagHotPoint   = s_FlagInstance->GetHotPointByType(HOT_POINT_TYPE_FLAG_MOUNT) ;
                        hot_point *PlayerHotPoint = s_PlayerInstance->GetHotPointByType(HOT_POINT_TYPE_FLAG_MOUNT) ;
                        if ((FlagHotPoint) && (PlayerHotPoint))
                        {
                            s_FlagInstance->SetParentInstance(s_PlayerInstance, HOT_POINT_TYPE_FLAG_MOUNT) ;
                            s_FlagInstance->SetPos(-FlagHotPoint->GetPos()) ;
                        }
                    }
                }

                // Belly turret?
                if (        (Shape->GetType() == SHAPE_TYPE_TURRET_BELLY_BASE)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_DEPLOYED)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_SENTRY)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_FIXED_BASE)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_TANK_BASE)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_BELLY_BASE) )
                {
                    s_TurretInstance = Instance ;
                    s_TurretInstance->GetCurrentAnimState().SetSpeed(0.1f) ;

                    // Attach to a vehicle?
                    if ((s_VehicleInstance) && (s_VehicleInstance->GetHotPointByType(HOT_POINT_TYPE_TURRET_MOUNT)))
                    {
                        // Attach to vehicle
                        s_TurretInstance->SetParentInstance(s_VehicleInstance, HOT_POINT_TYPE_TURRET_MOUNT) ;
                        s_TurretInstance->SetPos(vector3(0,0,0)) ;
                        s_TurretInstance->SetScale(vector3(1,1,1)) ;
                    }

                    // Add yaw anim
                    anim_state* YawAnimState = s_TurretInstance->AddAdditiveAnim(1) ;
                    ASSERT(YawAnimState) ;
                    s_TurretInstance->SetAdditiveAnimByType(1, ANIM_TYPE_TURRET_YAW) ;
                    YawAnimState->SetSpeed(0.03f) ;

                    // Add pitch anim
                    anim_state* PitchAnimState = s_TurretInstance->AddAdditiveAnim(0) ;
                    ASSERT(PitchAnimState) ;
                    s_TurretInstance->SetAdditiveAnimByType(0, ANIM_TYPE_TURRET_PITCH) ;
                    PitchAnimState->SetSpeed(0.02f) ;
                }

                // Turret barrel?
                if (        (Shape->GetType() == SHAPE_TYPE_TURRET_BARREL_AA)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_BARREL_ELF)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_BARREL_MISSILE)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_BARREL_PLASMA)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_BARREL_MORTAR) 
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_TANK_BARREL_MORTAR)
                        ||  (Shape->GetType() == SHAPE_TYPE_TURRET_TANK_BARREL_CHAIN) )
                {
                    s_BarrelInstance = Instance ;
                
                    // Attach to turret?
                    if (s_TurretInstance)
                    {
                        s_BarrelInstance->SetParentInstance(s_TurretInstance, HOT_POINT_TYPE_TURRET_BARREL_MOUNT) ;
                        s_BarrelInstance->SetPos(vector3(0,0,0)) ;
                        s_BarrelInstance->SetScale(vector3(1,1,1)) ;
                    }
                }


                // Mount pack to player
                if (        (Shape->GetType() >= SHAPE_TYPE_ARMOR_PACK_AMMO)
                        &&  (Shape->GetType() <= SHAPE_TYPE_DEPLOY_PACK_SATCHEL_CHARGE) )
                {
                    s_PackInstance = Instance ;

                    s_PackInstance->SetTextureAnimFPS(0.0f) ;
                    s_PackInstance->SetTextureFrame(1) ;
                    s_PackInstance->SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG) ;
                    s_PackInstance->SetScale(vector3(1,1,1)) ;

                    if (s_PlayerInstance)
                        //if (0)
                    {
                        //vector3 Pos = -m_BackpackInstance.GetModel()->GetHotPointByType(HOT_POINT_TYPE_BACKPACK_MOUNT)->GetPos() ;

                        s_PackInstance->SetParentInstance(s_PlayerInstance, HOT_POINT_TYPE_BACKPACK_MOUNT) ;
                
                        bbox    BBox = s_PackInstance->GetModel()->GetNode(0).GetBounds() ;
                        vector3 Pos ;
                        Pos.X = -(BBox.Min.X + BBox.Max.X) * 0.5f ;
                        Pos.Y = -(BBox.Min.Y + BBox.Max.Y) * 0.5f ;
                        Pos.Z = - BBox.Max.Z ;

                        //vector3 Pos = m_BackpackInstance.GetModel()->GetNode(0).GetBounds()
                        //radian3 Rot = radian3(0,0,0) ;
                        //Pos.Rotate(Rot) ;

                        s_PackInstance->SetPos(Pos) ;//-m_BackpackInstance.GetModel()->GetHotPointByType(HOT_POINT_TYPE_BACKPACK_MOUNT)->GetPos()) ;
                        //m_BackpackInstance.SetRot(Rot); //radian3(0,R_180,0)) ;//-m_BackpackInstance.GetModel()->GetHotPointByType(HOT_POINT_TYPE_BACKPACK_MOUNT)->GetDir().GetRotation()) ;

                    }
                }
            }
        }
    }

    // Setup normal view
    //View.SetZLimits(0.001f, 10.0f) ;
    //View.SetXFOV(R_70) ;
    //View.SetYFOV(R_70) ;
    View.OrbitPoint(vector3(0,0,0),     // point
                      6.0,              // Distance
                      0,              // Pitch,
                      0) ;             // Yaw

    EyeView = View ;

    radian3 Rot = radian3(0.0f ,0.0f, 0.0f) ;

    s32 Anim=0;
    f32 AnimTime=0.0f ;
    f32 a=0.0f ;

    s32 Frame=0 ;
    f32 Angle0=0 ;
    f32 Angle1=0 ;
    f32 Angle2=0 ;
    f32 Angle3=0 ;
    f32 Angle4=0 ;
    f32 Angle5=0 ;

    // Setup the damage system
    //texture* DamageTexture = s_ShapeManager.GetTextureManager().GetTextureByName("[p8]damage.xbmp") ;
    texture* DamageTexture = s_ShapeManager.GetTextureManager().GetTextureByName("damage.xbmp") ;
    if (DamageTexture)
        s_Damage.Init(DamageTexture) ;

    // Main loop
    while( CollectInput() )
    {
        // Update view
        View.SetXFOV(s_FOV * R_1) ;
        View.SetZLimits(s_Near, s_Far) ;

        // Calc frame increment
        f32 DeltaTime, FPS = 0.0f ;

        #ifdef TARGET_PC        
            FPS = eng_GetFPS() ;
        #endif

        #ifdef TARGET_PS2        
            FPS = 60.0f/2 ;
        #endif

        if (FPS > 1.0f)
            DeltaTime = 1.0f / FPS ;
        else
            DeltaTime = 0.0f ;

        #ifdef TARGET_PC
            DeltaTime = 1.0f / 60.0f ;
        #endif

		
		for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
		        shape_instance* Instance = s_InstanceArray[x][z] ;
                shape*          Shape = Instance->GetShape() ;

                if (Shape->GetAnimCount())
                    Instance->SetAnimByIndex(s_Anim % Shape->GetAnimCount(), 0.25f) ;
                
                Instance->SetCollisionModelByIndex(0) ;

                //Instance->SetTexturePlaybackType(material::PLAYBACK_TYPE_LOOP) ;
                //Instance->SetTexturePlaybackType(material::PLAYBACK_TYPE_PING_PONG) ;
                //Instance->SetTexturePlaybackType(material::PLAYBACK_TYPE_ONCE_ONLY) ;

            }
		}

        // Advance frame
		for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
                shape_instance* Instance = s_InstanceArray[x][z] ;

				//Instance->Advance(DeltaTime + (((f32)x * 0.001f) + ((f32)z * 0.001f))) ;
				Instance->Advance(DeltaTime) ;

                s32 TexFrame = (s32)s_InstanceArray[0][0]->GetTextureFrame() ;

                //if (x >= 2)
                    //Instance->SetTextureFrame((f32)((TexFrame>>1)<<1)) ;
			}
        }

        eng_Begin() ;



        // Update instances
        if (Triggered1stPerson)
        {
            // Attach view?
            if (s_PlayerInstance)
            {
                #if 0
                    // Attach view to player camera mount pos
                    EyeView.SetPosition(s_PlayerInstance->GetHotPointWorldPos(HOT_POINT_TYPE_CAMERA_MOUNT)) ;
                    EyeView.SetRotation(radian3(Pitch,0,0)) ;
                #else

                // HARD CODED RELATIVE TO WEAPON - LOOKS GREAT AND THE SAME ON ALL PLAYER MODELS - AND EASY TO TWEAK!
                static vector3 HeadOffset(0.0f, 0.1f, 0.05f) ;
                static vector3 HandOffset(0.25f, 0.5f, -0.4f) ;

                vector3 HandPos   = s_PlayerInstance->GetHotPointWorldPos(HOT_POINT_TYPE_WEAPON_MOUNT) ;
    
                quaternion HandRot(radian3(Pitch,0,0)) ;                     // Rotate around y and pitch
                quaternion HeadRot(radian3(0, 0, 0)) ;   // Only rotate around y axis

                // Offset view from hand
                EyeView.SetPosition(HandPos + (HeadRot * HeadOffset) + (HandRot * HandOffset)) ;
                EyeView.SetRotation(radian3(Pitch,0,0)) ;

                #endif


                // Add layered anims
                anim_state *BodyUD   = NULL ;
                anim_state *HeadLR   = NULL ;
                anim_state *HeadUD   = NULL ;

                BodyUD   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_BODY_LOOK_UD) ;
                HeadLR   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_LR) ;
                HeadUD   = s_PlayerInstance->AddAdditiveAnim(ADDITIVE_ANIM_ID_HEAD_LOOK_UD) ;
                
                if (BodyUD)
                {
                    BodyUD->SetSpeed(0.0f) ;
                    s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_BODY_LOOK_UD, ANIM_TYPE_CHARACTER_LOOK_SOUTH_NORTH) ;
                }

                if (HeadLR)
                {
                    HeadLR->SetSpeed(0.0f) ;
                    s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_LR, ANIM_TYPE_CHARACTER_HEAD_LEFT_RIGHT) ;
                }

                if (HeadUD)
                {
                    HeadUD->SetSpeed(0.0f) ;
                    s_PlayerInstance->SetAdditiveAnimByType(ADDITIVE_ANIM_ID_HEAD_LOOK_UD, ANIM_TYPE_CHARACTER_HEAD_UP_DOWN) ;
                }

                // Calculate pitch as a value from 0 (up - VIEW_MIN_PITCH) to 1 (down - VIEW_MAX_PITCH)
                f32 PitchRatio = (Pitch - VIEW_MIN_PITCH) / (VIEW_MAX_PITCH - VIEW_MIN_PITCH) ;
                if (PitchRatio < 0)
                    PitchRatio = 0 ;
                else
                if (PitchRatio > 1)
                    PitchRatio = 1 ;

	            // Update body look up/down additive anim
	            if ( (BodyUD) && (BodyUD->GetAnim()) )
		            BodyUD->SetFrameParametric(PitchRatio) ;

                // Update head look up/down additive anim
	            if ( (HeadUD) && (HeadUD->GetAnim()) )
		            HeadUD->SetFrameParametric(PitchRatio) ;

                // Update head look left/right additive anim
	            if ( (HeadLR) && (HeadLR->GetAnim()) )
                    HeadLR->SetFrameParametric(PitchRatio) ;

	            // Mask off upper body with idle animation when moving in first person view
	            // so that the weapon does not move all over the place
			    anim_state *IdleAnimState = s_PlayerInstance->AddMaskedAnim(MASKED_ANIM_ID_IDLE) ;

		        // Set masked anim
		        ASSERT(IdleAnimState) ;
		        s_PlayerInstance->SetMaskedAnimByType(MASKED_ANIM_ID_IDLE, ANIM_TYPE_CHARACTER_IDLE) ;
		        //if (m_ViewType == VIEW_TYPE_1ST_PERSON) 
			        IdleAnimState->SetWeight(0.95f) ;
		        //else
			        //IdleAnimState->SetWeight(0.5f) ;	// dampen down movement!

		        // Animate idle?
		        //if (m_PlayerState == PLAYER_STATE_IDLE)
			        //IdleAnimState->SetSpeed(1.0f) ;
		        //else
			        IdleAnimState->SetSpeed(0.0f) ;

                // Temp for now to stop head moving in demo...
		        //IdleAnimState->SetSpeed(0.0f) ;

                // Setup weapon orientation
                if (s_WeaponInstance)
                {
                    s_WeaponInstance->SetParentInstance(NULL, 0) ;
                    hot_point *MountHotPoint = s_WeaponInstance->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                    if (MountHotPoint)
                    {
                        vector3    AlivePos ;
                        quaternion AliveRot ;

                        // Get hand orientation
                        matrix4 mHand   = s_PlayerInstance->GetHotPointL2W(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                        vector3 HandPos = mHand.GetTranslation() ;
                        radian3 HandRot = mHand.GetRotation() ;

                        // Calculate alive orientation
                        AliveRot.Setup(radian3(Pitch, 0, 0)) ;
                        AlivePos = HandPos - (AliveRot * MountHotPoint->GetPos()) ;

                        // Blend between alive(0) and death(1) orientation
                        s_WeaponInstance->SetPos(AlivePos) ;
                        s_WeaponInstance->SetRot(AliveRot.GetRotation()) ;
                    }
                }
            }
        }


        // Update weapon instance like in the game
        if ((s_PlayerInstance) && (s_WeaponInstance))
        {
            hot_point *MountHotPoint = s_WeaponInstance->GetHotPointByType(HOT_POINT_TYPE_WEAPON_MOUNT) ;
            if (MountHotPoint)
            {
                // Clear attached
                s_WeaponInstance->SetParentInstance(NULL,0) ;

                // Get hand orientation
                matrix4 mHand   = s_PlayerInstance->GetHotPointL2W(HOT_POINT_TYPE_WEAPON_MOUNT) ;
                vector3 HandPos = mHand.GetTranslation() ;
                radian3 HandRot = mHand.GetRotation() ;

                radian3 WeaponRot = s_PlayerInstance->GetRot() ;
                //WeaponRot.Roll  = 0 ;
                //WeaponRot.Pitch = 0 ;

                vector3 MountPos = MountHotPoint->GetPos() ;
                MountPos.Rotate(WeaponRot) ;

                vector3 WeaponPos = HandPos - MountPos ;
                
                s_WeaponInstance->SetPos(WeaponPos) ;
                s_WeaponInstance->SetRot(WeaponRot) ;
            }
        }



        // Set active view
        if (In1stPerson)
        {
            // Set view
            eng_MaximizeViewport(EyeView) ;
            eng_SetView(EyeView, 0) ;
            eng_ActivateView ( 0 );
        }
        else
        {
            eng_MaximizeViewport(View) ;
            eng_SetView(View, 0) ;
            eng_ActivateView ( 0 );
        }

        // temp
        //eng_MaximizeViewport(View) ;
        //eng_SetView(View, 0) ;
        //eng_ActivateView ( 0 );

        // Update flag verts
        s32 TotalVerts = 0;
        if (s_FlagShape)
        {
            xtimer Timer ;
            Timer.Start() ;

            Angle0 += DeltaTime * R_2 ;    // dir1 angle
            Angle1 += DeltaTime * R_3 ;    // dir2 angle

            f32 CosAngle0 = x_cos(Angle0) ;
            f32 CosAngle1 = x_cos(Angle1) ;

            Angle2 += DeltaTime * R_130 * CosAngle0 ;    // dist angle1
            Angle3 += DeltaTime * R_140 * CosAngle1 ;    // dist angle2
            Angle4 += DeltaTime * R_150 * CosAngle0 ;    // dist scale angle
            Angle5 += DeltaTime * R_160 * CosAngle1 ;    // angle scale angle

            material& DestMat    = s_FlagShape->GetModelByIndex(Frame & 1)->GetMaterial(0) ;
            material& SourceMat  = s_FlagShape->GetModelByIndex(2)->GetMaterial(0) ;
            f32       DistScale  = (0.02f + (0.01f * x_cos(Angle4))) * s_FlagShape->GetModelByIndex(0)->GetPrecisionScale() ;
            f32       AngleScale = (5.0f + (0.5f * x_cos(Angle5))) / s_FlagShape->GetModelByIndex(0)->GetPrecisionScale() ;
            f32       AngleOffset, Dist, NormRot ;
            radian    S,C ;
            vector3   TempNorm ;  

            for (i = 0 ; i < SourceMat.GetDListCount() ; i++)
            {
                material_dlist& SourceDList = SourceMat.GetDList(i) ;
                material_dlist& DestDList   = DestMat.GetDList(i) ;

                for (j = 0 ; j < SourceDList.GetStripCount() ; j++)
                {
                    dlist_strip& SourceStrip = SourceDList.GetStrip(j) ;
                    dlist_strip& DestStrip   = DestDList.GetStrip(j) ;

                    v = SourceStrip.GetVertCount() ;
                    ASSERT(SourceStrip.GetVertCount() == DestStrip.GetVertCount()) ;

                    #ifdef TARGET_PC
                        dlist_strip::d3d_vert *SourceVert = (dlist_strip::d3d_vert*) SourceStrip.GetPosList() ;
                        dlist_strip::d3d_vert *DestVert =   (dlist_strip::d3d_vert*) DestStrip.GetPosList() ;
                        while(v--)
                        {
                            // Calc angle offset given position, and calc dist to move
                            AngleOffset = (SourceVert->vPos.X + SourceVert->vPos.Y) * AngleScale ;
                            Dist        = x_sin(Angle2 + AngleOffset) + x_sin(Angle3 + AngleOffset)  ; // range -2 to 2

                            // Move the vert in and out on the z axis
                            DestVert->vPos.Z = SourceVert->vPos.Z + (DistScale * Dist) ;
                            
                            // Rotate normal to give fudged lighting effect 
                            NormRot = R_20 * Dist ;
                            x_sincos( NormRot, S, C );
                            
                            // RotateX
                            TempNorm.X = SourceVert->vNormal.X ;
                            TempNorm.Y = (C * SourceVert->vNormal.Y) - (S * SourceVert->vNormal.Z) ;
                            TempNorm.Z = (C * SourceVert->vNormal.Z) + (S * SourceVert->vNormal.Y) ;

                            // RotateY
                            DestVert->vNormal.X = ((C * TempNorm.X) + (S * TempNorm.Z)) ;
                            DestVert->vNormal.Y = TempNorm.Y ;
                            DestVert->vNormal.Z = ((C * TempNorm.Z) - (S * TempNorm.X)) ;

                            // Next vert
                            DestVert++ ;
                            SourceVert++ ;
                            TotalVerts++ ;
                        }
                    #endif

                    #ifdef TARGET_PS2
                        dlist_strip::ps2_pos    *SourcePos  = (dlist_strip::ps2_pos*) SourceStrip.GetPosList() ;
                        dlist_strip::ps2_norm   *SourceNorm = (dlist_strip::ps2_norm*)SourceStrip.GetNormalList() ;
                        dlist_strip::ps2_pos    *DestPos    = (dlist_strip::ps2_pos*) DestStrip.GetPosList() ;
                        dlist_strip::ps2_norm   *DestNorm   = (dlist_strip::ps2_norm*)DestStrip.GetNormalList() ;
                        while(v--)
                        {
                            // Calc angle offset given position, and calc dist to move
                            AngleOffset = (SourcePos->x + SourcePos->y) * AngleScale ;
                            Dist        = x_sin(Angle2 + AngleOffset) + x_sin(Angle3 + AngleOffset)  ; // range -2 to 2

                            // Move the vert in and out on the z axis
                            DestPos->z = SourcePos->z + (s16)(DistScale * Dist) ;
                            
                            // Rotate normal to give fudged lighting effect 
                            NormRot = R_20 * Dist ;
                            x_sincos( NormRot, S, C );
                            
                            // RotateX
                            TempNorm.X = (f32)SourceNorm->x ;
                            TempNorm.Y = (C * (f32)SourceNorm->y) - (S * (f32)SourceNorm->z) ;
                            TempNorm.Z = (C * (f32)SourceNorm->z) + (S * (f32)SourceNorm->y) ;

                            // RotateY
                            DestNorm->x = (s8) ((C * TempNorm.X) + (S * TempNorm.Z)) ;
                            DestNorm->y = (s8) TempNorm.Y ;
                            DestNorm->z = (s8) ((C * TempNorm.Z) - (S * TempNorm.X)) ;

                            // Next vert
                            DestPos++ ;
                            DestNorm++ ;
                            SourcePos++ ;
                            SourceNorm++ ;
                            TotalVerts++ ;
                        }
                    #endif
                }
            }

            Timer.Stop() ;

            if (s_DrawDebugText)
                x_printfxy(0,10, "FlagTime:%f Verts:%d", Timer.ReadMs(), TotalVerts) ;
        }


                // Draw axis at origin
#define OSCALE  2
                vector3 O(0,0,0) ;
                vector3 Ox(OSCALE,0,0) ;
                vector3 Oy(0,OSCALE,0) ;
                vector3 Oz(0,0,OSCALE) ;
                
                if (s_DrawOrigin)
                {
                    draw_Line( O, Ox, XCOLOR_RED) ;
                    draw_Line( O, Oy, XCOLOR_BLUE) ;
                    draw_Line( O, Oz, XCOLOR_GREEN) ;
                }



#ifdef TARGET_PS2
        if( FALSE )
		{
			s32 Count ;
			f32 CPU, GS, INT, FPS ;
    
			eng_GetStats(Count, CPU, GS, INT, FPS) ;

			x_printfxy(0, 20, "%1d CPU:%4.1f  GS:%4.1f  INT:%4.1f  FPS:%4.1f/n", Count, CPU, GS, INT, FPS) ;
		}
#endif

        // Begin shape draw
        shape::BeginFrame() ;
        shape::BeginDraw() ;


        DEBUG_BEGIN_STATS(0,0)

        // Draw frame (clip is off right now...)
        DEBUG_BEGIN_TIMER(DrawTime)
        for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
                shape_instance* Instance = s_InstanceArray[x][z] ;

                // Skip player in first person view
                if ((In1stPerson) && (Instance == s_PlayerInstance))
                    continue ;

                // Calc fog
                xcolor  FogColor = shape::GetFogColor() ;
                FogColor.A = (u8)(255.0f * Instance->CalculateLinearFog(0.75f)) ;
                Instance->SetFogColor(FogColor) ;

                //if (x == 0)
                {
                //Instance->Draw(shape::DRAW_FLAG_CLIP) ;
                //| 
                                           //shape::DRAW_FLAG_TURN_OFF_LIGHTING) ;
                }

                // temp
                //Instance->SetTextureAnimFPS(0) ;
                //Instance->SetTextureFrame(0) ;


                // Toggle flag frames
                shape* Shape = Instance->GetShape() ;
                if ((Shape->GetType() == SHAPE_TYPE_MISC_FLAG) && (Shape->GetModelCount() == 3))
                    Instance->SetModel(Shape->GetModelByIndex(Frame & 1)) ;

                //Instance->Draw(shape::DRAW_FLAG_CLIP  | shape::DRAW_FLAG_REF_RANDOM | shape::DRAW_FLAG_TURN_OFF_LIGHTING) ;
                //if (z == 1)
                    
                // Setup draw flags
                u32 DrawFlags = 0 ;
                //u32 DrawFlags = shape::DRAW_FLAG_REF_WORLD_SPACE ;

                // Turn on clipping?
                if (s_Clipping)
                    DrawFlags |= shape::DRAW_FLAG_CLIP ;

                // Turn on culling?
                if (s_Cull)
                    DrawFlags |= shape::DRAW_FLAG_CULL ;
               
                // Spin reflection map?
                switch(Shape->GetType())
                {
                    case SHAPE_TYPE_MISC_SHIELD:
                    {
                        s32 Alpha = (s32)((x_sin( (radian)x_GetTimeSec() ) + 1.0f) * 127.6f);
                        xcolor Color( 255, 255, 255, Alpha );
                        Instance->SetColor( Color );
                        x_printfxy( 20, 0, "Alpha:%03d", Alpha );
                        //static radian3 RefRot(0,0,0) ;
                        //static s32 ShieldCount=0 ;
                        //static s32 ShieldSpeed=1 ; // FOR PS2!!!!
                        //ShieldCount++ ;
                        //if (ShieldCount > ShieldSpeed)
                        //{
                            //ShieldCount = 0;
                            //RefRot.Roll  = x_frand(-R_180, R_180) ;
                            //RefRot.Yaw   = x_frand(-R_180, R_180) ;
                            //RefRot.Pitch = x_frand(-R_180, R_180) ;
                        //}

                        //Instance->SetReflectionRot(RefRot) ;
                        //DrawFlags |= shape::DRAW_FLAG_REF_SPECIFIED ;
                        DrawFlags |= shape::DRAW_FLAG_REF_RANDOM ;
                        DrawFlags |= shape::DRAW_FLAG_PRIME_Z_BUFFER ;
                        DrawFlags |= shape::DRAW_FLAG_SORT;
                    }
                    break ;

                    case SHAPE_TYPE_MISC_LOGO_PROJECTOR:
                    case SHAPE_TYPE_MISC_LOGO_STORM:
                    case SHAPE_TYPE_MISC_LOGO_INFERNO:
                    case SHAPE_TYPE_MISC_LOGO_NEUTRAL:
                        DrawFlags |= shape::DRAW_FLAG_REF_RANDOM ;
                        break ;

                    case SHAPE_TYPE_SCENERY_NEXUS_EFFECT:
                        {
                            // Glow
                            static f32 GlowTime=0;
                            GlowTime += DeltaTime ;
                            f32 Col = (1.0f + x_cos(GlowTime * R_360)) * 0.5f ; // Make 0->1
                            f32 RGB = 1.0f + Col ;          // Brightens on the PS2/PC
                            f32 A   = 0.5f + (Col*0.5f) ;   // Fade in and out
                            Instance->SetColor(vector4(RGB, RGB, RGB, A)) ;
                        }
                        break ;


                    case SHAPE_TYPE_CHARACTER_LIGHT_FEMALE:
                    case SHAPE_TYPE_CHARACTER_MEDIUM_FEMALE:
                    case SHAPE_TYPE_CHARACTER_HEAVY_FEMALE:
                    case SHAPE_TYPE_CHARACTER_LIGHT_MALE:
                    case SHAPE_TYPE_CHARACTER_MEDIUM_MALE:
                    case SHAPE_TYPE_CHARACTER_HEAVY_MALE:
                    case SHAPE_TYPE_CHARACTER_LIGHT_BIO:
                    case SHAPE_TYPE_CHARACTER_MEDIUM_BIO:
                    case SHAPE_TYPE_CHARACTER_HEAVY_BIO: 
                        DrawFlags |= shape::DRAW_FLAG_TURN_ON_ANIM_MAT_DETAIL ;
                        break ;


                }


                // Lookup detail texture (if any)
                texture* DetailTexture ;
                s32      DetailClutHandle ;
                s_Damage.GetTexture(s_Health, DetailTexture, DetailClutHandle) ;
                Instance->Draw(DrawFlags, DetailTexture, DetailClutHandle) ;
                    
                //Instance->DrawFeatures() ;


#if 1

                if ((x == 0) && (z == 0) && (!In1stPerson) && (s_DrawDebugText))
                {
                    anim*  Anim  = Instance->GetCurrentAnimState().GetAnim() ;
                    shape* Shape = Instance->GetShape() ;
                    model* Model = Instance->GetModel() ;

                    // Print info
                    s32 y = 0 ;
                    x_printfxy( 0,y,"Shape           :%s", Shape->GetName() ) ; y++ ;
                    x_printfxy( 0,y,"Shape Memory    :%dbytes %dK", Shape->MemoryUsed(), (Shape->MemoryUsed()+1023)/1024) ; y++ ;
                    x_printfxy( 0,y,"ShapeType       :%s", ShapeTypesStrings[Shape->GetType()] ) ; y++ ;
                    x_printfxy( 0,y,"Models          :%d", Shape->GetModelCount() ) ; y++ ;
                    x_printfxy( 0,y,"Model           :%s", Model->GetName() ) ; y++ ;
                    x_printfxy( 0,y,"Model verts     :%d", Model->GetDListVertCount()) ; y++ ;
                    x_printfxy( 0,y,"ModelType       :%s", ModelTypesStrings[Model->GetType()] ) ; y++ ;
                    x_printfxy( 0,y,"Nodes           :%d", Model->GetNodeCount()) ; y++ ;
                    x_printfxy( 0,y,"Anims           :%d", Shape->GetAnimCount()) ; y++ ;
                    
                    if (Anim)
                    {
                        x_printfxy( 0,y,"Anim            :%s", Anim->GetName()) ; y++ ;
                        x_printfxy( 0,y,"AnimType        :%s", AnimTypesStrings[Anim->GetType()] ) ; y++ ;
                        x_printfxy( 0,y,"Additive        :%d", Anim->GetAdditiveFlag() ) ; y++ ;
                        x_printfxy( 0,y,"RemoveRootTrans :%d", Anim->GetRemoveRootTransFlag() ) ; y++ ;
                        x_printfxy( 0,y,"Frame:%03d - %03d LoopTo:%03d", 
                                    (s32)Instance->GetCurrentAnimState().GetFrame(),
                                    Anim->GetFrameCount(), Anim->GetLoopToFrame() ) ; y++ ;
                    }

                    for (h = 0 ; h < Model->GetHotPointCount() ; h++)
                    {
                        hot_point* HotPoint = Model->GetHotPointByIndex(h) ;
                        x_printfxy(0, y, "HotPointType: %s", HotPointTypesStrings[HotPoint->GetType()]) ; 
                            y++ ;
                    }
                }
#endif

			}
		}
        DEBUG_END_TIMER(DrawTime)
        DEBUG_END_STATS


        // End the shape draw
        shape::EndDraw() ;
        shape::EndFrame() ;


//      material::PrintStats() ;

        //draw_Frustum(EyeView, xcolor(255,255,255,255), 1.0f) ;


        // Draw collision polys
        for (x = 0 ; x < X_TOTAL ; x++)
		{
			for (z = 0 ; z < Z_TOTAL ; z++)
			{
                shape_instance* Instance = s_InstanceArray[x][z] ;

                //Instance->DrawCollisionModel() ;
            }
        }


        eng_End() ;


        // Draw texture        
        //g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE) ;
        //if (TextureManager.GetTextureCount())
            //TextureManager.GetTexture(0).Draw(0,0) ;

        // Renders a wire bbox 
        //draw_BBox(vector3(-100,-100,-100), vector3(100,100,100), xcolor(255,255,255,255)) ;

        // draw tri
        //draw_Begin(DRAW_TRIANGLES) ;            
        //draw_Color(0,0,1.0f,1.0f) ;
        //draw_Vertex(vector3(100,100,0)) ;
        //draw_Color(0,1.0f,0,1.0f) ;
        //draw_Vertex(vector3(100,0,0)) ;
        //draw_Color(1.0f,0,0,1.0f) ;
        //draw_Vertex(vector3(0,0,0)) ;
        //draw_End() ;



        // Show screen
        Frame++ ;
        eng_PageFlip();
    }

    // Close the systems
    s_Damage.Kill() ;
    s_ShapeManager.DeleteAllShapes() ;
}

