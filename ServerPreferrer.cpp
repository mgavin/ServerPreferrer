/**
 * TODO??????????????????????
 * - Add logging files
 * - persistent storage
 * - init data
 * - save data {csv}
 * - plugin menu
 *
 * - uh, checks for being in a party
 *
 * - drag and drop list (with multiple pages) of servers to automatically dislike/reject
 * ------------------------------------------------------------------
 *
 */

#include "ServerPreferrer.hpp"

#include <chrono>
#include <future>
#include <optional>
#include <regex>
#include <thread>
#include <variant>

// clang-format off
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h" // IWYU pragma: keep
#include "shellapi.h"
// #include "WinSock2.h"
#include "WS2tcpip.h"

// icmp echo
#include "iphlpapi.h" // IWYU pragma: keep
#include "IcmpAPI.h"
// clang-format on

#include "imgui.h"
#include "imgui_internal.h"

#include "vincentlaucsb-csv-parser/csv.hpp"

#include "bm_helper.hpp"
#include "CVarManager.hpp"
#include "HookedEvents.hpp"
#include "imgui_helper.hpp"
#include "Logger.hpp"
#include "PersistentManagedCVarStorage.hpp"

namespace {
using log = LOGGER;
}

BAKKESMOD_PLUGIN(ServerPreferrer, "ServerPreferrer", plugin_version, /*UNUSED*/ NULL);

/// <summary>
/// do the following when your plugin is loadedx
/// </summary>
void ServerPreferrer::onLoad() {
      // initialize things
      HookedEvents::gameWrapper = gameWrapper;

      // set up logging necessities
      log::set_cvarmanager(cvarManager);
      log::set_loglevel(LOGLEVEL::INFO);

      // set a prefix to attach in front of all cvars to avoid name clashes
      CVarManager::instance().set_cvar_prefix("sp_");  // INCLUDE PLUGIN CVAR PREFIX HERE!!!
      CVarManager::instance().set_cvarmanager(cvarManager);

      // MAKE SECOND PARAMETER A SHORT FORM NAME OF THE PLUGIN + "_cvars"
      cvar_storage = std::make_unique<PersistentManagedCVarStorage>(this, "serverpreferrer_cvars", true, true);

      init_cvars();
      init_data();
}

/// <summary>
/// group together the initialization of cvars
/// </summary>
void ServerPreferrer::init_cvars() {
      CVarManager::instance().register_cvars();

#define X(name, ...) cvar_storage->AddCVar(CVarManager::instance().get_cvar_prefix() + #name);
      LIST_OF_PLUGIN_CVARS
#undef X

      CVarManager::instance().get_cvar_enabled().addOnValueChanged([this](std::string oldValue, CVarWrapper newValue) {
            if (newValue.getBoolValue()) {
                  enable_plugin();
            } else {
                  disable_plugin();
            }
      });

      CVarManager::instance().get_cvar_server_ping_threshold().addOnValueChanged(
            [this](std::string oldValue, CVarWrapper newValue) { ping_threshold = newValue.getIntValue(); });

      CVarManager::instance().get_cvar_check_server_ping().addOnValueChanged(
            [this](std::string oldValue, CVarWrapper newValue) { check_server_ping = newValue.getBoolValue(); });

      CVarManager::instance().get_cvar_should_requeue_after_ping_test().addOnValueChanged(
            [this](std::string oldValue, CVarWrapper newValue) {
                  should_requeue_after_cancel = newValue.getBoolValue();
            });

      CVarManager::instance().get_cvar_should_focus_on_success().addOnValueChanged(
            [this](std::string oldValue, CVarWrapper newValue) { should_focus_on_success = newValue.getBoolValue(); });
}

/// <summary>
/// :)
/// </summary>
void ServerPreferrer::enable_plugin() {
      plugin_enabled = true;
      init_hooked_events();
}

/// <summary>
/// :)
/// </summary>
void ServerPreferrer::disable_plugin() {
      plugin_enabled = false;
      HookedEvents::RemoveAllHooks();
}

