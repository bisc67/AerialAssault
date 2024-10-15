
#include "BldManager.hpp"

///////////////////////////////////////////////////////////////////////////
// ADDITIONAL INCLUDES
///////////////////////////////////////////////////////////////////////////

#include "AUX_Bitmap.hpp"
#include "building.hpp"
#include "BldInstance.hpp"
#include "BldRender.hpp"
#include "fog\fog.hpp"

///////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////
extern volatile f32     PS2BUILDING_GLOBALK;
bld_manager             g_BldManager;
vector3                 g_LightPos;     // position of global light source in world
vector3                 g_LightDir;     // direction of global light source in world

xbool PRINT_BUILDING_STATS = FALSE;

bld_debug g_BldDebug =
{
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE,
};

///////////////////////////////////////////////////////////////////////////
// STATICS VARIABLES
///////////////////////////////////////////////////////////////////////////

static xbool            s_Init          = FALSE;

#ifdef BLD_TIMERS
bld_manager::timers     bld_manager::st;
#endif

//s32 AndyLMCount=0;

///////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////

//=========================================================================

void bld_manager::SetLightPos( const vector3& Pos )
{
    g_LightPos = Pos;
}

void bld_manager::SetLightDir( const vector3& Dir )
{
    g_LightDir = Dir;
    g_LightDir.Normalize();
}

void bld_manager::SetMissionDir( const char* Path )
{
    ASSERT( x_strlen( Path ) < 64 );
    x_strcpy( m_MissionDir, Path );
}

//=========================================================================

static 
void LoadTexture( xbitmap& BMP, const char* pFileName, xbool BuildMips )
{
    char FName[64];
    char Ext[16];
    char BitmapFileName[256];
    char Drive[64];
    char Dir[256];

    x_splitpath( pFileName, Drive, Dir, FName, Ext );
    x_sprintf  ( BitmapFileName, "%s%s%s.xbm", Drive, Dir, FName );
    //x_DebugMsg("XBM: %1s\n", BitmapFileName );

#ifdef TARGET_PC

    x_sprintf( BitmapFileName, "%s%s%s.tga", Drive, Dir, FName );
    if( !auxbmp_LoadD3D( BMP, BitmapFileName ) )
    {
        x_sprintf( BitmapFileName, "%s%s%s.bmp", Drive, Dir, FName );
        if( !auxbmp_LoadD3D( BMP, BitmapFileName ) )
        {
            auxbmp_SetupDefault( BMP );
            BMP.ConvertFormat( xbitmap::FMT_32_ARGB_8888 );
            //x_DebugMsg("**** FAILED TO LOAD ****\n");
        }
    }

#else
    
	if( !BMP.Load(BitmapFileName) )
    {
        x_sprintf( BitmapFileName, "%s%s%s.bmp", Drive, Dir, FName );

        if( !auxbmp_LoadPS2( BMP, BitmapFileName ) )
        {
            //auxbmp_SetupDefault( BMP );
            //x_DebugMsg("**** FAILED TO LOAD ****\n");
        }

        if( BuildMips )
        {
            BMP.BuildMips();
        }

        x_splitpath( pFileName, Drive, Dir, FName, Ext );
        x_sprintf  ( BitmapFileName, "%s%s%s.xbm", Drive, Dir, FName );
        BMP.Save( BitmapFileName );
        if( !BuildMips )
            BMP.SaveTGA( xfs("%s.tga",BitmapFileName) );
    }

#endif

}

//=========================================================================
static
void CreateLMBitmap( xbitmap& BMP, s32* pClutHandle )
{
    //BMP.SaveTGA(xfs("_AndyLM%03d.tga",AndyLMCount));
    //BMP.SavePaletteTGA(xfs("_AndyLMPAL%03d.tga",AndyLMCount));

#ifdef TARGET_PC

    vram_Register( BMP );
    pClutHandle[0] = pClutHandle[1] = pClutHandle[2] = NULL;

#else
    //
    // Allocate all the memory
    //
#if !ONEPASS
    
	u32* pClut = new u32[ (256*4)*3 ];
    ASSERT( pClut );
	
	//
    // Create the 3 palletes
    //
    s32 j;
    for( j=0; j<256; j++ )
    {
        xcolor& Color     = ((xcolor*)BMP.GetClutData())[j];
        const s32 MinIntensity = 60;

        if( Color.R < MinIntensity && Color.G < MinIntensity && Color.B < MinIntensity )
        {
            Color.R += MinIntensity;
            Color.G += MinIntensity;
            Color.B += MinIntensity;
        }
    }

    for( j=0; j<3; j++ )
    {
        u8* pColor = (u8*)&pClut[256*j];

        x_memcpy( pColor, BMP.GetClutData(), 256*4 );

		for( int k=0; k<256; k++ )
        {
            s32 A = pColor[k*4+j];
            A = A >> 1;
            //pColor[k*4+0] = (byte)A;//0;
            //pColor[k*4+1] = (byte)A;//0;
            //pColor[k*4+2] = (byte)A;//0;
            pColor[k*4+3] = (byte)A;
        }
        //
        // Register with the vram manager
        //
        if( j > 0 )
        {
            pClutHandle[j] = vram_RegisterClut( pColor, 256 );
        }
    }

    //
    // Set the new clut
    //
    struct fake_bitmap : public xbitmap
    { void SetClut( void* pClut, int nCluts )
    { 
        if( GetFlags() & FLAG_CLUT_OWNED ) delete [](void*)GetClutData();
        m_pClut = (byte*)pClut; 
        m_ClutSize = nCluts*256*4; 

    }};

    // Set the new clut us the active one
    ((fake_bitmap*)&BMP)->SetClut( pClut, 3 );

#endif

    // Get the handle of the first clut 
    vram_Register( BMP );
    pClutHandle[0] = vram_GetClutHandle( BMP );
    pClutHandle[1] = 0;
    pClutHandle[2] = 0;

    //BMP.SaveTGA(xfs("AndyLM%03d.tga",AndyLMCount));
    //BMP.SavePaletteTGA(xfs("AndyLMPAL%03d.tga",AndyLMCount));
    //AndyLMCount++;
#endif
}

//=========================================================================

void bld_manager::Initialize( void )
{
    x_memset( this, 0, sizeof(bld_manager) );
    s_Init = TRUE;

    //
    // Initialzile the prep info structure
    //
    for( s32 i=0; i<4; i++ )
    {
        m_PrepInfo[ i ].bViewInZoneZero = TRUE;
        m_PrepInfo[ i ].OutPortal.Clear();
        m_PrepInfo[ i ].ZonesRendered   = 0;
    }
    
    //
    // Initialize the building rendering
    //
    BLDRD_Initialize();
    m_pFog = NULL;
    m_nInstances = 0;
}

