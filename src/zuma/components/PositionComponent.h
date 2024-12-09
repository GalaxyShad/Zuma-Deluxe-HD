#ifndef ZUMAHD_POSITIONCOMPONENT_H
#define ZUMAHD_POSITIONCOMPONENT_H

#include "global/UsefulTypes.h"
#include "global/HQC.h"

typedef v2f_t PositionComponent;

typedef struct TransformComponent {
  v2f_t scale;
  v2f_t offset;
  float offsetRadial;
  float angle;
} TransformComponent;

typedef struct SpriteComponent {
  TransformComponent transform;
  HQC_Sprite sprite;
} SpriteComponent;

typedef struct AnimationComponent {
  TransformComponent transform;
  HQC_Animation animation;
} AnimationComponent;

#endif // ZUMAHD_POSITIONCOMPONENT_H