/// <summary>
/// group together the initialization of hooked events
/// </summary>
void ServerPreferrer::init_hooked_events() {
      HookedEvents::AddHookedEvent("Function TAGame.PlayerController_TA.ReportServer", [](std::string eventName) {
            // add server to the "skipped" list.
      });

      HookedEvents::AddHookedEvent(
            "Function TAGame.GFxData_Matchmaking_TA.StartMatchmaking",
            [this](std::string eventName) {
                  return;
                  if (last_map_command.empty()) {
                        // haven't found a game yet, so nothing to do here yet
                        return;
                  }
                  HookedEvents::AddHookedEvent("Function Engine.DateTime.EpochNow", [this](std::string eventName) {
                        if (check_launch_log(0) && plugin_enabled) {
                              // if we got what we wanted, we'll have a new
                              // server entry which is the server we're trying
                              // to connect to
                              // check_server_connection(server_entries.back());
                        }
                  });
            });

      HookedEvents::AddHookedEvent(
            "Function ProjectX.OnlineGameMatchmakingBase_X.OnSearchComplete",
            [this](std::string eventName) {  // stop reading the end of the Launch.log file
                  HookedEvents::RemoveHook("Function Engine.DateTime.EpochNow");
                  // LOG("log file closed");
            });

      HookedEvents::AddHookedEventWithCaller<ActorWrapper>(
            "Function Engine.GameInfo.EndLogging",
            [this](ActorWrapper unused, void * params, std::string eventName) {
                  log::log_debug("CALLING {} ...", eventName);

                  struct parms {
                        bm_helper::details::FString Reason;
                  } * p = reinterpret_cast<parms *>(params);

                  log::log_debug("PARAM'S LOCATION: {:X}", reinterpret_cast<uintptr_t>(params));
                  if (p == nullptr) {
                        log::log_error("p WAS NULLPTR!!!!");
                        return;
                  }

                  log::log_debug("REASON: {}", p->Reason.ToString());
            },
            true);

      HookedEvents::AddHookedEventWithCaller<ActorWrapper>(
            "Function TAGame.GFxData_DevUtil_TA.Log_Native",
            std::bind(
                  &ServerPreferrer::capture_logging_fn<ActorWrapper>,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3),
            true);

      HookedEvents::AddHookedEventWithCaller<ActorWrapper>(
            "Function TAGame.GFxData_DevUtil_TA.Log",
            std::bind(
                  &ServerPreferrer::capture_logging_fn<ActorWrapper>,
                  this,
                  std::placeholders::_1,
                  std::placeholders::_2,
                  std::placeholders::_3),
            true);
      /*
       * The following hooks use similar mechanisms for capturing the parameter data that
       * has the desired "console command". Unfortunately, (in my opinion), due to
       * something not aligning correctly in bakkesmod, (might be due to a static object
       * not being correctly accounted for, since it was suggested that these functions
       * come from one), alignments for the parameters are slightly off depending on when
       * they're used, usually during a game mode selection. Therefore there are different
       * hooks with different paddings.
       *
       * Engine.Actor.ConsoleCommand is somewhat used separately from its counterpart,
       * even though they perform similar functions... They may as well be the same
       * function.
       */
      HookedEvents::AddHookedEventWithCaller<PlayerControllerWrapper>(
            "Function Engine.PlayerController.ConsoleCommand",
            [this](PlayerControllerWrapper unused, void * params, std::string eventName) {
                  struct ParamType {
                        base_param_type data;
                  };
                  capture_console_fn<ParamType>(unused, params, eventName);
            },
            false);

      HookedEvents::AddHookedEventWithCaller<PlayerControllerWrapper>(
            "Function Engine.PlayerController.ConsoleCommand",
            [this](PlayerControllerWrapper unused, void * params, std::string eventName) {
                  struct ParamType {
                        uint8_t         pad[0x08];
                        base_param_type data;
                  };
                  capture_console_fn<ParamType>(unused, params, eventName);
            },
            true);

      HookedEvents::AddHookedEventWithCaller<PlayerControllerWrapper>(
            "Function Engine.Actor.ConsoleCommand",
            [this](PlayerControllerWrapper unused, void * params, std::string eventName) {
                  struct ParamType {
                        uint8_t         pad[0x18];
                        base_param_type data;
                  };
                  capture_console_fn<ParamType>(unused, params, eventName);
            },
            true);

      HookedEvents::AddHookedEventWithCaller<PlayerControllerWrapper>(
            "Function TAGame.MatchBroadcastComponent_TA.GetLogFileName",
            [this](PlayerControllerWrapper unused, void * params, std::string eventName) {
                  struct ParamType {
                        bm_helper::details::FString ReturnValue;
                        bm_helper::details::FString Filename;
                  } * p = reinterpret_cast<ParamType *>(params);

                  if (p == nullptr) {
                        log::log_error("p WAS NULLPTR!!!!");
                        return;
                  }

                  unsigned char * bytes = reinterpret_cast<unsigned char *>(p);

                  int         i   = static_cast<int>(-16) + 1;
                  int         end = 68 + i;
                  std::string outstr;
                  for (; i <= end; ++i) {
                        outstr += std::format("{:02X} ", bytes[i - 1]);
                        if (i % 8 == 0) {
                              log::log_debug("{}", outstr);
                              outstr.clear();
                        }
                  }
                  log::log_debug("{}", outstr);

                  log::log_debug(
                        L"ReturnValue: {{str:{}, len: {}}}; Filename: {{str:{}, len:{}}}",
                        p->ReturnValue.ToWideString(),
                        p->ReturnValue.length(),
                        p->Filename.ToWideString(),
                        p->Filename.length());
                  log::log_debug(
                        "ReturnValueARRAYDATA ADDR: {:X}, length: {} | FilenameARRAYDATA ADDR: {:X}, length: {}",
                        reinterpret_cast<uintptr_t>(p->ReturnValue.c_str()),
                        p->ReturnValue.length(),
                        reinterpret_cast<uintptr_t>(p->Filename.c_str()),
                        p->Filename.length());
            },
            true);

      HookedEvents::AddHookedEventWithCaller<ActorWrapper>(
            "Function ProjectX.OnlineGameJoinGame_X.SetActiveServerData",
            [this](ActorWrapper caller, void * params, std::string eventName) {
                  using namespace std::chrono_literals;
                  using bm_helper::details::UOnlineGameJoinGame_X;

                  std::this_thread::sleep_for(100ms);
                  log::log_debug("CALLING {}...", eventName);
                  bm_helper::details::FActiveServerData * p =
                        reinterpret_cast<bm_helper::details::FActiveServerData *>(params);
                  log::log_debug("PARAM'S LOCATION: {:X}", reinterpret_cast<uintptr_t>(params));
                  log::log_debug("CALLER'S LOCATION: {:X}", caller.memory_address);

                  if (p == nullptr) {
                        log::log_error("p WAS NULLPTR!!!!");
                        return;
                  }

                  UOnlineGameJoinGame_X * q = reinterpret_cast<UOnlineGameJoinGame_X *>(caller.memory_address);

                  if (q == nullptr) {
                        log::log_error("q WAS NULLPTR!!!!");
                        return;
                  }

                  int             i   = 0;
                  int             end = 0;
                  std::string     outstr {};
                  unsigned char * bytes = nullptr;
                  bytes                 = reinterpret_cast<unsigned char *>(p);
                  i                     = static_cast<int>(-0x100) + 1;
                  end                   = 0x100;
                  for (; i <= end; ++i) {
                        outstr += std::format("{:02X} ", bytes[i - 1]);
                        if (i % 8 == 0) {
                              log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[i - 8]), outstr);
                              outstr.clear();
                        }
                  }
                  log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[end - 8]), outstr);

                  log::log_debug(
                        "STARTING LOCATION (&q->JoinCountdownTime): {:X}",
                        reinterpret_cast<uintptr_t>(&q->JoinCountdownTime));
                  bytes = reinterpret_cast<unsigned char *>(&q->JoinCountdownTime);
                  i     = static_cast<int>(-0x100) + 1;
                  end   = 0x400;
                  for (; i <= end; ++i) {
                        outstr += std::format("{:02X} ", bytes[i - 1]);
                        if (i % 8 == 0) {
                              log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[i - 8]), outstr);
                              outstr.clear();
                        }
                  }
                  log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[end - 8]), outstr);

                  /*
                  eventually:
                              FActiveServerData    ActiveServer
                              FJoinMatchSettings   Settings
                              FCustomMatchSettings CustomMatch
                  */
                  log::log_debug(
                        L"{{[ JoinCountdownTime: {}, "
                        "FailCommand: {{str: {}, len: {}}}, "
                        "LoadingScreenCommand: {{str: {}, len: {}}}, "
                        "WaitingForPlayersString: {{str: {}, len: {}}}, "
                        "ReservationNotRespondingString: {{str: {}, len: {}}}, "
                        "ReservationFullString: {{str: {}, len: {}}}, "
                        "PartyTeamReservationFullString: {{str: {}, len: {}}}, "
                        "NoFriendJoinPrivateMatchString: {{str: {}, len: {}}}, "
                        "BeaconTimedOutString: {{str: {}, len: {}}}, "
                        "NotAllPlayersJoinedString: {{str: {}, len: {}}}, "
                        "CanceledString: {{str: {}, len: {}}}, "
                        "SecurityKeyAcquisitionFailed: {{str: {}, len: {}}}, "
                        "SecurityKeyVerificationFailed: {{str: {}, len: {}}}, "
                        "SendingReservationMessage: {{str: {}, len: {}}}, "
                        "JoiningPartyLeadersGame: {{str: {}, len: {}}}, "
                        "InvalidPassword: {{str: {}, len: {}}}, "
                        "WrongPlaylistString: {{str: {}, len: {}}}, "
                        "WrongRankedMatchString: {{str: {}, len: {}}}, "
                        "MatchEndedString: {{str: {}, len: {}}}, "
                        "CrossplayDisabled: {{str: {}, len: {}}}, "
                        "AnotherPlayerCanceled: {{str: {}, len: {} ]}}",
                        q->JoinCountdownTime,
                        q->FailCommand.ToWideString(),
                        q->FailCommand.length(),
                        q->LoadingScreenCommand.ToWideString(),
                        q->LoadingScreenCommand.length(),
                        q->WaitingForPlayersString.ToWideString(),
                        q->WaitingForPlayersString.length(),
                        q->ReservationNotRespondingString.ToWideString(),
                        q->ReservationNotRespondingString.length(),
                        q->ReservationFullString.ToWideString(),
                        q->ReservationFullString.length(),
                        q->PartyTeamReservationFullString.ToWideString(),
                        q->PartyTeamReservationFullString.length(),
                        q->NoFriendJoinPrivateMatchString.ToWideString(),
                        q->NoFriendJoinPrivateMatchString.length(),
                        q->BeaconTimedOutString.ToWideString(),
                        q->BeaconTimedOutString.length(),
                        q->NotAllPlayersJoinedString.ToWideString(),
                        q->NotAllPlayersJoinedString.length(),
                        q->CanceledString.ToWideString(),
                        q->CanceledString.length(),
                        q->SecurityKeyAcquisitionFailed.ToWideString(),
                        q->SecurityKeyAcquisitionFailed.length(),
                        q->SecurityKeyVerificationFailed.ToWideString(),
                        q->SecurityKeyVerificationFailed.length(),
                        q->SendingReservationMessage.ToWideString(),
                        q->SendingReservationMessage.length(),
                        q->JoiningPartyLeadersGame.ToWideString(),
                        q->JoiningPartyLeadersGame.length(),
                        q->InvalidPassword.ToWideString(),
                        q->InvalidPassword.length(),
                        q->WrongPlaylistString.ToWideString(),
                        q->WrongPlaylistString.length(),
                        q->WrongRankedMatchString.ToWideString(),
                        q->WrongRankedMatchString.length(),
                        q->MatchEndedString.ToWideString(),
                        q->MatchEndedString.length(),
                        q->CrossplayDisabled.ToWideString(),
                        q->CrossplayDisabled.length(),
                        q->AnotherPlayerCanceled.ToWideString(),
                        q->AnotherPlayerCanceled.length());

                  log::log_debug(
                        "ActiveServer LOCATION (&q->ActiveServer): {:X}",
                        reinterpret_cast<uintptr_t>(&q->ActiveServer));
                  log::log_debug(
                        "ActiveServer.Reservation LOCATION (&q->ActiveServer.Reservation): {:X}",
                        reinterpret_cast<uintptr_t>(&q->ActiveServer.Reservation));
                  log::log_debug(
                        "ActiveServer.PingURL LOCATION (&q->ActiveServer.PingURL): {:X}",
                        reinterpret_cast<uintptr_t>(&q->ActiveServer.PingURL));
                  log::log_debug(
                        "ActiveServer.GameURL LOCATION (&q->ActiveServer.GameURL): {:X}",
                        reinterpret_cast<uintptr_t>(&q->ActiveServer.GameURL));
                  log::log_debug(
                        "ActiveServer.JoinCredentials LOCATION (&q->ActiveServer.JoinCredentials): {:X}",
                        reinterpret_cast<uintptr_t>(&q->ActiveServer.JoinCredentials));
                  log::log_debug("Settings LOCATION (&q->Settings): {:X}", reinterpret_cast<uintptr_t>(&q->Settings));
                  log::log_debug(
                        "PendingFailMessage LOCATION (&q->PendingFailMessage): {:X}",
                        reinterpret_cast<uintptr_t>(&q->PendingFailMessage));
                  log::log_debug(
                        "CustomMatch LOCATION (&q->CustomMatch): {:X}",
                        reinterpret_cast<uintptr_t>(&q->CustomMatch));

                  bool is_safe =
                        (q->ActiveServer.JoinCredentials.length() < 512
                         && q->ActiveServer.JoinCredentials.length() > 0);
                  bool is_safe1 = (q->ActiveServer.PingURL.length() < 512 && q->ActiveServer.PingURL.length() > 0);
                  bool is_safe2 = (q->ActiveServer.GameURL.length() < 512 && q->ActiveServer.GameURL.length() > 0);
                  log::log_debug(
                        L"{{[ PingURL: {{str: {}, len: {}}},"
                        "GameURL: {{str: {}, len: {}}},"
                        "JoinCredentials: {{str: {}, len: {}}} ]}}",
                        is_safe1 ? q->ActiveServer.PingURL.ToWideString() : L"NOT SAFE!!!!",
                        q->ActiveServer.PingURL.length(),
                        is_safe2 ? q->ActiveServer.GameURL.ToWideString() : L"NOT SAFE!@#!@#",
                        q->ActiveServer.GameURL.length(),
                        is_safe ? q->ActiveServer.JoinCredentials.ToWideString() : L"NOT SAFE!!!",
                        q->ActiveServer.JoinCredentials.length());

                  is_safe  = (p->JoinCredentials.length() < 512 && p->JoinCredentials.length() > 0);
                  is_safe1 = (p->PingURL.length() < 512 && p->PingURL.length() > 0);
                  is_safe2 = (p->GameURL.length() < 512 && p->GameURL.length() > 0);

                  log::log_debug(
                        L"JoinCredentials: {{str:{}, len: {}}}; PingURL: {{str:{}, len:{}}}; GameURL: {{str:{}, len:{}}}",
                        is_safe ? p->JoinCredentials.ToWideString() : L"NOT SAFE!!!",
                        p->JoinCredentials.length(),
                        is_safe1 ? p->PingURL.ToWideString() : L"NOT SAFE!!!!",
                        p->PingURL.length(),
                        is_safe2 ? p->GameURL.ToWideString() : L"NOT SAFE!@#!@#",
                        p->GameURL.length());
            });

      HookedEvents::AddHookedEventWithCaller<PlayerControllerWrapper>(
            "Function ProjectX.OnlineGameJoinGame_X.StartJoin",
            [this](PlayerControllerWrapper unused, void * params, std::string eventName) {
                  log::log_debug("CALLING {}...POST", eventName);

                  using bm_helper::details::FString;
                  using bm_helper::details::TArray;

                  struct ARGH {
                        FString         a;
                        int32_t         playlist;
                        FString         b;
                        FString         c;
                        FString         d;
                        uint8_t         padding[0x38];
                        TArray<FString> e;
                        // struct FServerReservationData {
                        //       FString ServerName;     // 0x0000 (0x0010)
                        //       [0x0000000000400000] (CPF_NeedCtorLink) int32_t Playlist;
                        //       // 0x0010 (0x0004) [0x0000000000000000] FString Region;
                        //       // 0x0018 (0x0010) [0x0000000000400000]
                        //       (CPF_NeedCtorLink) FString ReservationID;  // 0x0028
                        //       (0x0010) [0x0000000000400000] (CPF_NeedCtorLink) FString
                        //       DSRToken;       // 0x0038 (0x0010) [0x0000000000400000]
                        //       (CPF_NeedCtorLink)
                        //       // UNetworkEncryptionKey * Keys;           // 0x0048
                        //       (0x0008) [0x0000000000000000] uint8_t padding[0x38];
                        //       FString JoinName;  // 0x0050 (0x0010)
                        //       [0x0000000000400000] (CPF_NeedCtorLink) uint8_t
                        //       pad2[0x8]; FString JoinPassword;  // 0x0060 (0x0010)
                        //       [0x0000000000400000] (CPF_NeedCtorLink)
                        // } RESERVATION;

                        // struct FJoinMatchSettings {
                        //       uint8_t MatchType;  // 0x0000 (0x0001)
                        //       [0x0000000000000000] FString one, two, three;
                        //       // int32_t       PlaylistId;            // 0x0004
                        //       (0x0004) [0x0000000000000000]
                        //       // uint32_t      bFriendJoin      : 1;  // 0x0008
                        //       (0x0004) [0x0000000000000000]
                        //       // [0x00000001] uint32_t      bMigration       : 1;  //
                        //       0x0008 (0x0004)
                        //       // [0x0000000000000000] [0x00000002] uint32_t
                        //       bRankedReconnect : 1;  // 0x0008
                        //       // (0x0004) [0x0000000000000000] [0x00000004] class
                        //       FString Password;  // 0x0010 (0x0010)
                        //       // [0x0000000000400000] (CPF_NeedCtorLink)
                        // } JOINSETTINGS;        // 0x0070 (0x0020) [0x0000000000400090]
                        //                        // (CPF_OptionalParm | CPF_Parm |
                        //                        CPF_NeedCtorLink)
                        // bool ReturnValue : 1;  // 0x0090 (0x0004) [0x0000000000000580]
                        // [0x00000001] (CPF_Parm |
                        //                        // CPF_OutParm
                        //                        // // | CPF_ReturnParm)
                        // struct FActiveServerData {
                        //       FServerReservationData
                        //               Reservation;      // 0x0000 (0x0070)
                        //               [0x0000000000400000] (CPF_NeedCtorLink)
                        //       FString PingURL;          // 0x0070 (0x0010)
                        //       [0x0000000000400000] (CPF_NeedCtorLink) FString GameURL;
                        //       // 0x0080 (0x0010) [0x0000000000400000]
                        //       (CPF_NeedCtorLink) FString JoinCredentials;  // 0x0090
                        //       (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
                        // } NEWACTIVESERVER;              // 0x0098 (0x00A0)
                        // [0x0000000000400000] (CPF_NeedCtorLink)
                  };

                  ARGH * p = reinterpret_cast<ARGH *>(params);

                  log::log_debug("PARAM'S LOCATION: {:X}", reinterpret_cast<uintptr_t>(params));
                  if (p == nullptr) {
                        log::log_error("p WAS NULLPTR!!!!");
                        return;
                  }

                  /*
                   * THIS IS LIKE, MY BEST BET AT A DAMN THING!!! ARGH!!!!!
                   *
                   * HAVE TO CHECK ALL OF THE ADDRESS SPACE!!!!
                   *
                   *
                   *
                   *
                   *
                   *
                   */

                  unsigned char * bytes = reinterpret_cast<unsigned char *>(p);

                  int         i   = static_cast<int>(-0x100) + 1;
                  int         end = 0x100;
                  std::string outstr;
                  for (; i <= end; ++i) {
                        outstr += std::format("{:02X} ", bytes[i - 1]);
                        if (i % 8 == 0) {
                              log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[i - 8]), outstr);
                              outstr.clear();
                        }
                  }
                  log::log_debug("{:X} {}", reinterpret_cast<uintptr_t>(&bytes[end - 8]), outstr);

                  log::log_debug(
                        L"a {{str: {}, len: {}}} | b {{str: {}, len: {}}} | c {{str: {}, len: {}}} | d {{str: {}, len: {}}}",
                        p->a.ToWideString(),
                        p->a.length(),
                        p->b.ToWideString(),
                        p->b.length(),
                        p->c.ToWideString(),
                        p->c.length(),
                        p->d.ToWideString(),
                        p->d.length());

                  // log::log_debug(
                  //       "NEW ACTIVE SERVER'S LOCATION: {:X}",
                  //       reinterpret_cast<uintptr_t>(&p->NEWACTIVESERVER));

                  // bool is_safe =
                  //       (p->NEWACTIVESERVER.JoinCredentials.length() < 512
                  //        && p->NEWACTIVESERVER.JoinCredentials.length() > 0);
                  // bool is_safe1 =
                  //       (p->NEWACTIVESERVER.PingURL.length() < 512 &&
                  //       p->NEWACTIVESERVER.PingURL.length() > 0);
                  // bool is_safe2 =
                  //       (p->NEWACTIVESERVER.GameURL.length() < 512 &&
                  //       p->NEWACTIVESERVER.GameURL.length() > 0);

                  // log::log_debug(
                  //       L"JoinCredentials: {{str:{}, len: {}}}; PingURL: {{str:{},
                  //       len:{}}}; GameURL: {{str:{}, len:{}}}", is_safe ?
                  //       p->NEWACTIVESERVER.JoinCredentials.ToWideString() : L"NOT
                  //       SAFE!!!", p->NEWACTIVESERVER.JoinCredentials.length(), is_safe1
                  //       ? p->NEWACTIVESERVER.PingURL.ToWideString() : L"NOT SAFE!!!!",
                  //       p->NEWACTIVESERVER.PingURL.length(),
                  //       is_safe2 ? p->NEWACTIVESERVER.GameURL.ToWideString() : L"NOT
                  //       SAFE!@#!@#", p->NEWACTIVESERVER.GameURL.length());

                  // ARGH::FServerReservationData ugh      =
                  // p->NEWACTIVESERVER.Reservation; bool                         is_safe3
                  // = ugh.ServerName.length() > 0 && ugh.ServerName.length() < 512;

                  // log::log_debug(
                  //       L"RESERVATION SERVERNAME: {{str: {}, len: {}}} | ",
                  //       is_safe3 ? ugh.ServerName.ToWideString() : L"NO SAEFO",
                  //       ugh.ServerName.length());

                  // ARGH::FJoinMatchSettings cmon = p->JOINSETTINGS;
                  // log::log_debug("JOINSETTINGS'S LOCATION: {:X}",
                  // reinterpret_cast<uintptr_t>(&p->JOINSETTINGS)); bool is_safe4 =
                  // cmon.one.length() > 0 && cmon.one.length() < 512; bool is_safe5 =
                  // cmon.two.length() > 0 && cmon.two.length() < 512; bool is_safe6 =
                  // cmon.three.length() > 0 && cmon.three.length() < 512;

                  // log::log_debug(
                  //       L"ONEOHCMON: {{str: {}, len: {}}} | "
                  //       L"TWOOHCMON: {{str: {}, len: {}}} | "
                  //       L"THREEEOHCMON: {{str: {}, len: {}}} | ",
                  //       is_safe4 ? cmon.one.ToWideString() : L"NO SAEFO",
                  //       cmon.one.length(),
                  //       is_safe5 ? cmon.two.ToWideString() : L"NO SAEFO",
                  //       cmon.two.length(),
                  //       is_safe6 ? cmon.three.ToWideString() : L"NO SAEFO",
                  //       cmon.three.length());
            });

      HookedEvents::AddHookedEvent("Function Engine.GameInfo.PreExit", [this](std::string eventName) {
            // ASSURED CLEANUP
            onUnload();
      });

      log::log_debug("hooked");
}

