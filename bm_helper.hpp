#ifndef __BM_HELPER_HPP__
#define __BM_HELPER_HPP__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <format>
#include <map>
#include <string>
#include <vector>

#include "Windows.h"

#include "bakkesmod/utilities/loadoututilities.h"
#include "bakkesmod/wrappers/arraywrapper.h"
#include "bakkesmod/wrappers/engine/actorwrapper.h"
#include "bakkesmod/wrappers/matchmakingwrapper.h"

// this was originally in the bakkesmodsdk under wrappers/matchmakingwrapper
// but it hasn't been updated in a while.

// this would be the updated list.
enum class PlaylistId {
      Unknown                 = -1337,
      Casual                  = 0,
      Duel                    = 1,
      Doubles                 = 2,
      Standard                = 3,
      Chaos                   = 4,
      PrivateMatch            = 6,
      Season                  = 7,
      OfflineSplitscreen      = 8,
      Training                = 9,
      RankedSoloDuel          = 10,
      RankedTeamDoubles       = 11,
      RankedStandard          = 13,
      MutatorMashup           = 14,
      SnowDayPromotion        = 15,
      Experimental            = 16,
      BasketballDoubles       = 17,
      Rumble                  = 18,
      Workshop                = 19,
      UGCTrainingEditor       = 20,
      UGCTraining             = 21,
      Tournament              = 22,
      Breakout                = 23,
      LANMatch                = 24,
      TenthAnniversary        = 25,  // not in codered log?
      FaceIt                  = 26,
      RankedBasketballDoubles = 27,
      RankedRumble            = 28,
      RankedBreakout          = 29,
      RankedSnowDay           = 30,
      HauntedBall             = 31,
      BeachBall               = 32,
      Rugby                   = 33,
      AutoTournament          = 34,
      RocketLabs              = 35,
      RumShot                 = 37,
      GodBall                 = 38,
      CoopVsAI                = 40,
      BoomerBall              = 41,
      GodBallDoubles          = 43,
      SpecialSnowDay          = 44,
      Football                = 46,
      Cubic                   = 47,
      TacticalRumble          = 48,
      SpringLoaded            = 49,
      SpeedDemon              = 50,
      RumbleBM                = 52,
      Knockout                = 54,
      Thirdwheel              = 55,
      MagnusFutball           = 62,
      GodBallSpooky           = 64,
      GodBallRicochet         = 66,
      CubicSpooky             = 67,
      GForceFrenzy            = 68,
};

