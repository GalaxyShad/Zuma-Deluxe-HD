#ifndef ZUMAHD_SYSTEM_H
#define ZUMAHD_SYSTEM_H

#include "Entity.h"

typedef void (*OnSystemCreateFunction)(int, void*, Entity*);
typedef void (*OnSystemUpdateFunction)(int, void*, Entity*);
typedef void (*OnSystemDrawFunction)(int, void*, Entity*);

typedef struct System {
  OnSystemCreateFunction onCreate;
  OnSystemUpdateFunction onUpdate;
  OnSystemDrawFunction   onDraw;
} System;

#endif // ZUMAHD_SYSTEM_H