template <
      typename ParamType,
      typename CallerWrapper,
      typename std::enable_if_t<std::is_base_of_v<ObjectWrapper, CallerWrapper>> *>
inline void ServerPreferrer::capture_console_fn(CallerWrapper cw, void * params, std::string eventName) {
      log::log_info("CALLING!!! {}...", eventName);

      ParamType * p = reinterpret_cast<ParamType *>(params);
      // debugging this console command
      log::log_debug("PARAM'S LOCATION: {:X}", reinterpret_cast<uintptr_t>(params));
      if (p == nullptr) {
            log::log_error("p WAS NULLPTR!!!!");
            return;
      }

      unsigned char * bytes = reinterpret_cast<unsigned char *>(p);

      int         i   = static_cast<int>(sizeof(ParamType) - sizeof(base_param_type)) + 1;
      int         end = 20 + i;
      std::string outstr;
      for (; i <= end; ++i) {
            outstr += std::format("{:02X} ", bytes[i - 1]);
            if (i % 8 == 0) {
                  log::log_debug("{}", outstr);
                  outstr.clear();
            }
      }
      log::log_debug("{}", outstr);

      /*
       * I WOULD LIKE A BETTER WAY TO VERIFY THAT THIS IS "SAFE", LIKE, "IS THIS IN ROCKET
       * LEAGUE'S ACTUAL MEMORY SPACE"
       * ... ... I just can't guarantee that trying to read random data in certain
       * sections will be the same on every machine :'(
       */
      bool is_safe = (p->data.command.length() < 512 && p->data.command.length() > 0);
      log::log_debug(
            L"cmd: {{{}, size: {}}}, write_to_log: {}",
            is_safe ? p->data.command.ToWideString() : L"NOTSAFE",
            p->data.command.length(),
            static_cast<bool>(p->data.write_to_log));
      log::log_debug(
            "ARRAYDATA ADDR: {:X}, length: {}, write_to_log: {}",
            reinterpret_cast<uintptr_t>(p->data.command.c_str()),
            p->data.command.length(),
            static_cast<bool>(p->data.write_to_log));

      log::log_debug(
            "DIFFERENCE BETWEEN PARAM LOCATION AND ARRAYDATA LOCATION: {:X}",
            static_cast<ptrdiff_t>(
                  reinterpret_cast<uintptr_t>(p->data.command.c_str()) - reinterpret_cast<uintptr_t>(params)));

      if (is_safe) {
            last_map_command     = p->data.command.ToString();
            std::string strstart = last_map_command.substr(0, 5)
                                   | std::views::transform([](unsigned char c) { return std::toupper(c); })
                                   | std::ranges::to<std::string>();
            log::log_debug("strstart: \"{}\"", strstart);
            if (strstart != "START" && strstart != "OPEN ") {
                  last_map_command = "start " + last_map_command;
            }
            log::log_debug("last_map_command: {}", last_map_command);
      }
}

