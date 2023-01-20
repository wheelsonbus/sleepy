#include "platform.h"
#include "platform_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

#include <stdlib.h>

#include "core/log/log.h"

b8 platform_initialize(struct platform* platform, const char* application_name, i32 x, i32 y, i32 width, i32 height)
{
    platform->state = malloc(sizeof(struct platform_windows_state));
    struct platform_windows_state* state = (struct platform_windows_state*)platform->state;

    state->hInstance = GetModuleHandleA(0);

    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = platform_windows_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "zz_window_class";

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Window registration failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    u32 window_x = x;
    u32 window_y = y;
    u32 window_width = width;
    u32 window_height = height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    RECT borderRect = {0, 0, 0, 0};
    AdjustWindowRectEx(&borderRect, window_style, 0, window_ex_style);

    window_x += borderRect.left;
    window_y += borderRect.top;

    window_width += borderRect.right - borderRect.left;
    window_height += borderRect.bottom - borderRect.top;

    HWND hWnd = CreateWindowExA(window_ex_style, "zz_window_class", application_name, window_style, window_x, window_y, window_width, window_height, 0, 0, state->hInstance, 0);
    if (hWnd == 0)
    {
        MessageBoxA(NULL, "Window creation failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        ZZ_LOG_FATAL("Window creation failed.");
        return FALSE;
    }
    else
    {
        state->hWnd = hWnd;
    }

    b32 should_activate = TRUE;
    i32 show_window_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state->hWnd, show_window_flags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    state->clock_frequency = 1.0f / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&(state->applicationStartTime));

    return TRUE;
}

void platform_deinitialize(struct platform* platform)
{
    struct platform_windows_state* state = (struct platform_windows_state*)platform->state;

    if(state->hWnd)
    {
        DestroyWindow(state->hWnd);
        state->hWnd = 0;
    }
}

b8 platform_pump_messages(struct platform* platform)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return TRUE;
}

LRESULT CALLBACK platform_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            // TODO: Fire quit event
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            {
                // TODO: Fire resize event
            }  
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            /*{
                b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                // TODO: Process input
            }*/
            break;
        case WM_MOUSEMOVE:
            /*{
                i32 mouse_x = GET_X_LPARAM(lParam);
                i32 mouse_y = GET_Y_LPARAM(lParam);
                // TODO: Process input
            }*/
            break;
        case WM_MOUSEWHEEL:
            /*{
                i32 mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (mouse_wheel_delta < 0)
                {
                    mouse_wheel_delta = -1;
                    // TODO: Process input
                }
                else if (mouse_wheel_delta > 0)
                {
                    mouse_wheel_delta = 1;
                    // TODO: Process input
                }
            }*/
            break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            /*{
                b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
                //TODO: Process input
            }*/
            break;
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

#endif