#include "entropy.hpp"
#include "ParticleEmitter.hpp"
#include "ParticleEffect.hpp"
#include "Shape\ShapeInstance.hpp"
#include "..\Demo1\Globals.hpp"
#include "LabelSets/Tribes2Types.hpp"
#include "Poly\Poly.hpp"

#ifdef TARGET_PS2
xbool pfx_UseDraw = FALSE;
#else
xbool pfx_UseDraw = TRUE;
#endif

xbool pfx_UseAlpha = TRUE;

#define MAX_VERTS               144      // MUST be divisible by 6

static vector2 s_UVS[6] =
{
    vector2( 1.0f, 0.0f ),
    vector2( 0.0f, 0.0f ),
    vector2( 0.0f, 1.0f ),
    vector2( 1.0f, 0.0f ),
    vector2( 0.0f, 1.0f ),
    vector2( 1.0f, 1.0f ),
};

extern xcolor g_PfxFogColor;

//=============================================================================
// The reusable shape instance
//=============================================================================

shape_instance  pfx_emitter::m_Instance;
radian3         pfx_emitter::m_ViewRot;

//=============================================================================
//  Random Helper Functions
//=============================================================================

static f32 RandVar( f32 Nominal, f32 Variance )
{
    return ((f32)x_rand()/X_RAND_MAX) * (Variance * 2.0f) - Variance + Nominal;
}

//=============================================================================
//  Particle emitter
//=============================================================================

pfx_emitter::pfx_emitter( void )
{
    m_Created           = FALSE;
    m_pParticles        = NULL;
    m_ActiveParticles   = 0;
    m_Life              = 0.0f;
    m_Age               = 0.0f;
    m_Rotation          = 0.0f;    

    m_UseAxis           = FALSE;
    m_MaxAlpha          = 1.0f;

    m_Velocity.Set    ( 0.0f, 0.0f, 0.0f );
    m_Acceleration.Set( 0.0f, 0.0f, 0.0f );
}

//=============================================================================

void pfx_emitter::Reset( void )
{
    m_Created           = FALSE;
    m_pParticles        = NULL;
    m_ActiveParticles   = 0;
    m_Life              = 0.0f;
    m_Age               = 0.0f;
    m_Rotation          = 0.0f;    

    m_UseAxis = FALSE;

    m_Velocity.Set    ( 0.0f, 0.0f, 0.0f );
    m_Acceleration.Set( 0.0f, 0.0f, 0.0f );
}

//=============================================================================

pfx_emitter::~pfx_emitter( void )
{
    // Free particles
    if( m_pParticles )
    {
        m_pParticlePool->FreeParticles( m_pParticles, m_MaxParticles );
    }
}

//=============================================================================

xbool pfx_emitter::Create( pfx_effect*          pEffect,
                           pfx_pool*            pParticlePool,
                           pfx_library*         pParticleLibrary,
                           pfx_emitter_def*     pEmitterDef,
                           pfx_particle_def*    pParticleDef,
                           const vector3&       Position )
{
    xbool   Success = TRUE;

    ASSERT( !m_Created );
    ASSERT( pEffect );
    ASSERT( pParticlePool );
    ASSERT( pEmitterDef );
    ASSERT( pParticleDef );
    ASSERT( pParticleDef->nKeys > 0 );

    // Init parameters
    m_pEffect          = pEffect;
    m_pParticlePool    = pParticlePool;
    m_pParticleLibrary = pParticleLibrary;
    m_pEmitterDef      = pEmitterDef;
    m_pParticleDef     = pParticleDef;
    m_Position         = Position;

    // Precalc some stuff for rotating emitter axis
    vector3 FXN = pEffect->GetNormal();
    if ( ( FXN.X == 0.0f ) &&
         ( FXN.Y == 0.0f ) &&
         ( FXN.Z == 0.0f ) )
    {
        FXN.Set(0,1,0);
    }

    plane   P;
    P.Setup( Position, FXN );
    P.GetOrthoVectors( m_A, m_B );

    // Allocate particles if needed
    m_MaxParticles     = m_pEmitterDef->MaxParticles;
    if( m_MaxParticles > 0 )
    {
        m_pParticles       = m_pParticlePool->AllocateParticles( m_MaxParticles ) ;
        m_ActiveParticles  = 0;

        // Clear particles
        if( m_MaxParticles > 0 )
            x_memset( m_pParticles, 0, sizeof(pfx_particle)*m_MaxParticles );

        //ASSERT( m_MaxParticles > 0 );
    }

    // Init times
    m_Age              = 0.0f;
    m_NextEmissionTime = RandVar( m_pEmitterDef->EmitDelay, m_pEmitterDef->EmitDelayVariance   );
    m_Life             = RandVar( m_pEmitterDef->Life,      m_pEmitterDef->LifeVariance        );

    // Set created flag
    m_Created          = TRUE;

    // Return success code
    return Success;
}

