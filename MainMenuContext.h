//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_MAINMENUCONTEXT_H
#define WOLFENSTEIN3D_MAINMENUCONTEXT_H

#include "TRT_Engine.h"
#include "Context.h"

#define MAIN_MENU_FADE_SPEED                (500)
#define ATTENTION_SCREEN_TITLE_FONT_COLOR   (0xFBF82B)
#define MAIN_MENU_OPEN_IMAGE       ("assets/main_menu/open_screen.png")
#define MAIN_MENU_FONT_COLOR       (0xBCBCBC)

static Image *openScreen;
static bool close = false;

//
// Context functions
//
void mainMenuContextInit();

bool mainMenuContextLoop();

void mainMenuContextClose();

void mainMenuKeyboardCallback(uint32_t key);

void mainMenuMouseCallback(Click click, uint32_t x, uint32_t y);

WolfensteinContext mainMenuContext = {
        mainMenuContextInit,
        mainMenuContextLoop,
        mainMenuKeyboardCallback,
        mainMenuMouseCallback,
        mainMenuContextClose};

//
// Renderers
//
static void renderTest();

static void renderAttentionScreen();

static void renderTitleScreen();

//
// Frame management
//
static uint32_t currentRenderer = 0;
static uint32_t renderersCount = 3;
static void (*renderers[])() = {renderTest, renderAttentionScreen, renderTitleScreen,};

#endif //WOLFENSTEIN3D_MAINMENUCONTEXT_H
