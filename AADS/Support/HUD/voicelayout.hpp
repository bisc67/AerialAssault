//==============================================================================
//
//  VoiceLayout.hpp
//
//==============================================================================

#include "hud.hpp"

#define DEF_STATE(_state_,_title_)          hud::voice_table    hud_vc_##_state_ = { _title_, {
#define END_STATE           } };

#define DEF_VOICE(_label_,_text_,_id_)      {_label_,_text_,_id_},
#define DEF_TRANS(_label_,_state_)          {_label_, (const char*)&hud_vc_##_state_, 0},
#define DEF_NULL                            {(const char*)NULL, (const char*)NULL, 0},

//==============================================================================

DEF_STATE( Animations, "Animations" )
    DEF_VOICE( "Move",                  "Move! Move! Move!",                           hud::HUD_VC_GLOBAL | 124 )
    DEF_VOICE( "I am the\ngreatest",    "I got your number! I got all your numbers!",  hud::HUD_VC_GLOBAL | 58  )
    DEF_VOICE( "Woohoo",                "Woohoo!",                                     hud::HUD_VC_GLOBAL | 62  )
    DEF_VOICE( "I'm on it",             "I'm on it!",                                  hud::HUD_VC_GLOBAL | 105 )
    DEF_VOICE( "Hi",                    "Hi.",                                         hud::HUD_VC_GLOBAL | 49  )
    DEF_VOICE( "Take that",             "HAHAHAHA!",                                   hud::HUD_VC_GLOBAL | 57  )
    DEF_VOICE( "Awww...\ntoo bad",      "Awww...too bad.",                             hud::HUD_VC_GLOBAL | 56  )
    DEF_VOICE( "Awesome",               "I love it!",                                  hud::HUD_VC_GLOBAL | 45  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Compliment, "Compliment" )
    DEF_VOICE( "Awesome!",              "I love it!",                                  hud::HUD_VC_GLOBAL | 45  )
    DEF_VOICE( "Good\ngame!",           "Good game!",                                  hud::HUD_VC_GLOBAL | 48  )
    DEF_VOICE( "You rock!",             "You rock!",                                   hud::HUD_VC_GLOBAL | 53  )
    DEF_VOICE( "Great\nshot!",          "Great shot!",                                 hud::HUD_VC_GLOBAL | 55  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Respond, "Respond" )
    DEF_VOICE( "Anytime",               "Anytime.",                                    hud::HUD_VC_GLOBAL | 44  )
    DEF_VOICE( "Don't know",            "Don't know.",                                 hud::HUD_VC_GLOBAL | 47  )
    DEF_VOICE( "Thanks",                "Thanks.",                                     hud::HUD_VC_GLOBAL | 60  )
    DEF_VOICE( "Wait",                  "Wait up.",                                    hud::HUD_VC_GLOBAL | 61  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Taunt, "Taunt" )
    DEF_VOICE( "Awww...\ntoo bad",      "Awww...too bad.",                             hud::HUD_VC_GLOBAL | 56  )
    DEF_VOICE( "Obnoxious",             "HAHAHAHA!",                                   hud::HUD_VC_GLOBAL | 57  )
    DEF_VOICE( "Brag",                  "I got your number! I got all your numbers!",  hud::HUD_VC_GLOBAL | 58  )
    DEF_VOICE( "When",                  "Eat my plasma!",                              hud::HUD_VC_GLOBAL | 59  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Attack, "Attack" )
    DEF_VOICE( "Attack!",               "Attack!",                                     1   )
    DEF_VOICE( "Attack\nenemy base",    "Attack the enemy base!",                      2   )
    DEF_VOICE( "Recover\nour flag",     "Recover our flag!",                           3   )
    DEF_VOICE( "Disrupt\nenemy",        "Disrupt the enemy defense!",                  4   )
    DEF_VOICE( "Get the\nenemy flag",   "Get the enemy flag!",                         5   )
    DEF_VOICE( "Destroy\ngenerator",    "Destroy the enemy generator!",                6   )
    DEF_VOICE( "Destroy\nobjective",    "Destroy the objective!",                      7   )
    DEF_VOICE( "Reinforce\noffense",    "Reinforce the offense!",                      8   )
    DEF_VOICE( "Destroy\nsensors",      "Destroy enemy sensors!",                      9   )
    DEF_VOICE( "Wait for\nmy signal",   "Wait for my signal before attacking.",        12  )
    DEF_VOICE( "Destroy\nturrets",      "Destroy enemy turrets!",                      10  )
    DEF_VOICE( "Destroy\nvehicle",      "Destroy enemy vehicle!",                      11  )