template <typename CallerWrapper, typename std::enable_if_t<std::is_base_of_v<ObjectWrapper, CallerWrapper>> *>
inline void ServerPreferrer::capture_logging_fn(CallerWrapper cw, void * params, std::string eventName) {
      log::log_debug("CALLING {} ...", eventName);

      struct parms {
            bm_helper::details::FString Category;
            bm_helper::details::FString Message;
      } * p = reinterpret_cast<parms *>(params);

      // debugging this hook - checking the alignment
      log::log_debug("PARAM'S LOCATION: {:X}", reinterpret_cast<uintptr_t>(params));
      if (p == nullptr) {
            log::log_error("p WAS NULLPTR!!!!");
            return;
      }

      unsigned char * bytes = reinterpret_cast<unsigned char *>(p);

      int         i   = static_cast<int>(0) + 1;
      int         end = 50 + i;
      std::string outstr;
      for (; i <= end; ++i) {
            outstr += std::format("{:02X} ", bytes[i - 1]);
            if (i % 8 == 0) {
                  log::log_debug("{}", outstr);
                  outstr.clear();
            }
      }
      log::log_debug("{}", outstr);

      log::log_debug(
            L"CATEGORY: {{str:{}, len: {}}}; MESSAGE: {{str:{}, len:{}}}",
            p->Category.ToWideString(),
            p->Category.length(),
            p->Message.ToWideString(),
            p->Message.length());
      log::log_debug(
            "CATEGORYARRAYDATA ADDR: {:X}, length: {} | MESSAGEARRAYDATA ADDR: {:X}, length: {}",
            reinterpret_cast<uintptr_t>(p->Category.c_str()),
            p->Category.length(),
            reinterpret_cast<uintptr_t>(p->Message.c_str()),
            p->Message.length());
}

