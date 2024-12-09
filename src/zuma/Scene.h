#pragma once

typedef void (*PFLoad)(void);
typedef void (*PFUpdate)(void);
typedef void (*PFDraw)(void);
typedef void (*PFUnload)(void);

typedef void* HScene;

HScene  Scene_New(const char* name, PFLoad load, PFUpdate update, PFDraw draw, PFUnload unload);
const char* Scene_GetName();
void Scene_Change(HScene nextScene);
void Scene_Update();
void Scene_Draw();

