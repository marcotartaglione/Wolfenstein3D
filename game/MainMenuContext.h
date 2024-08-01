//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_MAINMENUCONTEXT_H
#define WOLFENSTEIN3D_MAINMENUCONTEXT_H

#include "TRT_Engine.h"
#include "../Context.h"

#define ATTENTION_SCREEN_TITLE_FONT_COLOR   (0xFBF82B)
#define MAIN_MENU_OPEN_IMAGE                ("./assets/hud/mainmenu/open.png")
#define MAIN_MENU_RATING_IMAGE              ("./assets/hud/mainmenu/rating.png")
#define MAIN_MENU_FONT_COLOR                (0xBCBCBC)

static Image *openScreen;
static Image *ratingScreen;

static bool close = false;

//
// Context functions
//
void mainMenuContextInit();

LoopResult mainMenuContextLoop();

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
static void renderAttentionScreen();

static void renderRatingScreen();

static void renderTitleScreen();

//
// Frame management
//
static uint32_t editormenu_currentRenderer = 0;
static uint32_t editormenu_renderersCount = 3;
static void (*editormenu_renderers[])() = {renderAttentionScreen, renderRatingScreen, renderTitleScreen,};

#endif //WOLFENSTEIN3D_MAINMENUCONTEXT_H
