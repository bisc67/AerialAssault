#include "Clipper.hpp"



//=========================================================================
// Defines
//=========================================================================

#define MAX_VERTS   40



//=========================================================================
// Structures
//=========================================================================

struct vu_vert
{
    u32         Flags ;     // Clip flag
    vector4     HomoPos ;   // Position in homogenous co-ords
    vector3     Pos ;       // Position
    vector2     Tex ;       // Tex co-ords
    vector3     Col ;       // Color
} ;


//=========================================================================
// Data
//=========================================================================

vu_vert  s_VertBuffer[2][MAX_VERTS] ;
s32      s_DestBuffer=0 ;

s32      s_TrisProcessed=0 ;
s32      s_TrisDrawn= 0 ;

s32      s_VertsProcessed=0 ;
s32      s_VertsDrawn=0 ;


//=========================================================================
// Functions
//=========================================================================

void ClipVertsToPlane(vu_vert*     SourceVerts,
                      s32          nSourceVerts,
                      vu_vert*     DestVerts,
                      s32&         nDestVerts,
                      const plane* Plane,
                      const u32    ClipFlag)
{
    s32 v ;

    // Clear dest buffer
    nDestVerts = 0 ;

    // Tri completely rejected?
    if (nSourceVerts < 3)
        return ;

    // Setup clip flags
    for (v = 0 ; v < nSourceVerts ; v++)
    {
        vu_vert& Vert = SourceVerts[v] ;


        xbool Out = Plane->InBack(SourceVerts[v].Pos) ;
        //if (Out)
            //Vert.Flags |= ClipFlag ;

        //  0=Left  1=Right  2=Bottom  3=Top  4=Near  5=Far
        xbool HomoOut = FALSE ;
        switch(ClipFlag)
        {
            // Left
            case (1<<0):
                HomoOut = Vert.HomoPos.X < -Vert.HomoPos.W ;
                break ;

            // Right
            case (1<<1):
                HomoOut = Vert.HomoPos.X > Vert.HomoPos.W ;
                break ;

            // Bottom
            case (1<<2):
                HomoOut = Vert.HomoPos.Y < -Vert.HomoPos.W ;
                break ;

            // Top
            case (1<<3):
                HomoOut = Vert.HomoPos.Y > Vert.HomoPos.W ;
                break ;

            // Near
            case (1<<4):
                HomoOut = Vert.HomoPos.Z < -Vert.HomoPos.W ;
                break ;

            // Far
            case (1<<5):
                HomoOut = Vert.HomoPos.Z > Vert.HomoPos.W ;
                break ;
        }

        if (HomoOut)
            Vert.Flags |= ClipFlag ;

        ASSERT(Out == HomoOut) ;


    }

    // Get 1st edge
    vu_vert* Vert0 = &SourceVerts[nSourceVerts-1] ;
    vu_vert* Vert1 = &SourceVerts[0] ;

    // Copy verts to dest buffer, creating new verts when needed
    for (v = 0 ; v < nSourceVerts ; v++)
    {
        // If vert0 is not clipped, copy it to the dest buffer!
        if (!(Vert0->Flags & ClipFlag))
            DestVerts[nDestVerts++] = *Vert0 ;

        // Create a new clip vert?
        if ((Vert0->Flags ^ Vert1->Flags))
        {
            // Get intersection value
            f32 t ;
            VERIFY(Plane->Intersect(t, Vert0->Pos, Vert1->Pos)) ;

            f32 HomoT = 0 ;

            switch(ClipFlag)
            {
                // TO DO - Setup a W plane sign

                // Left
                case (1<<0):
                    //HomoT = (Vert0->HomoPos.X + Vert0->HomoPos.W) / (-Vert1->HomoPos.W + Vert0->HomoPos.W - Vert1->HomoPos.X + Vert0->HomoPos.X);
                    HomoT = (Vert0->HomoPos.X + Vert0->HomoPos.W) / (Vert0->HomoPos.X + Vert0->HomoPos.W - Vert1->HomoPos.W - Vert1->HomoPos.X) ;
                    break ;

                // Right
                case (1<<1):
                    //HomoT = (Vert0->HomoPos.X - Vert0->HomoPos.W) / (Vert1->HomoPos.W - Vert0->HomoPos.W - Vert1->HomoPos.X + Vert0->HomoPos.X);
                    HomoT = (Vert0->HomoPos.X - Vert0->HomoPos.W) / (Vert0->HomoPos.X - Vert0->HomoPos.W + Vert1->HomoPos.W - Vert1->HomoPos.X) ;
                    break ;

                // Bottom
                case (1<<2):
                    HomoT = (Vert0->HomoPos.Y + Vert0->HomoPos.W) / (-Vert1->HomoPos.W + Vert0->HomoPos.W - Vert1->HomoPos.Y + Vert0->HomoPos.Y);
                    break ;

                // Top
                case (1<<3):
                    HomoT = (Vert0->HomoPos.Y - Vert0->HomoPos.W) / (Vert1->HomoPos.W - Vert0->HomoPos.W - Vert1->HomoPos.Y + Vert0->HomoPos.Y);
                    break ;

                // Near
                case (1<<4):
                    HomoT = (Vert0->HomoPos.Z - Vert0->HomoPos.W) / (Vert1->HomoPos.W - Vert0->HomoPos.W - Vert1->HomoPos.Z + Vert0->HomoPos.Z);
                    break ;

                // Far
                case (1<<5):
                    HomoT = (Vert0->HomoPos.Z + Vert0->HomoPos.W) / (-Vert1->HomoPos.W + Vert0->HomoPos.W - Vert1->HomoPos.Z + Vert0->HomoPos.Z);
                    break ;
            }

            ASSERT(ABS(t - HomoT) < 0.001f) ;

          //O[0] = I[s]   + (I[e]   - I[s])*t;
          //O[1] = I[s+1] + (I[e+1] - I[s+1])*t;
          //O[2] = I[s+2] + (I[e+2] - I[s+2])*t;
          //O[3] = I[s+3] + (I[e+3] - I[s+3])*t;

            t = HomoT ;

            // Create a new vert
            vu_vert NewVert ;
            NewVert.Flags = 0 ;
            NewVert.Pos     = Vert0->Pos     + ((Vert1->Pos     - Vert0->Pos)       * t) ;
            NewVert.HomoPos = Vert0->HomoPos + ((Vert1->HomoPos - Vert0->HomoPos)   * t) ;
            NewVert.Tex     = Vert0->Tex     + ((Vert1->Tex     - Vert0->Tex)       * t) ;
            NewVert.Col     = Vert0->Col     + ((Vert1->Col     - Vert0->Col)       * t) ;

            // Add to dest verts
            DestVerts[nDestVerts++] = NewVert ;
        }

        // Next edge
        Vert0 = Vert1 ;
        Vert1++ ;
    }
}

