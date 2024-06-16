#include <math.h>
#include "BallChain.h"

#include "../global/HQC.h"
#include "ResourceStore.h"

#define BALL_RADIUS         24
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

    bool        isGoingBack;
    bool        isBulletInserting;

    BallColor   color;
    BallBonus   bonus;

    HQC_Animation animation;

    struct Ball* prev;
    struct Ball* next;
} Ball;


typedef struct BallChain {
    HLevel level;

    Ball* start;
    Ball* end;

    float spd;

    HBulletList bulletList;
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
HBall BallChain_ExplodeBalls(HBall hstartBall) {
    Ball* startBall = (Ball*)hstartBall;
    
    if (!startBall) return NULL;
    
    int count = 0;

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

    if (count < 3)
        return startBall;

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
    ball->isGoingBack = false;
    ball->isBulletInserting = false;
    ball->prev  = NULL;
    ball->next  = NULL;
    ball->bonus = BALL_BONUS_NONE;
    ball->color = color;

    ball->animation = HQC_Animation_Clone(Store_GetAnimationByID(ANIM_BALL_BLUE + ball->color));

    return ball;
}

static bool prs = false;

static void _print_pointer_list(BallChain* chain, Ball* dball) {
    
    int i = 0;
    HQC_Log("chain %p\n", chain);
    HQC_Log("start-%p end-%p\n", chain->start, chain->end);
    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        HQC_Log("[%d] p-%p m-%p n-%p     color-%d %d\n", i, ball->prev, ball, ball->next, ball->color, ball == dball);
        i++;
    }
}

static bool _Ball_IsCollidingBack(Ball* ball, float expand) {
    if (ball->prev == NULL) 
        return false;

    return (ball->prev->pos + 32 + expand >= ball->pos);
}


static bool _Ball_IsCollidingFront(Ball* ball) {
    if (ball->next == NULL)
        return false;

    return (ball->next->pos - 32 <= ball->pos);
}

float Ball_GetPositionOnCurve(HBall hball) {
    Ball* b = (Ball*)hball;

    if (b == NULL)
        return 0;

    return b->pos;
}

v2f_t Ball_GetPositionCoords(HBall ball) {
    Ball* b = (Ball*)ball;

    if (b == NULL)
        return v2f_t_default;

    return  Level_GetCurveCoords(b->chain->level, (b->pos >= 0) ? b->pos : 0);
}

float Ball_GetDistanceBetween(HBall hballLeft, HBall hballRight) {
    Ball* left = (Ball*)hballLeft;
    Ball* right = (Ball*)hballRight;

    return right->pos - left->pos;
}

HBallChain Ball_GetChain(HBall ball) {
    Ball* b = ((Ball*)ball);

    if (b == NULL)
        return NULL;

    return b->chain;
}

HBall BallChain_InsertBeforeBall(HBallChain hchain, BallColor color, HBall nextBall, float pos) {
    Ball* ball = (Ball*)nextBall;

    if (ball == NULL) return NULL;
    
    Ball* newBall = _Ball_Create(ball->chain, color);

    newBall->pos = pos;//ball->pos - 32;

    newBall->prev = ball->prev;
    newBall->next = ball;

    if (ball->prev != NULL) {
        ball->prev->next = newBall;
    } else {
        ball->chain->start = newBall;
    }

    ball->prev = newBall;

    return newBall;
}

HBall BallChain_InsertAfterBall(HBallChain hchain, BallColor color, HBall prevBall, float pos) {
    Ball* ball = (Ball*)prevBall;

    if (ball == NULL) return NULL;

    Ball* newBall = _Ball_Create(ball->chain, color);

    newBall->pos = pos;//ball->pos + 32;

    newBall->prev = ball;
    newBall->next = ball->next;

    if (ball->next != NULL) {
        ball->next->prev = newBall;
    } else {
        ball->chain->end = newBall;
    }

    ball->next = newBall;

    return newBall;
}


