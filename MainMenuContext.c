//
// Created by Marco on 15/07/2024.
//

#include "MainMenuContext.h"
#include "Commons.h"

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

    TRT_text_draw("Press any key to continue",
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_START},
                  FONT_HEIGHT,
                  MAIN_MENU_FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}

static void renderRatingScreen() {
    TRT_image_draw(ratingScreen, (Vec2) {0, 0}, TRT_window_getSize());
}

static void renderTitleScreen() {
    TRT_image_draw(openScreen, (Vec2) {0, 0}, TRT_window_getSize());
}

void mainMenuContextInit() {
    openScreen = TRT_image_get(MAIN_MENU_OPEN_IMAGE);
    if (openScreen == NULL) {
        exit(EXIT_FAILURE);
    }

    ratingScreen = TRT_image_get(MAIN_MENU_RATING_IMAGE);
    if (ratingScreen == NULL) {
        exit(EXIT_FAILURE);
    }
}

bool mainMenuContextLoop() {
    if (!TRT_animation_isFading())
        renderers[currentRenderer]();

    return close;
}

void mainMenuContextClose() {
    free(openScreen->data);
    free(openScreen);
}

void mainMenuKeyboardCallback(uint32_t key) {
    TRT_animation_startFade();
    currentRenderer++;

    if (currentRenderer == renderersCount)
        close = true;
}

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y) {
    mainMenuKeyboardCallback('a');
}