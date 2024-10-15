//==============================================================================
// Commands.cpp : implementation file
//==============================================================================

#include "stdafx.h"
#include "xTB.h"
#include "Commands.h"
#include "Shlwapi.h"
#include <process.h>
#include <errno.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==============================================================================

enum target_id
{
    TARGET_UNKNOWN,
    TARGET_PC,
    TARGET_PS2,
    TARGET_PS2_IOP,
    TARGET_GCN,
    TARGET_XBOX,
};

enum vendor_id
{
    VENDOR_UNKNOWN,
    VENDOR_MICROSOFT,
    VENDOR_SN,
    VENDOR_METROWERKS,
};

enum post_build_action
{
    POST_BUILD_NONE,
    POST_BUILD_LAUNCH,
    POST_BUILD_DEBUG,
};

//==============================================================================

LRESULT CALLBACK XTB_DialogConfigureXCL( HWND   hDlg, 
                                         UINT   message, 
                                         WPARAM wParam,
                                         LPARAM lParam );

//==============================================================================

static IApplication* pApp = NULL;

static
void CheckSpawnResult( int Result )
{
    if( Result != -1 ) 
        return;

    if( pApp == NULL )
        return;

    switch( errno )
    {
    case E2BIG:     pApp->PrintToOutputWindow( L"xTB: E2BIG" );  
                    break;
    case EINVAL:    pApp->PrintToOutputWindow( L"xTB: EINVAL" ); 
                    break;
    case ENOENT:    pApp->PrintToOutputWindow( L"xTB: ENOENT" ); 
                    break;
    case ENOEXEC:   pApp->PrintToOutputWindow( L"xTB: ENOEXEC" );
                    break;
    case ENOMEM:    pApp->PrintToOutputWindow( L"xTB: ENOMEM" ); 
                    break;
    default:        pApp->PrintToOutputWindow( L"xTB: Unknown error code" );
                    break;
    }
}

//==============================================================================

static
bool LookUpVerbose( void )
{
    HKEY  Key = 0;
    DWORD Disposition;
    DWORD Value;
    DWORD Size;

    RegCreateKeyEx( HKEY_CURRENT_USER, 
                    "software\\Inevitable\\xTB", 
                    0, 
                    "REG_DWORD", 
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS, 
                    NULL,
                    &Key,
                    &Disposition );

    RegQueryValueEx( Key, "Verbose", 0, NULL, (unsigned char*)(&Value), &Size );
    return( Value == 1 );
}

//==============================================================================
//==============================================================================
//  COMMAND FUNCTIONS
//==============================================================================
//==============================================================================

STDMETHODIMP CCommands::XTB_ConfigureXCL( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    DialogBox( AfxGetInstanceHandle(), 
               (LPCTSTR)IDD_CONFIG_XCL, 
               NULL, 
               (DLGPROC)XTB_DialogConfigureXCL );
    
    return( S_OK );
}

//==============================================================================

STDMETHODIMP CCommands::XTB_LaunchProgram( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    if( m_pApplicationEventsObj == NULL )
        return( S_OK );

    bool Verbose = LookUpVerbose();

    m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_NONE;

    info_block& Info = m_pApplicationEventsObj->m_Info;

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    // We need an executable.
    if( Info.Executable == "" )
        return( S_OK );

    // Handle the situation.

    //--------------------------------------------------------------------------
    if( Info.Target == TARGET_PC )
    {
        m_pApplication->ExecuteCommand( L"BuildExecute" );
    }
    //--------------------------------------------------------------------------
    else
    if( (Info.Target == TARGET_PS2) && 
        (Info.Vendor == VENDOR_SN ) )
    {
        pApp = m_pApplication;
        m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_LAUNCH;
        m_pApplication->ExecuteCommand( L"Build" );
    }
    //--------------------------------------------------------------------------
    else
    if( (Info.Target == TARGET_GCN) && 
        (Info.Vendor == VENDOR_SN ) )
    {
        pApp = m_pApplication;
        m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_LAUNCH;
        m_pApplication->ExecuteCommand( L"Build" );
    }
    //--------------------------------------------------------------------------

    // That's it.
    return( S_OK );
}

