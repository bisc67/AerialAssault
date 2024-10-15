//          Vehicles Script - Audio


//              Wildcat Vehicle

Complex	"SFX_VEHICLE_WILDCAT_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleLight02Mix01_LP1.aif";
	Volume	0.70;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.3;
}

/*
Complex	"SFX_VEHICLE_WILDCAT_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.6;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_WILDCAT_AFTERBURNER_LOOP"
{
	SoundFile "Armor\thrust_LP1.aif";
	Volume	1.0;
	Pitch	0.8;
    Priority    -1;
    Falloff 0.3;
}


//              Shrike Vehicle

Complex	"SFX_VEHICLE_SHRIKE_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleLight02Mix01_LP1.aif";
	Volume	0.90;
	Pitch	0.95;
    Priority    -1;
    Falloff 0.3;
}

/*
Complex	"SFX_VEHICLE_SHRIKE_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.53;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_SHRIKE_AFTERBURNER_LOOP"
{
	SoundFile "Vehicles\AfterBurnerSmall01Mix02_LP1.aif";
	Volume	0.98;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_SHRIKE_WEAPON_FIRE"
{
	SoundFile "weapons\BlasterFire09Mix01.aif";
	Volume	0.70;
	Pitch	0.85;
    Priority    90;
    Falloff 0.4;
}


//              Beowulf Vehicle - not used

Complex	"SFX_VEHICLE_BEOWULF_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleHeavy02Mix01_LP1.aif";
	Volume	0.65;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.3;
}

/*
Complex	"SFX_VEHICLE_BEOWULF_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.48;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_BEOWULF_AFTERBURNER_LOOP"
{
	SoundFile "Vehicles\AfterBurnerSmall01Mix02_LP1.aif";
	Volume	0.85;
	Pitch	0.90;
    Priority    -1;
    Falloff 0.3;
}


//              Thundersword Vehicle

Complex	"SFX_VEHICLE_THUNDERSWORD_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleHeavy02Mix02_LP2.aif";
	Volume	0.95;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_THUNDERSWORD_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.44;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_THUNDERSWORD_AFTERBURNER_LOOP"
{
	SoundFile "Vehicles\AfterBurnerHuge01Mix02_LP1.aif";
	Volume	0.98;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.3;
}

/*
Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_RELEASE"
{
	SoundFile "Vehicles\BombRelease01Mix01.aif";
	Volume	0.85;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_DROP_LOOP"
{
	SoundFile "Vehicles\bomber_bomb_projectile.aif";
	Volume	0.85;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.5;
}

Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_EXPLOSION"
{
    Falloff 0.99;
	SoundFile "Explosions\explosion02Mix01.aif"
    {
        Volume	0.75;
	    Pitch	0.80;
        Priority    90;
    }

    SoundFile "Explosions\explosion03Mix01.aif"
    {
        Delay   0.45;
        Volume	0.90;
	    Pitch	0.80;
        Priority    90;
    }
}

//              Havoc Vehicle

Complex	"SFX_VEHICLE_HAVOC_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleMedium02Mix02_LP1.aif";
	Volume	0.90;
	Pitch	1.1;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_HAVOC_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.4;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_HAVOC_AFTERBURNER_LOOP"
{
	SoundFile "Vehicles\AfterBurnerHuge01Mix02_LP1.aif";
	Volume	0.80;
	Pitch	1.1;
    Priority    -1;
    Falloff 0.2;
}


//              Jericho Vehicle - not used

Complex	"SFX_VEHICLE_JERICHO_ENGINE_LOOP"
{
	SoundFile "Vehicles\VehicleMedium02Mix02_LP1.aif";
	Volume	0.90;
	Pitch	0.95;
    Priority    -1;
    Falloff 0.2;
}

/*
Complex	"SFX_VEHICLE_JERICHO_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.aif";
	Volume	0.25;
	Pitch	0.37;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_JERICHO_AFTERBURNER_LOOP"
{
	SoundFile "Vehicles\AfterBurnerHuge01Mix02_LP1.aif";
	Volume	1.0;
	Pitch	0.85;
    Priority    -1;
    Falloff 0.2;
}
/*
Complex	"SFX_VEHICLE_JERICHO_MPB_DEPLOY"
{
	Streamed SoundFile "Vehicles\MPB_deploy.aif";
	Volume	0.90;
	Pitch	0.80;
    Priority    90;
}

Complex	"SFX_VEHICLE_JERICHO_MPB_UNDEPLOY"
{
	Streamed SoundFile "Vehicles\MPB_undeploy_turret.aif";
	Volume	0.90;
	Pitch	0.80;
    Priority    90;
}
*/
//                  Vehicle Collisions
/*

Complex	"SFX_VEHICLE_LIGHT_COLLIDE_TERRAIN"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_LIGHT_COLLIDE_BUILDING"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}


Complex	"SFX_VEHICLE_HEAVY_COLLIDE_TERRAIN"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_HEAVY_COLLIDE_BUILDING"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_SKID_TERRAIN_MEDIUM"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    50;
}

Complex	"SFX_VEHICLE_SKID_TERRAIN_HARD"
{
	SoundFile "misc\OBWarning01.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    50;
}
*/

