#ifndef __BM_HELPER_HPP__
#define __BM_HELPER_HPP__

#include <map>
#include <string>
#include <vector>

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

namespace details {
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
      // (0x0000 - 0x0004)
      struct FPointer {
            uintptr_t Dummy;  // 0x0000 (0x04)
      };

      // FString
      // (0x0000 - 0x0010)
      class FString {
      public:
#ifdef _UNICODE
            using ElementType = const wchar_t;
#else
#ifdef _MBCS
            using ElementType = const char;
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
#else
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
#ifdef _UNICODE
                  return (wcscmp(ArrayData, other.ArrayData) == 0);
#else
#ifdef _MCBS
                  return (strcmp(ArrayData, other.ArrayData) == 0);
#endif
#endif
            }

            bool operator!=(const FString & other) {
#ifdef _UNICODE
                  return (wcscmp(ArrayData, other.ArrayData) != 0);
#else
#ifdef _MCBS
                  return (strcmp(ArrayData, other.ArrayData) != 0);
#endif
#endif
            }
      };

      struct FName {
            // "An index and a number into a hardcoded table of strings"... or whatever
            int index;
            int number;
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
            uint8_t  UnknownData00[0x10];  // ? hmm
            UField * Children;
            uint32_t PropertySize;
            int32_t  MinAlignment;
            uint8_t  UnknownData01[0x98];
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
            uint8_t       MatchType;             // 0x0000 (0x0001) [0x0000000000000000]
            int32_t       PlaylistId;            // 0x0004 (0x0004) [0x0000000000000000]
            uint32_t      bFriendJoin      : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000001]
            uint32_t      bMigration       : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000002]
            uint32_t      bRankedReconnect : 1;  // 0x0008 (0x0004) [0x0000000000000000] [0x00000004]
            class FString Password;              // 0x0010 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
      };

      // ScriptStruct ProjectX._Types_X.ServerReservationData
      // 0x0070
      struct FServerReservationData {
            FString ServerName;     // 0x0000 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            int32_t Playlist;       // 0x0010 (0x0004) [0x0000000000000000]
            FString Region;         // 0x0018 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString ReservationID;  // 0x0028 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString DSRToken;       // 0x0038 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            // UNetworkEncryptionKey * Keys;           // 0x0048 (0x0008) [0x0000000000000000]
            uint8_t padding[0xC0];
            FString JoinName;      // 0x0050 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
            FString JoinPassword;  // 0x0060 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
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

      struct UOnlineGameJoinGame_X_execStartJoin_Params {
            struct FServerReservationData
                  Reservation;  // 0x0000 (0x0070) [0x0000000000400080] (CPF_Parm | CPF_NeedCtorLink)
            struct FJoinMatchSettings JoinSettings;  // 0x0070 (0x0020) [0x0000000000400090] (CPF_OptionalParm |
                                                     // CPF_Parm | CPF_NeedCtorLink)
            bool ReturnValue;  // 0x0090 (0x0004) [0x0000000000000580] [0x00000001] (CPF_Parm | CPF_OutParm |
                               // CPF_ReturnParm)
            // uint8_t padding[0xF];
            struct FActiveServerData NewActiveServer;  //
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
            // class UOnlineSubsystem *
            //                   OnlineSub;   // 0x0068 (0x0008) [0x0000004000002000] (CPF_Transient | CPF_PrivateWrite)
            // class UPsyNet_X * PsyNet;      // 0x0070 (0x0008) [0x0000004000002000] (CPF_Transient | CPF_PrivateWrite)
            // class UOnlineSubsystem * EOS;  // 0x0078 (0x0008) [0x0000008000002000] (CPF_Transient |
            // CPF_ProtectedWrite) struct FScriptDelegate
            //         __EventEOSInitialized__Delegate;  // 0x0080 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            // uint8_t __EventEOSInitialized__Delegate_UnknownData00[0x8];  // 0x0088 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // struct FScriptDelegate
            //         __EventEosInitTimeout__Delegate;  // 0x0098 (0x0018) [0x0000000000400000] (CPF_NeedCtorLink)
            // uint8_t __EventEosInitTimeout__Delegate_UnknownData01[0x8];  // 0x00A0 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            uint8_t padding[0x28];
      };

      // Class ProjectX.OnlineGameJoinGame_X
      // 0x03A0 (0x00B0 - 0x0450)
      class UOnlineGameJoinGame_X : public UOnline_X {
      public:
            uint8_t pad[0x20];             // no idea.
            int32_t JoinCountdownTime;     // 0x00B0 (0x0004) (CPF_Edit)
            FString FailCommand;           // 0x00B8 (0x0010) (CPF_Edit | CPF_NeedCtorLink)
            FString LoadingScreenCommand;  // 0x00C8 (0x0010) (CPF_Edit | CPF_NeedCtorLink)
            // UShakeComponent_X *
            //        JoinGameShake;  // 0x00D8 (0x0008) (CPF_Edit | CPF_ExportObject | CPF_Component | CPF_EditInline)
            uint8_t pad1[0x8];  // covers the above's pointer
            // UReservationBeacon_X *
            //        ReservationBeacon;        // 0x00E0 (0x0008) (CPF_ExportObject | CPF_Component | CPF_EditInline)
            uint8_t pad2[0x8];                       // covers the above's pointer
            FString WaitingForPlayersString;         // 0x00E8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString ReservationNotRespondingString;  // 0x00F8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString ReservationFullString;           // 0x0108 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString PartyTeamReservationFullString;  // 0x0118 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString NoFriendJoinPrivateMatchString;  // 0x0128 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString BeaconTimedOutString;            // 0x0138 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString NotAllPlayersJoinedString;       // 0x0148 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString CanceledString;                  // 0x0158 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString SecurityKeyAcquisitionFailed;    // 0x0168 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString SecurityKeyVerificationFailed;   // 0x0178 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString SendingReservationMessage;       // 0x0188 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString JoiningPartyLeadersGame;         // 0x0198 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString InvalidPassword;                 // 0x01A8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString WrongPlaylistString;             // 0x01B8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString WrongRankedMatchString;          // 0x01C8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString MatchEndedString;                // 0x01D8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString CrossplayDisabled;               // 0x01E8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FString AnotherPlayerCanceled;           // 0x01F8 (0x0010) (CPF_Const | CPF_Localized | CPF_NeedCtorLink)
            FActiveServerData    ActiveServer;  // 0x0208 (0x00A0) (CPF_Transient | CPF_NeedCtorLink | CPF_PrivateWrite)
            FJoinMatchSettings   Settings;      // 0x02A8 (0x0020) (CPF_Transient | CPF_NeedCtorLink | CPF_PrivateWrite)
            FString              PendingFailMessage;  // 0x02C8 (0x0010) (CPF_Transient | CPF_NeedCtorLink)
            // TArray<UPlayer *>  JoinedPlayers;  // 0x02D8 (0x0010) (CPF_Transient | CPF_NeedCtorLink |
            // CPF_PrivateWrite)
            uint8_t              pad3[0x10];   // covers the above's TArray
            FCustomMatchSettings CustomMatch;  // 0x02E8 (0x0090) (CPF_NeedCtorLink | CPF_PrivateWrite)
            // FScriptDelegate      __EventJoinGameComplete__Delegate;  // 0x0378 (0x0018) (CPF_NeedCtorLink)
            // uint8_t
            //       __EventJoinGameComplete__Delegate_UnknownData00[0x8];  // 0x0380 (0x0008) FIX WRONG SIZE OF
            //       PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventStatusUpdate__Delegate;               // 0x0390 (0x0018) (CPF_NeedCtorLink)
            // uint8_t __EventStatusUpdate__Delegate_UnknownData01[0x8];    // 0x0398 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventCountdownStarted__Delegate;           // 0x03A8 (0x0018) (CPF_NeedCtorLink)
            // uint8_t
            //       __EventCountdownStarted__Delegate_UnknownData02[0x8];  // 0x03B0 (0x0008) FIX WRONG SIZE OF
            //       PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventCountdownEnded__Delegate;             // 0x03C0 (0x0018) (CPF_NeedCtorLink)
            // uint8_t __EventCountdownEnded__Delegate_UnknownData03[0x8];  // 0x03C8 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventActiveServerChanged__Delegate;        // 0x03D8 (0x0018) (CPF_NeedCtorLink)
            // uint8_t         __EventActiveServerChanged__Delegate_UnknownData04
            //       [0x8];  // 0x03E0 (0x0008) FIX WRONG SIZE OF PREVIOUS PROPERTY  [Original:0x0018, Missing: 0x0008]
            // FScriptDelegate __EventServerReserved__Delegate;             // 0x03F0 (0x0018) (CPF_NeedCtorLink)
            // uint8_t __EventServerReserved__Delegate_UnknownData05[0x8];  // 0x03F8 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventPasswordRequired__Delegate;           // 0x0408 (0x0018) (CPF_NeedCtorLink)
            // uint8_t
            //       __EventPasswordRequired__Delegate_UnknownData06[0x8];  // 0x0410 (0x0008) FIX WRONG SIZE OF
            //       PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventJoiningGame__Delegate;                // 0x0420 (0x0018) (CPF_NeedCtorLink)
            // uint8_t __EventJoiningGame__Delegate_UnknownData07[0x8];     // 0x0428 (0x0008) FIX WRONG SIZE OF
            // PREVIOUS
            //                                                              // PROPERTY  [Original:0x0018, Missing:
            //                                                              0x0008]
            // FScriptDelegate __EventMaxPlayersChanged__Delegate;          // 0x0438 (0x0018) (CPF_NeedCtorLink)
            // uint8_t         __EventMaxPlayersChanged__Delegate_UnknownData08
            //       [0x8];  // 0x0440 (0x0008) FIX WRONG SIZE OF PREVIOUS PROPERTY  [Original:0x0018, Missing: 0x0008]
            // uint8_t pad4[0xD8];  // covers the above FScriptDelegate(s)s and their missing unknown data
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
}  // namespace details
}  // namespace bm_helper

#endif
