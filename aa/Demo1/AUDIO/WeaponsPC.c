//          Weapons Script - Audio

//          Waepon default vaules

//Default Priority 80


//              Spinfusor



Complex	"SFX_WEAPONS_SPINFUSORFIRE02MIX01"
{
        SoundFile "WEAPONS\SpinfusorFire02mix02.wav";
	Volume	0.90;
    Pitch	1.0;
    Priority    90;
}


Complex	"SFX_WEAPONS_SPINFUSOR_ACTIVATE"
{
        SoundFile "WEAPONS\grenadelauncher_activate.wav";
	Volume	0.55;
	Pitch	1.2;
    Priority    75;
}

/*
Complex	"SFX_WEAPONS_SPINFUSOR_DRYFIRE"
{
	SoundFile "weapons\spinfusor_dryfire.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_SPINFUSOR_IDLE_LP"
{
	SoundFile "WEAPONS\spinfusor_idle_LP.wav";
	Volume	0.30;
	Pitch	1.0;
    Priority    -1;      // Check to see if can be changed
}

Complex	"SFX_WEAPONS_SPINFUSOR_IMPACT"
{
        SoundFile "EXPLOSIONS\explosion01mix01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_SPINFUSOR_PROJECTILE_LP"
{
        SoundFile "WEAPONS\spinfusor_projectile_LP.wav";
	Volume	0.42;
	Pitch	1.0;
    Priority    -1;      // Check to see if can be changed
    Falloff 0.2;
}

Complex	"SFX_WEAPONS_SPINFUSOR_RELOAD"
{
        SoundFile "WEAPONS\spinfusor_reload.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    80;
}


//             Mortar

Complex	"SFX_WEAPONS_MORTAR_ACTIVATE"
{
        SoundFile "WEAPONS\mortar_activate.wav";
	Volume	0.51;
	Pitch	1.0;
    Priority    75;
}

Complex	"SFX_WEAPONS_MORTAR_FIRE"
{
        SoundFile "WEAPONS\MortarFire01Mix01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}

/*
Complex	"SFX_WEAPONS_MORTAR_DRYFIRE"
{
        SoundFile "WEAPONS\mortar_dryfire.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_MORTAR_RELOAD"
{
        SoundFile "WEAPONS\MortarReload01Mix01.wav"
    {
    	Volume	0.35;
	    Pitch	1.0;
        Priority    80;
        Delay   0.25;
    }
}

Complex	"SFX_WEAPONS_MORTAR_PROJECTILE_LOOP"
{
        SoundFile "WEAPONS\MortarProjectile01Mix03_LP1.wav";
	Volume	0.65;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.50;
}

Complex	"SFX_WEAPONS_MORTAR_IMPACT"
{
        SoundFile "WEAPONS\mortar_explode.wav";
	Volume	0.85;
	Pitch	1.0;
    Priority    90;
}
Complex	"SFX_WEAPONS_MORTARSHELL_IMPACT_HARD"
{
        SoundFile "WEAPONS\MortarShellImpactHard02.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_WEAPONS_MORTARSHELL_IMPACT_SOFT"
{
        SoundFile "WEAPONS\MortarShellImpactSoft01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}



//              Plasma Gun

Complex	"SFX_WEAPONS_PLASMA_FIRE"
{
        SoundFile "WEAPONS\PlasmaFire02Mix01.wav";
	Volume	1.2;
	Pitch	1.0;
    Priority    90;
}

/*
Complex	"SFX_WEAPONS_PLASMA_DRYFIRE"
{
        SoundFile "WEAPONS\plasma_dryfire.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_PLASMA_ACTIVATE"
{
        SoundFile "WEAPONS\plasma_rifle_activate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
}

Complex	"SFX_WEAPONS_PLASMA_RELOAD"
{
    SoundFile "WEAPONS\plasma_rifle_reload.wav"
    {
       	Volume	0.32;
	    Pitch	1.0;
        Priority    80;
        Delay   0.33;
    }
}

Complex	"SFX_WEAPONS_PLASMA_IDLE_LOOP"
{
        SoundFile "WEAPONS\plasma_rifle_idle_LP.wav";
	Volume	0.30;
	Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_WEAPONS_PLASMA_PROJECTILE_LOOP"
{
        SoundFile "WEAPONS\PlasmaProjectile01Mix01_LP2.wav";
	Volume	0.75;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.35;
}

Complex	"SFX_WEAPONS_PLASMA_PROJECTILE_IMPACT"
{
        SoundFile "WEAPONS\PlasmaImpact03Mix02a_TC.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}


//              Chaingun

Complex	"SFX_WEAPONS_CHAINGUN_FIRE_LOOP"
{
        SoundFile "WEAPONS\ChaingunFire03Mix01_Loop.wav";    //  Loop!!!
	Volume	0.78;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_CHAINGUN_FIRE"
{
        SoundFile "WEAPONS\ChaingunFire03Mix01_Loop_End.wav";    //  Oneshot!!!
	Volume	0.78;
	Pitch	1.0;
    Priority    90;
}

/*
Complex	"SFX_WEAPONS_CHAINGUN_DRYFIRE"
{
        SoundFile "WEAPONS\chaingun_dryfire.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_WEAPONS_CHAINGUN_ACTIVATE"
{
        SoundFile "WEAPONS\chaingun_activate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
}

Complex	"SFX_WEAPONS_CHAINGUN_SPINUP_LOOP"
{
        SoundFile "WEAPONS\chaingun_spinup_LP1.wav";
	Volume	0.35;
	Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_WEAPONS_CHAINGUN_SPINDOWN"
{
        SoundFile "WEAPONS\chaingun_spindown.wav";
	Volume	0.35;
	Pitch	1.0;
    Priority    70;
}

//                  Bullet Impacts

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_01"
{
        SoundFile "WEAPONS\BulletImpMetal01.wav";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_02"
{
        SoundFile "WEAPONS\BulletImpMetal02.wav";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_03"
{
        SoundFile "WEAPONS\BulletImpMetal03.wav";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_04"
{
        SoundFile "WEAPONS\BulletImpMetal04.wav";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_05"
{
        SoundFile "WEAPONS\BulletImpMetal05.wav";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_06"
{
        SoundFile "WEAPONS\BulletRicochet04.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_07"
{
        SoundFile "WEAPONS\BulletRicochet05.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_08"
{
        SoundFile "WEAPONS\BulletRicochet06.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_09"
{
        SoundFile "WEAPONS\BulletRicochet01.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_10"
{
        SoundFile "WEAPONS\BulletRicochet02.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_11"
{
        SoundFile "WEAPONS\BulletRicochet03.wav";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_01"
{
        SoundFile "WEAPONS\BulletImpactTerrain01.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_02"
{
        SoundFile "WEAPONS\BulletImpactTerrain02.wav";
	Volume	0.58;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_03"
{
        SoundFile "WEAPONS\BulletImpactTerrain03.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_04"
{
        SoundFile "WEAPONS\BulletRicochet01.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_05"
{
        SoundFile "WEAPONS\BulletRicochet02.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_06"
{
        SoundFile "WEAPONS\BulletRicochet03.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_01"
{
        SoundFile "WEAPONS\BulletImpactTerrain01.wav";
	Volume	0.50;
	Pitch	0.88;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_02"
{
        SoundFile "WEAPONS\BulletImpactTerrain02.wav";
	Volume	0.60;
	Pitch	0.88;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_03"
{
        SoundFile "WEAPONS\BulletImpactTerrain03.wav";
	Volume	0.50;
	Pitch	0.88;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_04"
{
        SoundFile "WEAPONS\BulletRicochet01.wav";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_05"
{
        SoundFile "WEAPONS\BulletRicochet02.wav";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_06"
{
        SoundFile "WEAPONS\BulletRicochet03.wav";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
}


//                  Bullet Flybys
/*
Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_01"
{
        SoundFile "WEAPONS\BulletFlyBy01.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_02"
{
        SoundFile "WEAPONS\BulletFlyBy02.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_03"
{
        SoundFile "WEAPONS\BulletFlyBy03.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_04"
{
        SoundFile "WEAPONS\BulletFlyBy04.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_05"
{
        SoundFile "WEAPONS\BulletFlyBy05.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}
*/