//==============================================================================

STDMETHODIMP CCommands::XTB_DebugProgram( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    if( m_pApplicationEventsObj == NULL )
        return( S_OK );

    m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_NONE;

    info_block& Info = m_pApplicationEventsObj->m_Info;

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    // We need an executable.
    if( Info.Executable == "" )
        return( S_OK );

    // Handle the situation.

    //--------------------------------------------------------------------------
    if( Info.Target == TARGET_PC )
    {
        m_pApplication->ExecuteCommand( L"DebugGo" );
    }
    //--------------------------------------------------------------------------
    else
    if( (Info.Target == TARGET_PS2) && 
        (Info.Vendor == VENDOR_SN ) )
    {
        pApp = m_pApplication;
        m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_DEBUG;
        m_pApplication->ExecuteCommand( L"Build" );
    }
    //--------------------------------------------------------------------------
    else
    if( (Info.Target == TARGET_GCN) && 
        (Info.Vendor == VENDOR_SN ) )
    {
        pApp = m_pApplication;
        m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_DEBUG;
        m_pApplication->ExecuteCommand( L"Build" );
    }
    //--------------------------------------------------------------------------

    // That's it.
    return( S_OK );
}

//==============================================================================

STDMETHODIMP CCommands::XTB_AttachDebugger( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    char        Command[  512 ];
    char        Args   [ 8192 ];
    int         Result;
    info_block  Info;
    char*       UserName = getenv( "UserName" );
    char*       PS2_IP   = getenv( "PS2_IP"   );

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    // Handle the situation.

    //--------------------------------------------------------------------------
    if( Info.Target == TARGET_PC )
    {
        m_pApplication->ExecuteCommand( L"DebugGo" );
    }
    //--------------------------------------------------------------------------
    else
    if( (Info.Target == TARGET_PS2) && 
        (Info.Vendor == VENDOR_SN ) )
    {
        if( Info.pBase == NULL )
        {
            AfxMessageBox( "xTB: Could not locate appropriate xCore directory." );
        	return( S_OK );
        }

        if( (UserName == NULL) || (*UserName == '\0') )
        {
            AfxMessageBox( "UserName environment variable not set." );
        	return( S_OK );
        }

        if( (PS2_IP == NULL) || (*PS2_IP == '\0') )
        {
            AfxMessageBox( "PS2_IP environment variable not set." );
        	return( S_OK );
        } 

        // PS2TM -a <UserName> -i <IP> -t <UserName> -f . -h . -m

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2TM.exe", Info.pBase );
        sprintf( Args,    " -a %s -i %s -t %s -f . -h . -m", 
                          UserName, PS2_IP, UserName );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );

        // PS2DBG -k1 -vs -t <UserName> <Program>

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2DBG.exe", Info.pBase );
        sprintf( Args,    " -k1 -vs -t %s %s ", UserName, Info.Executable.GetBuffer(0) );
        
        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }
    //--------------------------------------------------------------------------

    // That's it.
    return( S_OK );
}

//==============================================================================

STDMETHODIMP CCommands::XTB_TargetManager( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    char        Command[  512 ];
    char        Args   [ 8192 ];
    int         Result;
    info_block  Info;
    char*       UserName = getenv( "UserName" );
    char*       PS2_IP   = getenv( "PS2_IP"   );

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    // Handle the situation.

    //--------------------------------------------------------------------------
    if( ((Info.Target == TARGET_PS2) || (Info.Target == TARGET_PS2_IOP)) && 
        (Info.Vendor == VENDOR_SN) )
    {
        if( Info.pBase == NULL )
        {
            AfxMessageBox( "xTB: Could not locate appropriate xCore directory." );
        	return( S_OK );
        }

        if( (UserName == NULL) || (*UserName == '\0') )
        {
            AfxMessageBox( "UserName environment variable not set." );
        	return( S_OK );
        }

        if( (PS2_IP == NULL) || (*PS2_IP == '\0') )
        {
            AfxMessageBox( "PS2_IP environment variable not set." );
        	return( S_OK );
        } 

        // PS2TM -a <UserName> -i <IP> -t <UserName> -f . -h .

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2TM.exe", Info.pBase );
        sprintf( Args,    " -a %s -i %s -t %s -f . -h . -m", 
                          UserName, PS2_IP, UserName );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }
    //--------------------------------------------------------------------------
    if( (Info.Target == TARGET_GCN) && (Info.Vendor == VENDOR_SN) )
    {
        if( Info.pBase == NULL )
        {
            AfxMessageBox( "xTB: Could not locate appropriate xCore directory." );
        	return( S_OK );
        }

        sprintf( Command, "%s\\3rdParty\\GCN\\SN\\NGCTM.exe", Info.pBase );
        sprintf( Args,    " -m -d %s\\data",Info.ProjDir );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }
    //--------------------------------------------------------------------------

    // That's it.
    return( S_OK );
}

