//
// Created by Marco on 31/07/2024.
//

#ifndef WOLFENSTEIN3D_EDITORCONTEXT_H
#define WOLFENSTEIN3D_EDITORCONTEXT_H

#include "TRT_Engine.h"
#include "../Context.h"
#include "../Commons.h"
#include "../Episodes.h"

static int8_t selectedEpisode = 0;
static int8_t selectedFloor = 0;

static int32_t selectedTile = 0;

#define EDITOR_MAX_TILE_SIZE    (64)
#define EDITOR_MIN_TILE_SIZE    (12)
#define EDITOR_TILE_SIZE_STEP   (4)

static uint16_t tileSize = EDITOR_MIN_TILE_SIZE;

static WallTexture selectedWall = WALL_STONE1;
static WallTexture shownWall = WALL_STONE1;

static LoopResult editor_loopResult = LOOP_RESULT_IDLE;

static uint32_t currentRow = 0;
static uint32_t currentCol = 0;
static bool modifingRow = true;

void editorContextInit();

LoopResult editorContextLoop();

void editorContextClose();

void editorKeyboardCallback(uint32_t key);

void editorMouseCallback(Click click, uint32_t x, uint32_t y);

void Editor_setEpisodeAndFloor(int8_t episode, int8_t floor);

static void increaseTileSize();

static void decreaseTileSize();

static bool drawExitMessage = false;
static bool drawNavigationMenu = false;

static void drawExit();

static void drawNavigation();

void drawGrid();

void drawTileSelector();

static uint32_t editor_currentRenderer = 0;

static void (*editor_renderers[])() = {drawGrid, drawTileSelector};

void gridKeyboardCallback(uint32_t key);

void tileSelectorKeyboardCallback(uint32_t key);

static void (*editor_keyboardCallbacks[])(uint32_t key) = {gridKeyboardCallback, tileSelectorKeyboardCallback};

#endif //WOLFENSTEIN3D_EDITORCONTEXT_H
