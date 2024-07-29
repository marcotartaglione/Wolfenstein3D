//
// Created by Marco on 28/07/2024.
//

#ifndef WOLFENSTEIN3D_GAMECONTEXT_H
#define WOLFENSTEIN3D_GAMECONTEXT_H

#include "TRT_Engine.h"
#include "Context.h"
#include "Commons.h"

#define GAME_SIZE_MAX_X         (304)
#define GAME_SIZE_MAX_Y         (154)
#define GAME_SIZE_MIN_X         (82)
#define GAME_SIZE_MIN_Y         (42)

#define GAME_FRAME_BACKGROUND_COLOR (0x004141)
#define GAME_FRAME_OFFSET_FROM_TOP  (3)

static Vec2 gameSize = {GAME_SIZE_MAX_X, GAME_SIZE_MAX_Y};

void gameContextInit();

bool gameContextLoop();

void gameContextClose();

void gameKeyboardCallback(uint32_t key);

void gameMouseCallback(Click click, uint32_t x, uint32_t y);

void gameReduceSize();

void gameIncreaseSize();

void gameDrawFrame(Vec2 frameSize);

Vec2 gameGetSize();

void gameSetSize(Vec2 size);

#endif //WOLFENSTEIN3D_GAMECONTEXT_H
