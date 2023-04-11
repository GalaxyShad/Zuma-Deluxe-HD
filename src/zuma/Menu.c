#include "Menu.h"

#include "ResourceStore.h"

enum _ButtonState {
    BTN_IDLE,
    BTN_HOVERED,
    BTN_PRESSED
};

typedef struct Button {
    const char* text;

    int scale;

    v2i_t size;
    v2f_t pos;

    int state;

    HQC_Sound sndHover;
    HQC_Sound sndPressed;

    HQC_Sprite spr;
    HQC_Sprite sprHover;
    HQC_Sprite sprPressed;

    void (*onClick)(void);
} Button;


static void _Button_UpdateRect(HButton hbutton) {
    Button* btn = (Button*)hbutton;

    irect_t rect    = HQC_Sprite_GetRect(btn->spr);
    btn->size.x   = rect.width;
    btn->size.y   = rect.height;
}

static Button* _Btn(HButton hbutton) { return (Button*)hbutton; }

HButton Button_Create(float x, float y) {
    Button* btn = HQC_Memory_Allocate(sizeof(*btn));

    btn->pos.x      = x;
    btn->pos.y      = y;

    btn->text       = "button";

    btn->state      = BTN_IDLE;

    btn->sndHover   = NULL;
    btn->sndPressed = Store_GetSoundByID(SND_BUTTON1);

    btn->spr        = Store_GetSpriteByID(SPR_MENU_BUTTON);
    btn->sprHover   = Store_GetSpriteByID(SPR_MENU_BUTTON_HOVER);
    btn->sprPressed = Store_GetSpriteByID(SPR_MENU_BUTTON_PRESSED);

    btn->onClick    = NULL;

    btn->scale      = 1;

    _Button_UpdateRect(btn);

    return btn;
}


void Button_SetSprite(HButton hbutton, HQC_Sprite sprite) {
    _Btn(hbutton)->spr = sprite;
    _Button_UpdateRect(_Btn(hbutton));
}


void Button_SetSpriteHover(HButton hbutton, HQC_Sprite sprHover) {
    _Btn(hbutton)->sprHover = sprHover;
}


void Button_SetSpritePressed(HButton hbutton, HQC_Sprite sprPressed) {
    _Btn(hbutton)->sprPressed = sprPressed;
}


void Button_SetSoundHover(HButton hbutton, HQC_Sound sndHover) {
    _Btn(hbutton)->sndHover = sndHover;
}


void Button_SetSoundPressed(HButton hbutton, HQC_Sound sndPressed) {
    _Btn(hbutton)->sndHover = sndPressed;
}


void Button_SetText(HButton hbutton, const char* text) {
    _Btn(hbutton)->text = text;
}


void Button_OnClick(HButton hbutton, void (*onButtonClickHandle)(void)) {
    _Btn(hbutton)->onClick = onButtonClickHandle;
}


void Button_SetScale(HButton hbutton, int scale) {
    if (scale < 0) HQC_RaiseErrorFormat("Button scale cannot be %d", scale);

    _Btn(hbutton)->scale = scale;
}

static int _Button_GetSlice(Button* btn) {
    return btn->size.x / 3;
}

static irect_t _Button_GetRect(Button* btn) {
    int slice = _Button_GetSlice(btn);

    int rx = ((slice * (2+btn->scale)) / 2);
    int ry = btn->size.y / 2;

    irect_t rect = {
        btn->pos.x - rx,
        btn->pos.y - ry,
        rx*2,
        ry*2
    };

    return rect;
}

void Button_Update(HButton hbutton) {
    Button* btn = _Btn(hbutton);

    v2i_t mpos = HQC_Input_MouseGetPosition();

    irect_t rect = _Button_GetRect(btn);

    if ((mpos.x > rect.x) && (mpos.y > rect.y) && 
        (mpos.x < (rect.x+rect.width)) && (mpos.y < (rect.y+rect.height))
    ) {
        if (HQC_Input_MouseLeftPressed()) {
            if (btn->state != BTN_PRESSED) {
                btn->state = BTN_PRESSED;
                btn->sndPressed ? HQC_DJ_PlaySound(btn->sndPressed) : NULL;
            } else {
                // btn->onClick ? btn->onClick() : NULL;
            }
        } else {
            if (btn->state == BTN_PRESSED) {
                btn->onClick ? btn->onClick() : NULL;
            }

            if (btn->state != BTN_HOVERED)
                btn->sndHover ? HQC_DJ_PlaySound(btn->sndHover) : NULL;


            btn->state = BTN_HOVERED;
        }

        return;
    }

    btn->state = BTN_IDLE;
}


void Button_Draw(HButton hbutton) {
    Button* btn = _Btn(hbutton);

    int slice    = _Button_GetSlice(btn);
    irect_t rect = _Button_GetRect(btn);

    HQC_Texture tex = HQC_Sprite_GetTexture(btn->spr);

    irect_t texRect = HQC_Sprite_GetRect(
        (btn->sprPressed && btn->state == BTN_PRESSED) ? btn->sprPressed :
        (btn->sprHover   && btn->state == BTN_HOVERED) ? btn->sprHover   : btn->spr);
    texRect.width = slice;

    HQC_Artist_DrawTextureRectLeft(tex, rect.x, rect.y, texRect);
    texRect.x   += slice;
    rect.x      += slice;
    for (int i = 0; i < btn->scale; i++) {
        HQC_Artist_DrawTextureRectLeft(tex, rect.x, rect.y, texRect);
        rect.x += slice;
    }
    texRect.x += slice;
    HQC_Artist_DrawTextureRectLeft(tex, rect.x, rect.y, texRect);

    //////////////////////////////////////////////////////////////////////////////////

    if (btn->text != NULL) {
        int shift = (btn->state == BTN_PRESSED) ? 4 : 0;

        HQC_Artist_SetColorHex(C_YELLOW);
        HQC_Artist_DrawTextShadow(
            Store_GetFontByID(FONT_CANCUN_8),
            btn->text, btn->pos.x - shift, btn->pos.y-10 + shift
        );
        HQC_Artist_SetColorHex(C_WHITE);
    }

}


void Button_Destroy(HButton hbutton) {
    HQC_Memory_Free(hbutton);
    hbutton = NULL;
}