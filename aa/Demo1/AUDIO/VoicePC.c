//          Audio Script - Audio - Temporary

Default Pitch   1.0;
Default Priority    100;


//                  Announcer

Complex "SFX_VOICE_ANN_10SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_10sec.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_15SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_15sec.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_30SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_30.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_60SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_60.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_LOOKS_LIKE_CLOSE_ONE"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_01.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_FIGHT_TO_FINISH"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_03.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_REAL_BATTLE"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_04.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GAMEOVER"
{
    Streamed Soundfile "Voice\Announcer\ANN_gameover.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GAMEPOINT_IMMINENT"
{
    Streamed Soundfile "Voice\Announcer\ANN_gamepoint_imminent.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GREED_MODE"
{
    Streamed Soundfile "Voice\Announcer\ANN_greed.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_HOARD_MODE"
{
    Streamed Soundfile "Voice\Announcer\ANN_hoard.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_HOLD_ON_TO_FLAGS"
{
    Streamed Soundfile "Voice\Announcer\ANN_holdflags.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_DENIED"
{
    Streamed Soundfile "Voice\Announcer\ANN_infdenied.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_SCORES"
{
    Streamed Soundfile "Voice\Announcer\ANN_infscores.mp3"
    {
        Delay   1.2;
        Volume  0.90;
    }
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_WINS"
{
    Streamed Soundfile "Voice\Announcer\ANN_infwins.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_MATCH_BEGINS_NOW"
{
    Streamed Soundfile "Voice\Announcer\ANN_match_begins.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_PREPARE_YOURSELF"
{
    Streamed Soundfile "Voice\Announcer\ANN_prepare.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_STORM_DENIED"
{
    Streamed Soundfile "Voice\Announcer\ANN_stodenied.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_STORM_SCORES"
{
    Streamed Soundfile "Voice\Announcer\ANN_stoscores.mp3"
    {
        Delay   1.2;
        Volume  0.90;
    }
}

Complex "SFX_VOICE_ANN_TEAM_STORM_WINS"
{
    Streamed Soundfile "Voice\Announcer\ANN_stowins.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_FAIL"
{
    Streamed Soundfile "Voice\Announcer\ANN_votefail.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_INITIATED"
{
    Streamed Soundfile "Voice\Announcer\ANN_voteinit.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_PASSES"
{
    Streamed Soundfile "Voice\Announcer\ANN_votepass.mp3";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_YARDSALE"
{
    Streamed Soundfile "Voice\Announcer\ANN_yardsale.mp3";
    Volume  0.90;
}


//                  Bot 1 - 

Complex "SFX_VOICE_BOT_1"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Bot1\B1_att_attack.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_chase.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_distract.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_objective.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_reinforcements.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_att_wait.mp3";
    streamed Soundfile "Voice\Bot1\B1_bas_clear.mp3";
    streamed Soundfile "Voice\Bot1\B1_bas_enemy.mp3";
    streamed Soundfile "Voice\Bot1\B1_bas_retake.mp3";
    streamed Soundfile "Voice\Bot1\B1_bas_secure.mp3";
    streamed Soundfile "Voice\Bot1\B1_bas_taken.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_carrier.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_entrances.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_me.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_nexus.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_objective.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_reinforce.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_def_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_disarray.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_remotes.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_ene_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_give.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_huntergive.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_huntertake.mp3";
    streamed Soundfile "Voice\Null.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_secure.mp3";
    streamed Soundfile "Voice\Bot1\B1_flg_take.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_anytime.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_awesome.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_bye.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_dunno.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_hi.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_no.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_oops.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_quiet.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_rock.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_shooting.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_thanks.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_wait.mp3";
    streamed Soundfile "Voice\Null.mp3";
    streamed Soundfile "Voice\Bot1\B1_gbl_yes.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_bombardier.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_cover.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_driver.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_escort.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_gunship.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_hold.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_pilot.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_ride.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_support.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_tailgunner.mp3";
    streamed Soundfile "Voice\Bot1\B1_need_where.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_me.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_rep_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_attack.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_defend.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_acquired.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_base.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_flag.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_need.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_sensors.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_target.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_turret.mp3";
    streamed Soundfile "Voice\Bot1\B1_tgt_wait.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_anytime.mp3";
    streamed Soundfile "Voice\Null.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_dunno.mp3";
    streamed Soundfile "Voice\Null.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_move.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_no.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_sorry.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_thanks.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_wait.mp3";
    streamed Soundfile "Voice\Bot1\B1_vqk_yes.mp3";
    streamed Soundfile "Voice\Bot1\B1_wrn_bomber.mp3";
    streamed Soundfile "Voice\Bot1\B1_wrn_enemy.mp3";
    streamed Soundfile "Voice\Bot1\B1_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Bot1\B1_wrn_watchit.mp3";
    Soundfile "Voice\Bot1\B1_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Bot1\Null.mp3";
    streamed Soundfile "Voice\Bot1\B1_avo_grunt.mp3";
    Soundfile "Voice\Bot1\B1_avo_pain.mp3";
}



//                  BioDerm 1 - 

Complex "SFX_VOICE_DERM_1"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Derm1\D1_att_attack.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_chase.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_distract.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_objective.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_reinforcements.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_att_wait.mp3";
    streamed Soundfile "Voice\Derm1\D1_bas_clear.mp3";
    streamed Soundfile "Voice\Derm1\D1_bas_enemy.mp3";
    streamed Soundfile "Voice\Derm1\D1_bas_retake.mp3";
    streamed Soundfile "Voice\Derm1\D1_bas_secure.mp3";
    streamed Soundfile "Voice\Derm1\D1_bas_taken.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_carrier.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_entrances.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_me.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_nexus.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_objective.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_reinforce.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_disarray.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_remotes.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_ene_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_give.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_huntergive.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_huntertake.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_retrieve.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_secure.mp3";
    streamed Soundfile "Voice\Derm1\D1_flg_take.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_anytime.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_awesome.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_bye.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_dunno.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_hi.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_no.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_oops.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_quiet.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_rock.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_shooting.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_thanks.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_wait.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Derm1\D1_gbl_yes.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_bombardier.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_cover.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_driver.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_escort.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_gunship.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_hold.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_pilot.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_ride.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_support.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_tailgunner.mp3";
    streamed Soundfile "Voice\Derm1\D1_need_where.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_me.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_attack.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_defend.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_acquired.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_flag.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_need.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_sensors.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_target.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_turret.mp3";
    streamed Soundfile "Voice\Derm1\D1_tgt_wait.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_anytime.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_base.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_dunno.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_help.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_move.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_no.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_sorry.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_thanks.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_wait.mp3";
    streamed Soundfile "Voice\Derm1\D1_vqk_yes.mp3";
    streamed Soundfile "Voice\Derm1\D1_wrn_bomber.mp3";
    streamed Soundfile "Voice\Derm1\D1_wrn_enemy.mp3";
    streamed Soundfile "Voice\Derm1\D1_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Derm1\D1_wrn_watchit.mp3";
    Soundfile "Voice\Derm1\D1_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Derm1\Null.mp3";
    streamed Soundfile "Voice\Derm1\D1_avo_grunt.mp3";
    Soundfile "Voice\Derm1\D1_avo_pain.mp3";
}


//                  BioDerm 2 - 

Complex "SFX_VOICE_DERM_2"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Derm2\D2_att_attack.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_chase.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_distract.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_objective.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_reinforcements.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_att_wait.mp3";
    streamed Soundfile "Voice\Derm2\D2_bas_clear.mp3";
    streamed Soundfile "Voice\Derm2\D2_bas_enemy.mp3";
    streamed Soundfile "Voice\Derm2\D2_bas_retake.mp3";
    streamed Soundfile "Voice\Derm2\D2_bas_secure.mp3";
    streamed Soundfile "Voice\Derm2\D2_bas_taken.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_carrier.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_entrances.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_me.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_nexus.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_objective.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_reinforce.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_disarray.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_remotes.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_ene_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_give.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_huntergive.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_huntertake.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_retrieve.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_secure.mp3";
    streamed Soundfile "Voice\Derm2\D2_flg_take.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_anytime.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_awesome.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_bye.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_dunno.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_hi.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_no.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_oops.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_quiet.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_rock.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_shooting.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_thanks.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_wait.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Derm2\D2_gbl_yes.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_bombardier.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_cover.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_driver.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_escort.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_gunship.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_hold.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_pilot.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_ride.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_support.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_tailgunner.mp3";
    streamed Soundfile "Voice\Derm2\D2_need_where.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_me.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_attack.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_defend.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_acquired.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_flag.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_need.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_sensors.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_target.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_turret.mp3";
    streamed Soundfile "Voice\Derm2\D2_tgt_wait.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_anytime.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_base.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_dunno.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_help.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_move.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_no.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_sorry.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_thanks.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_wait.mp3";
    streamed Soundfile "Voice\Derm2\D2_vqk_yes.mp3";
    streamed Soundfile "Voice\Derm2\D2_wrn_bomber.mp3";
    streamed Soundfile "Voice\Derm2\D2_wrn_enemy.mp3";
    streamed Soundfile "Voice\Derm2\D2_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Derm2\D2_wrn_watchit.mp3";
    Soundfile "Voice\Derm2\D2_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Derm2\Null.mp3";
    streamed Soundfile "Voice\Derm2\D2_avo_grunt.mp3";
    Soundfile "Voice\Derm2\D2_avo_pain.mp3";
}


//                  BioDerm 3 - 

Complex "SFX_VOICE_DERM_3"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Derm3\D3_att_attack.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_chase.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_distract.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_objective.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_reinforcements.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_att_wait.mp3";
    streamed Soundfile "Voice\Derm3\D3_bas_clear.mp3";
    streamed Soundfile "Voice\Derm3\D3_bas_enemy.mp3";
    streamed Soundfile "Voice\Derm3\D3_bas_retake.mp3";
    streamed Soundfile "Voice\Derm3\D3_bas_secure.mp3";
    streamed Soundfile "Voice\Derm3\D3_bas_taken.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_carrier.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_entrances.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_me.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_nexus.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_objective.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_reinforce.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_disarray.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_remotes.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_ene_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_give.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_huntergive.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_huntertake.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_retrieve.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_secure.mp3";
    streamed Soundfile "Voice\Derm3\D3_flg_take.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_anytime.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_awesome.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_bye.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_dunno.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_hi.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_no.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_oops.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_quiet.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_rock.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_shooting.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_thanks.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_wait.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Derm3\D3_gbl_yes.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_bombardier.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_cover.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_driver.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_escort.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_gunship.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_hold.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_pilot.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_ride.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_support.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_tailgunner.mp3";
    streamed Soundfile "Voice\Derm3\D3_need_where.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_me.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_attack.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_defend.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_acquired.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_flag.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_need.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_sensors.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_target.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_turret.mp3";
    streamed Soundfile "Voice\Derm3\D3_tgt_wait.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_anytime.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_base.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_dunno.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_help.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_move.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_no.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_sorry.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_thanks.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_wait.mp3";
    streamed Soundfile "Voice\Derm3\D3_vqk_yes.mp3";
    streamed Soundfile "Voice\Derm3\D3_wrn_bomber.mp3";
    streamed Soundfile "Voice\Derm3\D3_wrn_enemy.mp3";
    streamed Soundfile "Voice\Derm3\D3_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Derm3\D3_wrn_watchit.mp3";
    Soundfile "Voice\Derm3\D3_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Derm3\Null.mp3";
    streamed Soundfile "Voice\Derm3\D3_avo_grunt.mp3";
    Soundfile "Voice\Derm3\D3_avo_pain.mp3";
}



//                  Male 1 - 

Complex "SFX_VOICE_MALE_1"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Male1\M1_att_attack.mp3";
    streamed Soundfile "Voice\Male1\M1_att_base.mp3";
    streamed Soundfile "Voice\Male1\M1_att_chase.mp3";
    streamed Soundfile "Voice\Male1\M1_att_distract.mp3";
    streamed Soundfile "Voice\Male1\M1_att_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_att_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_att_objective.mp3";
    streamed Soundfile "Voice\Male1\M1_att_reinforcements.mp3";
    streamed Soundfile "Voice\Male1\M1_att_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_att_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_att_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_att_wait.mp3";
    streamed Soundfile "Voice\Male1\M1_bas_clear.mp3";
    streamed Soundfile "Voice\Male1\M1_bas_enemy.mp3";
    streamed Soundfile "Voice\Male1\M1_bas_retake.mp3";
    streamed Soundfile "Voice\Male1\M1_bas_secure.mp3";
    streamed Soundfile "Voice\Male1\M1_bas_taken.mp3";
    streamed Soundfile "Voice\Male1\M1_def_base.mp3";
    streamed Soundfile "Voice\Male1\M1_def_carrier.mp3";
    streamed Soundfile "Voice\Male1\M1_def_entrances.mp3";
    streamed Soundfile "Voice\Male1\M1_def_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_def_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_def_me.mp3";
    streamed Soundfile "Voice\Male1\M1_def_nexus.mp3";
    streamed Soundfile "Voice\Male1\M1_def_objective.mp3";
    streamed Soundfile "Voice\Male1\M1_def_reinforce.mp3";
    streamed Soundfile "Voice\Male1\M1_def_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_def_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_def_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_base.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_disarray.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_remotes.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_ene_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_give.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_huntergive.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_huntertake.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_retrieve.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_secure.mp3";
    streamed Soundfile "Voice\Male1\M1_flg_take.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_anytime.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_awesome.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_bye.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_dunno.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_hi.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_no.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_oops.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_quiet.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_rock.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_shooting.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_thanks.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_wait.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Male1\M1_gbl_yes.mp3";
    streamed Soundfile "Voice\Male1\M1_need_bombardier.mp3";
    streamed Soundfile "Voice\Male1\M1_need_cover.mp3";
    streamed Soundfile "Voice\Male1\M1_need_driver.mp3";
    streamed Soundfile "Voice\Male1\M1_need_escort.mp3";
    streamed Soundfile "Voice\Male1\M1_need_gunship.mp3";
    streamed Soundfile "Voice\Male1\M1_need_hold.mp3";
    streamed Soundfile "Voice\Male1\M1_need_pilot.mp3";
    streamed Soundfile "Voice\Male1\M1_need_ride.mp3";
    streamed Soundfile "Voice\Male1\M1_need_support.mp3";
    streamed Soundfile "Voice\Male1\M1_need_tailgunner.mp3";
    streamed Soundfile "Voice\Male1\M1_need_where.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_base.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_me.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_attack.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_base.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_base.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_defend.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_base.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_acquired.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_base.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_flag.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_need.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_sensors.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_target.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_turret.mp3";
    streamed Soundfile "Voice\Male1\M1_tgt_wait.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_anytime.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_base.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_dunno.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_help.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_move.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_no.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_sorry.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_thanks.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_wait.mp3";
    streamed Soundfile "Voice\Male1\M1_vqk_yes.mp3";
    streamed Soundfile "Voice\Male1\M1_wrn_bomber.mp3";
    streamed Soundfile "Voice\Male1\M1_wrn_enemy.mp3";
    streamed Soundfile "Voice\Male1\M1_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Male1\M1_wrn_watchit.mp3";
    Soundfile "Voice\Male1\M1_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Male1\Null.mp3";
    streamed Soundfile "Voice\Male1\M1_avo_grunt.mp3";
    Soundfile "Voice\Male1\M1_avo_pain.mp3";
}


//                  Male 2 - 

Complex "SFX_VOICE_MALE_2"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Male2\M2_att_attack.mp3";
    streamed Soundfile "Voice\Male2\M2_att_base.mp3";
    streamed Soundfile "Voice\Male2\M2_att_chase.mp3";
    streamed Soundfile "Voice\Male2\M2_att_distract.mp3";
    streamed Soundfile "Voice\Male2\M2_att_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_att_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_att_objective.mp3";
    streamed Soundfile "Voice\Male2\M2_att_reinforcements.mp3";
    streamed Soundfile "Voice\Male2\M2_att_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_att_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_att_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_att_wait.mp3";
    streamed Soundfile "Voice\Male2\M2_bas_clear.mp3";
    streamed Soundfile "Voice\Male2\M2_bas_enemy.mp3";
    streamed Soundfile "Voice\Male2\M2_bas_retake.mp3";
    streamed Soundfile "Voice\Male2\M2_bas_secure.mp3";
    streamed Soundfile "Voice\Male2\M2_bas_taken.mp3";
    streamed Soundfile "Voice\Male2\M2_def_base.mp3";
    streamed Soundfile "Voice\Male2\M2_def_carrier.mp3";
    streamed Soundfile "Voice\Male2\M2_def_entrances.mp3";
    streamed Soundfile "Voice\Male2\M2_def_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_def_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_def_me.mp3";
    streamed Soundfile "Voice\Male2\M2_def_nexus.mp3";
    streamed Soundfile "Voice\Male2\M2_def_objective.mp3";
    streamed Soundfile "Voice\Male2\M2_def_reinforce.mp3";
    streamed Soundfile "Voice\Male2\M2_def_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_def_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_def_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_base.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_disarray.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_remotes.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_ene_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_give.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_huntergive.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_huntertake.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_retrieve.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_secure.mp3";
    streamed Soundfile "Voice\Male2\M2_flg_take.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_anytime.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_awesome.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_bye.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_dunno.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_hi.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_no.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_oops.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_quiet.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_rock.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_shooting.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_thanks.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_wait.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Male2\M2_gbl_yes.mp3";
    streamed Soundfile "Voice\Male2\M2_need_bombardier.mp3";
    streamed Soundfile "Voice\Male2\M2_need_cover.mp3";
    streamed Soundfile "Voice\Male2\M2_need_driver.mp3";
    streamed Soundfile "Voice\Male2\M2_need_escort.mp3";
    streamed Soundfile "Voice\Male2\M2_need_gunship.mp3";
    streamed Soundfile "Voice\Male2\M2_need_hold.mp3";
    streamed Soundfile "Voice\Male2\M2_need_pilot.mp3";
    streamed Soundfile "Voice\Male2\M2_need_ride.mp3";
    streamed Soundfile "Voice\Male2\M2_need_support.mp3";
    streamed Soundfile "Voice\Male2\M2_need_tailgunner.mp3";
    streamed Soundfile "Voice\Male2\M2_need_where.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_base.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_me.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_attack.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_base.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_base.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_defend.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_base.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_acquired.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_base.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_flag.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_need.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_sensors.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_target.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_turret.mp3";
    streamed Soundfile "Voice\Male2\M2_tgt_wait.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_anytime.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_base.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_dunno.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_help.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_move.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_no.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_sorry.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_thanks.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_wait.mp3";
    streamed Soundfile "Voice\Male2\M2_vqk_yes.mp3";
    streamed Soundfile "Voice\Male2\M2_wrn_bomber.mp3";
    streamed Soundfile "Voice\Male2\M2_wrn_enemy.mp3";
    streamed Soundfile "Voice\Male2\M2_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Male2\M2_wrn_watchit.mp3";
    Soundfile "Voice\Male2\M2_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Male2\Null.mp3";
    streamed Soundfile "Voice\Male2\M2_avo_grunt.mp3";
    Soundfile "Voice\Male2\M2_avo_pain.mp3";
}


//                  Male 3 - 

Complex "SFX_VOICE_MALE_3"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Male3\M3_att_attack.mp3";
    streamed Soundfile "Voice\Male3\M3_att_base.mp3";
    streamed Soundfile "Voice\Male3\M3_att_chase.mp3";
    streamed Soundfile "Voice\Male3\M3_att_distract.mp3";
    streamed Soundfile "Voice\Male3\M3_att_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_att_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_att_objective.mp3";
    streamed Soundfile "Voice\Male3\M3_att_reinforcements.mp3";
    streamed Soundfile "Voice\Male3\M3_att_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_att_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_att_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_att_wait.mp3";
    streamed Soundfile "Voice\Male3\M3_bas_clear.mp3";
    streamed Soundfile "Voice\Male3\M3_bas_enemy.mp3";
    streamed Soundfile "Voice\Male3\M3_bas_retake.mp3";
    streamed Soundfile "Voice\Male3\M3_bas_secure.mp3";
    streamed Soundfile "Voice\Male3\M3_bas_taken.mp3";
    streamed Soundfile "Voice\Male3\M3_def_base.mp3";
    streamed Soundfile "Voice\Male3\M3_def_carrier.mp3";
    streamed Soundfile "Voice\Male3\M3_def_entrances.mp3";
    streamed Soundfile "Voice\Male3\M3_def_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_def_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_def_me.mp3";
    streamed Soundfile "Voice\Male3\M3_def_nexus.mp3";
    streamed Soundfile "Voice\Male3\M3_def_objective.mp3";
    streamed Soundfile "Voice\Male3\M3_def_reinforce.mp3";
    streamed Soundfile "Voice\Male3\M3_def_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_def_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_def_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_base.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_disarray.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_remotes.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_ene_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_give.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_huntergive.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_huntertake.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_retrieve.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_secure.mp3";
    streamed Soundfile "Voice\Male3\M3_flg_take.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_anytime.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_awesome.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_bye.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_dunno.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_hi.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_no.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_oops.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_quiet.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_rock.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_shooting.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_thanks.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_wait.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Male3\M3_gbl_yes.mp3";
    streamed Soundfile "Voice\Male3\M3_need_bombardier.mp3";
    streamed Soundfile "Voice\Male3\M3_need_cover.mp3";
    streamed Soundfile "Voice\Male3\M3_need_driver.mp3";
    streamed Soundfile "Voice\Male3\M3_need_escort.mp3";
    streamed Soundfile "Voice\Male3\M3_need_gunship.mp3";
    streamed Soundfile "Voice\Male3\M3_need_hold.mp3";
    streamed Soundfile "Voice\Male3\M3_need_pilot.mp3";
    streamed Soundfile "Voice\Male3\M3_need_ride.mp3";
    streamed Soundfile "Voice\Male3\M3_need_support.mp3";
    streamed Soundfile "Voice\Male3\M3_need_tailgunner.mp3";
    streamed Soundfile "Voice\Male3\M3_need_where.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_base.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_me.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_attack.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_base.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_base.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_defend.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_base.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_acquired.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_base.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_flag.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_need.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_sensors.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_target.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_turret.mp3";
    streamed Soundfile "Voice\Male3\M3_tgt_wait.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_anytime.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_base.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_dunno.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_help.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_move.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_no.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_sorry.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_thanks.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_wait.mp3";
    streamed Soundfile "Voice\Male3\M3_vqk_yes.mp3";
    streamed Soundfile "Voice\Male3\M3_wrn_bomber.mp3";
    streamed Soundfile "Voice\Male3\M3_wrn_enemy.mp3";
    streamed Soundfile "Voice\Male3\M3_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Male3\M3_wrn_watchit.mp3";
    Soundfile "Voice\Male3\M3_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Male3\Null.mp3";
    streamed Soundfile "Voice\Male3\M3_avo_grunt.mp3";
    Soundfile "Voice\Male3\M3_avo_pain.mp3";
}


//                  Male 4 - 

Complex "SFX_VOICE_MALE_4"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Male4\M4_att_attack.mp3";
    streamed Soundfile "Voice\Male4\M4_att_base.mp3";
    streamed Soundfile "Voice\Male4\M4_att_chase.mp3";
    streamed Soundfile "Voice\Male4\M4_att_distract.mp3";
    streamed Soundfile "Voice\Male4\M4_att_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_att_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_att_objective.mp3";
    streamed Soundfile "Voice\Male4\M4_att_reinforcements.mp3";
    streamed Soundfile "Voice\Male4\M4_att_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_att_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_att_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_att_wait.mp3";
    streamed Soundfile "Voice\Male4\M4_bas_clear.mp3";
    streamed Soundfile "Voice\Male4\M4_bas_enemy.mp3";
    streamed Soundfile "Voice\Male4\M4_bas_retake.mp3";
    streamed Soundfile "Voice\Male4\M4_bas_secure.mp3";
    streamed Soundfile "Voice\Male4\M4_bas_taken.mp3";
    streamed Soundfile "Voice\Male4\M4_def_base.mp3";
    streamed Soundfile "Voice\Male4\M4_def_carrier.mp3";
    streamed Soundfile "Voice\Male4\M4_def_entrances.mp3";
    streamed Soundfile "Voice\Male4\M4_def_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_def_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_def_me.mp3";
    streamed Soundfile "Voice\Male4\M4_def_nexus.mp3";
    streamed Soundfile "Voice\Male4\M4_def_objective.mp3";
    streamed Soundfile "Voice\Male4\M4_def_reinforce.mp3";
    streamed Soundfile "Voice\Male4\M4_def_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_def_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_def_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_base.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_disarray.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_remotes.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_ene_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_give.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_huntergive.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_huntertake.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_retrieve.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_secure.mp3";
    streamed Soundfile "Voice\Male4\M4_flg_take.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_anytime.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_awesome.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_bye.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_dunno.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_hi.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_no.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_oops.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_quiet.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_rock.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_shooting.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_thanks.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_wait.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Male4\M4_gbl_yes.mp3";
    streamed Soundfile "Voice\Male4\M4_need_bombardier.mp3";
    streamed Soundfile "Voice\Male4\M4_need_cover.mp3";
    streamed Soundfile "Voice\Male4\M4_need_driver.mp3";
    streamed Soundfile "Voice\Male4\M4_need_escort.mp3";
    streamed Soundfile "Voice\Male4\M4_need_gunship.mp3";
    streamed Soundfile "Voice\Male4\M4_need_hold.mp3";
    streamed Soundfile "Voice\Male4\M4_need_pilot.mp3";
    streamed Soundfile "Voice\Male4\M4_need_ride.mp3";
    streamed Soundfile "Voice\Male4\M4_need_support.mp3";
    streamed Soundfile "Voice\Male4\M4_need_tailgunner.mp3";
    streamed Soundfile "Voice\Male4\M4_need_where.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_base.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_me.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_attack.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_base.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_base.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_defend.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_base.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_acquired.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_base.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_flag.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_need.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_sensors.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_target.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_turret.mp3";
    streamed Soundfile "Voice\Male4\M4_tgt_wait.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_anytime.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_base.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_dunno.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_help.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_move.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_no.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_sorry.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_thanks.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_wait.mp3";
    streamed Soundfile "Voice\Male4\M4_vqk_yes.mp3";
    streamed Soundfile "Voice\Male4\M4_wrn_bomber.mp3";
    streamed Soundfile "Voice\Male4\M4_wrn_enemy.mp3";
    streamed Soundfile "Voice\Male4\M4_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Male4\M4_wrn_watchit.mp3";
    Soundfile "Voice\Male4\M4_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Male4\Null.mp3";
    streamed Soundfile "Voice\Male4\M4_avo_grunt.mp3";
    Soundfile "Voice\Male4\M4_avo_pain.mp3";
}


//                  Male 5 - Psycho

Complex "SFX_VOICE_MALE_5"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Male5\M5_att_attack.mp3";
    streamed Soundfile "Voice\Male5\M5_att_base.mp3";
    streamed Soundfile "Voice\Male5\M5_att_chase.mp3";
    streamed Soundfile "Voice\Male5\M5_att_distract.mp3";
    streamed Soundfile "Voice\Male5\M5_att_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_att_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_att_objective.mp3";
    streamed Soundfile "Voice\Male5\M5_att_reinforcements.mp3";
    streamed Soundfile "Voice\Male5\M5_att_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_att_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_att_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_att_wait.mp3";
    streamed Soundfile "Voice\Male5\M5_bas_clear.mp3";
    streamed Soundfile "Voice\Male5\M5_bas_enemy.mp3";
    streamed Soundfile "Voice\Male5\M5_bas_retake.mp3";
    streamed Soundfile "Voice\Male5\M5_bas_secure.mp3";
    streamed Soundfile "Voice\Male5\M5_bas_taken.mp3";
    streamed Soundfile "Voice\Male5\M5_def_base.mp3";
    streamed Soundfile "Voice\Male5\M5_def_carrier.mp3";
    streamed Soundfile "Voice\Male5\M5_def_entrances.mp3";
    streamed Soundfile "Voice\Male5\M5_def_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_def_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_def_me.mp3";
    streamed Soundfile "Voice\Male5\M5_def_nexus.mp3";
    streamed Soundfile "Voice\Male5\M5_def_objective.mp3";
    streamed Soundfile "Voice\Male5\M5_def_reinforce.mp3";
    streamed Soundfile "Voice\Male5\M5_def_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_def_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_def_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_base.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_disarray.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_remotes.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_ene_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_give.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_huntergive.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_huntertake.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_retrieve.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_secure.mp3";
    streamed Soundfile "Voice\Male5\M5_flg_take.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_anytime.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_awesome.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_bye.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_dunno.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_hi.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_no.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_oops.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_quiet.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_rock.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_shooting.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_thanks.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_wait.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Male5\M5_gbl_yes.mp3";
    streamed Soundfile "Voice\Male5\M5_need_bombardier.mp3";
    streamed Soundfile "Voice\Male5\M5_need_cover.mp3";
    streamed Soundfile "Voice\Male5\M5_need_driver.mp3";
    streamed Soundfile "Voice\Male5\M5_need_escort.mp3";
    streamed Soundfile "Voice\Male5\M5_need_gunship.mp3";
    streamed Soundfile "Voice\Male5\M5_need_hold.mp3";
    streamed Soundfile "Voice\Male5\M5_need_pilot.mp3";
    streamed Soundfile "Voice\Male5\M5_need_ride.mp3";
    streamed Soundfile "Voice\Male5\M5_need_support.mp3";
    streamed Soundfile "Voice\Male5\M5_need_tailgunner.mp3";
    streamed Soundfile "Voice\Male5\M5_need_where.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_base.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_me.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_attack.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_base.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_base.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_defend.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_base.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_acquired.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_base.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_flag.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_need.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_sensors.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_target.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_turret.mp3";
    streamed Soundfile "Voice\Male5\M5_tgt_wait.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_anytime.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_base.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_dunno.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_help.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_move.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_no.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_sorry.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_thanks.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_wait.mp3";
    streamed Soundfile "Voice\Male5\M5_vqk_yes.mp3";
    streamed Soundfile "Voice\Male5\M5_wrn_bomber.mp3";
    streamed Soundfile "Voice\Male5\M5_wrn_enemy.mp3";
    streamed Soundfile "Voice\Male5\M5_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Male5\M5_wrn_watchit.mp3";
    Soundfile "Voice\Male5\M5_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Male5\Null.mp3";
    streamed Soundfile "Voice\Male5\M5_avo_grunt.mp3";
    Soundfile "Voice\Male5\M5_avo_pain.mp3";
}



//                  Female 1 - 

Complex "SFX_VOICE_FEMALE_1"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Female1\F1_att_attack.mp3";
    streamed Soundfile "Voice\Female1\F1_att_base.mp3";
    streamed Soundfile "Voice\Female1\F1_att_chase.mp3";
    streamed Soundfile "Voice\Female1\F1_att_distract.mp3";
    streamed Soundfile "Voice\Female1\F1_att_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_att_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_att_objective.mp3";
    streamed Soundfile "Voice\Female1\F1_att_reinforcements.mp3";
    streamed Soundfile "Voice\Female1\F1_att_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_att_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_att_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_att_wait.mp3";
    streamed Soundfile "Voice\Female1\F1_bas_clear.mp3";
    streamed Soundfile "Voice\Female1\F1_bas_enemy.mp3";
    streamed Soundfile "Voice\Female1\F1_bas_retake.mp3";
    streamed Soundfile "Voice\Female1\F1_bas_secure.mp3";
    streamed Soundfile "Voice\Female1\F1_bas_taken.mp3";
    streamed Soundfile "Voice\Female1\F1_def_base.mp3";
    streamed Soundfile "Voice\Female1\F1_def_carrier.mp3";
    streamed Soundfile "Voice\Female1\F1_def_entrances.mp3";
    streamed Soundfile "Voice\Female1\F1_def_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_def_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_def_me.mp3";
    streamed Soundfile "Voice\Female1\F1_def_nexus.mp3";
    streamed Soundfile "Voice\Female1\F1_def_objective.mp3";
    streamed Soundfile "Voice\Female1\F1_def_reinforce.mp3";
    streamed Soundfile "Voice\Female1\F1_def_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_def_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_def_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_base.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_disarray.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_remotes.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_ene_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_give.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_huntergive.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_huntertake.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_retrieve.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_secure.mp3";
    streamed Soundfile "Voice\Female1\F1_flg_take.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_anytime.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_awesome.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_bye.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_dunno.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_hi.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_no.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_oops.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_quiet.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_rock.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_shooting.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_thanks.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_wait.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Female1\F1_gbl_yes.mp3";
    streamed Soundfile "Voice\Female1\F1_need_bombardier.mp3";
    streamed Soundfile "Voice\Female1\F1_need_cover.mp3";
    streamed Soundfile "Voice\Female1\F1_need_driver.mp3";
    streamed Soundfile "Voice\Female1\F1_need_escort.mp3";
    streamed Soundfile "Voice\Female1\F1_need_gunship.mp3";
    streamed Soundfile "Voice\Female1\F1_need_hold.mp3";
    streamed Soundfile "Voice\Female1\F1_need_pilot.mp3";
    streamed Soundfile "Voice\Female1\F1_need_ride.mp3";
    streamed Soundfile "Voice\Female1\F1_need_support.mp3";
    streamed Soundfile "Voice\Female1\F1_need_tailgunner.mp3";
    streamed Soundfile "Voice\Female1\F1_need_where.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_base.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_me.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_attack.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_base.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_base.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_defend.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_base.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_acquired.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_base.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_flag.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_need.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_sensors.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_target.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_turret.mp3";
    streamed Soundfile "Voice\Female1\F1_tgt_wait.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_anytime.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_base.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_dunno.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_help.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_move.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_no.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_sorry.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_thanks.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_wait.mp3";
    streamed Soundfile "Voice\Female1\F1_vqk_yes.mp3";
    streamed Soundfile "Voice\Female1\F1_wrn_bomber.mp3";
    streamed Soundfile "Voice\Female1\F1_wrn_enemy.mp3";
    streamed Soundfile "Voice\Female1\F1_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Female1\F1_wrn_watchit.mp3";
    Soundfile "Voice\Female1\F1_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Female1\Null.mp3";
    streamed Soundfile "Voice\Female1\F1_avo_grunt.mp3";
    Soundfile "Voice\Female1\F1_avo_pain.mp3";
}


//                  Female 2 - 

Complex "SFX_VOICE_FEMALE_2"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Female2\F2_att_attack.mp3";
    streamed Soundfile "Voice\Female2\F2_att_base.mp3";
    streamed Soundfile "Voice\Female2\F2_att_chase.mp3";
    streamed Soundfile "Voice\Female2\F2_att_distract.mp3";
    streamed Soundfile "Voice\Female2\F2_att_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_att_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_att_objective.mp3";
    streamed Soundfile "Voice\Female2\F2_att_reinforcements.mp3";
    streamed Soundfile "Voice\Female2\F2_att_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_att_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_att_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_att_wait.mp3";
    streamed Soundfile "Voice\Female2\F2_bas_clear.mp3";
    streamed Soundfile "Voice\Female2\F2_bas_enemy.mp3";
    streamed Soundfile "Voice\Female2\F2_bas_retake.mp3";
    streamed Soundfile "Voice\Female2\F2_bas_secure.mp3";
    streamed Soundfile "Voice\Female2\F2_bas_taken.mp3";
    streamed Soundfile "Voice\Female2\F2_def_base.mp3";
    streamed Soundfile "Voice\Female2\F2_def_carrier.mp3";
    streamed Soundfile "Voice\Female2\F2_def_entrances.mp3";
    streamed Soundfile "Voice\Female2\F2_def_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_def_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_def_me.mp3";
    streamed Soundfile "Voice\Female2\F2_def_nexus.mp3";
    streamed Soundfile "Voice\Female2\F2_def_objective.mp3";
    streamed Soundfile "Voice\Female2\F2_def_reinforce.mp3";
    streamed Soundfile "Voice\Female2\F2_def_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_def_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_def_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_base.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_disarray.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_remotes.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_ene_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_give.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_huntergive.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_huntertake.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_retrieve.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_secure.mp3";
    streamed Soundfile "Voice\Female2\F2_flg_take.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_anytime.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_awesome.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_bye.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_dunno.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_hi.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_no.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_oops.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_quiet.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_rock.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_shooting.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_thanks.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_wait.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Female2\F2_gbl_yes.mp3";
    streamed Soundfile "Voice\Female2\F2_need_bombardier.mp3";
    streamed Soundfile "Voice\Female2\F2_need_cover.mp3";
    streamed Soundfile "Voice\Female2\F2_need_driver.mp3";
    streamed Soundfile "Voice\Female2\F2_need_escort.mp3";
    streamed Soundfile "Voice\Female2\F2_need_gunship.mp3";
    streamed Soundfile "Voice\Female2\F2_need_hold.mp3";
    streamed Soundfile "Voice\Female2\F2_need_pilot.mp3";
    streamed Soundfile "Voice\Female2\F2_need_ride.mp3";
    streamed Soundfile "Voice\Female2\F2_need_support.mp3";
    streamed Soundfile "Voice\Female2\F2_need_tailgunner.mp3";
    streamed Soundfile "Voice\Female2\F2_need_where.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_base.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_me.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_attack.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_base.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_base.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_defend.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_base.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_acquired.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_base.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_flag.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_need.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_sensors.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_target.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_turret.mp3";
    streamed Soundfile "Voice\Female2\F2_tgt_wait.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_anytime.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_base.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_dunno.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_help.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_move.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_no.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_sorry.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_thanks.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_wait.mp3";
    streamed Soundfile "Voice\Female2\F2_vqk_yes.mp3";
    streamed Soundfile "Voice\Female2\F2_wrn_bomber.mp3";
    streamed Soundfile "Voice\Female2\F2_wrn_enemy.mp3";
    streamed Soundfile "Voice\Female2\F2_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Female2\F2_wrn_watchit.mp3";
    Soundfile "Voice\Female2\F2_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Female2\Null.mp3";
    streamed Soundfile "Voice\Female2\F2_avo_grunt.mp3";
    Soundfile "Voice\Female2\F2_avo_pain.mp3";
}


//                  Female 3 - 

Complex "SFX_VOICE_FEMALE_3"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Female3\F3_att_attack.mp3";
    streamed Soundfile "Voice\Female3\F3_att_base.mp3";
    streamed Soundfile "Voice\Female3\F3_att_chase.mp3";
    streamed Soundfile "Voice\Female3\F3_att_distract.mp3";
    streamed Soundfile "Voice\Female3\F3_att_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_att_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_att_objective.mp3";
    streamed Soundfile "Voice\Female3\F3_att_reinforcements.mp3";
    streamed Soundfile "Voice\Female3\F3_att_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_att_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_att_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_att_wait.mp3";
    streamed Soundfile "Voice\Female3\F3_bas_clear.mp3";
    streamed Soundfile "Voice\Female3\F3_bas_enemy.mp3";
    streamed Soundfile "Voice\Female3\F3_bas_retake.mp3";
    streamed Soundfile "Voice\Female3\F3_bas_secure.mp3";
    streamed Soundfile "Voice\Female3\F3_bas_taken.mp3";
    streamed Soundfile "Voice\Female3\F3_def_base.mp3";
    streamed Soundfile "Voice\Female3\F3_def_carrier.mp3";
    streamed Soundfile "Voice\Female3\F3_def_entrances.mp3";
    streamed Soundfile "Voice\Female3\F3_def_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_def_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_def_me.mp3";
    streamed Soundfile "Voice\Female3\F3_def_nexus.mp3";
    streamed Soundfile "Voice\Female3\F3_def_objective.mp3";
    streamed Soundfile "Voice\Female3\F3_def_reinforce.mp3";
    streamed Soundfile "Voice\Female3\F3_def_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_def_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_def_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_base.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_disarray.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_remotes.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_ene_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_give.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_huntergive.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_huntertake.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_retrieve.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_secure.mp3";
    streamed Soundfile "Voice\Female3\F3_flg_take.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_anytime.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_awesome.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_bye.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_dunno.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_hi.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_no.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_oops.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_quiet.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_rock.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_shooting.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_thanks.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_wait.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Female3\F3_gbl_yes.mp3";
    streamed Soundfile "Voice\Female3\F3_need_bombardier.mp3";
    streamed Soundfile "Voice\Female3\F3_need_cover.mp3";
    streamed Soundfile "Voice\Female3\F3_need_driver.mp3";
    streamed Soundfile "Voice\Female3\F3_need_escort.mp3";
    streamed Soundfile "Voice\Female3\F3_need_gunship.mp3";
    streamed Soundfile "Voice\Female3\F3_need_hold.mp3";
    streamed Soundfile "Voice\Female3\F3_need_pilot.mp3";
    streamed Soundfile "Voice\Female3\F3_need_ride.mp3";
    streamed Soundfile "Voice\Female3\F3_need_support.mp3";
    streamed Soundfile "Voice\Female3\F3_need_tailgunner.mp3";
    streamed Soundfile "Voice\Female3\F3_need_where.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_base.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_me.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_attack.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_base.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_base.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_defend.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_base.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_acquired.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_base.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_flag.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_need.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_sensors.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_target.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_turret.mp3";
    streamed Soundfile "Voice\Female3\F3_tgt_wait.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_anytime.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_base.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_dunno.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_help.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_move.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_no.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_sorry.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_thanks.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_wait.mp3";
    streamed Soundfile "Voice\Female3\F3_vqk_yes.mp3";
    streamed Soundfile "Voice\Female3\F3_wrn_bomber.mp3";
    streamed Soundfile "Voice\Female3\F3_wrn_enemy.mp3";
    streamed Soundfile "Voice\Female3\F3_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Female3\F3_wrn_watchit.mp3";
    Soundfile "Voice\Female3\F3_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Female3\Null.mp3";
    streamed Soundfile "Voice\Female3\F3_avo_grunt.mp3";
    Soundfile "Voice\Female3\F3_avo_pain.mp3";
}


//                  Female 4 - 

Complex "SFX_VOICE_FEMALE_4"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Female4\F4_att_attack.mp3";
    streamed Soundfile "Voice\Female4\F4_att_base.mp3";
    streamed Soundfile "Voice\Female4\F4_att_chase.mp3";
    streamed Soundfile "Voice\Female4\F4_att_distract.mp3";
    streamed Soundfile "Voice\Female4\F4_att_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_att_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_att_objective.mp3";
    streamed Soundfile "Voice\Female4\F4_att_reinforcements.mp3";
    streamed Soundfile "Voice\Female4\F4_att_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_att_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_att_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_att_wait.mp3";
    streamed Soundfile "Voice\Female4\F4_bas_clear.mp3";
    streamed Soundfile "Voice\Female4\F4_bas_enemy.mp3";
    streamed Soundfile "Voice\Female4\F4_bas_retake.mp3";
    streamed Soundfile "Voice\Female4\F4_bas_secure.mp3";
    streamed Soundfile "Voice\Female4\F4_bas_taken.mp3";
    streamed Soundfile "Voice\Female4\F4_def_base.mp3";
    streamed Soundfile "Voice\Female4\F4_def_carrier.mp3";
    streamed Soundfile "Voice\Female4\F4_def_entrances.mp3";
    streamed Soundfile "Voice\Female4\F4_def_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_def_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_def_me.mp3";
    streamed Soundfile "Voice\Female4\F4_def_nexus.mp3";
    streamed Soundfile "Voice\Female4\F4_def_objective.mp3";
    streamed Soundfile "Voice\Female4\F4_def_reinforce.mp3";
    streamed Soundfile "Voice\Female4\F4_def_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_def_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_def_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_base.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_disarray.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_remotes.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_ene_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_give.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_huntergive.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_huntertake.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_retrieve.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_secure.mp3";
    streamed Soundfile "Voice\Female4\F4_flg_take.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_anytime.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_awesome.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_bye.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_dunno.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_hi.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_no.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_oops.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_quiet.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_rock.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_shooting.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_thanks.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_wait.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Female4\F4_gbl_yes.mp3";
    streamed Soundfile "Voice\Female4\F4_need_bombardier.mp3";
    streamed Soundfile "Voice\Female4\F4_need_cover.mp3";
    streamed Soundfile "Voice\Female4\F4_need_driver.mp3";
    streamed Soundfile "Voice\Female4\F4_need_escort.mp3";
    streamed Soundfile "Voice\Female4\F4_need_gunship.mp3";
    streamed Soundfile "Voice\Female4\F4_need_hold.mp3";
    streamed Soundfile "Voice\Female4\F4_need_pilot.mp3";
    streamed Soundfile "Voice\Female4\F4_need_ride.mp3";
    streamed Soundfile "Voice\Female4\F4_need_support.mp3";
    streamed Soundfile "Voice\Female4\F4_need_tailgunner.mp3";
    streamed Soundfile "Voice\Female4\F4_need_where.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_base.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_me.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_attack.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_base.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_base.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_defend.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_base.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_acquired.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_base.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_flag.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_need.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_sensors.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_target.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_turret.mp3";
    streamed Soundfile "Voice\Female4\F4_tgt_wait.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_anytime.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_base.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_dunno.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_help.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_move.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_no.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_sorry.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_thanks.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_wait.mp3";
    streamed Soundfile "Voice\Female4\F4_vqk_yes.mp3";
    streamed Soundfile "Voice\Female4\F4_wrn_bomber.mp3";
    streamed Soundfile "Voice\Female4\F4_wrn_enemy.mp3";
    streamed Soundfile "Voice\Female4\F4_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Female4\F4_wrn_watchit.mp3";
    Soundfile "Voice\Female4\F4_avo_deathcry_02.mp3";
    streamed Soundfile "Voice\Female4\Null.mp3";
    streamed Soundfile "Voice\Female4\F4_avo_grunt.mp3";
    Soundfile "Voice\Female4\F4_avo_pain.mp3";
}


//                  Female 5 - Amazon

Complex "SFX_VOICE_FEMALE_5"
{
    Volume  0.80;
    Pitch   1.0;
    Priority    40;

    streamed Soundfile "Voice\Female5\F5_att_attack.mp3";
    streamed Soundfile "Voice\Female5\F5_att_base.mp3";
    streamed Soundfile "Voice\Female5\F5_att_chase.mp3";
    streamed Soundfile "Voice\Female5\F5_att_distract.mp3";
    streamed Soundfile "Voice\Female5\F5_att_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_att_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_att_objective.mp3";
    streamed Soundfile "Voice\Female5\F5_att_reinforcements.mp3";
    streamed Soundfile "Voice\Female5\F5_att_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_att_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_att_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_att_wait.mp3";
    streamed Soundfile "Voice\Female5\F5_bas_clear.mp3";
    streamed Soundfile "Voice\Female5\F5_bas_enemy.mp3";
    streamed Soundfile "Voice\Female5\F5_bas_retake.mp3";
    streamed Soundfile "Voice\Female5\F5_bas_secure.mp3";
    streamed Soundfile "Voice\Female5\F5_bas_taken.mp3";
    streamed Soundfile "Voice\Female5\F5_def_base.mp3";
    streamed Soundfile "Voice\Female5\F5_def_carrier.mp3";
    streamed Soundfile "Voice\Female5\F5_def_entrances.mp3";
    streamed Soundfile "Voice\Female5\F5_def_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_def_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_def_me.mp3";
    streamed Soundfile "Voice\Female5\F5_def_nexus.mp3";
    streamed Soundfile "Voice\Female5\F5_def_objective.mp3";
    streamed Soundfile "Voice\Female5\F5_def_reinforce.mp3";
    streamed Soundfile "Voice\Female5\F5_def_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_def_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_def_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_base.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_disarray.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_remotes.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_ene_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_give.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_huntergive.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_huntertake.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_retrieve.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_secure.mp3";
    streamed Soundfile "Voice\Female5\F5_flg_take.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_anytime.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_awesome.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_bye.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_dunno.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_goodgame.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_hi.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_no.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_oops.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_quiet.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_rock.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_shazbot.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_shooting.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt1.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt2.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt3.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt4.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_thanks.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_wait.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_woohoo.mp3";
    streamed Soundfile "Voice\Female5\F5_gbl_yes.mp3";
    streamed Soundfile "Voice\Female5\F5_need_bombardier.mp3";
    streamed Soundfile "Voice\Female5\F5_need_cover.mp3";
    streamed Soundfile "Voice\Female5\F5_need_driver.mp3";
    streamed Soundfile "Voice\Female5\F5_need_escort.mp3";
    streamed Soundfile "Voice\Female5\F5_need_gunship.mp3";
    streamed Soundfile "Voice\Female5\F5_need_hold.mp3";
    streamed Soundfile "Voice\Female5\F5_need_pilot.mp3";
    streamed Soundfile "Voice\Female5\F5_need_ride.mp3";
    streamed Soundfile "Voice\Female5\F5_need_support.mp3";
    streamed Soundfile "Voice\Female5\F5_need_tailgunner.mp3";
    streamed Soundfile "Voice\Female5\F5_need_where.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_base.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_me.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_attack.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_base.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_att_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_base.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_defend.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_nexus.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_def_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_base.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_equipment.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_generator.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_repairing.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_rep_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_cover.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_defense.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_onit.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_remotes.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_turrets.mp3";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_vehicle.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_acquired.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_base.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_destroyed.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_flag.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_need.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_sensors.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_target.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_turret.mp3";
    streamed Soundfile "Voice\Female5\F5_tgt_wait.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_anytime.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_base.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_ceasefire.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_dunno.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_help.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_move.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_no.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_sorry.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_thanks.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_wait.mp3";
    streamed Soundfile "Voice\Female5\F5_vqk_yes.mp3";
    streamed Soundfile "Voice\Female5\F5_wrn_bomber.mp3";
    streamed Soundfile "Voice\Female5\F5_wrn_enemy.mp3";
    streamed Soundfile "Voice\Female5\F5_wrn_vehicles.mp3";
    streamed Soundfile "Voice\Female5\F5_wrn_watchit.mp3";
    Soundfile "Voice\Female5\F5_avo_deathcry_01.mp3";
    streamed Soundfile "Voice\Female5\Null.mp3";
    streamed Soundfile "Voice\Female5\F5_avo_grunt.mp3";
    Soundfile "Voice\Female5\F5_avo_pain.mp3";
}



//                  Training


Complex "T1_HEALTH_METER"
{
    Streamed Soundfile "Voice\Training\T1_HEALTH_METER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_HUD_TEXT1"
{
    Streamed Soundfile "Voice\Training\T1_HUD_TEXT1.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_HUD_TEXT2"
{
    Streamed Soundfile "Voice\Training\T1_HUD_TEXT2.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_HUD_TEXT3 "
{
    Streamed Soundfile "Voice\Training\T1_HUD_TEXT3 .mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_INTRO"
{
    Streamed Soundfile "Voice\Training\T1_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_INVENTORY"
{
    Streamed Soundfile "Voice\Training\T1_INVENTORY.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_JETPACK"
{
    Streamed Soundfile "Voice\Training\T1_JETPACK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_JUMP"
{
    Streamed Soundfile "Voice\Training\T1_JUMP.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_LOOK_DOWN "
{
    Streamed Soundfile "Voice\Training\T1_LOOK_DOWN .mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_LOOK_LEFTRIGHT"
{
    Streamed Soundfile "Voice\Training\T1_LOOK_LEFTRIGHT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_LOOK_UP"
{
    Streamed Soundfile "Voice\Training\T1_LOOK_UP.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_MISSILE_LOCK"
{
    Streamed Soundfile "Voice\Training\T1_MISSILE_LOCK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_MOVE_BACK"
{
    Streamed Soundfile "Voice\Training\T1_MOVE_BACK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_MOVE_FORWARD"
{
    Streamed Soundfile "Voice\Training\T1_MOVE_FORWARD.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_MOVE_SIDEWAYS"
{
    Streamed Soundfile "Voice\Training\T1_MOVE_SIDEWAYS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_RETICLE"
{
    Streamed Soundfile "Voice\Training\T1_RETICLE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_SCOREBOARD"
{
    Streamed Soundfile "Voice\Training\T1_SCOREBOARD.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_SUCCESS"
{
    Streamed Soundfile "Voice\Training\T1_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T1_TOUCH_SWITCHES"
{
    Streamed Soundfile "Voice\Training\T1_TOUCH_SWITCHES.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_BLASTER"
{
    Streamed Soundfile "Voice\Training\T2_BLASTER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_CHAINGUN "
{
    Streamed Soundfile "Voice\Training\T2_CHAINGUN .mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_CYCLE_WEAPONS"
{
    Streamed Soundfile "Voice\Training\T2_CYCLE_WEAPONS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_DISKLAUNCHER"
{
    Streamed Soundfile "Voice\Training\T2_DISKLAUNCHER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_ELFGUN"
{
    Streamed Soundfile "Voice\Training\T2_ELFGUN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_EXCHANGE_WEAPON"
{
    Streamed Soundfile "Voice\Training\T2_EXCHANGE_WEAPON.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_GOTO_SNIPER_RANGE"
{
    Streamed Soundfile "Voice\Training\T2_GOTO_SNIPER_RANGE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_GRENADELAUNCHER"
{
    Streamed Soundfile "Voice\Training\T2_GRENADELAUNCHER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_INTRO"
{
    Streamed Soundfile "Voice\Training\T2_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_PLASMA"
{
    Streamed Soundfile "Voice\Training\T2_PLASMA.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_SNIPERRIFLE"
{
    Streamed Soundfile "Voice\Training\T2_SNIPERRIFLE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_USE_ZOOM"
{
    Streamed Soundfile "Voice\Training\T2_USE_ZOOM.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_WELLDONE"
{
    Streamed Soundfile "Voice\Training\T2_WELLDONE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_COMPLETE"
{
    Streamed Soundfile "Voice\Training\T3_COMPLETE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_CONCUSSION_GRENADE"
{
    Streamed Soundfile "Voice\Training\T3_CONCUSSION_GRENADE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_FLARE_GRENADE"
{
    Streamed Soundfile "Voice\Training\T3_FLARE_GRENADE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_FRAG_GRENADE"
{
    Streamed Soundfile "Voice\Training\T3_FRAG_GRENADE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_MINE"
{
    Streamed Soundfile "Voice\Training\T3_MINE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_MISSILELAUNCHER"
{
    Streamed Soundfile "Voice\Training\T3_MISSILELAUNCHER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_MORTAR"
{
    Streamed Soundfile "Voice\Training\T3_MORTAR.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_REPAIR_PATCH"
{
    Streamed Soundfile "Voice\Training\T3_REPAIR_PATCH.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T3_SWITCH_TO_MISSILE"
{
    Streamed Soundfile "Voice\Training\T3_SWITCH_TO_MISSILE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_COMPLETE"
{
    Streamed Soundfile "Voice\Training\T4_COMPLETE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_DEPLOY_INVEN"
{
    Streamed Soundfile "Voice\Training\T4_DEPLOY_INVEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_DEPLOY_SENSOR"
{
    Streamed Soundfile "Voice\Training\T4_DEPLOY_SENSOR.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_DEPLOY_TURRET"
{
    Streamed Soundfile "Voice\Training\T4_DEPLOY_TURRET.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_GOTO_REPAIR_PACK"
{
    Streamed Soundfile "Voice\Training\T4_GOTO_REPAIR_PACK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_REPAIR_INVEN"
{
    Streamed Soundfile "Voice\Training\T4_REPAIR_INVEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_SHIELD_PACK"
{
    Streamed Soundfile "Voice\Training\T4_SHIELD_PACK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_SKI_TO_BUNKER"
{
    Streamed Soundfile "Voice\Training\T4_SKI_TO_BUNKER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_SWAP_TURRET_BARREL"
{
    Streamed Soundfile "Voice\Training\T4_SWAP_TURRET_BARREL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_USE_DEPLOYED_INVEN"
{
    Streamed Soundfile "Voice\Training\T4_USE_DEPLOYED_INVEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_USE_INVEN"
{
    Streamed Soundfile "Voice\Training\T4_USE_INVEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T4_WRONG_LOADOUT"
{
    Streamed Soundfile "Voice\Training\T4_WRONG_LOADOUT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_COMPLETE"
{
    Streamed Soundfile "Voice\Training\T5_COMPLETE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_GET_BOMBER"
{
    Streamed Soundfile "Voice\Training\T5_GET_BOMBER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_GET_GRAV_CYCLE"
{
    Streamed Soundfile "Voice\Training\T5_GET_GRAV_CYCLE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_GET_IN_VEHICLE"
{
    Streamed Soundfile "Voice\Training\T5_GET_IN_VEHICLE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_GET_SCOUT"
{
    Streamed Soundfile "Voice\Training\T5_GET_SCOUT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_GET_TRANSPORT"
{
    Streamed Soundfile "Voice\Training\T5_GET_TRANSPORT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_INFO_BOMBER"
{
    Streamed Soundfile "Voice\Training\T5_INFO_BOMBER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_INFO_GRAV_CYCLE"
{
    Streamed Soundfile "Voice\Training\T5_INFO_GRAV_CYCLE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_INFO_SCOUT"
{
    Streamed Soundfile "Voice\Training\T5_INFO_SCOUT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_INFO_TRANSPORT"
{
    Streamed Soundfile "Voice\Training\T5_INFO_TRANSPORT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_INTRO"
{
    Streamed Soundfile "Voice\Training\T5_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_JUMP_OUT"
{
    Streamed Soundfile "Voice\Training\T5_JUMP_OUT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T5_WRONG_SEAT"
{
    Streamed Soundfile "Voice\Training\T5_WRONG_SEAT.mp3";
    Volume  1.0;
    Priority    100;
}



//    NEW CAMPAIGN STUFF

Complex "M01_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M01_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M01_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M01_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M01_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M01_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M01_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M01_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M01_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M01_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M02_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M02_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M02_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_HALFWAY"
{
    Streamed Soundfile "Voice\Campaign\M02_HALFWAY.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M02_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_ONE_TO_GO"
{
    Streamed Soundfile "Voice\Campaign\M02_ONE_TO_GO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M02_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_TAUNT_1"
{
    Streamed Soundfile "Voice\Campaign\M02_TAUNT_1.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M02_TAUNT_2"
{
    Streamed Soundfile "Voice\Campaign\M02_TAUNT_2.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M03_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M03_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M03_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M03_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M03_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M03_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M03_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M03_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M03_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M03_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M04_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M04_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M04_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M04_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M04_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M04_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M04_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M04_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M04_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M04_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M05_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M05_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M05_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M05_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M05_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M05_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M05_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M05_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M05_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M05_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M06_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M06_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_DEFEND_BASE"
{
    Streamed Soundfile "Voice\Campaign\M06_DEFEND_BASE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M06_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_GO_TO_BASE"
{
    Streamed Soundfile "Voice\Campaign\M06_GO_TO_BASE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_GO_TO_GEN"
{
    Streamed Soundfile "Voice\Campaign\M06_GO_TO_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_GO_TO_STATION"
{
    Streamed Soundfile "Voice\Campaign\M06_GO_TO_STATION.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_GO_TO_TURRET"
{
    Streamed Soundfile "Voice\Campaign\M06_GO_TO_TURRET.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_HANG_IN_THERE"
{
    Streamed Soundfile "Voice\Campaign\M06_HANG_IN_THERE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M06_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_REPAIR_GEN"
{
    Streamed Soundfile "Voice\Campaign\M06_REPAIR_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_REPAIR_TURRET"
{
    Streamed Soundfile "Voice\Campaign\M06_REPAIR_TURRET.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_SCRUBBED"
{
    Streamed Soundfile "Voice\Campaign\M06_SCRUBBED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M06_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_SWAP_BARRELS"
{
    Streamed Soundfile "Voice\Campaign\M06_SWAP_BARRELS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M06_TIME"
{
    Streamed Soundfile "Voice\Campaign\M06_TIME.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_BASE_AHEAD"
{
    Streamed Soundfile "Voice\Campaign\M07_BASE_AHEAD.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_BASE_WAYPOINT"
{
    Streamed Soundfile "Voice\Campaign\M07_BASE_WAYPOINT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M07_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M07_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_DESTROY_GEN"
{
    Streamed Soundfile "Voice\Campaign\M07_DESTROY_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_GET_MOVING"
{
    Streamed Soundfile "Voice\Campaign\M07_GET_MOVING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_HEAVY_DESTROYED"
{
    Streamed Soundfile "Voice\Campaign\M07_HEAVY_DESTROYED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M07_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_PULLING_BACK"
{
    Streamed Soundfile "Voice\Campaign\M07_PULLING_BACK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_SCRUBBED"
{
    Streamed Soundfile "Voice\Campaign\M07_SCRUBBED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M07_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_WAY_TO_GO"
{
    Streamed Soundfile "Voice\Campaign\M07_WAY_TO_GO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M07_WE_ARE_IN"
{
    Streamed Soundfile "Voice\Campaign\M07_WE_ARE_IN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M08_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M08_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_DESTROY_TURRET"
{
    Streamed Soundfile "Voice\Campaign\M08_DESTROY_TURRET.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_DONT_WANDER"
{
    Streamed Soundfile "Voice\Campaign\M08_DONT_WANDER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M08_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_HEAVY_LOST"
{
    Streamed Soundfile "Voice\Campaign\M08_HEAVY_LOST.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_HIT_SWITCH"
{
    Streamed Soundfile "Voice\Campaign\M08_HIT_SWITCH.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M08_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_MOP_UP"
{
    Streamed Soundfile "Voice\Campaign\M08_MOP_UP.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_MOVE_OUT"
{
    Streamed Soundfile "Voice\Campaign\M08_MOVE_OUT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_SCRUBBED"
{
    Streamed Soundfile "Voice\Campaign\M08_SCRUBBED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_STAY_TOGETHER"
{
    Streamed Soundfile "Voice\Campaign\M08_STAY_TOGETHER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M08_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_TERMINATED"
{
    Streamed Soundfile "Voice\Campaign\M08_TERMINATED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M08_TURRET_DESTROYED"
{
    Streamed Soundfile "Voice\Campaign\M08_TURRET_DESTROYED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_BOMBER_LOST"
{
    Streamed Soundfile "Voice\Campaign\M10_BOMBER_LOST.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M10_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M10_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_GET_GOING"
{
    Streamed Soundfile "Voice\Campaign\M10_GET_GOING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_GET_SWITCH"
{
    Streamed Soundfile "Voice\Campaign\M10_GET_SWITCH.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_GO_TO_MARK"
{
    Streamed Soundfile "Voice\Campaign\M10_GO_TO_MARK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_GO_TO_SWITCH"
{
    Streamed Soundfile "Voice\Campaign\M10_GO_TO_SWITCH.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_INCOMING"
{
    Streamed Soundfile "Voice\Campaign\M10_INCOMING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M10_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_NO_BOMBER"
{
    Streamed Soundfile "Voice\Campaign\M10_NO_BOMBER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_ON_OWN"
{
    Streamed Soundfile "Voice\Campaign\M10_ON_OWN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_SCRUBBED"
{
    Streamed Soundfile "Voice\Campaign\M10_SCRUBBED.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M10_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M10_WHY_WAIT"
{
    Streamed Soundfile "Voice\Campaign\M10_WHY_WAIT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_BATTLE_STATIONS"
{
    Streamed Soundfile "Voice\Campaign\M11_BATTLE_STATIONS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M11_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M11_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_DEFEND_GEN"
{
    Streamed Soundfile "Voice\Campaign\M11_DEFEND_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_ENEMY_COMING"
{
    Streamed Soundfile "Voice\Campaign\M11_ENEMY_COMING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_EVEN_MORE_ENEMY"
{
    Streamed Soundfile "Voice\Campaign\M11_EVEN_MORE_ENEMY.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M11_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_FAIL_GENS_LOST"
{
    Streamed Soundfile "Voice\Campaign\M11_FAIL_GENS_LOST.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_GEN_LOST"
{
    Streamed Soundfile "Voice\Campaign\M11_GEN_LOST.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_GEN_ONLINE"
{
    Streamed Soundfile "Voice\Campaign\M11_GEN_ONLINE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_GO_TO_GEN"
{
    Streamed Soundfile "Voice\Campaign\M11_GO_TO_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M11_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_MORE_ENEMY"
{
    Streamed Soundfile "Voice\Campaign\M11_MORE_ENEMY.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_STAY_AT_GEN"
{
    Streamed Soundfile "Voice\Campaign\M11_STAY_AT_GEN.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M11_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M11_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_BRIEFING"
{
    Streamed Soundfile "Voice\Campaign\M13_BRIEFING.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_BYE"
{
    Streamed Soundfile "Voice\Campaign\M13_BYE.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_DEBRIEF"
{
    Streamed Soundfile "Voice\Campaign\M13_DEBRIEF.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_FAIL"
{
    Streamed Soundfile "Voice\Campaign\M13_FAIL.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_GO_TO_UNIT"
{
    Streamed Soundfile "Voice\Campaign\M13_GO_TO_UNIT.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_HIT_SWITCH"
{
    Streamed Soundfile "Voice\Campaign\M13_HIT_SWITCH.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_INTRO"
{
    Streamed Soundfile "Voice\Campaign\M13_INTRO.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_KEEP_IT_UP"
{
    Streamed Soundfile "Voice\Campaign\M13_KEEP_IT_UP.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_KICK_ASS"
{
    Streamed Soundfile "Voice\Campaign\M13_KICK_ASS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_STAY_TOGETHER"
{
    Streamed Soundfile "Voice\Campaign\M13_STAY_TOGETHER.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "M13_SUCCESS"
{
    Streamed Soundfile "Voice\Campaign\M13_SUCCESS.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_AUTOLOCK"
{
    Streamed Soundfile "Voice\Campaign\T2_AUTOLOCK.mp3";
    Volume  1.0;
    Priority    100;
}

Complex "T2_KILL_BOT"
{
    Streamed Soundfile "Voice\Campaign\T2_KILL_BOT.mp3";
    Volume  1.0;
    Priority    100;
}


