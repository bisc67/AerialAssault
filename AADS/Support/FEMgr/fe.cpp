#include <stdio.h>
#include "Entropy.hpp"
#include "fe.hpp"
#include "aux_bitmap.hpp"

#include "audiomgr/audio.hpp"

#include "../demo1/globals.hpp"


// Declare shp and xbmp output paths
#ifdef TARGET_PC

	#define SHP_PATH    "Data/Shapes/PC"        
	#define XBMP_PATH   "Data/Shapes/PC"        

#endif

#ifdef TARGET_PS2

	#define SHP_PATH    "Data/Shapes/PS2"        
	#define XBMP_PATH   "Data/Shapes/PS2"        

#endif

//=============================================================================
// The ui_obj class its own self

// keep track of which node indices represent a descendent of this ui_obj
void    ui_obj::SetChildren    ( s32 nChildren, s32* pChildList )
{
    m_nChildNodes = nChildren;

    m_pChildNodes = new s32[ m_nChildNodes ];

    x_memcpy( m_pChildNodes, pChildList, nChildren * sizeof(s32) );
}

ui_obj::ui_obj( )
{
    m_nChildNodes = 0;
    m_pChildNodes = NULL;
}

ui_obj::~ui_obj( )
{
    if ( m_nChildNodes != 0 )
        delete m_pChildNodes;

}

xbool   ui_obj::IsChild        ( s32 Idx )
{
    s32 i;

    for ( i = 0 ; i < m_nChildNodes; i++ )
    {
        if ( m_pChildNodes[i] == Idx )
            return TRUE;
    }

    return FALSE;
}


//=============================================================================
// The button Class

ui_button::~ui_button  ( )
{
}

void    ui_button::ProcInput   ( s32 Buttons )
{
    Buttons = Buttons;
}

void    ui_button::UpdateImage ( void )
{
}

void    ui_button::SetText     ( const char* pText )
{
    xstring tmpstr( pText );     // utilize constructor
    m_Text = tmpstr;
}




//=============================================================================
// The listbox Class

ui_listbox::ui_listbox ( )
{
    m_Items.Clear();
    m_Width = 32;
    m_ItemCount = 0;
    m_SelStart = 0;
    m_SelEnd = 0;
    m_ViewStart = 0;
    m_ViewHeight = 5;

}


ui_listbox::~ui_listbox ( )
{
    // delete all remaining items, last to first, to prevent all the shuffling
    s32 i;

    for ( i = 0; i < m_Items.GetCount(); i++ )
        delete m_Items[i];
}

void    ui_listbox::ProcInput   ( s32 Buttons )
{
    if ( ( Buttons & FE_UP ) && ( m_SelStart > 0 ) )
    {
        m_SelStart--;

        if ( m_SelStart < m_ViewStart )
            m_ViewStart = m_SelStart;

        SetDirty( TRUE );
    }
    else
    if ( ( Buttons & FE_DOWN ) && ( m_SelStart < (m_ItemCount-1) ) )
    {
        m_SelStart++;

        if ( m_SelStart == ( m_ViewStart + m_ViewHeight ) )
        {
            m_ViewStart++;
        }

        SetDirty( TRUE );
    }
}

void    ui_listbox::UpdateImage ( void )
{
    s32 i;
    s32 y_spacing;

    if ( IsDirty() == TRUE )
    {
        xbitmap& Bmp = m_pTM->GetTexture( m_Texture ).GetXBitmap();

        y_spacing = (s32)(m_pMgr->m_F1.GetHeight() * 1.1f);

        // loop through all the items, render the text for each line
        for ( i = m_ViewStart; i < (m_ViewStart + m_ViewHeight); i++ )
        {
            if ( i < m_ItemCount )
            {
                if ( i == m_SelStart )
                    m_pMgr->m_F2.Print( Bmp, 0, (i - m_ViewStart) * y_spacing, "%s", m_Items[i] );
                else
                    m_pMgr->m_F1.Print( Bmp, 0, (i - m_ViewStart) * y_spacing, "%s", m_Items[i] );
            }
        }

        SetDirty(FALSE);
    }
}

void    ui_listbox::AddText     ( const char* pText )
{
    char *pTmp;

    ASSERT( m_Width > 0 );
    ASSERT( x_strlen( pText ) < m_Width );

    // allocate the string and add it to the list
    pTmp = new char[ m_Width ];
    ASSERT( pTmp );

    x_strcpy( pTmp, pText );

    m_Items.Append( pTmp );

    m_ItemCount++;
    
    SetDirty( TRUE );
}