void ServerPreferrer::init_data() {
}

/// <summary>
/// Check the Launch.log file for a certain match
///
/// Adds to the server entries if there's a match.
/// </summary>
/// <param name="start_read">Where to start reading in the file.</param>
/// <returns>True if match is met. False if there's no match.</returns>
bool ServerPreferrer::check_launch_log(std::streamoff start_read) {
      gameWrapper->ExecuteUnrealCommand("flushlog");
      std::string line;

      std::regex server_info_match;
      try {
            server_info_match = std::regex {
                  ".*ServerName=\"([a-zA-Z0-9\\-]+)\""
                  ".*Playlist=([0-9]+)"
                  ".*Region=\"([a-zA-Z0-9\\-]+)\""
                  ".*PingURL=\"([0-9.:\\-]+)\""
                  ".*GameURL=\"([0-9.:\\-]+)\""
                  ".*",
                  std::regex::extended | std::regex::icase};
      } catch (const std::exception & e) {
            log::log_error("REGEX ERROR? HELP! {}", e.what());
            return false;
      }

      while (false) {
            std::smatch re_match;
            if (std::regex_match(line, re_match, server_info_match)) {
                  if (re_match.size() == 6) {
                        using namespace std::chrono;

                        // A MATCH to the needed line!
                        // 6 because the first "match" is basically the entire line.
                        std::ssub_match server_name {re_match[1]};
                        std::ssub_match server_playlist {re_match[2]};
                        std::ssub_match server_region {re_match[3]};
                        std::ssub_match server_pingurl {re_match[4]};
                        std::ssub_match server_gameurl {re_match[5]};

                        server_entries.emplace_back(server_info {
                              .tp = zoned_seconds {current_zone(), time_point_cast<seconds>(system_clock::now())},
                              .server_name = server_name.str(),
                              .playlist_id = server_playlist.str(),
                              .region      = server_region.str(),
                              .ping_url    = server_pingurl.str(),
                              .game_url    = server_gameurl.str()
                        });
                        const server_info & sv = server_entries.back();
                        log::log_debug(
                              "TIME: {}, SERVER_NAME: {}, PLAYLIST_ID: {}, REGION: "
                              "{}, "
                              "PING_URL: {}, GAME_URL: "
                              "{}",
                              std::vformat(DATETIME_FORMAT_STR, std::make_format_args(sv.tp)),
                              sv.server_name,
                              sv.playlist_id,
                              sv.region,
                              sv.ping_url,
                              sv.game_url);
                        return true;
                  }
            }
      }
      last_map_command.clear();
      return false;
}

