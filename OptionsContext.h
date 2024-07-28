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
static int8_t currentQuitMessage = -1;
static void drawQuitMessage();

#include "GameContext.h"

static bool showChangeView = false;

#define OPTIONS_QUIT_FONT_COLOR         (0x0)
#define OPTIONS_TITLE_IMAGE             ("assets/hud/options/title.png")
#define OPTIONS_GUN_IMAGE               ("assets/hud/options/gun.png")
#define OPTIONS_QUIT_IMAGE              ("assets/hud/options/quit.png")
#define OPTIONS_BACKGROUND_IMAGE        ("assets/hud/options/background.png")
#define OPTIONS_CONTROLS_IMAGE          ("assets/hud/options/controls.png")
#define OPTIONS_SECONDARY_COLOR         (0x590000)
#define OPTIONS_SCREEN_IMAGE_Y_OFFSET   (6)

static uint8_t currentSelectedOption = 0;
static uint8_t maxSelectedOption = 8;

static Image *options;
static Image *gun;
static Image *quitBackground;
static Image *background;
static Image *controls;

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
