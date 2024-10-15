#ifndef __FE_HPP
#define __FE_HPP

#include "Shape/ShapeInstance.hpp"
#include "Shape/ShapeManager.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Shape/UserData.hpp"


//==============================================================================
// enumerate the states for front end objects
enum ui_objstate
{
    FEO_ACTIVE, 
    FEO_HILITED,
    FEO_SELECTED,
};


#define FEMSG_INIT          0
#define FEMSG_SELECTED      1
#define FEMSG_ACTIVATED     2
#define FEMSG_DEACTIVATED   3
#define FEMSG_CANCELLED     4
#define FEMSG_EXITSCREEN    5

#define FEMSG_BUTTONMSG     100
#define FEMSG_LISTBOXMSG    200
#define FEMSG_SLIDERMSG     300
#define FEMSG_EDITBOXMSG    400

#define FEMSG_USERDEFINED	10000



//=============================================================================
// some defines for input
#define FE_ACCEPT       (1<<1)
#define FE_CANCEL       (1<<2)
#define FE_UP           (1<<3)
#define FE_DOWN         (1<<4)
#define FE_LEFT         (1<<5)
#define FE_RIGHT        (1<<6)
#define FE_MAIN1        (1<<7)
#define FE_MAIN2        (1<<8)
#define FE_MAIN3        (1<<9)
#define FE_MAIN4        (1<<10)
#define FE_SHOULDER1    (1<<11)
#define FE_SHOULDER2    (1<<12)
#define FE_SHOULDER3    (1<<13)
#define FE_SHOULDER4    (1<<14)

//=============================================================================
// forward references
class   ui_manager;
class   ui_obj;

//=============================================================================
// define a front-end object call-back function
typedef s32  (*ui_msgproc)      ( ui_obj* pObj, s32 Msg, s32 Parm1, s32 Parm2, s32 Parm3 );


typedef void (*ui_nodeproc)     ( const void *Param, const node &Node, matrix4 &mOrient);
typedef void (*ui_renderproc)   ( const void *Param, const node &Node, render_node &RenderNode);



enum fe_type
{
    FE_BUTTON,
    FE_LISTBOX,
    FE_SLIDER,
    FE_EDITBOX

};


//=============================================================================
// The font class
class   ui_font
{
    xbitmap     m_Bmp;
    xarray<s32> m_Table;          // index zero is a space, so offset is 32
                                  // i.e., Table['A'-32] is the letter A index
    s32         m_Height;         // size of the font vertically, in pixels

public:
    void Load( const char* pFilename );
    void Print( xbitmap& Target, s32 x, s32 y, const char* pFormatStr, ... );
    s32  GetHeight( void )        { return m_Height; }         
};


//=============================================================================
// define the base front-end object class that others inherit from
class ui_obj
{
protected:
    fe_type             m_Type;           // the type of construct this is
                        
    s32                 m_Up;             // links
    s32                 m_Down;
    s32                 m_Left;
    s32                 m_Right;
    s32                 m_ID;             // screen or object ID to branch to on select
    xbool               m_Dirty;
                        
    ui_objstate         m_State;          // the state of this object    
    ui_manager*         m_pMgr;           // pointer to the FE manager

    user_data_gui_info* m_pInfo ;         // Ptr to gui info
    s32                 m_Node ;          // Node index
    
    s32                 *m_pChildNodes ;     // Node texture is attached to
	s32					m_nChildNodes;

    s32                 m_Texture ;       // Texture index          
    texture_manager*    m_pTM;            // Texture Manager

public:

    // Set/Get nChildNodes
    s32     GetChildNodeCount      ( void )        { return m_nChildNodes; }
    void    SetChildren            ( s32 nChildren, s32* pChildList );
    xbool   IsChild                ( s32 Idx );

    // Set/Get m_State
    void            SetState    ( ui_objstate Tmp ) { m_State = Tmp; }
    ui_objstate     GetState    ( void ) { return m_State; }

    // Set/Get m_Type
    void            SetType     ( fe_type Tmp ) { m_Type = Tmp; }
    fe_type         GetType     ( void ) { return m_Type; }

    // Set/Get m_Up
    void    SetUp       ( s32 Tmp )         { m_Up = Tmp; }
    s32     GetUp       ( void )            { return m_Up; }
                                            
    // Set/Get m_Down                       
    void    SetDown     ( s32 Tmp )         { m_Down = Tmp; }
    s32     GetDown     ( void )            { return m_Down; }
                                            
