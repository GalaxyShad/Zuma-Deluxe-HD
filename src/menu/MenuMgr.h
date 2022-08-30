#ifndef MENU_MGR_H
#define MENU_MGR_H

#include "../global/Engine.h"
#include "../global/Enums.h"

#include "../gameplay/Level.h"

#define SKY_SPD 1

enum _BtnState {
    BTN_IDLE,
    BTN_HOVER,
    BTN_CLICKED
};

typedef struct _Button {
    bool withText;
    bool clicked;
    bool pressed;

    int xscale;
    int state;

    int deadBorder;

    int soundHoverSfxID;
    float sndSfxPitch;

    int soundHoverID;
    int soundClickID;

    SDL_Rect tRect;
    SDL_FPoint pos;

    int textureID;

    char text[32];
} Button;

void Button_Init(Button* btn);
void Button_SetText(Button* btn, const char* str);
void Button_Update(Button* btn);
int Button_IsClicked(Button* btn);
void Button_Draw(Button* btn);

////////////////////////////////////////////////////
////////////////////////////////////////////////////

#define CHECK_BOX_WIDTH     75
#define CHECK_BOX_HEIGHT    69

typedef struct _CheckBox {
    bool checked;
    bool clicked;

    SDL_Point pos;
} CheckBox;

void CheckBox_Init(CheckBox* cb);
void CheckBox_Update(CheckBox* cb);
int CheckBox_IsClicked(CheckBox* cb);
int CheckBox_IsChecked(CheckBox* cb);
void CheckBox_Draw(CheckBox* cb);

////////////////////////////////////////////////////
////////////////////////////////////////////////////

#define SLIDER_THUMB_WIDTH  61
#define SLIDER_WIDTH        307
#define SLIDER_HEIGHT       45

#define SLIDER_THUMB_MAX    (SLIDER_WIDTH-SLIDER_THUMB_WIDTH*1.125)

typedef struct _Slider {
    SDL_Point pos;
    float thumbPos;
    bool drag;
    float* value;
    float maxValue;
} Slider;

void Slider_Init(Slider* s, float* val);
void Slider_Update(Slider* s);
int Slider_IsDragging(Slider* s);
void Slider_Draw(Slider* s);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

typedef struct _DialogueBoxText {
    int FontID;
    char str[48];
    SDL_Point pos;
    SDL_Color color;
} DialogueBoxText;

void DialogueBoxText_Init(DialogueBoxText* dbt);

#define DB_ANIM_SPD 96

typedef struct _DialogueBox {
    bool drawGodHead;
    bool animation;

    int exitButton;
    int pressedBtn;
    int sizeW, sizeH;

    SDL_Point pos;
    int startY;

    char caption[16];

    int textsLen;
    DialogueBoxText* texts;
    
    int buttonsLen;
    Button* buttons;

    int chBoxesLen;
    CheckBox* chBoxes;

    int slidersLen;
    Slider* sliders;
} DialogueBox;

DialogueBox* DialogueBox_Create();

void DialogueBox_AddButton(DialogueBox* db, Button* btn);
void DialogueBox_AddCheckBox(DialogueBox* db, CheckBox* cb);
void DialogueBox_AddSlider(DialogueBox* db, Slider* s);
void DialogueBox_AddText(DialogueBox* db, DialogueBoxText* text);
void DialogueBox_SetTextStr(DialogueBox* db, int textID, const char* str);
void DialogueBox_Animate(DialogueBox* db);
int DialogueBox_GetBtn(DialogueBox* db);
void DialogueBox_Update(DialogueBox* db);
void DialogueBox_Draw(DialogueBox* db);
void DialogueBox_Destroy(DialogueBox* db);

DialogueBox* DialogueBox_CreateOptions();
void DialogueBox_UpdateOptions(DialogueBox* db);

DialogueBox* DialogueBox_CreateMenu();
void DialogueBox_UpdateMenu(DialogueBox* db);

enum _DbResultsTexts {
    DB_RES_TEXT_POINTS = 7,
    DB_RES_TEXT_COMBOS,
    DB_RES_TEXT_COINS,
    DB_RES_TEXT_MAX_CHAIN,
    DB_RES_TEXT_MAX_COMBO,
    DB_RES_TEXT_YOUR_TIME,
    DB_RES_TEXT_ACE_TIME
};

DialogueBox* DialogueBox_CreateResults();

enum _DbGameOverTexts {
    DB_GO_TEXT_TOTAL_TIME = 5,
    DB_GO_TEXT_COMBOS,
    DB_GO_TEXT_COINS,
    DB_GO_TEXT_MAX_CHAIN,
    DB_GO_TEXT_MAX_COMBO,
};

DialogueBox* DialogueBox_CreateGameOver();

////////////////////////////////////////////////////
////////////////////////////////////////////////////
typedef struct _Menu {
    bool haveSky;
    SDL_Rect trBg;
    SDL_Rect trBgSky;

    Button* buttons;
    int buttonsLen;

    int pressedBtn;
} Menu;

Menu* Menu_Create();
void Menu_AddButton(Menu* menu, Button* btn);
void Menu_SetBgRect(Menu* menu, SDL_Rect rect);
void Menu_SetSkyRect(Menu* menu, SDL_Rect rect);
int  Menu_GetButton(Menu* menu);
void Menu_Update(Menu* menu);
void Menu_Draw(Menu* menu);
void Menu_Destroy(Menu* menu);

////////////////////////////////////////////////////
////////////////////////////////////////////////////

enum _MenuMgrRooms {
    MR_GAME,
    MR_MAIN,
    MR_GAUNTLET
};

typedef struct _MenuMgr {
    bool inDialog;
    int roomID;
    int *curLvl, *curDiff;
    Menu* room;
    DialogueBox* dbOptions;
} MenuMgr;

void MenuMgr_Init(MenuMgr* m, int* curLvl, int* curDiff);
void MenuMgr_Set(MenuMgr* m, int room);
void MenuMgr_Update(MenuMgr* m);
void MenuMgr_Draw(MenuMgr* m);


#endif