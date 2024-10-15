//          Weapons Script - Audio

//          Waepon default vaules

//Default Priority 80


//              Spinfusor



Complex	"SFX_WEAPONS_SPINFUSORFIRE02MIX01"
{
	SoundFile "weapons\SpinfusorFire02mix02.aif";
	Volume	    0.85;
    Pitch	    1.0;
    Priority    90;
    Falloff     0.35;
}


Complex	"SFX_WEAPONS_SPINFUSOR_ACTIVATE"
{

    Falloff     0.05;
    SoundFile "weapons\blaster_activate.aif"  
    {
	    Volume	    0.40;
	    Pitch	    1.2;
        Priority    75;
    }
    
    SoundFile "weapons\spinfusor_reload.aif"
    {
        Delay       0.30;
        Volume	    0.4;
	    Pitch	    1.2;
        Priority    75;
    }
}

/*
Complex	"SFX_WEAPONS_SPINFUSOR_DRYFIRE"
{
	SoundFile "weapons\spinfusor_dryfire.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_SPINFUSOR_IDLE_LP"
{
	SoundFile "weapons\spinfusor_idle_LP.aif";
	Volume	    0.30;
	Pitch	    1.0;
    Priority    -1;      // Check to see if can be changed
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_SPINFUSOR_IMPACT"
{
	SoundFile "Explosions\explosion01mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.9;
}

Complex	"SFX_WEAPONS_SPINFUSOR_PROJECTILE_LP"
{
	SoundFile "weapons\spinfusor_projectile_LP.aif";
	Volume	0.42;
	Pitch	1.0;
    Priority    -1;      // Check to see if can be changed
    Falloff 0.1;
}

Complex	"SFX_WEAPONS_SPINFUSOR_RELOAD"
{
	SoundFile "weapons\spinfusor_reload.aif";
	Volume	    0.62;
	Pitch	    1.0;
    Priority    80;
    Falloff     0.2;
}


//             Mortar

Complex	"SFX_WEAPONS_MORTAR_ACTIVATE"
{
	SoundFile "weapons\mortar_activate.aif";
	Volume	    0.51;
	Pitch	    1.0;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_MORTAR_FIRE"
{
	SoundFile "weapons\MortarFire01Mix01.aif";
	Volume	    0.85;
	Pitch	    1.0;
    Priority    90;
    Falloff     0.25;
}

/*
Complex	"SFX_WEAPONS_MORTAR_DRYFIRE"
{
	SoundFile "weapons\mortar_dryfire.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_MORTAR_RELOAD"
{
    Falloff     0.2;
	SoundFile "weapons\MortarReload01Mix01.aif"
    {
  	    Volume	    0.35;
        Pitch	    1.0;
        Priority    80;
        Delay       0.25;
    }
}

Complex	"SFX_WEAPONS_MORTAR_PROJECTILE_LOOP"
{
	SoundFile "weapons\MortarProjectile01Mix03_LP2.aif";
	Volume	    0.65;
	Pitch	    1.0;
    Priority    -1;
    Falloff     0.20;
}

Complex	"SFX_WEAPONS_MORTAR_IMPACT"
{
	SoundFile "weapons\mortar_explode.aif";
	Volume	    0.85;
	Pitch	    1.0;
    Priority    90;
    Falloff 0.9;
}
Complex	"SFX_WEAPONS_MORTARSHELL_IMPACT_HARD"
{
    Flags channelsaver;
	SoundFile "weapons\MortarShellImpactHard02.aif";
	Volume	    0.90;
	Pitch	    1.0;
    Priority    80;
    Falloff     0.4;
}

Complex	"SFX_WEAPONS_MORTARSHELL_IMPACT_SOFT"
{
    Flags channelsaver;
	SoundFile "weapons\MortarShellImpactSoft01.aif";
	Volume	    0.90;
	Pitch	    1.0;
    Priority    80;
    Falloff     0.4;
}



//              Plasma Gun

Complex	"SFX_WEAPONS_PLASMA_FIRE"
{
	SoundFile "weapons\PlasmaFire02Mix01.aif";
	Volume	    1.0;
	Pitch	    1.0;
    Priority    90;
    Falloff     0.2;
}

/*
Complex	"SFX_WEAPONS_PLASMA_DRYFIRE"
{
	SoundFile "weapons\plasma_dryfire.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_PLASMA_ACTIVATE"
{
	SoundFile "weapons\plasma_rifle_activate.aif";
	Volume	    0.50;
	Pitch   	1.0;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_PLASMA_RELOAD"
{
    Falloff     0.1;
    SoundFile "weapons\plasma_rifle_reload.aif"
    {
   	    Volume	    0.30;
        Pitch	    1.0;
        Priority    80;
        Delay       0.33;
    }
}

Complex	"SFX_WEAPONS_PLASMA_IDLE_LOOP"
{
	SoundFile "weapons\plasma_rifle_idle_LP.aif";
	Volume	    0.30;
	Pitch	    1.0;
    Priority    -1;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_PLASMA_PROJECTILE_LOOP"
{
	SoundFile "weapons\PlasmaProjectile01Mix01_LP2.aif";
	Volume	    0.75;
	Pitch	    1.0;
    Priority    -1;
    Falloff     0.15;
}

Complex	"SFX_WEAPONS_PLASMA_PROJECTILE_IMPACT"
{
	SoundFile "weapons\PlasmaImpact03Mix02a_TC.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff 0.9;
}


//              Chaingun

Complex	"SFX_WEAPONS_CHAINGUN_FIRE_LOOP"
{
	SoundFile "weapons\ChaingunFire03Mix01_Loop.aif";    //  Loop!!!
	Volume	    0.75;
	Pitch   	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_CHAINGUN_FIRE"
{
	SoundFile "weapons\ChaingunFire03Mix01_Loop_End.aif";    //  Oneshot!!!
	Volume	    0.75;
	Pitch   	1.0;
    Priority    90;
    Falloff     0.2;
}

/*
Complex	"SFX_WEAPONS_CHAINGUN_DRYFIRE"
{
	SoundFile "weapons\chaingun_dryfire.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_WEAPONS_CHAINGUN_ACTIVATE"
{
	SoundFile "weapons\chaingun_activate.aif";
	Volume  	0.50;
	Pitch   	1.0;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_CHAINGUN_SPINUP_LOOP"
{
	SoundFile "weapons\chaingun_spinup_LP1.aif";
	Volume	    0.35;
	Pitch	    1.0;
    Priority    -1;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_CHAINGUN_SPINDOWN"
{
	SoundFile "weapons\chaingun_spindown.aif";
	Volume	    0.35;
	Pitch	    1.0;
    Priority    70;
    Falloff     0.2;
}

//                  Bullet Impacts

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_01"
{
	SoundFile "weapons\BulletImpMetal01.aif";
	Volume	    0.675;
	Pitch	    1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_02"
{
	SoundFile "weapons\BulletImpMetal02.aif";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_03"
{
	SoundFile "weapons\BulletImpMetal03.aif";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_04"
{
	SoundFile "weapons\BulletImpMetal04.aif";
	Volume	0.675;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_05"
{
	SoundFile "weapons\BulletImpMetal04.aif";
	Volume	0.675;
	Pitch	1.2;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_06"
{
	SoundFile "weapons\BulletRicochet04.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_07"
{
	SoundFile "weapons\BulletRicochet05.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_08"
{
	SoundFile "weapons\BulletRicochet06.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_09"
{
	SoundFile "weapons\BulletRicochet01.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_10"
{
	SoundFile "weapons\BulletRicochet02.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_ARMOR_11"
{
	SoundFile "weapons\BulletRicochet03.aif";
	Volume	0.50;
	Pitch	1.15;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_01"
{
	SoundFile "weapons\BulletImpactTerrain01.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_02"
{
	SoundFile "weapons\BulletImpactTerrain02.aif";
	Volume	0.58;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_03"
{
	SoundFile "weapons\BulletImpactTerrain03.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_04"
{
	SoundFile "weapons\BulletRicochet01.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_05"
{
	SoundFile "weapons\BulletRicochet02.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_TERRAIN_06"
{
	SoundFile "weapons\BulletRicochet03.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_01"
{
	SoundFile "weapons\BulletImpactTerrain01.aif";
	Volume	0.50;
	Pitch	0.88;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_02"
{
	SoundFile "weapons\BulletImpactTerrain02.aif";
	Volume	0.60;
	Pitch	0.88;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_03"
{
	SoundFile "weapons\BulletImpactTerrain03.aif";
	Volume	0.50;
	Pitch	0.88;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_04"
{
	SoundFile "weapons\BulletRicochet01.aif";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_05"
{
	SoundFile "weapons\BulletRicochet02.aif";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
    Falloff     0.45;
}

Complex	"SFX_WEAPONS_CHAINGUN_IMPACT_BUILDING_06"
{
	SoundFile "weapons\BulletRicochet03.aif";
	Volume	0.50;
	Pitch	0.92;
    Priority    20;
    Falloff     0.45;
}


//                  Bullet Flybys
/*
Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_01"
{
	SoundFile "weapons\BulletFlyBy01.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_02"
{
	SoundFile "weapons\BulletFlyBy02.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_03"
{
	SoundFile "weapons\BulletFlyBy03.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_04"
{
	SoundFile "weapons\BulletFlyBy04.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}

Complex	"SFX_WEAPONS_CHAINGUN_FLYBY_05"
{
	SoundFile "weapons\BulletFlyBy05.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    20;
}
*/

