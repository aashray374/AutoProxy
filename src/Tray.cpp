#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0601
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include "../include/Tray.h"

static NOTIFYICONDATA nid;

void InitTray(HWND hwnd){
    ZeroMemory(&nid, sizeof(nid));

    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    strcpy(nid.szTip, "Auto Proxy Switcher");

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTray(){
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowNotification(const char* title,
                      const char* message){
    nid.uFlags = NIF_INFO | NIF_MESSAGE | NIF_ICON;

    strcpy(nid.szInfoTitle, title);
    strcpy(nid.szInfo, message);
    nid.dwInfoFlags = NIIF_INFO;

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}