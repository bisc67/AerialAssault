//              Miscellaneous SFX


Complex	"SFX_MISC_PICKUP04"
{
	SoundFile "misc\pickup04.aif";
	Volume	0.65;
    Pitch	1.0;
    Priority    110;
    Falloff 0.15;
}

Complex	"SFX_MISC_RESPAWN"
{
	SoundFile "misc\Spawn02Mix01.aif";
	Volume	0.50;
    Pitch	1.0;
    Priority    100;
    Falloff 0.05;
}


Complex "SFX_MISC_HEALTH_PATCH"
{
    Soundfile "misc\health_patch.aif";
    Volume  0.70;
    Pitch   1.0;
    Priority    70;
    Falloff 0.1;
}


Complex "SFX_MISC_NEXUS_IDLE"
{
    Soundfile "misc\nexus_idle.aif";
    Volume  0.40;
    Pitch   1.0;
    Priority    -1;
    Falloff 0.3;
}

Complex "SFX_MISC_NEXUS_CAP"
{
    Streamed Soundfile "misc\nexus_cap.aif";
    Volume  0.80;
    Pitch   1.0;
    Priority    80;
}

Complex "SFX_MISC_SWITCH_GOOD"
{
    Soundfile "misc\switch_taken.aif";
    Volume  0.75;
    Pitch   1.0;
    Priority    80;
}

Complex "SFX_MISC_SWITCH_BAD"
{
    Soundfile "misc\flag_lost.aif";
    Volume  0.83;
    Pitch   1.0;
    Priority    80;
}

Complex "SFX_MISC_OUT_OF_BOUNDS_WARNING"
{
    Soundfile "misc\OBWarning01.aif";
    Volume  0.60;
    Pitch   1.0;
    Priority    50;
}


//              Flag Stuff

Complex "SFX_MISC_FLAG_CAPTURE_GOOD"
{
    Hybrid SoundFile "misc\GoodFlagCap02Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_DROP_GOOD"
{
    SoundFile "misc\BadFlagDrop02Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_LOST_GOOD"
{
    SoundFile "misc\BadFlagDrop02Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_RETURN_GOOD"
{
    SoundFile "misc\GoodFlagReturn03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_SNATCH_GOOD"
{
    Hybrid SoundFile "misc\GoodFlagGrab02Mix03.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_TAKEN_GOOD"
{
    Hybrid SoundFile "misc\GoodFlagGrab02Mix03.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_CAPTURE_BAD"
{
    Hybrid SoundFile "misc\BadFlagCap03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_DROP_BAD"
{
    SoundFile "misc\GoodFlagDrop03Mix01.aif"; 
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_LOST_BAD"
{
    SoundFile "misc\GoodFlagDrop03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_RETURN_BAD"
{
    SoundFile "misc\BadFlagReturn03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_SNATCH_BAD"
{
    Hybrid SoundFile "misc\BadFlagGrab03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_TAKEN_BAD"
{
    Hybrid SoundFile "misc\BadFlagGrab03Mix01.aif";
    Volume  0.93;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_FLAG_MINED_FEMALE"
{
    Streamed Soundfile "misc\flag_mined_female.aif";
    Volume  0.85;
    Pitch   1.0;
    Priority    100;
}
/*
Complex "SFX_MISC_FLAG_MINED_MALE"
{
    Streamed Soundfile "misc\flag_mined_male.aif";
    Volume  0.85;
    Pitch   1.0;
    Priority    100;
}
*/

Complex "SFX_MISC_HUNTERS_FLAG_GRAB"
{
    Soundfile "misc\GoodGrabFX_01.aif";
    Volume  0.90;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_HUNTERS_FLAG_DROP"
{
    Soundfile "misc\GoodDropFX_01.aif";
    Volume  0.90;
    Pitch   1.0;
    Priority    100;
}

Complex "SFX_MISC_ADMIN_CHANGE_STATE"
{
    Streamed Soundfile "misc\hunters_greed.aif";
    Volume  0.90;
    Pitch   1.0;
    Priority    100;
}




