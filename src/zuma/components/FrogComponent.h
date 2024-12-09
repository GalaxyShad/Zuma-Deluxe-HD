#ifndef ZUMAHD_FROGCOMPONENT_H
#define ZUMAHD_FROGCOMPONENT_H

#include "global/HQC.h"

#include "../BallColors.h"
#include "zuma/Bullets.h"

typedef struct FrogComponent {
  HBulletList bulletList;

  BallColor currentBallColor;
  BallColor nextBallColor;

  float tongueExpand;
  float ballExpand;

  bool isFire;

  float fireRecoilTick;

  bool isMouseLeftPressed;
} FrogComponent;

#endif // ZUMAHD_FROGCOMPONENT_H
