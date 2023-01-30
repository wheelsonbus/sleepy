#if defined(ZZ_PLATFORM_WINDOWS)

#include "platform/application.h"
#include "application.h"

#include <stdlib.h>

#include "zz/log.h"

static f64 platform_application_windows_clock_frequency;
static LARGE_INTEGER platform_application_windows_start_time;

static struct event* platform_event;
static struct input* platform_input;

b8 platform_application_create(struct platform_application* platform_application, struct platform_application_config* config)
{
    platform_event = config->event;
    platform_input = config->input;

    platform_application->hInstance = GetModuleHandleA(0);

    HICON icon = LoadIcon(platform_application->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = platform_application_windows_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = platform_application->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "zz_window_class";

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Window registration failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
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

    HWND hWnd = CreateWindowExA(window_ex_style, "zz_window_class", config->name, window_style, window_x, window_y, window_width, window_height, 0, 0, platform_application->hInstance, 0);
    if (hWnd == 0)
    {
        MessageBoxA(NULL, "Window creation failed.", "Error", MB_ICONEXCLAMATION | MB_OK);
        ZZ_LOG_FATAL("Window creation failed.");
        return FALSE;
    }
    else
    {
        platform_application->hWnd = hWnd;
    }

    b32 should_activate = TRUE;
    i32 show_window_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(platform_application->hWnd, show_window_flags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    platform_application_windows_clock_frequency = 1.0f / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&(platform_application_windows_start_time));

    return TRUE;
}

void platform_application_destroy(struct platform_application* platform_application)
{
    if(platform_application->hWnd)
    {
        DestroyWindow(platform_application->hWnd);
        platform_application->hWnd = 0;
    }
}

b8 platform_application_pump_messages(struct platform_application* platform_application)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return TRUE;
}

u64 platform_application_get_time(struct platform_application* platform_application)
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (f64)currentTime.QuadPart * platform_application_windows_clock_frequency;
}

void platform_application_sleep(struct platform_application* platform_application, u64 milliseconds)
{
    Sleep(milliseconds);
}

LRESULT CALLBACK platform_application_windows_process_message(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            event_send_null(platform_event, ZZ_EVENT_CODE_QUIT, 0);
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
                event_send(platform_event, ZZ_EVENT_CODE_RESIZE, 0, event_data);
            }  
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            {
                b8 down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                input_set_key_state(platform_input, (u16)wParam, down);
            }
            break;
        case WM_MOUSEMOVE:
            {
                i32 x = GET_X_LPARAM(lParam);
                i32 y = GET_Y_LPARAM(lParam);
                input_set_mouse_position(platform_input, x, y);
            }
            break;
        case WM_MOUSEWHEEL:
            {
                i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (delta < 0)
                {
                    input_move_mouse_wheel(platform_input, -1);
                }
                else if (delta > 0)
                {
                    input_move_mouse_wheel(platform_input, 1);
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
                    input_set_mouse_button_state(platform_input, code, down);
                }
            }
            break;
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

#endif