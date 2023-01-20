#pragma once

#include "zz.h"

#if defined(ZZ_PLATFORM_WINDOWS)

#include <windows.h>
#include <windowsx.h>

struct platform_application_windows_state
{
    HINSTANCE hInstance;
    HWND hWnd;
};

LRESULT CALLBACK platform_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

f64 platform_application_windows_time_get();

#endif