//=============================================================================

const vector3& pfx_emitter::GetPosition( void ) const
{
    return m_Position;
}

void pfx_emitter::SetPosition( const vector3& Position )
{
    m_Position = Position;
}

//=============================================================================

const vector3& pfx_emitter::GetVelocity( void ) const
{
    return m_Velocity;
}

void pfx_emitter::SetVelocity( const vector3& Velocity )
{
    m_Velocity = Velocity;
}

//=============================================================================

const vector3& pfx_emitter::GetAcceleration( void ) const
{
    return m_Acceleration;
}

void pfx_emitter::SetAcceleration( const vector3& Acceleration )
{
    m_Acceleration = Acceleration;
}

//=============================================================================

const vector3& pfx_emitter::GetAxis( void ) const
{
    return m_Axis;
}

//=============================================================================

void pfx_emitter::SetAxis( const vector3& Axis )
{
    m_Axis = Axis;
}

//=============================================================================
void pfx_emitter::UseAxis( xbool Use )
{
    m_UseAxis = Use;
}

//=============================================================================

xbool pfx_emitter::IsActive( void ) const
{
    xbool   IsActive;

    // Check if there are active particles or the potential for new particles
    // Immortal emitters are assumed to always have potential for more particles
    // so IsActive will tell is there are particles active
    if( (m_pEmitterDef->Flags & PFX_EF_IMMORTAL) )
        IsActive = (m_ActiveParticles > 0);
    else
        IsActive = ((m_Age < m_Life) || (m_ActiveParticles > 0));

    // Return active code
    return IsActive;
}

//=============================================================================
void pfx_emitter::SetAzimuth( radian Azim, radian Variance )
{
    m_pEmitterDef->EmitAzimuth = Azim;
    m_pEmitterDef->EmitAzimuthVariance = Variance;
}

//=============================================================================
void pfx_emitter::SetElevation( radian Elev, radian Variance )
{
    m_pEmitterDef->EmitElevation = Elev;
    m_pEmitterDef->EmitElevationVariance = Variance;
}

//=============================================================================