//=========================================================================

void bld_manager::SetFog( const fog* pFog )
{
    m_pFog = pFog;
}

//=========================================================================

void bld_manager::Kill( void )
{
	int i;
    
    //
    // Delete all the bitmaps
    //
    for( i=0; i<m_nBitmaps; i++ )
    {
        vram_Unregister( m_Bitmap[i].Bitmap );
        m_Bitmap[i].Bitmap.Kill();
	}    

    //
    // Delete all the buildings
    //
    for ( i=0; i<m_nBuildings; i++)
    {
        DeleteBuilding( m_Building[i].FileName );
    }
    
    x_memset( this, 0, sizeof( *this ) );
}

//=========================================================================

void bld_manager::RefreshTextures( void )
{
    s32 i;

    //
    //  Frist kill all the bitmaps
    //
    for( i=0; i<m_nBitmaps; i++ )
    {
        // Unregister all from the vram manager
        vram_Unregister( m_Bitmap[i].Bitmap );

        /*
        if( m_Bitmap[i].BuildMips == FALSE )
        {
            VRAM_UnRegisterClut( m_Bitmap[i].ClutHandle[1] );          
            VRAM_UnRegisterClut( m_Bitmap[i].ClutHandle[2] );
        }
        */

        // Now kill the data for this bitmap
        m_Bitmap[i].Bitmap.Kill();
    }

    //
    // Now reload them
    //
    for( i=0; i<m_nBitmaps; i++ )
    {
        LoadTexture( m_Bitmap[i].Bitmap, m_Bitmap[i].FileName, m_Bitmap[i].BuildMips );

        if( m_Bitmap[i].BuildMips )
        {
            vram_Register( m_Bitmap[i].Bitmap );
        }
        else
        {
            CreateLMBitmap( m_Bitmap[i].Bitmap, m_Bitmap[i].ClutHandle );
        }
    }
}

//=========================================================================

//
// special case for building viewer: reload base texture from a PNG file
//

#define NUM_PATHS	6

char *search_paths[NUM_PATHS]=
{
	"badland",
	"desert",
	"ice",
	"lava",
	"lush",
	"data\\interiors\\ps2tex"
};

char PNG_path[256];
	
static 
int ReLoadTexture( xbitmap& BMP, const char* pFileName, xbool BuildMips )
{
    char FName[64];
    char Ext[16];
    char BitmapFileName[256];
    char Drive[64];
    char Dir[256];

    x_splitpath( pFileName, Drive, Dir, FName, Ext );
	x_sprintf  ( BitmapFileName, "%s%s.png", PNG_path, FName );
	
	//x_DebugMsg("XBM: %1s\n", BitmapFileName );
	
	if( !auxbmp_Load( BMP, BitmapFileName ) )
	{
		//x_DebugMsg("**** FAILED TO LOAD ****\n");	
		return(0);
	}

    #ifdef TARGET_PS2
	BMP.ConvertFormat(xbitmap::FMT_P8_RGBA_8888);
    auxbmp_ConvertToPS2(BMP);
	#else
    BMP.ConvertFormat(xbitmap::FMT_32_ARGB_8888);
    #endif

   	if (BuildMips)
	{
		BMP.BuildMips();
	}

	return(1);
}

//=========================================================================

int FindBitmap(const char *bitmap)
{
	for (int i=0; i<NUM_PATHS; i++)
	{
		char name[256];
		X_FILE* fp;
		
		x_sprintf(name, "%s\\%s.PNG", search_paths[i], bitmap);
		
		if ((fp = x_fopen(name, "rb")))
		{
			x_sprintf(PNG_path, "%s\\", search_paths[i]);
			x_fclose(fp);
			return(1);
		}
	}
	
	return(0);
}

//=========================================================================

int bld_manager::RefreshTexture( int n )
{
    char FName[64];
    char Ext[16];
    char Drive[64];
    char Dir[256];

    x_splitpath( m_Bitmap[n].FileName, Drive, Dir, FName, Ext );
	
	if (!FindBitmap(FName))
	{
		return(0);
	}

	vram_Unregister( m_Bitmap[n].Bitmap );
	m_Bitmap[n].Bitmap.Kill();

	if (ReLoadTexture( m_Bitmap[n].Bitmap, m_Bitmap[n].FileName, m_Bitmap[n].BuildMips ))
	{
		if( m_Bitmap[n].BuildMips )
		{
			vram_Register( m_Bitmap[n].Bitmap );
		}
		else
		{
			CreateLMBitmap( m_Bitmap[n].Bitmap, m_Bitmap[n].ClutHandle );
		}
	
		return(1);
	}
	else
	{
		return(0);
	}
}

//=========================================================================

int bld_manager::GetBitmapNames(char *buffer, int size)
{
	int i;
	
	x_memset(buffer, 0, size);
	
	for (i=0; i<m_nBitmaps; i++)
	{
		ASSERT(strlen(buffer) + strlen(m_Bitmap[i].FileName) < (u32)size);
		x_strcpy(buffer + (i * 256), m_Bitmap[i].FileName);
	}

	return(m_nBitmaps);
}

//=========================================================================

bld_manager::bitmap_node* bld_manager::GetBitmapNode(xbitmap& Bitmap)
{
	for (int i=0; i<m_nBitmaps; i++)
	{
		if (&m_Bitmap[i].Bitmap == &Bitmap)
		{
			return(&m_Bitmap[i]);
		}
	}

	return(0);
}

//=========================================================================

bld_manager::bitmap_node& bld_manager::AddBitmap( const char* pFileName )
{
    ASSERT( m_nBitmaps < MAX_BITMAPS );

    char FName[64];
    x_splitpath( pFileName, NULL, NULL, FName, NULL );

    ASSERT(x_strlen(FName) < (s32)sizeof(m_Bitmap[m_nBitmaps].FileName));
    x_strcpy( m_Bitmap[m_nBitmaps].FileName, FName  );
    m_Bitmap[m_nBitmaps].RefCount     = 0;
    x_memset( &m_Bitmap[ m_nBitmaps ].Bitmap, 0, sizeof(xbitmap) );

    return m_Bitmap[ m_nBitmaps++ ];
}

//=========================================================================

bld_manager::bitmap_node* bld_manager::GetBitmap( const char* pFileName )
{
    int i;
    ASSERT( pFileName );

    char FName[64];
    x_splitpath( pFileName, NULL, NULL, FName, NULL );

    for( i=0; i<m_nBitmaps; i++ )
    {
        if( x_stricmp( m_Bitmap[i].FileName, FName ) == 0 )  
        {
            m_Bitmap[i].RefCount++;
            return &m_Bitmap[i];
        }
    }

    return NULL;
}