END_STATE

DEF_STATE( Defend, "Defend" )
    DEF_VOICE( "Defend\nbase",          "Defend our base!",                            18  )
    DEF_VOICE( "Cover flag\ncarrier",   "Cover our flag carrier!",                     19  )
    DEF_VOICE( "Defend\nentrances",     "Defend our entrances!",                       20  )
    DEF_VOICE( "Defend\nflag",          "Defend our flag!",                            21  )
    DEF_VOICE( "Defend\ngenerator",     "Defend our generator!",                       22  )
    DEF_VOICE( "Cover me!",             "Cover me!",                                   23  )
    DEF_VOICE( "Defend\nthe nexus",     "Defend the nexus!",                           24  )
    DEF_VOICE( "Defend\nobjective",     "Defend the objective!",                       25  )
    DEF_VOICE( "Reinforce\ndefense",    "Reinforce our defense!",                      26  )
    DEF_VOICE( "Defend\nsensors",       "Defend our sensors!",                         27  )
    DEF_VOICE( "Defend\nturrets",       "Defend our turrets!",                         28  )
    DEF_VOICE( "Defend\nvehicle",       "Defend our vehicle!",                         29  )
END_STATE


DEF_STATE( Repair, "Repair" )
    DEF_VOICE( "Repair\nour base",      "Repair our base!",                            75  )
    DEF_VOICE( "Repair\ngenerator",     "Repair our generator!",                       76  )
    DEF_VOICE( "Repair me",             "Repair me!",                                  77  )
    DEF_VOICE( "Repair\nsensors",       "Repair our sensors!",                         78  )
    DEF_VOICE( "Repair\nturrets",       "Repair our turrets!",                         79  )
    DEF_VOICE( "Repair\nvehicle bay",   "Repair our vehicle station!",                 80  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Base, "Base" )
    DEF_VOICE( "Our base\nis clear",    "Our base is clear.",                          13  )
    DEF_VOICE( "Enemy in\nour base",    "Enemy is in our base!",                       14  )
    DEF_VOICE( "Retake\nour base",      "Retake our base!",                            15  )
    DEF_VOICE( "Our base\nis secure",   "Our base is secure.",                         16  )
    DEF_VOICE( "Our base\nis taken",    "Our base is taken!",                          17  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Enemy, "Enemy" )
    DEF_VOICE( "Base\ndisabled",         "Enemy base disabled.",                        30  )
    DEF_VOICE( "Enemy in\ndisarray",     "Enemy is disrupted. Attack!",                 31  )
    DEF_VOICE( "Generator\ndestroyed",   "Enemy generator destroyed.",                  32  )
    DEF_VOICE( "Remotes\ndestroyed",     "Enemy remote equipment destroyed.",           33  )
    DEF_VOICE( "Sensors\ndestroyed",     "Enemy sensors destroyed.",                    34  )
    DEF_VOICE( "Turrets\ndestroyed",     "Enemy turrets destroyed.",                    35  )
    DEF_VOICE( "Vehicle bay\ndestroyed", "Enemy vehicle station destroyed.",            36  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Flag, "Flag" )
    DEF_VOICE( "Defend\nour flag",       "Defend our flag.",                            21  )
    DEF_VOICE( "I have\nenemy flag",     "I have the enemy flag.",                      37  )
    DEF_VOICE( "Give me\nthe flag",      "Give me the flag.",                           38  )
    DEF_VOICE( "Retrieve\nour flag",     "Retrieve our flag.",                          41  )
    DEF_VOICE( "Our flag\nis secure",    "Our flag is secure.",                         42  )
    DEF_VOICE( "Take the\nflag",         "Take the flag from me.",                      43  )
    DEF_VOICE( "Give me\nyour flags",    "Give your flags to me.",                      39  )
    DEF_VOICE( "Take my\nflags",         "Take my flags.",                              40  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Need, "Need" )
    DEF_VOICE( "Need a\nbombardier",     "Need a bombardier.",                          64  )
    DEF_VOICE( "Need cover",             "Need covering fire.",                         65  )
    DEF_VOICE( "Need a\ndriver",         "Need a driver for ground vehicle.",           66  )
    DEF_VOICE( "Need an\nescort",        "Vehicle needs escort.",                       67  )
    DEF_VOICE( "Gunship\nready",         "Gunship ready, need a ride?",                 68  )
    DEF_VOICE( "Hold\nvehicle",          "Hold that vehicle, I'm coming!",              69  )
    DEF_VOICE( "Need a\npilot",          "Need a pilot for turbo grav!",                70  )
    DEF_VOICE( "I need\na ride",         "I need a ride!",                              71  )
    DEF_VOICE( "Where to?",              "Where to?",                                   74  )
    DEF_VOICE( "Need vehicle\nsupport",  "Need vehicle support.",                       72  )
    DEF_VOICE( "Need a\ntailgunner",     "Need a tailgunner!",                          73  )
    DEF_NULL