void pfx_emitter::UpdatePoint( f32 DeltaTime )
{
    s32             i;
    f32             PreviousAge      = m_Age;
    vector3         PreviousPosition = m_Position;
    pfx_particle*   p;

    vector3         EmitPosition(0,0,0);
    vector3         Axis;
    f32             Elevation;
    f32             Azimuth;
    f32             Offset;
    f32             Velocity;
    f32             GravityTime;
    xbool           Enabled  = m_pEffect->IsEnabled();
    xbool           Relative = m_pEmitterDef->Flags & PFX_EF_RELATIVE;

    // Get View Z vector
    vector3 ViewZ = eng_GetView( 0 )->GetViewZ();

    ASSERT( m_Created );

    // Precalculate loop invariants
    GravityTime = -9.81f * DeltaTime;

    if( m_ActiveParticles > 0 )
    {
        // Clear active particle count
        m_ActiveParticles = 0;

        // Update existing particles
        p = m_pParticles;
        for( i=0 ; i<m_MaxParticles ; i++ )
        {
            // Does particle still live?
            if( p->Age < p->Life )
            {
                // Update particle characteristics
                p->Velocity.X -= p->Velocity.X * m_pParticleDef->DragCoefficient;
                p->Velocity.Y -= p->Velocity.Y * m_pParticleDef->DragCoefficient;
                p->Velocity.Z -= p->Velocity.Z * m_pParticleDef->DragCoefficient;
                p->Velocity.Y += GravityTime * m_pParticleDef->GravityCoefficient;

                p->Position.X += p->Velocity.X * DeltaTime;
                p->Position.Y += p->Velocity.Y * DeltaTime;
                p->Position.Z += p->Velocity.Z * DeltaTime;

                // Update age of particle
                p->Age += DeltaTime;

                // Increment number of active particles
                if( p->Age < p->Life )
                    m_ActiveParticles++;

                // Add to bounding box
            }

            // Next particle
            p++;
        }
    }

    // Generate any new particles if emitter is alive
    if( (m_Age <= m_Life) )
    {
        // Update age
        m_Age += DeltaTime;

        // Update position
        m_Position  += m_Velocity * DeltaTime;
        m_Velocity  += m_Acceleration * DeltaTime;

        // Emit particles that occured in this time slice
        if( Enabled )
        {
            while( (m_NextEmissionTime <= m_Age) && (m_ActiveParticles < m_MaxParticles) )
            {
                // Calculate point of emission with a lerp
                if( !Relative )
                {
                    EmitPosition = PreviousPosition;
                    if( m_Age > PreviousAge )
                        EmitPosition += (m_Position - PreviousPosition) * ((m_NextEmissionTime-PreviousAge)/(m_Age-PreviousAge));
                }

                // Find inactive particle
                p = NULL;
                for( i=0 ; i<m_MaxParticles ; i++ )
                {
                    if( m_pParticles[i].Age >= m_pParticles[i].Life )
                    {
                        p = &m_pParticles[i];
                        break;
                    }
                }
                ASSERT( p );

                // Calculate Axis of emission
                if ( !m_UseAxis )
                {                
                    Axis.Set( 0.0f, 0.0f, -1.0f );

                    Elevation = RandVar( m_pEmitterDef->EmitElevation, m_pEmitterDef->EmitElevationVariance );
                    Azimuth   = RandVar( m_pEmitterDef->EmitAzimuth,   m_pEmitterDef->EmitAzimuthVariance );
                    Axis.RotateX( Elevation );
                    Axis.RotateY( Azimuth );

                    // Rotate the vector to the normal of the effect
                    Axis = ( m_A * Axis.X ) + ( m_pEffect->GetNormal() * Axis.Y ) + ( m_B * Axis.Z );
                }
                else
                    Axis = m_Axis;

                // Set offset and velocity
                Offset    = RandVar( m_pEmitterDef->EmitOffset,    m_pEmitterDef->EmitOffsetVariance );
                Velocity  = RandVar( m_pEmitterDef->EmitVelocity,  m_pEmitterDef->EmitVelocityVariance );

                // Set lifetime
                p->Life        = RandVar( m_pParticleDef->Life,     m_pParticleDef->LifeVariance     );
                p->Age         = (m_Age - m_NextEmissionTime);

                // Set position and velocity
                p->Velocity = Axis * Velocity + m_Velocity * m_pParticleDef->InheritVelCoefficient;
                p->Position = EmitPosition + Axis * Offset + p->Velocity * p->Age;

                if( m_pParticleDef->Flags & PFX_PF_MOVETOVIEWER )
                    p->Velocity += ViewZ * -3.0f;

                // Set Rotation
                p->Angle    = RandVar( m_pParticleDef->Rotation, m_pParticleDef->RotationVariance );

                // Clamp age
                if( p->Age > p->Life )
                    p->Age = p->Life;

                // Increase number of active particles
                if( p->Age < p->Life )
                    m_ActiveParticles++;

                // Determine time to next emission
                m_NextEmissionTime += RandVar( m_pEmitterDef->EmitInterval, m_pEmitterDef->EmitIntervalVariance );
            }
        }
        else
        {
            // Reset next emission time so it will happen when effect is restarted
            m_NextEmissionTime = m_Age;
        }

        // Reset Age if immortal
        if( m_pEmitterDef->Flags & PFX_EF_IMMORTAL )
        {
            m_NextEmissionTime -= m_Age;
            m_Age = 0;
        }
    }
}

//=============================================================================

void pfx_emitter::UpdateShell( f32 DeltaTime )
{
    ASSERT( m_Created );

    // Update Emitter
    m_Position += m_Velocity * DeltaTime;
    m_Age      += DeltaTime;
}

//=============================================================================

void pfx_emitter::UpdateShape( f32 DeltaTime )
{
    ASSERT( m_Created );

    // Update Emitter
    m_Position += m_Velocity * DeltaTime;
    m_Age      += DeltaTime;
}

//=============================================================================

