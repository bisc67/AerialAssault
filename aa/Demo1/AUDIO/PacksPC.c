//              Pack SFX


/*
Complex	"SFX_PACK_PICKUP"
{
	SoundFile "packs\PickupPack.wav";
	Volume	0.4;
    Pitch	1.0;
    Priority    110;
}
*/

Complex	"SFX_PACK_THROW"
{
	SoundFile "PACKS\PackThrow01Mix01.wav";
	Volume	0.80;
    Pitch	1.0;
    Priority    90;
}

Complex	"SFX_PACK_SATCHEL_ACTIVATE"
{
	SoundFile "PACKS\satchel_pack_activate.wav";
	Volume	0.4;
    Pitch	1.0;
    Priority    100;
}

Complex	"SFX_PACK_SATCHEL_DETONATE"
{
	SoundFile "PACKS\satchel_pack_det_beep.wav"
        {
	        Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }
   	SoundFile "PACKS\satchel_pack_det_beep.wav"
        {
            Delay   0.286;
            Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }
	SoundFile "PACKS\satchel_pack_det_beep.wav"
        {
            Delay   0.572;
	        Volume	0.18;
            Pitch	1.0;
            Priority    90;
        }

}

Complex	"SFX_PACK_SATCHEL_EXPLOSION"
{
    SoundFile "EXPLOSIONS\Explosion01Mix01.wav"
    {
	    Volume	0.90;
	    Pitch	0.75;
        Priority    90;
    }

    SoundFile "EXPLOSIONS\Explosion03Mix01.wav"
    {
        Delay   0.35;
	    Volume	0.70;
	    Pitch	0.80;
        Priority    90;
    }

    SoundFile "EXPLOSIONS\Explosion02Mix01.wav"
    {
        Delay   0.55;
	    Volume	0.85;
	    Pitch	0.70;
        Priority    40;
    }

}


Complex	"SFX_PACK_SENSOR_JAMMER_LOOP"
{
	SoundFile "PACKS\sensorjammerpack_on_LP.wav";
	Volume	0.4;
    Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_PACK_SHIELD_ON_LOOP"
{
	SoundFile "PACKS\shield_on_LP.wav";
	Volume	0.48;
    Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_PACK_SHIELD_ACTIVATE"
{
	SoundFile "PACKS\Shield_Activate.wav";
	Volume	0.40;
    Pitch	1.0;
    Priority    -1;
}

/*
Complex	"SFX_PACK_SHIELD_IMPACT"
{
	SoundFile "packs\shield_hit.wav";
	Volume	0.4;
    Pitch	1.0;
    Priority    70;
}
*/

Complex	"SFX_PACK_INVENTORY_DEPLOY"
{
	SoundFile "PACKS\inventory_deploy.wav";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
}

Complex	"SFX_PACK_TURRET_BARREL_DEPLOY"
{
	SoundFile "PACKS\TurretDeploy01Mix02.wav";
	Volume	0.85;
    Pitch	1.0;
    Priority    90;
}

Complex	"SFX_PACK_SPIDER_DEPLOY"
{
	SoundFile "PACKS\turret_spider_deploy.wav";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
}

Complex	"SFX_PACK_SENSOR_DEPLOY"
{
	SoundFile "PACKS\Sensor_Deploy01.wav";
	Volume	0.55;
    Pitch	1.0;
    Priority    90;
}