void  Ball_MoveSubChainFrom(HBall hball, float value) {
    Ball* ball = ((Ball*)hball);

    if (ball == NULL) return;

    ball->pos += value;

    for (Ball* b = ball->next; b != NULL; b = b->next) {
        if (!_Ball_IsCollidingBack(b, 8))
            return;

        b->pos += value;
    }
}

HBall Ball_Next(HBall hball) {
    Ball* ball = ((Ball*)hball)->next;

    return ball;
}

HBall Ball_Previous(HBall hball) {
    Ball* ball = ((Ball*)hball)->prev;

    return ball;
}


static Ball* _Ball_HandleBulletCollision(Ball* ball, HBullet bullet) {
    if (bullet == NULL) 
        return NULL;

    v2f_t ballPos   = Level_GetCurveCoords(ball->chain->level, (ball->pos >= 0) ? ball->pos : 0);
    v2f_t bulletPos = Bullet_GetPosition(bullet);

    if (HQC_PointDistance(ballPos.x, ballPos.y, bulletPos.x, bulletPos.y) > 24 * 2)
        return NULL;

    if (Bullet_GetInsertionBall(bullet) == NULL) {
        v2f_t pointLeft  = Level_GetCurveCoords(ball->chain->level, ball->pos - BALL_RADIUS);
        v2f_t pointRight = Level_GetCurveCoords(ball->chain->level, ball->pos + BALL_RADIUS);

        float distanceLeft  = HQC_PointDistance(bulletPos.x, bulletPos.y, pointLeft.x, pointLeft.y);
        float distanceRight = HQC_PointDistance(bulletPos.x, bulletPos.y, pointRight.x, pointRight.y);

        ball->isBulletInserting = true;

        Bullet_SetInsertion(bullet, ball, (distanceRight < distanceLeft));
        //Bullet_SetInsertion(bullet, ball, false);
        //Bullet_SetInsertion(bullet, ball, true);
    }
}


static Ball* _Ball_HandleBulletListCollision(Ball* ball) {
    for (int i = 0; i < 8; i++) {
        _Ball_HandleBulletCollision(ball, BulletList_GetBullet(ball->chain->bulletList, i));
    }
}

void  Ball_BulletInsertDone(HBall hball) {
    Ball* ball = (Ball*)hball;

    if (ball == NULL) return;

    ball->isBulletInserting = false;
}

static Ball* _Ball_Update(Ball* ball) {
    if (!ball) return NULL;

    v2f_t pos = Level_GetCurveCoords(ball->chain->level, (ball->pos >= 0) ? ball->pos : 0);

    ball->pos += ball->spd;
    if (ball->pos >= (float)Level_GetCurveLength(ball->chain->level)) {
        ball->pos -= (float)Level_GetCurveLength(ball->chain->level);
        
        return ball->next;
    }
    

    _Ball_HandleBulletListCollision(ball);
 
    if (ball->prev == NULL) {
        ball->spd += 0.025; 

        if (ball->spd > ball->chain->spd)
            ball->spd = ball->chain->spd;

        return ball->next;
    }


    if (_Ball_IsCollidingBack(ball, 8)) {
        if (ball->isGoingBack && _Ball_IsCollidingBack(ball, 0)) {
            for (Ball* b = ball->prev; b != NULL; b = b->prev)
                b->spd = ball->spd / 2;

            ball->isGoingBack = false;

            return BallChain_ExplodeBalls(ball);
        }

        ball->pos = ball->prev->pos + 32;
        ball->spd = ball->prev->spd;
    }

    
    if (!_Ball_IsCollidingBack(ball, 8)) {
        if (ball->prev->color == ball->color && !ball->isBulletInserting) {
            ball->spd -= 0.05;
            ball->isGoingBack = true;
        } else {
            ball->spd = 0;
        }
    }

    return ball->next;
}

