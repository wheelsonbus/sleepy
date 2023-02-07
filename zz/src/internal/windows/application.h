#if defined(ZZ__WINDOWS)

#pragma once

#include "zz/zz.h"

#include <windows.h>
#include <windowsx.h>

struct internal_application
{
    HINSTANCE hInstance;
    HWND hWnd;
};

HINSTANCE internal_windows_application_get_hinstance();
HWND internal_windows_application_get_hwnd();

LRESULT CALLBACK internal_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

#endif