#include "SpriteDrawSystem.h"

#include "../components/PositionComponent.h"
#include "../components/ComponentID.h"
#include "../components/HudComponent.h"
#include "../components/FrogComponent.h"
#include "../ecs/World.h"

#include "zuma/Level.h"
#include "zuma/ResourceStore.h"

#include <math.h>

//static v2f_t getBallPos_(Frog* frog) {
//  float fcos = HQC_FCos(frog->angle);
//  float fsin = HQC_FSin(frog->angle);
//
//  v2f_t pos = {
//      frog->pos.x + frog->ballExpand * fcos,
//      frog->pos.y + frog->ballExpand * fsin
//  };
//
//  return pos;
//}

static void onUpdate_(int tag, void* comp, Entity* entity) {
  if (tag != COMPONENT_FROG) return;

  FrogComponent* frog = entity;
  TransformComponent* transform = Entity_QueryComponent(entity, COMPONENT_TRANSFORM);
  PositionComponent* pos = Entity_QueryComponent(entity, COMPONENT_POSITION);

  v2i_t mousePos = HQC_Input_MouseGetPosition();

  transform->angle = atan2f((float)mousePos.y - pos->y, (float)mousePos.x - pos->y);

  frog->isMouseLeftPressed = false;

  if (HQC_Input_MouseLeftPressed() && !frog->isMouseLeftPressed && !frog->isFire) {
    HQC_DJ_PlaySound(Store_GetSoundByID(SND_FIREBALL1));

    //  HQC_Animation_SetSpeed(frog->animBlink, 0.5);

    //BulletList_Add(frog->bulletList, frog->currentBallColor, )
  }

}

System FrogSystem() {
  System result = {};

  result.onUpdate = onUpdate_;

  return result;
}