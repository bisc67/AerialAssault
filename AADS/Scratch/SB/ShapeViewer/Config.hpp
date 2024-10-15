//=========================================================================
//
// Config.hpp
//
//=========================================================================


#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__


#include "Entropy.hpp"
#include "Shape/Shape.hpp"
#include "Shape/ShapeInstance.hpp"


//=========================================================================
// Structures
//=========================================================================

// Structure used to hold network paths since UHC paths do not work
struct NetworkPath
{
    char    Drive[3] ;      // "P:" etc..
    char    Path[256] ;     // "\\PROJECT1\TRIBES2" etc..
} ;

//=========================================================================
// Variables
//=========================================================================

extern const char       *g_Version ;
extern const char       *g_ConfigFilename ;
extern shape            *g_Shapes[256] ;
extern s32              g_nShapes ;
extern shape_instance   *g_Instances[256] ;
extern s32              g_nInstances ;
extern s32              g_CurrentInstance ;
extern f32              g_PlaybackSpeed ;
extern s32              g_ShowHelp ;
extern s32              g_ShowStats ;
extern texture          g_BackdropTexture ;
extern s32              g_RemoveRootTrans ;
extern vector3          g_BackgroundCol ;
extern NetworkPath      g_NetworkPaths[256] ;
extern s32              g_nNetworkPaths ;   
extern vector3          g_vLightDir ;
extern s32              g_BuildMips ;
extern texture_manager  g_TextureManager ;
extern s32              g_TextureFrame ;


//=========================================================================
// Functions
//=========================================================================

// Cleans up memory
void ConfigCleanup() ;

// Parses config file and reloads models
void ConfigParseFile() ;

// Initializes data
void ConfigInit() ;


#endif  //#ifndef __CONFIG_HPP__


