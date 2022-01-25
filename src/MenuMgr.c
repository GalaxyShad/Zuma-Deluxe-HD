#include "MenuMgr.h"


void Button_Init(Button* btn) {
    btn->state = BTN_IDLE;
    btn->clicked = false;
    btn->pressed = false;

    btn->textureID = TEX_MENU;

    btn->soundHoverID       = -1;
    btn->soundHoverSfxID    = -1;
    btn->sndSfxPitch        = 0.0;
    btn->soundClickID       = SND_BUTTON1;
    btn->deadBorder = 0;

    btn->withText = false;

    btn->tRect.x = 0;
    btn->tRect.y = 532;
    btn->tRect.w = 180;
    btn->tRect.h = 72;

    btn->xscale = 1;

    btn->pos.x = 0;
    btn->pos.y = 0;
}

void Button_SetText(Button* btn, const char* str) {
    strcpy(btn->text, str);
    btn->withText = true;
}

void Button_Update(Button* btn) {
    int mx, my;
    Engine_GetMousePos(&mx, &my);

    int bWidth = btn->tRect.w + (btn->tRect.w / 3) * (btn->xscale-1);

    if (mx > btn->pos.x + btn->deadBorder && 
        my > btn->pos.y + btn->deadBorder &&
        mx < btn->pos.x+bWidth - btn->deadBorder &&
        my < btn->pos.y+btn->tRect.h - btn->deadBorder) {
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                if (btn->state != BTN_CLICKED) {
                    btn->state = BTN_CLICKED;
                    Engine_PlaySound(btn->soundClickID);
                }
            } else {
                if (btn->state == BTN_CLICKED)
                    btn->clicked = true;
                else 
                    btn->clicked = false;

                if (!btn->state == BTN_HOVER) {
                    if (btn->soundHoverSfxID != -1) {
                        Engine_PlaySoundSfxPitch(btn->soundHoverSfxID, btn->sndSfxPitch);
                    } else if (btn->soundHoverID != -1) {
                        Engine_PlaySound(btn->soundHoverID);
                    }
                }

                btn->state = BTN_HOVER;
            }
    } else {
        btn->state = BTN_IDLE;
        btn->clicked = false;
    }
}

int Button_IsClicked(Button* btn) {
    return btn->clicked;
}

void Button_Draw(Button* btn) {
    SDL_Texture* tex = Engine_GetTextureSDL(btn->textureID);

    SDL_Rect dstRect = {
        btn->pos.x, 
        btn->pos.y, 
        btn->tRect.w,
        btn->tRect.h,
    };

    SDL_Rect srcRect = btn->tRect;
    if (btn->xscale == 1) {
        srcRect = btn->tRect;

        if (btn->state == BTN_HOVER)
            srcRect.x = btn->tRect.x + btn->tRect.w;
        else if (btn->state == BTN_CLICKED || btn->pressed)
            srcRect.x = btn->tRect.x + 2 * btn->tRect.w;
        else 
            srcRect.x = btn->tRect.x;

        SDL_RenderCopy(
            engine.render, 
            tex,
            &srcRect,
            &dstRect
        );
    } else {
        srcRect.w = btn->tRect.w / 3;
        dstRect.w = srcRect.w;
        int dx = 0;
        for (int i = 0; i < 2+btn->xscale; i++) {
            if (i == 0) dx = 0;
            else if (i == 2+btn->xscale-1) dx = 2;
            else dx = 1;


            if (btn->state == BTN_HOVER)
                srcRect.x = srcRect.w*dx + btn->tRect.x + btn->tRect.w;
            else if (btn->state == BTN_CLICKED)
                srcRect.x = srcRect.w*dx + btn->tRect.x + 2 * btn->tRect.w;
            else 
                srcRect.x = srcRect.w*dx + btn->tRect.x;

            SDL_RenderCopy(
                engine.render, 
                tex,
                &srcRect,
                &dstRect
            );

            dstRect.x += srcRect.w;
        }
    }

    // DrawText
    if (btn->withText) {
        int bWidth = btn->tRect.w + (btn->tRect.w / 3) * btn->xscale;
        int cx = btn->pos.x + bWidth / 2 - 24;

        int dx = 0;
        int dy = 0;

        SDL_Color color = {255, 255, 255, 255};
        if (btn->state == BTN_IDLE) 
            color.b = 0;

        if (btn->state == BTN_CLICKED) {
            dx = -2;
            dy = +2;
        }

        Engine_DrawTextExt(
            btn->text,
            FONT_CANCUN_10,
            color,
            true,
            true,
            cx + dx,
            btn->pos.y + 18 + dy
        );
    }

}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void CheckBox_Init(CheckBox* cb) {
    cb->checked = false;
    cb->clicked = false;

    cb->pos.x = 0;
    cb->pos.y = 0;
}