//=========================================================================

s16 bld_manager::GetBitmapID( const char* pFileName )
{
    s32 i;
    ASSERT( pFileName );

    char FName[64];
    x_splitpath( pFileName, NULL, NULL, FName, NULL );

    for( i=0; i<m_nBitmaps; i++ )
    {
        if( x_stricmp( m_Bitmap[i].FileName, FName ) == 0 )  
        {
            return (s16)i;
        }
    }

    return -1;
}

//=========================================================================

xcolor AverageColor( const xbitmap& Bitmap )
{
    u32 R = 0;
    u32 G = 0;
    u32 B = 0;
    u32 A = 0;

    for( s32 y=0; y<Bitmap.GetHeight(); y+=2 )
    {
        for( s32 x=0; x<Bitmap.GetWidth(); x+=2 )
        {
            xcolor C = Bitmap.GetPixelColor( x, y );

            R += C.R;
            G += C.G;
            B += C.B;
            A += C.A;
        }
    }

    s32 NumPixels = Bitmap.GetWidth() * Bitmap.GetHeight() / ( 2 * 2 );

    R /= NumPixels;
    G /= NumPixels;
    B /= NumPixels;
    A /= NumPixels;

    return( xcolor( R, G, B, A ));
}

//=========================================================================

const xbitmap& bld_manager::LoadBitmap( const char* pFileName, s32** pClutHandle, xbool LoadBitmap )
{
	bitmap_node* pBitmapNode = GetBitmap( pFileName );
    if( pBitmapNode == NULL ) 
    {
        pBitmapNode = &AddBitmap( pFileName );

        // Set whether we need mips or not
        pBitmapNode->BuildMips = pClutHandle == NULL;

        if( LoadBitmap )
        {
            // Load the texture
            LoadTexture( pBitmapNode->Bitmap, pFileName,  pBitmapNode->BuildMips );
            
            pBitmapNode->AverageColor = AverageColor( pBitmapNode->Bitmap );
        }
        else
        {
            // dont load the texture: create a small dummy texture instead (to save memory)

            byte* pPixelData =        new byte  [  16 * 16 ];
            byte* pClutData  = (byte*)new xcolor[    256   ];
                
            pBitmapNode->Bitmap.Setup( xbitmap::FMT_P8_ABGR_8888, 16, 16, TRUE, pPixelData, TRUE, pClutData );

            //x_DebugMsg( "Dont need to load alarm map! %s\n", pFileName );
        }

        if( pBitmapNode->BuildMips )
        {
            vram_Register( pBitmapNode->Bitmap );
		}
        else
        {
			CreateLMBitmap( pBitmapNode->Bitmap, pBitmapNode->ClutHandle );
            *pClutHandle = pBitmapNode->ClutHandle;
		}
    }
    else
    {
        int a;
        a=0;
	}

    return pBitmapNode->Bitmap;
}

//=========================================================================

bld_manager::building_node& bld_manager::AddBuilding( const char* pFileName )
{
    ASSERT( m_nBuildings < MAX_BUILDINGS );

    ASSERT(x_strlen(pFileName) < (s32)sizeof(m_Building[m_nBuildings].FileName));
    x_strcpy( m_Building[m_nBuildings].FileName, pFileName  );
    m_Building[ m_nBuildings ].pBuilding    = new building;
    m_Building[ m_nBuildings ].RefCount     = 0;
    ASSERT( m_Building[ m_nBuildings ].pBuilding );

    return m_Building[ m_nBuildings++ ];    
}

//=========================================================================

bld_manager::building_node* bld_manager::GetBuilding( const char* pFileName )
{
    int i;
    
    ASSERT( pFileName );

    for( i=0; i<m_nBuildings; i++ )
    {
        if( x_stricmp( m_Building[i].FileName, pFileName ) == 0 ) 
        {
            m_Building[i].RefCount++;
            return &m_Building[i];
        }
    }

    return NULL;
}

s32 bld_manager::GetBuildingNum( building& Building )
{
    for( s32 i=0; i<m_nBuildings; i++ )
    {
        if( m_Building[i].pBuilding == &Building )
        {
            return( i );
        }
    }
    
    ASSERT( 0 );
    return( 0 );
}

//=========================================================================

void bld_manager::LoadBuilding( const char* pFileName, instance& Instance, xbool LoadAlarmMaps )
{
    building_node* pBuildingNode = GetBuilding( pFileName );

    if( pBuildingNode == NULL ) 
    {
        X_FILE* Fp = NULL;

        pBuildingNode = &AddBuilding( pFileName );
        Fp = x_fopen( pFileName, "rb" );
        ASSERT( Fp );

        //x_DebugMsg( "Loading Building: %s\n", pFileName );

		pBuildingNode->NumInstances = pBuildingNode->pBuilding->ImportData( Fp, LoadAlarmMaps );
        pBuildingNode->pBuilding->Initialize();

        x_fclose( Fp );
    }

    Instance.m_pBuilding = pBuildingNode->pBuilding;
	Instance.m_ID        = pBuildingNode->RefCount;

    m_pInstance[ m_nInstances ] = &Instance;
    m_nInstances++;
    //x_DebugMsg("NInstances: %1d\n",m_nInstances);

    Instance.m_nZones      = pBuildingNode->pBuilding->m_nZones;
    Instance.m_ZoneVisible = 0;

	if ( pBuildingNode->RefCount > pBuildingNode->NumInstances-1 )
	{
        Instance.m_HashAdd = pBuildingNode->NumInstances-1;
	}
	else
	{
		Instance.m_HashAdd = pBuildingNode->RefCount;
    } 
}

//=========================================================================

void bld_manager::DeleteBuilding(const char* pFileName)
{
	building_node* bn = GetBuilding(pFileName);
	
	if (!bn) return;

	bn->pBuilding->DeleteBuilding();

	delete(bn->pBuilding);

	x_memset(bn, 0, sizeof(building_node));
	bn->RefCount = 0;
}

//=========================================================================

void bld_manager::ReleaseBuilding( building& Building )
{
    s32 i;

    for( i=0; i<m_nBuildings; i++ )
    {
        if( m_Building[i].pBuilding == &Building )
            break;
    }

    ASSERTS( i < m_nBuildings, "That is strange I could not find that building" );   
    ASSERT ( m_Building[i].RefCount > 0 );
    m_Building[i].RefCount--;
}

//=========================================================================