inline void pfx_emitter::ReadKey( pfx_key& out, f32 t )
{
    ASSERT( m_pParticleDef->nKeys > 0 );

    if( m_pParticleDef->nKeys == 1 )
    {
        out = *m_pParticleLibrary->GetKey( m_pParticleDef->iKey );
    }
    else
    {
        f32         tf;
        s32         i;
        f32         t2;
        pfx_key*    pk1;
        pfx_key*    pk2;

        tf = 1.0f / (m_pParticleDef->nKeys-1);
        i  = (s32)(t / tf);
        if( i > (m_pParticleDef->nKeys-2) ) i = m_pParticleDef->nKeys-2;
        t2 = (t-(i*tf)) / tf;

        pk1 = m_pParticleLibrary->GetKey( m_pParticleDef->iKey + i   );
        pk2 = m_pParticleLibrary->GetKey( m_pParticleDef->iKey + i+1 );

        ASSERT( t2 >= 0.0f );
//        ASSERT( t2 <= 1.0f );

        out.S     = pk1->S + (pk2->S - pk1->S) * t2;
        out.I     = pk1->I + (pk2->I - pk1->I) * t2;
        out.R     = pk1->R + (u8)((pk2->R - pk1->R) * t2);
        out.G     = pk1->G + (u8)((pk2->G - pk1->G) * t2);
        out.B     = pk1->B + (u8)((pk2->B - pk1->B) * t2);
        out.A     = pk1->A + (u8)((pk2->A - pk1->A) * t2);
    }
}

//=============================================================================

