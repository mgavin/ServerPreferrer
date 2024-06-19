#ifndef _SERVERPREFERRER_H_
#define _SERVERPREFERRER_H_

#include <atomic>
#include <chrono>
#include <deque>
#include <fstream>
#include <memory>
#include <optional>
#include <thread>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginsettingswindow.h"

// #include "bm_helper.h"
// #include "imgui_helper.h"

class ServerPreferrer : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow {
private:
        // const data members
        static inline const std::string cmd_prefix = "sp_";

        // the saving of data should have its functionality separated
        const std::filesystem::path RECORD_POPULATION_FILE =
                gameWrapper->GetDataFolder().append("ServerPreferrer\\ServerJoiningRecords.csv");
        const std::string                            DATETIME_FORMAT_STR = "{0:%F}T{0:%T%z}";
        const std::string                            DATETIME_PARSE_STR  = "%FT%T%z";
        static inline const std::chrono::time_zone * tz                  = std::chrono::current_zone();

        // for the threading
        std::atomic<std::optional<int>> current_ping_test;

        // launch log file data
        std::filesystem::path launch_log_path;  // needs to be computed at runtime
        std::ifstream         launch_file;

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

        // initialization related functions
        void init_cvars();
        void init_hooked_events();
        void init_data();
        // void init_graph_data(); I want to graph things, separated by regions

        // major func
        void enable_plugin();
        void disable_plugin();

        bool check_launch_log(std::streamoff start_read);
        void check_ping(std::string ip_address, int threshold);

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
