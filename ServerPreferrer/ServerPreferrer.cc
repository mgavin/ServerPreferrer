/**
 *  TODO???????????????????
 *    uh...
 *  I WANTED TO DO THE FUCKING NAMEPLATE PLUGIN!!! UGH FUCK CMAKE!
 *  AND FUCK BAKKESMOD PEOPLE
 *
 *  ------------------------------------------------------------------
 *  oh, vcpkg ... rapidcsv or something (I just think rapid csv is most popular)
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include "ServerPreferrer.h"

#include <regex>

// clang-format off
#include "Windows.h"
#include "shellapi.h"
// #include "WinSock2.h"
#include "WS2tcpip.h"

// icmp echo
#include "iphlpapi.h"
#include "IcmpAPI.h"
// clang-format on

#include "imgui.h"
#include "imgui_internal.h"

#include "HookedEvents.h"
#include "Logger.h"

BAKKESMOD_PLUGIN(ServerPreferrer, "ServerPreferrer", "0.0.0", /*UNUSED*/ NULL);
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

/// <summary>
/// do the following when your plugin is loaded
/// </summary>
void ServerPreferrer::onLoad() {
        // initialize things
        _globalCvarManager        = cvarManager;
        HookedEvents::gameWrapper = gameWrapper;

        init_cvars();
        init_hooked_events();
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
        HookedEvents::AddHookedEvent("Function TAGame.PlayerController_TA.ReportServer", [this](std::string eventName) {
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
                        ll.open(launchlog, std::ios::in);
                        ll.seekg(0, std::ios::end);
                        start_read = ll.tellg();
                        // LOG("log file opened, sought offset: {}", start_read);
                        HookedEvents::AddHookedEvent(
                                "Function Engine.DateTime.EpochNow",
                                [this](std::string eventName) { read_log_and_ping(); });
                });

        HookedEvents::AddHookedEvent(
                "Function ProjectX.OnlineGameMatchmakingBase_X.OnSearchComplete",
                //"Function ProjectX.OnlineGameMatchmakingBase_X.Joining.EndState",
                [this](std::string eventName) {
                        // stop reading the end of the Launch.log file
                        ll.close();
                        HookedEvents::RemoveHook("Function Engine.DateTime.EpochNow");
                        // LOG("log file closed");
                });

        HookedEvents::AddHookedEvent("Function Engine.GameInfo.PreExit", [this](std::string eventName) {
                // ASSURED CLEANUP
                onUnload();
        });
}

