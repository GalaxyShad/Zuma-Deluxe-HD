#include "Index.h"

#include "../Menu.h"


#define private static

private HButton _btn;

private void _OnBtnClick() {
    HQC_Log("Clicked!");
    Scene_Change(SC_GAME);
}

private void _Load() {
    _btn = Button_Create(700, 128);
    Button_OnClick(_btn, _OnBtnClick);
}

private void _Update() {
    Button_Update(_btn);
}

private void _Draw() {
    Button_Draw(_btn);
}

private void _Free() {
    Button_Destroy(_btn);
}

HScene Scene_Register_Test() {
    return Scene_New("test", _Load, _Update, _Draw, _Free);
}

void Scene_RegisterAll() {
    SC_GAME = Scene_Register_Game();
    SC_TEST = Scene_Register_Test();
}