char*   ui_listbox::GetText     ( s32 Row )
{
    ASSERT( Row < m_ItemCount );

    return m_Items[ Row ];
}


void    ui_listbox::InsertText  ( s32 Row, const char* pText )
{
    char *pTmp;

    ASSERT( m_Width > 0 );
    ASSERT( x_strlen( pText ) < m_Width );

    pTmp = new char[ m_Width ];
    ASSERT( pTmp );

    x_strcpy( pTmp, pText );

    m_Items.Insert( Row, pTmp );

    m_ItemCount++;

    SetDirty( TRUE );
}

void    ui_listbox::DeleteRow   ( s32 Row )
{
    ASSERT( Row < m_ItemCount );
    delete m_Items[Row];
    m_Items.Delete( Row, 1 );

    m_ItemCount--;

    SetDirty( TRUE );
}


void	ui_listbox::DeleteAll	( void )
{
	s32 i;
	
	for ( i = 0; i < m_ItemCount; i++ )
	{
		delete m_Items[i];
	}

	m_Items.Clear();
	m_ItemCount = 0;
	
	SetDirty( TRUE );

	m_SelStart = 0;
	m_SelEnd = 0;

}

//=============================================================================
// The slider Class

ui_slider::~ui_slider  ( )
{
}

void    ui_slider::ProcInput   ( s32 Buttons )
{
    Buttons = Buttons;
}

void    ui_slider::UpdateImage ( void )
{
}

void    ui_slider::SetParms    ( s32 MinVal, s32 MaxVal, s32 Step, s32 CVal )
{
    ASSERT( m_Val1 < m_Val2 );
    ASSERT( m_Val1 < CVal );
    ASSERT( m_Val2 > CVal );
    ASSERT( Step <= (m_Val2 - m_Val1) );

    m_Val1 = MinVal;
    m_Val2 = MaxVal;
    m_Increment = Step;
    m_CurVal = CVal;
}

void    ui_slider::SetCurVal   ( s32 Val )
{
    ASSERT( Val < m_Val2 );
    ASSERT( Val > m_Val1 );
    m_CurVal = Val;
}

void    ui_slider::GetCurVal   ( s32& Val )
{
    Val = m_CurVal;
}

void    ui_slider::IncVal      ( void )
{
    m_CurVal += m_Increment;

    if ( m_CurVal > m_Val2 )
        m_CurVal = m_Val2;
}

void    ui_slider::DecVal      ( void )
{
    m_CurVal -= m_Increment;

    if ( m_CurVal < m_Val1 )
        m_CurVal = m_Val1;
}


//=============================================================================
// The editBox Class
ui_edit::~ui_edit    ( )
{
}

void    ui_edit::ProcInput   ( s32 Buttons )
{
    Buttons = Buttons;
}

void    ui_edit::UpdateImage ( void )
{
}

void    ui_edit::SetText     ( const char* pText )
{
    xstring tmpstr( pText );
    m_Text = pText;
}

void    ui_edit::GetText     ( xstring& Txt )
{
    Txt = m_Text;
}

void    ui_edit::SetLen      ( s32 Min, s32 Max )
{
    ASSERT( Min < Max );
    ASSERT( Max > 0 );

    m_MinLen = Min;
    m_MaxLen = Max;
}

void    ui_edit::SetFilter   ( const char* pTextFilter )
{
    m_Filter = pTextFilter;
}


//=============================================================================
// The ui_manager itself

// constructor
ui_manager::ui_manager       ( )
{
    // Load a font
    m_F1.Load( "data/f1.tga" );
    m_F2.Load( "data/f1g.tga" );

    // Load labels that are present in .skel files
    m_pShape = NULL;

    m_FrontEndView.SetPosition(vector3(0.0f, 0.0f, -443.405006737f)) ;

    // NOTE: Never far clip!
    m_FrontEndView.SetZLimits(0.1f, F32_MAX) ;
}

ui_manager::~ui_manager         ( )
{
    s32 i, j;

    j = m_pObj.GetCount();

    for ( i = 0; i < j; i++ )
        delete( m_pObj[i] );

    audio_Stop( m_Hummer );
}