void try_icmp_ping(
        std::string                           ipaddr,
        int                                   threshold,
        std::shared_ptr<GameWrapper> *        gw,
        std::shared_ptr<CVarManagerWrapper> * cvmw) {
        auto CancelMatchmaking = [&gw]() {
                MatchmakingWrapper mw = (*gw)->GetMatchmakingWrapper();
                if (mw) {
                        mw.CancelMatchmaking();
                }
        };

        // https:  // learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho#examples
        HANDLE        hIcmpFile;
        unsigned long ip_addr  = INADDR_NONE;
        DWORD         dwRetVal = 0;

        inet_pton(AF_INET, ipaddr.c_str(), reinterpret_cast<void *>(&ip_addr));
        if (ip_addr == INADDR_NONE) {
                LOG("WTF inet_pton???");
                CancelMatchmaking();
                return;
        }

        hIcmpFile = IcmpCreateFile();
        if (hIcmpFile == INVALID_HANDLE_VALUE) {
                LOG("ICMP HANDLE VALUE ERROR: {}", GetLastError());
                CancelMatchmaking();
                return;
        }

        char   SendData[1] = "";
        LPVOID ReplyBuffer = NULL;
        DWORD  ReplySize   = 0;
        ReplySize          = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer        = (VOID *)malloc(ReplySize);
        int avg            = 0;
        int i              = 1;
        for (int j = 1; j <= 5; ++j) {
                auto t1  = std::chrono::high_resolution_clock::now();
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
                        // LOG("RECEIVED FROM: {}, STATUS: {}, ROUNDTRIP TIME: {}",
                        //     addr,
                        //     pEchoReply->Status,
                        //     pEchoReply->RoundTripTime);
                        avg += pEchoReply->RoundTripTime;
                } else {
                        LOG("PING ERROR ON #{}", i);
                        CancelMatchmaking();
                        return;
                }
                i = j;
        }

        LOG("AVERAGE PING {0:c} THRESHOLD, {1} {0:c} {2}",
            avg > threshold   ? '>'
            : avg < threshold ? '<'
                              : '=',
            avg,
            threshold);
        (*gw)->LogToChatbox(
                std::vformat(
                        "AVERAGE PING {0:c} THRESHOLD, {1} {0:c} {2}",
                        std::make_format_args(
                                avg > threshold   ? '>'
                                : avg < threshold ? '<'
                                                  : '=',
                                avg,
                                threshold)),
                "BMP");
        if (avg > threshold || avg < 0) {
                (*gw)->Execute([&cvmw](GameWrapper * gw) { (*cvmw)->executeCommand("queue_cancel"); });
                (*gw)->Execute([&cvmw](GameWrapper * gw) { (*cvmw)->executeCommand("queue"); });
        }
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
        inet_pton(AF_INET, ipaddr.c_str(), reinterpret_cast<void *>(&connection.sin_addr.s_addr));
        connection.sin_port = htons(std::stoi(port));

        for (int i = 0; i < 5; ++i) {
                // do 5 connection runs
                auto t1 = std::chrono::high_resolution_clock::now();

                iResult = connect(ConnectSocket, reinterpret_cast<SOCKADDR *>(&connection), sizeof(connection));
                if (iResult == SOCKET_ERROR) {
                        LOG("connect failed with error: {}", WSAGetLastError());
                        iResult = closesocket(ConnectSocket);
                        if (iResult == SOCKET_ERROR) {
                                LOG("CLOSE SOCKET FAILED??: {}", WSAGetLastError());
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
                    std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
        }

        WSACleanup();
}
*/

/// <summary>
/// Separate into read_log and check_ping
///
/// FUN FUN FUN
/// </summary>
void ServerPreferrer::read_log_and_ping() {
        gameWrapper->ExecuteUnrealCommand("flushlog");
        std::string line;
        std::regex  pingurlmatch {".*PingURL=\"(.*?)[\"].*"};
        while (std::getline(ll, line)) {
                std::string ipstr;
                try {
                        // LOG("{}", line);
                        std::smatch rematch;
                        if (std::regex_match(line, rematch, pingurlmatch)) {
                                if (rematch.size() == 2) {
                                        std::ssub_match m {rematch[1]};
                                        // LOG("PING URL: {}", m.str());
                                        ipstr = m.str();
                                }
                        }
                } catch (...) {
                        cvarManager->log("FORMAT ERROR: " + line);
                }

                std::string ipp1, ipp2, ipp3, ipp4;
                std::string port;
                if (!ipstr.empty()) {
                        std::regex  ipdiv {"(\\d+).(\\d+).(\\d+).(\\d+):(\\d+)"};
                        std::smatch ipmatch;
                        if (std::regex_match(ipstr, ipmatch, ipdiv)) {
                                if (ipmatch.size() == 6) {
                                        std::ssub_match ipm1 {ipmatch[1]};
                                        std::ssub_match ipm2 {ipmatch[2]};
                                        std::ssub_match ipm3 {ipmatch[3]};
                                        std::ssub_match ipm4 {ipmatch[4]};
                                        std::ssub_match ipm5 {ipmatch[5]};
                                        /* LOG("1st: {}, 2nd: {}, 3rd: "
                                             "{}, 4th: {} | PORT: {}",
                                            ipm1.str(),
                                            ipm2.str(),
                                            ipm3.str(),
                                            ipm4.str(),
                                            ipm5.str());*/

                                        ipp1 = ipm1.str();
                                        ipp2 = ipm2.str();
                                        ipp3 = ipm3.str();
                                        ipp4 = ipm4.str();
                                        port = ipm5.str();
                                }
                        } else {
                                LOG("COULDN'T BREAK APART IPADDR?");
                                break;
                        }

                        std::string lol       = ipp1 + "." + ipp2 + "." + ipp3 + "." + ipp4;
                        int         threshold = 0;
                        CVarWrapper cv        = cvarManager->getCvar(cmd_prefix + "server_ping_threshold");
                        if (cv) {
                                threshold = cv.getIntValue();
                        }
                        //// TRY TO PING!
                        std::thread t {try_icmp_ping, lol, threshold, &gameWrapper, &cvarManager};
                        t.detach();
                        //// try_tcp_ping(lol, port);
                }
        }
        ll.clear();
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
                        // What if the URL length is greater than int but less than size_t?
                        // well then the program should crash, but this is fine.
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
        // look up how to do drag and drop
        // and a file select modal.
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
        return "ServerPreferrer";
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
