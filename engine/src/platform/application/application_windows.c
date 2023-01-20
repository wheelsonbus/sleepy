#include "application.h"
#include "application_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

#include <stdlib.h>

#include "core/log/log.h"
#include "core/input/input.h"

static f64 platform_application_windows_clock_frequency;
static LARGE_INTEGER platform_application_windows_start_time;

b8 platform_application_initialize(struct platform_application* application, const char* name, i32 x, i32 y, i32 width, i32 height)
{
    application->state = malloc(sizeof(struct platform_application_windows_state));
    struct platform_application_windows_state* state = (struct platform_application_windows_state*)application->state;

    state->hInstance = GetModuleHandleA(0);

    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = platform_application_windows_process_message;
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

    HWND hWnd = CreateWindowExA(window_ex_style, "zz_window_class", name, window_style, window_x, window_y, window_width, window_height, 0, 0, state->hInstance, 0);
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
    platform_application_windows_clock_frequency = 1.0f / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&(platform_application_windows_start_time));

    return TRUE;
}

void platform_application_deinitialize(struct platform_application* application)
{
    struct platform_application_windows_state* state = (struct platform_application_windows_state*)application->state;

    if(state->hWnd)
    {
        DestroyWindow(state->hWnd);
        state->hWnd = 0;
    }
}

b8 platform_application_pump_messages(struct platform_application* application)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return TRUE;
}

LRESULT CALLBACK platform_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam)
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
            {
                b8 down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                input_set_key_state((u16)wParam, down);
            }
            break;
        case WM_MOUSEMOVE:
            {
                i32 x = GET_X_LPARAM(lParam);
                i32 y = GET_Y_LPARAM(lParam);
                input_set_mouse_position(x, y);
            }
            break;
        case WM_MOUSEWHEEL:
            {
                i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (delta < 0)
                {
                    input_move_mouse_wheel(-1);
                }
                else if (delta > 0)
                {
                    input_move_mouse_wheel(1);
                }
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            {
                b8 down = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
                enum input_mouse_button_code code = ZZ_INPUT_MOUSE_BUTTON_CODE_MAX;
                switch (msg)
                {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                        code = ZZ_INPUT_MOUSE_BUTTON_CODE_LEFT;
                        break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                        code = ZZ_INPUT_MOUSE_BUTTON_CODE_MIDDLE;
                        break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                        code = ZZ_INPUT_MOUSE_BUTTON_CODE_RIGHT;
                        break;
                }
                if (code != ZZ_INPUT_MOUSE_BUTTON_CODE_MAX)
                {
                    input_set_mouse_button_state(code, down);
                }
            }
            break;
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

f64 platform_application_windows_time_get()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (f64)currentTime.QuadPart * platform_application_windows_clock_frequency;
}

#endif