//
// Created by Marco on 24/07/2024.
//

#ifndef WOLFENSTEIN3D_OPTIONS_H
#define WOLFENSTEIN3D_OPTIONS_H

#include "TRT_Engine.h"
#include "Context.h"

static bool startNewGame = false;

void optionsContextInit();
bool optionsContextLoop();
void optionsContextClose();

void optionsContextKeyboardCallback(uint32_t key);
void optionsContextMouseCallback(Click click, uint32_t x, uint32_t y);

static bool showQuitMessage = false;
static Vec2 startingGameSize = {0, 0};

static int8_t currentQuitMessage = -1;
static void drawQuitMessage();

#define EPISODE_1_IMAGE ("assets/episode/1/thumbnail.png")

#define GAME_SIZE_X_CHANGE_FACTOR (10)
#define GAME_SIZE_Y_CHANGE_FACTOR (5)

static void drawOptions();
static void drawEpisodes();
static void drawSound();
static void drawControl();
static void drawChangeView();
static void drawReadThis();
static void drawShowScores();

static uint8_t currentRenderer = 0;
static void (*renderers[])() = {
        drawOptions,
        drawEpisodes,
        drawSound,
        drawControl,
        drawChangeView,
        drawReadThis,
        drawShowScores
};

static void optionsKeyboardCallback(uint32_t key);
static void episodesKeyboardCallback(uint32_t key);
static void soundKeyboardCallback(uint32_t key);
static void controlKeyboardCallback(uint32_t key);
static void changeViewKeyboardCallback(uint32_t key);
static void readThisKeyboardCallback(uint32_t key);
static void showScoresKeyboardCallback(uint32_t key);

static void (*keyboardCallbacks[])(uint32_t key) = {
        optionsKeyboardCallback,
        episodesKeyboardCallback,
        soundKeyboardCallback,
        controlKeyboardCallback,
        changeViewKeyboardCallback,
        readThisKeyboardCallback,
        showScoresKeyboardCallback
};

#include "GameContext.h"

#define OPTIONS_QUIT_FONT_COLOR         (0x0)
#define OPTIONS_TITLE_IMAGE             ("assets/hud/options/title.png")
#define OPTIONS_GUN_IMAGE               ("assets/hud/options/gun.png")
#define OPTIONS_QUIT_IMAGE              ("assets/hud/options/quit.png")
#define OPTIONS_BACKGROUND_IMAGE        ("assets/hud/options/background.png")
#define OPTIONS_CONTROLS_IMAGE          ("assets/hud/options/controls.png")
#define OPTIONS_SCREEN_IMAGE_Y_OFFSET   (6)

#define EPISODES_TITLE_COLOR            (0xFBF82B)

static uint8_t currentSelectedOption = 0;
static uint8_t maxSelectedOption = 8;

static uint32_t currentSelectedEpisode = 0;
static uint32_t maxSelectedEpisode = 1;

static Image *options;
static Image *gun;
static Image *quitBackground;
static Image *background;
static Image *controls;

static Image *episode1Thumbnail;

#define QUIT_MESSAGE_COUNT 7
static char *quitStrings[] = {
        "Dost thou wish to leave\nwith such hasty abandon?",
        "You are at an intersection.\nA sign says, \"Press Y to quit\".",
        "Press N if you are brave.\nPress Y to cower in shame.",
        "For guns and glory, press N.\nFor work and worry, press Y.",
        "Press N to save the world.\nPress Y to abandon it in\nits hour of need.",
        "Chicking out...\nalready?",
        "Heroes, press N.\nWimps, press Y."
};

#endif //WOLFENSTEIN3D_OPTIONS_H
