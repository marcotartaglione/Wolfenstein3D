//
// Created by Marco on 15/07/2024.
//

#include "MainMenuContext.h"

#include "GameContext.h"
#include "../Commons.h"

static void renderAttentionScreen() {
    Vec2 windowSize = TRT_window_getSize();

    drawHeaderLine();

    TRT_text_draw("Attention",
                  (Vec2) {ELEMENT_ALIGN_CENTER, windowSize.y - HEADER_LINE_Y_OFFSET - (HEADER_LINE_BACKGROUND_HEIGHT - FONT_HEIGHT) / 2},
                  FONT_HEIGHT,
                  ATTENTION_SCREEN_TITLE_FONT_COLOR,
                  TEXT_ALIGN_CENTER);

    TRT_text_draw("This game is NOT original\nFeel free to distribute it\n(With credits)\n\nM. Tartaglione",
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  MAIN_MENU_FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}

static void renderRatingScreen() {
    TRT_image_draw(MainMenu_RatingScreen, (Vec2) {0, 0}, TRT_window_getSize());
}

static void renderTitleScreen() {
    TRT_image_draw(MainMenu_OpenScreen, (Vec2) {0, 0}, TRT_window_getSize());
}

void mainMenuContextInit() {
    MainMenu_LoopResult = LOOP_RESULT_IDLE;
    MainMenu_CurrentRenderer = 0;

    MainMenu_OpenScreen = TRT_image_get(MAIN_MENU_OPEN_IMAGE);
    if (MainMenu_OpenScreen == NULL) {
        exit(EXIT_FAILURE);
    }

    MainMenu_RatingScreen = TRT_image_get(MAIN_MENU_RATING_IMAGE);
    if (MainMenu_RatingScreen == NULL) {
        exit(EXIT_FAILURE);
    }
}

LoopResult mainMenuContextLoop() {
    if (MainMenu_LoopResult != LOOP_RESULT_IDLE)
        return MainMenu_LoopResult;

    if (!TRT_animation_isFading())
        editormenu_renderers[MainMenu_CurrentRenderer]();

    return MainMenu_LoopResult;
}

void mainMenuContextClose() {
    free(MainMenu_OpenScreen->data);
    free(MainMenu_OpenScreen);
}

void mainMenuKeyboardCallback(uint32_t key) {
    if(TRT_animation_startFade())
        MainMenu_CurrentRenderer++;

    if (MainMenu_CurrentRenderer == MainMenu_RenderersCount)
        MainMenu_LoopResult = LOOP_RESULT_NEXT;
}

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y) {
    mainMenuKeyboardCallback('a');
}