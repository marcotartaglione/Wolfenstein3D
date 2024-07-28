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

}

bool gameContextLoop() {
    return false;
}

void gameContextClose() {

}

void gameKeyboardCallback(uint32_t key) {

}

void gameMouseCallback(Click click, uint32_t x, uint32_t y) {

}

void gameReduceSize() {
    gameSize.x -= GAME_SIZE_X_CHANGE_FACTOR;
    gameSize.y -= GAME_SIZE_Y_CHANGE_FACTOR;

    if (gameSize.x < GAME_SIZE_MIN_X || gameSize.y < GAME_SIZE_MIN_Y) {
        gameSize.y = GAME_SIZE_MIN_Y;
        gameSize.x = GAME_SIZE_MIN_X;
    }
}

void gameIncreaseSize() {
    gameSize.x += GAME_SIZE_X_CHANGE_FACTOR;
    gameSize.y += GAME_SIZE_Y_CHANGE_FACTOR;

    if (gameSize.x > GAME_SIZE_MAX_X || gameSize.y > GAME_SIZE_MAX_Y) {
        gameSize.x = GAME_SIZE_MAX_X;
        gameSize.y = GAME_SIZE_MAX_Y;
    }
}

void gameDrawFrame() {
    TRT_window_fill(GAME_FRAME_BACKGROUND_COLOR);

    Vec2 *size = TRT_text_size("Use arrows to size\nENTER to accept\nESC to cancel",
                  NULL,
                  FONT_HEIGHT,
                  FONT_SPACE_WIDTH,
                  FONT_LETTER_SPACING,
                  FONT_LINE_OFFSET);

    uint32_t yWindow = TRT_window_getSize().y;
    uint32_t yOffset = yWindow - size->y - gameSize.y;

    TRT_window_DrawRectangle((Vec2) {ELEMENT_ALIGN_CENTER, yWindow - yOffset / 2 - gameSize.y}, gameSize, 0x000000);

    TRT_text_draw("Use arrows to size\nENTER to accept\nESC to cancel",
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_START},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);

    free(size);
}