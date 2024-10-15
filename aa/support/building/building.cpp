
#include "Building.hpp"

#include "AUX_Bitmap.hpp"
#include "bldmanager.hpp"
#include "x_files.hpp"  

#include "ObjectMgr\ObjectMgr.hpp"

///////////////////////////////////////////////////////////////////////////
// DEFINES 
///////////////////////////////////////////////////////////////////////////

#define BUILDING_VERSION "BILD006A"

///////////////////////////////////////////////////////////////////////////
// VARIABLES
///////////////////////////////////////////////////////////////////////////

building::zone_walk building::s_ZoneWalk;

#define NUM_SKIP_NAMES  3

static char* s_SkipNames[NUM_SKIP_NAMES]=
{
    "spir",
    "rock",
    "plat",
};

//
// Environments
//

enum
{
    BADLAND,
    DESERT,
    ICE,
    LAVA,
    LUSH,
};

static f32 s_LightingValues[5][2]=
{
    { 0.15f, 0.25f },     // Badland
    { 0.30f, 0.25f },     // Desert
    { 0.20f, 0.10f },     // Ice
    { 0.15f, 0.10f },     // Lava
    { 0.20f, 0.20f },     // Lush
};

static s32 s_Environment;

xcolor LookupColors[3];

///////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////

//=========================================================================

void building::Initialize( void )
{
    f32 Average = 0;
    s32 i;

    //
    // Compute Average world pixel size for ALL display lists in building
    //

    s32 Num = 0;

    for( i=0; i<m_nDLists; i++ )
    {
        if( m_pDList[i].WorldPixelSize )
        {
            Average += m_pDList[i].WorldPixelSize;
            Num++;
        }
    }

    if( Num )
    {
        Average /= Num;
    }

    //
    // Look for unset world pixel sizes and set them to building average
    //
    
    for( i=0; i<m_nDLists; i++ )
    {
        if( m_pDList[i].WorldPixelSize == 0 )
        {
            m_pDList[i].WorldPixelSize = Average;
        }
    }
}

void building::GetStats( s32& DList, s32& Collision, s32& BSPData, s32& PlaneData )
{
    DList = 0;

    for( s32 i=0; i<m_nDLists; i++ )
    {
        DList += m_pDList[i].SizeData;
    }

    Collision = m_CGrid.m_MemSize;
    BSPData   = m_nBSPNodes * sizeof( bspnode );
    PlaneData = m_nPlanes   * sizeof(  plane  );
}


//=========================================================================

void building::IOFields( bld_fileio& FileIO )
{
    FileIO.HandleIOField( m_pDListData,         m_nBytesData            );
    FileIO.HandleIOField( m_pDList,             m_nDLists               );
    FileIO.HandleIOField( m_pPortal,            m_nPortals              );
    FileIO.HandleIOField( m_pPortalPoint,       m_nPortalPoints         );
    FileIO.HandleIOField( m_pZone,              m_nZones                );
    FileIO.HandleIOField( m_pMaterialHandle,    m_nMaterials            );
    FileIO.HandleIOField( m_pHash,              m_nHashs                );
    FileIO.HandleIOField( m_pHashMat,           m_nHashs                );
    FileIO.HandleIOField( m_pLightMap,          m_nLightMaps            );
    
    FileIO.HandleIOField( m_pBSPNode,           m_nBSPNodes             );
    FileIO.HandleIOField( m_pPlane,             m_nPlanes               );

    FileIO.HandleIOField( m_CGrid.m_pNGon,      m_CGrid.m_nNGons        );
    FileIO.HandleIOField( m_CGrid.m_pCell,      m_CGrid.m_nGridCells    );
    FileIO.HandleIOField( m_CGrid.m_pNGonRef,   m_CGrid.m_nNGonRefs     );
}

//=========================================================================

void building::ExportData( X_FILE* Fp, bld_fileio& FileIO, material* pMaterialList )
{
    // Save all the fields
    FileIO.WriteFields( *this, Fp );

    // Save the name of the textures
    FileIO.WriteChunck( pMaterialList, sizeof(material)*(m_nMaterials+m_nLightMaps), Fp );    
}

//=========================================================================

void building::DeleteBuilding()
{
    #ifdef TARGET_PC
    
    for (s32 i=0; i<m_nDLists; i++)
    {
        ASSERT ( m_pDList[i].pData );
        delete m_pDList[i].pData;
    }
    
    #endif
    
    delete(m_pDListData);
}

//=========================================================================

void building::CompilePCData( dlist& DList )
{
#ifdef TARGET_PC

    //
    // Allocate the pc data
    // 
    dlist::pc_data* pPCData = new dlist::pc_data;
    ASSERT( pPCData );

    pPCData->pVBuffer        = NULL;
    pPCData->pIBuffer        = NULL;
    pPCData->FacetCount      = 0;

    //
    // Build the vertices in to the vertex buffer
    //

    struct temp_uv
    {
        s16 BaseU, BaseV;
        s16 LMU, LMV;
    };

    s32                 i, c=0, m=0;
    dxerr               Error;
    dlist::pc_vertex*   pVertex;
    s16*                pIndex;
    vector4*            pPS2Vertex = (vector4*) ( ((u32*)DList.pData) + 8 );
    temp_uv*            pUV        = (temp_uv*) ( ((u32*)&pPS2Vertex[ DList.nVertices ]) + 4 );


    Error = g_pd3dDevice->CreateVertexBuffer( DList.nVertices * sizeof(dlist::pc_vertex), 
                                              D3DUSAGE_WRITEONLY, 0, 
                                              D3DPOOL_MANAGED, 
                                              &pPCData->pVBuffer );
    ASSERT( Error == 0 );

    Error = g_pd3dDevice->CreateIndexBuffer( DList.nVertices*3*sizeof(s16), 
                                             D3DUSAGE_WRITEONLY  , 
                                             D3DFMT_INDEX16, 
                                             D3DPOOL_MANAGED,
                                             &pPCData->pIBuffer );
    ASSERT( Error == 0 );


    Error = pPCData->pVBuffer->Lock( 0, 0, (byte**)&pVertex, 0);
    ASSERT( Error == 0 );

    Error = pPCData->pIBuffer->Lock( 0, 0, (byte**)&pIndex, 0);
    ASSERT( Error == 0 );

    for( c = 100, i=0; i<DList.nVertices; i++ )
    {
        f32 u,v;

        u = pUV[i].BaseU;
        v = pUV[i].BaseV;
        u *= (1.0f/(1<<10));
        v *= (1.0f/(1<<10));
        pVertex[i].UV[0].X = u;
        pVertex[i].UV[0].Y = v;

        u = pUV[i].LMU;
        v = pUV[i].LMV;
        u *= (1.0f/(1<<15));
        v *= (1.0f/(1<<15));
        pVertex[i].UV[1].X = u;
        pVertex[i].UV[1].Y = v;

        pVertex[i].Pos.X = pPS2Vertex[i].X;
        pVertex[i].Pos.Y = pPS2Vertex[i].Y;
        pVertex[i].Pos.Z = pPS2Vertex[i].Z;

        c++;
        if( c <= 2 )
        {
            pIndex[m++]  = i;
        }
        else if( *((u32*)&pPS2Vertex[i].W) & (1<<15)  )
        {
            c            = 0;
            pIndex[m++]  = i;
        }
        else
        {
            if( c&1 )
            {
                pIndex[m++]  = i-1;
                pIndex[m++]  = i-2;
                pIndex[m++]  = i;
            }
            else
            {
                pIndex[m++]  = i-2;
                pIndex[m++]  = i-1;
                pIndex[m++]  = i;
            }
        }
    }

    pPCData->pVBuffer->Unlock();
    pPCData->pIBuffer->Unlock();
    pPCData->FacetCount = m/3;

    pPCData->pDList = DList.pData;
    DList.pData     = (byte*)pPCData;

#else
    (void)DList;
#endif
}

//=========================================================================

// check the filename to see if its one we want to skip instance lightmaps for

xbool SkipBuilding( const char *pFileName )
{
    char FileName[64];
    s32 i, len;

    len = x_strlen( pFileName );
    ASSERT( len < 64 );
    x_strcpy( FileName, pFileName );
    
    for( i=0; i<len; i++)
    {
        if( FileName[i] >= 'A' && FileName[i] <= 'Z' )
        {
            FileName[i] += 'a'-'A';
        }
    }

    for( i=0; i<NUM_SKIP_NAMES; i++)
    {
        if( x_strstr( FileName, s_SkipNames[i] ))
        {
            return TRUE;
        }
    }

    return FALSE;
}

//=========================================================================

