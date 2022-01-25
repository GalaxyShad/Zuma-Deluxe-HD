#include <Game.h>

void constrain(int* a, int min, int max) {
    if (*a < min)
        *a = max;
    else if (*a > max)
        *a = min;
}

void Game_Init(Game* game, int lvlID, int difficulty) {
    game->stageID = 0;
    game->score = 0;
    game->lives = 0;
    game->isLosed = false;
    game->isWon = false;
    game->isOutroEnded = false;
    game->isIntroEnded = false;
    game->totalCoins = 0;
    game->headerFontScale = 0;
    game->subHeaderFontScale = 0;
    game->difficulty = difficulty;

    Stage* stg = LevelMgr_GetStage(game->stageID);

    game->lvlID = lvlID; 
    constrain(&game->lvlID, 0, stg->levelsLen-1);

    game->lvl = LevelMgr_GetLevelFromStage(game->stageID, game->lvlID);
    Level_Load(game->lvl);

    game->graphics = LevelMgr_GetLevelGraphics(game->lvl);
    switch(difficulty) {
        case 0:
            game->settings = &levelMgr.settings[4];
            game->settings->ballColors = 4;
            game->settings->partTime = 70;
            game->settings->ballStartCount = 40;
            break;
        case 1:
            game->settings = &levelMgr.settings[8];
            game->settings->ballColors = 5;
            game->settings->partTime = 100;
            game->settings->ballStartCount = 50;
            break;
        case 2:
            game->settings = &levelMgr.settings[13];
            game->settings->ballColors = 6;
            game->settings->repeatChance = 25;
            game->settings->partTime = 120;
            game->settings->ballStartCount = 60;
            break;
        case 3:
            game->settings = &levelMgr.settings[24];
            game->settings->ballColors = 6;
            game->settings->partTime = 150;
            game->settings->ballStartCount = 60;
            break;
    }

    if (levelMgr.bestTime[game->lvlID][game->difficulty] != 0)
        game->settings->partTime = levelMgr.bestTime[game->lvlID][game->difficulty];
    
    Frog_Init(&game->frog, (game->graphics->frogPos.x + 106) * engine.scale_x, 
        game->graphics->frogPos.y * engine.scale_y);

    BallChain_Init(&game->chain, game->lvl, game->settings);

    for(int i = 0; i < BULLETS_ARR_MAX_LEN; i++)
        game->bullets.bullets[i].onScreen = 0;
    game->bullets.len = 0;

    for(int i = 0; i < MAX_MSGS_ARRAY_LEN; i++)
        game->msgs.msgs[i].onScreen = 0;
    game->msgs.len = 0;

    for(int i = 0; i < MAX_MSGS_ARRAY_LEN; i++)
        game->particles.prtcls[i].onScreen = 0;
    game->particles.len = 0;

    game->DEBUG = false;
    game->isRunning = true;
    game->isFirstTime = true;
    game->chain.isGenerating = true;

    game->treasureActive = false;
    game->treasureBlinking = false;
    game->treasureFading = false;
    game->treasureTime = clock();

    game->lightTrailPitch = 0;

    SDL_Rect animTreasureRect = {340, 0, 54, 54};
    Animation_Init(&game->treasureAnim, TEX_GAME_OBJECTS, animTreasureRect);
    Animation_Set(&game->treasureAnim, 0, 30, 0.25);

    SDL_Rect animFinishRect = {629, 132, 132, 132};
    Animation_Init(&game->finishAnim, TEX_GAME_OBJECTS, animFinishRect);
    Animation_Set(&game->finishAnim, 0, 12, 0);

    game->finishPos.x = (game->lvl->spiralStart.x + 104)*engine.scale_x;
    game->finishPos.y = game->lvl->spiralStart.y*engine.scale_y;
    for (int i = 0; i < game->lvl->spiralLen; i++) {
        game->finishPos.x += game->lvl->spiral[i].dx * engine.scale_x;
        game->finishPos.y += game->lvl->spiral[i].dy * engine.scale_y;
    }

    game->scoreRect.x = 773;
    game->scoreRect.y = 28;
    game->scoreRect.w = 0;
    game->scoreRect.h = 28;

    Engine_PlaySound(SND_CHANT1);
    Engine_PlayMusic(MUS_GAME);

    Button_Init(&game->btnMenu);
    game->btnMenu.textureID = TEX_GAME_HUD;
    game->btnMenu.tRect.x = 925;
    game->btnMenu.tRect.y = 15;
    game->btnMenu.tRect.w = 118;
    game->btnMenu.tRect.h = 37;

    game->btnMenu.pos.x = 1128;
    game->btnMenu.pos.y = 4;

    // Menus
    game->dbMenu = NULL;
    game->dbStats = NULL;
    game->dbGameOver = NULL;
}