// fuck clang-lint/tidy "ODR" bullshit.
namespace bm_helper {
const std::map<PlaylistId, std::string> playlist_ids_str = {
  // NO MATCHING PLAYLIST
      {                PlaylistId::Unknown,                 "Unknown"},
 // NO MATCHING PLAYLIST
      {                 PlaylistId::Casual,                  "Casual"},
      {                   PlaylistId::Duel,                    "Duel"},
      {                PlaylistId::Doubles,                 "Doubles"},
      {               PlaylistId::Standard,                "Standard"},
      {                  PlaylistId::Chaos,                   "Chaos"},
      {           PlaylistId::PrivateMatch,            "PrivateMatch"},
      {                 PlaylistId::Season,                  "Season"},
      {     PlaylistId::OfflineSplitscreen,      "OfflineSplitscreen"},
      {               PlaylistId::Training,                "Training"},
      {         PlaylistId::RankedSoloDuel,          "RankedSoloDuel"},
      {      PlaylistId::RankedTeamDoubles,       "RankedTeamDoubles"},
      {         PlaylistId::RankedStandard,          "RankedStandard"},
      {          PlaylistId::MutatorMashup,           "MutatorMashup"},
      {       PlaylistId::SnowDayPromotion,        "SnowDayPromotion"},
 // NO MATCHING PLAYLIST
      {           PlaylistId::Experimental,            "Experimental"},
 // NO MATCHING PLAYLIST
      {      PlaylistId::BasketballDoubles,       "BasketballDoubles"},
 // NO MATCHING PLAYLIST
      {                 PlaylistId::Rumble,                  "Rumble"},
      {               PlaylistId::Workshop,                "Workshop"},
      {      PlaylistId::UGCTrainingEditor,       "UGCTrainingEditor"},
      {            PlaylistId::UGCTraining,             "UGCTraining"},
      {             PlaylistId::Tournament,              "Tournament"},
 // NO MATCHING PLAYLIST
      {               PlaylistId::Breakout,                "Breakout"},
 // NO MATCHING PLAYLIST
      {               PlaylistId::LANMatch,                "LANMatch"},
      {       PlaylistId::TenthAnniversary,        "TenthAnniversary"},
      {                 PlaylistId::FaceIt,                  "FaceIt"},
      {PlaylistId::RankedBasketballDoubles, "RankedBasketballDoubles"},
      {           PlaylistId::RankedRumble,            "RankedRumble"},
      {         PlaylistId::RankedBreakout,          "RankedBreakout"},
      {          PlaylistId::RankedSnowDay,           "RankedSnowDay"},
 // NO MATCHING PLAYLIST
      {            PlaylistId::HauntedBall,             "HauntedBall"},
 // NO MATCHING PLAYLIST
      {              PlaylistId::BeachBall,               "BeachBall"},
      {                  PlaylistId::Rugby,                   "Rugby"},
      {         PlaylistId::AutoTournament,          "AutoTournament"},
 // NO MATCHING PLAYLIST
      {             PlaylistId::RocketLabs,              "RocketLabs"},
 // NO MATCHING PLAYLIST
      {                PlaylistId::RumShot,                 "RumShot"},
 // NO MATCHING PLAYLIST
      {                PlaylistId::GodBall,                 "GodBall"},
 // NO MATCHING PLAYLIST
      {               PlaylistId::CoopVsAI,                "CoopVsAI"},
      {             PlaylistId::BoomerBall,              "BoomerBall"},
 // NO MATCHING PLAYLIST
      {         PlaylistId::GodBallDoubles,          "GodBallDoubles"},
 // NO MATCHING PLAYLIST
      {         PlaylistId::SpecialSnowDay,          "SpecialSnowDay"},
      {               PlaylistId::Football,                "Football"},
 // NO MATCHING PLAYLIST
      {                  PlaylistId::Cubic,                   "Cubic"},
 // NO MATCHING PLAYLIST
      {         PlaylistId::TacticalRumble,          "TacticalRumble"},
 // NO MATCHING PLAYLIST
      {           PlaylistId::SpringLoaded,            "SpringLoaded"},
      {             PlaylistId::SpeedDemon,              "SpeedDemon"},
 // NO MATCHING PLAYLIST
      {               PlaylistId::RumbleBM,                "RumbleBM"},
      {               PlaylistId::Knockout,                "Knockout"},
 // NO MATCHING PLAYLIST
      {             PlaylistId::Thirdwheel,              "Thirdwheel"},
 // NO MATCHING PLAYLIST
      {          PlaylistId::MagnusFutball,           "MagnusFutball"},
      {          PlaylistId::GodBallSpooky,           "GodBallSpooky"},
      {        PlaylistId::GodBallRicochet,         "GodBallRicochet"},
      {            PlaylistId::CubicSpooky,             "CubicSpooky"},
      {           PlaylistId::GForceFrenzy,            "GForceFrenzy"},
};

const std::map<std::string, PlaylistId> playlist_str_ids = {
      {                "Unknown",                 PlaylistId::Unknown},
      {                 "Casual",                  PlaylistId::Casual},
      {                   "Duel",                    PlaylistId::Duel},
      {                "Doubles",                 PlaylistId::Doubles},
      {               "Standard",                PlaylistId::Standard},
      {                  "Chaos",                   PlaylistId::Chaos},
      {           "PrivateMatch",            PlaylistId::PrivateMatch},
      {                 "Season",                  PlaylistId::Season},
      {     "OfflineSplitscreen",      PlaylistId::OfflineSplitscreen},
      {               "Training",                PlaylistId::Training},
      {         "RankedSoloDuel",          PlaylistId::RankedSoloDuel},
      {      "RankedTeamDoubles",       PlaylistId::RankedTeamDoubles},
      {         "RankedStandard",          PlaylistId::RankedStandard},
      {          "MutatorMashup",           PlaylistId::MutatorMashup},
      {       "SnowDayPromotion",        PlaylistId::SnowDayPromotion},
      {           "Experimental",            PlaylistId::Experimental},
      {      "BasketballDoubles",       PlaylistId::BasketballDoubles},
      {                 "Rumble",                  PlaylistId::Rumble},
      {               "Workshop",                PlaylistId::Workshop},
      {      "UGCTrainingEditor",       PlaylistId::UGCTrainingEditor},
      {            "UGCTraining",             PlaylistId::UGCTraining},
      {             "Tournament",              PlaylistId::Tournament},
      {               "Breakout",                PlaylistId::Breakout},
      {               "LANMatch",                PlaylistId::LANMatch},
      {       "TenthAnniversary",        PlaylistId::TenthAnniversary},
      {                 "FaceIt",                  PlaylistId::FaceIt},
      {"RankedBasketballDoubles", PlaylistId::RankedBasketballDoubles},
      {           "RankedRumble",            PlaylistId::RankedRumble},
      {         "RankedBreakout",          PlaylistId::RankedBreakout},
      {          "RankedSnowDay",           PlaylistId::RankedSnowDay},
      {            "HauntedBall",             PlaylistId::HauntedBall},
      {              "BeachBall",               PlaylistId::BeachBall},
      {                  "Rugby",                   PlaylistId::Rugby},
      {         "AutoTournament",          PlaylistId::AutoTournament},
      {             "RocketLabs",              PlaylistId::RocketLabs},
      {                "RumShot",                 PlaylistId::RumShot},
      {                "GodBall",                 PlaylistId::GodBall},
      {               "CoopVsAI",                PlaylistId::CoopVsAI},
      {             "BoomerBall",              PlaylistId::BoomerBall},
      {         "GodBallDoubles",          PlaylistId::GodBallDoubles},
      {         "SpecialSnowDay",          PlaylistId::SpecialSnowDay},
      {               "Football",                PlaylistId::Football},
      {                  "Cubic",                   PlaylistId::Cubic},
      {         "TacticalRumble",          PlaylistId::TacticalRumble},
      {           "SpringLoaded",            PlaylistId::SpringLoaded},
      {             "SpeedDemon",              PlaylistId::SpeedDemon},
      {               "RumbleBM",                PlaylistId::RumbleBM},
      {               "Knockout",                PlaylistId::Knockout},
      {             "Thirdwheel",              PlaylistId::Thirdwheel},
      {          "MagnusFutball",           PlaylistId::MagnusFutball},
      {          "GodBallSpooky",           PlaylistId::GodBallSpooky},
      {        "GodBallRicochet",         PlaylistId::GodBallRicochet},
      {            "CubicSpooky",             PlaylistId::CubicSpooky},
      {           "GForceFrenzy",            PlaylistId::GForceFrenzy},
};

// More familiar names for playlist ids
const std::map<PlaylistId, std::string> playlist_ids_str_spaced = {
      {                PlaylistId::Unknown,                      "Unknown"},
      {                 PlaylistId::Casual,                       "Casual"},
      {                   PlaylistId::Duel,                  "Casual Duel"},
      {                PlaylistId::Doubles,               "Casual Doubles"},
      {               PlaylistId::Standard,              "Casual Standard"},
      {                  PlaylistId::Chaos,                 "Casual Chaos"},
      {           PlaylistId::PrivateMatch,                "Private Match"},
      {                 PlaylistId::Season,               "Offline Season"},
      {     PlaylistId::OfflineSplitscreen,                   "Exhibition"},
      {               PlaylistId::Training,                    "Free Play"},
      {         PlaylistId::RankedSoloDuel,                  "Ranked Duel"},
      {      PlaylistId::RankedTeamDoubles,               "Ranked Doubles"},
      {         PlaylistId::RankedStandard,              "Ranked Standard"},
      {          PlaylistId::MutatorMashup,               "Mutator Mashup"},
      {       PlaylistId::SnowDayPromotion,              "Casual Snow Day"},
      {           PlaylistId::Experimental,                  "Rocket Labs"},
      {      PlaylistId::BasketballDoubles,                 "Casual Hoops"},
      {                 PlaylistId::Rumble,                "Casual Rumble"},
      {               PlaylistId::Workshop,                     "Workshop"},
      {      PlaylistId::UGCTrainingEditor,       "Custom Training Editor"},
      {            PlaylistId::UGCTraining,              "Custom Training"},
      {             PlaylistId::Tournament,            "Custom Tournament"},
      {               PlaylistId::Breakout,              "Casual Dropshot"},
      {               PlaylistId::LANMatch,                    "LAN Match"},
      {       PlaylistId::TenthAnniversary,            "Tenth Anniversary"},
      {                 PlaylistId::FaceIt,               "External Match"},
      {PlaylistId::RankedBasketballDoubles,                 "Ranked Hoops"},
      {           PlaylistId::RankedRumble,                "Ranked Rumble"},
      {         PlaylistId::RankedBreakout,              "Ranked Dropshot"},
      {          PlaylistId::RankedSnowDay,              "Ranked Snow Day"},
      {            PlaylistId::HauntedBall,                   "Ghost Hunt"},
      {              PlaylistId::BeachBall,                   "Beach Ball"},
      {                  PlaylistId::Rugby,                   "Spike Rush"},
      {         PlaylistId::AutoTournament,                   "Tournament"},
      {             PlaylistId::RocketLabs,                  "Rocket Labs"},
      {                PlaylistId::RumShot,              "Dropshot Rumble"},
      {                PlaylistId::GodBall,                   "Heatseeker"},
      {               PlaylistId::CoopVsAI,                   "Coop Vs AI"},
      {             PlaylistId::BoomerBall,                  "Boomer Ball"},
      {         PlaylistId::GodBallDoubles,           "Heatseeker Doubles"},
      {         PlaylistId::SpecialSnowDay,             "Winter Breakaway"},
      {               PlaylistId::Football,                     "Gridiron"},
      {                  PlaylistId::Cubic,                   "Super Cube"},
      {         PlaylistId::TacticalRumble,              "Tactical Rumble"},
      {           PlaylistId::SpringLoaded,                "Spring Loaded"},
      {             PlaylistId::SpeedDemon,                  "Speed Demon"},
      {               PlaylistId::RumbleBM,           "Gotham City Rumble"},
      {               PlaylistId::Knockout,                     "Knockout"},
      {             PlaylistId::Thirdwheel, "confidential_thirdwheel_test"},
      {          PlaylistId::MagnusFutball,             "Nike FC Showdown"},
      {          PlaylistId::GodBallSpooky,           "Haunted Heatseeker"},
      {        PlaylistId::GodBallRicochet,          "Heatseeker Ricochet"},
      {            PlaylistId::CubicSpooky,                  "Spooky Cube"},
      {           PlaylistId::GForceFrenzy,               "G-Force Frenzy"},
};

const std::map<std::string, std::vector<PlaylistId>> playlist_categories = {
  // S14 - casually viable modes go here
      {       "Casual",
       {PlaylistId::Duel,
       PlaylistId::Doubles,
       PlaylistId::Standard,
       PlaylistId::Chaos,

       PlaylistId::Rumble,
       PlaylistId::SnowDayPromotion,
       PlaylistId::BasketballDoubles,
       PlaylistId::Breakout,

       PlaylistId::MutatorMashup,
       PlaylistId::Experimental,
       PlaylistId::TenthAnniversary,
       PlaylistId::FaceIt,
       PlaylistId::HauntedBall,
       PlaylistId::BeachBall,
       PlaylistId::Rugby,
       PlaylistId::RocketLabs,
       PlaylistId::RumShot,
       PlaylistId::GodBall,
       PlaylistId::CoopVsAI,
       PlaylistId::BoomerBall,
       PlaylistId::GodBallDoubles,
       PlaylistId::SpecialSnowDay,
       PlaylistId::Football,
       PlaylistId::Cubic,
       PlaylistId::TacticalRumble,
       PlaylistId::SpringLoaded,
       PlaylistId::SpeedDemon,
       PlaylistId::RumbleBM,
       PlaylistId::Knockout,
       PlaylistId::Thirdwheel,
       PlaylistId::MagnusFutball,
       PlaylistId::GodBallSpooky,
       PlaylistId::GodBallRicochet,
       PlaylistId::CubicSpooky,
       PlaylistId::GForceFrenzy}                                                                      },
      {  "Competitive",
       {PlaylistId::RankedSoloDuel,
       PlaylistId::RankedTeamDoubles,
       PlaylistId::RankedStandard,
       PlaylistId::RankedRumble,
       PlaylistId::RankedBasketballDoubles,
       PlaylistId::RankedBreakout,
       PlaylistId::RankedSnowDay}                                                                     },
      {   "Tournament",                           {PlaylistId::AutoTournament, PlaylistId::Tournament}},
      {     "Training", {PlaylistId::Training, PlaylistId::UGCTraining, PlaylistId::UGCTrainingEditor}},
      {      "Offline",     {PlaylistId::OfflineSplitscreen, PlaylistId::Season, PlaylistId::Workshop}},
      {"Private Match",                               {PlaylistId::PrivateMatch, PlaylistId::LANMatch}},
};

const std::map<pluginsdk::Equipslot, std::string> equipslot_strs = {

      {            pluginsdk::Equipslot::BODY,             "BODY"},
      {           pluginsdk::Equipslot::DECAL,            "DECAL"},
      {          pluginsdk::Equipslot::WHEELS,           "WHEELS"},
      {     pluginsdk::Equipslot::ROCKETBOOST,      "ROCKETBOOST"},
      {         pluginsdk::Equipslot::ANTENNA,          "ANTENNA"},
      {          pluginsdk::Equipslot::TOPPER,           "TOPPER"},
      {          pluginsdk::Equipslot::BUMPER,           "BUMPER"},
      {     pluginsdk::Equipslot::PAINTFINISH,      "PAINTFINISH"},
      {             pluginsdk::Equipslot::BOT,              "BOT"},
      {            pluginsdk::Equipslot::LOGO,             "LOGO"},
      {       pluginsdk::Equipslot::UNDERGLOW,        "UNDERGLOW"},
      {          pluginsdk::Equipslot::CRATES,           "CRATES"},
      {    pluginsdk::Equipslot::CUSTOMFINISH,     "CUSTOMFINISH"},
      {     pluginsdk::Equipslot::ENGINEAUDIO,      "ENGINEAUDIO"},
      {           pluginsdk::Equipslot::TRAIL,            "TRAIL"},
      {   pluginsdk::Equipslot::GOALEXPLOSION,    "GOALEXPLOSION"},
      {    pluginsdk::Equipslot::PLAYERBANNER,     "PLAYERBANNER"},
      {pluginsdk::Equipslot::GARAGECOMPLEXROW, "GARAGECOMPLEXROW"},
      {     pluginsdk::Equipslot::GOALSTINGER,      "GOALSTINGER"},
      {    pluginsdk::Equipslot::PLAYERAVATAR,     "PLAYERAVATAR"},
      {    pluginsdk::Equipslot::AVATARBORDER,     "AVATARBORDER"},
      {     pluginsdk::Equipslot::PLAYERTITLE,      "PLAYERTITLE"},
      {     pluginsdk::Equipslot::ESPORTSTEAM,      "ESPORTSTEAM"},
      {   pluginsdk::Equipslot::ARCHIVEDITEMS,    "ARCHIVEDITEMS"},
      {      pluginsdk::Equipslot::BLUEPRINTS,       "BLUEPRINTS"},
      {        pluginsdk::Equipslot::SHOPITEM,         "SHOPITEM"},
      {        pluginsdk::Equipslot::CURRENCY,         "CURRENCY"},
      {             pluginsdk::Equipslot::MAX,              "MAX"},
};

const std::map<pluginsdk::Itempaint, std::string> itempaint_strs = {
      {         pluginsdk::Itempaint::NONE,          "NONE"},
      {      pluginsdk::Itempaint::CRIMSON,       "CRIMSON"},
      {         pluginsdk::Itempaint::LIME,          "LIME"},
      {        pluginsdk::Itempaint::BLACK,         "BLACK"},
      {      pluginsdk::Itempaint::SKYBLUE,       "SKYBLUE"},
      {       pluginsdk::Itempaint::COBALT,        "COBALT"},
      {  pluginsdk::Itempaint::BURNTSIENNA,   "BURNTSIENNA"},
      {  pluginsdk::Itempaint::FORESTGREEN,   "FORESTGREEN"},
      {       pluginsdk::Itempaint::PURPLE,        "PURPLE"},
      {         pluginsdk::Itempaint::PINK,          "PINK"},
      {       pluginsdk::Itempaint::ORANGE,        "ORANGE"},
      {         pluginsdk::Itempaint::GREY,          "GREY"},
      {pluginsdk::Itempaint::TITANIUMWHITE, "TITANIUMWHITE"},
      {      pluginsdk::Itempaint::SAFFRON,       "SAFFRON"},
      {         pluginsdk::Itempaint::GOLD,          "GOLD"},
      {     pluginsdk::Itempaint::ROSEGOLD,      "ROSEGOLD"},
      {    pluginsdk::Itempaint::WHITEGOLD,     "WHITEGOLD"},
      {         pluginsdk::Itempaint::ONYX,          "ONYX"},
      {     pluginsdk::Itempaint::PLATINUM,      "PLATINUM"},
};

const std::map<OnlinePlatform, std::string> onlineplatform_ids_str = {
      {OnlinePlatform_Unknown, "OnlinePlatform_Unknown"},
      {  OnlinePlatform_Steam,   "OnlinePlatform_Steam"},
      {    OnlinePlatform_PS4,     "OnlinePlatform_PS4"},
      {    OnlinePlatform_PS3,     "OnlinePlatform_PS3"},
      {  OnlinePlatform_Dingo,   "OnlinePlatform_Dingo"}, // xbox?
      {     OnlinePlatform_QQ,      "OnlinePlatform_QQ"},
      { OnlinePlatform_OldNNX,  "OnlinePlatform_OldNNX"},
      {    OnlinePlatform_NNX,     "OnlinePlatform_NNX"},
      { OnlinePlatform_PsyNet,  "OnlinePlatform_PsyNet"},
      {OnlinePlatform_Deleted, "OnlinePlatform_Deleted"},
      { OnlinePlatform_WeGame,  "OnlinePlatform_WeGame"},
      {   OnlinePlatform_Epic,    "OnlinePlatform_Epic"},
      {    OnlinePlatform_MAX,     "OnlinePlatform_MAX"}
};

const std::map<int, std::string> MapsFNames = {
      {     0,                  "Nothing"},
      {  1316,             "Farm_Night_P"},
      {  1781,            "CHN_Stadium_P"},
      {  3251,        "throwbackhockey_p"},
      {  3254,                  "music_p"},
      {  3257,             "Labs_Basin_P"},
      {  3260,          "Labs_Corridor_P"},
      {  3263,           "Labs_Galleon_P"},
      {  3269,        "CHN_Stadium_Day_P"},
      {  3272,                     "bb_p"},
      {  3755,       "Stadium_Race_Day_p"},
      {  3788,      "Labs_Galleon_Mast_P"},
      {  4094,         "Labs_Holyfield_P"},
      {  4316,              "park_bman_p"},
      {  4445,       "Labs_PillarGlass_P"},
      {  4451,        "Labs_PillarHeat_P"},
      {  4457,       "Labs_PillarWings_P"},
      {  4463,             "Park_Snowy_P"},
      {  5486,            "KO_Calavera_P"},
      {  5492,              "KO_Carbon_P"},
      {  5498,             "KO_Quadron_P"},
      {  6611,            "FNI_Stadium_P"},
      {  9082,            "Woods_Night_P"},
      { 13707,          "UtopiaStadium_P"},
      { 13708,     "UtopiaStadium_Dusk_P"},
      { 13709,     "UtopiaStadium_Snow_P"},
      { 13710,      "UtopiaStadium_Lux_P"},
      { 13711,                "Stadium_P"},
      { 13712,           "TrainStation_P"},
      { 13713,            "EuroStadium_P"},
      { 13714,                   "Park_P"},
      { 13715,      "TrainStation_Dawn_P"},
      { 13716,     "TrainStation_Night_P"},
      { 13717,   "Haunted_TrainStation_P"},
      { 13718,      "EuroStadium_Rainy_P"},
      { 13719,             "Park_Night_P"},
      { 13720,             "Park_Rainy_P"},
      { 13721,          "stadium_Rainy_p"},
      { 13722,              "Wasteland_P"},
      { 13723,        "Wasteland_Night_P"},
      { 13724,            "wasteland_s_p"},
      { 13725,      "wasteland_Night_S_P"},
      { 13726,               "NeoTokyo_P"},
      { 13727,      "NeoTokyo_Standard_P"},
      { 13728,             "Underwater_P"},
      { 13729,                    "ARC_P"},
      { 13730,           "arc_standard_p"},
      { 13734,         "Stadium_Winter_P"},
      { 13735,          "Stadium_Foggy_P"},
      { 13736,      "EuroStadium_Night_P"},
      { 13737,           "HoopsStadium_P"},
      { 13738,            "Labs_Cosmic_P"},
      { 13739,         "Labs_Cosmic_V4_P"},
      { 13740,        "Labs_DoubleGoal_P"},
      { 13741,     "Labs_DoubleGoal_V2_P"},
      { 13742,         "Labs_Underpass_P"},
      { 13743,            "Labs_Utopia_P"},
      { 13744,     "Labs_CirclePillars_P"},
      { 13745,           "Labs_Octagon_P"},
      { 13746,        "Labs_Octagon_02_P"},
      { 13747,            "ShatterShot_P"},
      { 13748,                     "cs_p"},
      { 13749,                 "cs_day_p"},
      { 13750,            "stadium_day_p"},
      { 13751,     "EuroStadium_Winter_P"},
      { 13752,  "eurostadium_snownight_p"},
      { 13753,                   "farm_p"},
      { 13754,       "throwbackstadium_P"},
      { 13755,                  "beach_P"},
      { 13756,                  "cs_hw_p"},
      { 13757,            "beach_night_p"},
      { 13759,        "Farm_UpsideDown_P"},
      { 13760,    "TrainStation_Spooky_P"},
      { 13761,                 "outlaw_p"},
      { 13762,               "ARC_Darc_P"},
      { 13763,          "NeoTokyo_Toon_p"},
      { 13764,                 "street_p"},
      { 13765,            "hoopsStreet_p"},
      { 13766,                "Farm_HW_P"},
      { 13767,                 "swoosh_p"},
      { 13768,           "Outlaw_Oasis_P"},
      { 13769,                "FF_Dusk_P"},
      { 13770,           "NeoTokyo_Hax_P"},
      { 13771,       "EuroStadium_Dusk_P"},
      { 13772,          "Wasteland_GRS_P"},
      { 13773,               "Farm_GRS_P"},
      { 13774,         "Underwater_GRS_P"},
      { 13775,        "Beach_Night_GRS_P"},
      { 13776,                  "woods_p"},
      { 22681,              "menu_main_p"},
      { 77499,      "Labs_Underpass_v0_p"},
      { 78310,  "MapThumb_NeoTokyo_Hax_P"},
      { 92722,           "BG_Woods_Day_P"},
      { 92723,         "BG_Woods_Night_P"},
      { 96033,        "HoopsStreet_Art_P"},
      { 96124, "NeoTokyo_Hax_Signs_Off_P"},
      { 96125,     "NeoTokyo_Hax_Signs_P"},
      { 96137,        "Outlaw_Oasis_FX_P"},
      {100721,    "Wasteland_Art_Night_P"},
      {100722,  "Wasteland_Art_Night_S_P"},
      {100723,          "Wasteland_Art_P"},
      {100724,        "Wasteland_Art_S_P"},
      {100725,      "Wasteland_GRS_Art_P"},
      {100732,    "Wasteland_Night_VFX_P"},
      {101944,           "Woods_Forest_P"},
      {114201,      "UEDPCBG_Woods_Day_P"}
};

namespace details {
      // PACK THESE STRUCTS INTO ALIGNMENTS OF 4 BYTES
      // I DONT KNOW WHY
      // A GAME THING I GUESS
      // #ifdef _MSC_VER
      // #pragma pack(push, 0x4)
      // #endif
      struct FSceNpOnlineId {
            unsigned char data[0x10];
            unsigned char term;
            unsigned char dummy[0x3];
      };