s16 bld_manager::GetHashEntryID( const char* pBase, const char* pLM, int instance )
{
    render_table RenderTable;
    s16          LastGood = -1;
    s32          i;

    //
    // Find whether the has entry exits or not
    //
    RenderTable.iBaseMap  = GetBitmapID( pBase );
    RenderTable.iLightMap = GetBitmapID( pLM   ) + instance;

    for( i=0; i<m_nHashEntries; i++ )
    {
        render_table& Entry = m_RenderTable[ m_HashIDToTable[ i ] ];
        if( Entry.iLightMap != RenderTable.iLightMap ) continue;

        LastGood = (s16)i;

        if( Entry.iBaseMap  != RenderTable.iBaseMap  ) continue;
        break;
    }

    //
    // If not found the n added
    //
    if( i == m_nHashEntries )
    {
        ASSERT( m_nHashEntries < MAX_HASH_ENTRIES );


        //
        // Is there a good plaze to add this entry?
        //
        if( LastGood == -1 )
        {
            m_HashIDToTable[ i ] = i;
            m_RenderTable  [ i ] = RenderTable;
            m_RenderTable  [ i ].pDList[0] = 
            m_RenderTable  [ i ].pDList[1] =
            m_RenderTable  [ i ].pDList[2] = NULL;

            m_RenderTable[i].MinDist    = 100000.0f;
            m_RenderTable[i].MaxDist    = -m_RenderTable[i].MinDist;
            m_RenderTable[i].MinK       = 100000.0f;
            m_RenderTable[i].MaxK       = -m_RenderTable[i].MinK;
        }
        else
        {
            //
            // Move all the nodes so that we can put the new one
            //
            s32 Index = m_HashIDToTable[ LastGood ];
            x_memmove( &m_RenderTable[ Index + 1], 
                       &m_RenderTable[ Index ],
                       ( m_nHashEntries - Index ) * sizeof( render_table ) );
                        
            //
            // Update all the other Indices
            //
            for( i=0;i<m_nHashEntries;i++ )
            {
                if( m_HashIDToTable[i] >= Index ) m_HashIDToTable[i]++;
            }

            //
            // Put the new one in the right plaze
            //
            m_HashIDToTable[ i ] = Index;
            m_RenderTable  [ Index ] = RenderTable;
            m_RenderTable  [ Index ].pDList[0] = 
            m_RenderTable  [ Index ].pDList[1] =
            m_RenderTable  [ Index ].pDList[2] = NULL;

            m_RenderTable[Index].MinDist    = 100000.0f;
            m_RenderTable[Index].MaxDist    = -m_RenderTable[i].MinDist;
            m_RenderTable[Index].MinK       = 100000.0f;
            m_RenderTable[Index].MaxK       = -m_RenderTable[i].MinK;

        }

        ASSERT( i == m_nHashEntries );
        m_nHashEntries++;
    }
    else
    {
		int a;
        a=0;
    }

    return (s16)i;
}

//=========================================================================

void bld_manager::UpdatePrepInfo( prep_info& PrepInfo,
                                  s32        ViewPortID )
{
    //
    // Update the prerender info
    //
    if( PrepInfo.bViewInZoneZero == FALSE )
    {
        // This should only happens ones per view at max
        m_PrepInfo[ ViewPortID ] = PrepInfo;

        //draw_Rect( PrepInfo.OutPortal, xcolor(255,255,255,255), TRUE );
    }
    else
    {
        if( m_PrepInfo[ ViewPortID ].bViewInZoneZero == TRUE )
        {
            m_PrepInfo[ ViewPortID ].ZonesRendered |= PrepInfo.ZonesRendered;
        }
    }
}

//=========================================================================

void bld_manager::AddDisplayListInHash( dlist_node&     Node, 
                                        s32             ViewPortID,
                                        f32             MinDist,
                                        f32             MaxDist,
										s32             Instance )
{
    ASSERT( Node.pDList );
    ASSERT( ViewPortID >= 0 );
    ASSERT( ViewPortID <  4 );

    // Add the display list into the hash node
    s32 Index  = m_HashIDToTable[ Node.pDList->iHash + Instance ];
    Node.pNext = m_RenderTable[ Index ].pDList[ ViewPortID ];
    m_RenderTable[ Index ].pDList[ ViewPortID ] = &Node;

    if( MinDist < m_RenderTable[ Index ].MinDist ) m_RenderTable[ Index ].MinDist = MinDist;
    if( MaxDist > m_RenderTable[ Index ].MaxDist ) m_RenderTable[ Index ].MaxDist = MaxDist;

    if( Node.pDList->K < m_RenderTable[ Index ].MinK ) m_RenderTable[ Index ].MinK = Node.pDList->K;
    if( Node.pDList->K > m_RenderTable[ Index ].MaxK ) m_RenderTable[ Index ].MaxK = Node.pDList->K;
}


//=========================================================================

#ifndef TARGET_PC
f32 g_MaxLod = 0.25f;
//#include "demo1/globals.hpp"
#endif

volatile xbool BLDMANAGER_Log = FALSE;
static X_FILE* LogFP = NULL;

#ifdef TARGET_PS2
extern s32 VRAM_BytesLoaded;
#endif
#ifdef TARGET_PC
#define VRAM_BytesLoaded 0
#endif

#if defined( X_DEBUG ) && defined( jcossigny )

extern building::dlist* pDLists[];
extern bld_manager g_BldManager;

s32 build=-1;
s32 ngon=0;
s32 zb=0;
s32 uuuu=0x55555555;

int strips=0;
int strip=0;
s32 jp_skip=0x55555555;
s32 jp_poly=0;
s32 jp_surf=0;
s32 jp_ngon=-1;
s32 jp_end=-1;

xbitmap* jp_base;
xbitmap* jp_lmap;
char* jp_basename;
char* jp_lmapname;

building::ngon jp_ngons[256];
s32 jp_num;

s32  ComputeNGonInfo(building::ngon* pNGons, const building::dlist& DList);

#endif

//#define VRAM_STATS

