//
// Created by Marco on 24/07/2024.
//

#include "OptionsContext.h"
#include "Commons.h"

WolfensteinContext optionsContext = {
        optionsContextInit,
        optionsContextLoop,
        optionsContextKeyboardCallback,
        optionsContextMouseCallback,
        optionsContextClose};

void optionsContextInit() {
    options = TRT_image_get(OPTIONS_TITLE_IMAGE);
    if (options == NULL) {
        exit(EXIT_FAILURE);
    }

    gun = TRT_image_get(OPTIONS_GUN_IMAGE);
    if (gun == NULL) {
        exit(EXIT_FAILURE);
    }

    quitBackground = TRT_image_get(OPTIONS_QUIT_IMAGE);
    if (quitBackground == NULL) {
        exit(EXIT_FAILURE);
    }

    controls = TRT_image_get(OPTIONS_CONTROLS_IMAGE);
    if (controls == NULL) {
        exit(EXIT_FAILURE);
    }

    background = TRT_image_get(OPTIONS_BACKGROUND_IMAGE);
    if (background == NULL) {
        exit(EXIT_FAILURE);
    }
}

bool optionsContextLoop() {
    if (TRT_animation_isFading())
        return false;

    if (showChangeView) {
        gameDrawFrame();
        return false;
    }

    drawHeaderLine();

    TRT_image_draw(options,
                   (Vec2) {-1, TRT_window_getSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET - 42},
                   (Vec2) {145, 42});

    TRT_image_draw(background,
                   (Vec2) {ELEMENT_ALIGN_CENTER, OPTIONS_SCREEN_IMAGE_Y_OFFSET * 2},
                   (Vec2) {175, 135});

    TRT_image_draw(controls,
                   (Vec2) {ELEMENT_ALIGN_CENTER, 0},
                   (Vec2) {102, 7});

    TRT_text_draw("New Game\nSound\nControl\nLoad Game\nSave Game\nChange View\nRead This\nView Scores\nQuit",
                  (Vec2) {ELEMENT_ALIGN_CENTER, 135},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_LEFT);

    TRT_image_draw(gun,
                   (Vec2) {82, 125 - (FONT_HEIGHT + FONT_LINE_OFFSET) * currentSelectedOption},
                   (Vec2) {19, 11});

    if (showQuitMessage)
        drawQuitMessage();

    return startNewGame;
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
        case 37: // left arrow
            if (!showChangeView)
                break;
            gameIncreaseSize();
            break;
        case 39: // right arrow
            if (!showChangeView)
                break;
            gameReduceSize();
            break;
        case 38: // up arrow
            if (currentSelectedOption == 0) {
                currentSelectedOption = maxSelectedOption;
            } else {
                currentSelectedOption--;
            }
            break;
        case 40: // down arrow
            if (currentSelectedOption == maxSelectedOption) {
                currentSelectedOption = 0;
            } else {
                currentSelectedOption++;
            }
            break;
        case 13:
            switch (currentSelectedOption) {
                case 0:
                    startNewGame = true;
                    break;
                case 5:
                    showChangeView = true;
                    break;
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
    uint32_t nLines;
    Vec2 textSize = TRT_text_size(quitStrings[currentQuitMessage], &nLines, FONT_HEIGHT, FONT_SPACE_WIDTH,
                                  FONT_LETTER_SPACING, FONT_LINE_OFFSET)[0];

    TRT_image_draw(quitBackground,
                   (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2) {textSize.x + 20, textSize.y + 20});

    TRT_text_draw(quitStrings[currentQuitMessage],
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  OPTIONS_QUIT_FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}