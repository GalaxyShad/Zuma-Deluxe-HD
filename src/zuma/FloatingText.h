#ifndef ZUMAHD_FLOATINGTEXT_H
#define ZUMAHD_FLOATINGTEXT_H

#include "../global/HQC.h"

void FloatingTextFactory_Instantiate(float x, float y, uint32_t color, const char* format, ...);

void FloatingTextFactory_Init();
void FloatingTextFactory_Update();
void FloatingTextFactory_Draw();

#endif //ZUMAHD_FLOATINGTEXT_H
