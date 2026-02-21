#pragma once
#include <windows.h>

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001

void InitTray(HWND hwnd);
void RemoveTray();
void ShowNotification(const char* title,
                      const char* message);