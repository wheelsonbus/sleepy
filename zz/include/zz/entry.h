#ifndef ZZ_ENTRY_H
#define ZZ_ENTRY_H

#include "client.h"
#include "log.h"

extern void entry(struct client_config* client_config);

int main()
{
    struct client_config client_config;
    entry(&client_config);
    if (!client_initialize(&client_config))
    {
        ZZ_LOG_FATAL("Failed to create client.");
        return 1;
    }

    if (!client_loop())
    {
        ZZ_LOG_ERROR("client_loop returned ZZ_FALSE.");
        return 2;
    }

    client_deinitialize();

    return 0;
}

#endif