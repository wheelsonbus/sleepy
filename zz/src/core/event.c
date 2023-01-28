#include "zz/event.h"

#include "zz/memory.h"
#include "zz/log.h"

b8 event_create(struct event* event, struct event_config* config)
{
    event->memory = config->memory;

    for (u16 i = 0; i < ZZ_EVENT_CODE_MAX; i += 1)
    {
        memory_array_create(event->memory, &event->registries[i].registrations);
    }

    ZZ_LOG_INFO("Event module created.");
    return TRUE;
}

void event_destroy(struct event* event)
{
    for (u16 i = 0; i < ZZ_EVENT_CODE_MAX; i += 1)
    {
        memory_array_destroy(event->memory, &event->registries[i].registrations);
    }

    ZZ_LOG_INFO("Event module destroyed.");
}

b8 event_register_receiver(struct event* event, u16 code, void* receiver, event_callback_t callback)
{
    u64 receivers_registered = event->registries[code].registrations.length;
    for (u64 i = 0; i < receivers_registered; i += 1)
    {
        struct event_receiver_registration registration = event->registries[code].registrations.data[i];
        if (registration.receiver == receiver && registration.callback == callback)
        {
            ZZ_LOG_ERROR("Attempted to register a duplicate event receiver.");
            return FALSE;
        }
    }

    struct event_receiver_registration registration;
    registration.receiver = receiver;
    registration.callback = callback;
    memory_array_push(event->memory, &event->registries[code].registrations, registration);
    return TRUE;
}

b8 event_unregister_receiver(struct event* event, u16 code, void* receiver, event_callback_t callback)
{
    u64 registered_receiver_count = event->registries[code].registrations.length;
    for (u64 i = 0; i < registered_receiver_count; i += 1)
    {
        struct event_receiver_registration registration = event->registries[code].registrations.data[i];
        if (registration.receiver == receiver && registration.callback == callback)
        {
            registration = memory_array_pop_at(event->memory, &event->registries[code].registrations, i);
            return TRUE;
        }
    }

    ZZ_LOG_ERROR("Attempted to unregister a non-existent event registration.");
    return FALSE;
}

b8 event_send(struct event* event, u16 code, void* sender, union event_data data)
{
    u64 registered_receiver_count = event->registries[code].registrations.length;
    for (u64 i = 0; i < registered_receiver_count; i += 1)
    {
        struct event_receiver_registration registration = event->registries[code].registrations.data[i];
        if (registration.callback(sender, registration.receiver, data))
        {
            return TRUE;
        }
    }

    //ZZ_LOG_WARNING("Event was not handled.");
    return FALSE;
}

b8 event_send_null(struct event* event, u16 code, void* sender)
{
    union event_data data;
    data.null = (struct event_data_null){};
    return event_send(event, code, 0, data);
}