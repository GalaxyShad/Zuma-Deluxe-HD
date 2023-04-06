#include "Game.h"

#include "../global/UsefulTypes.h"

#include "Level.h"

#include "objects/Frog.h"
#include "objects/Bullet.h"
#include "objects/BallChain.h"

typedef struct Board {
    Level* lvl;
	LevelSettings* settings;
	LevelGraphics* graphics;
} Board;

typedef struct Game {
    v2f_t mousePos;

    Frog frog;
    BallChain chain;
	BulletsArr bullets;

    Board board;

} Game;



HGame Game_Create(int lvlID) {
    Game* game = malloc(sizeof(*game));

    game->mousePos.x = 0;
    game->mousePos.y = 0;

    const int stageID = 0;
    game->board.lvl      = LevelMgr_GetLevelFromStage(stageID, lvlID);
    Level_Load(game->board.lvl);
    game->board.graphics = LevelMgr_GetLevelGraphics(game->board.lvl);

    

    return game;
}

