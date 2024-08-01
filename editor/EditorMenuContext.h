//
// Created by Marco on 30/07/2024.
//

#ifndef WOLFENSTEIN3D_EDITORMENUCONTEXT_H
#define WOLFENSTEIN3D_EDITORMENUCONTEXT_H

#include "TRT_Engine.h"
#include "../Context.h"
#include "../Commons.h"
#include "../Episodes.h"
#include "EditorContext.h"

#define EDITOR_TITLE_PATH ("./assets/hud/editor/title.png")

static Image* title;
static Image* controls;
static Image* quitBackground;

static LoopResult editormenu_loopResult = LOOP_RESULT_IDLE;

void editorMenuContextInit();

LoopResult editorMenuContextLoop();

void editorMenuContextClose();

void editorMenuKeyboardCallback(uint32_t key);

void editorMenuMouseCallback(Click click, uint32_t x, uint32_t y);

#define MENU_TITLE_OFFSET_TOP   (5)
#define EPISODE_OFFSET_TITLE    (50)
#define EPISODE_GAP             (10)

static int8_t currentEpisode = 0;
static int8_t currentFloor = 0;

static void drawChooseEpisode();
static void drawChooseFloor();

static bool isDrawExit = false;
static void drawExit();

static uint32_t editormenu_currentRenderer = 0;
static uint32_t editormenu_renderersCount = 1;
static void (*editormenu_renderers[])() = {drawChooseEpisode, drawChooseFloor };

static void chooseEpisodeKeyboardCallback(uint32_t key);
static void chooseFloorKeyboardCallback(uint32_t key);

static void (*editormenu_keyboardCallbacks[])(uint32_t key) = {chooseEpisodeKeyboardCallback, chooseFloorKeyboardCallback };

#endif //WOLFENSTEIN3D_EDITORMENUCONTEXT_H
