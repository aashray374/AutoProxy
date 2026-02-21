#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include "../include/Tray.h"
#include "../include/Monitor.h"
#include "../include/Config.h"
#include "../include/Logger.h"
#include "../include/ProxyManager.h"
#include "../include/GitProxy.h"
#include "../include/NetworkUtils.h"
#include "../include/Env.h"

void UpdateProxy();

AppConfig config;
HWND g_hwnd;  
EnvConfig env;
static bool g_lastProxyState = false;
static bool g_initialized = false;
static std::string g_activeProfile = "";


LRESULT CALLBACK WindowProc(HWND hwnd,
                            UINT msg,
                            WPARAM wParam,
                            LPARAM lParam){
    switch (msg){
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONDOWN){
            HMENU menu = CreatePopupMenu();
            InsertMenu(menu, -1, MF_BYPOSITION,
                       ID_TRAY_EXIT, "Exit");

            POINT pt;
            GetCursorPos(&pt);

            SetForegroundWindow(hwnd);
            TrackPopupMenu(menu,
                           TPM_BOTTOMALIGN | TPM_LEFTALIGN,
                           pt.x, pt.y,
                           0, hwnd, NULL);

            DestroyMenu(menu);
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT){
            DestroyWindow(hwnd);
        }
        break;

    case WM_DESTROY:
        RemoveTray();
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


DWORD WINAPI WorkerThread(LPVOID){
    MonitorNetwork(UpdateProxy);
    return 0;
}


void UpdateProxy()
{
    std::string ip = GetLocalIPv4();

    if (ip.empty())
        return;

    for (const auto& profile : config.profiles)
    {
        if (IsInSubnet(ip, profile.subnet))
        {
            if (g_activeProfile == profile.name)
                return;   // Already active

            g_activeProfile = profile.name;

            if (!profile.proxy.empty())
                SetProxy(true, profile.proxy);
            else
                SetProxy(false, "");

            if (profile.enableGit)
                SetGitProxy(true, profile.proxy,env.gitUser,env.gitPass);
            else
                SetGitProxy(false, "", "", "");

            ShowNotification("Auto Proxy",
                             ("Profile: " + profile.name).c_str());

            return;
        }
    }

    // If no profile matched
    if (!g_activeProfile.empty())
    {
        g_activeProfile = "";
        SetProxy(false, "");
        SetGitProxy(false, "", "", "");
        ShowNotification("Auto Proxy",
                         "No profile matched");
    }
}
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE,
                   LPSTR,
                   int)
{
    config = LoadConfig("config.ini");
    InitLogger(config.loggingEnabled,
               config.logFile);

    const char CLASS_NAME[] = "AutoProxyTray";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    g_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "AutoProxy",
        0,
        0, 0, 0, 0,
        NULL,
        NULL,
        hInstance,
        NULL);

    InitTray(g_hwnd);

    ShowNotification("Auto Proxy",
                     "Running in background");

    CreateThread(NULL, 0,
                 WorkerThread,
                 NULL, 0, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}