#include <core/logger.h>

int main()
{
    ZZ_LOG_FATAL("Test message: %f", 3.14f);
    ZZ_LOG_ERROR("Test message: %f", 3.14f);
    ZZ_LOG_WARNING("Test message: %f", 3.14f);
    ZZ_LOG_INFO("Test message: %f", 3.14f);
    ZZ_LOG_DEBUG("Test message: %f", 3.14f);
    ZZ_LOG_TRACE("Test message: %f", 3.14f);
    return 0;
}