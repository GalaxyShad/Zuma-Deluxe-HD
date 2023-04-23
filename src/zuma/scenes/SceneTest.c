#include "Index.h"

#include "../Menu.h"
#include "../ResourceStore.h"


#define private static

private HQC_VECTOR(HButton) _btnList;

private int spritesCount;
private int sprId;
private int sndId;

private void _OnBtnNextClick() { 
    sprId++; 
    sprId %= Store_CountSprites();
}


private void _OnBtnPrevClick() { 
    sprId--; 
    if (sprId < 0) sprId = Store_CountSprites()-1;
}


private void _OnBtnNextSndClick() { 
    HQC_DJ_PlaySound(Store_GetSoundByID(sndId)); 
    sndId++; 
    sndId %= Store_CountSounds();
}

private void _OnBtnPrevSndClick() {  
    HQC_DJ_PlaySound(Store_GetSoundByID(sndId)); 
    sndId--; 
    if (sndId < 0) sndId = Store_CountSounds()-1;
}

private void _Load() {
    _btnList = HQC_Container_CreateVector(sizeof(HButton));

    HButton btnNext = Button_Create(1100, 650);
    Button_OnClick(btnNext, _OnBtnNextClick);
    Button_SetText(btnNext, "next");

    HButton btnPrev = Button_Create(900, 650);
    Button_SetText(btnPrev, "prev");
    Button_OnClick(btnPrev, _OnBtnPrevClick);

    HButton btnNextSnd = Button_Create(1100, 650-100);
    Button_OnClick(btnNextSnd, _OnBtnNextSndClick);
    Button_SetText(btnNextSnd, "next");

    HButton btnPrevSnd = Button_Create(900, 650-100);
    Button_SetText(btnPrevSnd, "prev");
    Button_OnClick(btnPrevSnd, _OnBtnPrevSndClick);

    HQC_Container_VectorAdd(_btnList, &btnNext);
    HQC_Container_VectorAdd(_btnList, &btnPrev);
    HQC_Container_VectorAdd(_btnList, &btnNextSnd);
    HQC_Container_VectorAdd(_btnList, &btnPrevSnd);
}

private void _Update() {
    for (int i = 0; i < HQC_Container_VectorCount(_btnList); i++)
        Button_Update(*((HButton*)HQC_Container_VectorGet(_btnList, i)));

}

private void _Draw() {
    HQC_Artist_DrawSprite(Store_GetSpriteByID(sprId), 1280/2, 720/2);

    for (int i = 0; i < HQC_Container_VectorCount(_btnList); i++)
        Button_Draw(*((HButton*)HQC_Container_VectorGet(_btnList, i)));
}

private void _Free() {
    for (int i = 0; i < HQC_Container_VectorCount(_btnList); i++)
        Button_Destroy(*((HButton*)HQC_Container_VectorGet(_btnList, i)));
}

HScene Scene_Register_Test() {
    return Scene_New("test", _Load, _Update, _Draw, _Free);
}

void Scene_RegisterAll() {
    SC_GAME = Scene_Register_Game();
    SC_TEST = Scene_Register_Test();
}