//              Grenade Launcher

Complex	"SFX_WEAPONS_GRENADELAUNCHER_FIRE"
{
        SoundFile "WEAPONS\GrenLaunchFire02Mix01.wav";
	Volume	0.79;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_ACTIVATE"
{
        SoundFile "WEAPONS\grenadelauncher_activate.wav";
	Volume	0.55;
	Pitch	0.95;
    Priority    75;
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_RELOAD"
{
        SoundFile "WEAPONS\generic_switch.wav"
    {
	    Volume	0.25;
	    Pitch	1.0;
        Priority    80;
        Delay   0.2;
    }
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_PROJECTILE_LOOP"
{
        SoundFile "WEAPONS\GrenadeProjectile01Mix02.wav";
	Volume	0.65;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.50;
}


//              Grenades

Complex	"SFX_WEAPONS_GRENADE_THROW"
{
        SoundFile "WEAPONS\GrenadeThrow01Mix01.wav";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_GRENADE_BOUNCE_HARD"
{
        Flags channelsaver;
        SoundFile "WEAPONS\GrenadeImpactHard01.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_WEAPONS_GRENADE_BOUNCE_SOFT"
{
        Flags channelsaver;
        SoundFile "WEAPONS\GrenadeImpactSoft01.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
}

Complex	"SFX_WEAPONS_GRENADE_IMPACT"
{
        SoundFile "EXPLOSIONS\explosion02Mix01.wav";
	Volume	0.83;
	Pitch	1.0;
    Priority    90;
}

/*
Complex	"SFX_WEAPONS_GRENADE_SWITCH"
{
        SoundFile "WEAPONS\grenade_switch.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_GRENADE_FLASH_EXPLODE"
{
        SoundFile "WEAPONS\grenade_flash_explode.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}


//              Sniper Rifle

Complex	"SFX_WEAPONS_SNIPERRIFLE_FIRE"
{
        SoundFile "WEAPONS\SniperFire02Mix01_b.wav";
	Volume	0.68;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_SNIPERRIFLE_ACTIVATE"
{
        SoundFile "WEAPONS\sniper_activate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
}

Complex	"SFX_WEAPONS_SNIPERRIFLE_IMPACT"
{
        SoundFile "WEAPONS\SniperImpact03Mix01_b.wav";
	Volume	0.70;
	Pitch	1.0;
    Priority    90;
}


//              Blaster

Complex	"SFX_WEAPONS_BLASTER_FIRE"
{
        SoundFile "WEAPONS\BlasterFire09Mix01.wav";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_BLASTER_ACTIVATE"
{
        SoundFile "WEAPONS\blaster_activate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
}

Complex	"SFX_WEAPONS_BLASTER_IMPACT"
{
        SoundFile "WEAPONS\blaster_impact.wav";
	Volume	0.70;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_BLASTER_PROJECTILE_RICOCHET"
{
        SoundFile "WEAPONS\BlasterRicochet01Mix01.wav";
    Flags ChannelSaver;
	Volume	0.50;
	Pitch	0.70;
    Priority    70;
}


//              Missile Launcher

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRE"
{
        SoundFile "WEAPONS\MissLaunchFire02Mix01.wav";
	Volume	0.85;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_ACTIVATE"
{
        SoundFile "WEAPONS\missile_launcher_activate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_MISSILELAUNCHER_RELOAD"
{
        SoundFile "WEAPONS\mortar_reload.wav"
    {
    	Volume	0.35;
	    Pitch	0.70;
        Priority    80;
        Delay   0.15;
    }

        SoundFile "WEAPONS\plasma_rifle_reload.wav"
    {
    	Volume	0.25;
	    Pitch	0.60;
        Priority    80;
        Delay   0.35;
    }
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_PROJECTILE_LOOP"
{
        SoundFile "WEAPONS\missile_projectile.wav";
	Volume	0.65;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.40;
}

/*
Complex	"SFX_WEAPONS_MISSLELAUNCHER_DRYFIRE"
{
        SoundFile "WEAPONS\missile_launcher_dryfire.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_SEARCH"
{
        SoundFile "WEAPONS\missile_target_inbound.wav";
	Volume	0.60;
	Pitch	0.80;
    Priority    70;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRER_SEARCH"
{
        SoundFile "WEAPONS\missile_firer_search.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    70;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRER_LOCK"
{
        SoundFile "WEAPONS\missile_firer_search.wav";
	Volume	0.60;
	Pitch	1.15;
    Priority    70;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_LOCK"
{
        SoundFile "WEAPONS\missile_target_lock.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    70;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_INBOUND"
{
        SoundFile "WEAPONS\missile_target_inbound.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    70;
}
 

//              Targeting Laser

Complex	"SFX_WEAPONS_TARGETLASER_PAINT_LOOP"
{
        SoundFile "WEAPONS\TargetLaserPaint01Mix01_b_LP.wav";
	Volume	0.50;
	Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_WEAPONS_TARGETLASER_ACTIVATE"
{
        SoundFile "WEAPONS\generic_switch.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
}


//              ELF Gun

Complex	"SFX_WEAPONS_ELF_FIRE_LOOP"
{
        SoundFile "WEAPONS\ELFFire01Mix03_LP.wav";
	Volume	0.60;
	Pitch	1.0;
    Priority    -1;
}

Complex	"SFX_WEAPONS_ELF_ACTIVATE"
{
        SoundFile "WEAPONS\generic_switch.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
}


//              Repair Tool

Complex	"SFX_WEAPONS_REPAIR_USE"
{
        SoundFile "WEAPONS\RepairUse01Mix02_LP.wav";
	Volume	0.35;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_REPAIR_ACTIVATE"
{
        SoundFile "PACKS\RepairPackActivate.wav";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
}


//              Mines

Complex	"SFX_WEAPONS_MINE_THROW"
{
        SoundFile "WEAPONS\GrenadeThrow01Mix01.wav";
	Volume	0.80;
	Pitch	0.95;
    Priority    90;
}

Complex	"SFX_WEAPONS_MINE_DEPLOY"
{
        SoundFile "WEAPONS\mine_deploy.wav";
	Volume	0.65;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_MINE_DETONATE"
{
        SoundFile "WEAPONS\mine_detonate.wav";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_WEAPONS_MINE_FIZZLE"
{
        SoundFile "WEAPONS\MineFizzle01Mix01.wav";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
}

//              Generic Weapon

Complex	"SFX_WEAPONS_OUT_OF_AMMO_ALL"
{
        SoundFile "WEAPONS\OutOfAmmo04.wav";
	Volume	0.65;
	Pitch	1.0;
    Priority    85;
}


//              Explosions

Complex	"SFX_EXPLOSIONS_EXPLOSION01"
{
        SoundFile "EXPLOSIONS\Explosion01Mix01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_EXPLOSIONS_EXPLOSION02"
{
        SoundFile "EXPLOSIONS\Explosion02Mix01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}

Complex	"SFX_EXPLOSIONS_EXPLOSION03"
{
        SoundFile "EXPLOSIONS\Explosion03Mix01.wav";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}


