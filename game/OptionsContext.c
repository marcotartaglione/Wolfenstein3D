//
// Created by Marco on 24/07/2024.
//

#include "OptionsContext.h"
#include "../Commons.h"

WolfensteinContext optionsContext = {
    optionsContextInit,
    optionsContextLoop,
    optionsContextKeyboardCallback,
    optionsContextMouseCallback,
    optionsContextClose
};

void optionsContextInit() {
    OptionsContext_LoopResult = LOOP_RESULT_IDLE;
    OptionsContext_ShowQuitMessage = false;
    OptionsContext_CurrentRenderer = 0;
    OptionsContext_CurrentSelectedOption = 0;
    OptionsContext_CurrentSelectedEpisode = 0;
    OptionsContext_CurrentSelectedDifficulty = 0;

    OptionsContext_Options = TRT_image_get(OPTIONS_TITLE_IMAGE);
    if (OptionsContext_Options == NULL) {
        exit(EXIT_FAILURE);
    }

    OptionsContext_Gun = TRT_image_get(OPTIONS_GUN_IMAGE);
    if (OptionsContext_Gun == NULL) {
        exit(EXIT_FAILURE);
    }

    OptionsContext_QuitBackground = TRT_image_get(QUIT_IMAGE);
    if (OptionsContext_QuitBackground == NULL) {
        exit(EXIT_FAILURE);
    }

    OptionsContext_Controls = TRT_image_get(CONTROLS_IMAGE);
    if (OptionsContext_Controls == NULL) {
        exit(EXIT_FAILURE);
    }

    OptionsContext_Background = TRT_image_get(OPTIONS_BACKGROUND_IMAGE);
    if (OptionsContext_Background == NULL) {
        exit(EXIT_FAILURE);
    }

    for (uint8_t i = 0; i < DIFFICULTY_COUNT; ++i) {
        char path[128];
        snprintf(path, sizeof(path), "%s%d.png", DIFFICULTY_IMAGE_FOLDER, i + 1);
        path[strlen(DIFFICULTY_IMAGE_FOLDER) + 5] = '\0';

        OptionsContext_DifficultiesImages[i] = TRT_image_get(path);
    }
}

LoopResult optionsContextLoop() {
    if (TRT_animation_isFading())
        return LOOP_RESULT_IDLE;

    OptionsContext_Renderers[OptionsContext_CurrentRenderer]();
    return OptionsContext_LoopResult;
}

void optionsContextClose() {
    TRT_image_free(OptionsContext_Options);
    TRT_image_free(OptionsContext_Gun);
    TRT_image_free(OptionsContext_QuitBackground);
    TRT_image_free(OptionsContext_Background);
    TRT_image_free(OptionsContext_Controls);

    for (uint8_t i = 0; i < DIFFICULTY_COUNT; ++i) {
        TRT_image_free(OptionsContext_DifficultiesImages[i]);
    }
}

void optionsContextKeyboardCallback(uint32_t key) {
    OptionsContext_KeyboardCallbacks[OptionsContext_CurrentRenderer](key);

    if (key == 27) {
        // ESC
        TRT_animation_startFade();
        OptionsContext_CurrentSelectedOption = 0;
        OptionsContext_CurrentRenderer = 0;
    }
}

void optionsContextMouseCallback(Click click, uint32_t x, uint32_t y) {
}

