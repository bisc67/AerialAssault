//              Powered SFX


//                  Base stuff

Complex	"SFX_POWER_BASE_ON"
{
    Streamed SoundFile "power\BasePowerOn01Mix01.aif";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
}

Complex	"SFX_POWER_BASE_OFF"
{
    Streamed SoundFile "power\BasePowerOff01Mix01.aif";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
}

/*
Complex	"SFX_POWER_BASE_HUMM_LOOP"
{
	SoundFile "power\base_power_LP2.aif";
	Volume	0.5;
    Pitch	1.0;
    Priority    -1;
}
*/

Complex	"SFX_POWER_GENERATOR_HUMM_LOOP"
{
	SoundFile "power\GeneratorOnLoop02Mix01.aif";
	Volume	0.65;
    Pitch	1.0;
    Priority    -1;
    Falloff 0.18;
}


//                  Inventory Station

Complex	"SFX_POWER_INVENTORYSTATION_APPEAR"
{
	SoundFile "power\inv_pad_appear.aif";
	Volume	0.5;
    Pitch	1.0;
    Priority    75;
    Falloff 0.18;
}

Complex	"SFX_POWER_INVENTORYSTATION_DENIED"
{
    SoundFile "power\station_denied.aif";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
    Falloff 0.18;
}

/*
Complex	"SFX_POWER_INVENTORYSTATION_ON"
{
	SoundFile "power\inv_pad_on.aif";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
}
*/

Complex	"SFX_POWER_INVENTORYSTATION_HUMM_LOOP"
{
	SoundFile "power\StationOnLoop01Mix02.aif";
	Volume	0.35;
    Pitch	0.9;
    Falloff 0.18;
    Priority    -1;
}
/*
Complex	"SFX_POWER_INVENTORYSTATION_OFF"
{
        Hybrid SoundFile "power\inv_pad_off.aif";
	Volume	0.5;
    Pitch	1.0;
    Priority    75;
}
*/

Complex	"SFX_POWER_INVENTORYSTATION_USE_LOOP"
{
	SoundFile "power\InvenStationUse01Mix02_LP.aif";
	Volume	    1.0;
    Pitch	    1.0;
    Priority    -1;
    Falloff     0.25;

}

Complex	"SFX_POWER_INVENTORYSTATION_USE_OFF"
{
	SoundFile "power\InvenStationUseOff01Mix02.aif";
	Volume	0.9;
    Pitch	1.0;
    Priority    75;
    Falloff 0.2;

}


//                  Vehicle Station

Complex	"SFX_POWER_VEHICLESTATION_USE"
{
	SoundFile "power\VehicleStationUse01Mix01.aif";
	Volume	0.7;
    Pitch	1.0;
    Priority    75;
    Falloff 0.15;

}

Complex	"SFX_POWER_VEHICLESTATION_OFF"
{
	SoundFile "power\VehicleStationOff01Mix01.aif";
	Volume	0.6;
    Pitch	1.0;
    Priority    75;
    Falloff 0.15;

}

Complex	"SFX_POWER_VEHICLE_APPEAR"
{
	Streamed SoundFile "power\VehicleAppear01Mix01.aif";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
    Falloff 0.5;

}


/*
//          Ammo Station

Complex	"SFX_POWER_AMMOSTATION_HUMM_LOOP"
{
	SoundFile "power\StationLoop01Mix02_LP3.aif";
	Volume	0.5;
    Pitch	1.05;
    Falloff 0.2;
    Priority    -1;
}
*/


//          Sensor

Complex	"SFX_POWER_SENSOR_HUMM_LOOP"
{
	SoundFile "power\sensor_hum.aif";
	Volume	0.5;
    Pitch	1.0;
    Falloff 0.2;
    Priority    -1;
}


//          Misc


Complex	"SFX_POWER_DEPLOYABLE_STATION_USE"
{
	SoundFile "power\dep_inv_station.aif";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
    Falloff 0.2;
}