s32 building::ImportData( X_FILE* Fp, bld_fileio& FileIO, xbool LoadAlarmMaps )
{
    // Back up the list data original offset since is about to get over written
    void* pData = m_pDListData;
    s32 NumInstances = 0;


    // Read the main structure
    FileIO.ReadFields( *this, Fp );

    //
    // Sadly I have to fixup the data pointer of the dlists and collision
    //
    {
        s32 i ;
        for( i=0; i<m_nDLists; i++ )
        {
            m_pDList[i].pData = (byte*)((u32)m_pDListData + (u32)m_pDList[i].pData - (u32)pData);
        }

        for( i=0; i<m_CGrid.m_nNGons; i++ )
        {
            m_CGrid.m_pNGon[i].pVert = (vector4*)((u32)m_pDListData + (u32)m_CGrid.m_pNGon[i].pVert - (u32)pData);
        }
    }
    
    //
    // Read the Texture Names
    //
    material* pMaterialList = (material*)FileIO.ReadChunck( Fp );

    {
        s32          i;    
        bld_manager& Manager = g_BldManager;
        char TextureName[256];
        
        //
        // Load all the textures
        //
        for( i=0; i<m_nMaterials; i++ )
        {
            if( pMaterialList[i].FileName[0] )
            {
                x_sprintf( TextureName, "%s", pMaterialList[i].FileName );
                m_pMaterialHandle[i] = &Manager.LoadBitmap( TextureName, NULL );
            }
        }

        //
        // Load all the lightmaps
        //

        s32 LastNumInstances = 0;

        for( ; i<(m_nLightMaps+m_nMaterials); i++ )
        {
            s32 iLM = i - m_nMaterials;

            xbool LoadBitmap = TRUE;

            // check if we dont want to load alarm maps
             
            if( !LoadAlarmMaps )
            {
                // if the currect lightmap is an alarm map, and the building is using power circuit 0...
                // then to save memory on PS2, we dont need to load it!
            
                #ifdef TARGET_PS2
                
                if( m_pLightMap[iLM].bAlarmMap )
                {
                    LoadBitmap = FALSE;
                }
                
                #endif
            }

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            
            char* pt = pMaterialList[i].FileName + strlen( pMaterialList[i].FileName ) - 8;
            pt[0] = '0';
            pt[1] = '0';

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            
            
            if( pMaterialList[i].FileName[0] )
            {
                NumInstances = 0;
                char FileName[64], Ext[64], Path[64];
                X_FILE* fp;
                
                // check if we can load lightmap instance 0 from the mission directory.
                // if not, then we will load from the old place (data\interiors\ps2tex).
                // later when the level is lit, lightmaps will be saved into the
                // correct mission directory

                x_splitpath( pMaterialList[i].FileName, NULL, NULL, FileName, Ext );
                x_sprintf( Path, "%s\\%s%s", g_BldManager.m_MissionDir, FileName, Ext );

                if (( fp = x_fopen( Path, "rb" )))
                {
                    x_fclose( fp );
                }
                else
                {
                    x_strcpy( Path, pMaterialList[i].FileName );
                }
                
                //
                // determine the type of Environment
                //
                
                switch( FileName[0] )
                {
                    case 'x' : s_Environment = BADLAND; break;
                    case 'p' : s_Environment = DESERT;  break;
                    case 's' : s_Environment = ICE;     break;
                    case 'd' : s_Environment = LAVA;    break;
                    case 'b' : s_Environment = LUSH;    break;
                    default  : DEMAND( 0 );
                };
                
                //
                // Load a lightmap for all instances found
                //
                
                while( 1 )
                {
                    char tmp[8];
                
                    x_sprintf( tmp, "%02d", NumInstances);
                    x_sprintf( TextureName, "%s", Path );

                    char* t = TextureName + strlen( TextureName ) - 8;

                    t[0] = tmp[0];
                    t[1] = tmp[1];

                    #ifdef TARGET_PS2
                    strcpy( TextureName + strlen( TextureName ) - 3, "xbm" );
                    #endif

                    // attempt to open the next instanced lightmap
                    
                    if (( fp = x_fopen( TextureName, "rb" )))
                    {
                        x_fclose( fp );
                        //x_DebugMsg( ">>>>> loading %s <<<<<\n", TextureName );
                
                        const xbitmap* bitmap = &Manager.LoadBitmap( TextureName, &m_pLightMap[iLM].pHandle, LoadBitmap );
                        
                        #ifdef TARGET_PC
                        ASSERT( bitmap->GetBPP() == 32 );
                        #else
                        
                        #if !PALETTED
                        ASSERT( bitmap->GetBPP() == 32 ); 
                        #endif
                        
                        #endif
                        
                        if ( NumInstances == 0 )
                        {
                            // only store a pointer to first instance of the lightmap
                            
                            m_pLightMap[iLM].pLightMap = bitmap;
                        }

                        NumInstances++;
                    }
                    else
                    {
                        // exit the loop when we cant load any more instances
                        
                        break;
                    }

                    // determine whether this building is small enough to skip any more instances (eg. rocks and spires)
                    
                    if( SkipBuilding( TextureName ))
                    {
                        x_DebugMsg( "Skipping any more lightmap instances for this building!\n" );
                        break;
                    }
                }

                // NOTE: if you hit this assert then for some reason the loader has failed to find at least one instance of the lightmap!
                
                DEMAND( NumInstances > 0 );

                // ensure we have the same number of instances for all lightmaps

                if ( LastNumInstances )
                {
                    // NOTE: delete all non-zero lightmap files and retry!
                
                    DEMAND( LastNumInstances == NumInstances );
                }

                LastNumInstances = NumInstances;
                
                #ifdef TARGET_PS2
                #if PALETTED
                ASSERT( m_pLightMap[iLM].pHandle[0] >= 0 );
                #if !ONEPASS
                ASSERT( m_pLightMap[iLM].pHandle[1] >= 0 );
                ASSERT( m_pLightMap[iLM].pHandle[2] >= 0 );
                #endif
                #endif
                #endif
            }
        }

        //
        // Get the global hash ids for the display list
        //
        for( i=0; i<m_nDLists; i++ )
        {
            s32 IndexBase = m_pHashMat[ m_pDList[i].iHash ].iMaterialBase;
            s32 IndexLM   = m_nMaterials + m_pHashMat[ m_pDList[i].iHash ].iMaterialLightMap;

            // register all instances in the hash table

            for (s32 j=0; j<NumInstances; j++)
            {
                s32 IndexHash = g_BldManager.GetHashEntryID( pMaterialList[ IndexBase ].FileName,
                                                             pMaterialList[ IndexLM   ].FileName,
                                                             j );
            
                // Set the global hash id
                if ( j == 0 ) m_pDList[i].iHash = IndexHash;

                // register the alarm lightmap in the hash table
                
                if( m_pDList[i].bAlarm )
                {
                    g_BldManager.GetHashEntryID(  pMaterialList[ IndexBase ].FileName, 
                                                  pMaterialList[ IndexLM   ].FileName, 
                                                  j + NumInstances );
                }
            }
        }
    }
    
    delete []pMaterialList;

    //
    // Compile data for the PC
    //
#ifdef TARGET_PC
    for( s32 i=0; i<m_nDLists; i++ )
    {
        CompilePCData( m_pDList[i] );
    }
#endif

    return(NumInstances);
}

//=========================================================================

void building::ExportData( X_FILE* Fp, material* pMaterialList )
{
    bld_fileio FileIO;

    // Save the header
    FileIO.WriteHeader( BUILDING_VERSION, Fp );

    // Save the class
    FileIO.WriteClass( *this, Fp );

    // Write all the fields of the render class
    ExportData     ( Fp, FileIO, pMaterialList );
}

//=========================================================================

s32 building::ImportData( X_FILE* Fp, xbool LoadAlarmMaps )
{
    bld_fileio FileIO;

    FileIO.ReadHeader( BUILDING_VERSION, Fp );

    FileIO.ReadClass( *this, Fp );

    return (ImportData ( Fp, FileIO, LoadAlarmMaps ));
}

//=========================================================================
inline 
s32 building::IsBoxInView( 
    const bbox&         BBox,
    const plane* const  pClipPlane,
    u32                 SkipPlaneMask  ) const
{
    s32             i;
    s32             PlanesHit       = 0;
    const f32*      pF              = (const f32*)&BBox;
    const s32*      pMinI           = s_ZoneWalk.MinIndex;
    const s32*      pMaxI           = s_ZoneWalk.MaxIndex;
    const plane*    pPlane          = pClipPlane;

    for( i=0; i<6; i++, pMinI += 3, pMaxI += 3, pPlane++ )
    {
        if( SkipPlaneMask & (1<<i) )
            continue;

        // Compute max dist along normal
        f32 MaxDist = pPlane->Normal.X * pF[pMaxI[0]] +
                      pPlane->Normal.Y * pF[pMaxI[1]] +
                      pPlane->Normal.Z * pF[pMaxI[2]] +
                      pPlane->D;

        // If outside plane, we are culled.
        if( MaxDist < 0 )
            return -1;

        // Compute min dist along normal
        f32 MinDist  = pPlane->Normal.X * pF[pMinI[0]] +
                       pPlane->Normal.Y * pF[pMinI[1]] +
                       pPlane->Normal.Z * pF[pMinI[2]] +
                       pPlane->D;

        if(MinDist>=0)
            SkipPlaneMask |= (1<<i);
    }

    // If we have 6 bits set it means that we are completly inside the 
    // culling bounds So don't check for clipping.
    if( SkipPlaneMask == XBIN( 111111 ) ) 
        return 0;

    //
    // Check clipping planes
    //
    for( i=0; i<6; i++, pMinI += 3, pPlane++ )
    {
        if( SkipPlaneMask & (1<<i) )
            continue;

        // Compute min dist along normal
        f32 MinDist = pPlane->Normal.X * pF[pMinI[0]] +
                      pPlane->Normal.Y * pF[pMinI[1]] +
                      pPlane->Normal.Z * pF[pMinI[2]] +
                      pPlane->D;

        // We know that we must be inside or spanning plane
        if(MinDist<0)
            PlanesHit++;
    }

    // All points where inside of all the planes so don't clip
    return PlanesHit;


/*    
    vector3 MINP, MAXP;
    s32     PlanesHit=0;
    u32     SkipPlaneMask=0;
    s32     i;

    ASSERT(nPlanes==6);

    //
    // Check culling planes
    //
    for( i=0; i<6; i++ )
    {
        if(pClipPlane[6*0+i].Normal.X > 0)  { MAXP.X = BBox.Max.X; MINP.X = BBox.Min.X; }
        else                                { MAXP.X = BBox.Min.X; MINP.X = BBox.Max.X; }

        if(pClipPlane[6*0+i].Normal.Y > 0)  { MAXP.Y = BBox.Max.Y; MINP.Y = BBox.Min.Y; }
        else                                { MAXP.Y = BBox.Min.Y; MINP.Y = BBox.Max.Y; }

        if(pClipPlane[6*0+i].Normal.Z > 0)  { MAXP.Z = BBox.Max.Z; MINP.Z = BBox.Min.Z; }
        else                                { MAXP.Z = BBox.Min.Z; MINP.Z = BBox.Max.Z; }


        // If completely outside plane, we are culled
        // don't need to draw this dlist
        f32 MaxDot = pClipPlane[6*0+i].Distance( MAXP );
        if(MaxDot < 0)
            return -1;

        // If completely inside plane we don't need to check clipping plane
        f32 MinDot = pClipPlane[6*0+i].Distance( MINP );
        if(MinDot>=0)
            SkipPlaneMask |= (1<<i);
    }

    // If we have 6 bits set it means that we are completly inside the 
    // culling bounds So don't check for clipping.
    if( SkipPlaneMask == XBIN( 111111 ) ) 
        return 0;
/*
    //
    // Check clipping planes
    //
    PlanesHit = 0;
    for( i=0; i<6; i++ )
    {
        if( SkipPlaneMask & (1<<i) )
            continue;

        if(pClipPlane[6*1+i].Normal.X > 0){ MAXP.X = BBox.Max.X; MINP.X = BBox.Min.X; }
        else                              { MAXP.X = BBox.Min.X; MINP.X = BBox.Max.X; }

        if(pClipPlane[6*1+i].Normal.Y > 0){ MAXP.Y = BBox.Max.Y; MINP.Y = BBox.Min.Y; }
        else                              { MAXP.Y = BBox.Min.Y; MINP.Y = BBox.Max.Y; }

        if(pClipPlane[6*1+i].Normal.Z > 0){ MAXP.Z = BBox.Max.Z; MINP.Z = BBox.Min.Z; }
        else                              { MAXP.Z = BBox.Min.Z; MINP.Z = BBox.Max.Z; }

        f32 MinDot = pClipPlane[6*1+i].Distance( MINP );

        // We know that we must be inside or spanning plane
        if(MinDot<0)
            PlanesHit++;
    }

    // All points where inside of all the planes so don't clip
    return PlanesHit;
  */  
} 


