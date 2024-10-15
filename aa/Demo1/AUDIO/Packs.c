//              Pack SFX


/*
Complex	"SFX_PACK_PICKUP"
{
	SoundFile "packs\PickupPack.aif";
	Volume	0.4;
    Pitch	1.0;
    Priority    110;
}


Complex	"SFX_PACK_THROW"
{
	SoundFile "packs\PackThrow01Mix01.aif";
	Volume	0.80;
    Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_PACK_SATCHEL_ACTIVATE"
{
	SoundFile "packs\satchel_pack_activate.aif";
	Volume	0.4;
    Pitch	1.0;
    Priority    100;
    Falloff 0.3;
}

Complex	"SFX_PACK_SATCHEL_DETONATE"
{
    Falloff 0.5;
	SoundFile "packs\satchel_pack_det_beep.aif"
        {
	        Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }
   	SoundFile "packs\satchel_pack_det_beep.aif"
        {
            Delay   0.286;
            Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }
	SoundFile "packs\satchel_pack_det_beep.aif"
        {
            Delay   0.572;
	        Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }
}

Complex	"SFX_PACK_SATCHEL_EXPLOSION"
{
    SoundFile "Explosions\explosion01Mix01.aif"
    {
	    Volume	0.90;
	    Pitch	0.75;
        Priority    90;
    }

    SoundFile "Explosions\explosion03Mix01.aif"
    {
        Delay   0.35;
	    Volume	0.70;
	    Pitch	0.80;
        Priority    90;
    }

    SoundFile "Explosions\explosion02Mix01.aif"
    {
        Delay   0.55;
	    Volume	0.85;
	    Pitch	0.70;
        Priority    40;
    }

}

/*
Complex	"SFX_PACK_SENSOR_JAMMER_LOOP"
{
	SoundFile "packs\sensorjammerpack_on_LP.aif";
	Volume	0.4;
    Pitch	1.0;
    Priority    -1;
}
*/

Complex	"SFX_PACK_SHIELD_ON_LOOP"
{
	SoundFile "packs\shield_on_LP.aif";
	Volume	0.48;
    Pitch	1.0;
    Priority    -1;
    Falloff 0.3;
}

Complex	"SFX_PACK_SHIELD_ACTIVATE"
{
	SoundFile "packs\Shield_Activate.aif";
	Volume	0.40;
    Pitch	1.0;
    Priority    -1;
    Falloff 0.3;
}

/*
Complex	"SFX_PACK_SHIELD_IMPACT"
{
	SoundFile "packs\shield_hit.aif";
	Volume	0.4;
    Pitch	1.0;
    Priority    70;
}
*/

Complex	"SFX_PACK_INVENTORY_DEPLOY"
{
	SoundFile "packs\inventory_deploy.aif";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
    Falloff 0.3;
}

Complex	"SFX_PACK_TURRET_BARREL_DEPLOY"
{
	SoundFile "packs\TurretDeploy01Mix02.aif";
	Volume	0.85;
    Pitch	1.0;
    Priority    90;
    Falloff 0.3;
}

Complex	"SFX_PACK_SPIDER_DEPLOY"
{
	SoundFile "packs\turret_spider_deploy.aif";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
    Falloff 0.3;
}

Complex	"SFX_PACK_SENSOR_DEPLOY"
{
	SoundFile "packs\Sensor_Deploy01.aif";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
    Falloff 0.3;
}












