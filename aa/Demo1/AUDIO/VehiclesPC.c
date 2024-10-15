//          Vehicles Script - Audio


//              Wildcat Vehicle

Complex	"SFX_VEHICLE_WILDCAT_ENGINE_LOOP"
{
	SoundFile "VEHICLES\VehicleLight02Mix01_LP1.wav";
	Volume	0.60;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.78;
}

/*
Complex	"SFX_VEHICLE_WILDCAT_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.6;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_WILDCAT_AFTERBURNER_LOOP"
{
	SoundFile "ARMOR\thrust_LP1.wav";
	Volume	0.85;
	Pitch	0.8;
    Priority    -1;
    Falloff 0.78;
}


//              Shrike Vehicle

Complex	"SFX_VEHICLE_SHRIKE_ENGINE_LOOP"
{
	SoundFile "VEHICLES\VehicleLight02Mix01_LP1.wav";
	Volume	0.70;
	Pitch	0.95;
    Priority    -1;
    Falloff 0.78;
}

/*
Complex	"SFX_VEHICLE_SHRIKE_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.53;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_SHRIKE_AFTERBURNER_LOOP"
{
	SoundFile "VEHICLES\AfterBurnerSmall01Mix02_LP1.wav";
	Volume	0.75;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.78;
}

Complex	"SFX_VEHICLE_SHRIKE_WEAPON_FIRE"
{
	SoundFile "WEAPONS\BlasterFire09Mix01.wav";
	Volume	0.90;
	Pitch	0.85;
    Priority    90;
}


//              Beowulf Vehicle

Complex	"SFX_VEHICLE_BEOWULF_ENGINE_LOOP"
{
	SoundFile "VEHICLES\VehicleHeavy02Mix01_LP1.wav";
	Volume	0.65;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.78;
}

/*
Complex	"SFX_VEHICLE_BEOWULF_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.48;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_BEOWULF_AFTERBURNER_LOOP"
{
	SoundFile "VEHICLES\AfterBurnerSmall01Mix02_LP1.wav";
	Volume	0.85;
	Pitch	0.90;
    Priority    -1;
    Falloff 0.78;
}


//              Thundersword Vehicle

Complex	"SFX_VEHICLE_THUNDERSWORD_ENGINE_LOOP"
{
	SoundFile "VEHICLES\VehicleHeavy02Mix01_LP1.wav";
	Volume	0.75;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_THUNDERSWORD_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.44;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_THUNDERSWORD_AFTERBURNER_LOOP"
{
	SoundFile "VEHICLES\AfterBurnerHuge01Mix02_LP1.wav";
	Volume	0.85;
	Pitch	1.15;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_RELEASE"
{
	SoundFile "Vehicles\BombRelease01Mix01.wav";
	Volume	0.85;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_DROP_LOOP"
{
	SoundFile "VEHICLES\bomber_bomb_projectile.wav";
	Volume	0.85;
	Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_VEHICLE_THUNDERSWORD_BOMB_EXPLOSION"
{
	SoundFile "EXPLOSIONS\Explosion02Mix01.wav"
    {
        Volume	0.75;
	    Pitch	0.80;
        Priority    90;
    }

    SoundFile "EXPLOSIONS\Explosion03Mix01.wav"
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
	SoundFile "VEHICLES\VehicleMedium02Mix02_LP1.wav";
	Volume	0.80;
	Pitch	1.1;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_HAVOC_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.4;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_HAVOC_AFTERBURNER_LOOP"
{
	SoundFile "VEHICLES\AfterBurnerHuge01Mix02_LP1.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.45;
}


//              Jericho Vehicle

Complex	"SFX_VEHICLE_JERICHO_ENGINE_LOOP"
{
	SoundFile "VEHICLES\VehicleMedium02Mix02_LP1.wav";
	Volume	0.80;
	Pitch	0.87;
    Priority    -1;
    Falloff 0.45;
}

/*
Complex	"SFX_VEHICLE_JERICHO_WHINE_LOOP"
{
	SoundFile "Vehicles\VehicleSineWhine_LP1.wav";
	Volume	0.25;
	Pitch	0.37;
    Priority    -1;
    Falloff 0.45;
}
*/

Complex	"SFX_VEHICLE_JERICHO_AFTERBURNER_LOOP"
{
	SoundFile "VEHICLES\AfterBurnerHuge01Mix02_LP1.wav";
	Volume	0.95;
	Pitch	0.85;
    Priority    -1;
    Falloff 0.45;
}

Complex	"SFX_VEHICLE_JERICHO_MPB_DEPLOY"
{
	Streamed SoundFile "VEHICLES\MPB_deploy.wav";
	Volume	0.90;
	Pitch	0.80;
    Priority    90;
}

Complex	"SFX_VEHICLE_JERICHO_MPB_UNDEPLOY"
{
	Streamed SoundFile "VEHICLES\MPB_undeploy_turret.wav";
	Volume	0.90;
	Pitch	0.80;
    Priority    90;
}

//                  Vehicle Collisions


Complex	"SFX_VEHICLE_LIGHT_COLLIDE_TERRAIN"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_LIGHT_COLLIDE_BUILDING"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}


Complex	"SFX_VEHICLE_HEAVY_COLLIDE_TERRAIN"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_HEAVY_COLLIDE_BUILDING"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_SKID_TERRAIN_MEDIUM"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    50;
}

Complex	"SFX_VEHICLE_SKID_TERRAIN_HARD"
{
	SoundFile "MISC\OBWarning01.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    50;
}

Complex	"SFX_VEHICLE_MOUNT"
{
	SoundFile "VEHICLES\mount_dis.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    50;
}

Complex	"SFX_VEHICLE_DISMOUNT"
{
	SoundFile "VEHICLES\mount.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    50;
}


//              New collision SFX

Complex	"SFX_VEHICLE_COLLISION_IMPACT_01"
{
	SoundFile "VEHICLES\VehicleColl_Impact_01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}


/*
Complex	"SFX_VEHICLE_COLLISION_IMPACT_02"
{
	SoundFile "Vehicles\VehicleColl_Impact_02.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_VEHICLE_COLLISION_IMPACT_03"
{
	SoundFile "VEHICLES\VehicleColl_Impact_03.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_IMPACT_04"
{
	SoundFile "VEHICLES\VehicleColl_Impact_04.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_01"
{
	SoundFile "VEHICLES\VehicleColl_Boom_01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_BOOM_02"
{
	SoundFile "VEHICLES\VehicleColl_Boom_02.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

/*
Complex	"SFX_VEHICLE_COLLISION_BOOM_03"
{
	SoundFile "Vehicles\VehicleColl_Boom_03.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_VEHICLE_COLLISION_BOOM_04"
{
	SoundFile "VEHICLES\VehicleColl_Boom_04.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_01"
{
	SoundFile "VEHICLES\VehicleColl_Debris_01.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_02"
{
	SoundFile "VEHICLES\VehicleColl_Debris_02.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_VEHICLE_COLLISION_DEBRIS_03"
{
	SoundFile "VEHICLES\VehicleColl_Debris_03.wav";
	Volume	0.7;
	Pitch	1.0;
    Priority    80;
}




