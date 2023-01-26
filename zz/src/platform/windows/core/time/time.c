#if defined(ZZ_PLATFORM_WINDOWS)

#include "platform/time.h"

#include "platform/windows/core/application/application.h"

f64 platform_time_get()
{
    return platform_application_windows_time_get();
}

void platform_time_sleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

#endif