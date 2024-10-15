//          Audio Script - Audio - Temporary

Default Pitch   1.0;
Default Priority    120;


//                  Announcer

Complex "SFX_VOICE_ANN_10SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_10sec.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_15SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_15sec.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_30SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_30.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_60SECONDS"
{
    Streamed Soundfile "Voice\Announcer\ANN_60.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_LOOKS_LIKE_CLOSE_ONE"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_01.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_FIGHT_TO_FINISH"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_03.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_CLOSEGAME_REAL_BATTLE"
{
    Streamed Soundfile "Voice\Announcer\ANN_closegame_04.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GAMEOVER"
{
    Streamed Soundfile "Voice\Announcer\ANN_gameover.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GAMEPOINT_IMMINENT"
{
    Streamed Soundfile "Voice\Announcer\ANN_gamepoint_imminent.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_GREED_MODE"
{
    Streamed Soundfile "Voice\Announcer\ANN_greed.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_HOARD_MODE"
{
    Streamed Soundfile "Voice\Announcer\ANN_hoard.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_HOLD_ON_TO_FLAGS"
{
    Streamed Soundfile "Voice\Announcer\ANN_holdflags.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_DENIED"
{
    Streamed Soundfile "Voice\Announcer\ANN_infdenied.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_SCORES"
{
    Streamed Soundfile "Voice\Announcer\ANN_infscores.aif"
    {
        Delay   1.2;
        Volume  0.90;
    }
}

Complex "SFX_VOICE_ANN_TEAM_INFERNO_WINS"
{
    Streamed Soundfile "Voice\Announcer\ANN_infwins.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_MATCH_BEGINS_NOW"
{
    Streamed Soundfile "Voice\Announcer\ANN_match_begins.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_PREPARE_YOURSELF"
{
    Streamed Soundfile "Voice\Announcer\ANN_prepare.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_STORM_DENIED"
{
    Streamed Soundfile "Voice\Announcer\ANN_stodenied.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_TEAM_STORM_SCORES"
{
    Streamed Soundfile "Voice\Announcer\ANN_stoscores.aif"
    {
        Delay   1.2;
        Volume  0.90;
    }
}

Complex "SFX_VOICE_ANN_TEAM_STORM_WINS"
{
    Streamed Soundfile "Voice\Announcer\ANN_stowins.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_FAIL"
{
    Streamed Soundfile "Voice\Announcer\ANN_votefail.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_INITIATED"
{
    Streamed Soundfile "Voice\Announcer\ANN_voteinit.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_VOTE_PASSES"
{
    Streamed Soundfile "Voice\Announcer\ANN_votepass.aif";
    Volume  0.90;
}

Complex "SFX_VOICE_ANN_YARDSALE"
{
    Streamed Soundfile "Voice\Announcer\ANN_yardsale.aif";
    Volume  0.90;
}


//                  Bot 1 - 

Complex "SFX_VOICE_BOT_1"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Bot1\B1_att_attack.aif";
    streamed Soundfile "Voice\Bot1\B1_att_base.aif";
    streamed Soundfile "Voice\Bot1\B1_att_chase.aif";
    streamed Soundfile "Voice\Bot1\B1_att_distract.aif";
    streamed Soundfile "Voice\Bot1\B1_att_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_att_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_att_objective.aif";
    streamed Soundfile "Voice\Bot1\B1_att_reinforcements.aif";
    streamed Soundfile "Voice\Bot1\B1_att_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_att_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_att_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_att_wait.aif";
    streamed Soundfile "Voice\Bot1\B1_bas_clear.aif";
    streamed Soundfile "Voice\Bot1\B1_bas_enemy.aif";
    streamed Soundfile "Voice\Bot1\B1_bas_retake.aif";
    streamed Soundfile "Voice\Bot1\B1_bas_secure.aif";
    streamed Soundfile "Voice\Bot1\B1_bas_taken.aif";
    streamed Soundfile "Voice\Bot1\B1_def_base.aif";
    streamed Soundfile "Voice\Bot1\B1_def_carrier.aif";
    streamed Soundfile "Voice\Bot1\B1_def_entrances.aif";
    streamed Soundfile "Voice\Bot1\B1_def_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_def_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_def_me.aif";
    streamed Soundfile "Voice\Bot1\B1_def_nexus.aif";
    streamed Soundfile "Voice\Bot1\B1_def_objective.aif";
    streamed Soundfile "Voice\Bot1\B1_def_reinforce.aif";
    streamed Soundfile "Voice\Bot1\B1_def_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_def_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_def_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_base.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_disarray.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_remotes.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_ene_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_give.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_huntergive.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_huntertake.aif";
    streamed Soundfile "Voice\Null.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_secure.aif";
    streamed Soundfile "Voice\Bot1\B1_flg_take.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_anytime.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_awesome.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_bye.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_dunno.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_goodgame.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_hi.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_no.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_oops.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_quiet.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_rock.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_shazbot.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_shooting.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt1.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt2.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt3.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_taunt4.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_thanks.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_wait.aif";
    streamed Soundfile "Voice\Null.aif";
    streamed Soundfile "Voice\Bot1\B1_gbl_yes.aif";
    streamed Soundfile "Voice\Bot1\B1_need_bombardier.aif";
    streamed Soundfile "Voice\Bot1\B1_need_cover.aif";
    streamed Soundfile "Voice\Bot1\B1_need_driver.aif";
    streamed Soundfile "Voice\Bot1\B1_need_escort.aif";
    streamed Soundfile "Voice\Bot1\B1_need_gunship.aif";
    streamed Soundfile "Voice\Bot1\B1_need_hold.aif";
    streamed Soundfile "Voice\Bot1\B1_need_pilot.aif";
    streamed Soundfile "Voice\Bot1\B1_need_ride.aif";
    streamed Soundfile "Voice\Bot1\B1_need_support.aif";
    streamed Soundfile "Voice\Bot1\B1_need_tailgunner.aif";
    streamed Soundfile "Voice\Bot1\B1_need_where.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_base.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_me.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_rep_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_attack.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_base.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_base.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_defend.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_nexus.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_base.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_generator.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Bot1\B1_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_acquired.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_base.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_destroyed.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_flag.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_need.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_sensors.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_target.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_turret.aif";
    streamed Soundfile "Voice\Bot1\B1_tgt_wait.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_anytime.aif";
    streamed Soundfile "Voice\Null.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_dunno.aif";
    streamed Soundfile "Voice\Null.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_move.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_no.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_sorry.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_thanks.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_wait.aif";
    streamed Soundfile "Voice\Bot1\B1_vqk_yes.aif";
    streamed Soundfile "Voice\Bot1\B1_wrn_bomber.aif";
    streamed Soundfile "Voice\Bot1\B1_wrn_enemy.aif";
    streamed Soundfile "Voice\Bot1\B1_wrn_vehicles.aif";
    streamed Soundfile "Voice\Bot1\B1_wrn_watchit.aif";
    Soundfile "Voice\Bot1\B1_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Bot1\Null.aif";
    streamed Soundfile "Voice\Bot1\B1_avo_grunt.aif";
    Soundfile "Voice\Bot1\B1_avo_pain.aif";
}



//                  BioDerm 1 - 

Complex "SFX_VOICE_DERM_1"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Derm1\D1_att_attack.aif";
    streamed Soundfile "Voice\Derm1\D1_att_base.aif";
    streamed Soundfile "Voice\Derm1\D1_att_chase.aif";
    streamed Soundfile "Voice\Derm1\D1_att_distract.aif";
    streamed Soundfile "Voice\Derm1\D1_att_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_att_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_att_objective.aif";
    streamed Soundfile "Voice\Derm1\D1_att_reinforcements.aif";
    streamed Soundfile "Voice\Derm1\D1_att_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_att_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_att_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_att_wait.aif";
    streamed Soundfile "Voice\Derm1\D1_bas_clear.aif";
    streamed Soundfile "Voice\Derm1\D1_bas_enemy.aif";
    streamed Soundfile "Voice\Derm1\D1_bas_retake.aif";
    streamed Soundfile "Voice\Derm1\D1_bas_secure.aif";
    streamed Soundfile "Voice\Derm1\D1_bas_taken.aif";
    streamed Soundfile "Voice\Derm1\D1_def_base.aif";
    streamed Soundfile "Voice\Derm1\D1_def_carrier.aif";
    streamed Soundfile "Voice\Derm1\D1_def_entrances.aif";
    streamed Soundfile "Voice\Derm1\D1_def_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_def_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_def_me.aif";
    streamed Soundfile "Voice\Derm1\D1_def_nexus.aif";
    streamed Soundfile "Voice\Derm1\D1_def_objective.aif";
    streamed Soundfile "Voice\Derm1\D1_def_reinforce.aif";
    streamed Soundfile "Voice\Derm1\D1_def_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_def_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_def_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_base.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_disarray.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_remotes.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_ene_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_give.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_huntergive.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_huntertake.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_retrieve.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_secure.aif";
    streamed Soundfile "Voice\Derm1\D1_flg_take.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_anytime.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_awesome.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_bye.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_dunno.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_goodgame.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_hi.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_no.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_oops.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_quiet.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_rock.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_shazbot.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_shooting.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt1.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt2.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt3.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_taunt4.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_thanks.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_wait.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_woohoo.aif";
    streamed Soundfile "Voice\Derm1\D1_gbl_yes.aif";
    streamed Soundfile "Voice\Derm1\D1_need_bombardier.aif";
    streamed Soundfile "Voice\Derm1\D1_need_cover.aif";
    streamed Soundfile "Voice\Derm1\D1_need_driver.aif";
    streamed Soundfile "Voice\Derm1\D1_need_escort.aif";
    streamed Soundfile "Voice\Derm1\D1_need_gunship.aif";
    streamed Soundfile "Voice\Derm1\D1_need_hold.aif";
    streamed Soundfile "Voice\Derm1\D1_need_pilot.aif";
    streamed Soundfile "Voice\Derm1\D1_need_ride.aif";
    streamed Soundfile "Voice\Derm1\D1_need_support.aif";
    streamed Soundfile "Voice\Derm1\D1_need_tailgunner.aif";
    streamed Soundfile "Voice\Derm1\D1_need_where.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_base.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_me.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_attack.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_base.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_base.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_defend.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_nexus.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_base.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_generator.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Derm1\D1_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_acquired.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_base.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_destroyed.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_flag.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_need.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_sensors.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_target.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_turret.aif";
    streamed Soundfile "Voice\Derm1\D1_tgt_wait.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_anytime.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_base.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_dunno.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_help.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_move.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_no.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_sorry.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_thanks.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_wait.aif";
    streamed Soundfile "Voice\Derm1\D1_vqk_yes.aif";
    streamed Soundfile "Voice\Derm1\D1_wrn_bomber.aif";
    streamed Soundfile "Voice\Derm1\D1_wrn_enemy.aif";
    streamed Soundfile "Voice\Derm1\D1_wrn_vehicles.aif";
    streamed Soundfile "Voice\Derm1\D1_wrn_watchit.aif";
    Soundfile "Voice\Derm1\D1_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Derm1\Null.aif";
    streamed Soundfile "Voice\Derm1\D1_avo_grunt.aif";
    Soundfile "Voice\Derm1\D1_avo_pain.aif";
}


//                  BioDerm 2 - 

Complex "SFX_VOICE_DERM_2"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Derm2\D2_att_attack.aif";
    streamed Soundfile "Voice\Derm2\D2_att_base.aif";
    streamed Soundfile "Voice\Derm2\D2_att_chase.aif";
    streamed Soundfile "Voice\Derm2\D2_att_distract.aif";
    streamed Soundfile "Voice\Derm2\D2_att_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_att_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_att_objective.aif";
    streamed Soundfile "Voice\Derm2\D2_att_reinforcements.aif";
    streamed Soundfile "Voice\Derm2\D2_att_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_att_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_att_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_att_wait.aif";
    streamed Soundfile "Voice\Derm2\D2_bas_clear.aif";
    streamed Soundfile "Voice\Derm2\D2_bas_enemy.aif";
    streamed Soundfile "Voice\Derm2\D2_bas_retake.aif";
    streamed Soundfile "Voice\Derm2\D2_bas_secure.aif";
    streamed Soundfile "Voice\Derm2\D2_bas_taken.aif";
    streamed Soundfile "Voice\Derm2\D2_def_base.aif";
    streamed Soundfile "Voice\Derm2\D2_def_carrier.aif";
    streamed Soundfile "Voice\Derm2\D2_def_entrances.aif";
    streamed Soundfile "Voice\Derm2\D2_def_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_def_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_def_me.aif";
    streamed Soundfile "Voice\Derm2\D2_def_nexus.aif";
    streamed Soundfile "Voice\Derm2\D2_def_objective.aif";
    streamed Soundfile "Voice\Derm2\D2_def_reinforce.aif";
    streamed Soundfile "Voice\Derm2\D2_def_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_def_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_def_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_base.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_disarray.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_remotes.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_ene_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_give.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_huntergive.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_huntertake.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_retrieve.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_secure.aif";
    streamed Soundfile "Voice\Derm2\D2_flg_take.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_anytime.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_awesome.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_bye.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_dunno.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_goodgame.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_hi.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_no.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_oops.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_quiet.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_rock.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_shazbot.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_shooting.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt1.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt2.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt3.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_taunt4.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_thanks.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_wait.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_woohoo.aif";
    streamed Soundfile "Voice\Derm2\D2_gbl_yes.aif";
    streamed Soundfile "Voice\Derm2\D2_need_bombardier.aif";
    streamed Soundfile "Voice\Derm2\D2_need_cover.aif";
    streamed Soundfile "Voice\Derm2\D2_need_driver.aif";
    streamed Soundfile "Voice\Derm2\D2_need_escort.aif";
    streamed Soundfile "Voice\Derm2\D2_need_gunship.aif";
    streamed Soundfile "Voice\Derm2\D2_need_hold.aif";
    streamed Soundfile "Voice\Derm2\D2_need_pilot.aif";
    streamed Soundfile "Voice\Derm2\D2_need_ride.aif";
    streamed Soundfile "Voice\Derm2\D2_need_support.aif";
    streamed Soundfile "Voice\Derm2\D2_need_tailgunner.aif";
    streamed Soundfile "Voice\Derm2\D2_need_where.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_base.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_me.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_attack.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_base.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_base.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_defend.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_nexus.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_base.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_generator.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Derm2\D2_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_acquired.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_base.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_destroyed.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_flag.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_need.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_sensors.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_target.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_turret.aif";
    streamed Soundfile "Voice\Derm2\D2_tgt_wait.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_anytime.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_base.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_dunno.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_help.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_move.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_no.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_sorry.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_thanks.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_wait.aif";
    streamed Soundfile "Voice\Derm2\D2_vqk_yes.aif";
    streamed Soundfile "Voice\Derm2\D2_wrn_bomber.aif";
    streamed Soundfile "Voice\Derm2\D2_wrn_enemy.aif";
    streamed Soundfile "Voice\Derm2\D2_wrn_vehicles.aif";
    streamed Soundfile "Voice\Derm2\D2_wrn_watchit.aif";
    Soundfile "Voice\Derm2\D2_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Derm2\Null.aif";
    streamed Soundfile "Voice\Derm2\D2_avo_grunt.aif";
    Soundfile "Voice\Derm2\D2_avo_pain.aif";
}


//                  BioDerm 3 - 

Complex "SFX_VOICE_DERM_3"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Derm3\D3_att_attack.aif";
    streamed Soundfile "Voice\Derm3\D3_att_base.aif";
    streamed Soundfile "Voice\Derm3\D3_att_chase.aif";
    streamed Soundfile "Voice\Derm3\D3_att_distract.aif";
    streamed Soundfile "Voice\Derm3\D3_att_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_att_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_att_objective.aif";
    streamed Soundfile "Voice\Derm3\D3_att_reinforcements.aif";
    streamed Soundfile "Voice\Derm3\D3_att_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_att_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_att_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_att_wait.aif";
    streamed Soundfile "Voice\Derm3\D3_bas_clear.aif";
    streamed Soundfile "Voice\Derm3\D3_bas_enemy.aif";
    streamed Soundfile "Voice\Derm3\D3_bas_retake.aif";
    streamed Soundfile "Voice\Derm3\D3_bas_secure.aif";
    streamed Soundfile "Voice\Derm3\D3_bas_taken.aif";
    streamed Soundfile "Voice\Derm3\D3_def_base.aif";
    streamed Soundfile "Voice\Derm3\D3_def_carrier.aif";
    streamed Soundfile "Voice\Derm3\D3_def_entrances.aif";
    streamed Soundfile "Voice\Derm3\D3_def_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_def_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_def_me.aif";
    streamed Soundfile "Voice\Derm3\D3_def_nexus.aif";
    streamed Soundfile "Voice\Derm3\D3_def_objective.aif";
    streamed Soundfile "Voice\Derm3\D3_def_reinforce.aif";
    streamed Soundfile "Voice\Derm3\D3_def_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_def_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_def_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_base.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_disarray.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_remotes.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_ene_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_give.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_huntergive.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_huntertake.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_retrieve.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_secure.aif";
    streamed Soundfile "Voice\Derm3\D3_flg_take.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_anytime.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_awesome.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_bye.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_dunno.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_goodgame.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_hi.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_no.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_oops.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_quiet.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_rock.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_shazbot.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_shooting.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt1.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt2.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt3.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_taunt4.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_thanks.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_wait.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_woohoo.aif";
    streamed Soundfile "Voice\Derm3\D3_gbl_yes.aif";
    streamed Soundfile "Voice\Derm3\D3_need_bombardier.aif";
    streamed Soundfile "Voice\Derm3\D3_need_cover.aif";
    streamed Soundfile "Voice\Derm3\D3_need_driver.aif";
    streamed Soundfile "Voice\Derm3\D3_need_escort.aif";
    streamed Soundfile "Voice\Derm3\D3_need_gunship.aif";
    streamed Soundfile "Voice\Derm3\D3_need_hold.aif";
    streamed Soundfile "Voice\Derm3\D3_need_pilot.aif";
    streamed Soundfile "Voice\Derm3\D3_need_ride.aif";
    streamed Soundfile "Voice\Derm3\D3_need_support.aif";
    streamed Soundfile "Voice\Derm3\D3_need_tailgunner.aif";
    streamed Soundfile "Voice\Derm3\D3_need_where.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_base.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_me.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_attack.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_base.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_base.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_defend.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_nexus.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_base.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_generator.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Derm3\D3_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_acquired.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_base.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_destroyed.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_flag.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_need.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_sensors.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_target.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_turret.aif";
    streamed Soundfile "Voice\Derm3\D3_tgt_wait.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_anytime.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_base.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_dunno.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_help.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_move.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_no.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_sorry.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_thanks.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_wait.aif";
    streamed Soundfile "Voice\Derm3\D3_vqk_yes.aif";
    streamed Soundfile "Voice\Derm3\D3_wrn_bomber.aif";
    streamed Soundfile "Voice\Derm3\D3_wrn_enemy.aif";
    streamed Soundfile "Voice\Derm3\D3_wrn_vehicles.aif";
    streamed Soundfile "Voice\Derm3\D3_wrn_watchit.aif";
    Soundfile "Voice\Derm3\D3_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Derm3\Null.aif";
    streamed Soundfile "Voice\Derm3\D3_avo_grunt.aif";
    Soundfile "Voice\Derm3\D3_avo_pain.aif";
}



//                  Male 1 - 

Complex "SFX_VOICE_MALE_1"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Male1\M1_att_attack.aif";
    streamed Soundfile "Voice\Male1\M1_att_base.aif";
    streamed Soundfile "Voice\Male1\M1_att_chase.aif";
    streamed Soundfile "Voice\Male1\M1_att_distract.aif";
    streamed Soundfile "Voice\Male1\M1_att_flag.aif";
    streamed Soundfile "Voice\Male1\M1_att_generator.aif";
    streamed Soundfile "Voice\Male1\M1_att_objective.aif";
    streamed Soundfile "Voice\Male1\M1_att_reinforcements.aif";
    streamed Soundfile "Voice\Male1\M1_att_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_att_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_att_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_att_wait.aif";
    streamed Soundfile "Voice\Male1\M1_bas_clear.aif";
    streamed Soundfile "Voice\Male1\M1_bas_enemy.aif";
    streamed Soundfile "Voice\Male1\M1_bas_retake.aif";
    streamed Soundfile "Voice\Male1\M1_bas_secure.aif";
    streamed Soundfile "Voice\Male1\M1_bas_taken.aif";
    streamed Soundfile "Voice\Male1\M1_def_base.aif";
    streamed Soundfile "Voice\Male1\M1_def_carrier.aif";
    streamed Soundfile "Voice\Male1\M1_def_entrances.aif";
    streamed Soundfile "Voice\Male1\M1_def_flag.aif";
    streamed Soundfile "Voice\Male1\M1_def_generator.aif";
    streamed Soundfile "Voice\Male1\M1_def_me.aif";
    streamed Soundfile "Voice\Male1\M1_def_nexus.aif";
    streamed Soundfile "Voice\Male1\M1_def_objective.aif";
    streamed Soundfile "Voice\Male1\M1_def_reinforce.aif";
    streamed Soundfile "Voice\Male1\M1_def_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_def_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_def_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_ene_base.aif";
    streamed Soundfile "Voice\Male1\M1_ene_disarray.aif";
    streamed Soundfile "Voice\Male1\M1_ene_generator.aif";
    streamed Soundfile "Voice\Male1\M1_ene_remotes.aif";
    streamed Soundfile "Voice\Male1\M1_ene_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_ene_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_ene_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_flg_flag.aif";
    streamed Soundfile "Voice\Male1\M1_flg_give.aif";
    streamed Soundfile "Voice\Male1\M1_flg_huntergive.aif";
    streamed Soundfile "Voice\Male1\M1_flg_huntertake.aif";
    streamed Soundfile "Voice\Male1\M1_flg_retrieve.aif";
    streamed Soundfile "Voice\Male1\M1_flg_secure.aif";
    streamed Soundfile "Voice\Male1\M1_flg_take.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_anytime.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_awesome.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_bye.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_dunno.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_goodgame.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_hi.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_no.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_oops.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_quiet.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_rock.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_shazbot.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_shooting.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt1.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt2.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt3.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_taunt4.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_thanks.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_wait.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_woohoo.aif";
    streamed Soundfile "Voice\Male1\M1_gbl_yes.aif";
    streamed Soundfile "Voice\Male1\M1_need_bombardier.aif";
    streamed Soundfile "Voice\Male1\M1_need_cover.aif";
    streamed Soundfile "Voice\Male1\M1_need_driver.aif";
    streamed Soundfile "Voice\Male1\M1_need_escort.aif";
    streamed Soundfile "Voice\Male1\M1_need_gunship.aif";
    streamed Soundfile "Voice\Male1\M1_need_hold.aif";
    streamed Soundfile "Voice\Male1\M1_need_pilot.aif";
    streamed Soundfile "Voice\Male1\M1_need_ride.aif";
    streamed Soundfile "Voice\Male1\M1_need_support.aif";
    streamed Soundfile "Voice\Male1\M1_need_tailgunner.aif";
    streamed Soundfile "Voice\Male1\M1_need_where.aif";
    streamed Soundfile "Voice\Male1\M1_rep_base.aif";
    streamed Soundfile "Voice\Male1\M1_rep_generator.aif";
    streamed Soundfile "Voice\Male1\M1_rep_me.aif";
    streamed Soundfile "Voice\Male1\M1_rep_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_rep_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_rep_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_attack.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_base.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_flag.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_generator.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_base.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_defend.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_flag.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_generator.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_nexus.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_base.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_generator.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Male1\M1_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_acquired.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_base.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_destroyed.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_flag.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_need.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_sensors.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_target.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_turret.aif";
    streamed Soundfile "Voice\Male1\M1_tgt_wait.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_anytime.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_base.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_dunno.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_help.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_move.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_no.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_sorry.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_thanks.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_wait.aif";
    streamed Soundfile "Voice\Male1\M1_vqk_yes.aif";
    streamed Soundfile "Voice\Male1\M1_wrn_bomber.aif";
    streamed Soundfile "Voice\Male1\M1_wrn_enemy.aif";
    streamed Soundfile "Voice\Male1\M1_wrn_vehicles.aif";
    streamed Soundfile "Voice\Male1\M1_wrn_watchit.aif";
    Soundfile "Voice\Male1\M1_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Male1\Null.aif";
    streamed Soundfile "Voice\Male1\M1_avo_grunt.aif";
    Soundfile "Voice\Male1\M1_avo_pain.aif";
}


//                  Male 2 - 

Complex "SFX_VOICE_MALE_2"
{
    Volume  1.0;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Male2\M2_att_attack.aif";
    streamed Soundfile "Voice\Male2\M2_att_base.aif";
    streamed Soundfile "Voice\Male2\M2_att_chase.aif";
    streamed Soundfile "Voice\Male2\M2_att_distract.aif";
    streamed Soundfile "Voice\Male2\M2_att_flag.aif";
    streamed Soundfile "Voice\Male2\M2_att_generator.aif";
    streamed Soundfile "Voice\Male2\M2_att_objective.aif";
    streamed Soundfile "Voice\Male2\M2_att_reinforcements.aif";
    streamed Soundfile "Voice\Male2\M2_att_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_att_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_att_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_att_wait.aif";
    streamed Soundfile "Voice\Male2\M2_bas_clear.aif";
    streamed Soundfile "Voice\Male2\M2_bas_enemy.aif";
    streamed Soundfile "Voice\Male2\M2_bas_retake.aif";
    streamed Soundfile "Voice\Male2\M2_bas_secure.aif";
    streamed Soundfile "Voice\Male2\M2_bas_taken.aif";
    streamed Soundfile "Voice\Male2\M2_def_base.aif";
    streamed Soundfile "Voice\Male2\M2_def_carrier.aif";
    streamed Soundfile "Voice\Male2\M2_def_entrances.aif";
    streamed Soundfile "Voice\Male2\M2_def_flag.aif";
    streamed Soundfile "Voice\Male2\M2_def_generator.aif";
    streamed Soundfile "Voice\Male2\M2_def_me.aif";
    streamed Soundfile "Voice\Male2\M2_def_nexus.aif";
    streamed Soundfile "Voice\Male2\M2_def_objective.aif";
    streamed Soundfile "Voice\Male2\M2_def_reinforce.aif";
    streamed Soundfile "Voice\Male2\M2_def_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_def_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_def_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_ene_base.aif";
    streamed Soundfile "Voice\Male2\M2_ene_disarray.aif";
    streamed Soundfile "Voice\Male2\M2_ene_generator.aif";
    streamed Soundfile "Voice\Male2\M2_ene_remotes.aif";
    streamed Soundfile "Voice\Male2\M2_ene_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_ene_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_ene_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_flg_flag.aif";
    streamed Soundfile "Voice\Male2\M2_flg_give.aif";
    streamed Soundfile "Voice\Male2\M2_flg_huntergive.aif";
    streamed Soundfile "Voice\Male2\M2_flg_huntertake.aif";
    streamed Soundfile "Voice\Male2\M2_flg_retrieve.aif";
    streamed Soundfile "Voice\Male2\M2_flg_secure.aif";
    streamed Soundfile "Voice\Male2\M2_flg_take.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_anytime.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_awesome.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_bye.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_dunno.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_goodgame.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_hi.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_no.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_oops.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_quiet.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_rock.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_shazbot.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_shooting.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt1.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt2.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt3.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_taunt4.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_thanks.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_wait.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_woohoo.aif";
    streamed Soundfile "Voice\Male2\M2_gbl_yes.aif";
    streamed Soundfile "Voice\Male2\M2_need_bombardier.aif";
    streamed Soundfile "Voice\Male2\M2_need_cover.aif";
    streamed Soundfile "Voice\Male2\M2_need_driver.aif";
    streamed Soundfile "Voice\Male2\M2_need_escort.aif";
    streamed Soundfile "Voice\Male2\M2_need_gunship.aif";
    streamed Soundfile "Voice\Male2\M2_need_hold.aif";
    streamed Soundfile "Voice\Male2\M2_need_pilot.aif";
    streamed Soundfile "Voice\Male2\M2_need_ride.aif";
    streamed Soundfile "Voice\Male2\M2_need_support.aif";
    streamed Soundfile "Voice\Male2\M2_need_tailgunner.aif";
    streamed Soundfile "Voice\Male2\M2_need_where.aif";
    streamed Soundfile "Voice\Male2\M2_rep_base.aif";
    streamed Soundfile "Voice\Male2\M2_rep_generator.aif";
    streamed Soundfile "Voice\Male2\M2_rep_me.aif";
    streamed Soundfile "Voice\Male2\M2_rep_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_rep_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_rep_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_attack.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_base.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_flag.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_generator.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_base.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_defend.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_flag.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_generator.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_nexus.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_base.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_generator.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Male2\M2_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_acquired.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_base.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_destroyed.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_flag.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_need.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_sensors.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_target.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_turret.aif";
    streamed Soundfile "Voice\Male2\M2_tgt_wait.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_anytime.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_base.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_dunno.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_help.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_move.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_no.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_sorry.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_thanks.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_wait.aif";
    streamed Soundfile "Voice\Male2\M2_vqk_yes.aif";
    streamed Soundfile "Voice\Male2\M2_wrn_bomber.aif";
    streamed Soundfile "Voice\Male2\M2_wrn_enemy.aif";
    streamed Soundfile "Voice\Male2\M2_wrn_vehicles.aif";
    streamed Soundfile "Voice\Male2\M2_wrn_watchit.aif";
    Soundfile "Voice\Male2\M2_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Male2\Null.aif";
    streamed Soundfile "Voice\Male2\M2_avo_grunt.aif";
    Soundfile "Voice\Male2\M2_avo_pain.aif";
}


//                  Male 3 - 

Complex "SFX_VOICE_MALE_3"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Male3\M3_att_attack.aif";
    streamed Soundfile "Voice\Male3\M3_att_base.aif";
    streamed Soundfile "Voice\Male3\M3_att_chase.aif";
    streamed Soundfile "Voice\Male3\M3_att_distract.aif";
    streamed Soundfile "Voice\Male3\M3_att_flag.aif";
    streamed Soundfile "Voice\Male3\M3_att_generator.aif";
    streamed Soundfile "Voice\Male3\M3_att_objective.aif";
    streamed Soundfile "Voice\Male3\M3_att_reinforcements.aif";
    streamed Soundfile "Voice\Male3\M3_att_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_att_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_att_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_att_wait.aif";
    streamed Soundfile "Voice\Male3\M3_bas_clear.aif";
    streamed Soundfile "Voice\Male3\M3_bas_enemy.aif";
    streamed Soundfile "Voice\Male3\M3_bas_retake.aif";
    streamed Soundfile "Voice\Male3\M3_bas_secure.aif";
    streamed Soundfile "Voice\Male3\M3_bas_taken.aif";
    streamed Soundfile "Voice\Male3\M3_def_base.aif";
    streamed Soundfile "Voice\Male3\M3_def_carrier.aif";
    streamed Soundfile "Voice\Male3\M3_def_entrances.aif";
    streamed Soundfile "Voice\Male3\M3_def_flag.aif";
    streamed Soundfile "Voice\Male3\M3_def_generator.aif";
    streamed Soundfile "Voice\Male3\M3_def_me.aif";
    streamed Soundfile "Voice\Male3\M3_def_nexus.aif";
    streamed Soundfile "Voice\Male3\M3_def_objective.aif";
    streamed Soundfile "Voice\Male3\M3_def_reinforce.aif";
    streamed Soundfile "Voice\Male3\M3_def_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_def_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_def_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_ene_base.aif";
    streamed Soundfile "Voice\Male3\M3_ene_disarray.aif";
    streamed Soundfile "Voice\Male3\M3_ene_generator.aif";
    streamed Soundfile "Voice\Male3\M3_ene_remotes.aif";
    streamed Soundfile "Voice\Male3\M3_ene_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_ene_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_ene_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_flg_flag.aif";
    streamed Soundfile "Voice\Male3\M3_flg_give.aif";
    streamed Soundfile "Voice\Male3\M3_flg_huntergive.aif";
    streamed Soundfile "Voice\Male3\M3_flg_huntertake.aif";
    streamed Soundfile "Voice\Male3\M3_flg_retrieve.aif";
    streamed Soundfile "Voice\Male3\M3_flg_secure.aif";
    streamed Soundfile "Voice\Male3\M3_flg_take.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_anytime.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_awesome.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_bye.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_dunno.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_goodgame.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_hi.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_no.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_oops.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_quiet.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_rock.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_shazbot.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_shooting.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt1.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt2.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt3.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_taunt4.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_thanks.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_wait.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_woohoo.aif";
    streamed Soundfile "Voice\Male3\M3_gbl_yes.aif";
    streamed Soundfile "Voice\Male3\M3_need_bombardier.aif";
    streamed Soundfile "Voice\Male3\M3_need_cover.aif";
    streamed Soundfile "Voice\Male3\M3_need_driver.aif";
    streamed Soundfile "Voice\Male3\M3_need_escort.aif";
    streamed Soundfile "Voice\Male3\M3_need_gunship.aif";
    streamed Soundfile "Voice\Male3\M3_need_hold.aif";
    streamed Soundfile "Voice\Male3\M3_need_pilot.aif";
    streamed Soundfile "Voice\Male3\M3_need_ride.aif";
    streamed Soundfile "Voice\Male3\M3_need_support.aif";
    streamed Soundfile "Voice\Male3\M3_need_tailgunner.aif";
    streamed Soundfile "Voice\Male3\M3_need_where.aif";
    streamed Soundfile "Voice\Male3\M3_rep_base.aif";
    streamed Soundfile "Voice\Male3\M3_rep_generator.aif";
    streamed Soundfile "Voice\Male3\M3_rep_me.aif";
    streamed Soundfile "Voice\Male3\M3_rep_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_rep_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_rep_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_attack.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_base.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_flag.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_generator.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_base.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_defend.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_flag.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_generator.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_nexus.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_base.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_generator.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Male3\M3_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_acquired.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_base.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_destroyed.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_flag.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_need.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_sensors.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_target.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_turret.aif";
    streamed Soundfile "Voice\Male3\M3_tgt_wait.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_anytime.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_base.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_dunno.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_help.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_move.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_no.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_sorry.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_thanks.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_wait.aif";
    streamed Soundfile "Voice\Male3\M3_vqk_yes.aif";
    streamed Soundfile "Voice\Male3\M3_wrn_bomber.aif";
    streamed Soundfile "Voice\Male3\M3_wrn_enemy.aif";
    streamed Soundfile "Voice\Male3\M3_wrn_vehicles.aif";
    streamed Soundfile "Voice\Male3\M3_wrn_watchit.aif";
    Soundfile "Voice\Male3\M3_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Male3\Null.aif";
    streamed Soundfile "Voice\Male3\M3_avo_grunt.aif";
    Soundfile "Voice\Male3\M3_avo_pain.aif";
}


//                  Male 4 - 

Complex "SFX_VOICE_MALE_4"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Male4\M4_att_attack.aif";
    streamed Soundfile "Voice\Male4\M4_att_base.aif";
    streamed Soundfile "Voice\Male4\M4_att_chase.aif";
    streamed Soundfile "Voice\Male4\M4_att_distract.aif";
    streamed Soundfile "Voice\Male4\M4_att_flag.aif";
    streamed Soundfile "Voice\Male4\M4_att_generator.aif";
    streamed Soundfile "Voice\Male4\M4_att_objective.aif";
    streamed Soundfile "Voice\Male4\M4_att_reinforcements.aif";
    streamed Soundfile "Voice\Male4\M4_att_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_att_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_att_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_att_wait.aif";
    streamed Soundfile "Voice\Male4\M4_bas_clear.aif";
    streamed Soundfile "Voice\Male4\M4_bas_enemy.aif";
    streamed Soundfile "Voice\Male4\M4_bas_retake.aif";
    streamed Soundfile "Voice\Male4\M4_bas_secure.aif";
    streamed Soundfile "Voice\Male4\M4_bas_taken.aif";
    streamed Soundfile "Voice\Male4\M4_def_base.aif";
    streamed Soundfile "Voice\Male4\M4_def_carrier.aif";
    streamed Soundfile "Voice\Male4\M4_def_entrances.aif";
    streamed Soundfile "Voice\Male4\M4_def_flag.aif";
    streamed Soundfile "Voice\Male4\M4_def_generator.aif";
    streamed Soundfile "Voice\Male4\M4_def_me.aif";
    streamed Soundfile "Voice\Male4\M4_def_nexus.aif";
    streamed Soundfile "Voice\Male4\M4_def_objective.aif";
    streamed Soundfile "Voice\Male4\M4_def_reinforce.aif";
    streamed Soundfile "Voice\Male4\M4_def_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_def_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_def_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_ene_base.aif";
    streamed Soundfile "Voice\Male4\M4_ene_disarray.aif";
    streamed Soundfile "Voice\Male4\M4_ene_generator.aif";
    streamed Soundfile "Voice\Male4\M4_ene_remotes.aif";
    streamed Soundfile "Voice\Male4\M4_ene_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_ene_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_ene_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_flg_flag.aif";
    streamed Soundfile "Voice\Male4\M4_flg_give.aif";
    streamed Soundfile "Voice\Male4\M4_flg_huntergive.aif";
    streamed Soundfile "Voice\Male4\M4_flg_huntertake.aif";
    streamed Soundfile "Voice\Male4\M4_flg_retrieve.aif";
    streamed Soundfile "Voice\Male4\M4_flg_secure.aif";
    streamed Soundfile "Voice\Male4\M4_flg_take.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_anytime.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_awesome.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_bye.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_dunno.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_goodgame.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_hi.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_no.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_oops.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_quiet.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_rock.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_shazbot.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_shooting.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt1.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt2.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt3.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_taunt4.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_thanks.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_wait.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_woohoo.aif";
    streamed Soundfile "Voice\Male4\M4_gbl_yes.aif";
    streamed Soundfile "Voice\Male4\M4_need_bombardier.aif";
    streamed Soundfile "Voice\Male4\M4_need_cover.aif";
    streamed Soundfile "Voice\Male4\M4_need_driver.aif";
    streamed Soundfile "Voice\Male4\M4_need_escort.aif";
    streamed Soundfile "Voice\Male4\M4_need_gunship.aif";
    streamed Soundfile "Voice\Male4\M4_need_hold.aif";
    streamed Soundfile "Voice\Male4\M4_need_pilot.aif";
    streamed Soundfile "Voice\Male4\M4_need_ride.aif";
    streamed Soundfile "Voice\Male4\M4_need_support.aif";
    streamed Soundfile "Voice\Male4\M4_need_tailgunner.aif";
    streamed Soundfile "Voice\Male4\M4_need_where.aif";
    streamed Soundfile "Voice\Male4\M4_rep_base.aif";
    streamed Soundfile "Voice\Male4\M4_rep_generator.aif";
    streamed Soundfile "Voice\Male4\M4_rep_me.aif";
    streamed Soundfile "Voice\Male4\M4_rep_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_rep_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_rep_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_attack.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_base.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_flag.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_generator.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_base.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_defend.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_flag.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_generator.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_nexus.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_base.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_generator.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Male4\M4_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_acquired.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_base.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_destroyed.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_flag.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_need.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_sensors.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_target.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_turret.aif";
    streamed Soundfile "Voice\Male4\M4_tgt_wait.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_anytime.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_base.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_dunno.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_help.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_move.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_no.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_sorry.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_thanks.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_wait.aif";
    streamed Soundfile "Voice\Male4\M4_vqk_yes.aif";
    streamed Soundfile "Voice\Male4\M4_wrn_bomber.aif";
    streamed Soundfile "Voice\Male4\M4_wrn_enemy.aif";
    streamed Soundfile "Voice\Male4\M4_wrn_vehicles.aif";
    streamed Soundfile "Voice\Male4\M4_wrn_watchit.aif";
    Soundfile "Voice\Male4\M4_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Male4\Null.aif";
    streamed Soundfile "Voice\Male4\M4_avo_grunt.aif";
    Soundfile "Voice\Male4\M4_avo_pain.aif";
}


//                  Male 5 - Psycho

Complex "SFX_VOICE_MALE_5"
{
    Volume  0.85;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Male5\M5_att_attack.aif";
    streamed Soundfile "Voice\Male5\M5_att_base.aif";
    streamed Soundfile "Voice\Male5\M5_att_chase.aif";
    streamed Soundfile "Voice\Male5\M5_att_distract.aif";
    streamed Soundfile "Voice\Male5\M5_att_flag.aif";
    streamed Soundfile "Voice\Male5\M5_att_generator.aif";
    streamed Soundfile "Voice\Male5\M5_att_objective.aif";
    streamed Soundfile "Voice\Male5\M5_att_reinforcements.aif";
    streamed Soundfile "Voice\Male5\M5_att_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_att_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_att_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_att_wait.aif";
    streamed Soundfile "Voice\Male5\M5_bas_clear.aif";
    streamed Soundfile "Voice\Male5\M5_bas_enemy.aif";
    streamed Soundfile "Voice\Male5\M5_bas_retake.aif";
    streamed Soundfile "Voice\Male5\M5_bas_secure.aif";
    streamed Soundfile "Voice\Male5\M5_bas_taken.aif";
    streamed Soundfile "Voice\Male5\M5_def_base.aif";
    streamed Soundfile "Voice\Male5\M5_def_carrier.aif";
    streamed Soundfile "Voice\Male5\M5_def_entrances.aif";
    streamed Soundfile "Voice\Male5\M5_def_flag.aif";
    streamed Soundfile "Voice\Male5\M5_def_generator.aif";
    streamed Soundfile "Voice\Male5\M5_def_me.aif";
    streamed Soundfile "Voice\Male5\M5_def_nexus.aif";
    streamed Soundfile "Voice\Male5\M5_def_objective.aif";
    streamed Soundfile "Voice\Male5\M5_def_reinforce.aif";
    streamed Soundfile "Voice\Male5\M5_def_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_def_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_def_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_ene_base.aif";
    streamed Soundfile "Voice\Male5\M5_ene_disarray.aif";
    streamed Soundfile "Voice\Male5\M5_ene_generator.aif";
    streamed Soundfile "Voice\Male5\M5_ene_remotes.aif";
    streamed Soundfile "Voice\Male5\M5_ene_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_ene_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_ene_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_flg_flag.aif";
    streamed Soundfile "Voice\Male5\M5_flg_give.aif";
    streamed Soundfile "Voice\Male5\M5_flg_huntergive.aif";
    streamed Soundfile "Voice\Male5\M5_flg_huntertake.aif";
    streamed Soundfile "Voice\Male5\M5_flg_retrieve.aif";
    streamed Soundfile "Voice\Male5\M5_flg_secure.aif";
    streamed Soundfile "Voice\Male5\M5_flg_take.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_anytime.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_awesome.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_bye.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_dunno.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_goodgame.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_hi.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_no.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_oops.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_quiet.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_rock.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_shazbot.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_shooting.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt1.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt2.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt3.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_taunt4.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_thanks.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_wait.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_woohoo.aif";
    streamed Soundfile "Voice\Male5\M5_gbl_yes.aif";
    streamed Soundfile "Voice\Male5\M5_need_bombardier.aif";
    streamed Soundfile "Voice\Male5\M5_need_cover.aif";
    streamed Soundfile "Voice\Male5\M5_need_driver.aif";
    streamed Soundfile "Voice\Male5\M5_need_escort.aif";
    streamed Soundfile "Voice\Male5\M5_need_gunship.aif";
    streamed Soundfile "Voice\Male5\M5_need_hold.aif";
    streamed Soundfile "Voice\Male5\M5_need_pilot.aif";
    streamed Soundfile "Voice\Male5\M5_need_ride.aif";
    streamed Soundfile "Voice\Male5\M5_need_support.aif";
    streamed Soundfile "Voice\Male5\M5_need_tailgunner.aif";
    streamed Soundfile "Voice\Male5\M5_need_where.aif";
    streamed Soundfile "Voice\Male5\M5_rep_base.aif";
    streamed Soundfile "Voice\Male5\M5_rep_generator.aif";
    streamed Soundfile "Voice\Male5\M5_rep_me.aif";
    streamed Soundfile "Voice\Male5\M5_rep_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_rep_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_rep_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_attack.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_base.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_flag.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_generator.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_base.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_defend.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_flag.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_generator.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_nexus.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_base.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_generator.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Male5\M5_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_acquired.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_base.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_destroyed.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_flag.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_need.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_sensors.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_target.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_turret.aif";
    streamed Soundfile "Voice\Male5\M5_tgt_wait.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_anytime.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_base.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_dunno.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_help.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_move.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_no.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_sorry.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_thanks.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_wait.aif";
    streamed Soundfile "Voice\Male5\M5_vqk_yes.aif";
    streamed Soundfile "Voice\Male5\M5_wrn_bomber.aif";
    streamed Soundfile "Voice\Male5\M5_wrn_enemy.aif";
    streamed Soundfile "Voice\Male5\M5_wrn_vehicles.aif";
    streamed Soundfile "Voice\Male5\M5_wrn_watchit.aif";
    Soundfile "Voice\Male5\M5_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Male5\Null.aif";
    streamed Soundfile "Voice\Male5\M5_avo_grunt.aif";
    Soundfile "Voice\Male5\M5_avo_pain.aif";
}



//                  Female 1 - 

Complex "SFX_VOICE_FEMALE_1"
{
    Volume  0.90;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Female1\F1_att_attack.aif";
    streamed Soundfile "Voice\Female1\F1_att_base.aif";
    streamed Soundfile "Voice\Female1\F1_att_chase.aif";
    streamed Soundfile "Voice\Female1\F1_att_distract.aif";
    streamed Soundfile "Voice\Female1\F1_att_flag.aif";
    streamed Soundfile "Voice\Female1\F1_att_generator.aif";
    streamed Soundfile "Voice\Female1\F1_att_objective.aif";
    streamed Soundfile "Voice\Female1\F1_att_reinforcements.aif";
    streamed Soundfile "Voice\Female1\F1_att_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_att_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_att_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_att_wait.aif";
    streamed Soundfile "Voice\Female1\F1_bas_clear.aif";
    streamed Soundfile "Voice\Female1\F1_bas_enemy.aif";
    streamed Soundfile "Voice\Female1\F1_bas_retake.aif";
    streamed Soundfile "Voice\Female1\F1_bas_secure.aif";
    streamed Soundfile "Voice\Female1\F1_bas_taken.aif";
    streamed Soundfile "Voice\Female1\F1_def_base.aif";
    streamed Soundfile "Voice\Female1\F1_def_carrier.aif";
    streamed Soundfile "Voice\Female1\F1_def_entrances.aif";
    streamed Soundfile "Voice\Female1\F1_def_flag.aif";
    streamed Soundfile "Voice\Female1\F1_def_generator.aif";
    streamed Soundfile "Voice\Female1\F1_def_me.aif";
    streamed Soundfile "Voice\Female1\F1_def_nexus.aif";
    streamed Soundfile "Voice\Female1\F1_def_objective.aif";
    streamed Soundfile "Voice\Female1\F1_def_reinforce.aif";
    streamed Soundfile "Voice\Female1\F1_def_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_def_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_def_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_ene_base.aif";
    streamed Soundfile "Voice\Female1\F1_ene_disarray.aif";
    streamed Soundfile "Voice\Female1\F1_ene_generator.aif";
    streamed Soundfile "Voice\Female1\F1_ene_remotes.aif";
    streamed Soundfile "Voice\Female1\F1_ene_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_ene_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_ene_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_flg_flag.aif";
    streamed Soundfile "Voice\Female1\F1_flg_give.aif";
    streamed Soundfile "Voice\Female1\F1_flg_huntergive.aif";
    streamed Soundfile "Voice\Female1\F1_flg_huntertake.aif";
    streamed Soundfile "Voice\Female1\F1_flg_retrieve.aif";
    streamed Soundfile "Voice\Female1\F1_flg_secure.aif";
    streamed Soundfile "Voice\Female1\F1_flg_take.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_anytime.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_awesome.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_bye.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_dunno.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_goodgame.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_hi.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_no.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_oops.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_quiet.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_rock.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_shazbot.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_shooting.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt1.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt2.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt3.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_taunt4.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_thanks.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_wait.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_woohoo.aif";
    streamed Soundfile "Voice\Female1\F1_gbl_yes.aif";
    streamed Soundfile "Voice\Female1\F1_need_bombardier.aif";
    streamed Soundfile "Voice\Female1\F1_need_cover.aif";
    streamed Soundfile "Voice\Female1\F1_need_driver.aif";
    streamed Soundfile "Voice\Female1\F1_need_escort.aif";
    streamed Soundfile "Voice\Female1\F1_need_gunship.aif";
    streamed Soundfile "Voice\Female1\F1_need_hold.aif";
    streamed Soundfile "Voice\Female1\F1_need_pilot.aif";
    streamed Soundfile "Voice\Female1\F1_need_ride.aif";
    streamed Soundfile "Voice\Female1\F1_need_support.aif";
    streamed Soundfile "Voice\Female1\F1_need_tailgunner.aif";
    streamed Soundfile "Voice\Female1\F1_need_where.aif";
    streamed Soundfile "Voice\Female1\F1_rep_base.aif";
    streamed Soundfile "Voice\Female1\F1_rep_generator.aif";
    streamed Soundfile "Voice\Female1\F1_rep_me.aif";
    streamed Soundfile "Voice\Female1\F1_rep_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_rep_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_rep_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_attack.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_base.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_flag.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_generator.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_base.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_defend.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_flag.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_generator.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_nexus.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_base.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_generator.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Female1\F1_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_acquired.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_base.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_destroyed.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_flag.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_need.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_sensors.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_target.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_turret.aif";
    streamed Soundfile "Voice\Female1\F1_tgt_wait.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_anytime.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_base.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_dunno.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_help.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_move.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_no.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_sorry.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_thanks.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_wait.aif";
    streamed Soundfile "Voice\Female1\F1_vqk_yes.aif";
    streamed Soundfile "Voice\Female1\F1_wrn_bomber.aif";
    streamed Soundfile "Voice\Female1\F1_wrn_enemy.aif";
    streamed Soundfile "Voice\Female1\F1_wrn_vehicles.aif";
    streamed Soundfile "Voice\Female1\F1_wrn_watchit.aif";
    Soundfile "Voice\Female1\F1_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Female1\Null.aif";
    streamed Soundfile "Voice\Female1\F1_avo_grunt.aif";
    Soundfile "Voice\Female1\F1_avo_pain.aif";
}


//                  Female 2 - 

Complex "SFX_VOICE_FEMALE_2"
{
    Volume  0.90;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Female2\F2_att_attack.aif";
    streamed Soundfile "Voice\Female2\F2_att_base.aif";
    streamed Soundfile "Voice\Female2\F2_att_chase.aif";
    streamed Soundfile "Voice\Female2\F2_att_distract.aif";
    streamed Soundfile "Voice\Female2\F2_att_flag.aif";
    streamed Soundfile "Voice\Female2\F2_att_generator.aif";
    streamed Soundfile "Voice\Female2\F2_att_objective.aif";
    streamed Soundfile "Voice\Female2\F2_att_reinforcements.aif";
    streamed Soundfile "Voice\Female2\F2_att_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_att_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_att_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_att_wait.aif";
    streamed Soundfile "Voice\Female2\F2_bas_clear.aif";
    streamed Soundfile "Voice\Female2\F2_bas_enemy.aif";
    streamed Soundfile "Voice\Female2\F2_bas_retake.aif";
    streamed Soundfile "Voice\Female2\F2_bas_secure.aif";
    streamed Soundfile "Voice\Female2\F2_bas_taken.aif";
    streamed Soundfile "Voice\Female2\F2_def_base.aif";
    streamed Soundfile "Voice\Female2\F2_def_carrier.aif";
    streamed Soundfile "Voice\Female2\F2_def_entrances.aif";
    streamed Soundfile "Voice\Female2\F2_def_flag.aif";
    streamed Soundfile "Voice\Female2\F2_def_generator.aif";
    streamed Soundfile "Voice\Female2\F2_def_me.aif";
    streamed Soundfile "Voice\Female2\F2_def_nexus.aif";
    streamed Soundfile "Voice\Female2\F2_def_objective.aif";
    streamed Soundfile "Voice\Female2\F2_def_reinforce.aif";
    streamed Soundfile "Voice\Female2\F2_def_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_def_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_def_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_ene_base.aif";
    streamed Soundfile "Voice\Female2\F2_ene_disarray.aif";
    streamed Soundfile "Voice\Female2\F2_ene_generator.aif";
    streamed Soundfile "Voice\Female2\F2_ene_remotes.aif";
    streamed Soundfile "Voice\Female2\F2_ene_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_ene_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_ene_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_flg_flag.aif";
    streamed Soundfile "Voice\Female2\F2_flg_give.aif";
    streamed Soundfile "Voice\Female2\F2_flg_huntergive.aif";
    streamed Soundfile "Voice\Female2\F2_flg_huntertake.aif";
    streamed Soundfile "Voice\Female2\F2_flg_retrieve.aif";
    streamed Soundfile "Voice\Female2\F2_flg_secure.aif";
    streamed Soundfile "Voice\Female2\F2_flg_take.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_anytime.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_awesome.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_bye.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_dunno.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_goodgame.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_hi.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_no.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_oops.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_quiet.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_rock.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_shazbot.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_shooting.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt1.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt2.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt3.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_taunt4.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_thanks.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_wait.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_woohoo.aif";
    streamed Soundfile "Voice\Female2\F2_gbl_yes.aif";
    streamed Soundfile "Voice\Female2\F2_need_bombardier.aif";
    streamed Soundfile "Voice\Female2\F2_need_cover.aif";
    streamed Soundfile "Voice\Female2\F2_need_driver.aif";
    streamed Soundfile "Voice\Female2\F2_need_escort.aif";
    streamed Soundfile "Voice\Female2\F2_need_gunship.aif";
    streamed Soundfile "Voice\Female2\F2_need_hold.aif";
    streamed Soundfile "Voice\Female2\F2_need_pilot.aif";
    streamed Soundfile "Voice\Female2\F2_need_ride.aif";
    streamed Soundfile "Voice\Female2\F2_need_support.aif";
    streamed Soundfile "Voice\Female2\F2_need_tailgunner.aif";
    streamed Soundfile "Voice\Female2\F2_need_where.aif";
    streamed Soundfile "Voice\Female2\F2_rep_base.aif";
    streamed Soundfile "Voice\Female2\F2_rep_generator.aif";
    streamed Soundfile "Voice\Female2\F2_rep_me.aif";
    streamed Soundfile "Voice\Female2\F2_rep_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_rep_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_rep_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_attack.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_base.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_flag.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_generator.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_base.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_defend.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_flag.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_generator.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_nexus.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_base.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_generator.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Female2\F2_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_acquired.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_base.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_destroyed.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_flag.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_need.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_sensors.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_target.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_turret.aif";
    streamed Soundfile "Voice\Female2\F2_tgt_wait.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_anytime.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_base.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_dunno.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_help.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_move.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_no.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_sorry.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_thanks.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_wait.aif";
    streamed Soundfile "Voice\Female2\F2_vqk_yes.aif";
    streamed Soundfile "Voice\Female2\F2_wrn_bomber.aif";
    streamed Soundfile "Voice\Female2\F2_wrn_enemy.aif";
    streamed Soundfile "Voice\Female2\F2_wrn_vehicles.aif";
    streamed Soundfile "Voice\Female2\F2_wrn_watchit.aif";
    Soundfile "Voice\Female2\F2_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Female2\Null.aif";
    streamed Soundfile "Voice\Female2\F2_avo_grunt.aif";
    Soundfile "Voice\Female2\F2_avo_pain.aif";
}


//                  Female 3 - 

Complex "SFX_VOICE_FEMALE_3"
{
    Volume  0.90;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Female3\F3_att_attack.aif";
    streamed Soundfile "Voice\Female3\F3_att_base.aif";
    streamed Soundfile "Voice\Female3\F3_att_chase.aif";
    streamed Soundfile "Voice\Female3\F3_att_distract.aif";
    streamed Soundfile "Voice\Female3\F3_att_flag.aif";
    streamed Soundfile "Voice\Female3\F3_att_generator.aif";
    streamed Soundfile "Voice\Female3\F3_att_objective.aif";
    streamed Soundfile "Voice\Female3\F3_att_reinforcements.aif";
    streamed Soundfile "Voice\Female3\F3_att_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_att_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_att_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_att_wait.aif";
    streamed Soundfile "Voice\Female3\F3_bas_clear.aif";
    streamed Soundfile "Voice\Female3\F3_bas_enemy.aif";
    streamed Soundfile "Voice\Female3\F3_bas_retake.aif";
    streamed Soundfile "Voice\Female3\F3_bas_secure.aif";
    streamed Soundfile "Voice\Female3\F3_bas_taken.aif";
    streamed Soundfile "Voice\Female3\F3_def_base.aif";
    streamed Soundfile "Voice\Female3\F3_def_carrier.aif";
    streamed Soundfile "Voice\Female3\F3_def_entrances.aif";
    streamed Soundfile "Voice\Female3\F3_def_flag.aif";
    streamed Soundfile "Voice\Female3\F3_def_generator.aif";
    streamed Soundfile "Voice\Female3\F3_def_me.aif";
    streamed Soundfile "Voice\Female3\F3_def_nexus.aif";
    streamed Soundfile "Voice\Female3\F3_def_objective.aif";
    streamed Soundfile "Voice\Female3\F3_def_reinforce.aif";
    streamed Soundfile "Voice\Female3\F3_def_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_def_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_def_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_ene_base.aif";
    streamed Soundfile "Voice\Female3\F3_ene_disarray.aif";
    streamed Soundfile "Voice\Female3\F3_ene_generator.aif";
    streamed Soundfile "Voice\Female3\F3_ene_remotes.aif";
    streamed Soundfile "Voice\Female3\F3_ene_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_ene_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_ene_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_flg_flag.aif";
    streamed Soundfile "Voice\Female3\F3_flg_give.aif";
    streamed Soundfile "Voice\Female3\F3_flg_huntergive.aif";
    streamed Soundfile "Voice\Female3\F3_flg_huntertake.aif";
    streamed Soundfile "Voice\Female3\F3_flg_retrieve.aif";
    streamed Soundfile "Voice\Female3\F3_flg_secure.aif";
    streamed Soundfile "Voice\Female3\F3_flg_take.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_anytime.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_awesome.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_bye.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_dunno.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_goodgame.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_hi.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_no.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_oops.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_quiet.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_rock.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_shazbot.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_shooting.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt1.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt2.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt3.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_taunt4.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_thanks.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_wait.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_woohoo.aif";
    streamed Soundfile "Voice\Female3\F3_gbl_yes.aif";
    streamed Soundfile "Voice\Female3\F3_need_bombardier.aif";
    streamed Soundfile "Voice\Female3\F3_need_cover.aif";
    streamed Soundfile "Voice\Female3\F3_need_driver.aif";
    streamed Soundfile "Voice\Female3\F3_need_escort.aif";
    streamed Soundfile "Voice\Female3\F3_need_gunship.aif";
    streamed Soundfile "Voice\Female3\F3_need_hold.aif";
    streamed Soundfile "Voice\Female3\F3_need_pilot.aif";
    streamed Soundfile "Voice\Female3\F3_need_ride.aif";
    streamed Soundfile "Voice\Female3\F3_need_support.aif";
    streamed Soundfile "Voice\Female3\F3_need_tailgunner.aif";
    streamed Soundfile "Voice\Female3\F3_need_where.aif";
    streamed Soundfile "Voice\Female3\F3_rep_base.aif";
    streamed Soundfile "Voice\Female3\F3_rep_generator.aif";
    streamed Soundfile "Voice\Female3\F3_rep_me.aif";
    streamed Soundfile "Voice\Female3\F3_rep_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_rep_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_rep_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_attack.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_base.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_flag.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_generator.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_base.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_defend.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_flag.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_generator.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_nexus.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_base.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_generator.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Female3\F3_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_acquired.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_base.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_destroyed.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_flag.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_need.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_sensors.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_target.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_turret.aif";
    streamed Soundfile "Voice\Female3\F3_tgt_wait.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_anytime.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_base.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_dunno.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_help.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_move.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_no.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_sorry.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_thanks.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_wait.aif";
    streamed Soundfile "Voice\Female3\F3_vqk_yes.aif";
    streamed Soundfile "Voice\Female3\F3_wrn_bomber.aif";
    streamed Soundfile "Voice\Female3\F3_wrn_enemy.aif";
    streamed Soundfile "Voice\Female3\F3_wrn_vehicles.aif";
    streamed Soundfile "Voice\Female3\F3_wrn_watchit.aif";
    Soundfile "Voice\Female3\F3_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Female3\Null.aif";
    streamed Soundfile "Voice\Female3\F3_avo_grunt.aif";
    Soundfile "Voice\Female3\F3_avo_pain.aif";
}


//                  Female 4 - 

Complex "SFX_VOICE_FEMALE_4"
{
    Volume  0.90;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Female4\F4_att_attack.aif";
    streamed Soundfile "Voice\Female4\F4_att_base.aif";
    streamed Soundfile "Voice\Female4\F4_att_chase.aif";
    streamed Soundfile "Voice\Female4\F4_att_distract.aif";
    streamed Soundfile "Voice\Female4\F4_att_flag.aif";
    streamed Soundfile "Voice\Female4\F4_att_generator.aif";
    streamed Soundfile "Voice\Female4\F4_att_objective.aif";
    streamed Soundfile "Voice\Female4\F4_att_reinforcements.aif";
    streamed Soundfile "Voice\Female4\F4_att_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_att_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_att_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_att_wait.aif";
    streamed Soundfile "Voice\Female4\F4_bas_clear.aif";
    streamed Soundfile "Voice\Female4\F4_bas_enemy.aif";
    streamed Soundfile "Voice\Female4\F4_bas_retake.aif";
    streamed Soundfile "Voice\Female4\F4_bas_secure.aif";
    streamed Soundfile "Voice\Female4\F4_bas_taken.aif";
    streamed Soundfile "Voice\Female4\F4_def_base.aif";
    streamed Soundfile "Voice\Female4\F4_def_carrier.aif";
    streamed Soundfile "Voice\Female4\F4_def_entrances.aif";
    streamed Soundfile "Voice\Female4\F4_def_flag.aif";
    streamed Soundfile "Voice\Female4\F4_def_generator.aif";
    streamed Soundfile "Voice\Female4\F4_def_me.aif";
    streamed Soundfile "Voice\Female4\F4_def_nexus.aif";
    streamed Soundfile "Voice\Female4\F4_def_objective.aif";
    streamed Soundfile "Voice\Female4\F4_def_reinforce.aif";
    streamed Soundfile "Voice\Female4\F4_def_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_def_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_def_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_ene_base.aif";
    streamed Soundfile "Voice\Female4\F4_ene_disarray.aif";
    streamed Soundfile "Voice\Female4\F4_ene_generator.aif";
    streamed Soundfile "Voice\Female4\F4_ene_remotes.aif";
    streamed Soundfile "Voice\Female4\F4_ene_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_ene_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_ene_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_flg_flag.aif";
    streamed Soundfile "Voice\Female4\F4_flg_give.aif";
    streamed Soundfile "Voice\Female4\F4_flg_huntergive.aif";
    streamed Soundfile "Voice\Female4\F4_flg_huntertake.aif";
    streamed Soundfile "Voice\Female4\F4_flg_retrieve.aif";
    streamed Soundfile "Voice\Female4\F4_flg_secure.aif";
    streamed Soundfile "Voice\Female4\F4_flg_take.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_anytime.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_awesome.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_bye.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_dunno.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_goodgame.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_hi.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_no.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_oops.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_quiet.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_rock.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_shazbot.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_shooting.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt1.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt2.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt3.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_taunt4.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_thanks.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_wait.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_woohoo.aif";
    streamed Soundfile "Voice\Female4\F4_gbl_yes.aif";
    streamed Soundfile "Voice\Female4\F4_need_bombardier.aif";
    streamed Soundfile "Voice\Female4\F4_need_cover.aif";
    streamed Soundfile "Voice\Female4\F4_need_driver.aif";
    streamed Soundfile "Voice\Female4\F4_need_escort.aif";
    streamed Soundfile "Voice\Female4\F4_need_gunship.aif";
    streamed Soundfile "Voice\Female4\F4_need_hold.aif";
    streamed Soundfile "Voice\Female4\F4_need_pilot.aif";
    streamed Soundfile "Voice\Female4\F4_need_ride.aif";
    streamed Soundfile "Voice\Female4\F4_need_support.aif";
    streamed Soundfile "Voice\Female4\F4_need_tailgunner.aif";
    streamed Soundfile "Voice\Female4\F4_need_where.aif";
    streamed Soundfile "Voice\Female4\F4_rep_base.aif";
    streamed Soundfile "Voice\Female4\F4_rep_generator.aif";
    streamed Soundfile "Voice\Female4\F4_rep_me.aif";
    streamed Soundfile "Voice\Female4\F4_rep_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_rep_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_rep_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_attack.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_base.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_flag.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_generator.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_base.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_defend.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_flag.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_generator.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_nexus.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_base.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_generator.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Female4\F4_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_acquired.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_base.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_destroyed.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_flag.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_need.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_sensors.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_target.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_turret.aif";
    streamed Soundfile "Voice\Female4\F4_tgt_wait.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_anytime.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_base.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_dunno.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_help.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_move.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_no.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_sorry.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_thanks.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_wait.aif";
    streamed Soundfile "Voice\Female4\F4_vqk_yes.aif";
    streamed Soundfile "Voice\Female4\F4_wrn_bomber.aif";
    streamed Soundfile "Voice\Female4\F4_wrn_enemy.aif";
    streamed Soundfile "Voice\Female4\F4_wrn_vehicles.aif";
    streamed Soundfile "Voice\Female4\F4_wrn_watchit.aif";
    Soundfile "Voice\Female4\F4_avo_deathcry_02.aif";
    streamed Soundfile "Voice\Female4\Null.aif";
    streamed Soundfile "Voice\Female4\F4_avo_grunt.aif";
    Soundfile "Voice\Female4\F4_avo_pain.aif";
}


//                  Female 5 - Amazon

Complex "SFX_VOICE_FEMALE_5"
{
    Volume  0.90;
    Pitch   1.0;
    Priority    70;

    streamed Soundfile "Voice\Female5\F5_att_attack.aif";
    streamed Soundfile "Voice\Female5\F5_att_base.aif";
    streamed Soundfile "Voice\Female5\F5_att_chase.aif";
    streamed Soundfile "Voice\Female5\F5_att_distract.aif";
    streamed Soundfile "Voice\Female5\F5_att_flag.aif";
    streamed Soundfile "Voice\Female5\F5_att_generator.aif";
    streamed Soundfile "Voice\Female5\F5_att_objective.aif";
    streamed Soundfile "Voice\Female5\F5_att_reinforcements.aif";
    streamed Soundfile "Voice\Female5\F5_att_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_att_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_att_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_att_wait.aif";
    streamed Soundfile "Voice\Female5\F5_bas_clear.aif";
    streamed Soundfile "Voice\Female5\F5_bas_enemy.aif";
    streamed Soundfile "Voice\Female5\F5_bas_retake.aif";
    streamed Soundfile "Voice\Female5\F5_bas_secure.aif";
    streamed Soundfile "Voice\Female5\F5_bas_taken.aif";
    streamed Soundfile "Voice\Female5\F5_def_base.aif";
    streamed Soundfile "Voice\Female5\F5_def_carrier.aif";
    streamed Soundfile "Voice\Female5\F5_def_entrances.aif";
    streamed Soundfile "Voice\Female5\F5_def_flag.aif";
    streamed Soundfile "Voice\Female5\F5_def_generator.aif";
    streamed Soundfile "Voice\Female5\F5_def_me.aif";
    streamed Soundfile "Voice\Female5\F5_def_nexus.aif";
    streamed Soundfile "Voice\Female5\F5_def_objective.aif";
    streamed Soundfile "Voice\Female5\F5_def_reinforce.aif";
    streamed Soundfile "Voice\Female5\F5_def_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_def_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_def_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_ene_base.aif";
    streamed Soundfile "Voice\Female5\F5_ene_disarray.aif";
    streamed Soundfile "Voice\Female5\F5_ene_generator.aif";
    streamed Soundfile "Voice\Female5\F5_ene_remotes.aif";
    streamed Soundfile "Voice\Female5\F5_ene_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_ene_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_ene_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_flg_flag.aif";
    streamed Soundfile "Voice\Female5\F5_flg_give.aif";
    streamed Soundfile "Voice\Female5\F5_flg_huntergive.aif";
    streamed Soundfile "Voice\Female5\F5_flg_huntertake.aif";
    streamed Soundfile "Voice\Female5\F5_flg_retrieve.aif";
    streamed Soundfile "Voice\Female5\F5_flg_secure.aif";
    streamed Soundfile "Voice\Female5\F5_flg_take.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_anytime.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_awesome.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_bye.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_dunno.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_goodgame.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_hi.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_no.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_oops.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_quiet.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_rock.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_shazbot.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_shooting.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt1.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt2.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt3.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_taunt4.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_thanks.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_wait.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_woohoo.aif";
    streamed Soundfile "Voice\Female5\F5_gbl_yes.aif";
    streamed Soundfile "Voice\Female5\F5_need_bombardier.aif";
    streamed Soundfile "Voice\Female5\F5_need_cover.aif";
    streamed Soundfile "Voice\Female5\F5_need_driver.aif";
    streamed Soundfile "Voice\Female5\F5_need_escort.aif";
    streamed Soundfile "Voice\Female5\F5_need_gunship.aif";
    streamed Soundfile "Voice\Female5\F5_need_hold.aif";
    streamed Soundfile "Voice\Female5\F5_need_pilot.aif";
    streamed Soundfile "Voice\Female5\F5_need_ride.aif";
    streamed Soundfile "Voice\Female5\F5_need_support.aif";
    streamed Soundfile "Voice\Female5\F5_need_tailgunner.aif";
    streamed Soundfile "Voice\Female5\F5_need_where.aif";
    streamed Soundfile "Voice\Female5\F5_rep_base.aif";
    streamed Soundfile "Voice\Female5\F5_rep_generator.aif";
    streamed Soundfile "Voice\Female5\F5_rep_me.aif";
    streamed Soundfile "Voice\Female5\F5_rep_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_rep_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_rep_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_attack.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_base.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_flag.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_generator.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_slf_att_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_base.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_defend.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_flag.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_generator.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_nexus.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_slf_def_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_base.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_equipment.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_generator.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_repairing.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_slf_rep_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_cover.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_defense.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_onit.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_remotes.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_turrets.aif";
    streamed Soundfile "Voice\Female5\F5_slf_tsk_vehicle.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_acquired.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_base.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_destroyed.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_flag.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_need.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_sensors.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_target.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_turret.aif";
    streamed Soundfile "Voice\Female5\F5_tgt_wait.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_anytime.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_base.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_ceasefire.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_dunno.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_help.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_move.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_no.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_sorry.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_thanks.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_wait.aif";
    streamed Soundfile "Voice\Female5\F5_vqk_yes.aif";
    streamed Soundfile "Voice\Female5\F5_wrn_bomber.aif";
    streamed Soundfile "Voice\Female5\F5_wrn_enemy.aif";
    streamed Soundfile "Voice\Female5\F5_wrn_vehicles.aif";
    streamed Soundfile "Voice\Female5\F5_wrn_watchit.aif";
    Soundfile "Voice\Female5\F5_avo_deathcry_01.aif";
    streamed Soundfile "Voice\Female5\Null.aif";
    streamed Soundfile "Voice\Female5\F5_avo_grunt.aif";
    Soundfile "Voice\Female5\F5_avo_pain.aif";
}





