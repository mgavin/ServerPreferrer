#pragma once

#include <fstream>
#include <memory>
#include <thread>

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginsettingswindow.h"

// #include "bm_helper.h"
// #include "imgui_helper.h"

class ServerPreferrer :
        public BakkesMod::Plugin::BakkesModPlugin,
        public BakkesMod::Plugin::PluginSettingsWindow {
private:
        // data members
        static inline const std::string           cmd_prefix = "sp_";
        // for reading the Launch.log file
        // needs to be taken from the user... in like, a file dialog modal
        // workshop map loader plugin?
        static inline const std::filesystem::path launch_log_path =
                "C:/Users/matthew/Documents/My Games/Rocket League/TAGame/Logs/Launch.log";

        std::ifstream launch_file;

        std::unique_ptr<std::jthread> thread;

        // helper functions
        void init_cvars();
        void init_hooked_events();

        void read_log_and_ping();
        void check_launch_log(std::streamoff start_read);
        void check_ping(std::string ip_address);

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
