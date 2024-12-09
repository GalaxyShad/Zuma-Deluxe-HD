#ifndef ZUMAHD_FROGENTITY_H
#define ZUMAHD_FROGENTITY_H

#include "../ecs/World.h"
#include "../Level.h"

Entity* FrogEntity_AddToWorld(World* world, float x, float y);

Entity* TestEntity_AddToWorld(World* world, float x, float y, float dist);

Entity* HudEntity_AddToWorld(World* world, HLevel level);

#endif // ZUMAHD_FROGENTITY_H
