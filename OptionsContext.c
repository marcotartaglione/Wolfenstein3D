//
// Created by Marco on 24/07/2024.
//

#include "OptionsContext.h"
#include "Commons.h"

void optionsContextInit() {
    options = TRT_image_get(OPTIONS_TITLE_IMAGE);
    if (options == NULL) {
        exit(EXIT_FAILURE);
    }

    gun = TRT_image_get(OPTIONS_GUN_IMAGE);
    if (gun == NULL) {
        exit(EXIT_FAILURE);
    }

    quitBackground = TRT_image_get(OPTIONS_QUIT_BACKGROUND_IMAGE);
    if (quitBackground == NULL) {
        exit(EXIT_FAILURE);
    }
}

bool optionsContextLoop() {
    drawHeaderLine();

    TRT_image_draw(options,
                   (Vec2) {-1, TRT_window_getSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET - 42},
                   (Vec2) {145, 42});

    TRT_window_DrawRectangle((Vec2) {-1, OPTIONS_SCREEN_IMAGE_Y_OFFSET * 2},
                             (Vec2) {175, 135},
                             OPTIONS_SECONDARY_COLOR);

    TRT_text_draw("New Game\nSound\nControl\nLoad Game\nSave Game\nChange View\nRead This\nView Scores\nQuit",
                  (Vec2) {110, 144},
                  FONT_HEIGHT,
                  OPTIONS_FONT_COLOR,
                  ELEMENT_ALIGN_NONE, ELEMENT_ALIGN_NONE, TEXT_ALIGN_LEFT);

    TRT_image_draw(gun,
                   (Vec2) {82, 131 - (FONT_HEIGHT + FONT_LINE_OFFSET) * currentSelectedOption},
                   (Vec2) {19, 11});

    if (showQuitMessage)
        drawQuitMessage();

    return false;
}

void optionsContextClose() {

}

void optionsContextKeyboardCallback(uint32_t key) {
    if (showQuitMessage) {
        if (key == 'y' || key == 'Y') {
            exit(1);
        } else {
            showQuitMessage = false;
        }
        return;
    }

    switch (key) {
        case 38:
            if (currentSelectedOption == 0) {
                currentSelectedOption = maxSelectedOption;
            } else {
                currentSelectedOption--;
            }
            break;
        case 40:
            if (currentSelectedOption == maxSelectedOption) {
                currentSelectedOption = 0;
            } else {
                currentSelectedOption++;
            }
            break;
        case 13:
            switch (currentSelectedOption) {
                case 8:
                    showQuitMessage = true;
                    currentQuitMessage = rand() % QUIT_MESSAGE_COUNT;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

void optionsContextMouseCallback(Click click, uint32_t x, uint32_t y) {

}

void drawQuitMessage() {
    Vec2 windowSize = TRT_window_getSize();

    TRT_image_draw(quitBackground,
                   (Vec2) {-1, -1},
                   (Vec2) {windowSize.x / 2, windowSize.y / 3});

    TRT_text_draw(quitStrings[currentQuitMessage],
                  (Vec2) {0, 0},
                  FONT_HEIGHT,
                  OPTIONS_FONT_COLOR,
                  ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER, TEXT_ALIGN_CENTER);
}