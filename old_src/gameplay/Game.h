#ifndef GAME_H
#define GAME_H

typedef void* HGame;

HGame Game_Create(int lvlID);

void Game_Update(HGame game);

void Game_Draw(HGame game);

void Game_Destroy(HGame game);

#endif