void pfx_emitter::RenderPoint( void )
{
    s32                     i;
    pfx_particle*           p;
    pfx_particle_def*       pd;
    f32                     t;
    pfx_key                 key;
    vector3                 Origin;
    s32                     CurTexture = -1;
    s32                     TextureIndex;

    ASSERT( m_Created );

    // Early exit if there are no active particles for emitter
    if( m_ActiveParticles == 0 )
        return;

    // Set origin for rendering
    if( m_pEmitterDef->Flags & PFX_EF_RELATIVE )
        Origin = m_pEffect->GetPosition();
    else
        Origin.Zero();

    // Get Particle def pointer
    pd = m_pParticleDef;
    ASSERT( pd );

    if( pfx_UseDraw )
    {
        // Begin drawing
        draw_Begin( DRAW_SPRITES, DRAW_TEXTURED | ( pfx_UseAlpha ? DRAW_USE_ALPHA : 0 ));
        draw_ClearL2W();

        // Set Blend mode if additive Alpha required
        if( pd->Flags & PFX_PF_ADDITIVE )
        {
#ifdef TARGET_PC
            g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
#endif
#ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
            gsreg_SetZBufferUpdate( FALSE ) ;
            gsreg_End();
#endif
        }

        // Draw particle loop
        p = &m_pParticles[m_MaxParticles-1];
        for( i=0 ; i<m_MaxParticles ; i++ )
        {
            // Particle still alive?
            if( p->Age < p->Life )
            {
                // Get t (0.0 - 1.0) for particle & read key
                t = p->Age / p->Life;
                ASSERT( t >= 0.0f );
                ASSERT( t <= 1.0f );
                ReadKey( key, t );

                // Compute Alphas from fog color
                s32 A = ((s32)(key.A * (m_pEffect->GetMaxAlpha() * m_MaxAlpha) ) * (255-g_PfxFogColor.A)) >> 8;

                // Set Texture
                TextureIndex = (s32)key.I;
                if( TextureIndex != CurTexture )
                {
                    CurTexture = TextureIndex;
                    draw_SetTexture( pd->m_pTextures[CurTexture] );
                }

                // Draw Sprite
                draw_SpriteUV( Origin+p->Position,
                               vector2(key.S,key.S),
                               vector2(0,0),
                               vector2(1,1),
                               xcolor(key.R<<1,key.G<<1,key.B<<1,A<<1),
                               p->Angle );

                if ( m_pParticleDef->Flags & PFX_PF_HASGLOW )
                {
                    f32 Alpha = 38.0f * (1.0f - ( m_Age / m_Life ));
                    tgl.PostFX.AddFX( Origin+p->Position, vector2(key.S * 2.0f,key.S * 2.0f), xcolor(key.R<<1,key.G<<1,key.B<<1,(u8)Alpha) );
                }
            }

            // Next Particle
            p--;
        }

        // Flush drawing
        draw_End();
    }
    else
    {
        // Use PS2 accelerated rendering for Sprites
        vector3*  pVert;
        vector3*  pV;
        vector2*  pTex;
        vector2*  pT;
        ps2color* pCol;
        ps2color* pC;

        // Allocate buffers from scratch
        pVert = pV = (vector3*) smem_BufferAlloc( sizeof(vector3)  * m_ActiveParticles*6 );
        pTex  = pT = (vector2*) smem_BufferAlloc( sizeof(vector2)  * m_ActiveParticles*6 );
        pCol  = pC = (ps2color*)smem_BufferAlloc( sizeof(ps2color) * m_ActiveParticles*6 );

        // Check that we were able to allocate memory
        if( pV && pT && pC )
        {
            // Begin drawing
            poly_Begin( POLY_DRAW_SPRITES | ( pfx_UseAlpha ? POLY_USE_ALPHA : 0 ));
            #ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_DST,A_SRC,C_DST) );
            gsreg_End();
            #endif

            // Set Blend mode if additive Alpha required
            if( pd->Flags & PFX_PF_ADDITIVE )
            {
#ifdef TARGET_PC
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
#endif
#ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
                gsreg_SetZBufferUpdate( FALSE ) ;
                gsreg_End();
#endif
            }

            // Draw particle loop
            p = &m_pParticles[0]; //m_MaxParticles-1];
            for( i=0 ; i<m_MaxParticles ; i++ )
            {
                // Particle still alive?
                if( p->Age < p->Life )
                {
                    // Get t (0.0 - 1.0) for particle & read key
                    t = p->Age / p->Life;
                    ASSERT( t >= 0.0f );
                    ASSERT( t <= 1.0f );
                    ReadKey( key, t );

                    // Compute Alphas from fog color
                    s32 A = ((s32)(key.A * ( m_pEffect->GetMaxAlpha() * m_MaxAlpha) ) * (255-g_PfxFogColor.A)) >> 8;

                    // Set Texture
                    TextureIndex = (s32)key.I;
                    if( TextureIndex != CurTexture )
                    {
                        CurTexture = TextureIndex;
                        vram_Activate( pd->m_pTextures[CurTexture] );
                    }

                    // Add sprite to list
                    pVert[0]  = Origin + p->Position;                           // pass sprite origin in world pos 0
                    pVert[1].Set( key.S,      key.S,      0.0f ); //x_sin( p->Angle ));   // pass sprite dimensions in world pos 1      | sin(theta)
                    pVert[2].Set( s_UVS[5].X, s_UVS[5].Y, 1.0f ); //x_cos( p->Angle ));   // pass 4th texture coordinate in world pos 2 | cos(theta)

                    pCol[0].R = ((u8)key.R );                                   // 1st colour is used for all vertices
                    pCol[0].G = ((u8)key.G );
                    pCol[0].B = ((u8)key.B );
                    pCol[0].A = ((u8)A );

                    pTex[0]   = s_UVS[0];
                    pTex[1]   = s_UVS[1];
                    pTex[2]   = s_UVS[2];

                    pVert += 6;
                    pTex  += 6;
                    pCol  += 6;

                    // add the PostFX if req'd
                    if ( m_pParticleDef->Flags & PFX_PF_HASGLOW )
                    {
                        tgl.PostFX.AddFX( Origin+p->Position, vector2(key.S * 2.0f,key.S * 2.0f), xcolor(key.R,key.G,key.B,38) );
                    }

                }

                // Next Particle
                p++;
            }

            // Render the sprites
            poly_Render( pV, pT, pC, m_ActiveParticles*6 );
            poly_End();
        }
    }
}

//=============================================================================