void CheckBox_Update(CheckBox* cb) {
    int mx, my;
    Engine_GetMousePos(&mx, &my);

    if (mx > cb->pos.x && 
        my > cb->pos.y &&
        mx < cb->pos.x+CHECK_BOX_WIDTH &&
        my < cb->pos.y+CHECK_BOX_HEIGHT) {
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    if (!cb->clicked) {
                        Engine_PlaySound(SND_BUTTON2);
                        cb->checked = !cb->checked;
                        cb->clicked = true;
                    }
            } else {
                cb->clicked = false;
            }
    } 
}

int CheckBox_IsClicked(CheckBox* cb) {
    return cb->clicked;
}

int CheckBox_IsChecked(CheckBox* cb) {
    return cb->checked;
}

void CheckBox_Draw(CheckBox* cb) {
    SDL_Texture* texture = Engine_GetTextureSDL(TEX_MENU);

    SDL_Rect srcRect = {
        CHECK_BOX_WIDTH * !cb->checked, 
        462, 
        CHECK_BOX_WIDTH, 
        CHECK_BOX_HEIGHT
    };

    SDL_Rect dstRect = {
        cb->pos.x,
        cb->pos.y,
        CHECK_BOX_WIDTH,
        CHECK_BOX_HEIGHT
    };

    SDL_RenderCopy(
        engine.render,
        texture,
        &srcRect,
        &dstRect
    );
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void Slider_Init(Slider* s, float* val) {
    s->pos.x = 0;
    s->pos.y = 0;

    s->drag = false;

    s->value = val;
    s->maxValue = 1.0f;

    s->thumbPos = (SLIDER_THUMB_MAX * (*s->value)) / s->maxValue;
}

void Slider_Update(Slider* s) {
    int mx, my;
    Engine_GetMousePos(&mx, &my);

    if (mx > s->pos.x + s->thumbPos && 
        my > s->pos.y &&
        mx < s->pos.x + s->thumbPos + SLIDER_THUMB_WIDTH &&
        my < s->pos.y+SLIDER_HEIGHT) {
            if (SDL_GetMouseState(NULL, NULL) & 
                SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    s->drag = true;
            } else {
                s->drag = false;
            }
    }

    if (s->drag) {
         if (!(SDL_GetMouseState(NULL, NULL) & 
                SDL_BUTTON(SDL_BUTTON_LEFT)))
                    s->drag = false;

        s->thumbPos = mx - s->pos.x - SLIDER_THUMB_WIDTH / 2;

        if (s->thumbPos < 0)
            s->thumbPos = 0;
        if (s->thumbPos > SLIDER_THUMB_MAX)
            s->thumbPos = SLIDER_THUMB_MAX;
        
        *s->value = (s->maxValue * s->thumbPos) / SLIDER_THUMB_MAX;
    }
}

int Slider_IsDragging(Slider* s) {
    return s->drag;
}

void Slider_Draw(Slider* s) {
    SDL_Texture* texture = Engine_GetTextureSDL(TEX_MENU);

    SDL_Rect srcRect = {0, 365,  307, 49};
    SDL_Rect dstRect = {s->pos.x, s->pos.y, 307, 49};

    SDL_RenderCopy(
        engine.render,
        texture,
        &srcRect,
        &dstRect
    );

    // Thumb
    SDL_Rect srcSlRect = {0, 414,  61, 45};
    SDL_Rect dstSlRect = {
        s->pos.x + s->thumbPos, 
        s->pos.y, 
        61, 
        49
    };

    SDL_RenderCopy(
        engine.render,
        texture,
        &srcSlRect,
        &dstSlRect
    );
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void DialogueBoxText_Init(DialogueBoxText* dbt) {
    dbt->FontID = FONT_CANCUN_10;
    strcpy(dbt->str, "ERROR");

    dbt->pos.x = 0;
    dbt->pos.y = 0;

    dbt->color.a = 255;
    dbt->color.r = 255;
    dbt->color.g = 255;
    dbt->color.b = 0;
}

DialogueBox* DialogueBox_Create() {
    DialogueBox* db = malloc(sizeof(DialogueBox));
    if (!db)
        return NULL;

    db->drawGodHead = false;
    db->animation  = false;
    db->exitButton = 0;
    db->pressedBtn = 0;

    db->sizeW = 3;
    db->sizeH = 3;

    db->pos.x = WINDOW_WIDTH  / 2;
    db->pos.y = WINDOW_HEIGHT / 2;

    db->startY = db->pos.y;

    strcpy(db->caption, "test");

    db->textsLen = 0;
    db->texts = NULL;

    db->buttonsLen = 0;
    db->buttons = NULL;

    db->chBoxesLen = 0;
    db->chBoxes = NULL;

    db->slidersLen = 0;
    db->sliders = NULL;

    return db;
}

void DialogueBox_AddButton(DialogueBox* db, Button* btn) {
    Button* ptr = realloc(
        db->buttons, 
        sizeof(Button) * (db->buttonsLen+1)
    );

    if (!ptr)
        return;

    db->buttons = ptr;
    db->buttons[db->buttonsLen] = *btn;
    db->buttons[db->buttonsLen].pos.x = db->pos.x + btn->pos.x;
    db->buttons[db->buttonsLen].pos.y = db->pos.y + btn->pos.y;
    db->buttonsLen++;
}

void DialogueBox_AddCheckBox(DialogueBox* db, CheckBox* cb) {
    CheckBox* ptr = realloc(
        db->chBoxes, 
        sizeof(CheckBox) * (db->chBoxesLen+1)
    );

    if (!ptr)
        return;

    db->chBoxes = ptr;
    db->chBoxes[db->chBoxesLen] = *cb;
    db->chBoxes[db->chBoxesLen].pos.x = db->pos.x + cb->pos.x;
    db->chBoxes[db->chBoxesLen].pos.y = db->pos.y + cb->pos.y;
    db->chBoxesLen++;
}

void DialogueBox_AddSlider(DialogueBox* db, Slider* s) {
    Slider* ptr = realloc(
        db->sliders, 
        sizeof(Slider) * (db->slidersLen+1)
    );

    if (!ptr)
        return;

    db->sliders = ptr;
    db->sliders[db->slidersLen] = *s;
    db->sliders[db->slidersLen].pos.x = db->pos.x + s->pos.x;
    db->sliders[db->slidersLen].pos.y = db->pos.y + s->pos.y;
    db->slidersLen++;
}

void DialogueBox_AddText(DialogueBox* db, DialogueBoxText* t) {
    DialogueBoxText* ptr = realloc(
        db->texts, 
        sizeof(DialogueBoxText) * (db->textsLen+1)
    );

    if (!ptr)
        return;

    db->texts = ptr;
    db->texts[db->textsLen] = *t;

    db->texts[db->textsLen].pos.x = db->pos.x + t->pos.x;
    db->texts[db->textsLen].pos.y = db->pos.y + t->pos.y;

    db->textsLen++;
}

void DialogueBox_SetTextStr(DialogueBox* db, int textID, const char* str) {
    if (textID < 0 || textID >= db->textsLen)
        return;

    strcpy(db->texts[textID].str, str);
}

void DialogueBox_Animate(DialogueBox* db) {
    db->animation = true;

    int dy = WINDOW_HEIGHT + (121 * db->sizeH) / 2;
    db->pos.y += dy;

    for (int i = 0; i < db->buttonsLen; i++) 
        db->buttons[i].pos.y += dy;
    for (int i = 0; i < db->chBoxesLen; i++)
        db->chBoxes[i].pos.y += dy;
    for (int i = 0; i < db->slidersLen; i++)
        db->sliders[i].pos.y += dy;
    for (int i = 0; i < db->textsLen; i++)
        db->texts[i].pos.y += dy;
}

int DialogueBox_GetBtn(DialogueBox* db) {
    int btn = db->pressedBtn;
    db->pressedBtn = 0;
    return btn;
}

void DialogueBox_Update(DialogueBox* db) {

    if (db->animation)
        return;

    for (int i = 0; i < db->buttonsLen; i++) {
        Button_Update(&db->buttons[i]);

        if (Button_IsClicked(&db->buttons[i]))
            db->pressedBtn = i+1;
    }

    for (int i = 0; i < db->chBoxesLen; i++)
        CheckBox_Update(&db->chBoxes[i]);

    for (int i = 0; i < db->slidersLen; i++)
        Slider_Update(&db->sliders[i]);
}

void DialogueBox_Draw(DialogueBox* db) {

    if (db->animation) {
        if (db->pos.y > db->startY) {
            int spd = min(DB_ANIM_SPD, db->pos.y-db->startY);
            db->pos.y -= spd;
            for (int i = 0; i < db->buttonsLen; i++) 
                db->buttons[i].pos.y -= spd;
            for (int i = 0; i < db->chBoxesLen; i++)
                db->chBoxes[i].pos.y -= spd;
            for (int i = 0; i < db->slidersLen; i++)
                db->sliders[i].pos.y -= spd;
            for (int i = 0; i < db->textsLen; i++)
                db->texts[i].pos.y -= spd;
        } else {
            db->animation = false;
        }
    }

    SDL_Texture* texture = Engine_GetTextureSDL(TEX_MENU);

    SDL_Point dPos;
    dPos.x = db->pos.x - (121 * db->sizeW) / 2;
    dPos.y = db->pos.y - (121 * db->sizeH) / 2;

    SDL_Rect srcBgRect = {0, 0, 121, 121};
    SDL_Rect dstBgRect = {0, 0, 121, 121};
    for (int i = 0; i < db->sizeH; i++) {
        if (i == 0)
            srcBgRect.y = 0;
        else if (i == db->sizeH-1)
            srcBgRect.y = 242;
        else 
            srcBgRect.y = 121;

        for (int j = 0; j < db->sizeW; j++) {
            if (j == 0)
                srcBgRect.x = 0;
            else if (j == db->sizeW-1)
                srcBgRect.x = 242;
            else 
                srcBgRect.x = 121;

            dstBgRect.x = dPos.x + 121 * j;
            dstBgRect.y = dPos.y + 121 * i;

            SDL_RenderCopy(
                engine.render,
                texture,
                &srcBgRect,
                &dstBgRect
            );
        }
    }

    // God Head
    if (db->drawGodHead) {
        SDL_Rect srcRect = {363, 0, 234, 210};
        SDL_Rect dstRect = {
            db->pos.x - 117, 
            dPos.y - 112, 
            234, 
            210
        };

        SDL_RenderCopy(
            engine.render,
            texture,
            &srcRect,
            &dstRect
        );
    }

    // Caption
    SDL_Color cColor = {255, 255, 255, 255};
    int cDy = 32;
    if (db->drawGodHead)
        cDy = 72;
    Engine_DrawTextExt(
        db->caption, 
        FONT_NATIVE_ALIEN_EXT_18,
        cColor,
        false, 
        true,
        db->pos.x,
        dPos.y + cDy
    );

    for (int i = 0; i < db->buttonsLen; i++)
        Button_Draw(&db->buttons[i]);

    for (int i = 0; i < db->chBoxesLen; i++) 
        CheckBox_Draw(&db->chBoxes[i]);

    for (int i = 0; i < db->slidersLen; i++)
        Slider_Draw(&db->sliders[i]);

    for (int i = 0; i < db->textsLen; i++) {
        Engine_DrawTextExt(
            db->texts[i].str,
            db->texts[i].FontID,
            db->texts[i].color,
            true,
            false,
            db->texts[i].pos.x,
            db->texts[i].pos.y
        );
    }
}

void DialogueBox_Destroy(DialogueBox* db) {
    db->buttonsLen = 0;
    free(db->buttons);
    db->chBoxesLen = 0;
    free(db->chBoxes);
    db->slidersLen = 0;
    free(db->sliders);
    db->textsLen = 0;
    free(db->texts);

    free(db);
    db = NULL;
}


DialogueBox* DialogueBox_CreateOptions() {
    DialogueBox* db = DialogueBox_Create();
    db->sizeW = 5;
    db->sizeH = 4;
    db->exitButton = 1;
    strcpy(db->caption, "OPTIONS");

    Slider slMus;
    Slider_Init(&slMus, &engine.volMus);
    slMus.pos.x = -56;
    slMus.pos.y = -128;
    DialogueBox_AddSlider(db, &slMus);

    DialogueBoxText tMus;
    DialogueBoxText_Init(&tMus);
    tMus.pos.x = slMus.pos.x - 170;
    tMus.pos.y = slMus.pos.y + 10;
    strcpy(tMus.str, "MUSIC VOLUME");
    DialogueBox_AddText(db, &tMus);

    Slider slSnd;
    Slider_Init(&slSnd, &engine.volSnd);
    slSnd.pos.x = slMus.pos.x;
    slSnd.pos.y = slMus.pos.y + 64;
    DialogueBox_AddSlider(db, &slSnd);

    DialogueBoxText tSnd;
    DialogueBoxText_Init(&tSnd);
    tSnd.pos.x = slSnd.pos.x - 170;
    tSnd.pos.y = slSnd.pos.y + 10;
    strcpy(tSnd.str, "SOUNDS VOLUME");
    DialogueBox_AddText(db, &tSnd);

    CheckBox cbFullscr;
    CheckBox_Init(&cbFullscr);
    cbFullscr.checked = engine.fullScr;
    cbFullscr.pos.x = -236;
    cbFullscr.pos.y = slSnd.pos.y + 48;
    DialogueBox_AddCheckBox(db, &cbFullscr);

    DialogueBoxText tFullscr;
    DialogueBoxText_Init(&tFullscr);
    tFullscr.pos.x = cbFullscr.pos.x + 80;
    tFullscr.pos.y = cbFullscr.pos.y + 24;
    strcpy(tFullscr.str, "FULLSCREEN");
    DialogueBox_AddText(db, &tFullscr);

    Button bDone;
    Button_Init(&bDone);
    Button_SetText(&bDone, "DONE");
    bDone.xscale = 6;
    bDone.pos.x = -236;
    bDone.pos.y = 120;
    DialogueBox_AddButton(db, &bDone);

    return db;
}

void DialogueBox_UpdateOptions(DialogueBox* db) {
    DialogueBox_Update(db);

    if (CheckBox_IsClicked(&db->chBoxes[0])) {
        if (CheckBox_IsChecked(&db->chBoxes[0])) {
            SDL_SetWindowFullscreen(
                engine.win, SDL_WINDOW_FULLSCREEN);
            engine.fullScr = true;
        } else {
            SDL_SetWindowFullscreen(engine.win, 0);
            engine.fullScr = false;
        }
    }

    if (Slider_IsDragging(&db->sliders[0])) {
        BASS_ChannelSetAttribute(engine.music, 
            BASS_ATTRIB_VOL, engine.volMus);
    }
}

DialogueBox* DialogueBox_CreateMenu() {
    DialogueBox* db = DialogueBox_CreateOptions();
    strcpy(db->caption, "mENU");

    strcpy(db->buttons[0].text, "BACK TO GAME");

    Button bMMenu;
    Button_Init(&bMMenu);
    Button_SetText(&bMMenu, "RETURN TO MAIN MENU");
    bMMenu.xscale = db->buttons[0].xscale;
    bMMenu.pos.x = -236;
    bMMenu.pos.y = 48;
    DialogueBox_AddButton(db, &bMMenu);

    return db;
}

void DialogueBox_UpdateMenu(DialogueBox* db) {
    DialogueBox_UpdateOptions(db);
}

DialogueBox* DialogueBox_CreateResults() {
    DialogueBox* db = DialogueBox_Create();
    strcpy(db->caption, "STATS");
    db->sizeW = 6;
    db->sizeH = 4;
    db->drawGodHead = true;

    // Param Names
    DialogueBoxText tParamNames[7];
    for (int i = 0; i < 7; i++) {
        DialogueBoxText_Init(&tParamNames[i]);
        tParamNames[i].FontID = FONT_CANCUN_12;
        if (i < 5) {
            tParamNames[i].pos.x = -262;
            tParamNames[i].pos.y = -100 + (28 * i);
        } else {
            tParamNames[i].pos.x = 32;
            tParamNames[i].pos.y = -100 + (28 * (i-5));
        }
    }

    strcpy(tParamNames[0].str, "POINTS");
    strcpy(tParamNames[1].str, "COMBOS");
    strcpy(tParamNames[2].str, "COINS");
    strcpy(tParamNames[3].str, "MAX CHAIN");
    strcpy(tParamNames[4].str, "MAX COMBO");
    strcpy(tParamNames[5].str, "YOUR TIME");
    strcpy(tParamNames[6].str, "ACE TIME");

    for (int i = 0; i < 7; i++)
        DialogueBox_AddText(db, &tParamNames[i]);

    // Params
    DialogueBoxText tParam[7];
    for (int i = 0; i < 7; i++) {
        DialogueBoxText_Init(&tParam[i]);
        tParam[i].FontID = FONT_CANCUN_12;
        tParam[i].color.g = 152;
        tParam[i].pos.x = tParamNames[i].pos.x + 185;
        tParam[i].pos.y = tParamNames[i].pos.y;
        strcpy(tParam[i].str, "NULL");

        DialogueBox_AddText(db, &tParam[i]);
    }

    // Button
    Button bOk;
    Button_Init(&bOk);
    Button_SetText(&bOk, "OK");
    bOk.xscale = 8;
    bOk.pos.x = -297;
    bOk.pos.y = 120;
    DialogueBox_AddButton(db, &bOk);

    DialogueBox_Animate(db);

    return db;
}

DialogueBox* DialogueBox_CreateGameOver() {
    DialogueBox* db = DialogueBox_Create();
    strcpy(db->caption, "GAmE OvER");
    db->sizeW = 6;
    db->sizeH = 4;
    db->drawGodHead = true;

    // Param Names
    DialogueBoxText tParamNames[5];
    for (int i = 0; i < 5; i++) {
        DialogueBoxText_Init(&tParamNames[i]);
        tParamNames[i].FontID = FONT_CANCUN_12;
        if (i < 3) {
            tParamNames[i].pos.x = -262;
            tParamNames[i].pos.y = -100 + (28 * i);
        } else {
            tParamNames[i].pos.x = 32;
            tParamNames[i].pos.y = -100 + (28 * (i-3));
        }
    }

    strcpy(tParamNames[0].str, "TIME");
    strcpy(tParamNames[1].str, "COMBOS");
    strcpy(tParamNames[2].str, "COINS");
    strcpy(tParamNames[3].str, "MAX CHAIN");
    strcpy(tParamNames[4].str, "MAX COMBO");

    for (int i = 0; i < 5; i++)
        DialogueBox_AddText(db, &tParamNames[i]);

    // Params
    DialogueBoxText tParam[5];
    for (int i = 0; i < 5; i++) {
        DialogueBoxText_Init(&tParam[i]);
        tParam[i].FontID = FONT_CANCUN_12;
        tParam[i].color.g = 152;
        tParam[i].pos.x = tParamNames[i].pos.x + 185;
        tParam[i].pos.y = tParamNames[i].pos.y;
        strcpy(tParam[i].str, "NULL");

        DialogueBox_AddText(db, &tParam[i]);
    }

    // Button
    Button bOk;
    Button_Init(&bOk);
    Button_SetText(&bOk, "OK");
    bOk.xscale = 8;
    bOk.pos.x = -297;
    bOk.pos.y = 120;
    DialogueBox_AddButton(db, &bOk);

    DialogueBox_Animate(db);

    return db;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

Menu* Menu_Create() {
    Menu* menu = malloc(sizeof(Menu));

    menu->haveSky = false;

    menu->buttons = NULL;
    menu->buttonsLen = 0;

    menu->pressedBtn = 0;

    return menu;
}

void Menu_AddButton(Menu* menu, Button* btn) {
    Button* ptr = realloc(
        menu->buttons, 
        sizeof(Button) * (menu->buttonsLen+1));

    if (!ptr)
        return;

    menu->buttons = ptr;
    menu->buttons[menu->buttonsLen] = *btn;
    menu->buttonsLen++;
}

void Menu_SetBgRect(Menu* menu, SDL_Rect rect) {
    menu->trBg = rect;
}

void Menu_SetSkyRect(Menu* menu, SDL_Rect rect) {
    menu->haveSky = true;

    menu->trBgSky = rect;
}

int Menu_GetButton(Menu* menu) {
    int btn = menu->pressedBtn;
    menu->pressedBtn = 0;
    return btn;
}

void Menu_Update(Menu* menu) {
    for (int i = 0; i < menu->buttonsLen; i++) {
        Button_Update(&menu->buttons[i]);

        if (Button_IsClicked(&menu->buttons[i]))
            menu->pressedBtn = i+1;
    }
}

void Menu_Draw(Menu* menu) {
    SDL_Texture* tex = Engine_GetTextureSDL(TEX_MENU);

    if (menu->haveSky) {
        static int skyPos = 0;
        skyPos += SKY_SPD;

        skyPos %= 1280;

        SDL_Rect dtrSky = {
            skyPos, 
            0, 
            menu->trBgSky.w,
            menu->trBgSky.h
        };

        SDL_RenderCopy(engine.render, tex, &menu->trBgSky, &dtrSky);

        dtrSky.x = skyPos-1279;
        SDL_RenderCopy(engine.render, tex, &menu->trBgSky, &dtrSky);
    }

    SDL_Rect dtrBg = {0, 0, 1280, 720};
    SDL_RenderCopy(
        engine.render, 
        tex,
        &menu->trBg,
        &dtrBg
    );

    for (int i = 0; i < menu->buttonsLen; i++) {
        Button_Draw(&menu->buttons[i]);
    }

}

void Menu_Destroy(Menu* menu) {
    menu->buttonsLen = 0;
    free(menu->buttons);
    free(menu);
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void MenuMgr_Init(MenuMgr* m, int* curLvl, int* curDiff) {
    m->roomID = MR_MAIN;
    m->room = NULL;
    m->dbOptions = NULL;
    m->inDialog = false;
    m->curLvl = curLvl;
    m->curDiff = curDiff;
}

void MenuMgr_Set(MenuMgr* m, int room) {
    if (m->room != NULL) {
        Menu_Destroy(m->room);
    }

    m->roomID = room;
    if (m->roomID == MR_GAME) {
        m->room = NULL;
        return;
    }

    m->room = Menu_Create();
    if (m->roomID == MR_MAIN) {
        Engine_PlayMusic(MUS_MAIN_MENU);

        SDL_Rect mBgRect = {1282, 720, 1280, 720};
        Menu_SetBgRect(m->room, mBgRect);
        SDL_Rect mSkyRect = {0, 720, 1280, 250};
        Menu_SetSkyRect(m->room, mSkyRect);

        Button btnAdv, btnGaunt, btnOptions, btnMore, btnQuit;
        Button_Init(&btnAdv);
        Button_Init(&btnGaunt);
        Button_Init(&btnOptions);
        Button_Init(&btnMore);
        Button_Init(&btnQuit);

        SDL_Rect rBtnAdv = {2724, 901, 245, 138};
        SDL_Rect rBtnGaunt = {2648, 776, 270, 125};
        SDL_Rect rBtnOptions = {2563, 647, 299, 129};
        SDL_Rect rBtnMore = {2924, 1038, 177, 190};
        SDL_Rect rBtnQuit = {2917, 1226, 180, 211};

        btnAdv.tRect = rBtnAdv;
        btnAdv.soundHoverSfxID = SND_FX_CHORAL1;
        btnAdv.pos.x = 840;
        btnAdv.pos.y = 95;
        btnAdv.sndSfxPitch = 12;
        btnAdv.deadBorder = 16;

        btnGaunt.tRect = rBtnGaunt;
        btnGaunt.soundHoverSfxID = SND_FX_CHORAL1;
        btnGaunt.pos.x = 816;
        btnGaunt.pos.y = 230;
        btnGaunt.sndSfxPitch = 10;
        btnGaunt.deadBorder = 16;

        btnOptions.tRect = rBtnOptions;
        btnOptions.soundHoverSfxID = SND_FX_CHORAL1;
        btnOptions.pos.x = 790;
        btnOptions.pos.y = 354;
        btnOptions.sndSfxPitch = 8;
        btnOptions.deadBorder = 16;

        btnMore.tRect = rBtnMore;
        btnMore.soundHoverSfxID = SND_FX_CHORAL1;
        btnMore.pos.x = 750;
        btnMore.pos.y = 452;
        btnMore.sndSfxPitch = 4;
        btnMore.deadBorder = 16;

        btnQuit.tRect = rBtnQuit;
        btnQuit.soundHoverSfxID = SND_FX_CHORAL1;
        btnQuit.pos.x = 902;
        btnQuit.pos.y = 470;
        btnQuit.sndSfxPitch = -2;
        btnQuit.deadBorder = 16;

        Menu_AddButton(m->room, &btnAdv);
        Menu_AddButton(m->room, &btnGaunt);
        Menu_AddButton(m->room, &btnOptions);
        Menu_AddButton(m->room, &btnMore);
        Menu_AddButton(m->room, &btnQuit);

    } else if (m->roomID == MR_GAUNTLET) {
        Engine_PlayMusic(MUS_GAUNTLET);

        SDL_Rect mBgRect = {1282, 1440, 1280, 720};
        Menu_SetBgRect(m->room, mBgRect);
        SDL_Rect mSkyRect = {0, 1442, 1280, 250};
        Menu_SetSkyRect(m->room, mSkyRect);

        Button btnMainMenu, btnBack, btnPlay, btnNext;

        Button_Init(&btnMainMenu);
        SDL_Rect rBtnMainMenu = {1280, 2160, 211, 84};
        btnMainMenu.tRect = rBtnMainMenu;
        btnMainMenu.pos.x = 172;
        btnMainMenu.pos.y = 632;
        Menu_AddButton(m->room, &btnMainMenu);

        Button_Init(&btnBack);
        SDL_Rect rBtnBack = {1924, 2260, 139, 48};
        btnBack.tRect = rBtnBack;
        btnBack.pos.x = 426;
        btnBack.pos.y = 667;
        Menu_AddButton(m->room, &btnBack);

        Button_Init(&btnPlay);
        SDL_Rect rBtnPlay = {1922, 2210, 157, 49};
        btnPlay.tRect = rBtnPlay;
        btnPlay.pos.x = 576;
        btnPlay.pos.y = 667;
        Menu_AddButton(m->room, &btnPlay);

        Button_Init(&btnNext);
        SDL_Rect rBtnNext = {1922, 2161, 141, 48};
        btnNext.tRect = rBtnNext;
        btnNext.pos.x = 745;
        btnNext.pos.y = 667;
        Menu_AddButton(m->room, &btnNext);

        // Difficulty
        Button btnRabbit, btnEagle, btnJaguar, btnSunGod;
        Button_Init(&btnRabbit);
        SDL_Rect rBtnRabbit = {1280+328, 2336, 328, 63};
        btnRabbit.tRect = rBtnRabbit;
        btnRabbit.pos.x = 470;
        btnRabbit.pos.y = 270;
        Menu_AddButton(m->room, &btnRabbit);

        Button_Init(&btnEagle);
        SDL_Rect rBtnEagle = {1280+258, 2399, 258, 49};
        btnEagle.tRect = rBtnEagle;
        btnEagle.pos.x = 504;
        btnEagle.pos.y = 196;
        Menu_AddButton(m->room, &btnEagle);

        Button_Init(&btnJaguar);
        SDL_Rect rBtnJaguar = {1280+205, 2448, 205, 43};
        btnJaguar.tRect = rBtnJaguar;
        btnJaguar.pos.x = 530;
        btnJaguar.pos.y = 124;
        Menu_AddButton(m->room, &btnJaguar);

        Button_Init(&btnSunGod);
        SDL_Rect rBtnSunGod = {1280+159, 2491, 159, 54};
        btnSunGod.tRect = rBtnSunGod;
        btnSunGod.pos.x = 554;
        btnSunGod.pos.y = 45;
        Menu_AddButton(m->room, &btnSunGod);
    }

}

void MenuMgr_Update(MenuMgr* m) {
    if (!m)
        return;
    
    if (!m->inDialog)
        Menu_Update(m->room);

    if (m->roomID == MR_MAIN) {
        switch(Menu_GetButton(m->room)) {
            case 1: MenuMgr_Set(m, MR_GAUNTLET); break;
            case 2: break;
            case 3: 
                m->dbOptions = DialogueBox_CreateOptions();
                m->inDialog = true;
                break;
            case 4: break;          /* TODO NEVER GONNA GIVE YOU UP */
            case 5: exit(0); break; 
        }
    } else if (m->roomID == MR_GAUNTLET) {
        switch(Menu_GetButton(m->room)) {
            case 1: MenuMgr_Set(m, MR_MAIN); return; break;
            case 2: (*m->curLvl)--; break;
            case 3: MenuMgr_Set(m, MR_GAME); Engine_StopMusic(); return; break;
            case 4: (*m->curLvl)++; break;

            case 5: (*m->curDiff) = 0; break;
            case 6: (*m->curDiff) = 1; break;
            case 7: (*m->curDiff) = 2; break;
            case 8: (*m->curDiff) = 3; break;
        }

        if (*m->curLvl >= LEVELS_COUNT)
            *m->curLvl = 0;
        else if (*m->curLvl < 0)
            *m->curLvl = LEVELS_COUNT-1;

        for (int i = 4; i < 8; i++) {
            if (i == 4+(*m->curDiff))
                m->room->buttons[i].pressed = true;
            else
                m->room->buttons[i].pressed = false;
        }
    }

    if (m->dbOptions) {
        DialogueBox_UpdateOptions(m->dbOptions);

        if (DialogueBox_GetBtn(m->dbOptions) == 1) {
            Engine_SaveSettings();
            DialogueBox_Destroy(m->dbOptions);
            m->dbOptions = NULL;
            m->inDialog = false;
        }
    }
}

void MenuMgr_Draw(MenuMgr* m) {
    if (m->roomID == MR_GAME) 
        return;

    if (m->roomID == MR_GAUNTLET)
        Engine_DrawTexture(4+(*m->curLvl), WINDOW_WIDTH / 2+16, 500);
    
    Menu_Draw(m->room);
    if (m->dbOptions)
        DialogueBox_Draw(m->dbOptions);

    if (m->roomID == MR_MAIN) {
        SDL_Texture* tempTex = Engine_GetTextureSDL(TEX_MENU);

        SDL_Rect srcSunRect = {2563, 315, 130, 138};
        SDL_Rect dstSunRect = {158, 16, 130, 138};

        SDL_RenderCopy(engine.render, 
            tempTex, &srcSunRect, &dstSunRect);

        SDL_Rect srcGlowRect = {2563, 0, 315, 315};
        SDL_Rect dstGlowRect = {68, -68, 315, 315};

        SDL_SetTextureAlphaMod(tempTex, 152);
        SDL_SetTextureColorMod(tempTex, 255, 192, 0);

        SDL_RenderCopy(engine.render, 
            tempTex, &srcGlowRect, &dstGlowRect);

        SDL_SetTextureAlphaMod(tempTex, 255);
        SDL_SetTextureColorMod(tempTex, 255, 255, 255);

        Engine_DrawText("WELCOME TO ZUMA HD!", FONT_NATIVE_ALIEN_EXT_16, 426, 0);

        Engine_DrawText("REMASTER BY\nGALAXYSHAD AND S4LAT. 2021", FONT_CANCUN_10, 16, 660);
        Engine_DrawText("ORIGINAL GAME BY POPCAP. 2003", FONT_CANCUN_10, 16, 694);

    } else if (m->roomID == MR_GAUNTLET) {
        char buff[32];
        SDL_Color color = {255, 255, 255, 255};

        sprintf(buff, "BEST SCORE:");
        Engine_DrawTextExt(buff, FONT_CANCUN_10, color, true,
            false, 924, 632);

        sprintf(buff, "%d", levelMgr.bestScore[*m->curLvl][*m->curDiff]);
        color.b = 0;
        Engine_DrawTextExt(buff, FONT_CANCUN_10, color, true,
            false, 1140, 632);

        sprintf(buff, "BEST TIME:");
        color.b = 255;
        Engine_DrawTextExt(buff, FONT_CANCUN_10, color, true,
            false, 924, 686);

        int min, sec;
        min = levelMgr.bestTime[*m->curLvl][*m->curDiff] / 60;
        sec = levelMgr.bestTime[*m->curLvl][*m->curDiff] % 60;
        sprintf(buff, "%d:%02d", min, sec);
        color.b = 0;
        Engine_DrawTextExt(buff, FONT_CANCUN_10, color, true,
            false, 1140, 686);
    }
}

