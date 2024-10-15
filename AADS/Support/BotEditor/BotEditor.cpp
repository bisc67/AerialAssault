
#include "BotEditor.hpp"
#include "AssetEditor.hpp"
#include "PathEditor.hpp"
#include "Entropy.hpp"
#include "resource.h"
#include "Objects/Bot/BotObject.hpp"
#include "Objects/Bot/Graph.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Objects/Player/PlayerObject.hpp"
#include "Objects/Player/DefaultLoadouts.hpp"
#include "Demo1/Globals.hpp"
#include "Support/Building/BuildingOBJ.hpp"
#include <direct.h> 
#include "Objects/Bot/BotLog.hpp"

#define DEBUG_RUN_GRAPH_STRESS_TEST 0

///////////////////////////////////////////////////////////////////////////
// VARIABLES
///////////////////////////////////////////////////////////////////////////

static xbool            s_bInitEditor       = FALSE;
       path_editor      s_PathEditor;
       asset_editor     s_AssetEditor;
       bot_object*      s_pBotObject        = NULL;
       object::id       g_BotObjectID       = ObjMgr.NullID;
static HWND             s_DlgWindow;
static HWND             s_TemplateDlg       = 0; 
static HWND             s_CostAnalyzerDlg   = 0; 
static HWND             s_AssetDlg          = 0; 
static building_obj*    s_pBuilding         = NULL;
static char             s_pBName[256];
static xbool            s_bZBufferOn        = TRUE;
static xbool            s_bShowGraph        = TRUE;
static s32              s_GroundAerialSki   = 0;
static xbool            s_bLoose            = TRUE;
xbool    bUpdated    = FALSE;

extern u32              g_BotUpdateFlags;
extern bot_object::bot_debug g_BotDebug;
extern xbool            g_DrawEdgeLabels;
extern xbool            g_bCostAnalysisOn;
extern xbool            g_RunningPathEditor;
extern graph            g_Graph;
extern xbool            bShowBounds;
extern s32              g_StormSelected;
extern xbool            g_bNeutral;
extern s32              g_SelectedPriority;
extern s32              g_SelectedAssetType;
extern const void*      g_SelectedAsset;
extern xbool            g_AssetsChanged;

extern player_object*   g_pPlayer;

//==============================================================================
//  Function Declarations
//==============================================================================
void ClearDeployables( void );
void InitMissionSystems( void );
xbool LoadMission( void );
void UnloadMission( void );
void SelectAsset( s32 Type );
void SelectPriority ( s32 Priority);
void SelectTeam(s32 Team, xbool Neutral);

///////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////
void SelectTeam ( s32 Team, xbool Neutral) 
{
    g_bNeutral = Neutral;
    g_StormSelected = Team;

    if (g_bNeutral)
    {
        SendMessage( GetDlgItem(s_AssetDlg, IDC_STORM), BM_CLICK, 0,0);
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_STORM, 0);
    }
    else
    if (g_StormSelected)
    {
        SendMessage( GetDlgItem(s_AssetDlg, IDC_INFERNO ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_INFERNO, 0);
    }
    else
    {
        SendMessage( GetDlgItem(s_AssetDlg, IDC_NEUTRAL ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_NEUTRAL, 0);
    }
}
void SelectAsset(s32 Type)
{
    g_SelectedAssetType = Type;
    switch (g_SelectedAssetType)
    {
    case (0):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_TURRET ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_TURRET, 0);
    break;
    case (1):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_SENSOR ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_SENSOR, 0);
    break;
    case (2):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_MINE ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_MINE, 0);
    break;
    case (3):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_SNIPE ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_SNIPE, 0);
    break;
    case (4):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_INVEN ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_INVEN, 0);
    break;
    }
}

void SelectPriority(s32 Priority)
{
    g_SelectedPriority = Priority;
    switch (g_SelectedPriority)
    {
    case (0):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_MEDIUM ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_MEDIUM, 0);
    break;
    case (1):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_LOW ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_LOW, 0);
    break;
    case (2):
        SendMessage( GetDlgItem(s_AssetDlg, IDC_HIGH ), BM_CLICK, 0, 0 );
        SendMessage( s_AssetDlg, WM_COMMAND, IDC_HIGH, 0);
    break;
    }
}
xstring StripName( xstring Path )
{
    for (s32 i = Path.GetLength() - 1; Path[i] != '\\'; i--)
    {
    }

    ASSERT(Path[i] == '\\');
    return &Path[i+1];
}
//=========================================================================

static
xbool GetLoadFileName( char* pFileName )
{
    OPENFILENAME ofn;
    char CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );


    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Path Files(*.pth)\0*.pth\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Open AI Path Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetOpenFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//=========================================================================

static
xbool GetSaveFileName( char* pFileName )
{
    OPENFILENAME    ofn;
    char            CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );

    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Path Files(*.pth)\0*.pth\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Save AI Path Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetSaveFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//=========================================================================

static
xbool GetExportFileName( char* pFileName )
{
    OPENFILENAME    ofn;
    char            CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );

    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Path Files(*.gph)\0*.gph\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Export AI Path Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetSaveFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//==============================================================================

static
xbool GetImportFileName( char* pFileName )
{
    OPENFILENAME    ofn;
    char            CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );

    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Path Files(*.pth)\0*.pth\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Import AI Path Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetSaveFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//=========================================================================

static
xbool GetAssSaveFileName( char* pFileName )
{
    OPENFILENAME    ofn;
    char            CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );

    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Asset Files(*.ass)\0*.ass\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Save Asset Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetSaveFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//==============================================================================

static
xbool GetAssLoadFileName( char* pFileName )
{
    OPENFILENAME ofn;
    char CurrentPath[256];
    xbool           bRet;

    _getcwd( CurrentPath, 256 );


    pFileName[0]=0;

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    ofn.lpstrFilter       = NULL;
    ofn.lpstrFilter       = TEXT("Asset Files(*.ass)\0*.ass\0");
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = pFileName;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrTitle        = TEXT("Open Asset Files...\0");
    ofn.lpstrFileTitle    = NULL;
    ofn.lpstrDefExt       = TEXT("*\0");
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
    ofn.lpstrInitialDir   = FALSE;

    bRet = GetOpenFileName((LPOPENFILENAME)&ofn);
    _chdir( CurrentPath );

    return bRet;
}

