//
// Created by Marco on 28/07/2024.
//

#ifndef WOLFENSTEIN3D_GAMECONTEXT_H
#define WOLFENSTEIN3D_GAMECONTEXT_H

#include "TRT_Engine.h"
#include "Context.h"

#define GAME_SIZE_MAX_X (304)
#define GAME_SIZE_MAX_Y (154)
#define GAME_SIZE_MIN_X (82)
#define GAME_SIZE_MIN_Y (42)

#define FRAME_BACKGROUND_COLOR (004141)

Vec2 gameSize = {GAME_SIZE_MAX_X, GAME_SIZE_MAX_Y};

void gameContextInit();

bool gameContextLoop();

void gameContextClose();

void gameKeyboardCallback(uint32_t key);

void gameMouseCallback(Click click, uint32_t x, uint32_t y);

WolfensteinContext gameContext = {
        gameContextInit,
        gameContextLoop,
        gameKeyboardCallback,
        gameMouseCallback,
        gameContextClose};

#endif //WOLFENSTEIN3D_GAMECONTEXT_H
