#include <BallChain.h>

void Ball_Init(Ball* ball, float pos, float x, float y, char isPusher, 
	char color, char bonus) {
	ball->pos = pos;
	ball->x = (x + 104) * 1.5;
	ball->y = y * 1.5;
	ball->color = color;
	ball->isInserted = 0;
	ball->isExploding = 0;
	ball->inTunnel = false;
	ball->drawPrority = false;
	ball->bonus = bonus;
	ball->spd = 0;
	ball->combo = 0;
	ball->goBack = false;
	ball->isSingle = 0;
	ball->ang = 0;
	ball->startAnim = false;
	
	Ball_InitAnim(ball);
}

void Ball_InitAnim(Ball* ball) {
	SDL_Rect animBallRect = {48 * ball->color, 0, 48, 48};
	Animation_Init(&ball->anim, TEX_GAME_OBJECTS, animBallRect);
	if (ball->startAnim)
		ball->anim.color.a = 0;
	if (ball->color == 0)
		Animation_Set(&ball->anim, 0, 47, 0);
	else 
		Animation_Set(&ball->anim, 0, 50, 0);
}

void Ball_Draw(Ball* ball) {
	if (!ball->isExploding)
		Animation_SetFrame(&ball->anim, ((int)ball->pos) % ball->anim.endFrame);

	if (ball->startAnim) {
		ball->anim.color.a+=15;
		if (ball->anim.color.a >= 255) {
			ball->anim.color.a = 255;
			ball->startAnim = false;
		}
	}

	Animation_SetAngle(&ball->anim, ball->ang - 90);
	Animation_Draw(&ball->anim, ball->x, ball->y);
}