//==============================================================================
/*
STDMETHODIMP CCommands::XTB_Example( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    info_block  Info;

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    return( S_OK );
}
*/
//==============================================================================
/*
STDMETHODIMP CCommands::XTB_Example( void )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    // TO DO: Replace this with the actual code to execute this command
    //  Use m_pApplication to access the Developer Studio Application object,
    //  and VERIFY_OK to see error strings in DEBUG builds of your add-in
    //  (see stdafx.h)

    info_block  Info;

    // Collect required information.
    if( GetInformation( Info ) == false )
        return( S_OK );

    VERIFY_OK( m_pApplication->EnableModeless(VARIANT_FALSE) );
    ::MessageBox( NULL, "XTB Command invoked.", "XTB", MB_OK | MB_ICONINFORMATION );
    VERIFY_OK( m_pApplication->EnableModeless(VARIANT_TRUE ) );

    return( S_OK );
}
*/
//==============================================================================
//==============================================================================
//  UTILITY FUNCTIONS
//==============================================================================
//==============================================================================

bool CCommands::GetInformation( info_block& Info )
{
    // Create interface pointers (IDispatch is temporary).
    IDispatch*      pDisp   = NULL;
    IConfiguration* pConfig = NULL;
    IBuildProject*  pProj   = NULL;

    bool Verbose = LookUpVerbose();

    // Get information on the project.
    {
        CComBSTR Name;

        // Get the Dispatch interface.

        if( Verbose )   
            m_pApplication->PrintToOutputWindow( L"xTB: Getting active project." );

        m_pApplication->get_ActiveProject( &pDisp );
        if( pDisp == NULL )
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Failed." );
            AfxMessageBox( "xTB: Internal Error: Failed to retrieve project dispatch." );
            return( false );
        }
        else
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Succeeded." );
        }

        // QI for the Configuration interface.

        if( Verbose )   
            m_pApplication->PrintToOutputWindow( L"xTB: Getting project interface." );

        pDisp->QueryInterface( IID_IBuildProject, (void**)&pProj );
        if( pProj == NULL )
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Failed." );
            AfxMessageBox( "xTB: Internal Error: Failed to retrieve project interface." );
            return( false );
        }
        else
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Succeeded." );
        }

        // Grab the DSP file name.
        pProj->get_FullName( &Name );
        Info.DSPName = Name.m_str;       

        // Grab the project name.
        pProj->get_Name( &Name );
        Info.ProjectName = Name.m_str;       

        // Release the project and the dispatch.
        pProj->Release();
        pDisp->Release();
    }

    // Get information on the configuration.
    {
        CComBSTR Name;

        // Get the Dispatch interface.

        if( Verbose )   
            m_pApplication->PrintToOutputWindow( L"xTB: Getting active configuration." );

        m_pApplication->get_ActiveConfiguration( &pDisp );
        if( pDisp == NULL )
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Failed." );
            AfxMessageBox( "xTB: Internal Error: Failed to retrieve configuration dispatch." );
            return( false );
        }
        else
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Succeeded." );
        }

        // QI for the Configuration interface.

        if( Verbose )   
            m_pApplication->PrintToOutputWindow( L"xTB: Getting configuration interface." );

        pDisp->QueryInterface( IID_IConfiguration, (void**)&pConfig );
        if( pConfig == NULL )
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Failed." );
            AfxMessageBox( "xTB: Internal Error: Failed to retrieve configuration interface." );
            return( false );
        }
        else
        {
            if( Verbose )   
                m_pApplication->PrintToOutputWindow( L"     Succeeded." );
        }

        // Grab the configuration name.
        pConfig->get_Name( &Name );
        Info.ConfigName = Name.m_str;        
        pConfig->Release();

        // Release the dispatch.
        pDisp->Release();
    }

    // Get the working directory from the dsp name.
    Info.ProjDir = Info.DSPName;
    PathRemoveFileSpec( Info.ProjDir.GetBuffer(_MAX_PATH) );
    Info.ProjDir.ReleaseBuffer();

    //
    // Now the fun really starts.  We need to slog thru the DSP file to
    // get certain information.  (Thanks, Microsoft.)
    //

    Info.Target = TARGET_UNKNOWN;
    Info.Vendor = VENDOR_UNKNOWN;

    CString TargetDefine;
    CString VendorDefine;
    bool    LinkerFlag = false;
    char    Grab[128];
    char*   pBuffer = NULL;
    char*   pWalk;

    // Read the entire file into a buffer.
    try 
    {
        CFile File( Info.DSPName, CFile::modeRead | CFile::shareDenyNone );
        int   Size = File.GetLength();
        pBuffer = (char*)malloc( Size );
        if( pBuffer == NULL )
        {
            AfxMessageBox( "xTB: Internal Error: Failed to allocate DSP buffer." );
            return( false );
        }
        File.Read( (void*)pBuffer, Size );
        File.Close();
    } 
    catch( ... ) 
    {
        AfxMessageBox( "xTB: Internal Error: Failed to read DSP." );
        if( pBuffer )
            free( pBuffer );
        return( false );
    }    

    // We have to find the section that has parameters for the current 
    // configuration.  It will look like one of the following:
    //
    //  !IF      "$(CFG)" == "<project_name> - Win32 <config_name>"
    //  !ELSEIF  "$(CFG)" == "<project_name> - Win32 <config_name>"
    //  !IF      "$(CFG)" == "<project_name> - Xbox <config_name>"
    //  !ELSEIF  "$(CFG)" == "<project_name> - Xbox <config_name>"
    //
    // Just search for ["$(CFG)" == ].  Then see if the full configuration
    // string follows it.

    pWalk = strstr( pBuffer, "\"$(CFG)\" == " );

    while( pWalk )
    {
        if( strncmp( pWalk+13, (const char*)Info.ConfigName, Info.ConfigName.GetLength() ) == 0 )
        {
            // If we are here, then we found the begining of the configuarion
            // section.  Just break out.
            break;  
        }
        pWalk = strstr( pWalk+1, "\"$(CFG)\" == " );
    }

    // If pWalk is NULL, then we did not find the active configuration.
    if( pWalk == NULL )
    {
        AfxMessageBox( "xTB: Could not locate active configuration." );
        free( pBuffer );
        return( false );
    }

    // We have located the configuration section.  We want to find the 
    // following:
    //  - Name of the executable to be built.
    //  - Target and Vendor (via TARGET_??? and VENDOR_???).
    //  - Output directory.

    // While:
    //  - we haven't hit NULL, and 
    //  - we haven't hit a '!', and
    //  - we found another new line...

    while( (pWalk) && 
           (*pWalk != '!') && 
           (pWalk = strstr( pWalk, "\r\n" )) )
    {
        // Skip past the CR/LF.
        pWalk += 2;

        // Look for the output directory.
        if( (strncmp( pWalk, "# PROP Output_Dir",       17 ) == 0) || 
            (strncmp( pWalk, "# PROP       Output_Dir", 23 ) == 0) )
        {
            char* pTemp;
            pWalk += 19;
            pTemp  = pWalk;
            pWalk  = strchr( pWalk, '"' );
            memset( Grab, 0, 128 );
            memcpy( Grab, pTemp, pWalk-pTemp );
            Info.OutputDir = Grab;
        }
        else
        // Look for the link line.
        if( (strncmp( pWalk, "# ADD LINK32",      12 ) == 0) || 
            (strncmp( pWalk, "# ADD      LINK32", 17 ) == 0) )
        {
            // We have a LINK line.
            char* pHold = pWalk;
            char* pNext;
            pNext  = strstr( pWalk, "\r\n" );
            *pNext = '\0';

            // We may need this a little later.
            LinkerFlag = true;

            pWalk  = strstr( pHold, "/D:TARGET_" );
            if( pWalk )
            {
                char* pTemp = pWalk + 3;
                pWalk = pTemp;
                while( !isspace(*pWalk) )
                    pWalk++;
                memset( Grab, 0, 128 );
                memcpy( Grab, pTemp, pWalk-pTemp );
                TargetDefine = Grab;
            }

            pWalk  = strstr( pHold, "/D:VENDOR_" );
            if( pWalk )
            {
                char* pTemp = pWalk + 3;
                pWalk = pTemp;
                while( !isspace(*pWalk) )
                    pWalk++;
                memset( Grab, 0, 128 );
                memcpy( Grab, pTemp, pWalk-pTemp );
                VendorDefine = Grab;
            }

            pWalk  = strstr( pHold, "/out:" );
            if( pWalk )
            {
                char* pTemp = pWalk + 5;
                if( *pTemp = '"' )
                {
                    pTemp++;
                    pWalk = pTemp;
                    while( *pWalk != '"' )
                        pWalk++;
                }
                else
                {
                    pWalk = pTemp;
                    while( !isspace(*pWalk) )
                        pWalk++;
                }
                memset( Grab, 0, 128 );
                memcpy( Grab, pTemp, pWalk-pTemp );
                Info.Executable = Grab;
            }

            pWalk  = pNext;
            *pWalk = '\r';
        }               
    }

    // We are now done with the buffer.  Set it free!
    free( pBuffer );
    pBuffer = NULL;

    // Let's take a look at the TargetDefine and VendorDefine.
    // (Be careful on the order of PS2 and PS2_IOP.)
    
    if     ( TargetDefine             == ""         )   Info.Target = TARGET_PC;
    else if( TargetDefine.Mid( 7, 2 ) == "PC"       )   Info.Target = TARGET_PC;
    else if( TargetDefine.Mid( 7, 7 ) == "PS2_IOP"  )   Info.Target = TARGET_PS2_IOP;
    else if( TargetDefine.Mid( 7, 3 ) == "PS2"      )   Info.Target = TARGET_PS2;
    else if( TargetDefine.Mid( 7, 3 ) == "GCN"      )   Info.Target = TARGET_GCN;
    else if( TargetDefine.Mid( 7, 4 ) == "XBOX"     )   Info.Target = TARGET_XBOX;

    if     ( VendorDefine             == ""         )   Info.Vendor = VENDOR_MICROSOFT;
    else if( VendorDefine.Mid( 7, 2 ) == "MS"       )   Info.Vendor = VENDOR_MICROSOFT;
    else if( VendorDefine.Mid( 7, 2 ) == "SN"       )   Info.Vendor = VENDOR_SN;
    else if( VendorDefine.Mid( 7, 2 ) == "MW"       )   Info.Vendor = VENDOR_METROWERKS;

    // OK.  If there was a link line, BUT there was no executable identified,
    // then DevStudio is assuming a default.  We will go ahead and set our
    // Executable value accordingly.
    if( LinkerFlag && Info.Executable.IsEmpty() )
    {
        if( Info.Target == TARGET_XBOX )
            Info.Executable = Info.OutputDir + '\\' + Info.DSPName + ".exe";
        else
            Info.Executable = Info.OutputDir + '\\' + Info.DSPName + ".xbe";
    }

    // Set the working directory.
    SetCurrentDirectory( Info.ProjDir );

    // Find the xCore directory.
    Info.pBase = FindSubDirectory( "xCore" );

    // And that's a wrap!
    return( true );
}

