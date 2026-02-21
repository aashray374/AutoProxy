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

void UpdateProxy(){
    std::string ip = GetLocalIPv4();

    bool inside = false;

    if (!ip.empty()){
        inside = IsInSubnet(ip, config.subnet);
    }

    if (g_initialized && inside == g_lastProxyState){
        return;   
    }

    g_initialized = true;
    g_lastProxyState = inside;

    SetProxy(inside, config.proxy);

    if (config.enableGit)
        SetGitProxy(inside, config.proxy,env.gitUser,env.gitPass);

    ShowNotification("Auto Proxy",
                     inside ? "Proxy Enabled"
                            : "Proxy Disabled");
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