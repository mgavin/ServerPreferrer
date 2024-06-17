/**
 *  TODO???????????????????
 *    uh...
 *  I WANTED TO DO THE FUCKING NAMEPLATE PLUGIN!!! UGH FUCK CMAKE!
 *
 *  ------------------------------------------------------------------
 *
 */

#include "ServerPreferrer.h"

#include <chrono>
#include <regex>
#include <thread>

// clang-format off
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h" // NOLINT
#include "shellapi.h"
// #include "WinSock2.h"
#include "WS2tcpip.h"

// icmp echo
#include "iphlpapi.h" // NOLINT
#include "IcmpAPI.h"
// clang-format on

#include "imgui.h"
#include "imgui_internal.h"

#include "vincentlaucsb-csv-parser/csv.hpp"

#include "HookedEvents.h"
#include "Logger.h"

void time_icmp_ping(std::string, int, int &);

BAKKESMOD_PLUGIN(ServerPreferrer, "ServerPreferrer", "0.0.0", /*UNUSED*/ NULL);
std::shared_ptr<CVarManagerWrapper> _globalCVarManager;

/// <summary>
/// do the following when your plugin is loadedx
/// </summary>
void ServerPreferrer::onLoad() {
        // initialize things
        _globalCVarManager        = cvarManager;
        HookedEvents::gameWrapper = gameWrapper;

        // get the log file path
        char home_drive[128] = {0};
        char home_path[128]  = {0};
        GetEnvironmentVariableA("HOMEDRIVE", home_drive, 128);
        GetEnvironmentVariableA("HOMEPATH", home_path, 128);
        launch_log_path = std::string(home_drive) + std::string(home_path)
                          + "/Documents/My Games/Rocket League/TAGame/Logs/Launch.log";

        init_cvars();
        init_hooked_events();
        init_data();
}

/// <summary>
/// group together the initialization of cvars
/// </summary>
void ServerPreferrer::init_cvars() {
        cvarManager->registerCvar(
                cmd_prefix + "server_ping_threshold",
                "30",
                "ping threshold for a server connection",
                false,
                true,
                0);
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
                        // start reading the end of the Launch.log file

                        /* Seconds functions:
                        Function Engine.Actor.Timer
                        Function Engine.DateTime.EpochNow

                        ...
                        Function Core.Object.Subtract_QWordQWord (lol)
                        ...

                        Function ProjectX.Timers_X.Set
                        Function TAGame.RocketPass_TA.UpdateSecondsRemaining
                        */
                        gameWrapper->ExecuteUnrealCommand("flushlog");
                        launch_file.open(launch_log_path, std::ios::in);
                        launch_file.seekg(0, std::ios::end);
                        std::streamoff start = launch_file.tellg();
                        // LOG("log file opened, sought offset: {}", start_read);
                        HookedEvents::AddHookedEvent(
                                "Function Engine.DateTime.EpochNow",
                                [this, start](std::string eventName) {
                                        if (check_launch_log(start)) {
                                                // ready to process
                                                // spawn a thread to do the pinging
                                                check_ping(server_entries.back().ping_url, 40);
                                        }
                                });
                });

        HookedEvents::AddHookedEvent(
                "Function "
                "ProjectX.OnlineGameMatchmakingBase_X.OnSearchComplete",
                //"Function
                // ProjectX.OnlineGameMatchmakingBase_X.Joining.EndState",
                [this](std::string eventName) {
                        // stop reading the end of the Launch.log file
                        launch_file.close();
                        HookedEvents::RemoveHook("Function Engine.DateTime.EpochNow");
                        // LOG("log file closed");
                });

        HookedEvents::AddHookedEvent("Function Engine.GameInfo.PreExit", [this](std::string eventName) {
                // ASSURED CLEANUP
                onUnload();
        });
}

void ServerPreferrer::init_data() {
}