std::expected<bool, CONNECTION_STATUS> ServerPreferrer::is_good_ping_icmp(std::string pingaddr, int times) {
      // https://learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho#examples

      // do the pings
      HANDLE        hIcmpFile;
      unsigned long ip_addr  = INADDR_NONE;
      DWORD         dwRetVal = 0;

      inet_pton(AF_INET, pingaddr.c_str(), reinterpret_cast<void *>(&ip_addr));
      if (ip_addr == INADDR_NONE) {
            return std::unexpected(CONNECTION_STATUS::INET_PTON_FAILURE);
      }

      hIcmpFile = IcmpCreateFile();
      if (hIcmpFile == INVALID_HANDLE_VALUE) {
            return std::unexpected(CONNECTION_STATUS::IcmpCreateFile_INVALID_HANDLE_VALUE_FAILURE);
      }

      char   SendData[1] = {'\0'};
      LPVOID ReplyBuffer = NULL;
      DWORD  ReplySize   = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
      ReplyBuffer        = (VOID *)malloc(ReplySize);

      if (ReplyBuffer == NULL) {
            return std::unexpected(CONNECTION_STATUS::NO_MEMORY_REPLY_BUFFER);
      }

      int avg = 0;
      int i   = 1;
      for (int j = 1; j <= times; ++j) {
            dwRetVal = IcmpSendEcho(hIcmpFile, ip_addr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
            if (dwRetVal != 0) {
                  PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
                  char             addr[32]   = {0};
                  inet_ntop(AF_INET, reinterpret_cast<void *>(&(pEchoReply->Address)), addr, 32);
                  avg += pEchoReply->RoundTripTime;
            } else {
                  free(ReplyBuffer);
                  return std::unexpected(CONNECTION_STATUS::ICMP_PING_CALL_FAILURE);
            }
            i = j;
      }

      free(ReplyBuffer);

      //

      return (avg / i);
}

/*
* Unused for right now.
void try_tcp_ping(std::string pingaddr, std::string port) {
        // TRY TO CONNECT WITH TCP!~
        WSADATA wsaData;
        int     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
                LOG("ERROR INITIALIZING WINSOCK");
                return;
        }

        SOCKET ConnectSocket;
        ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ConnectSocket == INVALID_SOCKET) {
                LOG("CONNECT SOCKET IS INVALID!");
                WSACleanup();
                return;
        }

        sockaddr_in connection;
        connection.sin_family = AF_INET;
        inet_pton(AF_INET, pingaddr.c_str(), reinterpret_cast<void
*>(&connection.sin_addr.s_addr)); connection.sin_port = htons(std::stoi(port));

        for (int i = 0; i < 5; ++i) {
                // do 5 connection runs
                auto t1 = std::chrono::high_resolution_clock::now();

                iResult = connect(ConnectSocket, reinterpret_cast<SOCKADDR
*>(&connection), sizeof(connection)); if (iResult == SOCKET_ERROR) { LOG("connect failed
with error: {}", WSAGetLastError()); iResult = closesocket(ConnectSocket); if (iResult
== SOCKET_ERROR) { LOG("CLOSE SOCKET FAILED??: {}", WSAGetLastError());
                        }
                        WSACleanup();
                        break;
                }

                LOG("CONNECTED!");

                iResult = closesocket(ConnectSocket);
                if (iResult == SOCKET_ERROR) {
                        LOG("CLOSE SOCKET FAILED??: {}", WSAGetLastError());
                        WSACleanup();
                        break;
                }

                auto t2 = std::chrono::high_resolution_clock::now();
                LOG("THAT RUN ({}) TOOK {} MILLISECONDS!",
                    std::chrono::duration_cast<std::chrono::milliseconds>(t2 -
t1).count());
        }

        WSACleanup();
}
*/

/**
 * \brief okay
 *
 * \param playid The ID of the game's playlist
 *
 */
std::expected<bool, CONNECTION_STATUS> ServerPreferrer::is_valid_game_mode(PlaylistId playid) {
      // big assumption this playlist id is an actual playlist id

      // if (std::ranges::contains(
      //             bm_helper::playlist_categories["Tournament"],
      //             playid)  // you have no ability to control the servers you join
      //             in a tournament
      //     || std::ranges::contains(
      //             bm_helper::playlist_categories["Private Match"],
      //             playid)) {  // - [8988.26] JoinGame: private match travel
      //             immediately!
      //         return;
      // }
      // the statements above are the same, just logically opposite.
      if (!(std::ranges::contains(bm_helper::playlist_categories.at("Casual"), playid)
            || std::ranges::contains(bm_helper::playlist_categories.at("Competitive"), playid))) {
            // only works for casual and competitive playlists (afaik right now)
            return false;
      }
      return true;
}

static std::expected<int, CONNECTION_STATUS> time_icmp_ping(std::string pingaddr, int times) {
      // https://learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho#examples
      HANDLE        hIcmpFile;
      unsigned long ip_addr  = INADDR_NONE;
      DWORD         dwRetVal = 0;

      inet_pton(AF_INET, pingaddr.c_str(), reinterpret_cast<void *>(&ip_addr));
      if (ip_addr == INADDR_NONE) {
            return std::unexpected(CONNECTION_STATUS::INET_PTON_FAILURE);
      }

      hIcmpFile = IcmpCreateFile();
      if (hIcmpFile == INVALID_HANDLE_VALUE) {
            return std::unexpected(CONNECTION_STATUS::IcmpCreateFile_INVALID_HANDLE_VALUE_FAILURE);
      }

      char   SendData[1] = {'\0'};
      LPVOID ReplyBuffer = NULL;
      DWORD  ReplySize   = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
      ReplyBuffer        = (VOID *)malloc(ReplySize);

      if (ReplyBuffer == NULL) {
            return std::unexpected(CONNECTION_STATUS::NO_MEMORY_REPLY_BUFFER);
      }

      int avg = 0;
      int i   = 1;
      for (int j = 1; j <= times; ++j) {
            dwRetVal = IcmpSendEcho(hIcmpFile, ip_addr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
            if (dwRetVal != 0) {
                  PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
                  char             addr[32]   = {0};
                  inet_ntop(AF_INET, reinterpret_cast<void *>(&(pEchoReply->Address)), addr, 32);
                  avg += pEchoReply->RoundTripTime;
            } else {
                  free(ReplyBuffer);
                  return std::unexpected(CONNECTION_STATUS::ICMP_PING_CALL_FAILURE);
            }
            i = j;
      }

      free(ReplyBuffer);
      return (avg / i);
}

/// <summary>
/// fucking separate this...
/// </summary>
/// <param name="server"></param>
/// <param name="threshold"></param>
// void ServerPreferrer::check_server_connection(server_info server) {
//       // throw a thread outside of the game's thread so it doesn't lag the game.
//       std::string pingaddr = server.ping_url;
//       pingaddr             = pingaddr.substr(0, pingaddr.find(":"));
//
//       /*
//        * NEED TO THINK ABOUT THIS SECTION LATER UGGHHHHHHHHHHH
//        */
//
//       std::vector<std::future<std::expected<bool, CONNECTION_STATUS>>> checks;
//
//       if (check_server_ping) {
//             checks.emplace_back(std::async(std::launch::async, time_icmp_ping,
//             pingaddr, 5));
//       }
//
//       checks.emplace_back(std::async(
//             std::launch::async,
//             &ServerPreferrer::is_valid_game_mode,
//             this,
//             static_cast<PlaylistId>(std::stoi(server.playlist_id))));
//
//       // this is purely for checking for success
//       HookedEvents::AddHookedEvent(
//             "Function ProjectX.GFxEngine_X.Tick",
//             [&](...) {
//                   for (auto & check : checks) {
//                         std::future_status fs;
//                         using namespace std::chrono_literals;
//                         fs = check.wait_for(
//                               // std::chrono::duration<std::chrono::seconds::rep,
//                               std::chrono::seconds::period>
//                               // {0});
//                               0s);
//                         if (fs == std::future_status::ready) {
//                               // KEEP GOING !
//                               if (!check.get().has_value()) {
//                                     // ERROR OCCURRED!
//                                     //  QUIT!
//                               }
//                         }
//                   }
//
//                   log::LOG(
//                         "HAVE VALUE YET?: {}, THRESHOLD WTF?: {}, WHERERE MY "
//                         "VARIABLES?: "
//                         "{}",
//                         false,
//                         ping_threshold,
//                         should_requeue_after_cancel);
//
//                   int ping = 0;  // OH BOY ~fix this~
//                   // report
//                   if (ping < 0) {
//                         // an error occurred
//                         log::LOG(
//                               "ERROR WHILE RUNNING PING TEST: {}",
//                               ping == -1   ? "INET_PTON FAILURE"
//                               : ping == -2 ? "IcmpCreateFile INVALID_HANDLE_VALUE
//                               FAILURE" : ping == -3 ? "UNABLE TO CREATE MEMORY FOR
//                               REPLY BUFFER" : ping == -4 ? "ICMP PING CALL FAILURE"
//                                            : "UNKNOWN");
//                   } else {
//                         // success?
//                         auto str = std::format(
//                               "AVERAGE PING {0:c} THRESHOLD, {1} "
//                               "{0:c} {2}",
//                               ping > ping_threshold   ? '>'
//                               : ping < ping_threshold ? '<'
//                                                       : '=',
//                               ping,
//                               ping_threshold);
//
//                         log::LOG(str);
//                         gameWrapper->LogToChatbox(str, "SP");
//                   }
//
//                   // finally, if an error occurred or ping above the threshold
//                   if (ping < 0 || ping > ping_threshold) {
//                         // TURN OFF THE GAME'S FLASHING TASKBAR ICON!
//                         // because the player didn't join a game, so don't flash the
//                         taskbar
//                         // like they did
//                         FLASHWINFO stop_flashing {
//                               .cbSize    = sizeof(FLASHWINFO),
//                               .hwnd      = rl_hwnd,
//                               .dwFlags   = FLASHW_STOP,
//                               .uCount    = 0,
//                               .dwTimeout = 0};
//                         FlashWindowEx(&stop_flashing);
//
//                         gameWrapper->Execute([this](GameWrapper * gw) {
//                         cvarManager->executeCommand("queue_cancel");
//                         });
//
//                         if (should_requeue_after_cancel) {
//                               gameWrapper->Execute([this](GameWrapper * gw) {
//                               cvarManager->executeCommand("queue");
//                               });
//                         }
//                   }
//
//                   if (should_focus_on_success) {
//                         // idk if I need to check for states to determine what nCmdShow
//                         should be...
//                         // if (IsIconic(rl_hwnd)) {
//                         //      // means the rocket league window is minimized
//                         //      ShowWindowAsync(rl_hwnd, SW_RESTORE);
//                         //}
//                         ShowWindowAsync(rl_hwnd, SW_RESTORE);
//                   }
//
//                   // remove the hook that checks for the ping update.
//                   HookedEvents::RemoveHook("Function ProjectX.GFxEngine_X.Tick");
//             },
//             true);
//       /*** end check for ping ***/
//       // write_out_server_attempt();
// }

/// <summary>
/// WHY? BECAUSE .front() ON AN EMPTY DEQUE IS UB
/// https://en.cppreference.com/w/cpp/container/deque/front
/// AND I WOULD RATHER IT RETURN AN EMPTY ENTRY
/// WHEN ASSUMEDLY GETTING THE FIRST ENTRY (SOMETIMES)
///
/// THIS IS SOLELY TO ADDRESS WHEN THE BANK IS EMPTY
/// </summary>
ServerPreferrer::server_info ServerPreferrer::get_first_server_entry() {
      if (server_entries.empty()) {
            return server_info {};
      }

      return server_entries.front();
}

/// <summary>
/// WHY? BECAUSE .back() ON AN EMPTY DEQUE IS UB
/// https://en.cppreference.com/w/cpp/container/deque/back
/// AND I WOULD RATHER IT RETURN AN EMPTY ENTRY
/// WHEN ASSUMEDLY GETTING THE LAST ENTRY
///
/// THIS IS SOLELY TO ADDRESS WHEN THE BANK IS EMPTY
/// </summary>
ServerPreferrer::server_info ServerPreferrer::get_last_server_entry() {
      if (server_entries.empty()) {
            return server_info {};
      }

      return server_entries.back();
}

/// <summary>
/// RETURNS THE DATETIME AS A STRING!
/// </summary>
/// <returns>_now_ represented as a datetime string</returns>
std::string ServerPreferrer::get_current_datetime_str() {
      using namespace std::chrono;

      const zoned_seconds now {current_zone(), time_point_cast<seconds>(system_clock::now())};
      return std::vformat(DATETIME_FORMAT_STR, std::make_format_args(now));
}

/// <summary>
/// RETURNS THE TIMEPOINT AS INTERPRETED FROM A DATETIME STRING!
/// </summary>
/// <param name="str">a string representation of the datetime</param>
/// <returns>a time_point representing the string</returns>
std::chrono::zoned_seconds ServerPreferrer::get_timepoint_from_str(std::string str) {
      using namespace std::chrono;

      sys_time<seconds>  tmpd;
      std::istringstream ss(str);
      ss >> parse(DATETIME_PARSE_STR, tmpd);
      return zoned_seconds {current_zone(), tmpd};
}

/// <summary>
/// https://mastodon.gamedev.place/@dougbinks/99009293355650878
/// </summary>
static inline void AddUnderline(ImColor col_) {
      ImVec2 min = ImGui::GetItemRectMin();
      ImVec2 max = ImGui::GetItemRectMax();
      min.y      = max.y;
      ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
}

/// <summary>
/// taken from https://gist.github.com/dougbinks/ef0962ef6ebe2cadae76c4e9f0586c69
/// "hyperlink urls"
/// </summary>
static inline void TextURL(  // NOLINT
      const char * name_,
      const char * URL_,
      uint8_t      SameLineBefore_,
      uint8_t      SameLineAfter_) {
      if (1 == SameLineBefore_) {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
      }
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 165, 255, 255));
      ImGui::Text("%s", name_);
      ImGui::PopStyleColor();
      if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseClicked(0)) {
                  // What if the URL length is greater than int but less than
                  // size_t? well then the program should crash, but this is fine.
                  const int nchar =
                        std::clamp(static_cast<int>(std::strlen(URL_)), 0, (std::numeric_limits<int>::max)() - 1);
                  wchar_t * URL = new wchar_t[nchar + 1];
                  wmemset(URL, 0, nchar + 1);
                  MultiByteToWideChar(CP_UTF8, 0, URL_, nchar, URL, nchar);
                  ShellExecuteW(NULL, L"open", URL, NULL, NULL, SW_SHOWNORMAL);

                  delete[] URL;
            }
            AddUnderline(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            ImGui::SetTooltip("  Open in browser\n%s", URL_);
      } else {
            AddUnderline(ImGui::GetStyle().Colors[ImGuiCol_Button]);
      }
      if (1 == SameLineAfter_) {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
      }
}

