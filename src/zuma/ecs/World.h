#ifndef ZUMAHD_WORLD_H
#define ZUMAHD_WORLD_H

#include "System.h"

World *World_Create();
void World_Destroy(World *world);

void World_AddSystem(World *world, System system);
void World_RunSystems(World* world);
void World_DrawSystems(World* world);

#define HQC_ECS_Entity_AddComponent$(entity, tag, component) \
  Entity_AddComponent(entity, tag, &component, sizeof(component))

Entity* Entity_Create(World* world);
void Entity_Destroy(Entity* entity);

void* Entity_AddComponent(Entity *entity, int tagId, void* component, long componentSize);
void* Entity_QueryComponent(Entity* entity, int tagId);
void Entity_QueryComponentAll(Entity* entity, int tagId, void* componentListStart, int componentListSize);

#endif // ZUMAHD_WORLD_H