Complex	"SFX_VEHICLE_MOUNT"
{
	SoundFile "Vehicles\mount_dis.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    50;
    Falloff 0.1;
}

Complex	"SFX_VEHICLE_DISMOUNT"
{
	SoundFile "Vehicles\mount.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    50;
    Falloff 0.1;
}


//              New collision SFX


//*********  Impacts ***********************

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL1_01"
{
	SoundFile "Vehicles\VehicleColl_Impact_01.aif";
	Volume	0.4;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL1_02"
{
	SoundFile "Vehicles\VehicleColl_Impact_02.aif";
	Volume	0.4;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL1_03"
{
	SoundFile "Vehicles\VehicleColl_Impact_03.aif";
	Volume	0.37;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL2_01"
{
	SoundFile "Vehicles\VehicleColl_Impact_04.aif";
	Volume	0.4;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL2_02"
{
	SoundFile "Vehicles\VehicleColl_Impact_05.aif";
	Volume	0.4;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL2_03"
{
	SoundFile "Vehicles\VehicleColl_Impact_06.aif";
	Volume	0.37;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL3_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Impact_01.aif"
    {
	    Volume	0.30;
        Pan     -1.0;
	    Pitch	1.0;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.25;
        Pan     1.0;
	    Pitch	1.0;
        Delay   0.15;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL3_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Impact_02.aif"
    {
	    Volume	0.30;
        Pan     -1.0;
	    Pitch	1.0;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.25;
        Pan     1.0;
	    Pitch	1.0;
        Delay   0.22;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL3_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.30;
        Pan     -1.0;
	    Pitch	1.0;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.25;
        Pan     1.0;
	    Pitch	1.18;
        Delay   0.25;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL4_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.35;
        Pan     -1.0;
	    Pitch	1.10;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	0.92;
        Delay   0.08;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL4_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.35;
        Pan     -1.0;
	    Pitch	1.10;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	0.96;
        Delay   0.05;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL4_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.35;
        Pan     -1.0;
	    Pitch	1.10;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	0.97;
        Delay   0.05;
        Priority    70;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL5_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	0.90;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.40;
        Pan     1.0;
	    Pitch	0.97;
        Delay   0.05;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_01.aif"
    {
	    Volume	0.35;
        Pan     1.0;
	    Pitch	0.91;
        Delay   0.3;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL5_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	0.90;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.40;
        Pan     1.0;
	    Pitch	0.93;
        Delay   0.1;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_02.aif"
    {
	    Volume	0.35;
        Pan     1.0;
	    Pitch	0.89;
        Delay   0.28;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL5_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_01.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	0.90;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_01.aif"
    {
	    Volume	0.40;
        Pan     1.0;
	    Pitch	0.94;
        Delay   0.1;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.35;
        Pan     1.0;
	    Pitch	0.95;
        Delay   0.28;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL6_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	1.12;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.50;
        Pan     1.0;
	    Pitch	1.0;
        Delay   0.06;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	1.15;
        Delay   0.20;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL6_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	1.17;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.50;
        Pan     1.0;
	    Pitch	0.9;
        Delay   0.04;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	0.91;
        Delay   0.21;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL6_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.45;
        Pan     -1.0;
	    Pitch	1.2;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.50;
        Pan     1.0;
	    Pitch	0.85;
        Delay   0.04;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.45;
        Pan     1.0;
	    Pitch	0.89;
        Delay   0.21;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL7_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.65;
        Pan     -1.0;
	    Pitch	0.90;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_01.aif"
    {
	    Volume	0.65;
        Pan     1.0;
	    Pitch	0.86;
        Delay   0.02;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.55;
        Pan     1.0;
	    Pitch	0.88;
        Delay   0.21;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL7_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.65;
        Pan     -1.0;
	    Pitch	0.87;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_02.aif"
    {
	    Volume	0.63;
        Pan     1.0;
	    Pitch	0.86;
        Delay   0.03;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.57;
        Pan     1.0;
	    Pitch	0.88;
        Delay   0.28;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL7_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_02.aif"
    {
	    Volume	0.65;
        Pan     -1.0;
	    Pitch	0.92;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.68;
        Pan     1.0;
	    Pitch	0.82;
        Delay   0.06;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.54;
        Pan     1.0;
	    Pitch	0.83;
        Delay   0.19;
        Priority    65;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL8_01"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_03.aif"
    {
	    Volume	0.70;
        Pan     -1.0;
	    Pitch	0.92;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_06.aif"
    {
	    Volume	0.70;
        Pan     1.0;
	    Pitch	0.87;
        Delay   0.06;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_02.aif"
    {
	    Volume	0.48;
        Pan     1.0;
	    Pitch	0.82;
        Delay   0.19;
        Priority    65;
    }

    SoundFile "Vehicles\VehicleColl_Impact_01.aif"
    {
	    Volume	0.34;
        Pan     1.0;
	    Pitch	0.80;
        Delay   0.48;
        Priority    45;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL8_02"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_03.aif"
    {
	    Volume	0.71;
        Pan     -1.0;
	    Pitch	0.90;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_05.aif"
    {
	    Volume	0.70;
        Pan     1.0;
	    Pitch	0.82;
        Delay   0.04;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_03.aif"
    {
	    Volume	0.48;
        Pan     1.0;
	    Pitch	0.83;
        Delay   0.21;
        Priority    65;
    }

    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.34;
        Pan     1.0;
	    Pitch	0.82;
        Delay   0.44;
        Priority    45;
    }
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_LEVEL8_03"
{
    Falloff 0.3;
	SoundFile "Vehicles\VehicleColl_Boom_03.aif"
    {
	    Volume	0.72;
        Pan     -1.0;
	    Pitch	0.87;
        Priority    80;
    }
    
    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.73;
        Pan     1.0;
	    Pitch	0.81;
        Delay   0.06;
        Priority    70;
    }

    SoundFile "Vehicles\VehicleColl_Impact_04.aif"
    {
	    Volume	0.54;
        Pan     1.0;
	    Pitch	0.80;
        Delay   0.17;
        Priority    65;
    }

    SoundFile "Vehicles\VehicleColl_Impact_02.aif"
    {
	    Volume	0.32;
        Pan     1.0;
	    Pitch	0.75;
        Delay   0.35;
        Priority    45;
    }
}


//*********  Skids ***********************

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL1_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.25;
	Pitch	1.4;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL1_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.22;
	Pitch	1.4;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL1_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.27;
	Pitch	1.34;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL2_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.30;
	Pitch	1.31;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL2_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.27;
	Pitch	1.33;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL2_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.32;
	Pitch	1.24;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL3_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.35;
	Pitch	1.25;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL3_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.32;
	Pitch	1.28;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL3_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.37;
	Pitch	1.22;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL4_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.40;
	Pitch	1.15;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL4_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.37;
	Pitch	1.14;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL4_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.42;
	Pitch	1.16;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL5_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.45;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL5_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.42;
	Pitch	1.08;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL5_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.47;
	Pitch	1.04;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL6_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.50;
	Pitch	0.96;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL6_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.47;
	Pitch	0.99;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL6_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.52;
	Pitch	0.98;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL7_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.55;
	Pitch	0.90;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL7_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.52;
	Pitch	0.91;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL7_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.57;
	Pitch	0.92;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL8_01"
{
	SoundFile "Vehicles\VehicleColl_Skid_02.aif";
	Volume	0.60;
	Pitch	0.90;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL8_02"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.58;
	Pitch	0.91;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_SKID_LEVEL8_03"
{
	SoundFile "Vehicles\VehicleColl_Skid_01.aif";
	Volume	0.63;
	Pitch	0.92;
    Priority    80;
    Falloff 0.3;
}


//*********  Debris ***********************

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL1_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.35;
	Pitch	1.4;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL1_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.35;
	Pitch	1.4;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL1_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.35;
	Pitch	1.4;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL2_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.40;
	Pitch	1.3;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL2_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.40;
	Pitch	1.3;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL2_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.40;
	Pitch	1.3;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL3_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.45;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL3_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.45;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL3_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.45;
	Pitch	1.2;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL4_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.50;
	Pitch	1.1;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL4_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.50;
	Pitch	1.1;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL4_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.50;
	Pitch	1.1;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL5_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL5_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL5_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL6_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.60;
	Pitch	0.9;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL6_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.60;
	Pitch	0.9;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL6_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.60;
	Pitch	0.9;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL7_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.65;
	Pitch	0.85;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL7_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.65;
	Pitch	0.85;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL7_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.65;
	Pitch	0.85;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL8_01"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_01.aif";
	Volume	0.70;
	Pitch	0.77;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL8_02"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_02.aif";
	Volume	0.70;
	Pitch	0.77;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_LEVEL8_03"
{
	SoundFile "Vehicles\VehicleColl_DEBRIS_03.aif";
	Volume	0.70;
	Pitch	0.77;
    Priority    80;
    Falloff 0.3;
}



Complex	"SFX_VEHICLE_COLLISION_IMPACT_01"
{
	SoundFile "Vehicles\VehicleColl_Impact_01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_02"
{
	SoundFile "Vehicles\VehicleColl_Impact_02.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_03"
{
	SoundFile "Vehicles\VehicleColl_Impact_03.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_04"
{
	SoundFile "Vehicles\VehicleColl_Impact_04.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_05"
{
	SoundFile "Vehicles\VehicleColl_Impact_04.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_06"
{
	SoundFile "Vehicles\VehicleColl_Impact_04.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_01"
{
	SoundFile "Vehicles\VehicleColl_Boom_01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_02"
{
	SoundFile "Vehicles\VehicleColl_Boom_02.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_03"
{
	SoundFile "Vehicles\VehicleColl_Boom_03.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_04"
{
	SoundFile "Vehicles\VehicleColl_Boom_03.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_01"
{
	SoundFile "Vehicles\VehicleColl_Debris_01.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_02"
{
	SoundFile "Vehicles\VehicleColl_Debris_02.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_03"
{
	SoundFile "Vehicles\VehicleColl_Debris_03.aif";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
    Falloff 0.3;
}