//=========================================================================



//=========================================================================
static
LRESULT CALLBACK TemplateDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_INITDIALOG:
        {
            s_pBuilding = NULL;
            // Clear which building is selected 
            break;
        }
    case WM_CLOSE:
        {
            DestroyWindow( hWnd );
            break;
        }
    case WM_DESTROY:
        {
            s_pBuilding   = NULL;
            s_TemplateDlg = 0;
            break;
        }
    case WM_COMMAND: 
        switch(LOWORD(wParam))
        {
        case ID_SAVE_TEMPLATE:
            {
                //
                // Make sure that we have a selected building
                //
                if( s_pBuilding == NULL ) 
                {
                    MessageBox( NULL, "You must select a building",
                                      "Error", MB_OK | MB_ICONINFORMATION );                    
                    break;
                }

                //
                // Save the template for the given builing
                //
                matrix4 W2L = s_pBuilding->GetW2L();
                if( s_PathEditor.SaveTemplate( xfs( "%s.pth", s_pBName ), W2L ) == FALSE )
                {
                    MessageBox( NULL, "Fail to save the building",
                                      "Error", MB_OK | MB_ICONINFORMATION );                    
                }
                else 
                {
                    MessageBox( NULL, "Templaed Saved", "Info", MB_OK );                    
                }

                break;
            }
        case ID_LOAD_TEMPLATE:
            {
                //
                // Make sure that we have a selected building
                //
                if( s_pBuilding == NULL ) 
                {
                    MessageBox( NULL, "You must select a building",
                                      "Error", MB_OK | MB_ICONINFORMATION );                    
                    break;
                }

                //
                // Save the template for the given builing
                //
                matrix4 L2W = s_pBuilding->GetL2W();
                if( s_PathEditor.LoadTemplate( xfs( "%s.pth", s_pBName), L2W ) == FALSE )
                {
                    MessageBox( NULL, "Fail to load the building",
                                      "Error", MB_OK | MB_ICONINFORMATION );                    
                }

                break;
            }

        case ID_LOAD_ALL_TEMPLATES:
            {
                s_pBuilding = NULL;

                ObjMgr.StartTypeLoop( object::TYPE_BUILDING );
                while( (s_pBuilding = (building_obj*)ObjMgr.GetNextInType()) )
                {
                    x_splitpath( s_pBuilding->GetBuildingName(), NULL,NULL,s_pBName,NULL );
                    s_PathEditor.LoadTemplate( xfs( "%s.pth", s_pBName ), s_pBuilding->GetL2W() );
                }
                ObjMgr.EndTypeLoop();

                break;
            }

        case ID_UNSELECT_BUILDING: 
            {
                s_pBuilding = NULL;
                break;
            }
        case ID_SELECT_BUILDING:
            {
                //
                // Throw a array and see where it hits
                //
                vector3         Pos;
                radian          Pitch;
                radian          Yaw;
                const view&     View        = *eng_GetView(0);

                // Get eye position and orientation
                Pos = View.GetPosition();
                View.GetPitchYaw(Pitch,Yaw);

                // Fire ray straight down
                vector3 Dir;

                Dir = View.GetViewZ() * 10000.0f;
                collider            Ray;
                collider::collision Coll;

                Ray.RaySetup( NULL, Pos, Pos+Dir );
                ObjMgr.Collide( object::ATTR_BUILDING, Ray );

                if( Ray.HasCollided() == FALSE ) break;

                Ray.GetCollision( Coll );
                s_pBuilding = (building_obj*)Coll.pObjectHit;

                //
                // Get the file name
                //
                x_splitpath( s_pBuilding->GetBuildingName(), NULL,NULL,s_pBName,NULL );

                break;
            }

        }; // END 2 SWITCH
        break;

        default: return 0;
    };

    SetFocus( d3deng_GetWindowHandle() );
    return 0;
}

void PrintSomething( void )
{
    SetWindowText( GetDlgItem( s_CostAnalyzerDlg, IDC_EDIT2 ), "Hello World" );     
}

//=========================================================================
static
LRESULT CALLBACK CostAnalyzerDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_INITDIALOG:
        {
            break;
        }
    case WM_CLOSE:
        {
            DestroyWindow( hWnd );
            break;
        }
    case WM_DESTROY:
        {
            s_CostAnalyzerDlg = 0;
            break;
        }

    case WM_COMMAND: 
        switch(LOWORD(wParam))
        {        
        
        case ID_FAST_ANALYSIS:
            s_PathEditor.CostAnalysis(path_editor::WKTYPE_LOWEST_NODE);
        break;

        case ID_RAND_ANALYSIS:
            s_PathEditor.CostAnalysis(path_editor::WKTYPE_RAND_NODE);
        break;
    
        case ID_VIEW_DATA:
            s_PathEditor.ViewData((s32)hWnd);
        break;


        }; // END 2 SWITCH
        break;

        default: return 0;
    };

    SetFocus( d3deng_GetWindowHandle() );
    return 0;
}

