#ifndef _SERVERPREFERRER_H_
#define _SERVERPREFERRER_H_

#include <chrono>
#include <deque>
#include <expected>
#include <future>
#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"  // IWYU pragma: keep

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginsettingswindow.h"

#include "bm_helper.hpp"
#include "Logger.hpp"
#include "PersistentManagedCVarStorage.hpp"
#include "version.h"
// #include "imgui_helper.hpp"

namespace {
using log = LOGGER;

enum class CONNECTION_STATUS {
      NOT_READY,

      // pinging-related statuses / errors
      INET_PTON_FAILURE,
      IcmpCreateFile_INVALID_HANDLE_VALUE_FAILURE,
      NO_MEMORY_REPLY_BUFFER,
      ICMP_PING_CALL_FAILURE,

      //
      INVALID_GAME_MODE,

      //
      SUCCESS,
      UNKNOWN
};

class ConnectionState {
public:
      CONNECTION_STATUS status = CONNECTION_STATUS::UNKNOWN;
      int               ping   = 0;

      friend bool operator==(const ConnectionState & lhs, const CONNECTION_STATUS & rhs) { return lhs.status == rhs; }
};
}  // namespace

constexpr auto plugin_version =
      stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

// registerCvar([req] name,[req] default_value,[req] description, searchable, has_min,
// min, has_max, max, save_to_cfg)
#define LIST_OF_PLUGIN_CVARS                                                                             \
      X(enabled, "1", "Global flag to determine if plugin functions", false)                             \
      X(check_server_ping, "1", "Check the server ping before connecting?", false)                       \
      X(server_ping_threshold, "40", "Ping threshold for a server connection", false, true, 0)           \
      X(should_requeue_after_ping_test, "1", "Should plugin try to requeue if failed ping test?", false) \
      X(should_focus_on_success, "1", "Make sure game window is focused on success", false);

#include "CVarManager.hpp"  // needs to come _after_ previous define

class ServerPreferrer : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow {
private:
      // saving data to a file
      const std::filesystem::path RECORD_FP =
            gameWrapper->GetDataFolder().append("ServerPreferrer\\ServerJoiningRecords.csv");

      // time
      const std::string                            DATETIME_FORMAT_STR = "{0:%F}T{0:%T%z}";
      const std::string                            DATETIME_PARSE_STR  = "%FT%T%z";
      static inline const std::chrono::time_zone * tz                  = std::chrono::current_zone();

      // ROCKET LEAGUE'S WINDOW HANDLE!~
      // https://freebasic.net/forum/viewtopic.php?t=4863
      static inline const HWND rl_hwnd = []() {
            DWORD pid  = GetCurrentProcessId();
            HWND  hWnd = FindWindow(NULL, NULL);
            while (hWnd) {
                  // cycle through all windows to find one that has "Rocket" in the name
                  // then return its handle
                  DWORD procID;
                  GetWindowThreadProcessId(hWnd, &procID);
                  if (procID == pid) {
#ifdef _MBCS
                        char str[128] = {0};
                        GetWindowText(hWnd, str, 128);
                        if (strstr(str, "Rocket") != NULL) {
                              return hWnd;
                        }
#else
#ifdef _UNICODE
                        wchar_t str[128] = {0};
                        GetWindowText(hWnd, str, 128);
                        if (wcsstr(str, L"Rocket") != NULL) {
                              return hWnd;
                        }
#endif
#endif
                  };
                  // get next window
                  hWnd = GetWindow(hWnd, GW_HWNDNEXT);
            }
            return hWnd;
      }();

      std::unique_ptr<PersistentManagedCVarStorage> cvar_storage;

      // server data
      struct server_info {
            // join* entries  are unnecessary for my purposes
            std::chrono::zoned_seconds tp;
            std::string                server_name;
            std::string                playlist_id;
            std::string                region;
            std::string                ping_url;
            std::string                game_url;
      };
      std::deque<server_info>                                         server_entries;
      std::vector<std::future<std::expected<int, CONNECTION_STATUS>>> checks;

      // ping data
      int ping_threshold = 0;

      std::string last_map_command;

      // flags
      bool plugin_enabled              = false;
      bool should_requeue_after_cancel = false;
      bool should_focus_on_success     = true;
      bool check_server_ping           = false;

      struct base_param_type {
            bm_helper::details::FString command;
            bool                        write_to_log : 1;
      };

      // initialization related functions
      void init_cvars();
      void init_hooked_events();
      void init_data();
      // void init_graph_data(); I want to graph things, separated by regions

      // major func
      void enable_plugin();
      void disable_plugin();

      void check_server_connection(server_info server);

      // tests
      using test_t = std::expected<int, CONNECTION_STATUS>;
      test_t is_valid_game_mode(PlaylistId playid);
      test_t is_good_ping_icmp(std::string pingaddr, int times);

      // deque fns
      server_info get_first_server_entry();
      server_info get_last_server_entry();

      // time fns
      std::string                get_current_datetime_str();
      std::chrono::zoned_seconds get_timepoint_from_str(std::string);

public:
      // honestly, for the sake of inheritance,
      // the member access in this class doesn't matter.
      // (since it's not expected to be inherited from)

      void onLoad() override;
      void onUnload() override;

      void        RenderSettings() override;
      std::string GetPluginName() override;
      void        SetImGuiContext(uintptr_t ctx) override;

      //
      // inherit from
      //				public BakkesMod::Plugin::PluginWindow
      //	 for the following
      // void        OnOpen() override;
      // void        OnClose() override;
      // void        Render() override;
      // std::string GetMenuName() override;
      // std::string GetMenuTitle() override;
      // bool        IsActiveOverlay() override;
      // bool        ShouldBlockInput() override;
};
#endif