//==============================================================================

char* CCommands::FindSubDirectory( char* pName )
{
    struct _finddata_t Finder;
    int                Handle;
    char*              p;
    bool               Found = false;

    // Attempt to get the current working directory.
    p = _getcwd( m_SubDirectory, _MAX_PATH );
    if( !p )
        return( NULL );

    // Look for the desired subdirectory.
    // If not found, back up a level and try again.

    // Point p at the '\0' at the end of the current path.
    p = &m_SubDirectory[ strlen(m_SubDirectory) ];

    while( (!Found) && (p >= m_SubDirectory) )
    {
        // Terminate the string.  (May not be the first iteration!)
        *p = '\0';

        // Append the directory name to the candidate path.
        *p = '\\';
        strcpy( p+1, pName );

        // Start searching...
        if( (Handle = _findfirst( m_SubDirectory, &Finder )) != -1 )
        {
            // Found something with name in pName.  Is it a directory?
            do
            {
                if( Finder.attrib & _A_SUBDIR )
                    Found = true;
            } while( (!Found) && (_findnext( Handle, &Finder ) == 0) );
        }

        // If not found, back up a directory.
        if( !Found )
        {
            do
            {
                p--;
            } while( (p > m_SubDirectory) && 
                     (*p != '\\') &&
                     (*p != '/') );
        }
    }

    if( !Found )
        return( NULL );

    return( m_SubDirectory );
}