//=========================================================================
static
LRESULT CALLBACK AssetDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    const view&     View        = *eng_GetView(0);
    switch( uMsg )
    {
    case WM_INITDIALOG:
        {
            SendMessage( GetDlgItem(hWnd, IDC_STORM ), BM_CLICK, 0, 0 );
            SendMessage( GetDlgItem(hWnd, IDC_INVEN), BM_CLICK, 0, 0 );
            SendMessage( GetDlgItem(hWnd, IDC_HIGH), BM_CLICK, 0, 0 );
            break;
        }
    case WM_CLOSE:
        {
            DestroyWindow( hWnd );
            break;
        }
    case WM_DESTROY:
        {
            if( g_AssetsChanged )
            {
                s32 Answer;
                do
                {
                    Answer = MessageBox( NULL, "Do you want to save the current Assets File?",
                                         "Asset Editor Warning", MB_YESNO | MB_ICONINFORMATION );                    

                    // Take care of business
                    if( Answer == IDYES )
                        AssetDlgProc( hWnd, WM_COMMAND, IDC_SAVE, 0 );
                }
                while (g_AssetsChanged && Answer == IDYES);
            }
            s_AssetDlg = 0;
            break;
        }

    case WM_COMMAND: 
        switch(LOWORD(wParam))
        {        
            case IDC_NEUTRAL:   {g_bNeutral = TRUE;g_StormSelected=1;}        break;
            case IDC_INFERNO:   {g_bNeutral = FALSE;g_StormSelected=0;} break;
            case IDC_STORM:     {g_bNeutral = FALSE;g_StormSelected=1;} break;

            case IDC_INVEN:     {g_SelectedAssetType=0;}        break;
            case IDC_TURRET:    {g_SelectedAssetType=1;}        break;
            case IDC_SENSOR:    {g_SelectedAssetType=2;}        break;
            case IDC_MINE:      {g_SelectedAssetType=3;}        break;
            case IDC_SNIPE:     {g_SelectedAssetType=4;}        break;

            case IDC_HIGH:      {g_SelectedPriority=0;}     break;
            case IDC_MEDIUM:    {g_SelectedPriority=1;}     break;
            case IDC_LOW:       {g_SelectedPriority=2;}     break;

            case IDC_PLACE:     
            {
                switch (g_SelectedAssetType)
                {
                case 0:     s_AssetEditor.AddInven();   break;
                case 1:     s_AssetEditor.AddTurret();  break;
                case 2:     s_AssetEditor.AddSensor();  break;
                case 3:     s_AssetEditor.AddMine();    break;
                case 4:     s_AssetEditor.AddSnipePoint();  break;
                }
            }
            break;

            case IDC_SELECT:
            {
                g_SelectedAsset = s_AssetEditor.GetClosestAsset(View.GetPosition() );
            }
            break;

            case IDC_DELETE:
            {
                s_AssetEditor.DeleteSelected();
            }
            break;

            case IDC_LIMITS:
            {    
                SetWindowText( GetDlgItem( hWnd, IDC_LIMITPORT ), s_AssetEditor.ToggleLimits());     
            }
            break;

            case IDC_LIMITS2:
            {    
                SetWindowText( GetDlgItem( hWnd, IDC_LIMITPORT2 ), s_AssetEditor.ToggleSensorLimits());     
            }
            break;

            case IDC_SAVE:
            {
                char FileName[256];
                if( GetAssSaveFileName( FileName ) )
                {
                    s_AssetEditor.Save( FileName );
                }
            }
            break;

            case IDC_LOAD:
            {
                if( g_AssetsChanged )
                {
                    s32 Answer;
                    do
                    {
                        Answer = MessageBox( NULL, "Do you want to save the current Assets File?",
                                             "Asset Editor Warning", MB_YESNO | MB_ICONINFORMATION );                    

                        // Take care of business
                        if( Answer == IDYES )
                            AssetDlgProc( hWnd, WM_COMMAND, IDC_SAVE, 0 );
                    }
                    while (g_AssetsChanged && Answer == IDYES);
                }

                char FileName[256];
                if( GetAssLoadFileName( FileName ) )
                {
                    s_AssetEditor.Load( FileName );
                    ClearDeployables();
                    s_AssetEditor.ReplaceDeployables();
                }
            }
            break;

            case IDC_SNIPE_POINT:
            {
                s_AssetEditor.AddSnipePoint();
            }
            break;

            case IDC_REPORT:
            {
                s_AssetEditor.Report();
            }
            break;

            case IDC_REPORT_ALL:
            {
                if( g_AssetsChanged )
                {
                    s32 Answer;
                    do
                    {
                        Answer = MessageBox( NULL, "Do you want to save the current Assets File?",
                                             "Asset Editor Warning", MB_YESNO | MB_ICONINFORMATION );                    

                        // Take care of business
                        if( Answer == IDYES )
                            AssetDlgProc( hWnd, WM_COMMAND, IDC_SAVE, 0 );
                    }
                    while (g_AssetsChanged && Answer == IDYES);
                }
                if (s_AssetEditor.ReportAll("AssReport.txt"))
                    MessageBox( NULL, "Wrote file AssReport.txt", "Done", MB_OK );
                else
                    MessageBox( NULL, "Could not write AssReport.txt!", "Error", MB_OK );
            }
            break;
        }; // END 2 SWITCH Avalon
        break;

        default: return 0;
    };

    SetFocus( d3deng_GetWindowHandle() );
    return 0;
}