void Game_Update(Game* game, int* inMenu, int mouseClicked) {
    // Get mouse pos and rotate frog;
    Engine_GetMousePos(&game->mx, &game->my);
    Frog_Rotate(&game->frog, game->mx, game->my);

    if (!game->isIntroEnded) {
        Game_UpdateIntro(game);
        return;
    }

    char isShooted = 0;
    switch (mouseClicked) {
        case 1:
            if (!isShooted && !game->chain.isEndReached && !game->isFirstTime)
                isShooted = 1;
            break;
        case 2: {
            char t = game->frog.color;
            game->frog.color = game->frog.nextColor;
            game->frog.nextColor = t;
            Engine_PlaySound(SND_BUTTON2);
            break;
        }
    }

    if (game->isLosed || game->isWon) {
        if (game->isWon && !game->isOutroEnded) {
            Game_UpdateOutro(game);
            return;
        }

        if (game->dbStats) {
            DialogueBox_Update(game->dbStats);
            if (DialogueBox_GetBtn(game->dbStats) == 1) {
                DialogueBox_Destroy(game->dbStats);
                game->dbStats = NULL;

                if (game->score > levelMgr.bestScore[game->lvlID][game->difficulty])
                    levelMgr.bestScore[game->lvlID][game->difficulty] = game->score;

                if (game->time < levelMgr.bestTime[game->lvlID][game->difficulty] ||
                    levelMgr.bestTime[game->lvlID][game->difficulty] == 0)
                        levelMgr.bestTime[game->lvlID][game->difficulty] = game->time;

                LevelMgr_SaveProgress();

                *inMenu = 1;
                Level_Free(game->lvl);
            }
        }
        if (game->dbGameOver) {
            DialogueBox_Update(game->dbGameOver);
            if (DialogueBox_GetBtn(game->dbGameOver) == 1) {
                DialogueBox_Destroy(game->dbGameOver);
                game->dbGameOver = NULL;
                *inMenu = 1;
                Level_Free(game->lvl);
            }
        }
        return;
    }

    if (game->dbMenu) {
        DialogueBox_UpdateMenu(game->dbMenu);

        switch (DialogueBox_GetBtn(game->dbMenu)) {
            case 1:
                DialogueBox_Destroy(game->dbMenu);
                game->dbMenu = NULL;
                break;
            case 2:
                DialogueBox_Destroy(game->dbMenu);
                game->dbMenu = NULL;
                *inMenu = 1;
                Level_Free(game->lvl);
                break;
        }
        return;
    }

    BallChain_Update(&game->chain, game->lvl->spiral, game->lvl->spiralLen, 
        &game->score, &game->msgs);
    if (game->chain.len > 0) {
        BulletsArr_UpdateOnScreenStatus(&game->bullets, &game->chain.chainBonus);
        BulletsArr_Update(&game->bullets);
    }

    if (game->chain.isGenerating && game->score > game->settings->gaugeScore) {
        game->chain.isGenerating = 0;
        game->chain.isGlowing = 1;
        game->chain.glowTime = clock();
        Engine_PlaySound(SND_CHANT4);
    }

    
    game->chain.speed = game->settings->ballSpd;
    if (game->chain.isGenerating) {
        if (game->isFirstTime && game->chain.len < game->settings->ballStartCount) {
            if (game->chain.balls[game->chain.len-1].pos >= 0.5) {
                BallChain_Append(&game->chain, game->lvl, game->settings);
                game->chain.balls[game->chain.len-1].spd = game->chain.balls[game->chain.len-2].spd;
            }
        } else {
            if (game->chain.balls[game->chain.len-1].pos >= BALLS_CHAIN_PAD) {
                BallChain_Append(&game->chain, game->lvl, game->settings);
                game->chain.balls[game->chain.len-1].spd = game->chain.balls[game->chain.len-2].spd;
                game->chain.balls[game->chain.len-1].startAnim = true;
	            Ball_InitAnim(&game->chain.balls[game->chain.len-1]);
            } else if (game->chain.len == 0) {
                BallChain_Append(&game->chain, game->lvl, game->settings);
            }

            if (game->isFirstTime) {
                Engine_StopSound(SND_ROLLING);
            }
            game->isFirstTime = false;
        }
    }

    if (!game->chain.isEndReached && game->chain.len == 0 
        && game->score > game->settings->gaugeScore) {
        if (!game->isWon) {
            Engine_PlayMusic(MUS_WIN);

            game->dbStats = DialogueBox_CreateResults();

            char buff[8];
            sprintf(buff, "%d", game->score);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_POINTS, buff);
            
            sprintf(buff, "%d", game->totalCoins);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_COINS, buff);

            sprintf(buff, "%d", game->chain.totalCombos);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_COMBOS, buff);

            sprintf(buff, "%d", game->chain.maxChainBonus);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_MAX_CHAIN, buff);

            sprintf(buff, "%d", game->chain.maxCombo);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_MAX_COMBO, buff);

            game->time = ((float)clock())/CLOCKS_PER_SEC - game->time;
            sprintf(buff, "%d:%02d", 
                game->time / 60, 
                game->time % 60);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_YOUR_TIME, buff);

            if (game->time <= game->settings->partTime) {
                game->dbStats->texts[DB_RES_TEXT_YOUR_TIME].color.r = 0;
                game->dbStats->texts[DB_RES_TEXT_YOUR_TIME].color.g = 255;
                game->dbStats->texts[DB_RES_TEXT_YOUR_TIME].color.b = 0;
            }


            sprintf(buff, "%d:%02d", 
                game->settings->partTime / 60, 
                game->settings->partTime % 60);
            DialogueBox_SetTextStr(game->dbStats, 
                DB_RES_TEXT_ACE_TIME, buff);

            game->isWon = true;
        }
    }

    if (game->chain.len == 0 && game->chain.isEndReached) {
        if (game->lives >= 1) {
            int lives = game->lives - 1;
            Engine_PlaySound(SND_CHANT14);
            Game_Init(game, game->stageID, game->lvlID);
            game->lives = lives;
            return;
        } else {
            if (!game->isLosed) {
                Engine_PlaySound(SND_CHANT8);
                Engine_PlayMusic(MUS_GAME_OVER);

                game->dbGameOver = DialogueBox_CreateGameOver();

                char buff[8];
                
                sprintf(buff, "%d", game->totalCoins);
                DialogueBox_SetTextStr(game->dbGameOver, 
                    DB_GO_TEXT_COINS, buff);

                sprintf(buff, "%d", game->chain.totalCombos);
                DialogueBox_SetTextStr(game->dbGameOver, 
                    DB_GO_TEXT_COMBOS, buff);

                sprintf(buff, "%d", game->chain.maxChainBonus);
                DialogueBox_SetTextStr(game->dbGameOver, 
                    DB_GO_TEXT_MAX_CHAIN, buff);

                sprintf(buff, "%d", game->chain.maxCombo);
                DialogueBox_SetTextStr(game->dbGameOver, 
                    DB_GO_TEXT_MAX_COMBO, buff);

                game->time = ((float)clock())/CLOCKS_PER_SEC - game->time;
                sprintf(buff, "%d:%02d", game->time / 60, game->time % 60);
                DialogueBox_SetTextStr(game->dbGameOver, 
                    DB_GO_TEXT_TOTAL_TIME, buff);

                game->isLosed = true;
            }
        }
    }

    Button_Update(&game->btnMenu);
    if (Button_IsClicked(&game->btnMenu)) {
        game->dbMenu = DialogueBox_CreateMenu();
    }

    if(isShooted && !(int)game->frog.shift && game->btnMenu.state != BTN_CLICKED) {
        BulletsArr_AddBullet(&game->bullets, &game->frog);
        Engine_PlaySound(SND_FIREBALL1);

        game->frog.isShooted = 1;
        game->frog.color = game->frog.nextColor;
        game->frog.nextColor = randInt(0, game->chain.ballColors-1);
        while(!game->chain.colorInChain[game->frog.nextColor])
            game->frog.nextColor = randInt(0, game->chain.ballColors-1);
    }

    // Changing frog color if color not in chain
    if (!(game->isLosed || game->isWon || game->chain.isEndReached) 
        && (!game->chain.colorInChain[game->frog.nextColor] || !game->chain.colorInChain[game->frog.color])) {
        while(!game->chain.colorInChain[game->frog.nextColor])
            game->frog.nextColor = randInt(0, game->chain.ballColors-1);

        while(!game->chain.colorInChain[game->frog.color])
            game->frog.color = randInt(0, game->chain.ballColors-1);
    }


    if (game->chain.balls[BallChain_GetLastMovingBall(&game->chain)].pos > game->lvl->spiralLen * SLOWFACTOR_POS) {
        if (game->chain.isEndReached)
            game->chain.speed = ROLLING_TO_PIT_SPEED;
        else
            game->chain.speed = game->settings->ballSpd / game->settings->slowFactor;
    }
    else
        game->chain.speed = game->settings->ballSpd;
    
    BulletsArr_CollideWithChainUpdate(&game->bullets, &game->chain, game->lvl->spiral);
    Messages_Update(&game->msgs);
    Game_UpdateTreasure(game);
}