    // Set/Get m_Left                       
    void    SetLeft     ( s32 Tmp )         { m_Left = Tmp; }
    s32     GetLeft     ( void )            { return m_Left; }
                                            
    // Set/Get m_Right                      
    void    SetRight    ( s32 Tmp )         { m_Right = Tmp; }
    s32     GetRight    ( void )            { return m_Right; }
                                            
    // Set/Get m_Select                     
    void    SetID       ( s32 Tmp )         { m_ID = Tmp; }
    s32     GetID       ( void )            { return m_ID; }

    // Set/Get m_Dirty
    void    SetDirty        ( xbool Tmp )   { m_Dirty = Tmp; }
    xbool   IsDirty         ( void )        { return m_Dirty; }


    // Set/Get m_pMgr
    void        SetMgr ( ui_manager* Tmp )  { m_pMgr = Tmp; }
    ui_manager* GetMgr ( void )             { return m_pMgr; }

    // Set/Get m_pTM
    void                SetTM               ( texture_manager* Tmp )    { m_pTM = Tmp; }
    texture_manager*    GetTM               ( void )                    { return m_pTM; }

    // Set/Get m_pInfo
    void                SetInfo             ( user_data_gui_info* Tmp ) { m_pInfo = Tmp; }
    user_data_gui_info* GetInfo             ( void )                    { return m_pInfo; }

    // Set/Get m_Node
    void    SetNode     ( s32 Tmp )         { m_Node = Tmp; }
    s32     GetNode     ( void )            { return m_Node; }

    // Set/Get m_Texture
    void    SetTexture  ( s32 Tmp )         { m_Texture = Tmp; }
    s32     GetTexture  ( void )            { return m_Texture; }
    
    //-------------------------------------------------------------------------
    virtual         ~ui_obj     ( );
                    ui_obj      ( );
    virtual void    ProcInput   ( s32 Buttons ) = 0;
    virtual void    UpdateImage ( void ) = 0;           // call when image needs rebuilt

};


//=============================================================================
// now define the specific object types which inherit from the above
class   ui_button : public ui_obj
{ 
    xstring    m_Text;           // text of the button
    xbool       m_IsDown;         // is the button in the "pushed" state?

public:

    // Set/Get Text
    void    SetText     ( xstring& Tmp )       { m_Text = Tmp; }
    void    GetText     ( xstring& Tmp )       { Tmp = m_Text; }

    // Set/Get m_IsDown
    void    SetIsDown       ( xbool Tmp )       { m_IsDown = Tmp; }
    xbool   GetIsDown       ( void )            { return m_IsDown; }

    // define the virtual functions
            ~ui_button  ( );                // virtual destructor
    void    ProcInput   ( s32 Buttons );
    void    UpdateImage ( void );           // call when image needs rebuilt

    void    SetText     ( const char* pText );

};

//=============================================================================
class   ui_listbox : public ui_obj
{ 
    xarray<char*>       m_Items;             // the array of items in the list box
    s32                 m_ItemCount;          // number of items in the array
    s32                 m_SelStart;           // the selection
    s32                 m_SelEnd;             // for selecting a range of items
    s32                 m_ViewHeight;
    s32                 m_ViewStart;
    s32                 m_Width;              // max width for text items
        
public:
    // define the virtual functions
            ui_listbox  ( );
            ~ui_listbox ( );                // virtual destructor
    void    ProcInput   ( s32 Buttons );
    void    UpdateImage ( void );           // call when image needs rebuilt

    void    AddText     ( const char* pText );
    void    InsertText  ( s32 Row, const char* pText );
    void    DeleteRow   ( s32 Row );
	void	DeleteAll	( void );

    // Set/Get m_Width
    void    SetWidth    ( s32 Tmp )     { m_Width = Tmp; }
    s32     GetWidth    ( void )        { return m_Width; }

    s32     GetSelected ( void )        { return m_SelStart; }
    char*   GetText     ( s32 Row );

    s32     GetCount    ( void )        { return m_ItemCount; }

};

//=============================================================================
class   ui_slider : public ui_obj
{ 
    s32                 m_Val1, m_Val2;         // bookend values
    s32                 m_Increment;
    s32                 m_CurVal;

public:
    // define the virtual functions
            ~ui_slider  ( );                // virtual destructor
    void    ProcInput   ( s32 Buttons );
    void    UpdateImage ( void );           // call when image needs rebuilt

