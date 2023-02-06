#include "ecs.h"

b8 ecs_create(struct ecs* ecs, struct ecs_config* config)
{
    memory_array_create(&ecs->entities);

    return ZZ_TRUE;
}

void ecs_destroy(struct ecs* ecs)
{
    memory_array_destroy(&ecs->entities);
}