//=========================================================================

xbool SlowRender = TRUE;    // this will render non-clipped display lists by transforming points from L2C, then C2S

void building::PrepRenderZone( s32 iZone, u32 SkipPlaneMask )
{
    const zone& Zone = m_pZone[ iZone ];

#ifdef BLD_TIMERS
    bld_manager::st.BLD_PrepZone.Start();
#endif
    for( s32 i=0; i<Zone.nDLists; i++ )
    {
        dlist& DList = m_pDList[ i + Zone.iDList ];
        s32    Mode  = 0;

        //
        // Check each of the display lists bboxes
        //
        if( s_ZoneWalk.SkipPlaneMask != XBIN( 111111 ) )
        {
            Mode  = IsBoxInView( bbox(DList.BBMin, DList.BBMax), 
                                 s_ZoneWalk.Plane, 
                                 s_ZoneWalk.SkipPlaneMask );

            if( Mode == -1 ) s_ZoneWalk.nRejected++;
            if( Mode ==  0 ) s_ZoneWalk.nNoCliped++;
            if( Mode >   0 ) s_ZoneWalk.nCliped++;
            if( Mode == -1 ) continue;
        }
        else
        {
            s_ZoneWalk.nNoCliped++;
        }

        //
        // Recompute the K every time. TODO: This must be cache
        //
#ifdef TARGET_PS2              //DList.WorldPixelSize*0.75f
        //DList.K = -10;//vram_ComputeMipK( 1, *s_ZoneWalk.pView );
        DList.K = vram_ComputeMipK( DList.WorldPixelSize, *s_ZoneWalk.pView );
#endif

        //
        // Insert the node into the list
        //
        DList.pNext = s_ZoneWalk.pList;
        s_ZoneWalk.pList  = &DList;
        DList.bClip = (Mode > 0);

        // check if clipping is needed for ANY of the display lists
        
        if( SkipPlaneMask != XBIN( 111111 ))
        {
            // have to use the slower L2C then C2S renderer to minimize cracks
        
            if( SlowRender ) DList.bClip |= 0x2;
        }
    }

#ifdef BLD_TIMERS
    bld_manager::st.BLD_PrepZone.Stop();
#endif
}

//=========================================================================

void building::PrepRenderFromZone( 
    portal_clipper&     PortalClipper,
    s32                 iZone,
    u32                 SkipPlaneMask )
{
    xbool   bRenderThisZone;
    u32     iMask = iZone>>5;
    u32     iBit  = 1<<( iZone & ((1<<5)-1) );

    ASSERT( iZone >= 0 );

#ifdef BLD_TIMERS
    bld_manager::st.BLD_Walk.Start();
#endif

    // Have we render this zone already
    bRenderThisZone = ( s_ZoneWalk.RenderZoneMask[ iMask ] & iBit) == 0;
    if( bRenderThisZone ) 
    {
        // We can see this zone so mark it for rendering
#ifdef BLD_TIMERS
        bld_manager::st.BLD_Walk.Stop();
#endif
        PrepRenderZone( iZone, SkipPlaneMask );
#ifdef BLD_TIMERS
        bld_manager::st.BLD_Walk.Start();
#endif

        // Mark that we have visited the zone
        s_ZoneWalk.pRenderInfo->ZonesRendered |= (((u64)1)<<iZone);
    }

    // Set our zone bit
    s_ZoneWalk.RenderZoneMask [ iMask ] |= iBit;
    s_ZoneWalk.VisitedZoneMask[ iMask ] |= iBit;

    for( s32 i=0; i<m_pZone[ iZone ].nPortals; i++ )
    {
        portal_clipper      tPortalClipper;
        portal&             Portal = m_pPortal[ i + m_pZone[ iZone ].iPortal ];

        // If we are curently visiting this zone then we can't redo it again
        if( s_ZoneWalk.VisitedZoneMask[ Portal.iZoneGoingTo>>5 ] & (1<<( Portal.iZoneGoingTo & ((1<<5)-1) )) ) 
        {
            continue;
        }
        
        if( PortalClipper.ClipWithPortal( 
            tPortalClipper, 
            Portal.nPoints, 
            &m_pPortalPoint[ Portal.iPoint ], 
            Portal.Plane ) != 0 )
        {
            // Do recurse walk
#ifdef BLD_TIMERS
            bld_manager::st.BLD_Walk.Stop();
#endif
            PrepRenderFromZone( tPortalClipper, Portal.iZoneGoingTo, SkipPlaneMask );
#ifdef BLD_TIMERS
            bld_manager::st.BLD_Walk.Start();
#endif

            // Collect all the rectagles that leave to zone zero
            if( Portal.iZoneGoingTo == 0 )
            {
                s_ZoneWalk.pRenderInfo->OutPortal.AddRect( tPortalClipper.GetRect() );                
            }
        }
    }

    // Clear the visited zone
    s_ZoneWalk.VisitedZoneMask[ iZone>>5 ] &= ~(1<<( iZone & ((1<<5)-1) ));

#ifdef BLD_TIMERS
    bld_manager::st.BLD_Walk.Stop();
#endif
}

//=========================================================================

building::dlist* building::PrepRender( 
    const matrix4&    L2W, 
    const matrix4&    W2L, 
    const view&       View, 
    prep_render_info& RenderInfo,
    u32               SkipPlaneMask,
    f32               ClipX,
    f32               ClipY )
{
    s32       i;

    // Clear all to zero
    x_memset( &s_ZoneWalk, 0, sizeof(s_ZoneWalk) );

#ifdef BLD_TIMERS
    bld_manager::st.BLD_BuildPlanes.Start();
#endif

    //
    // Setup cull and clipping planes for dlists
    //
    if( SkipPlaneMask != XBIN( 111111 ) )
    {
        // Build the cull planes
        View.GetViewPlanes( s_ZoneWalk.Plane[6*0 + 3], 
                            s_ZoneWalk.Plane[6*0 + 2],
                            s_ZoneWalk.Plane[6*0 + 0],
                            s_ZoneWalk.Plane[6*0 + 1],
                            s_ZoneWalk.Plane[6*0 + 4],
                            s_ZoneWalk.Plane[6*0 + 5],
                            view::WORLD);

        View.GetViewPlanes(-ClipX,
                           -ClipY,
                            ClipX,
                            ClipY,
                            s_ZoneWalk.Plane[6*1 + 3],
                            s_ZoneWalk.Plane[6*1 + 2],
                            s_ZoneWalk.Plane[6*1 + 0],
                            s_ZoneWalk.Plane[6*1 + 1],
                            s_ZoneWalk.Plane[6*1 + 4],
                            s_ZoneWalk.Plane[6*1 + 5],
                            view::WORLD);
        //
        // Take all the planes into the local space of the building
        //
        for( i=0; i<6*2; i++ )
        {
            s_ZoneWalk.Plane[i].Transform( W2L );
            /*
            plane&   Plane = s_ZoneWalk.Plane[i];

            vector3 V;

            V = Plane.Normal * -Plane.D;
            V = W2L * V;

            Plane.Normal.Set(
            (L2W(0,0)*Plane.Normal.X) + (L2W(0,1)*Plane.Normal.Y) + (L2W(0,2)*Plane.Normal.Z),
            (L2W(1,0)*Plane.Normal.X) + (L2W(1,1)*Plane.Normal.Y) + (L2W(1,2)*Plane.Normal.Z),
            (L2W(2,0)*Plane.Normal.X) + (L2W(2,1)*Plane.Normal.Y) + (L2W(2,2)*Plane.Normal.Z) );

            Plane.Normal.Normalize();
            Plane.ComputeD( V );
            */
        }

        //
        // Compute the Min/Max Index lists for this planes
        //
        for( i=0; i<6*2;i++)
        {
            s_ZoneWalk.Plane[i].GetBBoxIndices( &s_ZoneWalk.MinIndex[i*3], 
                                                &s_ZoneWalk.MaxIndex[i*3] );
        }

    }

 
    //
    // Set the view in the walking structure
    //
    s_ZoneWalk.pView          = &View;
    s_ZoneWalk.pRenderInfo    = &RenderInfo;
    s_ZoneWalk.SkipPlaneMask  = SkipPlaneMask;

#ifdef BLD_TIMERS
    bld_manager::st.BLD_BuildPlanes.Stop();
#endif

    //---------------------------------------------------------------------
    // Determine which display lists we are going to use
    //---------------------------------------------------------------------
    if( (m_nPortals > 0) && (m_nZones > 1) )
    {
        s32             iZone;
        vector3         EyeInLocal;
        portal_clipper  PortalClipper;

#ifdef BLD_TIMERS
        bld_manager::st.BLD_FindZone.Start();
#endif

        // Get the eye in the local position of the building
        EyeInLocal = W2L * View.GetPosition();

        // Find which zone we are
        iZone = FindZoneFromPoint( EyeInLocal );

        //x_DebugMsg( "zone = %d\n", iZone );

        // Mark whether we are in zone zero
        s_ZoneWalk.pRenderInfo->bViewInZoneZero = ( iZone == 0 );

#ifdef BLD_TIMERS
        bld_manager::st.BLD_FindZone.Stop();
#endif

        // Initialize the portal cliper
#ifdef BLD_TIMERS
        bld_manager::st.BLD_PortalClipperSetup.Start();
#endif
        PortalClipper.Setup( View, L2W, W2L );
#ifdef BLD_TIMERS
        bld_manager::st.BLD_PortalClipperSetup.Stop();
#endif
        
        // Now start the recusive calls        
#ifdef BLD_TIMERS
        bld_manager::st.BLD_WalkPortals.Start();
#endif
        PrepRenderFromZone( PortalClipper, iZone, SkipPlaneMask );
#ifdef BLD_TIMERS
        bld_manager::st.BLD_WalkPortals.Stop();
#endif
    }
    else
    {
        // We are in zone zero
        s_ZoneWalk.pRenderInfo->bViewInZoneZero = TRUE;

        // Render all display lists
        for( s32 i=0; i<m_nZones; i++ )
        {
            PrepRenderZone( i, SkipPlaneMask );
        }
    }

    //
    // Set the return values
    //
#ifdef BLD_TIMERS
    bld_manager::st.DListnRejected += s_ZoneWalk.nRejected;
    bld_manager::st.DListnNoCliped += s_ZoneWalk.nNoCliped;
    bld_manager::st.DListnCliped   += s_ZoneWalk.nCliped;
#endif

    return s_ZoneWalk.pList;
}

