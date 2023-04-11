#include "Scene.h"

#include "../global/HQC.h"

typedef struct SceneProps {
    PFLoad      load;
    PFUpdate    update;
    PFDraw      draw;
    PFUnload    unload;

    const char* name;
} SceneProps;

static SceneProps* _curScene = NULL;

HScene Scene_New(const char* name, PFLoad load, PFUpdate update, PFDraw draw, PFUnload unload) {
    SceneProps* props = HQC_Memory_Allocate(sizeof(*props));

    props->load     = load;
    props->update   = update;
    props->draw     = draw;
    props->unload   = unload;

    props->name     = name;

    return props;
}


void Scene_Change(HScene nextScene) {
    if (_curScene != NULL) {
        _curScene->unload ? _curScene->unload() : HQC_RaiseErrorFormat("Scene %s has no Unload function", _curScene->name);
    }

    _curScene = nextScene;
    _curScene->load();
}


const char* Scene_GetName() {
    return _curScene->name;
}


void Scene_Update() {
    _curScene->update();
}


void Scene_Draw() {
    _curScene->draw();
}