      struct FSceNpId {
            FSceNpOnlineId Handle;

            unsigned char opt[0x8];
            unsigned char reserved[0x8];
      };

      struct FNinUserId {
            unsigned char data[0x8];
            unsigned char context[0x8];
      };

      struct FUniqueNetId {
            unsigned char uid[0x8];

            FSceNpId   NpId;
            FNinUserId NinUserHandle;

            unsigned char platform;
            unsigned char splitscreenID;
      };

      struct FUniqueLobbyId {  // defined in Engine_structs.h
            unsigned char Uid[0x8];
            unsigned char platform;
      };

      struct FVoter {                      // defined in TAGame_structs.h
            uintptr_t     priwrapper_ptr;  // PriWrapper POINTER
            unsigned char status;
      };

      // FPointer
      struct FPointer {
            uintptr_t Dummy;
      };

      // FString
      // (0x0000 - 0x0010)
      class FString {
      public:
#ifdef _MBCS
            using ElementType = const char;
#else
#ifdef _UNICODE
            using ElementType = const wchar_t;

#endif
#endif

            using ElementPointer = ElementType *;

      private:
            ElementPointer ArrayData;   // 0x0000 (0x08)
            int32_t        ArrayCount;  // 0x0008 (0x04)
            int32_t        ArrayMax;    // 0x000C (0x04)

