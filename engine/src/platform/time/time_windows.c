#include "time.h"
#include "time_windows.h"

#if defined(ZZ_PLATFORM_WINDOWS)

#include "platform/application/application_windows.h"

f64 platform_time_get()
{
    return platform_application_windows_time_get();
}

void platform_time_sleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

#endif