void bld_manager::Render( void )
{
    s32         nViewPorts      = eng_GetNActiveViews();
    s16         LastLightMapID  = -1;
    s16         LastBaseMapID   = -1;
    instance*   LastInstance    = NULL;
    s32         i,j;
    s32         TotalVerts=0;
    s32         TotalDataSize=0;
    s32         TotalDLists=0;
    u16         LastMask   = 0;

    // Return if there are no building pieces
    if( m_nHashEntries == 0 )
        return;

    if( BLDMANAGER_Log && (LogFP==NULL) )
    {
        LogFP = x_fopen("bldlog.txt","wt");
        ASSERT(LogFP);
    }

    if( BLDMANAGER_Log && LogFP )
    {
        x_fprintf(LogFP,"-------- RENDER --------\n");
    }

    //
    // Signel the begining of th rendering for the buildings
    //
    BLDRD_Begin();
#ifndef TARGET_PC
    ASSERT( m_pFog );
    BLDRD_UpdaloadFog( m_pFog->GetBMP() );
#endif

    // Compute best case texture uploads
    s32 BaseUploadSize=0;
    s32 LMapUploadSize=0;
    s32 LMapCount=0;
    s32 BaseCount=0;
#ifdef VRAM_STATS
    {
        s32 i,j,h;

        // Compute 
        for( i=0; i<m_nHashEntries; i++ )
        {
            // Find all entries with using base texture i
            f32 MinDist= 1000000.0f;
            f32 MaxDist=-1000000.0f;
            f32 MinK   = 1000000.0f;
            f32 MaxK   =-1000000.0f;
            s32 Count  = 0;

            for( h=0; h<m_nHashEntries; h++ )
            if( m_RenderTable[h].iBaseMap == i )
            {
                for( j=0; j<nViewPorts; j++ )
                if( m_RenderTable[h].pDList[j] != NULL)
                    break;

                if( j!=nViewPorts )                
                {
                    MinDist = MIN( MinDist, m_RenderTable[h].MinDist );
                    MaxDist = MAX( MaxDist, m_RenderTable[h].MaxDist );
                    MinK = MIN( MinK, m_RenderTable[h].MinK );
                    MaxK = MAX( MaxK, m_RenderTable[h].MaxK );
                    Count++;
                }
            }
            
            if( Count )
            {
                f32 MipKUsed = (MinK + MaxK)*0.5f*PS2BUILDING_GLOBALK;
                f32 MinLOD,MaxLOD;

                vram_ComputeMipRange( MinDist, 
                                      MaxDist, 
                                      MipKUsed, 
                                      MinLOD, 
                                      MaxLOD );
            
                BaseUploadSize += vram_GetUploadSize( m_Bitmap[i].Bitmap, MinLOD, MaxLOD );
                BaseCount++;
            }
        }

        // Compute lightmap upload sizes
        for( i=0; i<m_nHashEntries; i++ )
        {
            for( h=0; h<m_nHashEntries; h++ )
            if( m_RenderTable[h].iLightMap == i )
            {
                for( j=0; j<nViewPorts; j++ )
                if( m_RenderTable[h].pDList[j] != NULL)
                    break;

                if( j!=nViewPorts )                
                {
                    LMapCount++;
                    LMapUploadSize += vram_GetUploadSize( m_Bitmap[i].Bitmap, 0, 10 );
                    break;
                }
            }
        }
        // Add two more cluts
        LMapUploadSize += LMapCount*1024*2;

        x_printfxy(15,0,"BASE %3d %8d",BaseCount,BaseUploadSize);
        x_printfxy(15,1,"LMAP %3d %8d",LMapCount,LMapUploadSize);
    }
#endif

    BaseUploadSize=0;
    LMapUploadSize=0;
    LMapCount=0;
    BaseCount=0;
    s32 NHashEntries = 0;
    s32 NDLists = 0;
    for( i=0; i<m_nHashEntries; i++ )
    for( j=0; j<nViewPorts;     j++ )
    {
        if( m_RenderTable[i].pDList[j] == NULL ) 
            continue;

        NHashEntries++;

        if( BLDMANAGER_Log && LogFP )
        {
            x_fprintf(LogFP,"DLIST %2d %2d\n",
                            m_RenderTable[i].iBaseMap,
                            m_RenderTable[i].iLightMap );
        }

        //
        // Clear the instance when ever we have more than one view port
        //
        if( nViewPorts > 1 ) 
            LastInstance = NULL;

        //
        // Take care of uploading the textures
        //
        if( LastLightMapID != m_RenderTable[i].iLightMap)
        {
            LastLightMapID  = m_RenderTable[i].iLightMap;
            ASSERT( LastLightMapID >= 0 );
            ASSERT( m_Bitmap[ LastLightMapID ].BuildMips == FALSE );

            s32 LMBytes = VRAM_BytesLoaded;
            BLDRD_UpdaloadLightMap( m_Bitmap[ LastLightMapID ].Bitmap,  m_Bitmap[ LastLightMapID ].ClutHandle );                
            LMapCount++;
            LMapUploadSize += VRAM_BytesLoaded - LMBytes;

            if( BLDMANAGER_Log && LogFP )
            {
                x_fprintf(LogFP,"LMAP %2d\n", LastLightMapID );
            }

        }

        if( LastBaseMapID  != m_RenderTable[i].iBaseMap )
        {
            LastBaseMapID   = m_RenderTable[i].iBaseMap;


            f32 MinLOD = 0;
            f32 MaxLOD = 10;
            // Compute the min/max lod
#ifdef TARGET_PS2
            {
                // Get stats on following mips
                f32 MinDist= 1000000.0f;
                f32 MaxDist=-1000000.0f;
                f32 MinK   = 1000000.0f;
                f32 MaxK   =-1000000.0f;

                s32 h = i;
                while( (h<m_nHashEntries) && 
                       ((m_RenderTable[h].pDList[j] == NULL ) || (m_RenderTable[h].iBaseMap == LastBaseMapID)) )
                {
                    if( m_RenderTable[h].pDList[j] != NULL )
                    {
                        ASSERT( m_RenderTable[h].MinDist <= m_RenderTable[h].MaxDist );

                        MinDist = MIN( MinDist, m_RenderTable[h].MinDist );
                        MaxDist = MAX( MaxDist, m_RenderTable[h].MaxDist );
                        MinK = MIN( MinK, m_RenderTable[h].MinK );
                        MaxK = MAX( MaxK, m_RenderTable[h].MaxK );
                    }

                    h++;
                }

                f32 MipKUsed = (MinK + MaxK)*0.5f*PS2BUILDING_GLOBALK;

                vram_ComputeMipRange( MinDist, 
                                      MaxDist, 
                                      MipKUsed, 
                                      MinLOD, 
                                      MaxLOD );
                
                vram_SetMipEquation( MipKUsed );

            }
#endif
            ASSERT( m_Bitmap[ LastBaseMapID ].BuildMips == TRUE );
            s32 BMBytes = VRAM_BytesLoaded;
            BLDRD_UpdaloadBase( m_Bitmap[ LastBaseMapID ].Bitmap, MinLOD, MaxLOD ); 

            BaseCount++;
            BaseUploadSize += VRAM_BytesLoaded - BMBytes;

            if( BLDMANAGER_Log && LogFP )
            {
                x_fprintf(LogFP,"BMAP %2d %1.2f %1.2f\n", LastBaseMapID, MinLOD, MaxLOD );
            }

        }

        //
        // Render all the display list in this hash entry
        //
        for( dlist_node* pList = m_RenderTable[i].pDList[j]; pList ; pList = pList->pNext )
        {
            NDLists++;

            //
            // Take care of uploading the matrices
            //
            if( LastInstance != pList->pInstance )
            {
                LastInstance = pList->pInstance;

#ifdef TARGET_PS2

                f32 Dist0,Dist1;
                f32 DeltaY0,DeltaY1;
                vector4 FogMul;
                vector4 FogAdd;

                ASSERT( m_pFog );
                m_pFog->GetFogConst( Dist0, Dist1, DeltaY0, DeltaY1 );

                FogMul  = vector4(Dist0,DeltaY0,0,0);
                FogAdd  = vector4(Dist1,DeltaY1,1,1);

                vector3 WEP,LEP;
                const view* pView = eng_GetActiveView(0);
                WEP = pView->GetPosition();
                LEP = pList->pInstance->m_W2L * WEP;
                
                BLDRD_UpdaloadMatrices( pList->pInstance->m_RL2C[j],
                                        pList->pInstance->m_RC2S[j],
                                        pList->pInstance->m_RL2S[j],
                                        pList->pInstance->m_L2W,
                                        WEP,
                                        LEP,
                                        FogMul,
                                        FogAdd );
#else
                BLDRD_UpdaloadMatrices( pList->pInstance->m_RL2W[j], 
                                        pList->pInstance->m_RW2V[j], 
                                        pList->pInstance->m_RV2C[j],
                                        pList->pInstance->m_L2W,
                                        vector3(0,0,0),
                                        vector3(0,0,0),
                                        vector4(0,0,0,0),
                                        vector4(0,0,0,0) );
#endif

            }

#ifdef TARGET_PS2
            //
            // Set the mask in the display list
            //
            if( LastMask != pList->pDList->PassMask )
            {
                LastMask = pList->pDList->PassMask;
                BLDRD_UpdaloadPassMask( LastMask );
            }
#endif
            
            //--------------------------------------------------------------------------------------------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------------------------------------------

            #if defined( X_DEBUG ) && defined( jcossigny )

           	if (strips)
        	{
                building::dlist& DList = *pList->pDList;
            
            	if( jp_skip && (u32)&DList != (u32)(pDLists[strip]) ) continue;

                if( jp_poly )
                {
                    if( (u32)&DList == (u32)(pDLists[strip]) )
                    {
                        s32 iHash = DList.iHash;
        
                        jp_base = (xbitmap*)( (void*)&g_BldManager.GetBaseBitmap( iHash ));
                        jp_lmap = (xbitmap*)( (void*)&g_BldManager.GetLightmap( iHash ));
        
                        jp_basename = g_BldManager.GetBitmapNode( *jp_base )->FileName;
                        jp_lmapname = g_BldManager.GetBitmapNode( *jp_lmap )->FileName;
                
                        if( jp_ngon < 0 )
                        {
                            vector4* pVertex = (vector4* )( DList.pData + 32 );        // +32 skips over DMA tag and header
                        
                            draw_SetL2W( pList->pInstance->m_L2W );
                            draw_Begin( DRAW_TRIANGLES, 0 );

                            s32 c=0;
                            s32 num=0;

                            for( s32 k=0; k<DList.nVertices; k++, c++ )
                            {
                                if( *((u32*)&pVertex[k].W) & (1<<15)  )
                                {
                                    ASSERT( *((u32*)&pVertex[k+1].W) & (1<<15) );
                                    k += 2;
                                    c  = 2;
                                    num++;
                                }
                                ASSERT( k<DList.nVertices );

                                if( jp_surf && ( jp_surf != num )) continue;

                                for( int t=0; t<3; t++)
                                {
                                    static const int off[2][3]={ {2,1,0}, {1,2,0} };
                                    int m = k - off[c&1][t];
                                
                                    xcolor Color;
                                
                                    Color.R = (u32)(num * k * 30) & 255;
                                    Color.G = Color.B = 0;
                                    
                                    //Color.R = x_rand();
                                    //Color.G = x_rand();
                                    //Color.B = x_rand();
                                    
                                    draw_Color( Color );

                                    draw_Vertex( pVertex[m].X, 
                                                 pVertex[m].Y, 
                                                 pVertex[m].Z ); 
                                }
                            }
                            
                            draw_End();
                        
                            continue;
                        }
                        else
                        {
                            jp_num  = ComputeNGonInfo( jp_ngons, DList );
                            jp_ngon = MAX( 0, MIN( jp_ngon, jp_num-1 ));

                            building::ngon& NGon = jp_ngons[ jp_ngon ];
                
                            draw_Begin( DRAW_LINES, DRAW_NO_ZBUFFER );
                            draw_SetL2W( pList->pInstance->m_L2W );
                            draw_Color( XCOLOR_RED );
                            draw_Vertex( NGon.Pt[0] );
                            draw_Vertex( NGon.Pt[1] );
                            draw_Vertex( NGon.Pt[1] );
                            draw_Vertex( NGon.Pt[2] );
                            draw_Vertex( NGon.Pt[2] );
                            draw_Vertex( NGon.Pt[0] );
                            draw_End();
    
                            //s32 minu = (s32)x_floor( NGon.MinUV.X * jp_lmap->GetWidth()  - 0.5f );
                            //s32 minv = (s32)x_floor( NGon.MinUV.Y * jp_lmap->GetHeight() - 0.5f );
                            //s32 maxu = (s32)x_floor( NGon.MaxUV.X * jp_lmap->GetWidth()  + 0.5f );
                            //s32 maxv = (s32)x_floor( NGon.MaxUV.Y * jp_lmap->GetHeight() + 0.5f );
                            
                            //x_DebugMsg(" %3d %3d : %3d %3d\n", minu, minv, maxu, maxv );
                        }
                    }
                }
            }
    
            #endif
            
            //--------------------------------------------------------------------------------------------------------------------------------------------
            //--------------------------------------------------------------------------------------------------------------------------------------------
            
            //
            // Render the actual display list
            //
            ASSERT( pList->pDList );
            BLDRD_RenderDList( *pList->pDList, pList->bClip );

            // Increment stats
            TotalVerts += pList->pDList->nVertices;
            TotalDataSize += pList->pDList->SizeData;
            TotalDLists++;
		}

		//BLDRD_Delay();

        //
        // Make sure to clear the display list for the next pass
        //
        m_RenderTable[i].pDList[j] = NULL;

        if( j == (nViewPorts-1) )
        {
            m_RenderTable[i].MinDist    = 100000.0f;
            m_RenderTable[i].MaxDist    = -m_RenderTable[i].MinDist;
            m_RenderTable[i].MinK       = 100000.0f;
            m_RenderTable[i].MaxK       = -m_RenderTable[i].MinK;
        }
    }
#ifdef VRAM_STATS
    x_printfxy(15,4+0,"BASE   %3d %8d",BaseCount,BaseUploadSize);
    x_printfxy(15,4+1,"LMAP   %3d %8d",LMapCount,LMapUploadSize);
    x_printfxy(15,4+2,"NHash  %3d",NHashEntries);
    x_printfxy(15,4+3,"NDList %3d",NDLists);
#endif
    //
    // Initialzile the prep info structure
    //
    for( i=0; i<4; i++ )
    {
        m_PrepInfo[ i ].bViewInZoneZero = TRUE;
        m_PrepInfo[ i ].OutPortal.Clear();
        m_PrepInfo[ i ].ZonesRendered   = 0;
    }

    //
    // Signel the end of th rendering for the buildings
    //
    BLDRD_End();

#ifdef athyssen
    //x_printfxy(0,2,"BLD D:%1d V:%1d S:%1d\n",TotalDLists,TotalVerts,TotalDataSize);
#endif

    if( PRINT_BUILDING_STATS )
    {
        PRINT_BUILDING_STATS = FALSE;
        
        g_BldManager.PrintStats();
    }

    #if defined( X_DEBUG ) && defined( jcossigny )
    if( build >= 0 )
    {
        RenderNGon( build, ngon );
    }
    #endif
}

