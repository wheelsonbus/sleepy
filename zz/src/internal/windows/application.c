#ifdef ZZ_WINDOWS

#include "internal/application.h"
#include "application.h"

#include <stdlib.h>

#include "zz/log.h"

static struct internal_application internal_application;

static LARGE_INTEGER internal_application_windows_clock_frequency;
static LARGE_INTEGER internal_application_windows_start_time;

b8 internal_application_initialize(struct internal_application_config* config)
{
    internal_application.hInstance = GetModuleHandleA(0);

    HICON icon = LoadIcon(internal_application.hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = internal_application_windows_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = internal_application.hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(ZZ_NULL, IDC_ARROW);
    wc.hbrBackground = ZZ_NULL;
    wc.lpszClassName = "zz_window_class";

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Window registration failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return ZZ_FALSE;
    }

    u32 window_x = config->x;
    u32 window_y = config->y;
    u32 window_width = config->width;
    u32 window_height = config->height;

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

    HWND hWnd = CreateWindowExA(window_ex_style, "zz_window_class", config->name, window_style, window_x, window_y, window_width, window_height, 0, 0, internal_application.hInstance, 0);
    if (hWnd == 0)
    {
        MessageBoxA(ZZ_NULL, "Window creation failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        ZZ_LOG_FATAL("Window creation failed.");
        return ZZ_FALSE;
    }
    else
    {
        internal_application.hWnd = hWnd;
    }

    b32 should_activate = ZZ_TRUE;
    i32 show_window_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(internal_application.hWnd, show_window_flags);

    QueryPerformanceFrequency(&internal_application_windows_clock_frequency);
    QueryPerformanceCounter(&internal_application_windows_start_time);

    return ZZ_TRUE;
}

void internal_application_deinitialize()
{
    if(internal_application.hWnd)
    {
        DestroyWindow(internal_application.hWnd);
        internal_application.hWnd = 0;
    }
}

b8 internal_application_pump_messages()
{
    MSG msg;
    while (PeekMessageA(&msg, ZZ_NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return ZZ_TRUE;
}

void internal_application_get_size(u16* width, u16* height)
{
    RECT rect;
    GetClientRect(internal_application.hWnd, &rect);
    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

u64 internal_application_get_time()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return ((u64)1000 * (now.QuadPart - internal_application_windows_start_time.QuadPart)) / internal_application_windows_clock_frequency.QuadPart;
}

void internal_application_sleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

HINSTANCE internal_windows_application_get_hinstance()
{
    return internal_application.hInstance;
}

HWND internal_windows_application_get_hwnd()
{
    return internal_application.hWnd;
}

LRESULT CALLBACK internal_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            event_send_null(ZZ_EVENT_CODE_QUIT, 0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            {
                RECT rect;
                GetClientRect(hWnd, &rect);
                u32 width = rect.right - rect.left;
                u32 height = rect.bottom - rect.top;

                union event_data event_data;
                event_data.u16[0] = (u16)width;
                event_data.u16[1] = (u16)height;
                event_send(ZZ_EVENT_CODE_RESIZE, 0, event_data);
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

#endif