static void _Ball_Draw(Ball* ball) {
    if (!ball) return;
    if (ball->pos < 0.0) return;

    v2f_t pos       = Level_GetCurveCoords(ball->chain->level, ball->pos);
    v2f_t posNext   = Level_GetCurveCoords(ball->chain->level, ball->pos+1);

    HQC_Animation_SetFrame(ball->animation, ((int)ball->pos) % HQC_Animation_FramesCount(ball->animation));

    float angle = HQC_FAtan2(pos.y - posNext.y, pos.x - posNext.x) + M_PI_2;
    float alpha = HQC_FMin(ball->pos / 32.f, 1.f);

    HQC_Artist_DrawSetAngle(angle);
    HQC_Artist_DrawSetAlpha(alpha);
    HQC_Artist_DrawAnimation(ball->animation, pos.x, pos.y);
    HQC_Artist_DrawSetAlpha(1);
    HQC_Artist_DrawSetAngle(0);
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

HBallChain BallChain_Create(HLevel level, HBulletList bulletList) {
    BallChain* ballChain = HQC_Memory_Allocate(sizeof(*ballChain));

    ballChain->level    = level;
    ballChain->start    = NULL;
    ballChain->end      = NULL;
    ballChain->spd      = 1.0f;
    ballChain->bulletList = bulletList;

    return ballChain;
}


HBall BallChain_AddToStart(HBallChain hchain, BallColor color) {
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

    return ball;
}

HLevel BallChain_GetLevel(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    return chain->level;
}


void BallChain_Update(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = _Ball_Update(ball));
}


HBall BallChain_HasBall(HBallChain hchain, HBall hball) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        if (ball == hball) return ball;
    }

    return NULL;
}


void BallChain_Draw(HBallChain hchain) {
    BallChain* chain = _Cast(hchain);

    for (Ball* ball = chain->start; ball != NULL; ball = ball->next) {
        _Ball_Draw(ball);
    }
}   

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

typedef struct _Generator {
    BallChain* ballChain;

    bool fastMode;
    int fastModeBallsCountdown;
} Generator;


HBallChainGenerator BallChainGenerator_Create(HBallChain hballChain) {
    Generator* gen = HQC_Memory_Allocate(sizeof(*gen));

    gen->ballChain = hballChain;
    gen->fastMode = true;
    gen->fastModeBallsCountdown = 32;

    HQC_DJ_PlaySound(Store_GetSoundByID(SND_ROLLING));

    return gen;
}


void BallChainGenerator_GenerateSequence(HBallChainGenerator hballChainGenerator, size_t count) {

}


void BallChainGenerator_Stop(HBallChainGenerator hballChainGenerator) {

}


static void _Generator_UpdateFastMode(Generator* gen) {
    Ball* headBall = gen->ballChain->start;

    if (!headBall) {
        BallChain_AddToStart(gen->ballChain, 1);
        return;
    }

    headBall->pos += 8;

    if (headBall->pos > 32) {
        Ball* ball = BallChain_InsertBeforeBall(gen->ballChain, HQC_RandomRange(0, 3), headBall, headBall->pos - 32);
        ball->spd = headBall->spd;
        gen->fastModeBallsCountdown--;
    }

    if (gen->fastModeBallsCountdown <= 0)
        gen->fastMode = false;
}

void BallChainGenerator_Update(HBallChainGenerator hballChainGenerator) {
    Generator* gen = (Generator*)hballChainGenerator;

    if (gen->fastMode) {
        _Generator_UpdateFastMode(gen);
        return;
    }

    Ball* headBall = gen->ballChain->start;

    if (!headBall) {
        BallChain_AddToStart(gen->ballChain, 1);
        return;
    }

    if (headBall->pos > 32) {
        Ball* ball = BallChain_InsertBeforeBall(gen->ballChain, HQC_RandomRange(0, 3), headBall, 0);
        ball->spd = headBall->spd;
    }
}

 




