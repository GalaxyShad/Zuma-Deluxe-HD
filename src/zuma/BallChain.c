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
    float       spd;

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


// returns pointer to next ball after removing
static Ball* _BallChain_ExplodeBalls(Ball* startBall) {
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

    Ball* ball;
    for (ball = left; (ball != right) && (ball != NULL); ) { 
        Ball* next = ball->next;
        _Ball_Destroy(ball);
        ball = next;
    }
    
    HQC_DJ_PlaySound(Store_GetSoundByID(SND_CHIME1));

    return ball;
}


static Ball* _Ball_Create(BallChain* chain, BallColor color) {
    Ball* ball = HQC_Memory_Allocate(sizeof(*ball));

    ball->chain = chain;
    ball->pos   = 0.0;
    ball->spd   = 0.0;
    ball->prev  = NULL;
    ball->next  = NULL;
    ball->bonus = BALL_BONUS_NONE;
    ball->color = color;

    return ball;
}

static bool prs = false;

static void _print_pointer_list(BallChain* chain, Ball* dball) {
    
    int i = 0;
    printf("chain %p\n", chain);
    printf("start-%p end-%p\n", chain->start, chain->end);
    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        printf("[%d] p-%p m-%p n-%p     color-%d %d\n", i, ball->prev, ball, ball->next, ball->color, ball == dball);
        i++;
    }
}

static bool _Ball_IsCollidingBack(Ball* ball) {
    if (ball->prev == NULL) 
        return false;

    return (ball->prev->pos + BALL_RADIUS * 1.5 >= ball->pos);
}


static bool _Ball_IsCollidingFront(Ball* ball) {
    if (ball->next == NULL)
        return false;

    return (ball->next->pos - BALL_RADIUS <= ball->pos);
}


static Ball* _Ball_Update(Ball* ball) {
    if (!ball) return;

    v2f_t pos = Level_GetCurveCoords(ball->chain->level, ball->pos);
    v2i_t mpos = HQC_Input_MouseGetPosition();

    if (HQC_Input_MouseLeftPressed() && !prs && 
        (mpos.x > pos.x - BALL_RADIUS) && (mpos.x < pos.x + BALL_RADIUS) &&
        (mpos.y > pos.y - BALL_RADIUS) && (mpos.y < pos.y + BALL_RADIUS)
    ) {
        prs = true;
        return _BallChain_ExplodeBalls(ball);
    }

    if (!HQC_Input_MouseLeftPressed()) 
        prs = false;

    ball->pos += ball->spd;
    
    

    if (ball->prev == NULL) {
        ball->spd += 0.25; 

        if (ball->spd > ball->chain->spd)
            ball->spd = ball->chain->spd;

        return ball->next;
    }

    if (_Ball_IsCollidingBack(ball)) {
        if (ball->spd < 0.0) {
            printf("Collide");

            for (Ball* b = ball->prev; b != NULL; b = b->prev)
                b->spd = ball->spd;

            return ball->next;
        }

        ball->pos = ball->prev->pos + BALL_RADIUS;
        ball->spd = ball->prev->spd;
    }

    if (!_Ball_IsCollidingBack(ball)) {
        if (ball->prev->color == ball->color) {
            ball->spd -= 0.125;
        } else {
            ball->spd = 0;
        }
    }

    return ball->next;
}

#include <stdio.h>
static void _Ball_Draw(Ball* ball) {
    if (!ball) return;
    if (ball->pos < 0.0) return;

    v2f_t pos = Level_GetCurveCoords(ball->chain->level, ball->pos);

    HQC_Artist_DrawAnimation(
        Store_GetAnimationByID(ANIM_BALL_BLUE + ball->color),
        pos.x, pos.y
    );

    char text[25];

    
    sprintf(text, "s: %0.2lf", ball->spd);
    HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_8), text, pos.x, pos.y);
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

    if (chain->start == NULL) {
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

    for (Ball* ball = chain->start; ball != NULL; ball = _Ball_Update(ball));
}


void BallChain_Draw(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        _Ball_Draw(ball);
    }


}   

 




