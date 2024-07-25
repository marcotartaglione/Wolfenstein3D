//
// Created by Marco on 15/07/2024.
//

#include "MainMenuContext.h"
#include "Commons.h"

static void renderAttentionScreen() {
    Vec2 windowSize = TRT_window_getSize();

    drawHeaderLine();

    TRT_text_draw("Attention",
                  (Vec2) {0, windowSize.y - HEADER_LINE_BACKGROUND_HEIGHT + FONT_HEIGHT},
                  FONT_HEIGHT,
                  ATTENTION_SCREEN_TITLE_FONT_COLOR,
                  ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_NONE, TEXT_ALIGN_CENTER);

    TRT_text_draw("This game is NOT original\nFeel free to distribute it\n(With credits)\n\nM. Tartaglione",
                  (Vec2) {0, 0},
                  FONT_HEIGHT,
                  MAIN_MENU_FONT_COLOR,
                  ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER, TEXT_ALIGN_CENTER);

    TRT_text_draw("Press any key to continue",
                  (Vec2) {0, FONT_HEIGHT + 5},
                  FONT_HEIGHT,
                  MAIN_MENU_FONT_COLOR,
                  ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_NONE, TEXT_ALIGN_CENTER);
}

static void renderTitleScreen() {
    TRT_image_draw(openScreen, (Vec2) {0, 0}, TRT_window_getSize());
}

void mainMenuContextInit() {
    openScreen = TRT_image_get(MAIN_MENU_OPEN_IMAGE);
    if (openScreen == NULL) {
        exit(EXIT_FAILURE);
    }
}

#include <stdlib.h>

static bool fade = false;

bool mainMenuContextLoop() {
    if (close)
        return true;

    if (fade) {
        switch (TRT_animation_fade(MAIN_MENU_FADE_SPEED)) {
            case FADE_IN:
                break;
            case FADE_OUT:
                renderers[currentRenderer]();
                break;
            case FADE_OVER:
                fade = false;
                break;
        }
    } else {
        renderers[currentRenderer]();
    }

    return close;
}

void mainMenuContextClose() {
    free(openScreen->data);
    free(openScreen);
}

void mainMenuKeyboardCallback(uint32_t key) {
    fade = true;
    currentRenderer++;

    if (currentRenderer == renderersCount)
        close = true;
}

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y) {
    mainMenuKeyboardCallback('a');
}

static void renderTest() {
    TRT_window_fill(0x000000);

    TRT_text_draw("ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n1234567890\n!@#$%^&*()_+",
                  (Vec2) {10, 10},
                  FONT_HEIGHT,
                  MAIN_MENU_FONT_COLOR,
                  ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER, TEXT_ALIGN_CENTER);
}