//==============================================================================
// CCommands
//==============================================================================

CCommands::CCommands()
{
    m_pApplication          = NULL;
    m_pApplicationEventsObj = NULL;
    m_pDebuggerEventsObj    = NULL;
}

//==============================================================================

CCommands::~CCommands()
{
    ASSERT( m_pApplication != NULL );
    m_pApplication->Release();
}

//==============================================================================

void CCommands::SetApplicationObject( IApplication* pApplication )
{
    // This function assumes pApplication has already been AddRef'd
    //  for us, which CDSAddIn did in its QueryInterface call
    //  just before it called us.
    m_pApplication = pApplication;

    // Create Application event handlers
    XApplicationEventsObj::CreateInstance( &m_pApplicationEventsObj );
    m_pApplicationEventsObj->AddRef();
    m_pApplicationEventsObj->Connect( m_pApplication );
    m_pApplicationEventsObj->m_pCommands       = this;
    m_pApplicationEventsObj->m_PostBuildAction = POST_BUILD_NONE;

    // Create Debugger event handler
    CComPtr<IDispatch> pDebugger;
    if( SUCCEEDED( m_pApplication->get_Debugger( &pDebugger ) ) && (pDebugger != NULL) )
    {
        XDebuggerEventsObj::CreateInstance( &m_pDebuggerEventsObj );
        m_pDebuggerEventsObj->AddRef();
        m_pDebuggerEventsObj->Connect( pDebugger );
        m_pDebuggerEventsObj->m_pCommands = this;
    }
}

