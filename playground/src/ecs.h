#pragma once

#include <zz/zz.h>
#include <zz/memory.h>

struct ecs_entity
{

};
typedef struct {struct ecs_entity* data; u16 length, capacity;} memory_array_ecs_entity_t;

struct ecs_entity_config
{

};

struct ecs
{
    memory_array_ecs_entity_t entities;
};

struct ecs_config
{
    
};

b8 ecs_create(struct ecs* ecs, struct ecs_config* config);
void ecs_destroy(struct ecs* ecs);

b8 ecs_entity_create(struct ecs_entity* ecs, struct ecs_entity_config* config);
void ecs_entity_destroy(struct ecs_entity* ecs);