void pfx_emitter::RenderOrientedPoint( void )
{
    s32                     i;
    pfx_particle*           p;
    pfx_particle_def*       pd;
    f32                     t;
    pfx_key                 key;
    vector3                 ViewZ;
    vector3                 Dir;                // Direction of particle travel
    vector3                 CrossDir;           // Cross of direction of particle with ViewZ
    vector3                 Start;              // Leading edge of particle along path
    vector3                 End;                // Trailing edge of particle along path
    s32                     CurTexture = -1;
    s32                     TextureIndex;

    ASSERT( m_Created );

    // Early exit if there are no active particles for emitter
    if( m_ActiveParticles == 0 )
        return;

    // Get Particle def pointer
    pd = m_pParticleDef;
    ASSERT( pd );

    // Get view Z vector
    ViewZ = eng_GetActiveView(0)->GetViewZ();

    if( pfx_UseDraw )
    {
        // Begin drawing
        draw_Begin( DRAW_TRIANGLES, DRAW_TEXTURED | ( pfx_UseAlpha ? DRAW_USE_ALPHA : 0 ));
        draw_ClearL2W();

        // Set Blend mode if additive Alpha required
        if( pd->Flags & PFX_PF_ADDITIVE )
        {
#ifdef TARGET_PC
            g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
#endif
#ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
            gsreg_End();
#endif
        }

        // Particle loop
        p = &m_pParticles[0];
        for( i=0 ; i<m_MaxParticles ; i++ )
        {
            if( p->Age < p->Life )
            {
                // Get t (0.0 - 1.0) for particle & rad key
                t = p->Age / p->Life;
                ASSERT( t >= 0.0f );
                ASSERT( t <= 1.0f );
                ReadKey( key, t );

                // Compute Alphas from fog color
                s32 A = ((s32)(key.A * (m_pEffect->GetMaxAlpha() * m_MaxAlpha )) * (255-g_PfxFogColor.A)) >> 8;

                // Get Vectors for direction of particle and cross of view vector with that
                Dir       = p->Velocity;
                Dir.Normalize();
                CrossDir  = ViewZ.Cross( Dir );
                Dir      *= key.S;
                CrossDir *= key.S;
                Start     = p->Position + Dir;
                End       = p->Position - Dir;

                // Set Texture
                TextureIndex = (s32)key.I;
                if( TextureIndex != CurTexture )
                {
                    CurTexture = TextureIndex;
                    draw_SetTexture( pd->m_pTextures[CurTexture] );
                }

                // Draw Quad
                draw_Color( xcolor(key.R<<1,key.G<<1,key.B<<1,A<<1) );

                draw_UV( 1.0f, 0.0f ); draw_Vertex( Start - CrossDir );
                draw_UV( 0.0f, 0.0f ); draw_Vertex( End   - CrossDir );
                draw_UV( 0.0f, 1.0f ); draw_Vertex( End   + CrossDir );

                draw_UV( 1.0f, 0.0f ); draw_Vertex( Start - CrossDir );
                draw_UV( 0.0f, 1.0f ); draw_Vertex( End   + CrossDir );
                draw_UV( 1.0f, 1.0f ); draw_Vertex( Start + CrossDir );

                // add the PostFX if req'd
                if ( m_pParticleDef->Flags & PFX_PF_HASGLOW )
                {
                    f32 size = (Start - End).Length();
                    tgl.PostFX.AddFX( ((Start + End) * 0.5f), vector2(size,size), xcolor(key.R,key.G,key.B,38) );
                }
            }

            // Next Particle
            p++;
        }

        // Flush Drawing
        draw_End();
    }
    else
    {
        vector3*  pVert;
        vector3*  pV;
        vector2*  pTex;
        vector2*  pT;
        ps2color* pCol;
        ps2color* pC;

        pVert = pV = (vector3*) smem_BufferAlloc( sizeof(vector3)  * m_ActiveParticles*6 );
        pTex  = pT = (vector2*) smem_BufferAlloc( sizeof(vector2)  * m_ActiveParticles*6 );
        pCol  = pC = (ps2color*)smem_BufferAlloc( sizeof(ps2color) * m_ActiveParticles*6 );

        // Check that we were able to allocate memory
        if( pC && pT && pV  )
        {
            ps2color C;

            poly_Begin( pfx_UseAlpha ? POLY_USE_ALPHA : 0 );
#ifdef TARGET_PS2
            gsreg_Begin();
            gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_DST,A_SRC,C_DST) );
            gsreg_End();
#endif

            // Set Blend mode if additive Alpha required
            if( pd->Flags & PFX_PF_ADDITIVE )
            {
#ifdef TARGET_PC
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
#endif
#ifdef TARGET_PS2
                gsreg_Begin();
                gsreg_SetAlphaBlend( ALPHA_BLEND_MODE(C_SRC,C_ZERO,A_SRC,C_DST) );
                gsreg_End();
#endif
            }

            // Particle loop
            p = &m_pParticles[0];
            for( i=0 ; i<m_MaxParticles ; i++ )
            {
                if( p->Age < p->Life )
                {
                    // Get t (0.0 - 1.0) for particle & rad key
                    t = p->Age / p->Life;
                    ASSERT( t >= 0.0f );
                    ASSERT( t <= 1.0f );
                    ReadKey( key, t );

                    // Compute Alphas from fog color
                    s32 A = ((s32)(key.A * (m_pEffect->GetMaxAlpha() * m_MaxAlpha )) * (255-g_PfxFogColor.A)) >> 8;

                    // Get Vectors for direction of particle and cross of view vector with that
                    Dir       = p->Velocity;
                    Dir.Normalize();
                    CrossDir  = ViewZ.Cross( Dir );
                    Dir      *= key.S;
                    CrossDir *= key.S;
                    Start     = p->Position + Dir;
                    End       = p->Position - Dir;

                    // Set Texture
                    TextureIndex = (s32)key.I;
                    if( TextureIndex != CurTexture )
                    {
                        CurTexture = TextureIndex;
                        vram_Activate( pd->m_pTextures[CurTexture] );
                    }

                    pVert[0] = Start - CrossDir;
                    pVert[1] = End   - CrossDir;
                    pVert[2] = End   + CrossDir;
                    pVert[3] = Start - CrossDir;
                    pVert[4] = End   + CrossDir;
                    pVert[5] = Start + CrossDir;
            
                    C.R = ((u8)key.R );
                    C.G = ((u8)key.G );
                    C.B = ((u8)key.B );
                    C.A = ((u8)A );

                    pCol[0] = C;
                    pCol[1] = C;
                    pCol[2] = C;
                    pCol[3] = C;
                    pCol[4] = C;
                    pCol[5] = C;
            
                    pTex[0] = s_UVS[0];
                    pTex[1] = s_UVS[1];
                    pTex[2] = s_UVS[2];
                    pTex[3] = s_UVS[3];
                    pTex[4] = s_UVS[4];
                    pTex[5] = s_UVS[5];
            
                    pVert += 6;
                    pTex  += 6;
                    pCol  += 6;

                    // add the PostFX if req'd
                    if ( m_pParticleDef->Flags & PFX_PF_HASGLOW )
                    {
                        f32 size = (Start - End).Length();
                        tgl.PostFX.AddFX( ((Start + End) * 0.5f), vector2(size,size), xcolor(key.R,key.G,key.B,38) );
                    }
                }

                // Next Particle
                p++;
            }

            // Flush drawing
            poly_Render( pV, pT, pC, m_ActiveParticles*6 );
            poly_End();
        }
    }
}