//==============================================================================

void CCommands::UnadviseFromEvents()
{
    ASSERT( m_pApplicationEventsObj != NULL );
    m_pApplicationEventsObj->Disconnect( m_pApplication );
    m_pApplicationEventsObj->Release();
    m_pApplicationEventsObj = NULL;

    if( m_pDebuggerEventsObj != NULL )
    {
        // Since we were able to connect to the Debugger events, we
        //  should be able to access the Debugger object again to
        //  unadvise from its events (thus the VERIFY_OK below--see stdafx.h).
        CComPtr<IDispatch> pDebugger;
        VERIFY_OK( m_pApplication->get_Debugger( &pDebugger ) );
        ASSERT( pDebugger != NULL );
        m_pDebuggerEventsObj->Disconnect( pDebugger );
        m_pDebuggerEventsObj->Release();
        m_pDebuggerEventsObj = NULL;
    }
}

//==============================================================================
// Event handlers
//==============================================================================

// TODO: Fill out the implementation for those events you wish handle
//  Use m_pCommands->GetApplicationObject() to access the Developer
//  Studio Application object

//------------------------------------------------------------------------------
// Application events

HRESULT CCommands::XApplicationEvents::BeforeBuildStart()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    {
        info_block  Info;

        // Collect required information.
        if( (m_pCommands->GetInformation( Info ) == false) ||
            (Info.pBase == NULL) )
        {
            SetEnvironmentVariable( "X", "" );
        }
        else
        {        
            SetEnvironmentVariable( "X", Info.pBase );
        }
    }

    return( S_OK );
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::BuildFinish( long nNumErrors, long nNumWarnings )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    char    Command[  512 ];
    char    Args   [ 8192 ];
    int     Result;

    //--------------------------------------------------------------------------
    //  PS2
    //--------------------------------------------------------------------------

    if( (nNumErrors        == 0                ) &&
        (m_PostBuildAction == POST_BUILD_LAUNCH) &&
        (m_Info.Target     == TARGET_PS2       ) && 
        (m_Info.Vendor     == VENDOR_SN        ) )
    {
        char* UserName = getenv( "UserName" );
        char* PS2_IP   = getenv( "PS2_IP"   );

        if( (UserName == NULL) || (*UserName == '\0') )
        {
            AfxMessageBox( "UserName environment variable not set." );
        	return( S_OK );
        }

        if( (PS2_IP == NULL) || (*PS2_IP == '\0') )
        {
            AfxMessageBox( "PS2_IP environment variable not set." );
        	return( S_OK );
        } 

        // PS2TM -a <UserName> -i <IP> -t <UserName> -f . -h . -m

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2TM.exe", m_Info.pBase );
        sprintf( Args,    " -a %s -i %s -t %s -f . -h . -m", 
                          UserName, PS2_IP, UserName );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );

        // PS2Run -t <UserName> -r -p <Program>

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2Run.exe", m_Info.pBase );
        sprintf( Args,    " -t %s -r -p %s ", UserName, m_Info.Executable.GetBuffer(0) );
    
        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }

    //--------------------------------------------------------------------------

    if( (nNumErrors        == 0               ) &&
        (m_PostBuildAction == POST_BUILD_DEBUG) &&
        (m_Info.Target     == TARGET_PS2      ) && 
        (m_Info.Vendor     == VENDOR_SN       ) )
    {
        char* UserName = getenv( "UserName" );
        char* PS2_IP   = getenv( "PS2_IP"   );

        if( (UserName == NULL) || (*UserName == '\0') )
        {
            AfxMessageBox( "UserName environment variable not set." );
        	return( S_OK );
        }

        if( (PS2_IP == NULL) || (*PS2_IP == '\0') )
        {
            AfxMessageBox( "PS2_IP environment variable not set." );
        	return( S_OK );
        } 

        // PS2TM -a <UserName> -i <IP> -t <UserName> -f . -h . -m

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2TM.exe", m_Info.pBase );
        sprintf( Args,    " -a %s -i %s -t %s -f . -h . -m", 
                          UserName, PS2_IP, UserName );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );

        // PS2DBG -vs -t <UserName> -re <Program>

        sprintf( Command, "%s\\3rdParty\\PS2\\SN\\bin\\PS2DBG.exe", m_Info.pBase );
        sprintf( Args,    " -vs -t %s -re %s", 
                          UserName, 
                          m_Info.Executable.GetBuffer(0) );
    
        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }

    //--------------------------------------------------------------------------
    //  GCN
    //--------------------------------------------------------------------------

    if( (nNumErrors        == 0                ) &&
        (m_PostBuildAction == POST_BUILD_LAUNCH) &&
        (m_Info.Target     == TARGET_GCN       ) && 
        (m_Info.Vendor     == VENDOR_SN        ) )
    {        
        sprintf( Command, "%s\\3rdParty\\GCN\\SN\\BIN\\NGCTM.exe", m_Info.pBase );
        sprintf( Args,    " -t gdev0 -r -x %s -d %s\\data", 
                          m_Info.Executable.GetBuffer(0),
                          m_Info.ProjDir );

        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );        
    }

    //--------------------------------------------------------------------------

    if( (nNumErrors        == 0               ) &&
        (m_PostBuildAction == POST_BUILD_DEBUG) &&
        (m_Info.Target     == TARGET_GCN      ) && 
        (m_Info.Vendor     == VENDOR_SN       ) )
    {    
        // NGCTM -t gdev0 -m -xd <Program> -d <DataDir>

        sprintf( Command, "%s\\3rdParty\\GCN\\SN\\bin\\NGCTM.exe", m_Info.pBase );
        sprintf( Args,    " -t gdev0 -m -xd %s -d %s\\Data",
                          m_Info.Executable.GetBuffer(0),
                          m_Info.ProjDir );
            
        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result ); 
        
        // NGCDBG -d -e -r -xd <Program>

        sprintf( Command, "%s\\3rdParty\\GCN\\SN\\BIN\\NGCDBG.exe", m_Info.pBase );
        sprintf( Args,    " -e -r -p3 -vs %s", m_Info.Executable.GetBuffer(0) );
    
        Result = _spawnl( _P_NOWAIT, Command, Args, NULL );
        CheckSpawnResult( Result );
    }

    //--------------------------------------------------------------------------
    
    m_PostBuildAction = POST_BUILD_NONE;

    return( S_OK );
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::BeforeApplicationShutDown()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::DocumentOpen(IDispatch* theDocument)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::BeforeDocumentClose(IDispatch* theDocument)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::DocumentSave(IDispatch* theDocument)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::NewDocument(IDispatch* theDocument)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::WindowActivate(IDispatch* theWindow)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::WindowDeactivate(IDispatch* theWindow)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::WorkspaceOpen()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::WorkspaceClose()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------