s32 ui_manager::FindControl(s32 NodeIndex)
{
    s32 i, j;

    j = m_pObj.GetCount();

    // Perform linear search
    for ( i = 0 ; i < j ; i++ )
    {
        // Is this button connected to the node?
        if (m_pObj[i]->GetNode() == NodeIndex)
            return i ;
    }

    // Not found
    return -1 ;
}

s32 ui_manager::FindControlByID(s32 ID)
{
    s32 i, j;

    j = m_pObj.GetCount();

    // Perform linear search
    for ( i = 0 ; i < j ; i++ )
    {
        // Is this button connected to the node?
        if (m_pObj[i]->GetID() == ID)
            return i ;
    }

    // Not found
    return -1 ;
}

ui_obj* ui_manager::FindControlPtrByID		(s32 ID)
{
	s32 i, j;

    j = m_pObj.GetCount();

    // Perform linear search
    for ( i = 0 ; i < j ; i++ )
    {
        // Is this button connected to the node?
        if (m_pObj[i]->GetID() == ID)
            return m_pObj[i] ;
    }

    // Not found
    return NULL ;
}

s32     ui_manager::LoadShapeData  ( const char* pFilename, ui_msgproc pMP, 
                                  ui_nodeproc pNP, ui_renderproc pRP )
{
    static  s32 count = 0;

    ASSERT( x_strlen( pFilename ) < 65 );

    // load the front end shape file
    m_ShapeMgr.AddAndLoadShapes( pFilename, shape_manager::MODE_LOAD_BINARY) ;
    m_pShape = m_ShapeMgr.GetShapeByIndex( 0 );

    m_Hummer = audio_Play( SFX_FRONTEND_SHELL_HUM_LP2 );

    // set the instance
    m_Instance.SetShape( m_pShape );

    // Setup user functions to hi-light the selected node
    m_Instance.SetNodeFunc(pNP) ;
    m_Instance.SetRenderNodeFunc(pRP) ;
    SetMsgProc( pMP );

    // Get current model
    model *Model = m_Instance.GetModel() ;
    ASSERT(Model) ;
                 
    // active control defaults to first one
    m_Active = 2;

	// first, wrap up the nodes with a front-end specific data type

    // parse the incoming data, allocate front end constructs
    for (s32 i = 0 ; i < Model->GetNodeCount() ; i++)
    {
        // Get node
        node &Node = Model->GetNode(i) ;
        
        // Is this node a gui node?
        user_data_gui_info  *Info = (user_data_gui_info *)Node.GetUserData() ;
        
        if (Info)
        {
            ui_button* NewBut = NULL;
            
            if ( Info->Type == GUI_TYPE_BUTTON )
                NewBut = new ui_button;
            else if ( Info->Type == GUI_TYPE_LIST_BOX )
                NewBut = (ui_button*)new ui_listbox;

            ASSERT( NewBut );

            NewBut->SetMgr( this );

            // fill it out
            if ( Info->Type == GUI_TYPE_BUTTON )
                NewBut->SetType( FE_BUTTON );
            else
                NewBut->SetType( FE_LISTBOX );

            NewBut->SetID( Node.GetType() );

            // Setup button
            NewBut->SetInfo( Info );
            NewBut->SetNode( i ) ;
            NewBut->SetTexture( -1 ) ;
            NewBut->SetTM( &m_pShape->GetTextureManager() );

            // Setup directions
            NewBut->SetDown     ( -1 );
            NewBut->SetUp       ( -1 ); 
            NewBut->SetLeft     ( -1 ); 
            NewBut->SetRight    ( -1 );


            if ( Info->HitDown )    NewBut->SetDown     ( Model->GetNodeIndexByType(Info->HitDown) );
            if ( Info->HitUp )      NewBut->SetUp       ( Model->GetNodeIndexByType(Info->HitUp) );
            if ( Info->HitLeft )    NewBut->SetLeft     ( Model->GetNodeIndexByType(Info->HitLeft) );
            if ( Info->HitRight )   NewBut->SetRight    ( Model->GetNodeIndexByType(Info->HitRight) );
            
            // how many descendents are there for this node?
			s32 nChildren = Model->GetNodeDescendentCount( i );

            if ( nChildren > 0 )
			{
                s32* tmp = new s32[ nChildren ];
                ASSERT(tmp);

                // get all descendents of this node
                Model->GetNodeDescendents( i, tmp, nChildren );

                // set the node indices for all descendents
                NewBut->SetChildren( nChildren, tmp );

                // go through each of the descendents looking for the magic texture
                for ( s32 j = 0; j < nChildren; j++ )
                {
                    s32 texidx;

                    //node &TNode = Model->GetNode( tmp[j] );

                    texidx = -1;
                    Model->GetNodeTextures( tmp[j], &texidx, 1 );

                    if ( texidx != -1 )
                    {
                        if ( x_strstr( m_pShape->GetTextureManager().GetTexture(texidx).GetName(), "UI_" ) != NULL )
                        {
                            // found the magic texture, all done
                            NewBut->SetTexture( texidx );
                            break;
                        }
                    }
                }
        
                delete tmp;

			}

            // all done, now add it formally to the list
            m_pObj.Append( NewBut );

            // finally let the application initialize the state as needed
            (*pMP)( (ui_obj*)NewBut, FEMSG_INIT, 0, 0, 0 );
        }
    }

    // let the screen procedure initialize
    (*m_MsgProc)( (ui_obj*)NULL, FEMSG_INIT, 0, 0, 0 );

    return count++;
}