END_STATE

DEF_STATE( Attack_Self, "Self - Attack" )
    DEF_VOICE( "I'll attack",              "I'll attack!",                                81  )
    DEF_VOICE( "I'll attack\nenemy base",  "I'll attack the enemy base!",                 82  )
    DEF_VOICE( "I'll attack\nenemy flag",  "I'll go for the enemy flag!",                 83  )
    DEF_VOICE( "I'll attack\ngenerator",   "I'll attack the enemy generator!",            84  )
    DEF_VOICE( "I'll attack\nsensors",     "I'll attack the enemy sensors!",              85  )
    DEF_VOICE( "I'll attack\nturrets",     "I'll attack the enemy turrets!",              86  )
    DEF_VOICE( "I'll attack\nvehicle bay", "I'll attack the enemy vehicle station!",      87  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Defend_Self, "Self - Defend" )
    DEF_VOICE( "I'll defend\nbase",        "I'll defend our base.",                       88  )
    DEF_VOICE( "I'm\ndefending",           "I'm defending.",                              89  )
    DEF_VOICE( "I'll defend\nflag",        "I'll defend our flag.",                       90  )
    DEF_VOICE( "I'll defend\ngenerator",   "I'll defend our generator.",                  91  )
    DEF_VOICE( "I'll defend\nnexus",       "I'll defend the nexus.",                      92  )
    DEF_VOICE( "I'll defend\nsensors",     "I'll defend our sensors.",                    93  )
    DEF_VOICE( "I'll defend\nturrets",     "I'll defend our turrets.",                    94  )
    DEF_VOICE( "I'll defend\nvehicle bay", "I'll defend our vehicle station.",            95  )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Repair_Self, "Self - Repair" )
    DEF_VOICE( "I'll repair\nbase",           "I'll repair our base.",                       96  )
    DEF_VOICE( "I'll repair\nequipment",      "I'll repair our equipment.",                  97  )
    DEF_VOICE( "I'll repair\ngenerator",      "I'll repair our generators.",                 98  )
    DEF_VOICE( "I'm on\nrepairs",             "I'm on repairs.",                             99  )
    DEF_VOICE( "I'll repair\nsensors",        "I'll repair our sensors.",                    100 )
    DEF_VOICE( "I'll repair\nturrets",        "I'll repair our turrets.",                    101 )
    DEF_VOICE( "I'll repair\nvehicle bay",    "I'll repair our vehicle station.",            102 )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Task_Self, "Self - Task" )
    DEF_VOICE( "I'll\ncover you",       "I'll cover you.",                             103 )
    DEF_VOICE( "Setup\ndefenses",       "I'll deploy sensors.",                        104 )
    DEF_VOICE( "I'm\nOn it",            "I'm on it.",                                  105 )
    DEF_VOICE( "I'll deploy\nremotes",  "I'll deploy remote equipment.",               106 )
    DEF_VOICE( "I'll deploy\nsensors",  "I'll deploy sensors.",                        107 )
    DEF_VOICE( "I'll deploy\nturrets",  "I'll deploy turrets.",                        108 )
    DEF_VOICE( "I'll get\na vehicle",   "I'll get a vehicle ready.",                   109 )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Target, "Target" )
    DEF_VOICE( "Target\nacquired",      "Target acquired.",                            110 )
    DEF_VOICE( "Target\nenemy base",    "Target the enemy base, I'm in position.",     111 )
    DEF_VOICE( "Target\ndestroyed",     "Target destroyed.",                           112 )
    DEF_VOICE( "Target\nenemy flag",    "Target their flag, I'm in position.",         113 )
    DEF_VOICE( "Fire on\nmy target",    "Fire on my target.",                          116 )
    DEF_VOICE( "Need a\ntarget",        "Need a target painted.",                      114 )
    DEF_VOICE( "Target\nsensors",       "Target their sensors, I'm in position.",      115 )
    DEF_VOICE( "Target\nturret",        "Target their turrets, I'm in position.",      117 )
    DEF_VOICE( "Wait",                  "Wait, I'll be in range soon.",                118 )
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Warning, "Warning" )
    DEF_VOICE( "Incoming\nbomber",      "Incoming bomber!",                            130 )
    DEF_VOICE( "Incoming\nenemies",     "Incoming hostiles!",                          131 )
    DEF_VOICE( "Incoming\nvehicles",    "Incoming vehicles!",                          132 )
    DEF_VOICE( "Watch\nyour fire",      "Watch where your shooting!",                  133 )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Quick, "Quick" )
    DEF_VOICE( "Yes",                   "Yes.",                                        129 )
    DEF_VOICE( "No",                    "No.",                                         125 )
    DEF_VOICE( "Anytime",               "Anytime.",                                    119 )
    DEF_VOICE( "Base\nsecure?",         "Is our base secure?",                         120 )
    DEF_VOICE( "Cease\nfire",           "Cease fire!",                                 121 )
    DEF_VOICE( "Don't\nknow",           "No clue.",                                    122 )
    DEF_VOICE( "HELP!",                 "Help!",                                       123 )
    DEF_VOICE( "Move",                  "Move! Move! Move!",                           124 )
    DEF_VOICE( "Sorry",                 "Sorry.",                                      126 )
    DEF_VOICE( "Thanks",                "Thanks.",                                     127 )
    DEF_VOICE( "Wait",                  "Wait up.",                                    128 )
    DEF_NULL