//=========================================================================

void bld_manager::PrintStats( void )
{
    x_DebugMsg( "Buildings    = %d\n", m_nBuildings );

    s32 DisplayList = 0;
    s32 Collision   = 0;
    s32 BSPData     = 0;
    s32 PlaneData   = 0;

    for( s32 i=0; i<m_nBuildings; i++ )
    {
        s32 d, c, b, p;
        
        m_Building[i].pBuilding->GetStats( d, c, b, p );
        
        DisplayList += d;
        Collision   += c;
        BSPData     += b;
        PlaneData   += p;
    }

    x_DebugMsg( "Display List = %d\n", DisplayList  );
    x_DebugMsg( "Collision    = %d\n", Collision    );
    x_DebugMsg( "BSPData      = %d\n", BSPData      );
    x_DebugMsg( "PlaneData    = %d\n", PlaneData    );
    x_DebugMsg( "-------------------------------\n" );
    x_DebugMsg( "Total Size   = %d\n", DisplayList + Collision + BSPData + PlaneData );
}

//=========================================================================

void bld_manager::CleanHash( void )
{
    s32 i;

    for( i=0; i<m_nHashEntries; i++ )
    {
        m_RenderTable[i].pDList[0] = NULL;
        m_RenderTable[i].pDList[1] = NULL;
        m_RenderTable[i].pDList[2] = NULL;
        m_RenderTable[i].pDList[3] = NULL;
        m_RenderTable[i].MinDist    = 100000.0f;
        m_RenderTable[i].MaxDist    = -m_RenderTable[i].MinDist;
        m_RenderTable[i].MinK       = 100000.0f;
        m_RenderTable[i].MaxK       = -m_RenderTable[i].MinK;
    }

    //
    // Initialzile the prep info structure
    //
    for( i=0; i<4; i++ )
    {
        m_PrepInfo[ i ].bViewInZoneZero = TRUE;
        m_PrepInfo[ i ].OutPortal.Clear();
        m_PrepInfo[ i ].ZonesRendered   = 0;
    }
}