static void drawQuitMessage() {
    uint32_t nLines;
    Vec2 textSize = TRT_text_size(OptionsContext_QuitStrings[OptionsContext_CurrentQuitMessage],
                                  &nLines,
                                  FONT_HEIGHT,
                                  FONT_SPACE_WIDTH,
                                  FONT_LETTER_SPACING,
                                  FONT_LINE_OFFSET)[0];

    TRT_image_draw(OptionsContext_QuitBackground,
                   (Vec2){ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2){textSize.x + 20, textSize.y + 20});

    TRT_text_draw(OptionsContext_QuitStrings[OptionsContext_CurrentQuitMessage],
                  (Vec2){ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  QUIT_FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}

static void drawOptions() {
    drawHeaderLine();

    TRT_image_draw(OptionsContext_Options,
                   (Vec2){-1, TRT_window_getSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET - 42},
                   (Vec2){145, 42});

    TRT_image_draw(OptionsContext_Background,
                   (Vec2){ELEMENT_ALIGN_CENTER, OPTIONS_SCREEN_IMAGE_Y_OFFSET * 2},
                   (Vec2){175, 135});

    TRT_image_draw(OptionsContext_Controls,
                   (Vec2){ELEMENT_ALIGN_CENTER, 0},
                   (Vec2){102, 7});

    TRT_text_draw("New Game\nSound\nControl\nLoad Game\nSave Game\nChange View\nRead This\nView Scores\nQuit",
                  (Vec2){ELEMENT_ALIGN_CENTER, 135},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_LEFT);

    TRT_image_draw(OptionsContext_Gun,
                   (Vec2){82, 125 - (FONT_HEIGHT + FONT_LINE_OFFSET) * OptionsContext_CurrentSelectedOption},
                   (Vec2){19, 11});

    if (OptionsContext_ShowQuitMessage)
        drawQuitMessage();
}

static void drawEpisodes() {
    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_text_draw("Which episode to play?",
                  (Vec2){ELEMENT_ALIGN_CENTER, TRT_window_getSize().y - OPTIONS_SCREEN_IMAGE_Y_OFFSET},
                  FONT_HEIGHT,
                  EPISODES_TITLE_COLOR,
                  TEXT_ALIGN_CENTER);

    TRT_image_draw(OptionsContext_Background,
                   (Vec2){ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2){309, 163});

    for (uint8_t i = 0; i < EPISODES_COUNT; i++) {
        TRT_image_draw(episodes[i]->thumbnail,
                       (Vec2){
                           EPISODES_CONTENT_OFFSET_LEFT,
                           TRT_window_getSize().y - EPISODES_CONTENT_OFFSET_TOP * (i + 1) - EPISODES_THUMBNAIL_SIZE.y -
                           i * EPISODES_CONTENT_GAP
                       },
                       EPISODES_THUMBNAIL_SIZE);

        char episodeText[128];
        snprintf(episodeText, 128, "Episode %d\n%s", i + 1, episodes[i]->title);

        TRT_text_draw(episodeText,
                      (Vec2){
                          98, TRT_window_getSize().y - EPISODES_CONTENT_OFFSET_TOP * (i + 1) - i * EPISODES_CONTENT_GAP
                      },
                      FONT_HEIGHT,
                      FONT_COLOR,
                      TEXT_ALIGN_LEFT);
    }

    TRT_image_draw(OptionsContext_Gun,
                   (Vec2){11, TRT_window_getSize().y - 23 * (OptionsContext_CurrentSelectedEpisode + 1) - 12},
                   (Vec2){19, 11});

    TRT_image_draw(OptionsContext_Controls,
                   (Vec2){ELEMENT_ALIGN_CENTER, 0},
                   (Vec2){102, 7});
}

static void drawSound() {
}

static void drawControl() {
}

static void drawChangeView() {
    if (OptionsContext_StartingGameSize.x == 0) {
        OptionsContext_StartingGameSize = Game_getSize();
    }

    Game_drawFrame(OptionsContext_StartingGameSize);

    TRT_text_draw("Use arrows to size\nENTER to accept\nESC to cancel",
                  (Vec2){ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_START},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}

static void drawReadThis() {
}

static void drawShowScores() {
}

static void drawDifficiculty() {
    Vec2 winSize = TRT_window_getSize();

    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_text_draw("How tough are you?",
                  (Vec2){ELEMENT_ALIGN_CENTER, winSize.y - DIFFICULTY_TITLE_OFFSET_TOP},
                  FONT_HEIGHT,
                  DIFFICULTY_TITLE_COLOR,
                  TEXT_ALIGN_CENTER);

    TRT_image_draw(OptionsContext_Background,
                   (Vec2){ELEMENT_ALIGN_CENTER, winSize.y - DIFFICULTY_BACKGROUND_OFFSET_TOP},
                   (Vec2){226, 68});

    TRT_text_draw("Can I play, Daddy?\nDon't hurt me.\nBring 'em on!\nI am Death incarnate!",
                  (Vec2){79, winSize.y - DIFFICULTY_CONTENT_OFFSET_TOP},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_LEFT);

    TRT_image_draw(OptionsContext_Gun,
                   (Vec2){
                       56,
                       winSize.y - DIFFICULTY_CONTENT_OFFSET_TOP - 11 - OptionsContext_CurrentSelectedDifficulty * 13
                   },
                   (Vec2){19, 11});

    TRT_image_draw(OptionsContext_DifficultiesImages[OptionsContext_CurrentSelectedDifficulty],
                   (Vec2){winSize.x - DIFFICULTY_IMAGE_OFFSET_RIGHT, winSize.y - DIFFICULTY_IMAGE_OFFSET_TOP},
                   (Vec2){23, 31});

    TRT_image_draw(OptionsContext_Controls,
                   (Vec2){ELEMENT_ALIGN_CENTER, 0},
                   (Vec2){102, 7});
}

static void optionsKeyboardCallback(uint32_t key) {
    if (OptionsContext_ShowQuitMessage) {
        if (key == 'y' || key == 'Y') exit(EXIT_SUCCESS);
        else OptionsContext_ShowQuitMessage = false;
    }

    switch (key) {
        case VK_ESCAPE:
            OptionsContext_LoopResult = LOOP_RESULT_PREVIOUS;
            break;
        case VK_UP:
            if (OptionsContext_CurrentSelectedOption == 0) {
                OptionsContext_CurrentSelectedOption = OptionsContext_MaxSelectedOption;
            }
            else {
                OptionsContext_CurrentSelectedOption--;
            }
            break;
        case VK_DOWN:
            if (OptionsContext_CurrentSelectedOption == OptionsContext_MaxSelectedOption) {
                OptionsContext_CurrentSelectedOption = 0;
            }
            else {
                OptionsContext_CurrentSelectedOption++;
            }
            break;
        case VK_RETURN:
            if (OptionsContext_CurrentSelectedOption != 8)
                TRT_animation_startFade();

            switch (OptionsContext_CurrentSelectedOption) {
                case 0:
                    OptionsContext_CurrentRenderer = 1;
                    break;
                case 5:
                    OptionsContext_CurrentRenderer = 4;
                    break;
                case 8:
                    OptionsContext_ShowQuitMessage = true;
                    OptionsContext_CurrentQuitMessage = rand() % QUIT_MESSAGE_COUNT;
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
        case VK_UP:
            if (OptionsContext_CurrentSelectedEpisode == 0) {
                OptionsContext_CurrentSelectedEpisode = EPISODES_COUNT - 1;
            }
            else {
                OptionsContext_CurrentSelectedEpisode--;
            }
            break;
        case VK_DOWN:
            if (OptionsContext_CurrentSelectedEpisode == EPISODES_COUNT - 1) {
                OptionsContext_CurrentSelectedEpisode = 0;
            }
            else {
                OptionsContext_CurrentSelectedEpisode++;
            }
            break;
        default:
            break;
    }

    if (key == 13) {
        TRT_animation_startFade();
        Game_setEpisode(OptionsContext_CurrentSelectedEpisode);
        OptionsContext_CurrentRenderer = 7;
    }
}

static void soundKeyboardCallback(uint32_t key) {
}

static void controlKeyboardCallback(uint32_t key) {
}

static void changeViewKeyboardCallback(uint32_t key) {
    switch (key) {
        case 37: // left arrow
            OptionsContext_StartingGameSize.x += GAME_SIZE_X_CHANGE_FACTOR;
            OptionsContext_StartingGameSize.y += GAME_SIZE_Y_CHANGE_FACTOR;

            if (OptionsContext_StartingGameSize.x > GAME_SIZE_MAX_X || OptionsContext_StartingGameSize.y >
                GAME_SIZE_MAX_Y) {
                OptionsContext_StartingGameSize.x = GAME_SIZE_MAX_X;
                OptionsContext_StartingGameSize.y = GAME_SIZE_MAX_Y;
            }
            break;
        case 39: // right arrow
            OptionsContext_StartingGameSize.x -= GAME_SIZE_X_CHANGE_FACTOR;
            OptionsContext_StartingGameSize.y -= GAME_SIZE_Y_CHANGE_FACTOR;

            if (OptionsContext_StartingGameSize.x < GAME_SIZE_MIN_X || OptionsContext_StartingGameSize.y <
                GAME_SIZE_MIN_Y) {
                OptionsContext_StartingGameSize.y = GAME_SIZE_MIN_Y;
                OptionsContext_StartingGameSize.x = GAME_SIZE_MIN_X;
            }
            break;
        case 13:
            Game_setSize(OptionsContext_StartingGameSize);
            OptionsContext_StartingGameSize.x = 0;

            TRT_animation_startFade();
            OptionsContext_CurrentRenderer = 0;

            break;
        case 27:
            OptionsContext_StartingGameSize.x = 0;
            break;
        default:
            break;
    }
}

static void readThisKeyboardCallback(uint32_t key) {
}

static void showScoresKeyboardCallback(uint32_t key) {
}

static void difficultyKeyboardCallback(uint32_t key) {
    switch (key) {
        case VK_UP:
            if (OptionsContext_CurrentSelectedDifficulty == 0) {
                OptionsContext_CurrentSelectedDifficulty = DIFFICULTY_COUNT - 1;
            }
            else {
                OptionsContext_CurrentSelectedDifficulty--;
            }
            break;
        case VK_DOWN:
            if (OptionsContext_CurrentSelectedDifficulty == DIFFICULTY_COUNT - 1) {
                OptionsContext_CurrentSelectedDifficulty = 0;
            }
            else {
                OptionsContext_CurrentSelectedDifficulty++;
            }
            break;
        case VK_RETURN:
            TRT_animation_startFade();
            Game_setDifficulty(OptionsContext_CurrentSelectedDifficulty);
            OptionsContext_LoopResult = LOOP_RESULT_NEXT;
            break;
        default:
            break;
    }
}
