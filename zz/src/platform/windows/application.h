#if defined(ZZ_PLATFORM_WINDOWS)

#pragma once

#include "zz/zz.h"

#include <windows.h>
#include <windowsx.h>

struct platform_application
{
    HINSTANCE hInstance;
    HWND hWnd;
};

LRESULT CALLBACK platform_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

#endif