      public:
            FString() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) {}

            FString(ElementPointer other) : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) { assign(other); }

            ~FString() {}

      public:
#ifdef _MBCS
            FString & assign(ElementPointer other) {
                  ArrayCount = (other ? (strlen(other) + 1) : 0);
                  ArrayMax   = ArrayCount;
                  ArrayData  = (ArrayCount > 0 ? other : nullptr);
                  return *this;
            }

            std::string ToString() const {
                  if (!empty()) {
                        return std::string(ArrayData);
                  }

                  return "";
            }
#else
#ifdef _UNICODE
            FString & assign(ElementPointer other) {
                  ArrayCount = (other ? (wcslen(other) + 1) : 0);
                  ArrayMax   = ArrayCount;
                  ArrayData  = (ArrayCount > 0 ? other : nullptr);
                  return *this;
            }

            std::wstring ToWideString() const {
                  if (!empty()) {
                        return std::wstring {c_str()};
                  }

                  return L"";
            }

            std::string ToString() const {
                  if (!empty()) {
                        std::wstring wstr {ToWideString()};

                        char str[1024];
                        memset(str, 0, sizeof(str));
                        WideCharToMultiByte(
                              CP_UTF8,
                              WC_NO_BEST_FIT_CHARS,
                              wstr.c_str(),
                              wstr.length(),
                              str,
                              1024,
                              " ",
                              NULL);
                        // return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ToWideString());

                        return std::string {str};
                  }

                  return "";
            }

#endif
#endif