void Game_Draw(Game* game) {
    Level_Draw(game->lvl);
    Game_DrawFinish(game);
    Frog_Draw(&game->frog, game->isIntroEnded);

    if (!game->isIntroEnded) {
        Game_DrawHUD(game);
        Game_DrawIntro(game);
        return;
    }

    if (game->isWon && !game->isOutroEnded) {
        Game_DrawHUD(game);
        Game_DrawOutro(game);
        return;
    }

    if (game->dbMenu || game->dbGameOver || game->dbStats) {
        Game_DrawHUD(game);
        return;
    } 
    
    if (!game->graphics->textureTopLayerFile) {
        BallChain_Draw(&game->chain, false);
    } else {
        BallChain_Draw(&game->chain, false);
        Level_DrawTopLayer(game->lvl);
        BallChain_Draw(&game->chain, true);
    }



    BulletsArr_Draw(&game->bullets);
    Messages_Draw(&game->msgs);
    Game_DrawHUD(game);

    if (game->treasureActive) {
        if (game->treasureBlinking) {
            int delta = (int)(((float)(clock() - game->treasureTime))/(CLOCKS_PER_SEC / TREASURE_BLINK_SPEED));
            if (delta % 2 == 0)
                Game_DrawTreasure(game);
        } else
            Game_DrawTreasure(game);
    }

    if (game->treasureFading) {
        game->treasureAnim.scale += TREASURE_FADING_SPEED;
        Game_DrawTreasure(game);
    }

    if (game->DEBUG) {
        char buff[48];

        sprintf(buff, "isGlowing: %d", game->chain.isGlowing);
        Engine_DrawText(buff, FONT_ARIAL_12, 600, 16);

        int w, h;
        SDL_GetWindowSize(engine.win, &w, &h);
        sprintf(buff, "Window_Size: (%d, %d)", w, h);
        Engine_DrawText(buff, FONT_ARIAL_12, 16, 16);

        sprintf(buff, "Mouse_POS: (%d, %d)", game->mx, game->my);
        Engine_DrawText(buff, FONT_ARIAL_12, 16, 32);
        sprintf(buff, "MAX_COMBO: %d", game->chain.maxCombo);
        Engine_DrawText(buff, FONT_ARIAL_12, 16, 128);

        sprintf(buff, "BULLETS: ");
        Engine_DrawText(buff, FONT_ARIAL_12, 240, 16);
        for (int i = 0; i < game->bullets.len; i++) {
            sprintf(buff, "    %d. (%lf, %lf, %d)", i + 1, 
                game->bullets.bullets[i].x, game->bullets.bullets[i].y, 
                game->bullets.bullets[i].onScreen);
            Engine_DrawText(buff, FONT_ARIAL_12, 240, 16*(i+2));
        }

        Level_DrawDebug(game->lvl);
    }
}