//=========================================================================

s32 building::FindZoneFromPoint( const vector3& Point ) const
{
    const bspnode* pNode = m_pBSPNode;

    do
    {
        plane               Plane       = getBSPPlane( pNode->iPlane );
        f32                 Distance    =  Plane.Distance( Point );
        u16                 iTraverse   = 0;

        if( IsBSPPlaneFlipped( pNode->iPlane ) ) 
        {
            Distance = -Distance;
        }

        if( Distance >= 0 )
            iTraverse = pNode->iFront;
        else
            iTraverse = pNode->iBack;

        // Check if it is a leaf 
        if( isBSPLeafIndex( iTraverse ) )
        {
            // If it is a leaf is it a solid leaf
            if( isBSPSolidLeaf( iTraverse ) )
            {
                return 0;
            } 
            else 
            {
                u16 Zone  = getBSPZone( iTraverse );
                if( Zone == 0x0fff ) 
                    Zone = 0;
                return Zone;
            }
        }

        ASSERT( iTraverse <  m_nBSPNodes );
        pNode = &m_pBSPNode[ iTraverse ];

    } while( true );
}

//=========================================================================

xbool building::IsPointInside( const vector3& Point ) const
{
    const bspnode* pNode = m_pBSPNode;

    do
    {
        plane               Plane       = getBSPPlane( pNode->iPlane );
        f32                 Distance    =  Plane.Distance( Point );
        u16                 iTraverse   = 0;

        if( IsBSPPlaneFlipped( pNode->iPlane ) ) 
        {
            Distance = -Distance;
        }

        if( Distance >= 0 )
            iTraverse = pNode->iFront;
        else
            iTraverse = pNode->iBack;

        // Check if it is a leaf 
        if( isBSPLeafIndex( iTraverse ) )
        {
            // If it is a leaf is it a solid leaf
            if( isBSPSolidLeaf( iTraverse ) )
            {
                return TRUE;
            } 
            else 
            {
                return FALSE;
            }
        }

        ASSERT( iTraverse <  m_nBSPNodes );
        pNode = &m_pBSPNode[ iTraverse ];

    } while( true );
}


//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================
//=========================================================================

xbool building::CastRay(
    const u16      iNode,
    const u16      iPlane,
    const vector3& S,
    const vector3& E,
    raycol&        Info ) const
{

    //
    // Walk the BSP tree
    //
    if( isBSPLeafIndex( iNode ) == false ) 
    {
        const bspnode&  Node    = m_pBSPNode[ iNode ];
        plane           Plane   = getBSPPlane( Node.iPlane );

        if( IsBSPPlaneFlipped(Node.iPlane) ) Plane.Negate();

        f32               SideS       = Plane.Distance( S );
        f32               SideE       = Plane.Distance( E );


        if( (SideS >= 0 && SideE > 0) || (SideS > 0 && SideE >= 0))
        {
            return CastRay( Node.iFront, iPlane, S, E, Info );
        }

        if( (SideS <= 0 && SideE < 0) || (SideS < 0 && SideE <= 0) )
        {
            return CastRay( Node.iBack, iPlane, S, E, Info );
        }

        if( SideS == 0 && SideE == 0 )
        {
            if( isBSPLeafIndex( Node.iBack ) == false) 
            {
                if( CastRay( Node.iBack, iPlane, S, E, Info ) )
                    return TRUE;
            }

            if( isBSPLeafIndex( Node.iFront ) == false ) 
            {
                if( CastRay( Node.iFront, iPlane, S, E, Info ) )
                    return TRUE;
            }

            return FALSE;
        }

        if( SideS >= 0 && SideE < 0 )
        {
            vector3 Point;
            f32     t;

            VERIFY( Plane.Intersect( t, S, E ) );
            Point = S + (t+1) * ( E - S );

            if( CastRay( Node.iFront, iPlane, S, Point, Info ) )
                return TRUE;

            return CastRay( Node.iBack, Node.iPlane, Point, E, Info );
        }

        if( SideS < 0 && SideE >= 0 )
        {
            vector3 Point;
            f32     t;

            VERIFY( Plane.Intersect( t, S, E ) );
            Point = S + (t+1) * ( E - S );
            
            if( CastRay( Node.iBack, iPlane, S, Point, Info ) )
                return TRUE;

            return CastRay( Node.iFront, Node.iPlane, Point, E, Info );
        }
    }

    //
    // We have colided with a poly collect the info and return
    //
    if( isBSPSolidLeaf( iNode ) ) 
    {
        Info.Point = S;

        if( iPlane != 0xffff ) 
        {
            const plane& Plane = getBSPPlane( iPlane );

            Info.Plane = Plane;

            // Make sure that the plane facets our point
            if( Info.Plane.InFront( E ) == false )
                Info.Plane.Negate();

            Info.Plane.Negate();
        } 
        else 
        {
            // Point started in solid;

            if( (S-E).LengthSquared() < 1e-6 )
            {
                Info.Plane.Normal.Zero();
                Info.Plane.D = 0;
                //ASSERTS( 0, "The ray is too cshort and is inside the wall!" );
            }
            else
            {
                Info.Plane.Setup( S, (S-E) );
            }
        }

        return true;
    }

    return false;
}

//=========================================================================

xbool building::CastRay( const vector3& S, const vector3& E, raycol& Info ) const
{
    // DMM: Going to need normal here eventually.
    xbool Hit = CastRay( 0, 0xffff, S, E, Info );

    if( Hit ) 
    {
        vector3 V = E - S;
        f32     L = V.Length();

        if( L < 1e-6 ) 
        {
            Info.T = 0.0f;
        } 
        else 
        {
            Info.T = (Info.Point - S).Dot( (V / L) ) / L;
        }
    }

    return Hit;
}

//==============================================================================

#define LMAP_UV(x) ( ((f32)(x))/(f32)(1<<15) )

struct dlist_uv
{
    s16 TU,TV;
    u16 LU,LV;
};

s32 StripToNGonWithUVs( vector3* pDstVert, vector4* pSrcVert, 
                        vector2* pDstUV,   dlist_uv* pSrcUV,
                        s32 MaxVerts )
{
    
    vector3 OVert[64];
    vector3 EVert[64];
    vector2 OUV  [64];
    vector2 EUV  [64];
    s32 o=0,e=0;

    ASSERT( *((u32*)&pSrcVert[0].W) & (1<<15) );
    ASSERT( *((u32*)&pSrcVert[1].W) & (1<<15) );
    ASSERT( (*((u32*)&pSrcVert[2].W) & (1<<15))==0 );

    //*((u16*)&pUV->U) = (u16)(pVertex[i].vMap[1].X * (1<<15) + 0.5f);
    //*((u16*)&pUV->V) = (u16)(pVertex[i].vMap[1].Y * (1<<15) + 0.5f);

    for( s32 c=0, j=0; j<MaxVerts; j++, c++ )
    {
        if( *((u32*)&pSrcVert[j].W) & (1<<15) )
        {
            if( o )
                break;

            EVert[0].Set( pSrcVert[j+0].X, pSrcVert[j+0].Y, pSrcVert[j+0].Z );
            OVert[0].Set( pSrcVert[j+1].X, pSrcVert[j+1].Y, pSrcVert[j+1].Z );
            EVert[1].Set( pSrcVert[j+2].X, pSrcVert[j+2].Y, pSrcVert[j+2].Z );
            EUV[0].Set( LMAP_UV(pSrcUV[j+0].LU), LMAP_UV(pSrcUV[j+0].LV) );
            OUV[0].Set( LMAP_UV(pSrcUV[j+1].LU), LMAP_UV(pSrcUV[j+1].LV) );
            EUV[1].Set( LMAP_UV(pSrcUV[j+2].LU), LMAP_UV(pSrcUV[j+2].LV) );

            j+=2;
            c = 0;
            o = 1;
            e = 2; 
        }
        else
        {
            if( c&1 )
            {
                OUV[ o ].Set( LMAP_UV(pSrcUV[j-0].LU), LMAP_UV(pSrcUV[j-0].LV) );
                OVert[ o++ ].Set( pSrcVert[j-0].X, pSrcVert[j-0].Y, pSrcVert[j-0].Z );
            }
            else
            {
                EUV[ e ].Set( LMAP_UV(pSrcUV[j-0].LU), LMAP_UV(pSrcUV[j-0].LV) );
                EVert[ e++ ].Set( pSrcVert[j-0].X, pSrcVert[j-0].Y, pSrcVert[j-0].Z );
            }
        }
    }

    //
    // Put face in the colided
    //

    // Copy all the off facets into the even list
    while( --e >= 0 ) 
    {
        OUV  [o  ] = EUV  [e];
        OVert[o++] = EVert[e];
    }

    // Move verts into dest
    for( s32 i=0; i<o; i++ )
    {
        pDstVert[i] = OVert[i];
        pDstUV  [i] = OUV  [i];
    }

    ASSERT( o == MaxVerts );
    return o;
}

//==========================================================================

