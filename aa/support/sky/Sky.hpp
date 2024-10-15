//=========================================================================
//
// SKY.HPP
//
//=========================================================================
#ifndef SKY_HPP
#define SKY_HPP

#include "Entropy.hpp"
#include "Fog/Fog.hpp"
#include "Tokenizer/Tokenizer.hpp"

//=========================================================================

class sky
{
private:

    xbitmap     m_Material[5];
    const fog*  m_Fog;
    radian      m_HazeStartAngle;
    radian      m_HazeFinishAngle;
    vector3     m_EyePos;
    radian      m_Yaw;

    void    RenderFogDome   ( void );
    void    SeedZBuffer     ( void );
public:
            sky             ( void );
           ~sky             ( void );
    void    Load            ( token_stream& TOK );
    void    SetFog          ( const fog* pFog );
    void    SetYaw          ( radian Yaw );
    void    Kill            ( void );
    void    Render          ( void );
};


//=========================================================================
#endif
//=========================================================================