            ElementPointer c_str() const { return ArrayData; }

            bool empty() const {
                  if (ArrayData) {
                        return (ArrayCount == 0);
                  }

                  return true;
            }

            int32_t length() const { return ArrayCount; }

            int32_t size() const { return ArrayMax; }

      public:
            FString & operator=(ElementPointer other) { return assign(other); }

            FString & operator=(const FString & other) { return assign(other.c_str()); }

            bool operator==(const FString & other) {
#ifdef _MCBS
                  return (strcmp(ArrayData, other.ArrayData) == 0);
#else
#ifdef _UNICODE
                  return (wcscmp(ArrayData, other.ArrayData) == 0);
#endif
#endif
            }

            bool operator!=(const FString & other) {
#ifdef _MCBS
                  return (strcmp(ArrayData, other.ArrayData) != 0);
#else
#ifdef _UNICODE
                  return (wcscmp(ArrayData, other.ArrayData) != 0);

#endif
#endif
            }
      };

      struct FName {
            // "An index and a number into a hardcoded table of strings"... or whatever
            int32_t FNameEntryId;
            int32_t InstanceNumber;
      };

      template <typename TArray> class TIterator {
      public:
            using ElementType           = typename TArray::ElementType;
            using ElementPointer        = ElementType *;
            using ElementReference      = ElementType &;
            using ElementConstReference = const ElementType &;

      private:
            ElementPointer IteratorData;

      public:
            TIterator(ElementPointer inElementPointer) : IteratorData(inElementPointer) {}
            ~TIterator() {}

      public:
            TIterator & operator++() {
                  IteratorData++;
                  return *this;
            }

            TIterator operator++(int32_t) {
                  TIterator iteratorCopy = *this;
                  ++(*this);
                  return iteratorCopy;
            }

            TIterator & operator--() {
                  IteratorData--;
                  return *this;
            }

            TIterator operator--(int32_t) {
                  TIterator iteratorCopy = *this;
                  --(*this);
                  return iteratorCopy;
            }

            ElementReference operator[](int32_t index) { return *(IteratorData[index]); }

            ElementPointer operator->() { return IteratorData; }

            ElementReference operator*() { return *IteratorData; }

      public:
            bool operator==(const TIterator & other) const { return (IteratorData == other.IteratorData); }

            bool operator!=(const TIterator & other) const { return !(*this == other); }
      };

      template <typename InElementType> class TArray {
      public:
            using ElementType           = InElementType;
            using ElementPointer        = ElementType *;
            using ElementReference      = ElementType &;
            using ElementConstPointer   = const ElementType *;
            using ElementConstReference = const ElementType &;
            using Iterator              = TIterator<TArray<ElementType>>;

      private:
            ElementPointer ArrayData;
            int32_t        ArrayCount;
            int32_t        ArrayMax;

      public:
            TArray() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) {
                  // ReAllocate(sizeof(ElementType));
            }

            ~TArray() {
                  // clear();
                  //::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
            }

      public:
            ElementConstReference operator[](int32_t index) const { return ArrayData[index]; }

            ElementReference operator[](int32_t index) { return ArrayData[index]; }

            ElementConstReference at(int32_t index) const { return ArrayData[index]; }

            ElementReference at(int32_t index) { return ArrayData[index]; }

            ElementConstPointer data() const { return ArrayData; }

            void push_back(ElementConstReference newElement) {
                  if (ArrayCount >= ArrayMax) {
                        ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
                  }

                  new(&ArrayData[ArrayCount]) ElementType(newElement);
                  ArrayCount++;
            }

            void push_back(ElementReference & newElement) {
                  if (ArrayCount >= ArrayMax) {
                        ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
                  }

                  new(&ArrayData[ArrayCount]) ElementType(newElement);
                  ArrayCount++;
            }

            void pop_back() {
                  if (ArrayCount > 0) {
                        ArrayCount--;
                        ArrayData[ArrayCount].~ElementType();
                  }
            }

            void clear() {
                  for (int32_t i = 0; i < ArrayCount; i++) {
                        ArrayData[i].~ElementType();
                  }

                  ArrayCount = 0;
            }

            int32_t size() const { return ArrayCount; }

            int32_t capacity() const { return ArrayMax; }

            bool empty() const {
                  if (ArrayData) {
                        return (size() == 0);
                  }

                  return true;
            }

            Iterator begin() { return Iterator(ArrayData); }

            Iterator end() { return Iterator(ArrayData + ArrayCount); }

      private:
            void ReAllocate(int32_t newArrayMax) {
                  ElementPointer newArrayData = (ElementPointer)::operator new(newArrayMax * sizeof(ElementType));
                  int32_t        newNum       = ArrayCount;

                  if (newArrayMax < newNum) {
                        newNum = newArrayMax;
                  }

                  for (int32_t i = 0; i < newNum; i++) {
                        new(newArrayData + i) ElementType(std::move(ArrayData[i]));
                  }

                  for (int32_t i = 0; i < ArrayCount; i++) {
                        ArrayData[i].~ElementType();
                  }

                  ::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
                  ArrayData = newArrayData;
                  ArrayMax  = newArrayMax;
            }
      };

      class UClass;

      // Class Core.Object
      // (0x0000 - 0x0034)
      class UObject {
      public:
            FPointer  VfTableObject;
            FPointer  HashNext;
            int64_t   ObjectFlags;
            FPointer  HashOuterNext;
            FPointer  StateFrame;
            UObject * Linker;
            FPointer  LinkerIndex;
            int32_t   ObjectInternalInteger;
            int32_t   NetIndex;
            UObject * Outer;
            FName     Name;
            UClass *  Class;
            UObject * ObjectArchetype;
      };

      struct FScriptDelegate {
            UObject * Object;        // 0x0000 (0x04)
            FName     FunctionName;  // 0x0004 (0x08)
            uint8_t   unknown[0x8];  // the sdk was complaining about the size, saying there was something missing here
      };

      // Class Core.Field
      // 0x0008 (0x0034 - 0x003C)
      class UField : public UObject {
      public:
            class UField * Next;
            uint8_t        UnknownData[0x08];
      };

      // Class Core.Struct
      // 0x00C8
      class UStruct : public UField {
      public:
            uint8_t  UnknownData01[0x10];
            UField * SuperField;
            UField * Children;
            uint32_t PropertySize;
            uint8_t  UnknownData02[0x1C];
            int32_t  MinAlignment;
            uint8_t  UnknownData03[0x7C];
      };

      // Class Core.State
      // 0x0060 (0x0058 - 0x0118)
      class UState : public UStruct {
      public:
            uint8_t UnknownData00[0x60];
      };

      // Class Core.Class
      // 0x0228 (0x0058 - 0x0278)
      class UClass : public UState {
      public:
            uint8_t UnknownData00[0x228];
      };

      // Class Engine.NetworkEncryptionKey
      // 0x0040 (0x0060 - 0x00A0)
      class UNetworkEncryptionKey : public UObject {
      public:
            TArray<uint8_t> Key;        // 0x0060 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            TArray<uint8_t> IV;         // 0x0070 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            TArray<uint8_t> HMACKey;    // 0x0080 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            TArray<uint8_t> SessionId;  // 0x0090 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // ScriptStruct ProjectX._Types_X.JoinMatchSettings
      // 0x0020
      struct FJoinMatchSettings {
            uint8_t  MatchType;             // 0x0000 (0x0001) [0x0000000000000000]
            int32_t  PlaylistId;            // 0x0004 (0x0004) [0x0000000000000000]
            uint32_t bFriendJoin      : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000001]
            uint32_t bMigration       : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000002]
            uint32_t bRankedReconnect : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000004]
            FString  Password;              // 0x0010 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // ScriptStruct ProjectX._Types_X.ServerReservationData
      // 0x0070
      struct FServerReservationData {
            FString   ServerName;          // 0x0000 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            int32_t   Playlist;            // 0x0010 (0x0004) [0x0000000000000000]
            uint8_t   UnknownData00[0x4];  // 0x0014 (0x0004) MISSED OFFSET
            FString   Region;              // 0x0018 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString   ReservationID;       // 0x0028 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString   DSRToken;            // 0x0038 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            uintptr_t Keys_ptr;            // 0x0048 (0x0008) [0x0000000000000000]
            FString   JoinName;            // 0x0050 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString   JoinPassword;        // 0x0060 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // ScriptStruct ProjectX._Types_X.ActiveServerData
      // 0x00A0
      struct FActiveServerData {
            FServerReservationData Reservation;      // 0x0000 (0x0070) [0x0000000000400000] (CPF_NeedCtorLink)
            FString                PingURL;          // 0x0070 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString                GameURL;          // 0x0080 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString                JoinCredentials;  // 0x0090 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // ScriptStruct ProjectX._Types_X.ClubColorSet
      // 0x0008
      struct FClubColorSet {
            uint8_t  TeamColorID;          // 0x0000 (0x0001) [0x0000000000000000]
            uint8_t  CustomColorID;        // 0x0001 (0x0001) [0x0000000000000000]
            uint32_t bTeamColorSet   : 1;  // 0x0004 (0x0004) [0x0000000000000000] [0x00000001]
            uint32_t bCustomColorSet : 1;  // 0x0004 (0x0004) [0x0000000000000000] [0x00000002]
      };

      // ScriptStruct ProjectX._Types_X.CustomMatchTeamSettings
      // 0x001C
      struct FCustomMatchTeamSettings {
            FString       Name;       // 0x0000 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FClubColorSet Colors;     // 0x0010 (0x0008) [0x0000000000000000]
            int32_t       GameScore;  // 0x0018 (0x0004) [0x0000000000000000]
      };

      // ScriptStruct ProjectX._Types_X.CustomMatchSettings
      // 0x008C
      struct FCustomMatchSettings {
            FString                  GameTags;         // 0x0000 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FName                    MapName;          // 0x0010 (0x0008) [0x0000000000000000]
            uint8_t                  GameMode;         // 0x0018 (0x0001) [0x0000000000000000]
            int32_t                  MaxPlayerCount;   // 0x001C (0x0004) [0x0000000000000000]
            FString                  ServerName;       // 0x0020 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString                  Password;         // 0x0030 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            uint32_t                 bPublic : 1;      // 0x0040 (0x0004) [0x0000000000000000] [0x00000001]
            FCustomMatchTeamSettings TeamSettings[2];  // 0x0048 (0x0040) [0x0000000000400000] (CPF_NeedCtorLink)
            uint32_t                 bClubServer : 1;  // 0x0088 (0x0004) [0x0001000000000000] [0x00000001]
      };

      // Function ProjectX.OnlineGameJoinGame_X.StartJoin
      // [0x00824002]
      struct UOnlineGameJoinGame_X_execStartJoin_Params {
            FServerReservationData Reservation;   // 0x0000 (0x0070) [0x0000000000400080] (CPF_Parm | CPF_NeedCtorLink)
            FJoinMatchSettings     JoinSettings;  // 0x0070 (0x0020) [0x0000000000400090] (CPF_OptionalParm |
                                                  // CPF_Parm | CPF_NeedCtorLink)
            bool ReturnValue : 1;  // 0x0090 (0x0004) [0x0000000000000580] [0x00000001] (CPF_Parm | CPF_OutParm |
                                   // CPF_ReturnParm)
            // FActiveServerData                        NewActiveServer;                                  //
            // 0x0098 (0x00A0) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // Class ProjectX.StateObject_X
      // 0x0004 (0x0060 - 0x0064)
      class UStateObject_X : public UObject {
      public:
            uint32_t bDebug : 1;  // 0x0060 (0x0004) [0x0000000000000001] [0x00000001] (CPF_Edit)
      };

      // Class ProjectX.Online_X
      // 0x004C (0x0064 - 0x00B0)
      class UOnline_X : public UStateObject_X {
      public:
            uintptr_t       OnlineSub;  // 0x0068 (0x0008) [0x0000004000002000] (CPF_Transient | CPF_PrivateWrite)
            uintptr_t       PsyNet;     // 0x0070 (0x0008) [0x0000004000002000] (CPF_Transient | CPF_PrivateWrite)
            uintptr_t       EOS;        // 0x0078 (0x0008) [0x0000008000002000] (CPF_Transient | CPF_ProtectedWrite)
            FScriptDelegate __EventEOSInitialized__Delegate;  // 0x0080 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate __EventEosInitTimeout__Delegate;  // 0x0098 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // Class ProjectX.OnlineGameJoinGame_X
      // 0x03A0 (0x00B0 - 0x0450)
      class UOnlineGameJoinGame_X : public UOnline_X {
      public:
            int32_t   JoinCountdownTime;     // 0x00B0 (0x0004) (CPF_Edit)
            FString   FailCommand;           // 0x00B8 (0x0010) (CPF_Edit | CPF_NeedCtorLink)
            FString   LoadingScreenCommand;  // 0x00C8 (0x0010) (CPF_Edit | CPF_NeedCtorLink)
            uintptr_t JoinGameShake;  // 0x00D8 (0x0008) (CPF_Edit | CPF_ExportObject | CPF_Component | CPF_EditInline)
            uintptr_t ReservationBeacon;        // 0x00E0 (0x0008) (CPF_ExportObject | CPF_Component | CPF_EditInline)
            FString   WaitingForPlayersString;  // 0x00E8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   ReservationNotRespondingString;  // 0x00F8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   ReservationFullString;           // 0x0108 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   PartyTeamReservationFullString;  // 0x0118 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   NoFriendJoinPrivateMatchString;  // 0x0128 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   BeaconTimedOutString;            // 0x0138 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   NotAllPlayersJoinedString;       // 0x0148 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   CanceledString;                  // 0x0158 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   SecurityKeyAcquisitionFailed;    // 0x0168 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   SecurityKeyVerificationFailed;   // 0x0178 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   SendingReservationMessage;       // 0x0188 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   JoiningPartyLeadersGame;         // 0x0198 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   InvalidPassword;                 // 0x01A8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   WrongPlaylistString;             // 0x01B8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   WrongRankedMatchString;          // 0x01C8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   MatchEndedString;                // 0x01D8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   CrossplayDisabled;               // 0x01E8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString   AnotherPlayerCanceled;           // 0x01F8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FActiveServerData  ActiveServer;  // 0x0208 (0x00A0) (CPF_Transient | CPF_NeedCtorLink | CPF_PrivateWrite)
            FJoinMatchSettings Settings;      // 0x02A8 (0x0020) (CPF_Transient | CPF_NeedCtorLink | CPF_PrivateWrite)
            FString            PendingFailMessage;  // 0x02C8 (0x0010) (CPF_Transient | CPF_NeedCtorLink)
            TArray<uintptr_t>  JoinedPlayers;  // 0x02D8 (0x0010) (CPF_Transient | CPF_NeedCtorLink | CPF_PrivateWrite)
            FCustomMatchSettings CustomMatch;  // 0x02E8 (0x0090) (CPF_NeedCtorLink | CPF_PrivateWrite)
            FScriptDelegate
                  __EventJoinGameComplete__Delegate;        // 0x0378 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate __EventStatusUpdate__Delegate;  // 0x0390 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate
                  __EventCountdownStarted__Delegate;          // 0x03A8 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate __EventCountdownEnded__Delegate;  // 0x03C0 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate
                  __EventActiveServerChanged__Delegate;       // 0x03D8 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate __EventServerReserved__Delegate;  // 0x03F0 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate
                  __EventPasswordRequired__Delegate;       // 0x0408 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate __EventJoiningGame__Delegate;  // 0x0420 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            FScriptDelegate
                  __EventMaxPlayersChanged__Delegate;  // 0x0438 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      template <> class ArrayWrapper<details::FVoter> {};
      // Class TAGame.VoteActor_TA
      // 0x00B0 (0x02C0 - 0x0210)
      struct AVoteActor_TA : public ActorWrapper {
            AVoteActor_TA(std::uintptr_t mem) : ActorWrapper(mem) {}
            AVoteActor_TA(const AVoteActor_TA & other) = default;
            AVoteActor_TA & operator=(AVoteActor_TA &) = default;
            ~AVoteActor_TA()                           = default;

            // an FString of 0x0C size.
            // starts at 0x210 because of ActorWrapper
            unsigned char subject[0xC];

            int time_remaining;

            unsigned long b_unanimous_vote;
            unsigned long b_finished;

            ArrayWrapper<details::FVoter> voters;
            details::FVoter               replicated_voters[0x8];

            // taking a chance here. L13925 in TAGame_classes.h
            unsigned char __event_started_delegate[0x10];
            unsigned char __event_voters_changed_delegate[0x10];
            unsigned char __event_time_remaining_changed_delegate[0x10];
            unsigned char __event_finished_delegate[0x10];
            unsigned char __event_destroyed_delegate[0x10];

            void eventDestroyed();
            void DestroySelf();
            bool Failed();
            bool Passed();
            int  RequiredVotes();
            int  NoVotes();
            int  YesVotes();
            void FinishVote();
            void CheckFinished();
            void SetVoteStatus(PriWrapper * PRI, unsigned char Status);
            void OnVotersChanged();
            void RemoveVoter(PriWrapper * PRI);
            void AddVoter(PriWrapper * PRI);
            void UpdateTimeRemaining();
            void AddTeam(TeamWrapper * Team);
            void AddGameEvent(GameEventWrapper * GameEvent);
            void eventReplicatedEvent(details::FName VarName);
            void eventPostBeginPlay();
            void EventDestroyed(AVoteActor_TA * VoteActor);
            void EventFinished(AVoteActor_TA * VoteActor);
            void EventTimeRemainingChanged(AVoteActor_TA * VoteActor);
            void EventVotersChanged(AVoteActor_TA * VoteActor);
            void EventStarted(AVoteActor_TA * VoteActor);

            // it's 256 bytes so this may work :)
      };

      struct PartyChangeParams {
            // IM WORKING WITH VERY OLD OLD DATA!
            // FUniqueLobbyId party_id; FUniqueLobbyId (is 0x0C bytes)
            // FUniqueNetId leader_id; FUniqueNetId (is 0x48 bytes)
            char party_id[0x10];  // 3 MISSING BYTES SOMEWHERE BETWEEN PARTY_ID AND LEADER_ID
            // SO I JUST PADDED IT OUT IN PARTY_ID. NOW IT WORKS RIGHT.
            char leader_id[0x48];
            // FUniqueLobbyId party_id;  // SIZE = 0x09
            // unsigned char  padding[3];
            // FUniqueNetId   leader_id;  // SIZE = 0x46
            // unsigned char  padding2[2];

            unsigned long bLeader;
            unsigned int  party_size;
            unsigned int  local_players;
            unsigned int  remote_players;
      };

      // #ifdef _MSC_VER
      // #pragma pack(pop)
      // #endif
}  // namespace details
}  // namespace bm_helper

template <> struct std::formatter<bm_helper::details::FString> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FString & obj, std::format_context & ctx) const {
#ifdef _MCBS
            return std::format_to(ctx.out(), "{{str: {}, len: {}}}", obj.ToString(), obj.length())
#else
#ifdef _UNICODE
            return std::format_to(ctx.out(), L"{{str: {}, len: {}}}", obj.ToWideString(), obj.length());
#endif
#endif
      }
};

#define PARENS       ()
#define EXPAND(...)  EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...)            __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(, ) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN()                FOR_EACH_HELPER

#define PRINT_NAMEANDVAR(name) std::format("{}: {}", #name, name)

template <> struct std::formatter<bm_helper::details::FName> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FName & obj, std::format_context & ctx) const {
            auto FNameEntryId   = obj.FNameEntryId;
            auto InstanceNumber = obj.InstanceNumber;

            return std::format_to(ctx.out(), "{{{}, {}}}", FOR_EACH(PRINT_NAMEANDVAR, FNameEntryId, InstanceNumber));
      }
};