//=============================================================================

void pfx_emitter::RenderShell( void )
{
    pfx_particle_def*       pd;
    f32                     t;

    ASSERT( m_Created );

    // Get Particle def pointer
    pd = m_pParticleDef;
    ASSERT( pd );

    // Check if we need to render anything
    if( m_Age < m_Life )
    {
        // Check if older than start delay
        if( m_Age >= m_NextEmissionTime )
        {
            // Get t (0.0 - 1.0) for particle
            ASSERT( m_Life > 0.0f );
            t = (m_Age - m_NextEmissionTime) / (m_Life - m_NextEmissionTime );

            // Read animation key
            ASSERT( t >= 0.0f );
            ASSERT( t <= 1.0f );
            pfx_key key;
            ReadKey( key, t );

            // Render the hemisphere shape
            m_Instance.SetShape( tgl.GameShapeManager.GetShapeByType( SHAPE_TYPE_PARTICLE_HEMISPHERE ) );

            // Force instance to always create render nodes each time it's drawn
            // since all the particles share this one instance
            m_Instance.SetFlag(shape_instance::FLAG_ALWAYS_DIRTY, TRUE) ;
            
            // Compute Alphas from fog color
            s32 A = ((s32)(key.A<<1) * (255-g_PfxFogColor.A)) >> 8;

            m_Instance.SetPos( m_Position );
            m_Instance.SetRot( m_ViewRot );
            m_Instance.SetScale( vector3(key.S,key.S,key.S) );
            m_Instance.SetColor( xcolor(key.R<<1,key.G<<1,key.B<<1,A) );
            m_Instance.SetFogColor( xcolor(g_PfxFogColor.R, g_PfxFogColor.G, g_PfxFogColor.B, (g_PfxFogColor.A*A)>>8) );
            m_Instance.Draw( shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_FOG | shape::DRAW_FLAG_TURN_OFF_LIGHTING ) ;
        }
    }
}

