//          Turrets Script - Audio


//              Generic Turret Sounds - Heavy

Complex	"SFX_TURRET_HEAVY_ACTIVATE"
{
	SoundFile "Turrets\turret_heavy_activate.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}

Complex	"SFX_TURRET_HEAVY_DEACTIVATE"
{
	SoundFile "Turrets\TurretHeavDeactMove01.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}
/*
Complex	"SFX_TURRET_HEAVY_CLUNK"
{
	SoundFile "Turrets\TurretHeavDeactClunk01.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    85;
}


Complex	"SFX_TURRET_HEAVY_RELOAD"
{
	SoundFile "Turrets\turret_heavy_reload.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    80;
}
*/

/*
Complex	"SFX_TURRET_HEAVY_MOVE_LOOP"
{
	SoundFile "Turrets\TurretHeavyMove01_LP3.aif";
	Volume	0.70;
	Pitch	1.0;
    Priority    -1;      // Check to see if can change
}
*/

Complex	"SFX_TURRET_HEAVY_IDLE_LOOP"
{
	SoundFile "Turrets\turret_idle_LP1.aif";
	Volume	0.90;
	Pitch	0.75;
    Priority    -1;      // Check to see if can change
    Falloff 0.2;
}


/*
Complex	"SFX_TURRET_HEAVY_CLUNK_01"
{
	SoundFile "Turrets\TurretClunkHeavy01.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}

Complex	"SFX_TURRET_HEAVY_CLUNK_02"
{
	SoundFile "Turrets\TurretClunkHeavy02.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}
*/

/*
Complex	"SFX_TURRET_HEAVY_CLUNK_03"
{
	SoundFile "Turrets\TurretClunkHeavy03.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}

Complex	"SFX_TURRET_HEAVY_CLUNK_04"
{
	SoundFile "Turrets\TurretClunkHeavy04.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}
*/

//              Generic Turret Sounds - Light


Complex	"SFX_TURRET_LIGHT_ACTIVATE"
{
	SoundFile "Turrets\TurretLightActivate01Mix02.aif";
	Volume	0.88;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}

Complex	"SFX_TURRET_LIGHT_DEACTIVATE"
{
	SoundFile "Turrets\TurretLightDeact01Mix01.aif";
	Volume	0.88;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}

/*
Complex	"SFX_TURRET_LIGHT_RELOAD"
{
	SoundFile "Turrets\turret_light_reload.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_TURRET_LIGHT_MOVE_LOOP"
{
	SoundFile "Turrets\TurretLightMove01_LP2.aif";
	Volume	0.70;
	Pitch	1.0;
    Priority    -1;      // Check to see if can change
}


Complex	"SFX_TURRET_LIGHT_IDLE_LOOP"
{
	SoundFile "Turrets\turret_idle_LP1.aif";
	Volume	0.60;
	Pitch	1.3;
    Priority    -1;      // Check to see if can change
}
*/

/*
Complex	"SFX_TURRET_LIGHT_CLUNK_01"
{
	SoundFile "Turrets\TurretClunkLight01.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}

Complex	"SFX_TURRET_LIGHT_CLUNK_02"
{
	SoundFile "Turrets\TurretClunkLight02.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}
*/

/*
Complex	"SFX_TURRET_LIGHT_CLUNK_03"
{
	SoundFile "Turrets\TurretClunkLight03.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}

Complex	"SFX_TURRET_LIGHT_CLUNK_04"
{
	SoundFile "Turrets\TurretClunkLight04.aif";
	Volume	0.75;
	Pitch	1.0;
    Priority    65;
}
*/



//              Mortar Turret

Complex	"SFX_TURRET_MORTAR_FIRE"
{
	SoundFile "turrets\MortarTurretFire02Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_MORTAR_PROJECTILE_LOOP"
{
	SoundFile "weapons\MortarProjectile01Mix03_LP2.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.2;
}

Complex	"SFX_TURRET_MORTAR_IMPACT"
{
	SoundFile "Explosions\explosion02Mix01.aif";
	Volume	0.80;
	Pitch	0.68;
    Priority    90 ;
    Falloff 0.65;
}



//                Indoor Turret

/*
Complex	"SFX_TURRET_INDOOR_FIRE"
{
	SoundFile "turrets\IndoorFire01Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/


//                Plasma Turret


Complex	"SFX_TURRET_PLASMA_FIRE"
{
	SoundFile "turrets\PlasmaTurretFire02Mix01_b.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_PLASMA_IMPACT"
{
	SoundFile "turrets\turret_plasma_explode.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff 0.65;
}


//                AA Turret


Complex	"SFX_TURRET_AATURRET_FIRE"
{
	SoundFile "turrets\TurretAAFire02Mix01_b.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_AATURRET_IMPACT"
{
	SoundFile "Explosions\explosion03Mix01.aif";
	Volume	0.80;
	Pitch	1.2;
    Priority    90;
    Falloff 0.6;
}

/*
Complex	"SFX_TURRET_AATURRET_ACTIVATE"
{
	SoundFile "turrets\turret_aa_activate.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/


//                Sentry Turret


Complex	"SFX_TURRET_SENTRY_FIRE"
{
	SoundFile "turrets\SentryFire02Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_SENTRY_IMPACT"
{
	SoundFile "turrets\turret_sentry_impact.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff 0.30;
}

/*
Complex	"SFX_TURRET_SENTRY_ACTIVATE"
{
	SoundFile "turrets\turret_sentry_activate.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/



//                Missile Turret


Complex	"SFX_TURRET_MISSILE_FIRE"
{
	SoundFile "turrets\MissileTurretFire02Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_MISSILE_IMPACT"
{
	SoundFile "Explosions\explosion01Mix01.aif";
	Volume	0.80;
	Pitch	0.80;
    Priority    90;
    Falloff 0.55;
}

/*
Complex	"SFX_TURRET_MISSILE_ACTIVATE"
{
	SoundFile "turrets\turret_missile_activate.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/


//                ELF Turret

/*
Complex	"SFX_TURRET_ELF_FIRE_LOOP"
{
	SoundFile "weapons\ELFFire01Mix03_LP.aif";
	Volume	0.8;
	Pitch	0.87;
    Priority    -1;
}
*/

/*
//                Spike Turret


Complex	"SFX_TURRET_SPIKE_FIRE"
{
	SoundFile "turrets\IndoorFire01Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_TURRET_SPIKE_IMPACT"
{
	SoundFile "turrets\SpikeImpact01Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/



//                Spider Turret


Complex	"SFX_TURRET_SPIDER_FIRE"
{
	SoundFile "turrets\SentryFire03Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.4;
}

Complex	"SFX_TURRET_SPIDER_IMPACT"
{
	SoundFile "turrets\SpiderImpact01Mix01.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff 0.55;
}


Complex	"SFX_TURRET_REMOTE_ACTIVATE"
{
	SoundFile "Turrets\TurretLightActivate01Mix02.aif";
	Volume	0.85;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}

Complex	"SFX_TURRET_REMOTE_DEACTIVATE"
{
	SoundFile "Turrets\TurretLightDeact01Mix01.aif";
	Volume	0.85;
	Pitch	1.0;
    Priority    85;
    Falloff 0.35;
}







