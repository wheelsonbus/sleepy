#include "event.h"

#include "core/memory/memory.h"
#include "core/log/log.h"
#include "containers/dynamic_array/dynamic_array.h"

struct event_receiver_registration
{
    void* receiver;
    event_callback_t callback;
};

struct event_code_registry
{
    struct event_receiver_registration* registrations;
};

#define MAX_CODES 16384
struct event_system
{
    struct event_code_registry registries[MAX_CODES];
};

static b8 initialized = FALSE;
static struct event_system event_system;

b8 event_initialize()
{
    if (initialized)
    {
        ZZ_LOG_ERROR("event_initialize called more than once.");
        return FALSE;
    }

    memory_zero(&event_system, sizeof(event_system));

    initialized = TRUE;
    return TRUE;
}

void event_deinitialize()
{
    for (u16 i = 0; i < MAX_CODES; i += 1)
    {
        if (event_system.registries[i].registrations != 0)
        {
            dynamic_array_destroy(event_system.registries[i].registrations);
            event_system.registries[i].registrations = 0;
        }
    }
}

b8 event_register_receiver(u16 code, void* receiver, event_callback_t callback)
{
    if (event_system.registries[code].registrations == 0)
    {
        event_system.registries[code].registrations = dynamic_array_create(struct event_receiver_registration);
    }

    u64 receivers_registered = dynamic_array_get_length(event_system.registries[code].registrations);
    for (u64 i = 0; i < receivers_registered; i += 1)
    {
        struct event_receiver_registration registration = event_system.registries[code].registrations[i];
        if (registration.receiver == receiver && registration.callback == callback)
        {
            ZZ_LOG_ERROR("Attempted to register a duplicate event receiver.");
            return FALSE;
        }
    }

    struct event_receiver_registration registration;
    registration.receiver = receiver;
    registration.callback = callback;
    dynamic_array_push(event_system.registries[code].registrations, registration);
    return TRUE;
}

b8 event_unregister_receiver(u16 code, void* receiver, event_callback_t callback)
{
    if (event_system.registries[code].registrations == 0)
    {
        ZZ_LOG_ERROR("Attempted to unregister a non-existent event registration.");
        return FALSE;
    }

    u64 receivers_registered = dynamic_array_get_length(event_system.registries[code].registrations);
    for (u64 i = 0; i < receivers_registered; i += 1)
    {
        struct event_receiver_registration registration = event_system.registries[code].registrations[i];
        if (registration.receiver == receiver && registration.callback == callback)
        {
            dynamic_array_pop_at(event_system.registries[code].registrations, i, &registration);
            return TRUE;
        }
    }

    ZZ_LOG_ERROR("Attempted to unregister a non-existent event registration.");
    return FALSE;
}

b8 event_send(u16 code, void* sender, union event_data data)
{
    if (event_system.registries[code].registrations == 0)
    {
        ZZ_LOG_WARNING("Attemped to send an event with no receivers.");
        return FALSE;
    }

    u64 receivers_registered = dynamic_array_get_length(event_system.registries[code].registrations);
    for (u64 i = 0; i < receivers_registered; i += 1)
    {
        struct event_receiver_registration registration = event_system.registries[code].registrations[i];
        if (registration.callback(code, sender, registration.receiver, data))
        {
            return TRUE;
        }
    }

    ZZ_LOG_WARNING("Event was not handled.");
    return FALSE;
}