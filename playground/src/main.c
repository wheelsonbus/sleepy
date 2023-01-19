#include <core/logger.h>
#include <core/asserter.h>

#include <platform/platform.h>

int main()
{
    ZZ_LOG_FATAL("Test message: %f", 3.14f);
    ZZ_LOG_ERROR("Test message: %f", 3.14f);
    ZZ_LOG_WARNING("Test message: %f", 3.14f);
    ZZ_LOG_INFO("Test message: %f", 3.14f);
    ZZ_LOG_DEBUG("Test message: %f", 3.14f);
    ZZ_LOG_TRACE("Test message: %f", 3.14f);

    struct platform_state platform_state;
    if (platform_initialize(&platform_state, "Sleepy Engine Playground", 100, 100, 1200, 720))
    {
        while (TRUE)
        {
            platform_pump_messages(&platform_state);
        }
    }
    platform_deinitialize(&platform_state);

    return 0;
}