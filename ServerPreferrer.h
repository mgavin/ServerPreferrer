#ifndef _SERVERPREFERRER_H_
#define _SERVERPREFERRER_H_

#include <atomic>
#include <chrono>
#include <deque>
#include <expected>
#include <fstream>
#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"  // IWYU pragma: keep

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginsettingswindow.h"

#include "bm_helper.h"
#include "Logger.h"
// #include "imgui_helper.h"

namespace {
namespace log = LOGGER;

enum class PING_TEST_ERROR {
      NOT_READY,
      INET_PTON_FAILURE,
      IcmpCreateFile_INVALID_HANDLE_VALUE_FAILURE,
      NO_MEMORY_REPLY_BUFFER,
      ICMP_PING_CALL_FAILURE,
      UNKNOWN
};
}  // namespace

class ServerPreferrer : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow {
private:
      // if I had a "cvar manager", I'd bake this prefix in.
      static inline const std::string cmd_prefix = "sp_";

      // saving data to a file
      const std::filesystem::path RECORD_FP =
            gameWrapper->GetDataFolder().append("ServerPreferrer\\ServerJoiningRecords.csv");

      // Launch.log file data
      static inline const std::filesystem::path launch_log_path = []() {
            char home_drive[128] = {0};
            char home_path[128]  = {0};
            GetEnvironmentVariableA("HOMEDRIVE", home_drive, 128);
            GetEnvironmentVariableA("HOMEPATH", home_path, 128);
            return std::string(home_drive) + std::string(home_path)
                   + "/Documents/My Games/Rocket League/TAGame/Logs/Launch.log";
      }();
      std::ifstream launch_file;

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
                  DWORD procID;
                  GetWindowThreadProcessId(hWnd, &procID);
                  if (procID == pid) {
                        char str[128] = {0};
                        GetWindowText(hWnd, str, 128);
                        if (strstr(str, "Rocket") != NULL) {
                              return hWnd;
                        }
                  };
                  hWnd = GetWindow(hWnd, GW_HWNDNEXT);
            }
            return hWnd;
      }();

      // for the threading
      std::atomic<std::expected<int, PING_TEST_ERROR>> current_ping_test;

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
      std::deque<server_info> server_entries;

      // ping data
      int ping_threshold = 0;

      // flags
      bool should_requeue_after_cancel = false;

      // initialization related functions
      void init_cvars();
      void init_hooked_events();
      void init_data();
      // void init_graph_data(); I want to graph things, separated by regions

      // major func
      void enable_plugin();
      void disable_plugin();

      bool check_launch_log(std::streamoff start_read);
      void check_server_connection(server_info server);
      bool is_valid_game_mode(PlaylistId playid);
      int  check_ping(std::string ip_address);

      // deque func
      server_info get_first_server_entry();
      server_info get_last_server_entry();

      // chrono func
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