void ComputeBarys( const vector3& P0, 
                   const vector3& P1, 
                   const vector3& P2, 
                   const vector3& TP, 
                   f32& W0, f32& W1, f32& W2 )
{
//
//          0         
//         /|\        
//        / | \       
//       /  |  \      
//      / 2 | 1 \     
//     /  _-P-_  \
//    / _-     -_ \
//   /_-    0    -_\
//  1---------------2
//
    vector3 Normal;
    f32 NormalLen;

    // Compute normal
    Normal    = v3_Cross((P1-P0),(P2-P0));
    NormalLen = (f32)x_sqrt( Normal.X*Normal.X + Normal.Y*Normal.Y + Normal.Z*Normal.Z );
    NormalLen = 1.0f/NormalLen;
    Normal.X *= NormalLen;
    Normal.Y *= NormalLen;
    Normal.Z *= NormalLen;

    vector3 C;
    f32     D;
    
    C  = v3_Cross(P2-P1,TP-P1);
    D  = v3_Dot(C,Normal);
    W0 = D * NormalLen;

    C  = v3_Cross(P0-P2,TP-P2);
    D  = v3_Dot(C,Normal);
    W1 = D * NormalLen;

    C  = v3_Cross(P1-P0,TP-P0);
    D  = v3_Dot(C,Normal);
    W2 = D * NormalLen;
/*
    // Test weights
    vector3 TestP;
    TestP.X = (f32)((f64)W0*P0.X + (f64)W1*P1.X + (f64)W2*P2.X);
    TestP.Y = (f32)((f64)W0*P0.Y + (f64)W1*P1.Y + (f64)W2*P2.Y);
    TestP.Z = (f32)((f64)W0*P0.Z + (f64)W1*P1.Z + (f64)W2*P2.Z);

    printf("(%f,%f,%f) (%f,%f,%f) (%f,%f,%f) (%f %f %f, %f)\n",
        TP.X,TP.Y,TP.Z,
        TestP.X,TestP.Y,TestP.Z,
        TP.X-TestP.X,TP.Y-TestP.Y,TP.Z-TestP.Z,
        W0,W1,W2,W0+W1+W2);
*/
}

//=========================================================================
volatile xbool LIGHTING_MUNGE = FALSE;

xcolor  building::LookupLighting( const building_grid::ngon& NGon, 
                                  const vector3& Point,
                                  s32   HashAdd,
                                  xbool UseAlarm ) const
{
    // Decide which dlist contains the ngon
#ifdef TARGET_PS2
    s32 D;
    for( D=0; D<m_nDLists; D++ )
    if( (((u32)NGon.pVert) - ((u32)m_pDList[D].pData)) < ((u32)m_pDList[D].SizeData) )
        break;
#endif

#ifdef TARGET_PC
    s32 D;
    for( D=0; D<m_nDLists; D++ )
    {
        byte* PS2Data = (byte*)(((dlist::pc_data*)m_pDList[D].pData)->pDList);
        if( (((u32)NGon.pVert) - ((u32)PS2Data)) < ((u32)m_pDList[D].SizeData) )
            break;
    }
#endif
    
    if( D == m_nDLists ) 
        return XCOLOR_WHITE;

    ASSERT( D!=m_nDLists );

    // Generate vertices and lightmap uvs
    vector3 Vert[64];
    vector2 UV[64];
    dlist_uv* pUV;
    
#ifdef TARGET_PS2
    // Compute UV ptr
    {
        s32 VertIndex = ((u32)NGon.pVert - (u32)(m_pDList[D].pData+32)) / sizeof(vector4);
        byte* pBaseUV = m_pDList[D].pData + 32 + 16 + sizeof(vector4)*m_pDList[D].nVertices;
        pUV = &((dlist_uv*)pBaseUV)[VertIndex];
    }
#endif

#ifdef TARGET_PC
    // Compute UV ptr
    {
        byte* PS2Data = (byte*)(((dlist::pc_data*)m_pDList[D].pData)->pDList);
        s32 VertIndex = ((u32)NGon.pVert - (u32)(PS2Data+32)) / sizeof(vector4);
        byte* pBaseUV = PS2Data + 32 + 16 + sizeof(vector4)*m_pDList[D].nVertices;
        pUV = &((dlist_uv*)pBaseUV)[VertIndex];
    }
#endif

    VERIFY( StripToNGonWithUVs( Vert, NGon.pVert, UV, pUV, NGon.nVerts ) == NGon.nVerts );

    // Decide which triangle the pt is in
    s32 i;
    vector3 P[3];
    vector2 T[3];
    for( i=1; i<NGon.nVerts-1; i++ )
    {
        vector3 Normal;

        P[0] = Vert[0];
        P[1] = Vert[i];
        P[2] = Vert[i+1];

        // Check if pt is in triangle
        Normal = NGon.Plane.Normal.Cross( P[1] - P[0] );
        if( Normal.Dot( Point - P[0] ) < -0.0001f ) continue;
        Normal = NGon.Plane.Normal.Cross( P[2] - P[1] );
        if( Normal.Dot( Point - P[1] ) < -0.0001f ) continue;
        Normal = NGon.Plane.Normal.Cross( P[0] - P[2] );
        if( Normal.Dot( Point - P[2] ) < -0.0001f ) continue;

        // Gather uvs and move on
        T[0] = UV[0];
        T[1] = UV[i];
        T[2] = UV[i+1];
        break;
    }
    ASSERT( i != NGon.nVerts-1 );
    
    // Compute UVs of pt
    f32 W[3];
    ComputeBarys( P[0], P[1], P[2], Point, W[0], W[1], W[2] );
    vector2 PointUV = T[0]*W[0] + T[1]*W[1] + T[2]*W[2];
    //vector3 TestP   = P[0]*W[0] + P[1]*W[1] + P[2]*W[2];

    // Get texture
    //x_printf("W  (%1.3f %1.3f %1.3f)\n",W[0],W[1],W[2]);
    //x_printf("UV (%1.3f,%1.3f)\n",PointUV.X,PointUV.Y);
    //x_printf("P  (%1.3f,%1.3f,%1.3f)\n",Point.X,Point.Y,Point.Z);
    //x_printf("BP (%1.3f,%1.3f,%1.3f)\n",TestP.X,TestP.Y,TestP.Z);
    //x_printf("\n");

    xcolor C;
    if( x_isvalid(PointUV.X) && x_isvalid(PointUV.Y) )
    {
        s32 iHash = m_pDList[D].iHash;
        
        if( m_pDList[D].bAlarm )
        {
            iHash += HashAdd * 2;
            
            if( UseAlarm )
                iHash++;
        }
        else
        {
            iHash += HashAdd;
        }
    
        // Lookup pixel in texture
        xbitmap& BMP = (xbitmap&)g_BldManager.GetLightmap( iHash );
        xcolor   B;
        
        C = BMP.GetBilinearColor( PointUV.X, PointUV.Y, TRUE  );
        B = g_BldManager.GetAverageColor( m_pDList[D].iHash );

        // Touch pixel
        if( LIGHTING_MUNGE )
        {
            s32 PX = (s32)(PointUV.X * BMP.GetWidth());
            s32 PY = (s32)(PointUV.Y * BMP.GetHeight());
            PX = MAX(0,PX);
            PY = MAX(0,PY);
            PX = MIN(BMP.GetWidth(),PX);
            PY = MIN(BMP.GetHeight(),PY);
            BMP.SetClutColor( XCOLOR_BLUE, 0 );
            BMP.SetPixelIndex( 0, PX, PY );
        }

        LookupColors[0] = B;
        LookupColors[1] = C;

        if( m_pDList[D].PassMask == XBIN( 1101 ))
        {
            C.R = C.A;
            C.G = C.A;
            C.B = C.A;
        }
        else
        {
            C.R = (u8)( ( MAX( 0, (s32)( B.R - C.R )) * C.A ) >> 7 );
            C.G = (u8)( ( MAX( 0, (s32)( B.G - C.G )) * C.A ) >> 7 );
            C.B = (u8)( ( MAX( 0, (s32)( B.B - C.B )) * C.A ) >> 7 );
        }
    }
    else
    {
        C.R = 200;
        C.G = 200;
        C.B = 200;
    }
    
    C.A = 255;
    LookupColors[2] = C;
    
    return C;
}

//========================================================================================================================================================
//  DEBUGGING STUFF!
//========================================================================================================================================================

//vector3  Org[16384];
//plane Planes[16384];
s32 nPlanes;

//building::ngon Polys[16384];
s32 nPolys;

int nDLists;

int lines=0;
int normals=0;
int raycast=1;

//vector3 Pts[65536][2];
//int Hit[65536];

s32 nPts=0;

int start=0;
int len=10;
int mod=10;


void draw_normals( const building& Building, matrix4& L2W )
{
#if 0
    s32 n = 0;

    if( lines )
    {
        eng_Begin();

        for( s32 i=0; i<nPts; i++ )
        {
            if (!( i % mod ))
            {
                draw_Begin( DRAW_LINES, 0 );
        
                if (Hit[i])
                {
                    draw_Color( XCOLOR_RED );
                    draw_Vertex( Pts[i][0] );
                    draw_Color( XCOLOR_WHITE );
                    draw_Vertex( Pts[i][1] );
                }
                else
                {
                    draw_Color( XCOLOR_GREEN );
                    draw_Vertex( Pts[i][0] );
                    draw_Color( XCOLOR_BLACK );
                    draw_Vertex( Pts[i][1] );
                }
                
                draw_End();
            }
        }
        eng_End();
    }

    if ( !normals ) return;

    // prepare local-to-world matrix for transforming surface normals
    
    matrix4 M = L2W;
    M.Invert();
    M.Transpose();

    eng_Begin();
    
    for( s32 i=0; i<Building.m_nDLists; i++)
    {
        building::dlist& DList = Building.m_pDList[i];

        if( DList.PassMask != XBIN( 1101 )) continue;

        for( s32 j=0; j<DList.nGons; j++ )
        {
            plane& Plane = Planes[n];
        
            vector3 N =  M  * Plane.Normal;
            vector3 S = L2W * Org[n];
            vector3 E = S + ( N * 1.0f );
        
            draw_Begin( DRAW_LINES, 0 );
            draw_Color( XCOLOR_GREEN );
            draw_Vertex( S );
            draw_Color( XCOLOR_BLACK );
            draw_Vertex( E );
            draw_End();
            
            n++;
        }
    }

    eng_End();
#else
(void)Building;
(void)L2W;
#endif
}

//========================================================================================================================================================
//  Compute nGon information
//========================================================================================================================================================

