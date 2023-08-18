#pragma once

#include "Bullets.h"

typedef void* HFrog;

HFrog Frog_Create(float x, float y, HBulletList bulletList);

void Frog_Update(HFrog hfrog);
void Frog_Draw(HFrog hfrog);

void Frog_Destroy(HFrog hfrog);