void time_icmp_ping(std::string pingaddr, int times, int & out) {
        // https:  //
        // learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho#examples
        HANDLE        hIcmpFile;
        unsigned long ip_addr  = INADDR_NONE;
        DWORD         dwRetVal = 0;

        std::string ipaddr = pingaddr.substr(0, pingaddr.find(":"));  // exclude port part

        inet_pton(AF_INET, ipaddr.c_str(), reinterpret_cast<void *>(&ip_addr));
        if (ip_addr == INADDR_NONE) {
                LOG("WTF inet_pton???");
                out = -1;
                return;
        }

        hIcmpFile = IcmpCreateFile();
        if (hIcmpFile == INVALID_HANDLE_VALUE) {
                LOG("ICMP HANDLE VALUE ERROR: {}", GetLastError());
                out = -1;
                return;
        }

        char   SendData[1] = "";
        LPVOID ReplyBuffer = NULL;
        DWORD  ReplySize   = 0;
        ReplySize          = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer        = (VOID *)malloc(ReplySize);

        int avg = 0;
        int i   = 1;
        for (int j = 1; j <= times; ++j) {
                dwRetVal = IcmpSendEcho(
                        hIcmpFile,
                        ip_addr,
                        SendData,
                        sizeof(SendData),
                        NULL,
                        ReplyBuffer,
                        ReplySize,
                        1000);
                if (dwRetVal != 0) {
                        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
                        char             addr[32]   = {0};
                        inet_ntop(AF_INET, reinterpret_cast<void *>(&(pEchoReply->Address)), addr, 32);
                        avg += pEchoReply->RoundTripTime;
                } else {
                        out = -1;
                        return;
                }
                i = j;
        }

        out = avg / i;
}

/*
* Unused for right now.
void try_tcp_ping(std::string ipaddr, std::string port) {
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
        inet_pton(AF_INET, ipaddr.c_str(), reinterpret_cast<void
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

        std::regex server_info_match {
                ".*?ServerName=\"([a-zA-Z0-9-]+)\""
                ".*?Playlist=([0-9]+)"
                ".*?Region=\"([a-zA-Z0-9-]+)\""
                ".*?PingURL=\"([0-9.:-]+)\""
                ".*?GameURL=\"([0-9.:-]+)\".*",
                std::regex::extended | std::regex::icase};

        while (std::getline(launch_file, line)) {
                std::smatch re_match;
                if (std::regex_match(line, re_match, server_info_match)) {
                        if (re_match.size() == 6) {
                                // A MATCH to the needed line!
                                // 6 because the first "match" is basically the entire line.
                                std::ssub_match server_name {re_match[1]};
                                std::ssub_match server_playlist {re_match[2]};
                                std::ssub_match server_region {re_match[3]};
                                std::ssub_match server_pingurl {re_match[4]};
                                std::ssub_match server_gameurl {re_match[5]};

                                server_entries.emplace_back(server_info {
                                        .tp =
                                                std::chrono::zoned_seconds {
                                                                            std::chrono::current_zone(),
                                                                            std::chrono::time_point_cast<std::chrono::seconds>(
                                                                            std::chrono::system_clock::now())},
                                        .server_name = server_name.str(),
                                        .playlist_id = server_playlist.str(),
                                        .region      = server_region.str(),
                                        .ping_url    = server_pingurl.str(),
                                        .game_url    = server_gameurl.str()
                                });

                                return true;
                        }
                }
        }
        launch_file.clear();
        return false;
}

void ServerPreferrer::check_ping(std::string ip_address, int threshold) {
        int          avg_ping = 0;
        std::jthread jt {time_icmp_ping, ip_address, 5, std::ref(avg_ping)};
        jt.detach();

        HookedEvents::AddHookedEvent(
                "Function ProjectX.GFxEngine_X.Tick",
                [this, &avg_ping, &threshold, &jt](...) {
                        if (!jt.joinable()) {
                                return;
                        }

                        jt.join();

                        LOG("AVERAGE PING {0:c} THRESHOLD, {1} {0:c} {2}",
                            avg_ping > threshold   ? '>'
                            : avg_ping < threshold ? '<'
                                                   : '=',
                            avg_ping,
                            threshold);

                        gameWrapper->LogToChatbox(
                                std::vformat(
                                        "AVERAGE PING {0:c} THRESHOLD, {1} "
                                        "{0:c} {2}",
                                        std::make_format_args(
                                                avg_ping > threshold   ? '>'
                                                : avg_ping < threshold ? '<'
                                                                       : '=',
                                                avg_ping,
                                                threshold)),
                                "BMP");
                        if (avg_ping > threshold || avg_ping < 0) {
                                gameWrapper->Execute(
                                        [this](GameWrapper * gw) { cvarManager->executeCommand("queue_cancel"); });
                                gameWrapper->Execute(
                                        [this](GameWrapper * gw) { cvarManager->executeCommand("queue"); });
                        }
                },
                true);
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
static inline void TextURL(const char * name_, const char * URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_) {
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
                        const int nchar = std::clamp(
                                static_cast<int>(std::strlen(URL_)),
                                0,
                                (std::numeric_limits<int>::max)() - 1);
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