END_STATE

DEF_STATE( Global, "Global" )
    DEF_VOICE( "Yes",                   "Yes.",                                        63 )
    DEF_VOICE( "Hi",                    "Hi.",                                         49 )
    DEF_VOICE( "No",                    "No.",                                         50 )
    DEF_VOICE( "Bye",                   "Bye.",                                        46 )
    DEF_VOICE( "Oops",                  "Oops.",                                       51 )
    DEF_VOICE( "Quiet!",                "Quiet!",                                      52 )
    DEF_VOICE( "Shazbot!",              "Shazbot!",                                    54 )
    DEF_VOICE( "Woohoo!",               "Woohoo!",                                     62 )
    DEF_TRANS( "Respond",           Respond    )
    DEF_TRANS( "Animations",        Animations )
    DEF_TRANS( "Taunt",             Taunt      )
    DEF_TRANS( "Compliment",        Compliment )
END_STATE

DEF_STATE( Self, "Self" )
    DEF_TRANS( "Attack",            Attack_Self )
    DEF_TRANS( "Defend",            Defend_Self )
    DEF_TRANS( "Repair",            Repair_Self )
    DEF_TRANS( "Task",              Task_Self   )
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
    DEF_NULL
END_STATE

DEF_STATE( Root, "" )
    DEF_TRANS( "Global",            Global  )
    DEF_TRANS( "Attack",            Attack  )
    DEF_TRANS( "Defend",            Defend  )
    DEF_TRANS( "Repair",            Repair  )
    DEF_TRANS( "Base",              Base    )
    DEF_TRANS( "Enemy",             Enemy   )
    DEF_TRANS( "Flag",              Flag    )
    DEF_TRANS( "Need",              Need    )
    DEF_TRANS( "Quick",             Quick   )
    DEF_TRANS( "Self",              Self    )
    DEF_TRANS( "Target",            Target  )
    DEF_TRANS( "Warning",           Warning )
END_STATE

//==============================================================================
