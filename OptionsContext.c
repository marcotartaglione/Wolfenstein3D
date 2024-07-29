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

    episode1Thumbnail = TRT_image_get(EPISODE_1_IMAGE);
    if (episode1Thumbnail == NULL) {
        exit(EXIT_FAILURE);
    }
}

bool optionsContextLoop() {
    if (TRT_animation_isFading())
        return false;

    renderers[currentRenderer]();
    return startNewGame;
}

void optionsContextClose() {

}

void optionsContextKeyboardCallback(uint32_t key) {
    keyboardCallbacks[currentRenderer](key);

    if (key == 27) { // ESC
        TRT_animation_startFade();
        currentSelectedOption = 0;
        currentRenderer = 0;
        return;
    }
}

void optionsContextMouseCallback(Click click, uint32_t x, uint32_t y) {

}

static void drawQuitMessage() {
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

static void drawOptions() {
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
}

static void drawEpisodes() {
    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_text_draw("Which episode to play?",
                  (Vec2) {ELEMENT_ALIGN_CENTER, TRT_window_getSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET},
                  FONT_HEIGHT,
                  EPISODES_TITLE_COLOR,
                  TEXT_ALIGN_CENTER);

    TRT_image_draw(episode1Thumbnail,
                   (Vec2) {ELEMENT_ALIGN_CENTER, 50},
                   (Vec2) {47, 24});

    TRT_image_draw(background,
                   (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2) {309, 163});

    TRT_image_draw(controls,
                   (Vec2) {ELEMENT_ALIGN_CENTER, 0},
                   (Vec2) {102, 7});
}

static void drawSound() {

}

static void drawControl() {

}

static void drawChangeView() {
    if (startingGameSize.x == 0) {
        startingGameSize = gameGetSize();
    }

    gameDrawFrame(startingGameSize);
}

static void drawReadThis() {

}

static void drawShowScores() {

}

static void optionsKeyboardCallback(uint32_t key) {
    if (showQuitMessage) {
        if (key == 'y' || key == 'Y') exit(EXIT_SUCCESS);
        else showQuitMessage = false;
    }

    switch (key) {
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
            if (currentSelectedOption != 8)
                TRT_animation_startFade();

            switch (currentSelectedOption) {
                case 0:
                    currentRenderer = 1;
                    break;
                case 5:
                    currentRenderer = 4;
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

static void episodesKeyboardCallback(uint32_t key) {
    switch (key) {
        case 38:
            if (currentSelectedEpisode == 0) {
                currentSelectedEpisode = maxSelectedEpisode - 1;
            } else {
                currentSelectedEpisode--;
            }
            break;
        case 40:
            if (currentSelectedEpisode == maxSelectedEpisode - 1) {
                currentSelectedEpisode = 0;
            } else {
                currentSelectedEpisode++;
            }
            break;
    }

    if (key == 13) {
        startNewGame = true;
    }
}

static void soundKeyboardCallback(uint32_t key) {

}

static void controlKeyboardCallback(uint32_t key) {

}

static void changeViewKeyboardCallback(uint32_t key) {
    switch (key) {
        case 37: // left arrow
            startingGameSize.x += GAME_SIZE_X_CHANGE_FACTOR;
            startingGameSize.y += GAME_SIZE_Y_CHANGE_FACTOR;

            if (startingGameSize.x > GAME_SIZE_MAX_X || startingGameSize.y > GAME_SIZE_MAX_Y) {
                startingGameSize.x = GAME_SIZE_MAX_X;
                startingGameSize.y = GAME_SIZE_MAX_Y;
            }
            break;
        case 39: // right arrow
            startingGameSize.x -= GAME_SIZE_X_CHANGE_FACTOR;
            startingGameSize.y -= GAME_SIZE_Y_CHANGE_FACTOR;

            if (startingGameSize.x < GAME_SIZE_MIN_X || startingGameSize.y < GAME_SIZE_MIN_Y) {
                startingGameSize.y = GAME_SIZE_MIN_Y;
                startingGameSize.x = GAME_SIZE_MIN_X;
            }
            break;
        case 13:
            gameSetSize(startingGameSize);
            startingGameSize.x = 0;

            TRT_animation_startFade();
            currentRenderer = 0;

            break;
        case 27:
            startingGameSize.x = 0;
            break;
        default:
            break;
    }
}

static void readThisKeyboardCallback(uint32_t key) {

}

static void showScoresKeyboardCallback(uint32_t key) {

}