s32 ComputeNGonInfo(building::ngon* pNGons, const building::dlist& DList)
{
    s32 nVertices = DList.nVertices;
    
    #ifdef TARGET_PS2
    vector4*  pVt = (vector4* )( DList.pData + 32 );        // +32 skips over DMA tag and header
    #else
    byte* PS2Data = (byte*)(((building::dlist::pc_data*)DList.pData)->pDList);
    vector4*  pVt = (vector4* )( PS2Data + 32 );
    #endif
    
    dlist_uv* pUV = (dlist_uv*)( pVt + nVertices  ) + 2;    // + 2 skips over 128-bit UV header
    s32 j=0, k, NumNGons = 0, TotalVerts = 0;
    vector3 Pt[64];
    vector2 UV[64];

    //
    // run through all vertices in display list and gather information for each ngon
    //

    for ( s32 i=0; i<nVertices-1; i++ )
    {
        u32 t0 = *(u32*)&pVt[ i ].W;
        u32 t1 = *(u32*)&pVt[i+1].W;

        // wait until the start of the next ngon by looking at the ADC bit

        if (( t0 & 0x8000 ) && ( t1 & 0x8000 ))
        {
            building::ngon& NGon = pNGons[NumNGons];
            s32 MinU, MinV;
            s32 MaxU, MaxV;
            s32 NumV = 0;
            
            MinU = MinV =  999999;
            MaxU = MaxV = -999999;

            //
            // search forward until the start of the next ngon (or no more vertices)
            //
            
            for ( j=i; j<nVertices; j++ )
            {
                // skip over first 2 vertices which we know have the ADC bit set
                
                if ( j > i+2 )
                {
                    t0 = *(u32*)&pVt[j].W;
                    
                    // loop until the start of a new ngon
                    
                    if ( t0 & 0x8000 )
                    {
                        break;
                    }
                }
                
                // increment number of vertices in this strip
                
                NumV++;
            }

            //
            // convert triangle strip to a triangle fan
            //

            ASSERT( NumV >=  3 );
            ASSERT( NumV <= 64 );
            
            VERIFY( StripToNGonWithUVs( Pt, &pVt[i], UV, &pUV[i], NumV ) == NumV );

            //
            // find the UV bounds of the NGon
            //

            NGon.MinUV.X = NGon.MinUV.Y =  99999.0f;
            NGon.MaxUV.X = NGon.MaxUV.Y = -99999.0f;
            
            for( k=0; k<NumV; k++)
            {
                if( UV[k].X < NGon.MinUV.X ) NGon.MinUV.X = UV[k].X;
                if( UV[k].Y < NGon.MinUV.Y ) NGon.MinUV.Y = UV[k].Y;
                if( UV[k].X > NGon.MaxUV.X ) NGon.MaxUV.X = UV[k].X;
                if( UV[k].Y > NGon.MaxUV.Y ) NGon.MaxUV.Y = UV[k].Y;
            }

            ASSERT( NGon.MinUV.X >= 0.0f);
            ASSERT( NGon.MinUV.Y >= 0.0f);
            ASSERT( NGon.MaxUV.X <= 1.0f);
            ASSERT( NGon.MaxUV.Y <= 1.0f);

            //
            // compute the area of each triangle in the fan and find the biggest
            //
            
            f32 maxa  = 0;
            s32 index = 0;
            
            vector3 normal = vector3( 0, 0, 0 );
            vector3 centre = Pt[0];
            
            for( k=1; k<NumV-1; k++ )
            {
                vector3 v0 = Pt[ k ] - Pt[0];
                vector3 v1 = Pt[k+1] - Pt[0];
                
                vector3 v2 = v0.Cross( v1 );
                f32 area   = v2.Length();
                
                if ( area > maxa )
                {
                    maxa  = area;
                    index = k - 1;
                }
                
                normal += v2;
                centre += Pt[k];
            }

            centre += Pt[k];

            //
            // calculate the face normal, plane and centre of the ngon
            //

            f32 len = normal.Length();

            if ( len < 0.0001f ) len = 0.0001f;
            
            NGon.Centre       = centre / (f32)NumV;
            NGon.Plane.Normal = normal / len;
            NGon.Plane.Normal.Normalize();
            NGon.Plane.D      = -NGon.Plane.Normal.Dot( NGon.Centre );

            //
            // store 3 reference points from the biggest triangle for use with barycentric coordinates
            //

            NGon.Pt[0].X = Pt[   0   ].X;
            NGon.Pt[0].Y = Pt[   0   ].Y;
            NGon.Pt[0].Z = Pt[   0   ].Z;
            NGon.Pt[1].X = Pt[index+1].X;
            NGon.Pt[1].Y = Pt[index+1].Y;
            NGon.Pt[1].Z = Pt[index+1].Z;
            NGon.Pt[2].X = Pt[index+2].X;
            NGon.Pt[2].Y = Pt[index+2].Y;
            NGon.Pt[2].Z = Pt[index+2].Z;
            
            NGon.UV[0].X = UV[   0   ].X;
            NGon.UV[0].Y = UV[   0   ].Y;
            NGon.UV[1].X = UV[index+1].X;
            NGon.UV[1].Y = UV[index+1].Y;
            NGon.UV[2].X = UV[index+2].X;
            NGon.UV[2].Y = UV[index+2].Y;

            NGon.BadUV = FALSE;
            
            f32 E = 0.00001f;

            //
            // check for collapsed UV coordinates
            //

            if( ( ABS( NGon.UV[0].X - NGon.UV[1].X ) < E ) && ( ABS( NGon.UV[0].Y - NGon.UV[1].Y ) < E )) NGon.BadUV = TRUE;
            if( ( ABS( NGon.UV[1].X - NGon.UV[2].X ) < E ) && ( ABS( NGon.UV[1].Y - NGon.UV[2].Y ) < E )) NGon.BadUV = TRUE;
            if( ( ABS( NGon.UV[2].X - NGon.UV[0].X ) < E ) && ( ABS( NGon.UV[2].Y - NGon.UV[0].Y ) < E )) NGon.BadUV = TRUE;
                
            if( NGon.BadUV == TRUE )
            {
                x_DebugMsg( "Collapsed UV's on ngon %d!\n", NumNGons );
            }
            
            // increment running totals
            
            TotalVerts += NumV;
            
            NumNGons++;
            ASSERT(NumNGons <= 256);
        }
    }

    ASSERT( j          == nVertices   );
    ASSERT( TotalVerts == nVertices   );
    ASSERT( NumNGons   == DList.nGons );

    return( NumNGons );
}

//========================================================================================================================================================
//  Cast ray into world and return collision
//========================================================================================================================================================

xbool building::CastRay( const vector3& S, const vector3& E, f32& Dist, vector3& Point ) const
{
    collider::collision Collision;
    collider Collider;
    xbool    Hit;

    Collider.RaySetup( NULL, S, E );
    ObjMgr.Collide( object::ATTR_SOLID_STATIC, Collider );

    Hit   = Collider.GetCollision( Collision );
    Dist  = Collision.T * (E - S).Length();
    Point = Collision.Point;

    return( Hit );
}

//========================================================================================================================================================
//  Calculate Hemisphere Points
//========================================================================================================================================================

#define HEMI_DIV_XY     8
#define HEMI_DIV_XZ     8
#define HEMI_POINTS     ( HEMI_DIV_XY * HEMI_DIV_XZ )

static vector3 s_HemiPts[ HEMI_DIV_XY ][ HEMI_DIV_XZ ];

void ComputeHemiPoints( const vector3& Normal )
{
    vector3 Up   = vector3( 0, 1.0f, 0 );
    vector3 N    = Normal;

    // check if the normal is parallel with the y-axis
    
    if( ABS( Up.Dot( N )) > 0.999f )
    {
        // fix the normal so we are not exactly parallel with the y-axis
        // the lighting does not require exact normals anyway
    
        N.X = 0.001f;
        N.Normalize();
    }
    
    vector3 Axis = Up.Cross( N );
    
    Axis.Normalize();

    f32 CosTheta = Up.Dot( N );
    radian Theta = x_acos( CosTheta );

    // setup a rotation matrix to orientate the hemisphere points by the face normal
    
    matrix4 M;
    M.Setup( Axis, Theta );
    
    for( s32 i=0; i<HEMI_DIV_XY; i++)
    {
        for( s32 j=0; j<HEMI_DIV_XZ; j++)
        {
            vector3& V = s_HemiPts[i][j];
            
            const f32 theta = ( 0.5f * PI / ( HEMI_DIV_XY - 0 )) * i;
            const f32 phi   = ( 2.0f * PI / ( HEMI_DIV_XZ - 0 )) * j;

            // calculate hemisphere points

            V.Y = x_cos( theta );
            V.X = x_sin( theta ) * x_sin( phi );
            V.Z = x_sin( theta ) * x_cos( phi );
        
            V = M * V;
        }
    }
}

//========================================================================================================================================================
//  Light Pixel
//========================================================================================================================================================

#define MIN_LIGHT               0.3f        // the ambient light
#define MAX_LIGHT               1.0f        // max light intensity
#define SHADOW_MINDIST          0.05f       // collisions less than this distance are ignored
#define SHADOW_MINLUM           0.40f       // minimum intensity for the darkest part of the shadow
#define SHADOW_FALLOFF          20.0f       // shadow falls off over this range
#define SHADOW_LINELEN          0.01f       // small adjustment made to ensure the world point is in front of ngon
#define DIFFUSE_RAY_LENGTH      20.0f       // length of rays cast when computing the diffuse component of the lighting

f32 s_Ambient;                              // the ambient light
f32 s_ShadowMin;                            // minimum intensity for the darkest part of the shadow 

xbool Zone0;

s32 building::LightPixel( const vector3& Point, f32 DP )
{
    f32 L, Dist;
    s32 i;
    s32 NumHits = 0;

    //
    // calculate diffuse component based on how much of the sun the pixel can see
    //

    vector3 *pNormal = s_HemiPts[0];

    for( i=0; i<HEMI_POINTS; i++ )
    {
        vector3 P;
    
        if( CastRay( Point, Point + ( pNormal[i] * DIFFUSE_RAY_LENGTH ), Dist, P ))
        {
            NumHits++;
        }
    }
    
    L  = 1.0f - ( (f32)NumHits / HEMI_POINTS );
    L *= L * L;     // cube the diffuse intensity for a more dramatic look
    
    //
    // calculate shadows on ngons front facing to the light direction
    //

    if( DP < 0 )
    {
        vector3 P;
        f32 ShadowK = 0;
        NumHits = 0;
    
        // cast a ray from our point on the ngon towards the light source

        if( CastRay( Point, Point + ( -g_LightDir * SHADOW_FALLOFF * 1.5f ), Dist, P ))
        {
            if( Dist > SHADOW_MINDIST )
            {
                if( Dist <= SHADOW_FALLOFF )
                {
                    f32 K = s_ShadowMin + (( 1.0f - s_ShadowMin ) * ( Dist / SHADOW_FALLOFF ));
                    ShadowK += K;
                    NumHits++;
                }
            }
        }

        if( NumHits )
        {
            L *= ShadowK / NumHits;
        }
    }

    //
    // add in some ambient light and clamp it
    //

    if( Zone0 )
    {
        L += s_Ambient;
    }
    else
    {
        L += 1.0f / 255.0f;
    }

    if( L > MAX_LIGHT ) L = MAX_LIGHT;

    s32 Lum = (s32)x_floor( L * 255.0f );

    ASSERT( Lum );
    return( Lum );
}