void Game_UpdateTreasure(Game* game) {
    clock_t t = clock();
    float delta = ((float)(t - game->treasureTime)) / CLOCKS_PER_SEC;

    if (game->treasureActive && game->treasureBlinking) {
        if (delta > TREASURE_BLINK_TIME) {
            game->treasureActive = false;
            game->treasureBlinking = false;
            game->treasureTime = t;
            Engine_PlaySound(SND_GEMVANISHES);
        }
    } else if (game->treasureActive && !game->treasureBlinking) {
        if (delta > TREASURE_LIFE_TIME) {
            game->treasureBlinking = true;
            game->treasureTime = t;
        }
    } else {
        if (delta > TREASURE_LIFE_TIME) {
            Engine_PlaySound(SND_JEWELAPPEAR);
            game->treasureActive = true;
            game->treasurePos = randInt(0, game->graphics->coinsLen - 1);
            game->treasureTime = t;
        }
        if (game->treasureFading) {
            if (game->treasureAnim.scale > TREASURE_MAX_SCALE)  {
                game->treasureFading = false;
                game->treasureAnim.color.a = 255;
                game->treasureAnim.scale = 1;
                game->treasureAnim.spd = 0.25;
            }
        }
    }
 
    if (game->treasureActive) {
        //Check bullets collide with treasure
        for (int i = 0; i < game->bullets.len; i++) {
            if (!game->bullets.bullets[i].onScreen) continue;
            float coinX = (game->graphics->coinsPos[game->treasurePos].x + 104) * engine.scale_x;
            float coinY = game->graphics->coinsPos[game->treasurePos].y * engine.scale_y;

            float dist = (game->bullets.bullets[i].x - coinX) * (game->bullets.bullets[i].x - coinX);
            dist += (game->bullets.bullets[i].y - coinY) * (game->bullets.bullets[i].y - coinY);

            if (dist < TREASURE_COLLIDE_DIST) {
                game->chain.chainBonus = 0;
                game->treasureActive = false;
                game->treasureBlinking = false;
                game->treasureFading = true;
                game->treasureAnim.spd = 1;
                game->treasureTime = t;
                game->bullets.bullets[i].onScreen = 0;
                game->score += 500;
                game->totalCoins++;
                char txt[MAX_MSG_LEN];
                sprintf(txt, "BONUS +500");
                Messages_NewMsg(&game->msgs, txt, coinX, coinY, FONT_CANCUN_FLOAT_14, 2);
                Engine_PlaySound(SND_COINGRAB);
            }
        }
    }
}

