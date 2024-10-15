//=========================================================================
//
// FOG.HPP
//
//=========================================================================

#ifndef FOG_HPP
#define FOG_HPP

#include "Tokenizer/Tokenizer.hpp"

#include "x_math.hpp"
#include "x_color.hpp"
#include "x_bitmap.hpp"

//=========================================================================

#define MAX_FOG_VOLUMES 8

class fog
{
public:

    struct volume
    {
        f32 MaxY;
        f32 MinY;
        f32 VisDistance;
        f32 VisFactor;      // 1 / VisDistance
    };

private:
/*
    struct band
    {
        f32 Factor;
        f32 Height;
        s32 IsFog;
    };
*/
    volume          m_Volume[MAX_FOG_VOLUMES];
    s32             m_NVolumes;
    f32             m_MinVolumeY;
    f32             m_MaxVolumeY;

    xcolor          m_FogColor;
    xcolor          m_HazeColor;
    
    f32             m_HazeStartDist;
    f32             m_HazeFinishDist;
    f32             m_MaxBandAlpha;
    f32             m_MinBandAlpha;
//  band            m_PosBand[MAX_FOG_VOLUMES];
//  band            m_NegBand[MAX_FOG_VOLUMES];
//  s32             m_NPosBands;
//  s32             m_NNegBands;

    vector3         m_EyePos;

//  f32             m_UVMinDeltaY;
//  f32             m_UVMaxDeltaY;
    f32             m_UVMinDist;
    f32             m_UVMaxDist;
    s32             m_UVWidth;
    s32             m_UVHeight;
    xbitmap         m_BMP[4];
    s32             m_BMPIndex;

    f32             m_WorldMaxY;
    f32             m_WorldMinY;

    f32             m_UVDeltaYConst0;
    f32             m_UVDeltaYConst1;
    f32             m_UVDistConst0;
    f32             m_UVDistConst1;

    radian          m_HazeStartAngle;
    radian          m_HazeFinishAngle;

    xbool           m_Initialized;

    void    BuildFogMap         ( void );

public:
            fog                 ( void );
           ~fog                 ( void );

    void    Load                ( token_stream& TOK );
    void    Load                ( const char* );
    void    Unload              ( void );

    void    SaveSettings        ( const char* pFileName );
    void    LoadSettings        ( const char* pFileName );

    void    SetEyePos           ( const vector3& Pos );

    void    SetMinMaxY          ( f32 MinY, f32 MaxY );

    xcolor  ComputeFog          ( f32 Dist, f32 DeltaY ) const;
    xcolor  ComputeFog          ( const vector3& Pos ) const;
    xcolor  ComputeParticleFog  ( const vector3& Pos ) const;

    void    ComputeUVs          ( f32 Dist, f32 DeltaY, f32& U, f32& V ) const;
    void    ComputeUVs          ( const vector3& Pos, f32& U, f32& V ) const;

    void    RenderFogMap        ( void ) const;

    void    GetFogConst         ( f32& Dist0, f32& Dist1, f32& DeltaY0, f32& DeltaY1 ) const;

    f32     GetVisDistance      ( void ) const;
    
    const xbitmap& GetBMP       ( void ) const;

    xcolor  GetHazeColor        ( void ) const;
    void    GetHazeAngles       ( radian& StartAngle, radian& FinishAngle ) const;

    s32     GetNVolumes         ( void ) const;
    const volume& GetVolume     ( s32 Index ) const;

};


//=========================================================================
#endif
//=========================================================================