//              Grenade Launcher

Complex	"SFX_WEAPONS_GRENADELAUNCHER_FIRE"
{
	SoundFile "weapons\GrenLaunchFire02Mix01.aif";
	Volume	0.79;
	Pitch	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_ACTIVATE"
{
	SoundFile "weapons\grenadelauncher_activate.aif";
	Volume	0.50;
	Pitch	0.95;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_RELOAD"
{
    Falloff     0.1;
	SoundFile "weapons\generic_switch.aif"
    {
        Volume	0.25;
        Pitch	1.0;
        Priority    80;
        Delay   0.2;
    }
}

Complex	"SFX_WEAPONS_GRENADELAUNCHER_PROJECTILE_LOOP"
{
	SoundFile "weapons\GrenadeProjectile01Mix02.aif";
	Volume	0.65;
	Pitch	1.2;
    Priority    -1;
    Falloff 0.23;
}


//              Grenades

Complex	"SFX_WEAPONS_GRENADE_THROW"
{
	SoundFile "weapons\GrenadeThrow01Mix01.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_GRENADE_BOUNCE_HARD"
{
    Flags channelsaver;
	SoundFile "weapons\GrenadeImpactHard01.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
    Falloff     0.3;
}

Complex	"SFX_WEAPONS_GRENADE_BOUNCE_SOFT"
{
    Flags channelsaver;
	SoundFile "weapons\GrenadeImpactSoft01.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
    Falloff     0.3;
}

Complex	"SFX_WEAPONS_GRENADE_IMPACT"
{
	SoundFile "Explosions\explosion02Mix01.aif";
	Volume	0.83;
	Pitch	1.0;
    Priority    90;
}

/*
Complex	"SFX_WEAPONS_GRENADE_SWITCH"
{
	SoundFile "Weapons\grenade_switch.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
}
*/

Complex	"SFX_WEAPONS_GRENADE_FLASH_EXPLODE"
{
	SoundFile "Weapons\grenade_flash_explode.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff     0.9;
}


//              Sniper Rifle

Complex	"SFX_WEAPONS_SNIPERRIFLE_FIRE"
{
	SoundFile "Weapons\SniperFire02Mix01_b.aif";
	Volume	0.68;
	Pitch	1.0;
    Priority    90;
    Falloff     0.1;
}

Complex	"SFX_WEAPONS_SNIPERRIFLE_ACTIVATE"
{
	SoundFile "Weapons\sniper_activate.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_SNIPERRIFLE_IMPACT"
{
	SoundFile "Weapons\SniperImpact03Mix01_b.aif";
	Volume	0.65;
	Pitch	1.0;
    Priority    90;
    Falloff     0.4;
}


//              Blaster

Complex	"SFX_WEAPONS_BLASTER_FIRE"
{
	SoundFile "weapons\BlasterFire09Mix01.aif";
	Volume	0.74;
	Pitch	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_BLASTER_ACTIVATE"
{
	SoundFile "weapons\blaster_activate.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    75;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_BLASTER_IMPACT"
{
	SoundFile "weapons\blaster_impact.aif";
    Flags ChannelSaver;
	Volume	0.55;
	Pitch	1.0;
    Priority    90;
    Falloff     0.46;
}

Complex	"SFX_WEAPONS_BLASTER_PROJECTILE_RICOCHET"
{
	SoundFile "weapons\BlasterRicochet01Mix01.aif";
    Flags ChannelSaver;
	Volume	    0.65;
	Pitch	    1.0;
    Priority    70;
    Falloff     0.3;
}


//              Missile Launcher

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRE"
{
	SoundFile "weapons\MissLaunchFire02Mix01.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_ACTIVATE"
{
	SoundFile "weapons\missile_launcher_activate.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    90;
    Falloff     0.05;
}

Complex	"SFX_WEAPONS_MISSILELAUNCHER_RELOAD"
{
    Falloff     0.2;
	SoundFile "weapons\mortar_reload.aif"
    {
    	Volume	0.35;
	    Pitch	0.70;
        Priority    80;
        Delay   0.15;
    }

   	SoundFile "weapons\plasma_rifle_reload.aif"
    {
    	Volume	0.25;
	    Pitch	0.60;
        Priority    80;
        Delay   0.35;
    }
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_PROJECTILE_LOOP"
{
	SoundFile "weapons\missile_projectile.aif";
	Volume	0.65;
	Pitch	1.0;
    Priority    -1;
    Falloff 0.20;
}

/*
Complex	"SFX_WEAPONS_MISSLELAUNCHER_DRYFIRE"
{
	SoundFile "weapons\missile_launcher_dryfire.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    80;
}
*/

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_SEARCH"
{
	SoundFile "weapons\missile_target_inbound.aif";
	Volume	0.52;
	Pitch	0.80;
    Priority    70;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRER_SEARCH"
{
	SoundFile "weapons\missile_firer_search.aif";
	Volume	0.52;
	Pitch	1.0;
    Priority    70;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_FIRER_LOCK"
{
	SoundFile "weapons\missile_firer_search.aif";
	Volume	0.52;
	Pitch	1.15;
    Priority    70;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_LOCK"
{
	SoundFile "weapons\missile_target_lock.aif";
	Volume	0.52;
	Pitch	1.0;
    Priority    70;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MISSLELAUNCHER_TARGET_INBOUND"
{
	SoundFile "weapons\missile_target_inbound.aif";
	Volume	0.52;
	Pitch	1.0;
    Priority    70;
    Falloff     0.2;
}
 

//              Targeting Laser
/*
Complex	"SFX_WEAPONS_TARGETLASER_PAINT_LOOP"
{
	SoundFile "weapons\TargetLaserPaint01Mix01_b_LP.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    -1;
}
*/

Complex	"SFX_WEAPONS_TARGETLASER_ACTIVATE"
{
	SoundFile "weapons\generic_switch.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
    Falloff     0.05;
}


//              ELF Gun
/*
Complex	"SFX_WEAPONS_ELF_FIRE_LOOP"
{
	SoundFile "weapons\ELFFire01Mix03_LP.aif";
	Volume	0.60;
	Pitch	1.0;
    Priority    -1;
}
*/

Complex	"SFX_WEAPONS_ELF_ACTIVATE"
{
	SoundFile "weapons\generic_switch.aif";
	Volume	0.55;
	Pitch	1.0;
    Priority    80;
    Falloff     0.05;
}


//              Repair Tool

Complex	"SFX_WEAPONS_REPAIR_USE"
{
	SoundFile "weapons\RepairUse01Mix02_LP.aif";
	Volume	    0.40;
	Pitch	    1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_REPAIR_ACTIVATE"
{
	Hybrid SoundFile "packs/RepairPackActivate.aif";
	Volume	0.50;
	Pitch	1.0;
    Priority    80;
    Falloff     0.05;
}


//              Mines

Complex	"SFX_WEAPONS_MINE_THROW"
{
	SoundFile "weapons\GrenadeThrow01Mix01.aif";
	Volume	0.80;
	Pitch	0.95;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MINE_DEPLOY"
{
	SoundFile "weapons\mine_deploy.aif";
	Volume	0.65;
	Pitch	1.0;
    Priority    90;
    Falloff     0.2;
}

Complex	"SFX_WEAPONS_MINE_DETONATE"
{
	SoundFile "weapons\mine_detonate.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff     0.88;
}

Complex	"SFX_WEAPONS_MINE_FIZZLE"
{
	Hybrid SoundFile "weapons\MineFizzle01Mix01.aif";
	Volume	0.80;
	Pitch	1.0;
    Priority    90;
    Falloff     0.5;
}

//              Generic Weapon

Complex	"SFX_WEAPONS_OUT_OF_AMMO_ALL"
{
	SoundFile "weapons\OutOfAmmo04.aif";
	Volume	0.65;
	Pitch	1.0;
    Priority    85;
    Falloff     0.015;
}


//              Explosions

Complex	"SFX_EXPLOSIONS_EXPLOSION01"
{
	SoundFile "Explosions\explosion01Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff     0.9;
}

Complex	"SFX_EXPLOSIONS_EXPLOSION02"
{
	SoundFile "Explosions\explosion02Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff     0.9;
}

Complex	"SFX_EXPLOSIONS_EXPLOSION03"
{
	SoundFile "Explosions\explosion03Mix01.aif";
	Volume	0.90;
	Pitch	1.0;
    Priority    90;
    Falloff     0.9;
}


