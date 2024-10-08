//
// Created by Marco on 24/07/2024.
//

#ifndef WOLFENSTEIN3D_OPTIONS_H
#define WOLFENSTEIN3D_OPTIONS_H

#include "TRT_Engine.h"
#include "../Context.h"

static LoopResult OptionsContext_LoopResult = LOOP_RESULT_IDLE;
static Vec2 OptionsContext_StartingGameSize = {0, 0};

void optionsContextInit();
LoopResult optionsContextLoop();
void optionsContextClose();

void optionsContextKeyboardCallback(uint32_t key);
void optionsContextMouseCallback(Click click, uint32_t x, uint32_t y);

static bool OptionsContext_ShowQuitMessage;
static int8_t OptionsContext_CurrentQuitMessage;
static void drawQuitMessage();

#define GAME_SIZE_X_CHANGE_FACTOR (10)
#define GAME_SIZE_Y_CHANGE_FACTOR (5)

static void drawOptions();
static void drawEpisodes();
static void drawSound();
static void drawControl();
static void drawChangeView();
static void drawReadThis();
static void drawShowScores();
static void drawDifficiculty();

static uint8_t OptionsContext_CurrentRenderer = 0;
static void (*OptionsContext_Renderers[])() = {
        drawOptions,
        drawEpisodes,
        drawSound,
        drawControl,
        drawChangeView,
        drawReadThis,
        drawShowScores,
        drawDifficiculty
};

static void optionsKeyboardCallback(uint32_t key);
static void episodesKeyboardCallback(uint32_t key);
static void soundKeyboardCallback(uint32_t key);
static void controlKeyboardCallback(uint32_t key);
static void changeViewKeyboardCallback(uint32_t key);
static void readThisKeyboardCallback(uint32_t key);
static void showScoresKeyboardCallback(uint32_t key);
static void difficultyKeyboardCallback(uint32_t key);

static void (*OptionsContext_KeyboardCallbacks[])(uint32_t key) = {
        optionsKeyboardCallback,
        episodesKeyboardCallback,
        soundKeyboardCallback,
        controlKeyboardCallback,
        changeViewKeyboardCallback,
        readThisKeyboardCallback,
        showScoresKeyboardCallback,
        difficultyKeyboardCallback
};

#include "GameContext.h"

#define OPTIONS_TITLE_IMAGE             ("./assets/hud/options/title.png")
#define OPTIONS_GUN_IMAGE               ("./assets/hud/options/gun.png")
#define OPTIONS_BACKGROUND_IMAGE        ("./assets/hud/options/background.png")
#define OPTIONS_SCREEN_IMAGE_Y_OFFSET   (6)

#define EPISODES_TITLE_COLOR            (0xFBF82B)

static uint8_t OptionsContext_CurrentSelectedOption = 0;
static uint8_t OptionsContext_MaxSelectedOption = 8;

#define EPISODES_CONTENT_OFFSET_TOP     (23)
#define EPISODES_CONTENT_OFFSET_LEFT    (40)
#define EPISODES_CONTENT_GAP            (2)
#define EPISODES_THUMBNAIL_SIZE         (Vec2) {48, 24}

static uint32_t OptionsContext_CurrentSelectedEpisode = 0;

#define DIFFICULTY_TITLE_COLOR              (0xFBF82B)
#define DIFFICULTY_TITLE_OFFSET_TOP         (68)
#define DIFFICULTY_BACKGROUND_OFFSET_TOP    (DIFFICULTY_TITLE_OFFSET_TOP + 79 + 11)
#define DIFFICULTY_CONTENT_OFFSET_TOP       (101)
#define DIFFICULTY_IMAGE_FOLDER             ("./assets/hud/difficulty/")
#define DIFFICULTY_IMAGE_OFFSET_RIGHT       (88)
#define DIFFICULTY_IMAGE_OFFSET_TOP         (DIFFICULTY_BACKGROUND_OFFSET_TOP - 20)

static uint8_t OptionsContext_CurrentSelectedDifficulty = 0;

static Image *OptionsContext_Options;
static Image *OptionsContext_Gun;
static Image *OptionsContext_QuitBackground;
static Image *OptionsContext_Background;
static Image *OptionsContext_Controls;

static Image *OptionsContext_DifficultiesImages[DIFFICULTY_COUNT];

#define QUIT_MESSAGE_COUNT 7
static char *OptionsContext_QuitStrings[] = {
        "Dost thou wish to leave\nwith such hasty abandon?",
        "You are at an intersection.\nA sign says, \"Press Y to quit\".",
        "Press N if you are brave.\nPress Y to cower in shame.",
        "For guns and glory, press N.\nFor work and worry, press Y.",
        "Press N to save the world.\nPress Y to abandon it in\nits hour of need.",
        "Chicking out...\nalready?",
        "Heroes, press N.\nWimps, press Y."
};

#endif //WOLFENSTEIN3D_OPTIONS_H
