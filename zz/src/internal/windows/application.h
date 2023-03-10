#ifdef ZZ_WINDOWS
#ifndef ZZ_INTERNAL_WINDOWS_APPLICATION_H
#define ZZ_INTERNAL_WINDOWS_APPLICATION_H

#include "zz/zz.h"

#include <windows.h>
#include <windowsx.h>

struct zz_internal_application
{
    HINSTANCE hInstance;
    HWND hWnd;
};

HINSTANCE zz_internal_windows_application_get_hinstance();
HWND zz_internal_windows_application_get_hwnd();

LRESULT CALLBACK zz_internal_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

#endif
#endif