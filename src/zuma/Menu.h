#pragma once

#include "../global/HQC.h"

///////////////////////////////////////////////////////////////////////

typedef void* HButton;

HButton Button_Create(float x, float y);

void Button_SetSprite(HButton hbutton, HQC_Sprite sprite);
void Button_SetSpriteHover(HButton hbutton, HQC_Sprite sprHover);
void Button_SetSpritePressed(HButton hbutton, HQC_Sprite sprPressed);

void Button_SetSoundHover(HButton hbutton, HQC_Sound sndHover);
void Button_SetSoundPressed(HButton hbutton, HQC_Sound sndPressed);

void Button_SetRectDivider(HButton hbutton, int divWidth);
void Button_SetText(HButton hbutton, const char* text);

void Button_OnClick(HButton hbutton, void (*onButtonClickHandle)(void) );

void Button_Update(HButton hbutton);
void Button_Draw(HButton hbutton);

void Button_Destroy(HButton hbutton);

///////////////////////////////////////////////////////////////////////

typedef void* HCheckbox;

HCheckbox Checkbox_Create(float x, float y);

void Checkbox_OnChecked(HCheckbox hcheckbox, void (*onCheckedFunction)(void));
void Checkbox_OnUnchecked(HCheckbox hcheckbox, void (*onUncheckedFunction)(void));

void Checkbox_Update(HCheckbox hcheckbox);
void Checkbox_Draw(HCheckbox hcheckbox);

void Checkbox_Destroy(HCheckbox hcheckbox);

///////////////////////////////////////////////////////////////////////

typedef void* HSlider;

HSlider Slider_Create(float x, float y);

void Slider_OnChange(HSlider hslider, void (*onChange)(float));

void Slider_Update(HSlider hslider);
void Slider_Draw(HSlider hslider);

void Slider_Destroy(HSlider hslider);

///////////////////////////////////////////////////////////////////////

typedef void* HDialogbox;

HDialogbox Dialogbox_Create(float x, float y);

void Dialogbox_AddButton(HDialogbox hdialogbox, HButton hbutton);
void Dialogbox_AddSlider(HDialogbox hdialogbox, HSlider hslider);
void Dialogbox_AddCheckbox(HDialogbox hdialogbox, HCheckbox hcheckbox);

void Dialogbox_SetTitle(HDialogbox hdialogbox, const char* title);

void Dialogbox_AddText(HDialogbox hdialogbox, const char* text, float x, float y);

void Dialogbox_Update(HDialogbox hdialogbox);
void Dialogbox_Draw(HDialogbox hdialogbox);

void Dialogbox_Destroy(HDialogbox hdialogbox);
