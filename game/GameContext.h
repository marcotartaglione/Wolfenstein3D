//
// Created by Marco on 28/07/2024.
//

#ifndef WOLFENSTEIN3D_GAMECONTEXT_H
#define WOLFENSTEIN3D_GAMECONTEXT_H

#include "TRT_Engine.h"
#include "../Context.h"
#include "../Commons.h"
#include "../Episodes.h"

#define GAME_SIZE_MAX_X         (304)
#define GAME_SIZE_MAX_Y         (154)
#define GAME_SIZE_MIN_X         (82)
#define GAME_SIZE_MIN_Y         (42)

#define GAME_FRAME_BACKGROUND_COLOR     (0x004141)
#define GAME_FRAME_OFFSET_FROM_BOTTOM   (40)

#define GAME_FOV_ANGLE                  (PI / 2)

static Vec2 gameSize = {GAME_SIZE_MAX_X, GAME_SIZE_MAX_Y};

void Game_setSize(Vec2 size);

Vec2 Game_getSize();

static uint32_t currentEpisode = 0;
static uint32_t currentFloor = 0;
static uint8_t difficulty = 0;

static Map* currentMap;

void Game_setEpisode(uint32_t episode);

void Game_setFloor(uint32_t floor);

void Game_setDifficulty(uint8_t newDifficulty);

void Game_contextInit();

LoopResult Game_contextLoop();

void Game_contextClose();

void Game_keyboardCallback(uint32_t key);

void Game_mouseCallback(Click click, uint32_t x, uint32_t y);

void Game_drawFrame(Vec2 frameSize);

static void Game_drawHUD();

static void Game_drawPlayerView();

static void Game_drawTextureColumn(int column, float hitPerc, float distance, Wall wall);

static Wall Game_raycast(float maxDistance, float angle, float *distance, float *hitPerc, int *side);

#define GAME_HUD_PLAYER_STATS_IMAGE ("./assets/hud/stats/playerStats.png")
#define GAME_HUD_SIZE               (Vec2) {320, 40}
#define GAME_HUD_OFFSET_BOTTOM      (5)

static Image *playerStats;

#endif //WOLFENSTEIN3D_GAMECONTEXT_H