//=========================================================================
static
LRESULT CALLBACK DlgBotProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    const view&     View        = *eng_GetView(0);

    switch( uMsg )
    {
    case WM_DESTROY:
        {
            if( bUpdated )
            {
                s32 Answer = MessageBox( NULL, "Do you want to save before exiting?",
                                         "Bot Editor Warning", MB_YESNO | MB_ICONINFORMATION );                    

                // Take care of business
                if( Answer == IDYES )
                    DlgBotProc( hWnd, WM_COMMAND, ID_SAVE, 0 );
            }
            break;
        }
    case WM_INITDIALOG:
        {
//            g_RunningPathEditor = TRUE;
            SendMessage( GetDlgItem(hWnd, ID_NODE_GROUND ), BM_CLICK, 0, 0 );
            s_GroundAerialSki = 0;
            SendMessage( GetDlgItem(hWnd, ID_EDGE_LOOSE  ), BM_CLICK, 0, 0 );
            s_bLoose = TRUE;
            SetWindowText( GetDlgItem( hWnd, IDC_BBOXSIZE ), "5");     
            bUpdated = FALSE;

            break;
        }
    case WM_COMMAND: 

        switch(LOWORD(wParam))
        {
        case IDC_EDGE_LABELS:
            {
                if( BST_CHECKED & SendMessage( GetDlgItem(hWnd, IDC_EDGE_LABELS), BM_GETSTATE, 0, 0 ) )
                {                    
                    g_DrawEdgeLabels = TRUE;
                }
                else
                {
                    g_DrawEdgeLabels = FALSE;
                }
                break;
            }

        case IDC_BOTMARKERS:
            {
                if( BST_CHECKED & SendMessage( GetDlgItem(hWnd, IDC_BOTMARKERS ), BM_GETSTATE, 0, 0 ) )
                {                    
                    g_BotDebug.DrawSteeringMarkers = TRUE;
                }
                else
                {
                    g_BotDebug.DrawSteeringMarkers = FALSE;
                }
                break;
            }

        case IDC_ZBUFFER:
            {
                if( BST_CHECKED & SendMessage( GetDlgItem(hWnd, IDC_ZBUFFER ), BM_GETSTATE, 0, 0 ) )
                {                    
                    s_bZBufferOn = FALSE;
                }
                else
                {
                    s_bZBufferOn = TRUE;
                }
                break;
            }

        case IDC_SHOWGRAPH:
            {
                if( BST_CHECKED & SendMessage( GetDlgItem(hWnd, IDC_SHOWGRAPH ), BM_GETSTATE, 0, 0 ) )
                {                    
                    s_bShowGraph = FALSE;
                }
                else
                {
                    s_bShowGraph = TRUE;
                }

                break;
            }
        case IDC_SHOWBOUNDS:
            {
                if( BST_CHECKED & SendMessage( GetDlgItem(hWnd, IDC_SHOWBOUNDS ), BM_GETSTATE, 0, 0 ) )
                {                    
                    bShowBounds = TRUE;
                }
                else
                {
                    bShowBounds = FALSE;
                }

                break;
            }

        case ID_BOT_ACTIVATE:
            {
                if (s_PathEditor.m_bChanged)
                {
                    Path_Manager_ForceLoad();
                    s_PathEditor.Export("temp.gph" );
                    s_pBotObject->LoadGraph("temp.gph");
                }
#if SHOW_EDGE_COSTS
                g_Graph.ResetEdgeCosts();
#endif
                vector3 Src, Dst;
                s_PathEditor.GetSelectedSpawnPos( Src );
                s_PathEditor.GetSelectedPos( Dst );

                if ( (Dst - vector3( 0,0,0 )).LengthSquared() < 0.000001f )
                {
                    // use the human player's position
                    s_PathEditor.GetPlayerPos( Dst );
                }

                if (Src != Dst)
                    s_pBotObject->RunEditorPath( Src, Dst );

                break;
            }
        case ID_UNSELECT_EDGE:  s_PathEditor.UnselectedActiveEdge();                            break;
        case ID_INSERT_NODE:    bUpdated = TRUE;s_PathEditor.InsertNodeInActiveEdge( View.GetPosition() );      break;      
        case ID_START_BOT:      s_PathEditor.SelectSpawnNode    ( View.GetPosition() );         break;
        case ID_CAM_ADD_NODE:   bUpdated = TRUE;s_PathEditor.AddNode            ( View.GetPosition() );         break;
        case ID_SELECT_EDGE:    s_PathEditor.SelectEdge         ( View.GetPosition() );       
            if (s_CostAnalyzerDlg)                  s_PathEditor.ViewData((s32)s_CostAnalyzerDlg);   break;
        case ID_MOVE_NODE:      bUpdated = TRUE;s_PathEditor.MoveSelectedNode   ( View.GetPosition() );         break;
        case ID_SELECT_NODE:    s_PathEditor.SelectNode         ( View.GetPosition() );         break;
        case ID_UNSELECT_NODE:  s_PathEditor.UnSelectActiveNode ();                             break;
        case ID_CHANGE_EDGE:    bUpdated = TRUE;s_PathEditor.ChangeActiveEdge   ();             break;
        case ID_DELETE_EDGE:    bUpdated = TRUE;s_PathEditor.DeleteActiveEdge   ();             break;
        case ID_DELETE_NODE:    bUpdated = TRUE;s_PathEditor.RemoveSelectedNode ();             break;
        case ID_DROP_NODE:      bUpdated = TRUE;s_PathEditor.DropNode           ();             break;
        case ID_NODE_AERIAL:    {s_GroundAerialSki=1;s_PathEditor.SetAerial     ();}            break;
        case ID_NODE_GROUND:    {s_GroundAerialSki=0;s_PathEditor.SetGround     ();}            break;
        case ID_NODE_SKI:       {s_GroundAerialSki=2;s_PathEditor.SetSki        ();}            break;
        case ID_EDGE_LOOSE:     {s_bLoose=TRUE;s_PathEditor.SetLoose            ();}            break;
        case ID_EDGE_TIGHT:     {s_bLoose=FALSE;s_PathEditor.SetTight           ();}            break;
        case ID_GRID:           
        {
            if (!IsDlgButtonChecked(hWnd, ID_TEST_ALL))
            {
                if (s_PathEditor.m_bChanged || g_Graph.m_nNodes <= 0)
                {
                    s_PathEditor.Refresh_g_GRAPH();
                }
                s_PathEditor.RecomputeGrid();
            }          
            else
            {
                xarray<xstring> BaseFileNames;
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Abominable\\Abominable"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Avalon\\Avalon"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\BeggarsRun\\BeggarsRun"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Damnation\\Damnation"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\DeathBirdsFly\\DeathBirdsFly"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Desiccator\\Desiccator"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Equinox\\Equinox"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Escalade\\Escalade"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Firestorm\\Firestorm"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Flashpoint\\Flashpoint"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Gehenna\\Gehenna"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Insalubria\\Insalubria"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Invictus\\Invictus"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\JacobsLadder\\JacobsLadder"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Katabatic\\Katabatic"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission01\\Mission01"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission02\\Mission02"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission03\\Mission03"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission04\\Mission04"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission05\\Mission05"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission06\\Mission06"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission07\\Mission07"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission08\\Mission08"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission09\\Mission09"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission10\\Mission10"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission11\\Mission11"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission12\\Mission12"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission13\\Mission13"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Myrkwood\\Myrkwood"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Oasis\\Oasis"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Overreach\\Overreach"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Paranoia\\Paranoia"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Pyroclasm\\Pyroclasm"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Quagmire\\Quagmire"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Rasp\\Rasp"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Recalescence\\Recalescence"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Reversion\\Reversion"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Rimehold\\Rimehold"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Sanctuary\\Sanctuary"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Sirocco\\Sirocco"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Slapdash\\Slapdash"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\SunDried\\SunDried"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\ThinIce\\ThinIce"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Tombstone\\Tombstone"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Underhill\\Underhill"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Whiteout\\Whiteout"));

                s32 i;

                for ( i = 0; i < BaseFileNames.GetCount(); ++i )
                {
                    xstring PthFileName = BaseFileNames[i] + ".pth";
                    xstring Map = StripName(BaseFileNames[i]);

                    s_PathEditor.Load( PthFileName );

                    x_DebugMsg("Testing %s... ", Map);
                    s_PathEditor.Refresh_g_GRAPH();
                    s_PathEditor.RecomputeGrid();

                    x_DebugMsg("done.  Writing stats to botlog.txt.\n");
                    bot_log::Log( "Map: %s, %dx%d", Map, 
                        s_PathEditor.m_Grid.nCellsWide, 
                        s_PathEditor.m_Grid.nCellsDeep );
                    bot_log::Log( "\nCell Size: %2.1f x %2.1f \tEdge List Count: %d \tMax edges per cell: %d \n",  
                        s_PathEditor.m_Grid.CellWidth, s_PathEditor.m_Grid.CellDepth, 
                        s_PathEditor.m_Grid.nGridEdges, s_PathEditor.m_nMaxEdgesPerCell);
                }
            }
        }        
        break;
        case IDC_RESET:         
        {
#ifdef TARGET_PC
            xbool bSuccess;
            s32 Value = GetDlgItemInt(hWnd, IDC_BBOXSIZE, &bSuccess, TRUE );
            if (bSuccess)
                s_PathEditor.BlowEdges(Value);     
            else
                s_PathEditor.BlowEdges(5);
#endif
}            break;

        case ID_PLR_ADD_NODE:
        {
            bUpdated = TRUE;
            vector3         Pos;
            player_object*  pPlayer;

            ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
            pPlayer = (player_object*)ObjMgr.GetNextInType();
            ObjMgr.EndTypeLoop();
            Pos = pPlayer->GetPos();
            s_PathEditor.AddNode( Pos + vector3(0,2,0) );

            break;
        }
            
        case ID_LOAD:
        {
            if( bUpdated )
            {
                s32 Answer;
                do
                {
                    Answer = MessageBox( NULL, "Do you want to save before closing this map?",
                                         "Bot Editor Warning - Map Changed", MB_YESNO | MB_ICONINFORMATION );                    


                    // Take care of business
                    if( Answer == IDYES )
                        DlgBotProc( hWnd, WM_COMMAND, ID_SAVE, 0 );
                }
                while (bUpdated && Answer == IDYES);
            }
            char FileName[256];
            if( GetLoadFileName( FileName ) )
            {
                bUpdated = FALSE;
                s_PathEditor.Load( FileName );
            }
            break;
        }
        case ID_SAVE:
        {
            char FileName[256];
            if( GetSaveFileName( FileName ) )
            {
                s_PathEditor.Save( FileName );
                bUpdated = FALSE;
            }
            break;
        }
        case ID_EXPORT:
        {
            char FileName[256];

            // This doesn't work.
            if (!IsDlgButtonChecked(hWnd, ID_TEST_ALL))
            {
                if( GetExportFileName( FileName ) )
                {
                    s_PathEditor.Export( FileName );
                    if ( !g_Graph.DebugConnected() )
                    {
                        s32 i;
                        for (i = 0; i < s_PathEditor.m_NodeList.GetCount(); i++)
                        {
                            if (g_Graph.m_pNodeList[i].PlayerInfo[0].GetVisited() )
                            {
                                s_PathEditor.m_NodeList[i].Type = path_editor::NDTYPE_UNCONNECTED;
                            }
                            else
                                if (s_PathEditor.m_NodeList[i].Type != path_editor::NDTYPE_QUESTIONABLE
                                    && s_PathEditor.m_NodeList[i].Type != path_editor::NDTYPE_ADJUSTED)
                                    s_PathEditor.m_NodeList[i].Type = path_editor::NDTYPE_NULL;
                        }
                        MessageBox( NULL, "Graph is not connected\nCheck projects\\T2\\Demo1\\BotLog.txt for node positions",
                            "Error", MB_OK | MB_ICONINFORMATION );
                    }
                    else
                    {
                        s32 i;
                        for (i = 0; i < s_PathEditor.m_NodeList.GetCount(); i++)
                        {
                            if (s_PathEditor.m_NodeList[i].Type != path_editor::NDTYPE_QUESTIONABLE
                                && s_PathEditor.m_NodeList[i].Type != path_editor::NDTYPE_ADJUSTED)
                                s_PathEditor.m_NodeList[i].Type = path_editor::NDTYPE_NULL;
                        }
                    }
                }
            }
            else
            {
                // This doesn't work
                break;
                    // Unload
                    UnloadMission();

    static const char* List[] = 
                {
                    "Abominable"   , 
                    "Avalon"       , 
                    "BeggarsRun"   , 
                    "Damnation"    , 
                    "DeathBirdsFly", 
                    "Desiccator"   , 
                    "Equinox"      , 
                    "Escalade"     , 
                    "Firestorm"    , 
                    "Flashpoint"   , 
                    "Gehenna"      , 
                    "Insalubria"   , 
                    "Invictus"     , 
                    "JacobsLadder" , 
                    "Katabatic"    , 
                    "Mission01"    , 
                    "Mission02"    , 
                    "Mission03"    , 
                    "Mission04"    , 
                    "Mission05"    , 
                    "Mission06"    , 
                    "Mission07"    , 
                    "Mission08"    , 
                    "Mission09"    , 
                    "Mission10"    , 
                    "Mission11"    , 
                    "Mission12"    , 
                    "Mission13"    , 
                    "Myrkwood"     , 
                    "Oasis"        , 
                    "Overreach"    , 
                    "Paranoia"     , 
                    "Pyroclasm"    , 
                    "Quagmire"     , 
                    "Rasp"         , 
                    "Recalescence" , 
                    "Reversion"    , 
                    "Rimehold"     , 
                    "Sanctuary"    , 
                    "Sirocco"      , 
                    "Slapdash"     , 
                    "SunDried"     , 
                    "ThinIce"      , 
                    "Tombstone"    , 
                    "Underhill"    , 
                    "Whiteout"     , 

                    "FINISHED",
                };
                // Loop through missions
                s32 i=0;
                while( x_stricmp(List[i],"FINISHED") != 0 )
                {
                    // Init mission systems
                    InitMissionSystems();

                    // Setup directory
                    x_strcpy( tgl.MissionName, List[i] );
                    x_strcpy( tgl.MissionDir, xfs("Data/Missions/%s",tgl.MissionName) );
                    tgl.MissionPrefix[0] = '\0';

                    // Load mission in
                    while( !LoadMission() );

                    xfs PthFileName("Data/Missions/%s/%s", List[i], ".pth");
                    xfs GphFileName("Data/Missions/%s/%s", List[i], ".gph");

                    s_PathEditor.Load( PthFileName );
                    x_DebugMsg("Exporting %s... ", List[i]);
                    s_PathEditor.Export( GphFileName );
                    x_DebugMsg("done.  Testing continuity...");
                    if ( !g_Graph.DebugConnected() )
                    {
                        MessageBox( NULL, "Graph is not connected\nCheck projects\\T2\\Demo1\\BotLog.txt for node positions",
                            "Error", MB_OK | MB_ICONINFORMATION );
                    }
                    else
                        x_DebugMsg("OK\n");

                    // load graph
    //                g_Graph.Load( xfs( "%s/%s.gph", tgl.MissionDir, tgl.MissionName),FALSE );



                    // Unload
                    UnloadMission();

                    i++;
                }

                    // Init mission systems
                    InitMissionSystems();

                    // Setup directory
                    x_strcpy( tgl.MissionName, List[i] );
                    x_strcpy( tgl.MissionDir, xfs("Data/Missions/%s",tgl.MissionName) );
                    tgl.MissionPrefix[0] = '\0';

                    // Load mission in
                    while( !LoadMission() );

    /*
                xarray<xstring> BaseFileNames;
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Abominable\\Abominable"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Avalon\\Avalon"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\BeggarsRun\\BeggarsRun"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Damnation\\Damnation"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\DeathBirdsFly\\DeathBirdsFly"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Desiccator\\Desiccator"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Equinox\\Equinox"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Escalade\\Escalade"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Firestorm\\Firestorm"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Flashpoint\\Flashpoint"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Gehenna\\Gehenna"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Insalubria\\Insalubria"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Invictus\\Invictus"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\JacobsLadder\\JacobsLadder"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Katabatic\\Katabatic"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission01\\Mission01"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission02\\Mission02"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission03\\Mission03"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission04\\Mission04"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission05\\Mission05"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission06\\Mission06"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission07\\Mission07"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission08\\Mission08"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission09\\Mission09"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission10\\Mission10"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission11\\Mission11"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission12\\Mission12"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Mission13\\Mission13"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Myrkwood\\Myrkwood"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Oasis\\Oasis"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Overreach\\Overreach"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Paranoia\\Paranoia"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Pyroclasm\\Pyroclasm"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Quagmire\\Quagmire"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Rasp\\Rasp"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Recalescence\\Recalescence"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Reversion\\Reversion"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Rimehold\\Rimehold"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Sanctuary\\Sanctuary"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Sirocco\\Sirocco"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Slapdash\\Slapdash"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\SunDried\\SunDried"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\ThinIce\\ThinIce"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Tombstone\\Tombstone"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Underhill\\Underhill"));
                BaseFileNames.Append(
                    xstring("c:\\projects\\t2\\demo1\\data\\missions\\Whiteout\\Whiteout"));

                s32 i;

                for ( i = 0; i < BaseFileNames.GetCount(); ++i )
                {
                    xstring PthFileName = BaseFileNames[i] + ".pth";
                    xstring GphFileName = BaseFileNames[i] + ".gph";
                    s_PathEditor.Load( PthFileName );
                    x_DebugMsg("Exporting %s... ", GphFileName);
                    s_PathEditor.Export( GphFileName, s_pBotObject );
                    x_DebugMsg("done.  Testing continuity...");
                    if ( !g_Graph.DebugConnected() )
                    {
                        MessageBox( NULL, "Graph is not connected\nCheck projects\\T2\\Demo1\\BotLog.txt for node positions",
                            "Error", MB_OK | MB_ICONINFORMATION );
                    }
                    else
                        x_DebugMsg("OK\n");
                }
            }
                            */

            }
#if DEBUG_RUN_GRAPH_STRESS_TEST
            ASSERT( s_pBotObject );
            s_PathEditor.RunStressTest();
#endif

            break;
        }

        case ID_IMPORT:
        {
            s32 Answer = MessageBox( NULL, "Do you want to save before clearing?",
                                     "Bot Editor Warning", MB_YESNO | MB_ICONINFORMATION );                    

            // Take care of business
            if( Answer == IDYES )
                DlgBotProc( hWnd, WM_COMMAND, ID_SAVE, 0 );

            char FileName[256];
            if( GetImportFileName( FileName ) )
            {
                s_PathEditor.Import( FileName );
            }
#if DEBUG_RUN_GRAPH_STRESS_TEST
            ASSERT( s_pBotObject );
            s_PathEditor.RunStressTest();
#endif
            if ( !g_Graph.DebugConnected() )
            {
                MessageBox( NULL, "Graph is not connected\nCheck projects\\T2\\Demo1\\BotLog.txt for node positions",
                    "Error", MB_OK | MB_ICONINFORMATION );
            }

            break;
        }

        case ID_TEMPLATES:
        {
            if( s_TemplateDlg == 0 )
            {
                s_TemplateDlg = CreateDialog( d3deng_GetInstace(), MAKEINTRESOURCE(IDD_TEMPLATE_DIALOG), 
                                            d3deng_GetWindowHandle(), (DLGPROC)TemplateDlgProc );
            }
            else
            {
                DestroyWindow( s_TemplateDlg );
            }
            
            break;
        }
        case ID_COST_ANALYZER:
        {
            if( s_CostAnalyzerDlg == 0 )
            {
                s_CostAnalyzerDlg = CreateDialog( d3deng_GetInstace(), MAKEINTRESOURCE(IDD_COST_ANALYZER_DIALOG), 
                                            d3deng_GetWindowHandle(), (DLGPROC)CostAnalyzerDlgProc );
            }
            else
            {
                DestroyWindow( s_CostAnalyzerDlg );
            }
            
            break;
        }
        case ID_ASSET_DIALOG:
        {
            if( s_AssetDlg== 0 )
            {
                s_AssetDlg = CreateDialog( d3deng_GetInstace(), MAKEINTRESOURCE(IDD_ASSET_DIALOG), 
                                            d3deng_GetWindowHandle(), (DLGPROC)AssetDlgProc );
            }
            else
            {
                DestroyWindow( s_AssetDlg );
            }
            
            break;
        }

        case ID_MOVE_PLAYER:
        {
            vector3 Pos;
            radian  Pitch;
            radian  Yaw;

            const view&     View        = *eng_GetView(0);

            // Get eye position and orientation
            Pos = View.GetPosition();
            View.GetPitchYaw(Pitch,Yaw);

            // Fire ray straight down
            vector3 Dir;

            Dir = View.GetViewZ() * 10000.0f;
            collider  Ray;
            Ray.RaySetup( NULL, Pos, Pos+Dir );
            ObjMgr.Collide( object::ATTR_SOLID_STATIC, Ray );
            collider::collision Coll;
            vector3 Pos2 = Pos + Ray.GetCollisionT()*Dir;

            player_object *pPlayer ;
            ObjMgr.StartTypeLoop( object::TYPE_PLAYER );
            while( (pPlayer = (player_object*)ObjMgr.GetNextInType()) )
            {
                pPlayer->SetPos( Pos2 );
            }
            ObjMgr.EndTypeLoop();

            break;
        }

        } // END OF 2 SWITCH
        break;        

        default: return 0;
    };

    SetFocus( d3deng_GetWindowHandle() );
    return 0;
}