void    ui_manager::Update      ( f32 FrameInc )
{
    s32 Buttons = 0;
    s32 i, j;
//    model*  pModel = m_Instance.GetModel();
    x_rand();

    m_Instance.Advance( FrameInc );   

    // decode user input
    //while(input_UpdateState())
    {
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_CROSS) )      Buttons |= FE_ACCEPT   ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_SQUARE) )     Buttons |= FE_CANCEL   ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L_UP) )       Buttons |= FE_UP       ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L_DOWN) )     Buttons |= FE_DOWN     ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L_LEFT) )     Buttons |= FE_LEFT     ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L_RIGHT) )    Buttons |= FE_RIGHT    ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_TRIANGLE) )   Buttons |= FE_MAIN1    ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_SQUARE) )     Buttons |= FE_MAIN2    ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_CIRCLE) )     Buttons |= FE_MAIN3    ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_CROSS) )      Buttons |= FE_MAIN4    ;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L1) )         Buttons |= FE_SHOULDER1;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_L2) )         Buttons |= FE_SHOULDER2;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_R1) )         Buttons |= FE_SHOULDER3;
        if ( input_WasPressed((input_gadget)INPUT_PS2_BTN_R2) )         Buttons |= FE_SHOULDER4;
    }

#ifdef TARGET_PC
        if ( input_WasPressed((input_gadget)INPUT_KBD_RETURN) )   Buttons |= FE_ACCEPT   ;
        if ( input_WasPressed((input_gadget)INPUT_KBD_BACK) )     Buttons |= FE_CANCEL   ;
        if ( input_WasPressed((input_gadget)INPUT_KBD_UP) )       Buttons |= FE_UP       ;
        if ( input_WasPressed((input_gadget)INPUT_KBD_DOWN) )     Buttons |= FE_DOWN     ;
        if ( input_WasPressed((input_gadget)INPUT_KBD_LEFT) )     Buttons |= FE_LEFT     ;
        if ( input_WasPressed((input_gadget)INPUT_KBD_RIGHT) )    Buttons |= FE_RIGHT    ;
#endif
    // send the input code to the controls on the screen
    j = m_pObj.GetCount();

    m_pObj[m_Active]->ProcInput( Buttons );

    for ( i = 0; i < j; i++ )
    {
        m_pObj[i]->UpdateImage();
    }

    // navigate if necessary
    s32 curactive = m_Active;

    if ( ( Buttons & FE_LEFT )  && ( m_pObj[m_Active]->GetLeft() != -1 ) )
        m_Active = FindControl( m_pObj[m_Active]->GetLeft() );
    else if ( ( Buttons & FE_RIGHT )  && ( m_pObj[m_Active]->GetRight() != -1 ) )
        m_Active = FindControl( m_pObj[m_Active]->GetRight() );
    else if ( ( Buttons & FE_UP )  && ( m_pObj[m_Active]->GetUp() != -1 ) )
        m_Active = FindControl( m_pObj[m_Active]->GetUp() );
    else if ( ( Buttons & FE_DOWN )  && ( m_pObj[m_Active]->GetDown() != -1) )
        m_Active = FindControl( m_pObj[m_Active]->GetDown() );

    // notify the screen of the changes
    if ( curactive != m_Active )
    {
        (*m_MsgProc)( m_pObj[curactive], FEMSG_DEACTIVATED, 0, 0, 0 );
        (*m_MsgProc)( m_pObj[m_Active],  FEMSG_ACTIVATED,   0, 0, 0 );
        audio_Play( SFX_FRONTEND_BUTTONOVER );
    }

    if ( Buttons & FE_ACCEPT )
    {
        (*m_MsgProc)( m_pObj[m_Active], FEMSG_SELECTED, 0, 0, 0 );
        audio_Play( SFX_FRONTEND_BUTTONDOWN );
    }

    if ( Buttons & FE_CANCEL )
    {
        (*m_MsgProc)( m_pObj[m_Active], FEMSG_CANCELLED, 0, 0, 0 );
        audio_Play( SFX_FRONTEND_CANCELSELECTION01 );
    }
}


