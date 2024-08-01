//
// Created by Marco on 28/07/2024.
//
#include "GameContext.h"

WolfensteinContext gameContext = {
        gameContextInit,
        gameContextLoop,
        gameKeyboardCallback,
        gameMouseCallback,
        gameContextClose};

void gameContextInit() {
    playerStats = TRT_image_get(GAME_HUD_PLAYER_STATS_IMAGE);
    if (playerStats == NULL) {
        exit(EXIT_FAILURE);
    }
}

LoopResult gameContextLoop() {
    gameDrawFrame(gameSize);
    gameDrawHUD();

    return LOOP_RESULT_IDLE;
}

void gameContextClose() {
    TRT_image_free(playerStats);
}

void gameKeyboardCallback(uint32_t key) {

}

void gameMouseCallback(Click click, uint32_t x, uint32_t y) {

}

void gameDrawFrame(Vec2 frameSize) {
    TRT_window_fill(GAME_FRAME_BACKGROUND_COLOR);

    uint32_t yWindow = TRT_window_getSize().y;
    uint32_t yOffset = yWindow - GAME_FRAME_OFFSET_FROM_BOTTOM - frameSize.y;

    TRT_window_DrawRectangle((Vec2) {ELEMENT_ALIGN_CENTER, yWindow - yOffset / 2 - frameSize.y}, frameSize, 0x000000, true);
}

Vec2 gameGetSize() {
    return gameSize;
}

void gameSetSize(Vec2 size) {
    gameSize = size;
}

void gameSetEpisode(uint32_t episode) {
    currentEpisode = episode;
}

void gameSetDifficulty(uint8_t newDifficulty) {
    difficulty = newDifficulty;
}

static void gameDrawHUD() {
    TRT_image_draw(playerStats,
                   (Vec2) {0, GAME_HUD_OFFSET_BOTTOM},
                   GAME_HUD_SIZE);
}