template <> struct std::formatter<bm_helper::details::FJoinMatchSettings> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FJoinMatchSettings & obj, std::format_context & ctx) const {
            bool     bFriendJoin     = obj.bFriendJoin;
            bool     bMigration      = obj.bMigration;
            bool     RankedReconnect = obj.bRankedReconnect;
            uint8_t  MatchType       = obj.MatchType;
            uint32_t PlaylistId      = obj.PlaylistId;
            auto     Password        = obj.Password;

            return std::format_to(
                  ctx.out(),
                  "FJoinMatchSettings{{{}, {}, {}, {}, {}, {}}}",
                  FOR_EACH(
                        PRINT_NAMEANDVAR,
                        bFriendJoin,
                        bMigration,
                        RankedReconnect,
                        MatchType,
                        PlaylistId,
                        Password));
      }
};

template <> struct std::formatter<bm_helper::details::FServerReservationData> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FServerReservationData & obj, std::format_context & ctx) const {
            int32_t Playlist      = obj.Playlist;
            auto    DSRToken      = obj.DSRToken;
            auto    JoinName      = obj.JoinName;
            auto    JoinPassword  = obj.JoinPassword;
            auto    Region        = obj.Region;
            auto    ReservationID = obj.ReservationID;
            auto    ServerName    = obj.ServerName;

            return std::format_to(
                  ctx.out(),
                  "FServerReservationData{{{}, {}, {}, {}, {}, {}, {}}}",
                  FOR_EACH(
                        PRINT_NAMEANDVAR,
                        Playlist,
                        DSRToken,
                        JoinName,
                        JoinPassword,
                        Region,
                        ReservationID,
                        ServerName));
      }
};

