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
                  log::log_debug("MATCHMAKING HAS STARTED! NOW CHECKING!");
                  HookedEvents::AddHookedEventWithCaller<ActorWrapper>(
                        "Function ProjectX.OnlineGameJoinGame_X.EventActiveServerChanged",
                        [this](ActorWrapper caller, void * params, std::string eventName) {
                              log::log_debug("CALLING {}...POST", eventName);

                              using OGJG = bm_helper::details::UOnlineGameJoinGame_X;
                              OGJG * p   = reinterpret_cast<OGJG *>(caller.memory_address);

                              log::log_debug("THE GOD DAMN SERVER INFORMATION WAS CAPTURED!");
                              log::log_debug("{}", *p);

                              if (p->ActiveServer.PingURL.length() != 0) {  // WE HAVE THE INFORMATION!
                                    // check it
                                    using namespace std::chrono;
                                    server_info info {
                                          .tp =
                                                zoned_seconds {
                                                               current_zone(),
                                                               time_point_cast<seconds>(system_clock::now())},
                                          .server_name = p->ActiveServer.Reservation.ServerName.ToString(),
                                          .playlist_id = std::to_string(p->ActiveServer.Reservation.Playlist),
                                          .region      = p->ActiveServer.Reservation.Region.ToString(),
                                          .ping_url    = p->ActiveServer.PingURL.ToString(),
                                          .game_url    = p->ActiveServer.GameURL.ToString()
                                    };
                                    // server_entries.emplace_back(info);
                                    check_server_connection(info);
                              }
                        });
            });

      HookedEvents::AddHookedEvent(
            "Function ProjectX.OnlineGameMatchmakingBase_X.OnSearchComplete",
            [this](std::string eventName) {
                  // stop operations
                  log::log_debug("DONE CHECKING, LOL!");
                  HookedEvents::RemoveHook("Function ProjectX.OnlineGameJoinGame_X.EventActiveServerChanged");
            });

      HookedEvents::AddHookedEvent("Function Engine.GameInfo.PreExit", [this](std::string eventName) {
            // ASSURED CLEANUP
            onUnload();
      });

      log::log_debug("hooked");
}

void ServerPreferrer::init_data() {
}

/**
 * @brief
 * @param pingaddr
 * @param times
 * @return
 */
ServerPreferrer::test_t ServerPreferrer::is_good_ping_icmp(std::string pingaddr, int times) {
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
ServerPreferrer::test_t ServerPreferrer::is_valid_game_mode(PlaylistId playid) {
      log::log_debug("TESTING THAT GAME MODE!");
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
      log::log_debug("TESTING THAT PING!!!!");
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

      log::log_debug("DONE TESTING THAT PING!!!!");
      return (avg / i);
}

/**
 * @brief fucking separate this...
 * @param server
 */
void ServerPreferrer::check_server_connection(server_info server) {
      // remove all previous checks
      checks.clear();

      // throw a thread outside of the game's thread so it doesn't lag the game.
      std::string pingaddr = server.ping_url;
      pingaddr             = pingaddr.substr(0, pingaddr.find(":"));

      /*
       * NEED TO THINK ABOUT THIS SECTION LATER UGGHHHHHHHHHHH
       */

      if (check_server_ping) {
            checks.emplace_back(std::async(std::launch::async, time_icmp_ping, pingaddr, 5));
      }

      // checks.emplace_back(std::async(
      //       std::launch::async,
      //       &ServerPreferrer::is_valid_game_mode,
      //       this,
      //       static_cast<PlaylistId>(std::stoi(server.playlist_id))));

      // this is purely for checking for success
      log::log_debug("IN CHECK_SERVER_CONNECTION; {}", PRINT_NAMEANDVAR(checks.size()));
      HookedEvents::AddHookedEvent("Function ProjectX.GFxEngine_X.Tick", [this](...) {
            log::log_debug("IN TICK; {}", PRINT_NAMEANDVAR(checks.size()));
            for (auto check = checks.begin(); auto & elem : checks) {
                  using namespace std::chrono_literals;
                  std::future_status fs = elem.wait_for(0s);
                  if (fs == std::future_status::ready) {
                        // HANDLE IT
                        checks.clear();
                        break;
                  }
            }

            int ping = checks[0].get().value_or(0);  // .get() MAKES TASK INVALID

            log::log_debug(
                  "HAVE VALUE YET?: {}, THRESHOLD WTF?: {}, WHERERE MY VARIABLES?: {}",
                  ping,
                  ping_threshold,
                  should_requeue_after_cancel);

            // report
            if (ping < 0) {
                  // an error occurred
                  log::log_error(
                        "ERROR WHILE RUNNING PING TEST: {}",
                        ping == -1   ? "INET_PTON FAILURE"
                        : ping == -2 ? "IcmpCreateFile INVALID_HANDLE_VALUE FAILURE"
                        : ping == -3 ? " UNABLE TO CREATE MEMORY FOR REPLY BUFFER"
                        : ping == -4 ? " ICMP PING CALL FAILURE "
                                     : "UNKNOWN");
            } else {
                  // success?
                  auto str = std::format(
                        "AVERAGE PING {0:c} THRESHOLD, {1} "
                        "{0:c} {2}",
                        ping > ping_threshold   ? '>'
                        : ping < ping_threshold ? '<'
                                                : '=',
                        ping,
                        ping_threshold);

                  log::log_error(str);
                  gameWrapper->LogToChatbox(str, "SP");
            }

            // finally, if an error occurred or ping above the threshold
            if (ping < 0 || ping > ping_threshold) {
                  // TURN OFF THE GAME'S FLASHING TASKBAR ICON!
                  // because the player didn't join a game, so don't flash the taskbar
                  // like they did
                  FLASHWINFO stop_flashing {
                        .cbSize    = sizeof(FLASHWINFO),
                        .hwnd      = rl_hwnd,
                        .dwFlags   = FLASHW_STOP,
                        .uCount    = 0,
                        .dwTimeout = 0};
                  FlashWindowEx(&stop_flashing);

                  gameWrapper->Execute([this](GameWrapper * gw) { cvarManager->executeCommand("queue_cancel"); });

                  if (should_requeue_after_cancel) {
                        gameWrapper->Execute([this](GameWrapper * gw) { cvarManager->executeCommand("queue"); });
                  }
            }

            if (should_focus_on_success) {
                  // idk if I need to check for states to determine what nCmdShow should be...
                  // if (IsIconic(rl_hwnd)) {
                  //      // means the rocket league window is minimized
                  //      ShowWindowAsync(rl_hwnd, SW_RESTORE);
                  //}
                  ShowWindowAsync(rl_hwnd, SW_RESTORE);
            }
            HookedEvents::RemoveHook("Function ProjectX.GFxEngine_X.Tick");
      });
      /*** end check for ping ***/
      // write_out_server_attempt();
}

/// <summary>
/// WHY? BECAUSE .front() ON AN EMPTY DEQUE IS UB
/// https://en.cppreference.com/w/cpp/container/deque/front
/// AND I WOULD RATHER IT RETURN AN EMPTY ENTRY
/// WHEN ASSUMEDLY GETTING THE FIRST ENTRY (SOMETIMES)
///
/// THIS IS SOLELY TO ADDRESS WHEN THE BANK IS EMPTY
/// </summary>
ServerPreferrer::server_info ServerPreferrer::get_first_server_entry() {
      return server_entries.empty() ? server_info {} : server_entries.front();
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
      return server_entries.empty() ? server_info {} : server_entries.back();
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
/// so ¯\(°_o)/¯
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