//========================================================================================================================================================
//  Get a lightmap pixel 
//========================================================================================================================================================

xbool GetLightmapPixel( s32 x, s32 y, s32 MinU, s32 MinV, s32 MaxU, s32 MaxV, xbitmap& Bitmap, xcolor& Color )
{
    if(( x <  MinU ) ||
       ( y <  MinV ) ||
       ( x >= MaxU ) ||
       ( y >= MaxV ))
    {
        return FALSE;
    }

    Color = Bitmap.GetPixelColor( x, y );
    return TRUE;
}

//========================================================================================================================================================
//  Filter the lightmap to smooth it out
//========================================================================================================================================================

static f32 s_Filter[9] = 
{
    0.7f, 1.0f, 0.7f,
    1.0f, 2.5f, 1.0f,
    0.7f, 1.0f, 0.7f
};

void FilterLightmap( s32 MinU, s32 MinV, s32 MaxU, s32 MaxV, xbitmap& Bitmap, xcolor* pBuffer )
{
    s32 i, j, x, y, sx, sy;
    xcolor C;
    f32 FilterSum = 0, R, G, B, A;
    
    for( i=0; i<9; i++) FilterSum += s_Filter[i];
    
    for( sy=0, y=MinV; y<MaxV; y++, sy++ )
    {
        for( sx=0, x=MinU; x<MaxU; x++, sx++ )
        {
            R = G = B = A = 0;
            
            for( i=-1; i<=1; i++)
            {
                for( j=-1; j<=1; j++)
                {
                    f32 F = s_Filter[ (j+1) + (i+1) * 3 ];
                
                    if( !GetLightmapPixel( x+j, y+i, MinU, MinV, MaxU, MaxV, Bitmap, C ))
                    {
                        VERIFY( GetLightmapPixel( x, y, MinU, MinV, MaxU, MaxV, Bitmap, C ));
                    }
                    
                    R += C.R * F;
                    G += C.G * F;
                    B += C.B * F;
                    A += C.A * F;
                }
            }
            
            R /= FilterSum;
            G /= FilterSum;
            B /= FilterSum;
            A /= FilterSum;
            
            pBuffer[( sy * 256 ) + sx] = xcolor( (s32)R, (s32)G, (s32)B, (s32)A );
        }
    }

    for( sy=0, y=MinV; y<MaxV; y++, sy++ )
    {
        for( sx=0, x=MinU; x<MaxU; x++, sx++ )
        {
            C = pBuffer[( sy * 256 ) + sx];
            Bitmap.SetPixelColor( C, x, y );
        }
    }
}

//========================================================================================================================================================
//
//  Light Building
//
//========================================================================================================================================================

void building::LightBuilding( s32 Instance, const matrix4& L2W )
{
    building::ngon NGons[256];
    f32 FilterSum = 0;
    xcolor Color;

    xbitmap TestMaps[16];
    
    for( s32 b=0; b<m_nLightMaps; b++)
    {
        TestMaps[b] = *m_pLightMap[b].pLightMap;
        x_memset( (void*)TestMaps[b].GetPixelData(), 0, TestMaps[b].GetDataSize() );
    }

    xcolor* pBuffer = (xcolor*)x_malloc( sizeof(xcolor) * 256 * 256 );
    DEMAND( pBuffer );

    // prepare filter

    for( s32 f=0; f<9; f++ ) FilterSum += s_Filter[f];
    
    // prepare local-to-world matrix for transforming surface normals
    
    matrix4 M = L2W;
    M.Invert();
    M.Transpose();

    nPlanes = 0;
    nDLists = 0;

    x_DebugMsg( "Number of Display Lists = %d\n\n", m_nDLists );

    // setup lighting values
    
    s_Ambient   = s_LightingValues[ s_Environment ][0]; 
    s_ShadowMin = s_LightingValues[ s_Environment ][1];

    //
    // process all display lists in building
    //

    for( s32 i=0; i<m_nDLists; i++ )
    {
        s32 j;
    
        if( m_pDList[i].PassMask == XBIN( 1101 ))
        {
            Zone0 = TRUE;
        }
        else
        {
            Zone0 = FALSE;
        }

        ComputeNGonInfo( NGons, m_pDList[i] );

        //
        // light the normal and alarm lightmaps
        //
        for( s32 k=0; k<2; k++ )
        {
            s32 iHash = m_pDList[i].iHash;
            
            if( m_pDList[i].bAlarm )
            {
                iHash += Instance * 2;
            }
            else
            {
                iHash += Instance;
            }
            
            if( k )
            {
                if( m_pDList[i].bAlarm )
                {
                    iHash++;
                }
                else
                {
                    // no alarm map so skip
                
                    continue;
                }
            }

            //
            // get the lightmap page and use it to find the correct testmap
            //

            xbitmap& Bitmap = (xbitmap&)g_BldManager.GetLightmap( iHash );
            ASSERT( Bitmap.GetBPP() == 32 );
        
            for( j=0; j<m_nLightMaps; j++ )
            {
                if( &Bitmap == m_pLightMap[j].pLightMap )
                {
                    break;
                }
            }
            
            DEMAND( j != m_nLightMaps );
            xbitmap& TestMap = TestMaps[j];

            x_DebugMsg( "%3d : Lighting %3d NGons...\n", m_nDLists - i - 1, m_pDList[i].nGons );

            //
            // light each lightmap associated with an ngon
            //

            for( j=0; j<m_pDList[i].nGons; j++ )
            {
                s32 x, y;
                s32 MinU, MinV;
                s32 MaxU, MaxV;
                s32 W = Bitmap.GetWidth();
                s32 H = Bitmap.GetHeight();
                f32 DP;
    
                building::ngon& NGon = NGons[j];
                
                // calculate dot product between ngon face normal and light direction
    
                vector3 V = g_LightDir;
                vector3 N = M * NGon.Plane.Normal;
    
                V.Normalize();
                N.Normalize();

                DP = V.Dot( N );
    
                // calculate the hemisphere points with the local "up" vector aligned with the face normal
                
                ComputeHemiPoints( N );
    
                //Org[nPlanes]      = NGon.Centre;    //DEBUG!
                //Planes[nPlanes++] = NGon.Plane;     //DEBUG! 
                //ASSERT( nPlanes < 16384 );          //DEBUG! 
                //Polys[nPolys++] = NGon;             //DEBUG!
    
                // get integer bounds of lightmap in texel space
    
                //--------------------------------------------------------------------------------------------------------------------------------------------
                //
                // IMPORTANT PS2 NOTE:
                //
                // This code is like this for a reason - to force the FPU on the PS2 to multiply the two values in the correct order.
                // Apparently, mul.s f0,f0,f1 does not equal mul.s f0,f1,f0 - it can be off by one, which will cause the floor function to
                // take the value down to the next lowest integer causing an overlap in the lightmaps.
                // 
                // Also note that this may not even work with later compilers, and/or release builds!
                // A big thanks to Sony for the ghetto style FPU unit.
                //
                //--------------------------------------------------------------------------------------------------------------------------------------------
    
                f32 minu = NGon.MinUV.X * W;
                f32 minv = NGon.MinUV.Y * H;
                f32 maxu = NGon.MaxUV.X * W;
                f32 maxv = NGon.MaxUV.Y * H;
    
                MinU = (s32)x_floor( minu - 0.5f );
                MinV = (s32)x_floor( minv - 0.5f );
                MaxU = (s32) x_ceil( maxu + 0.5f );
                MaxV = (s32) x_ceil( maxv + 0.5f );
    
                //--------------------------------------------------------------------------------------------------------------------------------------------
                
                ASSERT( MinU >= 0 );
                ASSERT( MinV >= 0 );
                ASSERT( MaxU <= Bitmap.GetWidth()  );
                ASSERT( MaxV <= Bitmap.GetHeight() );
                
                // set up triangle points in texel space
    
                vector3 p0 = vector3( NGon.UV[0].X * W, NGon.UV[0].Y * H, 0 );
                vector3 p1 = vector3( NGon.UV[1].X * W, NGon.UV[1].Y * H, 0 );
                vector3 p2 = vector3( NGon.UV[2].X * W, NGon.UV[2].Y * H, 0 );
                
                ASSERT( p0.X >= MinU );
                ASSERT( p0.Y >= MinV );
                ASSERT( p0.X <  MaxU );
                ASSERT( p0.Y <  MaxV );
    
                ASSERT( p1.X >= MinU );
                ASSERT( p1.Y >= MinV );
                ASSERT( p1.X <  MaxU );
                ASSERT( p1.Y <  MaxV );
    
                ASSERT( p2.X >= MinU );
                ASSERT( p2.Y >= MinV );
                ASSERT( p2.X <  MaxU );
                ASSERT( p2.Y <  MaxV );
    
                s32 nPixels  = 0;
                s32 nInvalid = 0;
    
                //
                // process rectangular region containing this ngons lightmap pixels
                //
    
                for( y=MinV; y<MaxV; y++ )
                {
                    for( x=MinU; x<MaxU; x++ )
                    {
                        f32 Lum;
                        s32 Num;
    
                        // keep subdividing pixel until we get a valid pixel (or hit max subdivisions)
                        
                        for( s32 nDiv=1; nDiv < 32; nDiv += nDiv )
                        {
                            // average a number of points evenly spaced within the pixel
                        
                            f32 w0, w1, w2;
                            f32 Add = ( 1.0f / nDiv );
                            f32 Off = Add / 2.0f;
                            
                            Lum = 0;
                            Num = 0;
                            
                            //
                            // attempt to light the sub-pixels
                            //
                            
                            for( f32 t=0; t < 1.0f; t += Add )
                            {
                                for( f32 s=0; s < 1.0f; s += Add )
                                {
                                    vector3 UV = vector3( x+s+Off, y+t+Off, 0 );
                                    vector3 lp;

                                    if( NGon.BadUV )
                                    {
                                        // we dont have valid UV coordinates so just average the 3 points
                                    
                                        w0 = w1 = w2 = 1 / 3.0f;
                                    }
                                    else
                                    {
                                        ComputeBarys( p0, p1, p2, UV, w0, w1, w2 );
                                    }
                        
                                    // use weights to compute a point on the ngon in local space
    
                                    lp = ( NGon.Pt[0] * w0 ) + ( NGon.Pt[1] * w1 ) + ( NGon.Pt[2] * w2 );
    
                                    // ensure point is in front of plane
    
                                    if( NGon.Plane.InBack( lp ))
                                    {
                                        lp += NGon.Plane.Normal * ( ABS( NGon.Plane.Distance( lp )) * 2.0f );
                                    }
    
                                    lp += NGon.Plane.Normal * SHADOW_LINELEN;
                                    
                                    // reject points that are inside the geometry
                                        
                                    if( !IsPointInside( lp ))
                                    {
                                        lp += NGon.Plane.Normal * SHADOW_MINDIST;
                                    
                                        s32 I = LightPixel( L2W * lp, DP );
                                    
                                        Lum += I;
                                        Num += 1;

                                        // NOTE: for speed reasons we are going to break out as soon as we get any valid point
                                        
                                        break;
                                    }
                                }
                            }
                            
                            // we can exit this loop early if we got a valid pixel
                            
                            if( Num ) break;
                        }
    
                        //
                        // set the pixel to the correct colour
                        //
    
                        Color = Bitmap.GetPixelColor( x, y );
                        
                        s32 R = Color.R;
                        s32 G = Color.G;
                        s32 B = Color.B;
                        s32 A = Color.A;
    
                        // did this pixel get lit?
                    
                        if( Num )
                        {
                            Lum /= Num;
                            
                            // if the pixel has not been lit then add the light value into the lightmap and clamp it
    
                            Color = TestMap.GetPixelColor( x, y );
                            
                            if( Color.A == 0 )
                            {
                                TestMap.SetPixelColor( XCOLOR_WHITE, x, y );
                                
                                A = MIN( A + (s32)Lum, 255 );
                                
                                // this causes wrong lighting on the bounds between zone0 and non-zone0 
                                
                                //if( Zone0 )
                                //{
                                //    R = MIN( R + (s32)Lum, 255 );
                                //    G = MIN( G + (s32)Lum, 255 );
                                //    B = MIN( B + (s32)Lum, 255 );
                                //}
                            }
                        }
                        else
                        {
                            // the pixel did not get lit so we will need to generate a value from its neighbours
                        
                            R = 255;
                            G = 0;
                            B = 0;
                            A = 0;
                            
                            nInvalid++;
                        }
                        
                        Bitmap.SetPixelColor( xcolor( R, G, B, A ), x, y );
                        nPixels++;
                    }
                }
    
                //
                // if ALL the pixels in the lightmap could not be lit then it is invalid
                //
    
                if( nPixels == nInvalid )
                {
                    x_DebugMsg( "Warning: Invalid lightmap!\n" );
                    for( y=MinV; y<MaxV; y++ )
                    {
                        for( x=MinU; x<MaxU; x++ )
                        {
                            Bitmap.SetPixelColor( xcolor( 0, 0, 0, 0 ), x, y );
                        }
                    }
                    
                    // skip over this invalid lightmap
                    
                    continue;
                }
                
                //
                // replace any invalid pixels with the average of its neighbours
                //
    
                if( nInvalid )
                {
                    xbool  InvalidPixels;
                    
                    s32 nPasses = 0;
    
                    do
                    {
                        InvalidPixels = FALSE;
                
                        for( y=MinV; y<MaxV; y++ )
                        {
                            for( x=MinU; x<MaxU; x++ )
                            {
                                Color = Bitmap.GetPixelColor( x, y );
        
                                if( Color.A == 0 )
                                {
                                    f32 R=0, G=0, B=0, A=0;
                                    s32 N=0;
                                    xcolor C;
                                
                                    InvalidPixels = TRUE;
                                    
                                    //
                                    // find the average of the neighbours using a 3x3 grid centred on the pixel
                                    //
                                    
                                    for( s32 ty=-1; ty<=1; ty++ )
                                    {
                                        for( s32 tx=-1; tx<=1; tx++ )
                                        {
                                            if( !GetLightmapPixel( x+tx, y+ty, MinU, MinV, MaxU, MaxV, Bitmap, C ))
                                            {
                                                VERIFY( GetLightmapPixel( x, y, MinU, MinV, MaxU, MaxV, Bitmap, C ));
                                            }
                                            
                                            if( C.A )
                                            {
                                                R += C.R;
                                                G += C.G;
                                                B += C.B;
                                                A += C.A;
                                                N += 1;
                                            }
                                        }
                                    }
                                    
                                    if( N )
                                    {
                                        R = MIN( R / N, 255.0f );
                                        G = MIN( G / N, 255.0f );
                                        B = MIN( B / N, 255.0f );
                                        A = MIN( A / N, 255.0f );
                                        A = MAX( A, 1.0f );
                                    
                                        Bitmap.SetPixelColor( xcolor( (s32)R, (s32)G, (s32)B, (s32)A ), x, y );
                                    }
                                }
                            }
                        }
    
                        // just in case we get a bad lightmap UV bounds - we can skip over it eventually
                        
                        nPasses++;
                        if( nPasses > nPixels )
                        {
                            x_DebugMsg( "Bad Lightmap!  DList=%d NGon=%d\n", i, j );
                            break;
                        }
                    }
                    while( InvalidPixels );
                }
    
                // smooth out the lightmap by filtering it
    
                if( Zone0 )
                {
                    FilterLightmap( MinU, MinV, MaxU, MaxV, Bitmap, pBuffer );
                }
            }
        }
    }
    
    x_free( pBuffer );
}