/// <summary>
/// This call usually includes ImGui code that is shown and rendered (repeatedly,
/// on every frame rendered) when your plugin is selected in the plugin
/// manager. AFAIK, if your plugin doesn't have an associated *.set file for its
/// settings, this will be used instead.
/// </summary>
void ServerPreferrer::RenderSettings() {
      bool plugin_enabled = CVarManager::instance().get_cvar_enabled().getBoolValue();
      if (ImGui::Checkbox("Enable Plugin", &plugin_enabled)) {
            CVarManager::instance().get_cvar_enabled().setValue(plugin_enabled);
      }

      ImGuiTabBarFlags tbf = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton;
      if (ImGui::BeginTabBar("##tabbar", tbf)) {
            if (ImGui::BeginTabItem("Options", 0, ImGuiTabItemFlags_NoCloseButton | ImGuiTabItemFlags_SetSelected)) {
                  ImGui::TextUnformatted("I'm in a tab!");
                  int threshold = CVarManager::instance().get_cvar_server_ping_threshold().getIntValue();
                  if (ImGui::SliderInt("Maximum allowed ping to server.", &threshold, 0, 400)) {
                        threshold = std::clamp(threshold, 0, 400);
                        CVarManager::instance().get_cvar_server_ping_threshold().setValue(threshold);
                  }

                  bool check_ping = CVarManager::instance().get_cvar_check_server_ping().getBoolValue();
                  if (ImGui::Checkbox("Check the server ping before connecting?", &check_ping)) {
                        CVarManager::instance().get_cvar_check_server_ping().setValue(check_ping);
                  }

                  bool should_requeue =
                        CVarManager::instance().get_cvar_should_requeue_after_ping_test().getBoolValue();
                  if (ImGui::Checkbox("Should requeue automatically after failing ping test?", &should_requeue)) {
                        CVarManager::instance().get_cvar_should_requeue_after_ping_test().setValue(should_requeue);
                  }

                  bool should_focus = CVarManager::instance().get_cvar_should_focus_on_success().getBoolValue();
                  if (ImGui::Checkbox("Should focus Rocket League on success?", &should_focus)) {
                        CVarManager::instance().get_cvar_should_focus_on_success().setValue(should_requeue);
                  }
                  ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
      }
}

/// <summary>
/// "SetImGuiContext happens when the plugin's ImGui is initialized."
/// https://wiki.bakkesplugins.com/imgui/custom_fonts/
///
/// also:
/// "Don't call this yourself, BM will call this function with a pointer
/// to the current ImGui context" -- pluginsettingswindow.h
/// ...
///
/// so �\(�_o)/�
/// </summary>
/// <param name="ctx">AFAIK The pointer to the ImGui context</param>
void ServerPreferrer::SetImGuiContext(uintptr_t ctx) {
      ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext *>(ctx));
}