template <> struct std::formatter<bm_helper::details::FActiveServerData> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FActiveServerData & obj, std::format_context & ctx) const {
            auto Reservation     = obj.Reservation;
            auto GameURL         = obj.GameURL;
            auto PingURL         = obj.PingURL;
            auto JoinCredentials = obj.JoinCredentials;

            return std::format_to(
                  ctx.out(),
                  "{{{}, {}, {}, {}}}",
                  FOR_EACH(PRINT_NAMEANDVAR, Reservation, GameURL, PingURL, JoinCredentials));
      }
};

template <> struct std::formatter<bm_helper::details::UOnlineGameJoinGame_X_execStartJoin_Params> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::UOnlineGameJoinGame_X_execStartJoin_Params & obj, std::format_context & ctx)
            const {
            auto JoinSettings = obj.JoinSettings;
            auto Reservation  = obj.Reservation;
            bool ReturnValue  = obj.ReturnValue;

            return std::format_to(
                  ctx.out(),
                  "{{{}, {}, {}}}",
                  FOR_EACH(PRINT_NAMEANDVAR, JoinSettings, Reservation, ReturnValue));
      }
};

template <> struct std::formatter<bm_helper::details::FClubColorSet> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FClubColorSet & obj, std::format_context & ctx) const {
            auto TeamColorID     = obj.TeamColorID;
            auto CustomColorID   = obj.CustomColorID;
            auto bTeamColorSet   = obj.bTeamColorSet;
            auto bCustomColorSet = obj.bCustomColorSet;

            return std::format_to(
                  ctx.out(),
                  "{{{}, {}, {}, {}}}",
                  FOR_EACH(PRINT_NAMEANDVAR, TeamColorID, CustomColorID, bTeamColorSet, bCustomColorSet));
      }
};

