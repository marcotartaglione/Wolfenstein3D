//
// Created by Marco on 15/07/2024.
//

#include "MainMenu.h"
#include "Commons.h"

static void drawHeaderLine() {
    Vec2 windowSize = TRT_getWindowSize();

    TRT_fillScreenWithColor(MAIN_MENU_BACKGROUND_COLOR);

    TRT_windowDrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_BACKGROUND_Y_OFFSET -
                                       HEADER_LINE_BACKGROUND_HEIGHT},
                            (Vec2) {-1, HEADER_LINE_BACKGROUND_HEIGHT},
                            HEADER_LINE_BACKGROUND_COLOR);

    TRT_windowDrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_BACKGROUND_Y_OFFSET -
                                       HEADER_LINE_BACKGROUND_HEIGHT - 2},
                            (Vec2) {-1, 1},
                            HEADER_LINE_BACKGROUND_COLOR);
}

static void renderAttentionScreen() {
    Vec2 windowSize = TRT_getWindowSize();

    drawHeaderLine();

    TRT_windowDrawText("Attention",
                       (Vec2) {0, windowSize.y - HEADER_LINE_BACKGROUND_HEIGHT + MAIN_MENU_FONT_HEIGHT},
                       MAIN_MENU_FONT_HEIGHT,
                       ATTENTION_SCREEN_TITLE_FONT_COLOR,
                       ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_NONE, TEXT_ALIGN_CENTER);

    TRT_windowDrawText("This game is NOT original\nFeel free to distribute it\n(With credits)\n\nM. Tartaglione",
                       (Vec2) {0, 0},
                       MAIN_MENU_FONT_HEIGHT,
                       MAIN_MENU_FONT_COLOR,
                       ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER, TEXT_ALIGN_CENTER);

    TRT_windowDrawText("Press any key to continue",
                       (Vec2) {0, MAIN_MENU_FONT_HEIGHT + 5},
                       MAIN_MENU_FONT_HEIGHT,
                       MAIN_MENU_FONT_COLOR,
                       ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_NONE, TEXT_ALIGN_CENTER);
}

static void renderTitleScreen() {
    TRT_windowDrawImage(openScreen, (Vec2) {0, 0}, TRT_getWindowSize());
}

static void renderMainMenuOptions() {
    drawHeaderLine();

    TRT_windowDrawImage(options,
                        (Vec2) {-1, TRT_getWindowSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET - 42},
                        (Vec2) {145, 42});

    TRT_windowDrawRectangle((Vec2) {-1, OPTIONS_SCREEN_IMAGE_Y_OFFSET * 2},
                            (Vec2) {175, 135},
                            MAIN_MENU_SECONDARY_COLOR);

    TRT_windowDrawText("New Game\nSound\nControl\nLoad Game\nSave Game\nChange View\nRead This\nView Scores\nQuit",
                       (Vec2) {100, 144},
                       MAIN_MENU_FONT_HEIGHT,
                       MAIN_MENU_FONT_COLOR,
                       ELEMENT_ALIGN_NONE, ELEMENT_ALIGN_NONE, TEXT_ALIGN_LEFT);

    TRT_windowDrawImage(gun,
                        (Vec2) {77, 131 - (MAIN_MENU_FONT_HEIGHT + FONT_LINE_OFFSET_MIN) * currentSelectedOption},
                        (Vec2) {19, 11});
}

void mainMenuInit() {
    openScreen = TRT_getImage(MAIN_MENU_OPEN_IMAGE);
    if (openScreen == NULL) {
        exit(EXIT_FAILURE);
    }

    options = TRT_getImage(MAIN_MENU_OPTIONS_IMAGE);
    if (options == NULL) {
        exit(EXIT_FAILURE);
    }

    gun = TRT_getImage(MAIN_MENU_GUN_IMAGE);
    if (gun == NULL) {
        exit(EXIT_FAILURE);
    }
}

#include <stdlib.h>

static bool fade = false;

void mainMenuLoop() {
    if (fade) {
        switch (TRT_windowFade(MAIN_MENU_FADE_SPEED)) {
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
}

void mainMenuClose() {
    free(openScreen->data);
    free(openScreen);
}

void mainMenuKeyboardCallback(uint32_t key) {
    switch (key) {
        case VK_ESCAPE:
            exit(0);
        case VK_UP:
            if (currentRenderer != 2)
                break;

            currentSelectedOption = (currentSelectedOption > 0) ? currentSelectedOption - 1 : 8;
            break;
        case VK_DOWN:
            if (currentRenderer != 2)
                break;

            currentSelectedOption = (currentSelectedOption + 1) % 9;
            break;
        default:
            if (currentRenderer == 2)
                break;

            fade = true;
            currentRenderer++;
            break;
    }
}

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y) {
    mainMenuKeyboardCallback('a');
}