void Ball_Copy(Ball* src, Ball* dst) {
	dst->x = src->x;
	dst->y = src->y;
	dst->pos = src->pos;
	dst->color = src->color;
	dst->combo = src->combo;
	dst->bonus = src->bonus;
	dst->anim = src->anim;
	dst->spd = src->spd;
	dst->goBack = src->goBack;
	dst->isInserted = src->isInserted;
	dst->isExploding = src->isExploding;
	dst->anim = src->anim;
	dst->ang = src->ang;
	dst->inTunnel = src->inTunnel;
	dst->drawPrority = src->drawPrority;
	dst->startAnim = src->startAnim;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int BallChain_GetLastMovingBall(BallChain* ballChain) {
	for (int i = ballChain->len - 1; i >= 0; i--) {
		if (!BallChain_CollidesFront(ballChain, i, BALLS_CHAIN_PAD_ROUGH))
			return i;
	}
	return ballChain->len - 1;
}

void BallChain_Init(BallChain* ballChain, Level* lvl, LevelSettings* settings) {
	ballChain->speed = settings->ballSpd;
	ballChain->maxCombo = 0;
	ballChain->isGenerating = 1;
	ballChain->isEndReached = 0;
	ballChain->isGlowing = 0;
	ballChain->chainBonus = 0;
	
	ballChain->maxChainBonus = 0;
	ballChain->totalCombos = 0;

	ballChain->startX = lvl->spiralStart.x;
	ballChain->startY = lvl->spiralStart.y;

	ballChain->ballColors = settings->ballColors;

	for (int i = 0; i < 6; i++)
		ballChain->colorInChain[i] = 0;

	Ball_Init(
		&ballChain->balls[0],
		0,
		ballChain->startX,
		ballChain->startY,
		0,
		randInt(0, ballChain->ballColors - 1),
		0
	);
	ballChain->len = 1;

	SDL_Rect aRect = {134, 2595, 48, 50};
	Animation_Init(&ballChain->ballShadow, TEX_GAME_OBJECTS, aRect);
	ballChain->ballShadow.color.a = 128;
}

void BallChain_UpdateColorInChain(BallChain* ballChain) {
	for (int i = 0; i < 6; i++)
		ballChain->colorInChain[i] = 0;

	for (int i = 0; i < ballChain->len; i++) {
		ballChain->colorInChain[ballChain->balls[i].color] = 1;
	}
}

char BallChain_CollidesBack(BallChain* ballChain, int idx, int collideDist) {
	if (idx == ballChain->len - 1) return 0;

	return (ballChain->balls[idx].pos - ballChain->balls[idx + 1].pos < collideDist);
}

char BallChain_CollidesFront(BallChain* ballChain, int idx, int collideDist) {
	if (idx == 0) return 0;

	return (ballChain->balls[idx - 1].pos - ballChain->balls[idx].pos < collideDist);
}

void BallChain_Append(BallChain* ballChain, Level* lvl, LevelSettings* settings) {
	bool isSingle = 0;
	char color;
	if (ballChain->balls[ballChain->len-1].isSingle) {
		color = ballChain->balls[ballChain->len-1].color;
		while (color == ballChain->balls[ballChain->len-1].color)
			color = randInt(0, ballChain->ballColors - 1);

	} else if (randInt(1, 100) < settings->singleChance) {
		color = ballChain->balls[ballChain->len - 1].color;
		while (color == ballChain->balls[ballChain->len - 1].color)
			color = randInt(0, ballChain->ballColors - 1);

		ballChain->balls[ballChain->len].isSingle = 1;

	}  else if (randInt(1, 100) < settings->repeatChance) {
		int start, end;
		BallChain_FindSubChain(ballChain, ballChain->len-1, &start, &end);
		if (end - start + 1 < 4)
			color = ballChain->balls[ballChain->len - 1].color;
		else {
			while (color == ballChain->balls[ballChain->len - 1].color)
					color = randInt(0, ballChain->ballColors - 1);
		}

	} else {
	
			color = randInt(0, ballChain->ballColors - 1);
	}


	Ball_Init(
		&ballChain->balls[ballChain->len],
		0,
		lvl->spiralStart.x,
		lvl->spiralStart.y,
		0,
		color,
		0
	);
	ballChain->len++;
}

void BallChain_Update(BallChain* ballChain, SpiralDot* spiral, int spiralLen, int* score, Messages* msgs) {
	for (int i = ballChain->len-1; i >= 0; i--) {
		Ball* ball = &ballChain->balls[i];


		if (i == ballChain->len-1) {
			if (ballChain->speed == ROLLING_TO_PIT_SPEED)
				ball->spd = ROLLING_TO_PIT_SPEED;
			else 
				ball->spd += BALL_ACC;

			if (ball->spd > ballChain->speed)
				ball->spd = ballChain->speed;

		} else {
			Ball* preBall = &ballChain->balls[i+1];
			if (BallChain_CollidesBack(ballChain, i, BALLS_CHAIN_PAD_ROUGH)) {
				if (ball->goBack) {
					for (int j = i; j < ballChain->len; j++) {
						ballChain->balls[j].spd = ballChain->balls[i].spd / BALL_WEIGHT_RATIO;

						if (!BallChain_CollidesBack(ballChain, j, BALLS_CHAIN_PAD_ROUGH)) {
							Engine_PlaySound(SND_BALLCLICK1);
							break;
						}
					}
					ballChain->balls[i].spd = 0;

					if (!ballChain->balls[i].isExploding && !ballChain->isEndReached) {
						int start, end;
						BallChain_FindSubChain(ballChain, i, &start, &end);

						if (end - start + 1 >= 3) {
							BallChain_ExplodeBalls(ballChain, start, end);
							int combo = 0;
							for (int i = start; i <= end; i++){
								if (ballChain->balls[i].combo) {
									combo = ballChain->balls[i].combo;
									break;
								}
							}

							if (end + 1 < ballChain->len) {
								ballChain->balls[end+1].combo = combo + 1;
							}

							int score_delta = (end - start + 1) * 10 + combo * 100;
							*score = *score + score_delta;
							char txt[MAX_MSG_LEN];
							if (combo) {
								sprintf(txt, "+%d\nCOMBO x%d", score_delta, combo);
    							// NEW
								ballChain->totalCombos++;
							}else
								sprintf(txt, "+%d", score_delta);

							if (combo > ballChain->maxCombo) ballChain->maxCombo = combo;

							float x = ballChain->balls[(int)(end - ((end - start + 1) / 2))].x;
							float y = ballChain->balls[(int)(end - ((end - start + 1) / 2))].y;
							Messages_NewMsg(msgs, txt, x, y, FONT_CANCUN_FLOAT_14, ballChain->balls[start].color);
						} else {
							ballChain->balls[i + 1].combo = 0;
						}
					}

					ball->goBack = false;
				} else {
					ball->spd = preBall->spd;
				}
			} else {
				if (ball->color == preBall->color && !ballChain->isEndReached) {
					ball->spd -= BALL_DECC;
					if (ball->spd < BALL_MAX_BACK_SPEED)
						ball->spd = BALL_MAX_BACK_SPEED;
					ball->goBack = true;
				} else {
					ball->spd -= fmin(fabs(ball->spd), BALL_FRC) * fsign(ball->spd);
				}
			}
		}
	}

	for (int i = ballChain->len-1; i >= 0; i--) {
		Ball* ball = &ballChain->balls[i];

		if (ball->pos < 0) {
			if (ballChain->len == 1)
				ball->pos = 0;
			else
				BallChain_Destroy(ballChain, i, i);
		}

		if ((int)ball->pos >= spiralLen) {
			BallChain_Destroy(ballChain, i, i);
			ballChain->isGenerating = 0;
			ballChain->isEndReached = 1;
		}

		float xx, yy;
		xx = (ballChain->startX+104) * 1.5;
		yy = ballChain->startY * 1.5;
		
		ball->pos += ball->spd;

		if (i != ballChain->len-1) {
			Ball* pBall = &ballChain->balls[i+1];

			if (!pBall->isInserted) {
				while (BallChain_CollidesBack(ballChain, i, BALLS_CHAIN_PAD)) {
					ball->pos++;
				}
			} else {
				if (BallChain_CollidesBack(ballChain, i, BALLS_CHAIN_PAD))
					ball->pos+=BALL_INSERTION_SPD;
				else {
					int start, end;
					BallChain_FindSubChain(ballChain, i + 1, &start, &end);

					if (end - start + 1 >= 3) {
						BallChain_ExplodeBalls(ballChain, start, end);
						ballChain->chainBonus++;
						if (ballChain->maxChainBonus < ballChain->chainBonus)
							ballChain->maxChainBonus = ballChain->chainBonus;


						int score_delta = (end - start + 1) * 10;
						if (ballChain->chainBonus >= 5)
							score_delta += 100 + (ballChain->chainBonus - 5) * 10;

						*score = *score + score_delta;
						if (end + 1 < ballChain->len)
							ballChain->balls[end + 1].combo = 1;

						char txt[MAX_MSG_LEN];
						if (ballChain->chainBonus >= 5)
							sprintf(txt, "+%d\nCHAIN BONUS x%d", score_delta, ballChain->chainBonus);
						else
							sprintf(txt, "+%d", score_delta);
						float x = ballChain->balls[(int)(end - ((end - start + 1) / 2))].x;
						float y = ballChain->balls[(int)(end - ((end - start + 1) / 2))].y;
						Messages_NewMsg(msgs, txt, x, y, FONT_CANCUN_FLOAT_14, ballChain->balls[start].color);
					} else {
						ballChain->chainBonus = 0;
					}
					pBall->isInserted = 0;
				}
			}
		} 


		if (i == 0 && ballChain->balls[i].isInserted && !ballChain->balls[i].isExploding) {
			int start, end;
			BallChain_FindSubChain(ballChain, i, &start, &end);

			if (end - start + 1 >= 3) {
				BallChain_ExplodeBalls(ballChain, start, end);
				ballChain->chainBonus++;
				if (ballChain->maxChainBonus < ballChain->chainBonus)
					ballChain->maxChainBonus = ballChain->chainBonus;

				int score_delta = (end - start + 1) * 10;
				if (ballChain->chainBonus >= 5)
					score_delta += 100 + (ballChain->chainBonus - 5) * 10;
				*score = *score + score_delta;

				if (end + 1 < ballChain->len)
					ballChain->balls[end + 1].combo = 1;

				char txt[MAX_MSG_LEN];
				if (ballChain->chainBonus >= 5)
					sprintf(txt, "+%d\nCHAIN BONUS x%d", score_delta, ballChain->chainBonus);
				else
					sprintf(txt, "+%d", score_delta);
				float x = ballChain->balls[(int)(end - ((end - start + 1) / 2))].x;
				float y = ballChain->balls[(int)(end - ((end - start + 1) / 2))].y;
				Messages_NewMsg(msgs, txt, x, y, FONT_CANCUN_FLOAT_14, ballChain->balls[start].color);
			} else {
				ballChain->chainBonus = 0;
			}
			ballChain->balls[i].isInserted = 0;
		}


		int j;
		for (j = 0; j < (int)ball->pos; j++) {
			xx += spiral[j].dx * engine.scale_x;
			yy += spiral[j].dy * engine.scale_x;

			ball->ang = atan2(spiral[j].dy, spiral[j].dx) * RAD_TO_DEG;
			ball->inTunnel = spiral[j].t1;
			ball->drawPrority = spiral[j].t2;
		}

		xx = lerp(xx, xx + spiral[j].dx * engine.scale_x, modff(ball->pos, NULL));
		yy = lerp(yy, yy + spiral[j].dy * engine.scale_x, modff(ball->pos, NULL));

		ball->x = xx;
		ball->y = yy;
	}

	for (int i = 0; i < ballChain->len; i++) {
		if (ballChain->balls[i].isExploding) {
			if (ballChain->balls[i].anim.endFrame - 1 == (int)ballChain->balls[i].anim.frame) {
				int start, end;
				BallChain_FindSubChain(ballChain, i, &start, &end);
				BallChain_Destroy(ballChain, start, end);
			}
		}
	}
	BallChain_UpdateColorInChain(ballChain);
	BallChain_UpdateCombos(ballChain);

	if (ballChain->isGlowing) {
		float delta = ((float)(clock() - ballChain->glowTime))/CLOCKS_PER_SEC;
		if (delta > BALLCHAIN_BLINK_TIME) ballChain->isGlowing = 0;
	}
}

void BallChain_UpdateCombos(BallChain* ballChain) {
	ballChain->balls[0].combo = 0;
	for (int i = 1; i < ballChain->len; i++){
		if ((BallChain_CollidesFront(ballChain, i, BALLS_CHAIN_PAD) && !ballChain->balls[i - 1].isExploding) ||
		 (!ballChain->balls[i - 1].isExploding && ballChain->balls[i - 1].color != ballChain->balls[i].color))
				ballChain->balls[i].combo = 0;
	}
}

void BallChain_ExplodeBalls(BallChain* ballChain, int start, int end) {
	SDL_Rect animBallRect = {395, 0, 105, 120};	
	int combo = 0;
	for (int i = start; i <= end; i++) {
		Animation* ballAnim = &ballChain->balls[i].anim;
		if (ballChain->balls[i].combo > combo) combo = ballChain->balls[i].combo;
		
		Animation_Init(ballAnim, TEX_GAME_OBJECTS, animBallRect);

		switch(ballChain->balls[i].color) {
			case 0: 
				ballAnim->color.r = 0x00; 
				ballAnim->color.g = 0xFE; 
				ballAnim->color.b = 0xFD; 
				break;
			case 1:
				ballAnim->color.r = 0x00; 
				ballAnim->color.g = 0xFA; 
				ballAnim->color.b = 0x44; 
				break;
			case 2:
				ballAnim->color.r = 0xFD; 
				ballAnim->color.g = 0xFD; 
				ballAnim->color.b = 0x06; 
				break;
			case 3:
				ballAnim->color.r = 0xF9; 
				ballAnim->color.g = 0x6F; 
				ballAnim->color.b = 0x4C; 
				break;
			case 4:
				ballAnim->color.r = 0xFD; 
				ballAnim->color.g = 0x88; 
				ballAnim->color.b = 0xF8; 
				break;
			case 5:
				ballAnim->color.r = 0xFD; 
				ballAnim->color.g = 0xEA; 
				ballAnim->color.b = 0xBB; 
				break;
		}

		Animation_Set(ballAnim, 0, 16, BALL_EXPLODE_SPEED);

		ballChain->balls[i].isExploding = 1;
	}

	float comboPitch = 0;
	switch (combo) {
		case 0:
			Engine_PlaySound(SND_BALLSDESTROYED1);
			break;
		case 1:
			Engine_PlaySound(SND_BALLSDESTROYED2);
			comboPitch = 2;
			break;
		case 2:
			Engine_PlaySound(SND_BALLSDESTROYED3);
			comboPitch = 4;
			break;
		case 3:
			Engine_PlaySound(SND_BALLSDESTROYED4);
			comboPitch = 6;
			break;
		default:
			Engine_PlaySound(SND_BALLSDESTROYED5);
			comboPitch = 8;
			break;
	}
	Engine_PlaySoundSfxPitch(SND_FX_CHIME1, comboPitch);
}

void BallChain_FindSubChain(BallChain* ballChain, int idx, int* start, int* end) {
	*start = idx; *end = idx;

	for (int i = idx; i < ballChain->len - 1; i++) {
		if (BallChain_CollidesBack(ballChain, i, BALLS_CHAIN_PAD_ROUGH)  
			&& ballChain->balls[i].color == ballChain->balls[i + 1].color
			&& ballChain->balls[i].isExploding == ballChain->balls[i + 1].isExploding) {
			*end = i + 1;
		} else {
			break;
		}
	}

	for (int i = idx; i > 0; i--) {
		if (BallChain_CollidesFront(ballChain, i, BALLS_CHAIN_PAD_ROUGH)  
			&& ballChain->balls[i].color == ballChain->balls[i - 1].color
			&& ballChain->balls[i].isExploding == ballChain->balls[i - 1].isExploding) {
			*start = i - 1;
		} else {
			break;
		}
	}
}

void BallChain_Destroy(BallChain* ballChain, int start, int end) {
	int amount = end - start + 1;
	ballChain->len -= amount;
	for (int i=start; i < ballChain->len; i++){
		Ball_Copy(&ballChain->balls[i+amount], &ballChain->balls[i]);
	}
}

void BallChain_Insert(BallChain* ballChain, int idx, char color, bool isInsertBack) {
	Ball curr;
	Ball prev;

	Ball_Copy(&ballChain->balls[idx], &prev);
	for (int i = idx + 1; i < ballChain->len + 1; i++) {
		Ball_Copy(&ballChain->balls[i], &curr);
		Ball_Copy(&prev, &ballChain->balls[i]);
		Ball_Copy(&curr, &prev);
	}

	if (isInsertBack) {
		if (!BallChain_CollidesBack(ballChain, idx, BALLS_CHAIN_PAD)) {
			ballChain->balls[idx].pos = ballChain->balls[idx-1].pos - BALLS_CHAIN_PAD;
		} else {
			ballChain->balls[idx].pos = ballChain->balls[idx-1].pos;
			ballChain->balls[idx].x = ballChain->balls[idx-1].x;
			ballChain->balls[idx].y = ballChain->balls[idx-1].y;
		}
	}


	ballChain->balls[idx].color = color;
	ballChain->balls[idx].isExploding = 0;
	ballChain->balls[idx].isInserted = 1;

	Engine_PlaySound(SND_BALLCLICK2);
	Ball_InitAnim(&ballChain->balls[idx]);
	ballChain->len++;
}

void BallChain_Draw(BallChain* ballChain, bool priorBallsOnly) {
	// Shadow
	for (int i = 0; i < ballChain->len; i++) {
		if (ballChain->balls[i].drawPrority == priorBallsOnly &&
			!ballChain->balls[i].isExploding) {
				ballChain->ballShadow.color.a = ballChain->balls[i].anim.color.a;
				if (ballChain->ballShadow.color.a > 128)
					ballChain->ballShadow.color.a = 128;
				Animation_Draw(&ballChain->ballShadow, ballChain->balls[i].x-4, ballChain->balls[i].y+6);
			}
	}
	// Balls
	for (int i = 0; i < ballChain->len; i++) {
		if (ballChain->balls[i].drawPrority == priorBallsOnly)
			Ball_Draw(&ballChain->balls[i]);
	}
	// Blinking
	if (ballChain->isGlowing) {
		Animation whiteCircle;
		SDL_Rect circleRect = {182, 2595, 48, 50};
		Animation_Init(&whiteCircle, TEX_GAME_OBJECTS, circleRect);
		whiteCircle.color.a = 128;
		for (int i = 0; i < ballChain->len; i++) {
			if (ballChain->balls[i].drawPrority == priorBallsOnly &&
				!ballChain->balls[i].isExploding) {
					Animation_Draw(&whiteCircle, ballChain->balls[i].x, ballChain->balls[i].y);
			}
		}
	}
}