//=========================================================================

void DrawClippedTri(vu_vert      Verts[3], 
                    const plane* Planes,
                    s32          nPlanes)
{
    s32 i ;

    // Update stats
    s_TrisProcessed++ ;

    // Update stats
    s_VertsProcessed += 3 ;

    // Setup source verts
    vu_vert*    SourceVerts  = Verts ;
    s32         nSourceVerts = 3 ;

    // Setup dest buffer
    vu_vert*    DestVerts    = s_VertBuffer[s_DestBuffer] ;
    s32         nDestVerts = 0 ;

    // Clip verts with all planes
    for (i = 0 ; i < nPlanes ; i++)
    {
        // Clip verts to plane
        ClipVertsToPlane(SourceVerts,
                         nSourceVerts,
                         DestVerts,
                         nDestVerts,
                         &Planes[i],
                         (1<<i)) ;

        // Toggle buffers
        SourceVerts  = DestVerts ;
        nSourceVerts = nDestVerts ;

        s_DestBuffer ^= 1 ;
        DestVerts  = s_VertBuffer[s_DestBuffer] ;
    }

    // Update stats
    s_VertsDrawn += nSourceVerts ;

    // Draw the clipped triangles
    if (nSourceVerts >= 3)
    {
        // Emulate drawing a fan
        vu_vert* Vert0 = &SourceVerts[0] ;
        vu_vert* Vert1 = &SourceVerts[1] ;
        vu_vert* Vert2 = &SourceVerts[2] ;

        for (i = 0 ; i < (nSourceVerts-2) ; i++)
        {
            draw_Color  (Vert0->Col.X, Vert0->Col.Y, Vert0->Col.Z) ;
            draw_UV     (Vert0->Tex.X, Vert0->Tex.Y) ;
            draw_Vertex (Vert0->Pos.X, Vert0->Pos.Y, Vert0->Pos.Z) ;

            draw_Color  (Vert1->Col.X, Vert1->Col.Y, Vert1->Col.Z) ;
            draw_UV     (Vert1->Tex.X, Vert1->Tex.Y) ;
            draw_Vertex (Vert1->Pos.X, Vert1->Pos.Y, Vert1->Pos.Z) ;

            draw_Color  (Vert2->Col.X, Vert2->Col.Y, Vert2->Col.Z) ;
            draw_UV     (Vert2->Tex.X, Vert2->Tex.Y) ;
            draw_Vertex (Vert2->Pos.X, Vert2->Pos.Y, Vert2->Pos.Z) ;

            // Next tri in fan
            Vert1 = Vert2 ;
            Vert2++ ;

            // Update stats
            s_TrisDrawn++ ;
        }
    }
}