//=============================================================================

void pfx_emitter::RenderShape( void )
{
    pfx_particle_def*       pd;
    f32                     t;

    ASSERT( m_Created );

    // Get Particle def pointer
    pd = m_pParticleDef;
    ASSERT( pd );

    // Check if we need to render anything
    if( m_Age < m_Life )
    {
        // Check if older than start delay
        if( m_Age >= m_NextEmissionTime )
        {
            // Get t (0.0 - 1.0) for particle
            ASSERT( m_Life > 0.0f );
            t = (m_Age - m_NextEmissionTime) / (m_Life - m_NextEmissionTime );

            // Read animation key
            ASSERT( t >= 0.0f );
            ASSERT( t <= 1.0f );
            pfx_key key;
            ReadKey( key, t );

            // Render the hemisphere shape
            m_Instance.SetShape( tgl.GameShapeManager.GetShapeByType( m_pEmitterDef->ShapeID ) );

            // Force instance to always create render nodes each time it's drawn
            // since all the particles share this one instance
            m_Instance.SetFlag(shape_instance::FLAG_ALWAYS_DIRTY, TRUE) ;

            m_Instance.SetPos( m_Position );
            m_Instance.SetTextureFrame( t * 28.0f );
            m_Instance.SetTextureAnimFPS(0);
           
            radian3 rot;

            if ( !(m_pParticleDef->Flags & 0x08) )
            {
                rot = m_ViewRot;
                rot.Roll = m_Rotation;
            }
            else
            {
                rot.Set( 0.0f, 0.0f, 0.0f );
            }

            // Compute Alphas from fog color
            s32 A = ((s32)(key.A<<1) * (255-g_PfxFogColor.A)) >> 8;

            m_Instance.SetRot( rot );
            m_Instance.SetScale( vector3(key.S,key.S,key.S) );
            m_Instance.SetColor( xcolor(key.R<<1,key.G<<1,key.B<<1,A) );
            m_Instance.SetFogColor( xcolor(g_PfxFogColor.R, g_PfxFogColor.G, g_PfxFogColor.B, (g_PfxFogColor.A*A)>>8) );
            m_Instance.Draw( shape::DRAW_FLAG_CLIP | shape::DRAW_FLAG_FOG | shape::DRAW_FLAG_TURN_OFF_LIGHTING ) ;

            // add the PostFX if req'd
            if ( m_pParticleDef->Flags & PFX_PF_HASGLOW )
            {
                vector2 size( key.S * 1.5f, key.S * 1.5f );
                f32 Alpha = 38.0f * (1.0f - ( m_Age / m_Life ));
                tgl.PostFX.AddFX( m_Position, size, xcolor(key.R,key.G,key.B,(u8)Alpha) );
            }
        }
    }
}

//=============================================================================

void pfx_emitter::UpdateEmitter( f32 DeltaTime )
{
    switch( m_pEmitterDef->Type )
    {
    case pfx_emitter_def::POINT:
    case pfx_emitter_def::ORIENTED_POINT:
        UpdatePoint( DeltaTime );
        break;

    case pfx_emitter_def::SHELL:
        UpdateShell( DeltaTime );
        break;

    case pfx_emitter_def::SHOCKWAVE:
        break;

    case pfx_emitter_def::SHAPE:
        UpdateShape( DeltaTime );
        break;

    default:
        ASSERT( 0 );
    }
}

//=============================================================================

void pfx_emitter::RenderEmitter( void )
{
    switch( m_pEmitterDef->Type )
    {
    case pfx_emitter_def::POINT:
        RenderPoint( );
        break;

    case pfx_emitter_def::ORIENTED_POINT:
        RenderOrientedPoint( );
        break;

    case pfx_emitter_def::SHELL:
        RenderShell( );
        break;

    case pfx_emitter_def::SHOCKWAVE:
        break;

    case pfx_emitter_def::SHAPE:
        RenderShape( );
        break;

    default:
        ASSERT( 0 );
    }
}

//=============================================================================