void Game_DrawTreasure(Game* game) {
    float x = (game->graphics->coinsPos[game->treasurePos].x + 104) * engine.scale_x;
    float y = game->graphics->coinsPos[game->treasurePos].y * engine.scale_y;

    if (game->treasureFading) {
        float rescale = (1 - game->treasureAnim.scale / TREASURE_MAX_SCALE);
        if (rescale > 0)
            game->treasureAnim.color.a = 255.0 * rescale;
        else
            game->treasureAnim.color.a = 0;
    }

    SDL_Color tc = game->treasureAnim.color;
    SDL_Color c = {0, 0, 0, game->treasureAnim.color.a / 2};
    game->treasureAnim.color = c;
    Animation_Draw(&game->treasureAnim, x-12, y+16);

    game->treasureAnim.color = tc;
    Animation_Draw(&game->treasureAnim, x, y);
}

void Game_DrawFinish(Game* game) {
    Animation anim;
    SDL_Rect animFinishRect = {629, 0, 132, 132};
    Animation_Init(&anim, TEX_GAME_OBJECTS, animFinishRect);
    Animation_Set(&anim, 0, 1, 0);
    Animation_Draw(&anim, game->finishPos.x, game->finishPos.y);

    float dist_to_finish = game->lvl->spiralLen - game->chain.balls[0].pos;
    int frame = 12 * ((game->lvl->spiralLen * PIT_OPEN_POS - dist_to_finish) / (game->lvl->spiralLen * PIT_OPEN_POS));
    if (dist_to_finish < game->lvl->spiralLen * PIT_OPEN_POS)
        Animation_SetFrame(&game->finishAnim, frame);

    Animation_Draw(&game->finishAnim, game->finishPos.x, game->finishPos.y);
}