    void    SetParms    ( s32 MinVal, s32 MaxVal, s32 Step, s32 CVal );
    void    SetCurVal   ( s32 Val );
    void    GetCurVal   ( s32& Val );
    void    IncVal      ( void );
    void    DecVal      ( void );

};

//=============================================================================
class   ui_edit : public ui_obj
{ 
    xstring            m_Text;               // storage for the text
    xstring            m_Filter;             // string of acceptable input characters
    s32                 m_MinLen;
    s32                 m_MaxLen;

public:
    // define the virtual functions
            ~ui_edit    ( );                // virtual destructor
    void    ProcInput   ( s32 Buttons );
    void    UpdateImage ( void );           // call when image needs rebuilt

    void    SetText     ( const char* pText );
    void    GetText     ( xstring& Txt );
    void    SetLen      ( s32 Min, s32 Max );
    void    SetFilter   ( const char* pTextFilter );
};

//=============================================================================
// Now define the class to manage allocation/deallocation/sending messages
class   ui_manager
{
    xarray<ui_obj*>     m_pObj;       // an array of pointers to front end objects
                        
    ui_msgproc          m_MsgProc;        // the current message handler callback

    s32                 m_Active;         // active control (not necessarily selected)

    // Screen data
    shape*              m_pShape;
    shape_instance      m_Instance;

    shape_manager       m_ShapeMgr;
    view                m_FrontEndView ;  // camera view

    ui_nodeproc         m_NodeProc;
    ui_renderproc       m_RenderProc;

    // common elements for a screen/front-end
    s32                 m_Hummer;
    //sound     SndActivate;    // sounds
    //sound     SndSelect;
    

public:
    ui_font             m_F1, m_F2, m_F3;     // primary, secondary, tertiary fonts
                                                            
    
    s32     FindControl         (s32 NodeIndex);
    s32     FindControlByID     (s32 ID);
	ui_obj* FindControlPtrByID	(s32 ID);

    // Set/Get m_Active
    void    SetActive   ( s32 Tmp )                         { m_Active = Tmp; }
    s32     GetActive   ( void )                            { return m_Active; }
                                                            
    // Set/Get m_MsgProc                                    
    void    SetMsgProc  ( ui_msgproc& Tmp )                 { m_MsgProc = Tmp; }
    void    GetMsgProc  ( ui_msgproc& Tmp )                 { Tmp = m_MsgProc; }
                                                            
    // Set/Get m_NodeProc
    void    SetNodeProc         ( ui_nodeproc& Tmp )        { m_NodeProc = Tmp; }
    void    GetNodeProc         ( ui_nodeproc& Tmp )        { Tmp = m_NodeProc; }

    // Set/Get m_RenderProc
    void    SetRenderProc       ( ui_renderproc& Tmp )      { m_RenderProc = Tmp; }
    void    GetRenderProc       ( ui_renderproc& Tmp )      { Tmp = m_RenderProc; }

    // Set/Get m_pShape
    void    SetShape            ( shape* Tmp )              { m_pShape = Tmp; }
    shape*  GetShape            ( void )                    { return m_pShape; }

    void    SetAnim             ( s32 Anim )                { m_Instance.SetAnimByType( Anim ); }

    // Get a pointer to an object
    ui_obj& GetObj              ( s32 Index )               { return *(m_pObj[Index]); }

    ui_manager          ( );    // constructor
    ~ui_manager         ( );    // destructor

	// allow other parts of the code to send messages to the current screen
	s32		SendMessage		( s32 Msg, s32 Parm1, s32 Parm2, s32 Parm3 );

    // load a screen and set the callback for the screen
    s32     LoadShapeData   ( const char* pFilename, ui_msgproc pMP, 
                                ui_nodeproc pNP, ui_renderproc pRP );
                            
    void    Update          ( f32 FrameInc );      
    void    Render          ( void );
    void    UnloadTextures  ( void );
};


/*  
    ------------------------------------------------------------------------
    The application specifies a callback for a screen.  This callback is the
    message handler.  The message handler receives system-general messages,
    such as button presses.  It also receives control messages, such as when
    a control is activated or selected.  Lastly, it receives messages sent
    from the individual controls, for example when a listbox is scrolled up
    or down.

    The message handler routine can then call the interface functions for a
    control class directly.  This gives the callback complete control over
    the front-end, allowing it to override an action or submit default data
    to a control (like automatically putting text in a text box).

    When a screen is finished, the code can hand off to another callback for
    that screen.
    ------------------------------------------------------------------------
*/



#endif