//=========================================================================

struct dbg_portal
{
    s32   Zone;
    s32   Portal;
    xbool UseZBuffer;
    xbool ScaledPortals;
};

dbg_portal g_Portal =
{
    -1,
    -1,
    TRUE,
    TRUE,
};

void building::DrawPortal( const portal& Portal, const vector3* pPortalPoint, xcolor Color )
{
    vector3 Centre = vector3( 0, 0, 0 );
    vector3 Points[64];
    
    for( s32 n=0; n<Portal.nPoints; n++ )
    {
        Points[n] = pPortalPoint[ Portal.iPoint + n ];
        Centre += Points[n];
    }

    Centre /= (f32)Portal.nPoints;    

    if( g_Portal.ScaledPortals )
    {
        for( s32 n=0; n<Portal.nPoints; n++ )
        {
            vector3 Pos;
            float   l;
            
            Pos  = Points[n] - Centre;
            l    = Pos.Length();
            Pos /= l;
            l   += l * 0.05f;   // make the portal 5% bigger
            Pos *= l;
            
            Points[n] = Centre + Pos;
        }
    }
    
    s32 flag = g_Portal.UseZBuffer ? 0 : DRAW_NO_ZBUFFER;
    
    draw_Begin( DRAW_TRIANGLES, DRAW_USE_ALPHA | flag );
    draw_Color( Color );

    for( s32 j=0; j<Portal.nPoints-2; j++ )
    {
        draw_Vertex( Points[  0  ] );
        draw_Vertex( Points[ j+1 ] );
        draw_Vertex( Points[ j+2 ] );
    }

    draw_End();
    draw_Line( Centre, Centre + Portal.Plane.Normal );
}

//=========================================================================

static s32 s_Delay = 0;

void building::RenderPortals( const matrix4& L2W )
{
    s_Delay++;
    if( s_Delay == 60 )
    {
        const view* pView = eng_GetActiveView( 0 );
        
        matrix4 W2L = L2W;
        W2L.Invert();
        
        s32 Zone = FindZoneFromPoint( W2L * pView->GetPosition() );
    
        x_DebugMsg( "Zone=%d\n", Zone );
        s_Delay = 0;
    }

    draw_SetL2W( L2W );

    if( g_Portal.Zone < 0 )
    {
        for( s32 i=0; i<m_nZones; i++ )
        {
            building::zone& Zone = m_pZone[ i ];
    
            for( s32 j=0; j<Zone.nPortals; j++ )
            {
                xcolor Color = xcolor( 255, 0, 0, 32);
            
                if( j == g_Portal.Portal )
                {
                    Color = xcolor( 0, 255, 0, 32 );
                }

                DrawPortal( m_pPortal[ Zone.iPortal + j ], m_pPortalPoint, Color );
            }
        }
    }
    else
    {
        if( g_Portal.Zone > m_nZones - 1 ) g_Portal.Zone = m_nZones - 1;
        
        building::zone& Zone = m_pZone[ g_Portal.Zone ];

        for( s32 j=0; j<Zone.nPortals; j++ )
        {
            xcolor Color = xcolor( 255, 0, 0, 32);
        
            if( j == g_Portal.Portal )
            {
                Color = xcolor( 0, 255, 0, 32 );
            }
    
            DrawPortal( m_pPortal[ Zone.iPortal + j ], m_pPortalPoint, Color );
        }
    }

    draw_ClearL2W();
}

//=========================================================================

void RenderLookupColors( void )
{
    rect Base( vector2(  20,  60 ), vector2(  70, 110 ));
    rect LMap( vector2( 100,  60 ), vector2( 150, 110 ));
    rect LCol( vector2( 200,  60 ), vector2( 250, 110 ));

    LookupColors[0].A = 255;
    LookupColors[1].A = 255;
    LookupColors[2].A = 255;

    draw_Rect( Base, LookupColors[0], FALSE );
    draw_Rect( LMap, LookupColors[1], FALSE );
    draw_Rect( LCol, LookupColors[2], FALSE );

    draw_Rect( Base, XCOLOR_WHITE, TRUE );
    draw_Rect( LMap, XCOLOR_WHITE, TRUE );
    draw_Rect( LCol, XCOLOR_WHITE, TRUE );
}

//=========================================================================