void Game_DrawHUD(Game* game) {
    Animation anim;
    SDL_Rect animBorderRect = {0, 51, 1280, 730};
    Animation_Init(&anim, TEX_GAME_HUD, animBorderRect);
    Animation_Draw(&anim, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 1);

    SDL_Color yellow = {255, 255, 0, 255};

    char buff[64];
    sprintf(buff, "%9d", game->score);
    Engine_DrawTextExt(buff, FONT_CANCUN_12, yellow, 0, 0, 545, 6);

    sprintf(buff, "lvl%d-%d", game->stageID + 1, game->lvlID + 1);
    Engine_DrawTextExt(buff, FONT_CANCUN_12, yellow, 0, 1, 460, 6);

    SDL_Rect animFrogRect = {40, 18, 40, 36};
    Animation_Init(&anim, TEX_GAME_HUD, animFrogRect);
    if (game->lives <= 3) {
        for (int i = 0; i < game->lives; i++) {
            anim.scale = 0.9;
            Animation_Draw(&anim, 57 + (40 * i), 22);
        }
    } else {
        Animation_Draw(&anim, 57, 22);
        sprintf(buff, "x%d", game->lives);
        Engine_DrawText(buff, FONT_CANCUN_FLOAT_14, 77, 12);
    }

    if (game->score < game->settings->gaugeScore) {
        int targetW = 96 * game->score / game->settings->gaugeScore;
        if (game->scoreRect.w < targetW)
            game->scoreRect.w++;
    } else {
        game->scoreRect.y = 0;
        game->scoreRect.w = 96;
    }

    Animation_Init(&anim, TEX_GAME_HUD, game->scoreRect);
    Animation_Draw(&anim, 820 - (96 - game->scoreRect.w) / 2, 16);

    Button_Draw(&game->btnMenu);
    if (game->dbStats && game->isOutroEnded) DialogueBox_Draw(game->dbStats);
    if (game->dbMenu) DialogueBox_Draw(game->dbMenu);
    if (game->dbGameOver) DialogueBox_Draw(game->dbGameOver);
}

void Game_UpdateOutro(Game* game) {
    if (game->particles.len == 0) {
        SDL_Rect animExplRect = {528, 0, 100, 130};
        Animation_Init(&game->particles.anim, TEX_GAME_OBJECTS, animExplRect);
        game->particles.anim.color.b = 255;
        game->particles.anim.scale = 1.5;

        game->particles.x = game->chain.balls[0].x;
        game->particles.y = game->chain.balls[0].y;
        game->particles.pos = game->chain.balls[0].pos;
        game->particles.prevPos = game->chain.balls[0].pos;

        game->score += 100;
        Engine_PlaySound(SND_ENDOFLEVELPOP1);
        Messages_NewMsg(&game->msgs, "+100", game->particles.x, game->particles.y, FONT_CANCUN_FLOAT_14, 2);
        Particles_NewParticle(&game->particles, game->particles.x, game->particles.y);
        return;
    }

    Messages_Update(&game->msgs);
    Particles_Update(&game->particles, EXPLOSION_LAST_FRAME, EXPLOSION_ANIM_SPEED);

    if (game->particles.pos > game->lvl->spiralLen) {
        for (int i = 0; i < game->particles.len; i++) {
            if (game->particles.prtcls[i].onScreen) return;
        }

        if (!game->isOutroEnded)
            Engine_PlaySound(SND_CHANT2);
        game->isOutroEnded = true;
        game->particles.len = 0;

        char buff[8];
        sprintf(buff, "%d", game->score);
        DialogueBox_SetTextStr(game->dbStats, 
            DB_RES_TEXT_POINTS, buff);
        return;
    }

    if (game->particles.pos - game->particles.prevPos > EXPLOSIONS_PAD) {
        game->score += 100;
        Engine_PlaySound(SND_ENDOFLEVELPOP1);
        Messages_NewMsg(&game->msgs, "+100", game->particles.x, game->particles.y, FONT_CANCUN_FLOAT_14, 2);
        Particles_NewParticle(&game->particles, game->particles.x, game->particles.y);
        game->particles.prevPos = game->particles.pos;
    }

    
    game->particles.x += game->lvl->spiral[(int)game->particles.pos].dx * engine.scale_x * EXPLOSION_MOVE_SPEED;
    game->particles.y += game->lvl->spiral[(int)game->particles.pos].dy * engine.scale_y * EXPLOSION_MOVE_SPEED;
    game->particles.pos +=  EXPLOSION_MOVE_SPEED;
}