//=========================================================================

xbool bld_manager::IsViewInZoneZero( s32 ViewPortID )
{
    ASSERT( ViewPortID >= 0 );
    ASSERT( ViewPortID <  4 );
    return m_PrepInfo[ ViewPortID ].bViewInZoneZero;
}

//=========================================================================

xbool bld_manager::IsZoneZeroVisible( s32 ViewPortID )
{
    ASSERT( ViewPortID >= 0 );
    ASSERT( ViewPortID <  4 );
    return !!(m_PrepInfo[ ViewPortID ].ZonesRendered & 1) || IsViewInZoneZero(0);
}

//=========================================================================

rect bld_manager::GetZoneZeroRect( s32 ViewPortID )
{
    ASSERT( ViewPortID >= 0 );
    ASSERT( ViewPortID <  4 );
    return m_PrepInfo[ ViewPortID ].OutPortal;
}

//=========================================================================

xcolor bld_manager::GetAverageColor( s32 iHash )
{
    return m_Bitmap[ m_RenderTable[ m_HashIDToTable[iHash] ].iBaseMap ].AverageColor;
}

//=========================================================================

const xbitmap& bld_manager::GetBaseBitmap( s32 iHash )
{
    return m_Bitmap[ m_RenderTable[ m_HashIDToTable[iHash] ].iBaseMap ].Bitmap;

//    return m_Bitmap[ m_RenderTable[iHash].iBaseMap ].Bitmap;
}

//=========================================================================

const xbitmap& bld_manager::GetLightmap( s32 iHash )
{
    return m_Bitmap[ m_RenderTable[ m_HashIDToTable[iHash]].iLightMap ].Bitmap;
    
//    return m_Bitmap[ m_RenderTable[iHash].iLightMap ].Bitmap;
}

//=========================================================================

const char* bld_manager::GetBuildingName( const building& Building ) const
{
    for( s32 i=0; i<m_nBuildings; i++ )
    {
        const building_node& Node = m_Building[i];
        if( Node.pBuilding == &Building )
            return Node.FileName;
    }

    return NULL;
}

//=========================================================================

xbool bld_manager::IsZoneVisible( s32 BuildingIndex, s32 ZoneID )
{
    ASSERT( (BuildingIndex>=0) && (BuildingIndex<m_nInstances) );
    ASSERT( (ZoneID>=0) && (ZoneID<m_pInstance[BuildingIndex]->m_nZones) );
    return (m_pInstance[BuildingIndex]->m_ZoneVisible & (((u64)1)<<ZoneID)) ? (1):(0);
}

//=========================================================================

void bld_manager::RenderZoneInfo( void )
{
    for( s32 i=0; i<m_nInstances; i++ )
    {
        x_printfxy(0,2+i,"[%2d] ",i);
        for( s32 j=0; j<m_pInstance[i]->m_nZones; j++ )
            x_printfxy(6+j,2+i,"%1c",(m_pInstance[i]->m_ZoneVisible & (((u64)1)<<j)) ? 'X':'.');
    }
}

//=========================================================================

