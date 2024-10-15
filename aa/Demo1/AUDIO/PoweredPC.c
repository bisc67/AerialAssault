//              Powered SFX


//                  Base stuff

Complex	"SFX_POWER_BASE_ON"
{
    Streamed SoundFile "POWER\BasePowerOn01Mix01.wav";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
}

Complex	"SFX_POWER_BASE_OFF"
{
    Streamed SoundFile "POWER\BasePowerOff01Mix01.wav";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
}

/*
Complex	"SFX_POWER_BASE_HUMM_LOOP"
{
	SoundFile "power\base_power_LP2.wav";
	Volume	0.5;
    Pitch	1.0;
    Priority    -1;
}
*/

Complex	"SFX_POWER_GENERATOR_HUMM_LOOP"
{
	SoundFile "POWER\GeneratorOnLoop02Mix01.wav";
	Volume	0.5;
    Pitch	1.0;
    Priority    -1;
    Falloff 0.2;

}


//                  Inventory Station

Complex	"SFX_POWER_INVENTORYSTATION_APPEAR"
{
	SoundFile "POWER\inv_pad_appear.wav";
	Volume	0.5;
    Pitch	1.0;
    Priority    75;
}

Complex	"SFX_POWER_INVENTORYSTATION_DENIED"
{
        SoundFile "POWER\station_denied.wav";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
}

/*
Complex	"SFX_POWER_INVENTORYSTATION_ON"
{
	SoundFile "power\inv_pad_on.wav";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
}
*/

Complex	"SFX_POWER_INVENTORYSTATION_HUMM_LOOP"
{
	SoundFile "POWER\StationOnLoop01Mix02.wav";
	Volume	0.4;
    Pitch	0.9;
    Falloff 0.2;
    Priority    -1;
}

Complex	"SFX_POWER_INVENTORYSTATION_OFF"
{
        Hybrid SoundFile "POWER\inv_pad_off.wav";
	Volume	0.5;
    Pitch	1.0;
    Priority    75;
}

Complex	"SFX_POWER_INVENTORYSTATION_USE_LOOP"
{
	SoundFile "POWER\InvenStationUse01Mix02_LP.wav";
	Volume	0.9;
    Pitch	1.0;
    Priority    75;
    Falloff 0.7;

}

Complex	"SFX_POWER_INVENTORYSTATION_USE_OFF"
{
	SoundFile "POWER\InvenStationUseOff01Mix02.wav";
	Volume	0.9;
    Pitch	1.0;
    Priority    75;
    Falloff 0.7;

}


//                  Vehicle Station

Complex	"SFX_POWER_VEHICLESTATION_USE"
{
	SoundFile "POWER\VehicleStationUse01Mix01.wav";
	Volume	0.6;
    Pitch	1.0;
    Priority    75;
    Falloff 0.7;

}

Complex	"SFX_POWER_VEHICLESTATION_OFF"
{
	SoundFile "POWER\VehicleStationOff01Mix01.wav";
	Volume	0.6;
    Pitch	1.0;
    Priority    75;
    Falloff 0.7;

}

Complex	"SFX_POWER_VEHICLE_APPEAR"
{
	Streamed SoundFile "POWER\VehicleAppear01Mix01.wav";
	Volume	1.0;
    Pitch	1.0;
    Priority    75;
//    Falloff 0.7;

}


/*
//          Ammo Station

Complex	"SFX_POWER_AMMOSTATION_HUMM_LOOP"
{
	SoundFile "power\StationLoop01Mix02_LP3.wav";
	Volume	0.5;
    Pitch	1.05;
    Falloff 0.2;
    Priority    -1;
}
*/


//          Sensor

Complex	"SFX_POWER_SENSOR_HUMM_LOOP"
{
	SoundFile "POWER\sensor_hum.wav";
	Volume	0.5;
    Pitch	1.0;
    Falloff 0.2;
    Priority    -1;
}


//          Misc


Complex	"SFX_POWER_DEPLOYABLE_STATION_USE"
{
	SoundFile "POWER\dep_inv_station.wav";
	Volume	0.75;
    Pitch	1.0;
    Priority    75;
}











