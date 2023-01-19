#pragma once

#if defined(ZZ_PLATFORM_WINDOWS)

#include <windows.h>
#include <windowsx.h>

struct platform_windows_state
{
    HINSTANCE hInstance;
    HWND hWnd;

    f64 clock_frequency;
    LARGE_INTEGER applicationStartTime;
};

LRESULT CALLBACK platform_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

#endif