template <> struct std::formatter<bm_helper::details::FCustomMatchTeamSettings> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FCustomMatchTeamSettings & obj, std::format_context & ctx) const {
            auto Name      = obj.Name;
            auto Colors    = obj.Colors;
            auto GameScore = obj.GameScore;

            return std::format_to(ctx.out(), "{{{}, {}, {}}}", FOR_EACH(PRINT_NAMEANDVAR, Name, Colors, GameScore));
      }
};

template <> struct std::formatter<bm_helper::details::FCustomMatchSettings> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::FCustomMatchSettings & obj, std::format_context & ctx) const {
            auto GameTags       = obj.GameTags;  // FString
            auto MapName        = obj.MapName;   // FName
            auto MapNameStr     = bm_helper::MapsFNames.at(obj.MapName.FNameEntryId);
            auto GameMode       = obj.GameMode;         // uint8_t
            auto MaxPlayerCount = obj.MaxPlayerCount;   // int32_t
            auto ServerName     = obj.ServerName;       // FString
            auto Password       = obj.Password;         // FString
            auto bPublic        = obj.bPublic;          // uint32_t
            auto TeamSettings0  = obj.TeamSettings[0];  // FCustomMatchTeamSettings
            auto TeamSettings1  = obj.TeamSettings[1];  // FCustomMatchTeamSettings
            auto bClubServer    = obj.bClubServer;      // uint32_t

            return std::format_to(
                  ctx.out(),
                  "{{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}}",
                  FOR_EACH(
                        PRINT_NAMEANDVAR,
                        GameTags,
                        MapName,
                        MapNameStr,
                        GameMode,
                        MaxPlayerCount,
                        ServerName,
                        Password,
                        bPublic,
                        TeamSettings0,
                        TeamSettings1,
                        bClubServer));
      }
};

template <> struct std::formatter<bm_helper::details::UOnlineGameJoinGame_X> {
      constexpr auto parse(std::format_parse_context & ctx) { return ctx.begin(); }

      auto format(const bm_helper::details::UOnlineGameJoinGame_X & obj, std::format_context & ctx) const {
            auto JoinCountdownTime              = obj.JoinCountdownTime;               // int32_t
            auto FailCommand                    = obj.FailCommand;                     // FString
            auto LoadingScreenCommand           = obj.LoadingScreenCommand;            // FString
            auto JoinGameShake                  = obj.JoinGameShake;                   // uintptr_t
            auto ReservationBeacon              = obj.ReservationBeacon;               // uintptr_t
            auto WaitingForPlayersString        = obj.WaitingForPlayersString;         // FString
            auto ReservationNotRespondingString = obj.ReservationNotRespondingString;  // FString
            auto ReservationFullString          = obj.ReservationFullString;           // FString
            auto PartyTeamReservationFullString = obj.PartyTeamReservationFullString;  // FString
            auto NoFriendJoinPrivateMatchString = obj.NoFriendJoinPrivateMatchString;  // FString
            auto BeaconTimedOutString           = obj.BeaconTimedOutString;            // FString
            auto NotAllPlayersJoinedString      = obj.NotAllPlayersJoinedString;       // FString
            auto CanceledString                 = obj.CanceledString;                  // FString
            auto SecurityKeyAcquisitionFailed   = obj.SecurityKeyAcquisitionFailed;    // FString
            auto SecurityKeyVerificationFailed  = obj.SecurityKeyVerificationFailed;   // FString
            auto SendingReservationMessage      = obj.SendingReservationMessage;       // FString
            auto JoiningPartyLeadersGame        = obj.JoiningPartyLeadersGame;         // FString
            auto InvalidPassword                = obj.InvalidPassword;                 // FString
            auto WrongPlaylistString            = obj.WrongPlaylistString;             // FString
            auto WrongRankedMatchString         = obj.WrongRankedMatchString;          // FString
            auto MatchEndedString               = obj.MatchEndedString;                // FString
            auto CrossplayDisabled              = obj.CrossplayDisabled;               // FString
            auto AnotherPlayerCanceled          = obj.AnotherPlayerCanceled;           // FString
            auto ActiveServer                   = obj.ActiveServer;                    // FActiveServerData
            auto Settings                       = obj.Settings;                        // FJoinMatchSettings
            auto PendingFailMessage             = obj.PendingFailMessage;              // FString
            auto JoinedPlayers                  = "<check source code>";  // obj.JoinedPlayers; // TArray<uintptr_t>
            auto CustomMatch                    = obj.CustomMatch;        // FCustomMatchSettings

            return std::format_to(
                  ctx.out(),
                  "{{{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}}}",
                  FOR_EACH(
                        PRINT_NAMEANDVAR,
                        JoinCountdownTime,
                        FailCommand,
                        LoadingScreenCommand,
                        JoinGameShake,
                        ReservationBeacon,
                        WaitingForPlayersString,
                        ReservationNotRespondingString,
                        ReservationFullString,
                        PartyTeamReservationFullString,
                        NoFriendJoinPrivateMatchString,
                        BeaconTimedOutString,
                        NotAllPlayersJoinedString,
                        CanceledString,
                        SecurityKeyAcquisitionFailed,
                        SecurityKeyVerificationFailed,
                        SendingReservationMessage,
                        JoiningPartyLeadersGame,
                        InvalidPassword,
                        WrongPlaylistString,
                        WrongRankedMatchString,
                        MatchEndedString,
                        CrossplayDisabled,
                        AnotherPlayerCanceled,
                        ActiveServer,
                        Settings,
                        PendingFailMessage,
                        JoinedPlayers,
                        CustomMatch));
      }
};

#endif