//=========================================================================
static
void Initialize( void )
{
//    g_BotEditorActive = TRUE;

    s_DlgWindow = CreateDialog( d3deng_GetInstace(), MAKEINTRESOURCE(IDD_BOTEDIT_DIALOG), 
                                d3deng_GetWindowHandle(), (DLGPROC)DlgBotProc );

    if( s_DlgWindow == NULL )
    {
        LPVOID lpMsgBuf;
        FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL 
        );

        MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
        LocalFree( lpMsgBuf );
    }
    else
    {
        RECT Rect, DlgRect; 
        GetWindowRect( GetDesktopWindow(), &Rect );
        GetWindowRect( s_DlgWindow,        &DlgRect );

        DlgRect.left = DlgRect.right - DlgRect.left;
        DlgRect.top  = DlgRect.bottom - DlgRect.top;

        MoveWindow( s_DlgWindow, 
                    Rect.right  - DlgRect.left,
                    Rect.bottom - DlgRect.top,
                    DlgRect.left,
                    DlgRect.top,
                    TRUE );

        SetFocus( d3deng_GetWindowHandle() );
    }

    //
    // Get a pointer to our wanderfull bot
    //
   
    if( 1 )
    {
        s_pBotObject = (bot_object*)ObjMgr.CreateObject( object::TYPE_BOT );
        s_pBotObject->Initialize( vector3(0,100,0),                             // Pos
                                  eng_GetTVRefreshRate(),                       // TV refresh in Hz
                                  player_object::CHARACTER_TYPE_FEMALE,         // Character type
                                  player_object::SKIN_TYPE_BEAGLE,              // Skin type
                                  player_object::VOICE_TYPE_FEMALE_HEROINE,     // Voice type
                                  default_loadouts::STANDARD_BOT_HEAVY,         // Default loadout
                                  NULL,                         // Name
                                  TRUE );                                       // Created from editor or not

        s_pBotObject->SetTeamBits(2);

        ObjMgr.AddObject( s_pBotObject, obj_mgr::AVAILABLE_SERVER_SLOT );
        s_pBotObject->Respawn() ;
        s_pBotObject->Player_SetupState( player_object::PLAYER_STATE_IDLE );

        g_BotObjectID = s_pBotObject->GetObjectID();
    }
}

