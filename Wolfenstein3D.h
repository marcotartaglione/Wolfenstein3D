//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_WOLFENSTEIN_H
#define WOLFENSTEIN3D_WOLFENSTEIN_H

#include "TRT_Engine/TRT_Engine.h"
#include "Context.h"
#include "Commons.h"
#include "Episodes.h"

#define GAME_TARGET_FPS (30)

static bool isEditor = false;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow);
void loop();
void close();

bool deactivateCurrentContext();
bool activateCurrentContext();

//
// Episodes
//
void loadEpisodes();

//
// Wall
//
void loadWallTextures();

//
// Game contexts
//
extern WolfensteinContext mainMenuContext;
extern WolfensteinContext optionsContext;
extern WolfensteinContext gameContext;

extern WolfensteinContext editorMenuContext;
extern WolfensteinContext editorContext;

static uint8_t currentContext = 0;
static uint8_t lastContext = 0;
static uint8_t contextsCount = 0;
WolfensteinContext contexts[256];

uint8_t optionsContextIndex;

#endif //WOLFENSTEIN3D_WOLFENSTEIN_H