//=========================================================================

void DrawClippedInstance(shape_instance* Instance)
{
    // Clear counts
    s_TrisProcessed=0 ;
    s_TrisDrawn= 0 ;

    s_VertsProcessed=0 ;
    s_VertsDrawn=0 ;


    #ifdef TARGET_PC
	    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ) ;
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE) ;
        g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE) ;
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ;
	    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ; 
    #endif

    draw_ClearL2W() ;

    // Calc matrices from anim
	render_node* RenderNodes = Instance->CalculateRenderNodes() ;
    ASSERT(RenderNodes) ;

    // Get view and planes
    const view* View = eng_GetView() ;
    ASSERT(View) ;

    vector3 Pos = vector3(5,0,10) ;
    Pos = View->ConvertV2W(Pos) ;

    // Setup normal view
    view MyView ;
    static f32 Rot = R_45 ;

    MyView.SetZLimits(0.001f, 100.0f) ;
    MyView.SetXFOV(R_60) ;
    MyView.SetPosition(Pos) ;
    MyView.SetRotation(radian3(0,Rot,0)) ;
    //Rot += R_1 ;

    //MyView = *View ;
    draw_Frustum( MyView, XCOLOR_WHITE, 100.0f) ;

    // Get view planes
    const plane* ViewPlanes = MyView.GetViewPlanes( view::WORLD ) ;
    ASSERT(ViewPlanes) ;

    // Get model to display
    model* Model = Instance->GetModel() ;
    ASSERT(Model) ;

    //draw_Begin(DRAW_TRIANGLES, DRAW_TEXTURED) ;
    draw_Begin(DRAW_TRIANGLES, 0) ;

    // Loop through all materials in the model
    for (s32 m = 0 ; m < Model->GetMaterialCount() ; m++)
    {
        material& Mat = Model->GetMaterial(m) ;

        // Loop through all display lists in material
        for (s32 dl = 0 ; dl < Mat.GetDListCount() ; dl++)
        {
            material_dlist& DList = Mat.GetDList(dl) ;

            // Get local to world matrix for this dlist
            matrix4 L2W = RenderNodes[DList.GetNode()].L2W ;

            // Loop through all strips in dlist
            for (s32 s = 0 ; s < DList.GetStripCount() ; s++)
            {
                dlist_strip& Strip = DList.GetStrip(s) ;

                // Loop through all verts in strip
                ASSERT(Strip.GetVertCount() >= 3) ;
                for (s32 v = 0 ; v < Strip.GetVertCount()-2 ; v++)
                {
                    // Setup tri
                    vu_vert Verts[3] ;
                    for (s32 t = 0 ; t < 3 ; t++)
                    {
                        vu_vert& Vert = Verts[t] ;

                        // Lookup vert
                        Strip.GetVert(v+t, Vert.Pos, Vert.Col, Vert.Tex) ;

                        // Clear clip flags
                        Vert.Flags = 0 ;

                        // Put into world space
                        Vert.Pos = L2W * Vert.Pos ;

                        // Convert normal to a valid color
                        Vert.Col = vector3(0.5f, 0.5f, 0.5f) + (Vert.Col * 0.5f) ;

                        // Calc homo pos in clip space
                        vector4 Pos = vector4(Vert.Pos.X, Vert.Pos.Y, Vert.Pos.Z, 1.0f) ;
                        Vert.HomoPos = MyView.GetW2C() * Pos ;
                    }

                    // Render clipped tri!
                    DrawClippedTri(Verts, ViewPlanes, 6) ;
                }
            }
        }
    }

    draw_End() ;

    // Show stats
    x_printfxy(2,15, "TrisProcessed:%d TrisDrawn:%d VertsProcessed:%d VertsDrawn:%d",
                     s_TrisProcessed, s_TrisDrawn, s_VertsProcessed, s_VertsDrawn) ;

}

//=========================================================================
