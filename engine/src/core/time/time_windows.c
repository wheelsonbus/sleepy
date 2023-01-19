#include "time.h"
#include "time_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

f64 time_get(struct platform* platform)
{
    struct platform_windows_state* state = (struct platform_windows_state*)platform->state;

    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (f64)currentTime.QuadPart * state->clock_frequency;
}

void time_sleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

#endif