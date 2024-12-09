#include "SpriteDrawSystem.h"

#include "../ecs/World.h"
#include "../components/PositionComponent.h"
#include "../components/ComponentID.h"

static void onDraw(int tag, void* comp, Entity* entity) {
  if (tag != COMPONENT_SPRITE) return;

  PositionComponent* pos = Entity_QueryComponent(entity, COMPONENT_POSITION);

  if (!pos) return;

  SpriteComponent* sprite = comp;

  TransformComponent* transform = Entity_QueryComponent(entity, COMPONENT_TRANSFORM);

  v2f_t display_pos = *pos;

  if (transform) {
    HQC_Artist_DrawSetAngle(transform->angle);

    if (transform->offsetRadial != 0.f) {
      float fcos = HQC_FCos(transform->angle);
      float fsin = -HQC_FSin(transform->angle);

      display_pos.x += fcos * transform->offsetRadial;
      display_pos.y += fsin * transform->offsetRadial;
    }
  }

  HQC_Artist_DrawSprite(sprite->sprite, display_pos.x, display_pos.y);

  HQC_Artist_DrawSetAngle(0);

  HQC_Log("%p", sprite->sprite);
}


System SpriteDrawSystem() {
  System result = {};

  result.onDraw = onDraw;

  return result;
}