void Game_DrawOutro(Game* game) {
    Particles_Draw(&game->particles);
    Messages_Draw(&game->msgs);
    return;
}

void Game_UpdateIntro(Game* game) {
    if (game->particles.len == 0) {
        SDL_Rect animSparkleRect = {732, 2253, 30, 30};
        Animation_Init(&game->particles.anim, TEX_GAME_OBJECTS, animSparkleRect);
        game->particles.anim.color.b = 0;
        game->particles.anim.scale = 1.5;

        game->particles.x = (game->lvl->spiralStart.x + 104) * engine.scale_x;
        game->particles.y = game->lvl->spiralStart.y * engine.scale_y;
        game->particles.pos = 0;
        game->particles.prevPos = 0;
        Particles_NewParticle(&game->particles, game->particles.x, game->particles.y);

        game->lightTrailTime = ((float)clock())/CLOCKS_PER_SEC;
        return;
    }

    Particles_Update(&game->particles, SPARKLE_LAST_FRAME, SPARKLES_ANIM_SPEED);

    float distToEnd = game->particles.pos / (float)game->lvl->spiralLen;
    
    if (distToEnd > 0.8  && game->headerFontScale > 0)
        game->headerFontScale -= 0.1;
    else if (distToEnd > 0.1 && game->headerFontScale < 1.0) 
        game->headerFontScale += 0.1;

    if (distToEnd > 0.75  && game->subHeaderFontScale > 0) {
        game->subHeaderFontScale -= 0.1;
    } else if (distToEnd > 0.15 &&  game->subHeaderFontScale < 1.0)
        game->subHeaderFontScale += 0.1;

    if (game->particles.pos > game->lvl->spiralLen) {
        for (int i = 0; i < game->particles.len; i++) {
            if (game->particles.prtcls[i].onScreen) return;
        }

        Engine_PlaySound(SND_ROLLING);
        game->isIntroEnded = true;
        game->particles.len = 0;
        game->time = ((float)clock())/CLOCKS_PER_SEC;
        return;
    }

    if (game->particles.pos - game->particles.prevPos > SPARKLES_PAD) {
        Particles_NewParticle(&game->particles, game->particles.x, game->particles.y);
        game->particles.prevPos = game->particles.pos;
    }
    
    game->particles.x += game->lvl->spiral[(int)game->particles.pos].dx * engine.scale_x * SPARKLES_MOVE_SPEED;
    game->particles.y += game->lvl->spiral[(int)game->particles.pos].dy * engine.scale_y * SPARKLES_MOVE_SPEED;
    game->particles.pos +=  SPARKLES_MOVE_SPEED;

    float currTime = ((float)clock())/CLOCKS_PER_SEC;

    if (currTime - game->lightTrailTime > 0.3) {
        Engine_PlaySoundSfxPitch(SND_FX_LIGHTTRAIL2, game->lightTrailPitch);

        game->lightTrailTime = currTime;

        game->lightTrailPitch -= 1;
    }

}

void Game_DrawIntro(Game* game) {
    Particles_Draw(&game->particles);

    char buff[16];
    SDL_Color color = {255, 255, 255, 255};

    sprintf(buff, "%s", game->graphics->dispName);
    Engine_DrawTextExtScale(buff, FONT_NATIVE_ALIEN_EXT_16, game->subHeaderFontScale, color, false, true, 
        WINDOW_WIDTH - 250, WINDOW_HEIGHT - 95);

    sprintf(buff, "LEVEL %d-%d", game->lvlID + 1, game->difficulty + 1);
    Engine_DrawTextExtScale(buff, FONT_NATIVE_ALIEN_48, game->headerFontScale, color, false, true, 
        WINDOW_WIDTH - 250, WINDOW_HEIGHT - 200);
    return;
}