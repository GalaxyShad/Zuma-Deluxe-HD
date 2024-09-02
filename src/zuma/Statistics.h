#ifndef ZUMAHD_STATISTICS_H
#define ZUMAHD_STATISTICS_H

#include "BallColors.h"

void Statistics_Init();

void Statistics_AddExplodedBalls(int explodedBalls, BallColor ballsColor);
void Statistics_IncrementChain();
void Statistics_BreakChain();
void Statistics_AddBulletGap(float distance);

void Statistics_BuildAndInstantiateFloatingText(float x, float y);

#endif //ZUMAHD_STATISTICS_H
