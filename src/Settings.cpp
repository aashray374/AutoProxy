#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include "../include/Settings.h"

extern bool g_appEnabled;
extern bool g_showNotifications;

#define ID_CHECK_APP 3001
#define ID_CHECK_NOTIFY 3002
#define ID_SAVE_BTN 3003

LRESULT CALLBACK SettingsProc(HWND hwnd,
                              UINT msg,
                              WPARAM wParam,
                              LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        HWND hAppCheck = CreateWindow(
            "BUTTON", "Enable App",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 20, 150, 25,
            hwnd, (HMENU)ID_CHECK_APP,
            NULL, NULL);

        HWND hNotifyCheck = CreateWindow(
            "BUTTON", "Show Notifications",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 60, 180, 25,
            hwnd, (HMENU)ID_CHECK_NOTIFY,
            NULL, NULL);

        CreateWindow(
            "BUTTON", "Save",
            WS_VISIBLE | WS_CHILD,
            60, 100, 80, 30,
            hwnd, (HMENU)ID_SAVE_BTN,
            NULL, NULL);

        // 🔥 Set initial state from global variables
        SendMessage(hAppCheck,
                    BM_SETCHECK,
                    g_appEnabled ? BST_CHECKED : BST_UNCHECKED,
                    0);

        SendMessage(hNotifyCheck,
                    BM_SETCHECK,
                    g_showNotifications ? BST_CHECKED : BST_UNCHECKED,
                    0);

        break;
    }

    case WM_COMMAND:

        if (LOWORD(wParam) == ID_SAVE_BTN)
        {
            g_appEnabled =
                SendMessage(GetDlgItem(hwnd, ID_CHECK_APP),
                            BM_GETCHECK, 0, 0) == BST_CHECKED;

            g_showNotifications =
                SendMessage(GetDlgItem(hwnd, ID_CHECK_NOTIFY),
                            BM_GETCHECK, 0, 0) == BST_CHECKED;

            DestroyWindow(hwnd);
        }

        break;

    case WM_DESTROY:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CreateSettingsWindow()
{
    const char CLASS_NAME[] = "SettingsWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = SettingsProc;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    CreateWindowEx(
        0,
        CLASS_NAME,
        "Settings",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        200, 200, 300, 200,
        NULL,
        NULL,
        NULL,
        NULL);
}