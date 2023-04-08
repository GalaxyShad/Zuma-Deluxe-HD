#pragma once

typedef void* HFrog;

HFrog Frog_Create(float x, float y);

void Frog_Update(HFrog hfrog);
void Frog_Draw(HFrog hfrog);

void Frog_Destroy(HFrog hfrog);