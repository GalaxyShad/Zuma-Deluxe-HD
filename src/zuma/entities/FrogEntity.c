#include "FrogEntity.h"

#include "../components/PositionComponent.h"
#include "../components/ComponentID.h"
#include "../components/HudComponent.h"
#include "../components/FrogComponent.h"

#include "../ResourceStore.h"

Entity *FrogEntity_AddToWorld(World* world, float x, float y) {
  Entity* frog = Entity_Create(world);

  PositionComponent cPos = { x, y };

  SpriteComponent cSprPlate = { {}, Store_GetSpriteByID(SPR_FROG_PLATE) };
  SpriteComponent cSprFrog = { {}, Store_GetSpriteByID(SPR_FROG) };
  SpriteComponent cSprTongue = { {}, Store_GetSpriteByID(SPR_FROG_TONGUE) };

  HQC_ECS_Entity_AddComponent$(frog, COMPONENT_POSITION, cPos);

  HQC_ECS_Entity_AddComponent$(frog, COMPONENT_SPRITE, cSprTongue);
  HQC_ECS_Entity_AddComponent$(frog, COMPONENT_SPRITE, cSprFrog);
  HQC_ECS_Entity_AddComponent$(frog, COMPONENT_SPRITE, cSprPlate);

  FrogComponent cFrog = {
      .ballExpand = 32,
      .tongueExpand = 24,
      .currentBallColor = HQC_RandomRange(0, 3),
      .nextBallColor = HQC_RandomRange(0, 3),
      .fireRecoilTick = 0,
      .isFire = false,
  };

  HQC_ECS_Entity_AddComponent$(frog, COMPONENT_FROG, cFrog);

  return frog;
}

Entity *TestEntity_AddToWorld(World *world, float x, float y, float dist) {
  Entity* test = Entity_Create(world);

  PositionComponent cPos = { x, y };

  SpriteComponent cSpr = { {}, Store_GetSpriteByID(SPR_MENU_HEAD) };
  TransformComponent cTransform = {};
  cTransform.angle = 3.14f / 4;
  cTransform.offsetRadial = dist;

  HQC_ECS_Entity_AddComponent$(test, COMPONENT_POSITION, cPos);
  HQC_ECS_Entity_AddComponent$(test, COMPONENT_SPRITE, cSpr);
  HQC_ECS_Entity_AddComponent$(test, COMPONENT_TRANSFORM, cTransform);

  return test;
}

Entity *HudEntity_AddToWorld(World *world, HLevel level) {
  Entity* hud = Entity_Create(world);

  HudComponent cHud = {
      .level = level
  };

  HQC_ECS_Entity_AddComponent$(hud, COMPONENT_HUD, cHud);

  return hud;
}
