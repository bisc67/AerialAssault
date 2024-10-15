#ifndef ASSET_EDITOR_HPP
#define ASSET_EDITOR_HPP

//==============================================================================
//  INCLUDES
//==============================================================================
#include "x_Files.hpp"
#include "Objects/Projectiles/asset.hpp"
#include "Objects/Bot/Deployable.hpp"

//==============================================================================
//  Asset Editor Class Definition
//==============================================================================

class asset_editor
{
//==============================================================================

public:
    enum 
    {
        MAX_SNIPE_POINTS = 50,
    };
    
    enum asset_type
    {
        AT_INVEN,
        AT_TURRET,
        AT_SENSOR,
        AT_MINE,
    };

    struct team_assets
    {
        asset_spot::team            m_Team;

        inven_spot      m_pInvenList[asset_spot::MAX_INVENS];
        s32             m_nInvens;

        turret_spot     m_pTurretList[asset_spot::MAX_TURRETS];
        s32             m_nTurrets;

        sensor_spot     m_pSensorList[asset_spot::MAX_SENSORS];
        s32             m_nSensors;

        mine_spot       m_pMineList[asset_spot::MAX_MINES];
        s32             m_nMines;

        vector3         m_pSnipePoint[MAX_SNIPE_POINTS ];
        s32             m_nSnipePoints;
    };


//==============================================================================
    public:
    
                    asset_editor            ( void );
                   ~asset_editor            ( void );

        void        Save                    ( const char* pFileName );
        void        Load                    ( const char* pFileName );

        xbool       AddInven                ( void );
        xbool       AddTurret               ( void );
        xbool       AddSensor               ( void );
        xbool       AddMine                 ( void );

        void        Render                  ( void );

        // Select closest asset to given position
const   void*       GetClosestAsset         (const vector3& Pos);
const   inven_spot* GetClosestInven         (const vector3& Pos, asset_spot::team Team, f32& Dist);
const   turret_spot* GetClosestTurret       (const vector3& Pos, asset_spot::team Team, f32& Dist);
const   sensor_spot* GetClosestSensor       (const vector3& Pos, asset_spot::team Team, f32& Dist);
const   mine_spot* GetClosestMine           (const vector3& Pos, asset_spot::team Team, f32& Dist);
const   vector3*     GetClosestSP            (const vector3& Pos, asset_spot::team Team, f32& Dist);
const   void*       GetNextBadAsset         ( void );

        void        Deselect                ( void );

        void        DeleteSelected          ( void );             

        void        ReplaceDeployables      ( void );

        void        AddSnipePoint           ( void );

        void        Report                  ( X_FILE* FP = NULL );
        xbool       ReportAll               ( const char* pFileName );

const   char*       ToggleLimits            ( void );
const   char*       ToggleSensorLimits            ( void );

        xbool       ValidateInvenSpot       ( inven_spot* Spot );
        xbool       ValidateTurretSpot      ( turret_spot* Spot );
        xbool       ValidateSensorSpot      ( sensor_spot* Spot );

//==============================================================================
protected:

        void        InitClass               ( void );
        void        RegisterInven           ( asset* Asset, radian Yaw );
        void        RegisterTurret          ( asset* Asset, vector3& Normal);
        void        RegisterSensor          ( asset* Asset, vector3& Normal);

        xbool       AttemptDeletion         ( void* List, s32& Size );

        void        SortAssetsLists         ( void );

//==============================================================================
protected:
    // members
        team_assets     m_StormAssets;
        team_assets     m_InfernoAssets;

        u32             m_Limits;
        u32             m_SensorLimits;
};

//==============================================================================
//  END
//==============================================================================
#endif