void ui_manager::Render( void )
{
    ASSERT(m_pShape) ;
    ASSERT(m_Instance.GetModel()) ;
    ASSERT(m_Instance.GetCurrentAnimState().GetAnim()) ;

    // Activate view
    eng_MaximizeViewport( m_FrontEndView) ;
    eng_SetView(m_FrontEndView, 0) ;
    eng_ActivateView ( 0 );

    //x_printfxy( 0,0,"Shape:%s", m_pShape->GetName() ) ;
    //x_printfxy( 0,1,"Model:%s", m_Instance.GetModel()->GetName() ) ;
    //x_printfxy( 0,2,"Anim :%s", m_Instance.GetCurrentAnimState().GetAnim()->GetName() ) ;
    //x_printfxy( 0,3,"Frame:%d - %d", (s32)m_Instance.GetCurrentAnimState().GetFrame(),
                                     //m_Instance.GetCurrentAnimState().GetAnim()->GetFrameCount() ) ;

    // Begin shape draw...
    shape::BeginDraw() ;

    // Set instance draw vars
    m_Instance.SetLightDirection    (vector3(0.5f, 0.5f, 1.0f), TRUE) ;
    m_Instance.SetLightColor        (vector4(0.75f, 0.75f, 0.75f, 1.0f)) ;
    m_Instance.SetLightAmbientColor (vector4(0.3f, 0.3f, 0.3f, 1.0f)) ;
    m_Instance.SetFogColor          (xcolor(0,0,0,0)) ;

    // Draw instances
    m_Instance.Draw( shape::DRAW_FLAG_CLIP ) ;

    // End shape draw...
    shape::EndDraw() ;
}

s32	ui_manager::SendMessage		( s32 Msg, s32 Parm1, s32 Parm2, s32 Parm3 )
{
	ASSERTS( m_MsgProc, "Message procedure for this screen undefined.\n" );
	return (*m_MsgProc)( (ui_obj*)NULL, Msg, Parm1, Parm2, Parm3 );	
}

void ui_manager::UnloadTextures  ( void )
{
    m_ShapeMgr.DeleteAllShapes();    
}


///////////////////////////////////////////////////////////////////////////////
// Basic Font Support
void ui_font::Load( const char* pFilename )
{
    s32         i, width;
    
    m_Table.Clear();
    
    if ( auxbmp_LoadNative( m_Bmp, pFilename ) != FALSE )
    {
        width = m_Bmp.GetWidth();
        m_Height = m_Bmp.GetHeight() - 1;  // subtract 1 for top row (spacing info)

        m_Table.Append(0);

        for ( i = 0; i < width; i++ )
        {
            xcolor c = m_Bmp.GetPixelColor( i, 0 );


            if ( c.R != 255 )
            {
                m_Table.Append(i);

            }
        }    
    }
}

void ui_font::Print( xbitmap& Target, s32 x, s32 y, const char* pFormatStr, ... )
{
    char        tbuf[100];                  // fixed size for speed and efficiency
    s32         i, j;
    x_va_list   Args;
    x_va_start( Args, pFormatStr );

    x_vsprintf( tbuf, pFormatStr, Args );

    // now blit the text into the bitmap
    vram_Unregister(Target);                    

    j = 0;
    for ( i = 0 ; i < x_strlen(tbuf); i++ )
    {
        int width = (m_Table[(s32)tbuf[i]-31]) - (m_Table[(s32)tbuf[i]-32]);

        Target.Blit( x + j, y, m_Table[(s32)tbuf[i]-32], 1, width, m_Height, m_Bmp );         
        
        j += width;

    }
    // update vram
    vram_Register(Target);                      
}    
