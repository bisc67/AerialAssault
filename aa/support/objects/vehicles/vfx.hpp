//==============================================================================
//
//  Vfx.hpp
//
//==============================================================================

//==============================================================================
//  DEFINES
//==============================================================================
#ifndef __VFX_HPP__
#define __VFX_HPP__


#define MAX_GEOMS   23             // max. # of geometric shapes per effect

//==============================================================================
//  TYPES
//==============================================================================
class vfx_geom
{

public:
//------------------------------------------------------------------------------
//  Public Types
//------------------------------------------------------------------------------
    enum type
    {
        GLOWY_SPOT =        2,      // per the values from the export
        EXTRUSION_START =   3,
        EXTRUSION_END =     4,
        GLOWY_PT_SPRITE =   5
    };

    struct face
    {
        s32         A, B, C;        // face indices
    
    };

    struct geom
    {
        type        m_Type;
        char        ID[16];         // Identifier
        
        vector3*    m_pVerts;       // ptr to raw vertex data
        s32         m_nVerts;

        vector2*    m_pTVerts;      // ptr to texture vertices
        s32         m_nTVerts;

        face*       m_pFaces;       // ptr to face indices
        face*       m_pTFaces;      // ptr to face indices for texture coords
        s32         m_nFaces;
    };

public:
//------------------------------------------------------------------------------
//  Public Functions
//------------------------------------------------------------------------------
    // ctor
    vfx_geom                ( );
                            
    // dtor                 
   ~vfx_geom                ( );
                            
   // initialization        
   void CreateFromFile      ( const char* pPath );
                            
   // database              
   s32  FindGeomByID        ( const char* pID );

   // rendering
   void RenderGlowyEffect   ( const matrix4& L2W, s32 ID, f32 Alpha );
   
   void RenderExtrusion     ( const matrix4& L2W, 
                              s32 ID_Start,
                              s32 ID_End, 
                              f32 Parametric, 
                              f32 AlphaStart, 
                              f32 AlphaEnd      );
   
   void RenderGlowyPtSpr    ( const matrix4& L2W, s32 ID, f32 Alpha );
                              

protected:
//------------------------------------------------------------------------------
//  Protected Data
//------------------------------------------------------------------------------
    byte*           m_pData;
    geom*           m_pGeom[ MAX_GEOMS ];       // geometry data

};






#endif