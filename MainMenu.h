//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_MAINMENU_H
#define WOLFENSTEIN3D_MAINMENU_H

#include "TRT_Engine.h"
#include "Wolfenstein3D.h"
#include "Context.h"

#define MAIN_MENU_OPEN_IMAGE       ("assets/main_menu/open_screen.png")
#define MAIN_MENU_OPTIONS_IMAGE    ("assets/hud/options.png")
#define MAIN_MENU_GUN_IMAGE        ("assets/hud/gun.png")
#define MAIN_MENU_FONT_COLOR       (0xBCBCBC)

static Image *openScreen;
static Image *options;
static Image *gun;

static uint8_t currentSelectedOption = 0;

//
// Context functions
//
void mainMenuInit();

void mainMenuLoop();

void mainMenuClose();

void mainMenuKeyboardCallback(uint32_t key);

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y);

void mainMenuFade(uint32_t fadeSpeedMilliseconds, uint32_t *pixels);

WolfensteinContext mainMenuContext = {mainMenuInit, mainMenuLoop, mainMenuKeyboardCallback, mainMenuMouseCallback,
                                      mainMenuClose};

#define MAIN_MENU_FADE_SPEED                (500)
#define MAIN_MENU_BACKGROUND_COLOR          (0x74180A)
#define MAIN_MENU_SECONDARY_COLOR           (0x590000)
#define MAIN_MENU_FONT_HEIGHT               (10)

#define HEADER_LINE_BACKGROUND_COLOR        (0x000000)
#define ATTENTION_SCREEN_TITLE_FONT_COLOR   (0xFBF82B)
#define HEADER_LINE_BACKGROUND_Y_OFFSET     (10)
#define HEADER_LINE_BACKGROUND_HEIGHT       (22)

#define OPTIONS_SCREEN_IMAGE_Y_OFFSET       (6)

//
// Renderers
//
static void renderAttentionScreen();

static void renderTitleScreen();

static void renderMainMenuOptions();

//
// Frame management
//
static uint32_t currentRenderer = 0;
static const uint32_t renderersCount = 3;

static void (*renderers[])() = {renderAttentionScreen, renderTitleScreen, renderMainMenuOptions};

#endif //WOLFENSTEIN3D_MAINMENU_H