/// <summary>
/// Get the name of the plugin for the plugins tab in bakkesmod
/// </summary>
std::string ServerPreferrer::GetPluginName() {
      return "Server Preferrer";
}

/*
 * for when you've inherited from BakkesMod::Plugin::PluginWindow.
 * this lets  you do "togglemenu (GetMenuName())" in BakkesMod's console...
 * ie
 * if the following GetMenuName() returns "xyz", then you can refer to your
 * plugin's window in game through "togglemenu xyz"
 */

/*
/// <summary>
/// do the following on togglemenu open
/// </summary>
void ServerPreferrer::OnOpen() {};

/// <summary>
/// do the following on menu close
/// </summary>
void ServerPreferrer::OnClose() {};

/// <summary>
/// (ImGui) Code called while rendering your menu window
/// </summary>
void ServerPreferrer::Render() {};

/// <summary>
/// Returns the name of the menu to refer to it by
/// </summary>
/// <returns>The name used refered to by togglemenu</returns>
std::string ServerPreferrer::GetMenuName() {
        return "$safeprojectname";
};

/// <summary>
/// Returns a std::string to show as the title
/// </summary>
/// <returns>The title of the menu</returns>
std::string ServerPreferrer::GetMenuTitle() {
        return "ServerPreferrer";
};

/// <summary>
/// Is it the active overlay(window)?
/// </summary>
/// <returns>True/False for being the active overlay</returns>
bool ServerPreferrer::IsActiveOverlay() {
        return true;
};

/// <summary>
/// Should this block input from the rest of the program?
/// (aka RocketLeague and BakkesMod windows)
/// </summary>
/// <returns>True/False for if bakkesmod should block input</returns>
bool ServerPreferrer::ShouldBlockInput() {
        return false;
};
*/

/// <summary>
///  do the following when your plugin is unloaded
///
///  destroy things here, don't throw
///  don't rely on this to assuredly run when RL is closed
/// </summary>
void ServerPreferrer::onUnload() {
}