HRESULT CCommands::XApplicationEvents::NewWorkspace()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}

//------------------------------------------------------------------------------
// Debugger event

HRESULT CCommands::XDebuggerEvents::BreakpointHit(IDispatch* pBreakpoint)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return S_OK;
}  

//==============================================================================
// Message handler for Dialog Box.
//==============================================================================

LRESULT CALLBACK XTB_DialogConfigureXCL( HWND   hDlg, 
                                         UINT   message, 
                                         WPARAM wParam,
                                         LPARAM lParam )
{
    HKEY  Key = 0;
    DWORD Disposition;
    DWORD Value;
    DWORD Size;

    RegCreateKeyEx( HKEY_CURRENT_USER, 
                    "software\\Inevitable\\xTB", 
                    0, 
                    "REG_DWORD", 
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS, 
                    NULL,
                    &Key,
                    &Disposition );

    switch( message )
    {
        case WM_INITDIALOG:
            {
                RegQueryValueEx( Key, "Verbose", 0, NULL, (unsigned char*)(&Value), &Size );
                CheckDlgButton( hDlg, IDC_VERBOSE, (Value==1) ? BST_CHECKED : BST_UNCHECKED );

                RegQueryValueEx( Key, "CommandLineEcho", 0, NULL, (unsigned char*)(&Value), &Size );
                CheckDlgButton( hDlg, IDC_CMDLN_ECHO, (Value==1) ? BST_CHECKED : BST_UNCHECKED );

                RegQueryValueEx( Key, "ShowCompileTimes", 0, NULL, (unsigned char*)(&Value), &Size );
                CheckDlgButton( hDlg, IDC_SHOW_TIME, (Value==1) ? BST_CHECKED : BST_UNCHECKED );

                RegCloseKey( Key );
                return( TRUE );
            }
        case WM_COMMAND:
            {
                int Command = LOWORD(wParam);

                if( Command == IDOK )
                {
                    Value = IsDlgButtonChecked( hDlg, IDC_VERBOSE );
                    RegSetValueEx( Key, "Verbose", 0, REG_DWORD, (unsigned char*)(&Value), 4 );

                    Value = IsDlgButtonChecked( hDlg, IDC_CMDLN_ECHO );
                    RegSetValueEx( Key, "CommandLineEcho", 0, REG_DWORD, (unsigned char*)(&Value), 4 );

                    Value = IsDlgButtonChecked( hDlg, IDC_SHOW_TIME );
                    RegSetValueEx( Key, "ShowCompileTimes", 0, REG_DWORD, (unsigned char*)(&Value), 4 );

                    RegCloseKey( Key );
                    EndDialog( hDlg, Command );
                    return( TRUE );
                }

                if( Command == IDCANCEL )
                {
                    EndDialog( hDlg, Command );
                    return( TRUE );
                }
            }
            break;
    }

    RegCloseKey( Key );
    return( FALSE );
}

//==============================================================================
