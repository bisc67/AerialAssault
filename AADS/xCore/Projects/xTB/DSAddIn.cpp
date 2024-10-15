// AddInMod.cpp : implementation file
//

#include "stdafx.h"
#include "xTB.h"
#include "DSAddIn.h"
#include "Commands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called when the user first loads the add-in, and on start-up
//  of each subsequent Developer Studio session
STDMETHODIMP CDSAddIn::OnConnection( IApplication*  pApp, 
                                     VARIANT_BOOL   bFirstTime,
                                     long           dwCookie, 
                                     VARIANT_BOOL*  pOnConnection )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );
    
    // Store info passed to us
    IApplication* pApplication = NULL;
    if( FAILED( pApp->QueryInterface( IID_IApplication, (void**)&pApplication ) )
        || (pApplication == NULL) )
    {
        *pOnConnection = VARIANT_FALSE;
        return( S_OK );
    }

    m_dwCookie = dwCookie;

    // Create command dispatch, send info back to DevStudio
    CCommandsObj::CreateInstance( &m_pCommands );
    m_pCommands->AddRef();

    // The QueryInterface above AddRef'd the Application object.  It will
    //  be Release'd in CCommand's destructor.
    m_pCommands->SetApplicationObject( pApplication );

    // (See stdafx.h for the definition of VERIFY_OK.)

    VERIFY_OK( pApplication->SetAddInInfo( (long)AfxGetInstanceHandle(),
                                           (LPDISPATCH)m_pCommands, 
                                           IDR_TOOLBAR_MEDIUM, 
                                           IDR_TOOLBAR_LARGE, 
                                           m_dwCookie ) );

    // Inform DevStudio of the commands we implement.
    // The "AddCmd" used below is a helper function.

    //          [Function Name]          [String ID]
//  AddCmd( _T( "XTB_Example"        ),  IDS_XTB_EXAMPLE,     pApplication, bFirstTime, pOnConnection );
    AddCmd( _T( "XTB_ConfigureXCL"   ),  IDS_XTB_CONFIG_XCL,  pApplication, bFirstTime, pOnConnection );
    AddCmd( _T( "XTB_LaunchProgram"  ),  IDS_XTB_LAUNCH,      pApplication, bFirstTime, pOnConnection );
    AddCmd( _T( "XTB_DebugProgram"   ),  IDS_XTB_DEBUG,       pApplication, bFirstTime, pOnConnection );
    AddCmd( _T( "XTB_AttachDebugger" ),  IDS_XTB_ATTACH,      pApplication, bFirstTime, pOnConnection );
    AddCmd( _T( "XTB_TargetManager"  ),  IDS_XTB_TMANAGER,    pApplication, bFirstTime, pOnConnection );

    if( bFirstTime == VARIANT_TRUE )
    {
        // Establish the key bindings we want.

        // RUN
        {
            CComBSTR bszKeystroke( "CTRL+F5" );
            CComBSTR bszCmdName  ( "xTB - Launch Program" );
            CComBSTR bszEditor   ( "Main" );

            VERIFY( SUCCEEDED( pApplication->AddKeyBinding( bszKeystroke, bszCmdName, bszEditor ) ) );
        }

        // DEBUG (GO)
        {
            CComBSTR bszKeystroke( "F5" );
            CComBSTR bszCmdName  ( "xTB - Debug Program" );
            CComBSTR bszEditor   ( "Main" );

            VERIFY( SUCCEEDED( pApplication->AddKeyBinding( bszKeystroke, bszCmdName, bszEditor ) ) );
        }
    }

    *pOnConnection = VARIANT_TRUE;
    return( S_OK );
}

//==============================================================================

HRESULT CDSAddIn::AddCmd( LPCTSTR       szCommandMethod, 
                          int           StringID,
                          IApplication* pApplication, 
                          VARIANT_BOOL  bFirstTime,
                          VARIANT_BOOL* pOnConnection )
{
    // The command name should not be localized to other languages.  The 
    //  tooltip, command description, and other strings related to this
    //  command are stored in the string table (IDS_CMD_STRING) and should
    //  be localized.

    static int      BitmapOffset = 0;
    VARIANT_BOOL    bRet;
    CString         strCmdString;
    CString         strCmdName;

    strCmdString.LoadString( StringID );
    strCmdName = strCmdString.SpanExcluding( "\n" );

    CComBSTR bszCmdString( strCmdString    );
    CComBSTR bszCmdName  ( strCmdName      );
    CComBSTR bszMethod   ( szCommandMethod );

    VERIFY_OK( pApplication->AddCommand( bszCmdString, 
                                         bszMethod, 
                                         BitmapOffset++, 
                                         m_dwCookie, 
                                         &bRet ) );

    if( bRet == VARIANT_FALSE )
    {
        // AddCommand failed because a command with this name already
        //  exists.  You may try adding your command under a different name.
        //  Or, you can fail to load as we will do here.
        *pOnConnection = VARIANT_FALSE;
        return( S_FALSE );
    }

    // Add toolbar buttons only if this is the first time the add-in
    //  is being loaded.  Toolbar buttons are automatically remembered
    //  by Developer Studio from session to session, so we should only
    //  add the toolbar buttons once.
    if( bFirstTime == VARIANT_TRUE )
    {
        VERIFY_OK( pApplication->AddCommandBarButton( dsGlyph|dsText, 
                                                      bszCmdName, 
                                                      m_dwCookie ) );
    }
    
    return( S_OK );
}

//==============================================================================

// This is called on shut-down, and also when the user unloads the add-in
STDMETHODIMP CDSAddIn::OnDisconnection( VARIANT_BOOL bLastTime )
{
    AFX_MANAGE_STATE( AfxGetStaticModuleState() );

    if( bLastTime == VARIANT_TRUE )
    {
        // Remove the key bindings we added.  Switch back to defaults.

        IApplication* pApp = m_pCommands->GetApplicationObject();

        // RUN
        {
            CComBSTR bszKeystroke( "CTRL+F5" );
            CComBSTR bszCmdName  ( "BuildExecute" );
            CComBSTR bszEditor   ( "Main" );

            VERIFY( SUCCEEDED( pApp->AddKeyBinding( bszKeystroke, bszCmdName, bszEditor ) ) );
        }

        // DEBUG (GO)
        {
            CComBSTR bszKeystroke( "F5" );
            CComBSTR bszCmdName  ( "DebugGo" );
            CComBSTR bszEditor   ( "Main" );

            VERIFY( SUCCEEDED( pApp->AddKeyBinding( bszKeystroke, bszCmdName, bszEditor ) ) );
        }
    }

    m_pCommands->UnadviseFromEvents();
    m_pCommands->Release();
    m_pCommands = NULL;

    // TODO: Perform any cleanup work here

    return( S_OK );
}

//==============================================================================