void bld_manager::ClearZoneInfo( void )
{
    s32 i;

    //
    // Initialzile the prep info structure
    //
    for( i=0; i<4; i++ )
    {
        m_PrepInfo[ i ].bViewInZoneZero = TRUE;
        m_PrepInfo[ i ].OutPortal.Clear();
        m_PrepInfo[ i ].ZonesRendered   = 0;
    }

    for( i=0; i<m_nInstances; i++ )
    {
        m_pInstance[i]->m_ZoneVisible = 0;
    }
}

//=========================================================================

void bld_manager::GetZoneSet( zone_set& ZS, const bbox& BBox, xbool JustCenter )
{
    vector3 P[9];
    P[0] = BBox.GetCenter();
    P[1].X = BBox.Min.X;    P[1].Y = BBox.Min.Y;    P[1].Z = BBox.Min.Z;
    P[2].X = BBox.Min.X;    P[2].Y = BBox.Min.Y;    P[2].Z = BBox.Max.Z;
    P[3].X = BBox.Min.X;    P[3].Y = BBox.Max.Y;    P[3].Z = BBox.Min.Z;
    P[4].X = BBox.Min.X;    P[4].Y = BBox.Max.Y;    P[4].Z = BBox.Max.Z;
    P[5].X = BBox.Max.X;    P[5].Y = BBox.Min.Y;    P[5].Z = BBox.Min.Z;
    P[6].X = BBox.Max.X;    P[6].Y = BBox.Min.Y;    P[6].Z = BBox.Max.Z;
    P[7].X = BBox.Max.X;    P[7].Y = BBox.Max.Y;    P[7].Z = BBox.Min.Z;
    P[8].X = BBox.Max.X;    P[8].Y = BBox.Max.Y;    P[8].Z = BBox.Max.Z;

    ZS.NZones = 0;
    ZS.InZoneZero = 0;
    s32 i,j,k;
    s32 NPoints = (JustCenter)?(1):(9);

    // Loop through all buildings
    for( i=0; i<m_nInstances; i++ )
    {
        // Check if bboxes overlap
        bbox BB;
        ((bld_instance*)m_pInstance[i])->GetBBox(BB);
        if( !BBox.Intersect(BB) )
            continue;

        // BBoxes overlap so check for zones
        for( j=0; j<NPoints; j++ )
        {
            s32 Z = ((bld_instance*)m_pInstance[i])->GetZoneFromPoint( P[j] );
            if( Z==0 )
            {
                ZS.InZoneZero = TRUE;
            }
            else
            {
                // Check if already in list
                for( k=0; k<ZS.NZones; k++ )
                {
                    if( (ZS.ZoneID[k] == (byte)Z) &&
                        (ZS.BuildingID[k] == (byte)i)) 
                        break;
                }

                // Add zone if unique
                if( (k==ZS.NZones) && (ZS.NZones<4) )
                {
                    ZS.ZoneID[ ZS.NZones ] = (byte)Z;
                    ZS.BuildingID[ ZS.NZones ] = (byte)i;
                    ZS.NZones++;
                }
            }
        }
    }

    if( ZS.NZones==0 )
        ZS.InZoneZero = TRUE;
}

//=========================================================================

xbool bld_manager::IsZoneSetVisible( zone_set& ZS )
{
    if( ZS.InZoneZero && IsZoneZeroVisible(0) )
        return TRUE;

    // Check individual zones
    for( s32 i=0; i<ZS.NZones; i++ )
    {
        if( m_pInstance[ZS.BuildingID[i]]->m_ZoneVisible & (((u64)1)<<ZS.ZoneID[i]) )
            return TRUE;
    }

    return FALSE;
}

//=========================================================================

#if defined( X_DEBUG ) && defined( jcossigny )

struct dlist_uv
{
    s16 TU,TV;
    u16 LU,LV;
};

xcolor ngon_cols[8] =
{
    xcolor( 128, 128, 128 ),
    xcolor( 128,   0,   0 ),
    xcolor(   0, 128,   0 ),
    xcolor(   0,   0, 128 ),
    xcolor( 128,   0, 128 ),
    xcolor( 128, 128,   0 ),
    xcolor(   0, 128, 128 ),
    xcolor(  64,  64,  64 ),
};

vector3 ngon_pts[64];
s32 ngon_npts;
s32 ngon_edge=0;
f32 ngon_area;
f32 ngon_ang;
f32 ngon_dot;

s32 StripToNGonWithUVs( vector3* pDstVert, vector4*  pSrcVert, 
                        vector2* pDstUV,   dlist_uv* pSrcUV,
                        s32 MaxVerts );

void bld_manager::RenderNGon( s32 iBuilding, s32 iNGon )
{
    building* pBuilding = m_Building[ iBuilding ].pBuilding;

    if( !pBuilding ) return;    

    if( iNGon >= pBuilding->m_CGrid.m_nNGons ) return;
    
    building::building_grid::ngon* pNGon = &pBuilding->m_CGrid.m_pNGon[ iNGon ];

    vector2  uvs[32];
    dlist_uv lst[32];
    
    ngon_npts = StripToNGonWithUVs( ngon_pts, pNGon->pVert, uvs, lst, pNGon->nVerts );
    ngon_area = 0;

    draw_Begin( DRAW_TRIANGLES, zb ? 0 : DRAW_NO_ZBUFFER );

    for( s32 i=1; i<pNGon->nVerts-1; i++ )
    {
        draw_Color( ngon_cols[i & 0x7] );
        
        draw_Vertex( ngon_pts[ 0 ] );
        draw_Vertex( ngon_pts[ i ] );
        draw_Vertex( ngon_pts[i+1] );
        
        vector3 V0 = ngon_pts[ i ] - ngon_pts[0];
        vector3 V1 = ngon_pts[i+1] - ngon_pts[0];
        
        vector3 V  = V0.Cross( V1 );
        ngon_area += V.Length() / 2.0f;
    }

    draw_End();

    {
        draw_Begin( DRAW_LINES, DRAW_NO_ZBUFFER );

        for( s32 i=0; i<pNGon->nVerts; i++ )
        {
            s32 A = i;
            s32 B = ( i + 1) % pNGon->nVerts;
            s32 C = ( i + 2) % pNGon->nVerts;
        
            vector3 V0 = ngon_pts[A] - ngon_pts[B];
            vector3 V1 = ngon_pts[C] - ngon_pts[B];

            V0.Normalize();
            V1.Normalize();

            if( ngon_edge == i )
            {
                ngon_dot = V0.Dot( V1 );
                ngon_ang = RAD_TO_DEG( x_acos( ngon_ang ));
                
                draw_Color( XCOLOR_WHITE );
            }
            else
            {
                draw_Color( XCOLOR_RED );
            }
        
            draw_Vertex( ngon_pts[A] );
            draw_Vertex( ngon_pts[B] );
        }
        
        draw_End();
    }
}

#endif