//=========================================================================

void BOTEDIT_ProcessLoop( void )
{    
    //
    // Initialize the bot editor
    //
    if( s_bInitEditor == FALSE )
    {
        if( input_WasPressed( INPUT_KBD_PAUSE, 0 ) )
        {
            Initialize();
            s_bInitEditor = TRUE;
            ObjMgr.StartTypeLoop(object::TYPE_PLAYER);
            g_pPlayer = (player_object*)ObjMgr.GetNextInType();
            ObjMgr.EndTypeLoop();
        }
        return;
    }

    //
    // See whether we should add a new box
    //
    if( input_WasPressed( INPUT_KBD_NUMPAD0, 0 ) || input_WasPressed( INPUT_KBD_A, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_CAM_ADD_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD4, 0 ) || input_WasPressed( INPUT_KBD_R, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_SELECT_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD1, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_SELECT_EDGE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD2, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_DELETE_EDGE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD3, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_CHANGE_EDGE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD5, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_UNSELECT_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD6, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_DELETE_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD7, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_MOVE_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD8, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_INSERT_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_NUMPAD9, 0 ) )
    {
        s_pBotObject->ToggleArmor();
    }
    
    if( input_WasPressed( INPUT_KBD_DECIMAL, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_DROP_NODE, 0 );
    }

    if( input_WasPressed( INPUT_KBD_SUBTRACT, 0 ) )
    {
        SendMessage( s_DlgWindow, WM_COMMAND, ID_LOAD, 0 );
    }

    if( input_WasPressed( INPUT_KBD_MULTIPLY, 0 ) )
    {

        switch (s_GroundAerialSki)
        {
            case (0):
            SendMessage( GetDlgItem(s_DlgWindow, ID_NODE_AERIAL ), BM_CLICK, 0, 0 );
            SendMessage( s_DlgWindow, WM_COMMAND, ID_NODE_AERIAL, 0 );
            break;
            case (1):
            SendMessage( GetDlgItem(s_DlgWindow, ID_NODE_SKI ), BM_CLICK, 0, 0 );
            SendMessage( s_DlgWindow, WM_COMMAND, ID_NODE_SKI, 0 );
            break;
            case (2):
            SendMessage( GetDlgItem(s_DlgWindow, ID_NODE_GROUND ), BM_CLICK, 0, 0 );
            SendMessage( s_DlgWindow, WM_COMMAND, ID_NODE_GROUND, 0 );
            break;
            default:
            ASSERT(0);
            break;
        }
    }

    if( input_WasPressed( INPUT_KBD_DIVIDE, 0 ) )
    {
        if (s_bLoose)
        {
            SendMessage( GetDlgItem(s_DlgWindow, ID_EDGE_TIGHT  ), BM_CLICK, 0, 0 );
            SendMessage( s_DlgWindow, WM_COMMAND, ID_EDGE_TIGHT, 0 );
        }
        else
        {
            SendMessage( GetDlgItem(s_DlgWindow, ID_EDGE_LOOSE  ), BM_CLICK, 0, 0 );
            SendMessage( s_DlgWindow, WM_COMMAND, ID_EDGE_LOOSE, 0 );
        }
    }

    if ( input_WasPressed( INPUT_KBD_Y, 0) )
    {
        if (g_bNeutral)
        {
            SendMessage( GetDlgItem(s_AssetDlg, IDC_STORM), BM_CLICK, 0,0);
            SendMessage( s_AssetDlg, WM_COMMAND, IDC_STORM, 0);
        }
        else
        if (g_StormSelected)
        {
            SendMessage( GetDlgItem(s_AssetDlg, IDC_INFERNO ), BM_CLICK, 0, 0 );
            SendMessage( s_AssetDlg, WM_COMMAND, IDC_INFERNO, 0);
        }
        else
        {
            SendMessage( GetDlgItem(s_AssetDlg, IDC_NEUTRAL ), BM_CLICK, 0, 0 );
            SendMessage( s_AssetDlg, WM_COMMAND, IDC_NEUTRAL, 0);
        }
    }
    if ( input_WasPressed( INPUT_KBD_W, 0) )
    {
        SelectAsset(g_SelectedAssetType);
//        switch (g_SelectedAssetType)
//        {
//        case (0):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_TURRET ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_TURRET, 0);
//        break;
//        case (1):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_SENSOR ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_SENSOR, 0);
//        break;
//        case (2):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_MINE ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_MINE, 0);
//        break;
//        case (3):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_SNIPE ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_SNIPE, 0);
//        break;
//        case (4):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_INVEN ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_INVEN, 0);
//        break;
//        }
    }

    if ( input_WasPressed( INPUT_KBD_B, 0) )
    {
        SelectPriority(g_SelectedPriority);
//        switch (g_SelectedPriority)
//        {
//        case (0):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_MEDIUM ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_MEDIUM, 0);
//        break;
//        case (1):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_LOW ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_LOW, 0);
//        break;
//        case (2):
//            SendMessage( GetDlgItem(s_AssetDlg, IDC_HIGH ), BM_CLICK, 0, 0 );
//            SendMessage( s_AssetDlg, WM_COMMAND, IDC_HIGH, 0);
//        break;
//        }
    }

    if ( input_WasPressed( INPUT_KBD_BACKSLASH, 0) )
    {
        if ( input_IsPressed( INPUT_KBD_LSHIFT )
            || input_IsPressed( INPUT_KBD_RSHIFT ) )
        {
            s_AssetEditor.Deselect();
        }
        else
            SendMessage(s_AssetDlg, WM_COMMAND, IDC_SELECT, 0);
    }

    if (input_WasPressed( INPUT_KBD_BACK, 0) || input_WasPressed(INPUT_KBD_DELETE, 0))
    {
        SendMessage(s_AssetDlg, WM_COMMAND, IDC_DELETE, 0);
    }

    if (input_WasPressed(INPUT_KBD_INSERT, 0))
    {        
        SendMessage(s_AssetDlg, WM_COMMAND, IDC_LIMITS, 0);
    }

    if (input_WasPressed(INPUT_KBD_HOME, 0))
    {        
        SendMessage(s_AssetDlg, WM_COMMAND, IDC_LIMITS2, 0);
    }

    static xbool HeldDown = FALSE;
    if ( input_IsPressed( INPUT_MOUSE_BTN_L, 0))
    {
        if (!HeldDown)
        {
            HeldDown = TRUE;
            SendMessage( s_AssetDlg, WM_COMMAND, IDC_PLACE, 0);
        }
    }
    else
    {
        HeldDown = FALSE;
    }
    if ( input_WasPressed( INPUT_MOUSE_BTN_R, 0))
    {
        g_SelectedAsset = s_AssetEditor.GetNextBadAsset();
    }

    if (input_WasPressed( INPUT_KBD_DOWN, 0))
    {
        s_PathEditor.LowerNode();
    }

#if EDGE_COST_ANALYSIS
    if (g_bCostAnalysisOn)
    {
        // Check if any edge costs need updating.
        if (g_BotUpdateFlags)
        {
            // Determine which bots have new edge cost info.
            s32 i = 0;

            while (g_BotUpdateFlags)
            {
                if (g_BotUpdateFlags & 1)
                {
                    // Update the edge cost from bot[i].
                     s_PathEditor.UpdateCost(i);
                }
                // Check for remaining updates.
                i++;
                g_BotUpdateFlags >>= 1;
            }
            ASSERT (!g_BotUpdateFlags);
        }
        else 
        {
            s_PathEditor.RenewDestination();
        }
    }               
#endif
    //
    // Render the curent paths
    //
    if( s_bShowGraph )
    {

        s_PathEditor.Render( s_bZBufferOn );
    }

    if( s_pBuilding )
    {
        eng_Begin();
        draw_BBox( s_pBuilding->GetBBox(), xcolor( 255,0,0) );
        eng_End();
    }

    if (s_AssetDlg)
    {
        s_AssetEditor.Render();
    }

}

//==============================================================================
