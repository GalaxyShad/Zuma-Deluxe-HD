#include "BallChain.h"

#include "../global/HQC.h"
#include "ResourceStore.h"

#define BALL_RADIUS         32
#define BALLS_TO_EXPLODE    3

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

typedef enum BallBonus {
    BALL_BONUS_NONE,
    BALL_BONUS_ACCURACY,
    BALL_BONUS_EXPLOSION,
    BALL_BONUS_ROLL_BACK,
    BALL_BONUS_PAUSE
} BallBonus;


typedef struct Ball {
    struct BallChain*  chain;

    float       pos;

    BallColor   color;
    BallBonus   bonus;

    struct Ball* prev;
    struct Ball* next;
} Ball;


typedef struct BallChain {
    HLevel level;

    Ball* start;
    Ball* end;

    float spd;
} BallChain;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

static BallChain* _Cast(HBallChain hchain) {
    return (BallChain*)hchain;
}


static void _Ball_Destroy(Ball* ball) {
    if (!ball) return;

    if (ball->prev != NULL)
        ball->prev->next = ball->next;
    else 
        ball->chain->start = ball->next;

    if (ball->next != NULL)
        ball->next->prev = ball->prev;
    else 
        ball->chain->end = ball->prev;

    HQC_Memory_Free(ball);
}


static void _BallChain_ExplodeBalls(Ball* startBall) {
    if (!startBall) return;
    
    int count = -1;

    Ball* right;
    for (right = startBall; right != NULL; right = right->next) {
        if (right->color != startBall->color) break;
        count++;
    }

    Ball* left;
    for (left = startBall; left->prev != NULL; left = left->prev) {
        if (left->color != startBall->color) break;
        count++;
    }

    if (left->color != startBall->color){
        left = left->next;
        count--;
    }

    for (Ball* ball = left; (ball != right) && (ball != NULL); ) { 
        Ball* next = ball->next;
        _Ball_Destroy(ball);
        ball = next;
    }

    HQC_DJ_PlaySound(Store_GetSoundByID(SND_CHIME1));
}


static Ball* _Ball_Create(BallChain* chain, BallColor color) {
    Ball* ball = HQC_Memory_Allocate(sizeof(*ball));

    ball->chain = chain;
    ball->pos   = 0;
    ball->prev  = NULL;
    ball->next  = NULL;
    ball->bonus = BALL_BONUS_NONE;
    ball->color = color;

    return ball;
}

static bool prs = false;

static void _Ball_Update(Ball* ball) {
    if (!ball) return;

    v2f_t pos = Level_GetCurveCoords(ball->chain->level, ball->pos);
    v2i_t mpos = HQC_Input_MouseGetPosition();

    if (HQC_Input_MouseLeftPressed() && !prs && 
        (mpos.x > pos.x - BALL_RADIUS) && (mpos.x < pos.x + BALL_RADIUS) &&
        (mpos.y > pos.y - BALL_RADIUS) && (mpos.y < pos.y + BALL_RADIUS)
    ) {
        _BallChain_ExplodeBalls(ball);
        // _Ball_Destroy(ball);
        prs = true;
        return;
    }

    if (!HQC_Input_MouseLeftPressed()) prs = false;
    
    if (ball->prev == NULL) {
        ball->pos += ball->chain->spd;
        return; 
    }

    ball->pos = ball->prev->pos + BALL_RADIUS;
}


static void _Ball_Draw(Ball* ball) {
    if (!ball) return;

    v2f_t pos = Level_GetCurveCoords(ball->chain->level, ball->pos);

    HQC_Artist_DrawAnimation(
        Store_GetAnimationByID(ANIM_BALL_BLUE + ball->color),
        pos.x, pos.y
    );
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

HBallChain BallChain_Create(HLevel level) {
    BallChain* ballChain = HQC_Memory_Allocate(sizeof(*ballChain));

    ballChain->level    = level;
    ballChain->start    = NULL;
    ballChain->end      = NULL;
    ballChain->spd      = 1.0f;

    return ballChain;
}





void BallChain_AddToStart(HBallChain hchain, BallColor color) {
    BallChain* chain = _Cast(hchain);

    Ball* ball = _Ball_Create(chain, color);

    // Привязка нового узла к началу списка
    if (chain->start == NULL) {
        // Список пустой - создаем единственный элемент
        ball->prev = NULL;
        ball->next = NULL;
        chain->start = ball;
    } else {
        ball->prev = NULL;
        ball->next = chain->start;
        chain->start->prev = ball; 
        chain->start = ball;
    }
}


void BallChain_Update(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        _Ball_Update(ball);
    }
}


void BallChain_Draw(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        _Ball_Draw(ball);
    }


}   

 




