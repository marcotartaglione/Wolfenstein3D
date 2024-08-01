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

bool isEditor = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
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

uint8_t currentContext = 0;
uint8_t lastContext = 0;
uint8_t contextsCount = 0;
WolfensteinContext contexts[256];

#endif //WOLFENSTEIN3D_WOLFENSTEIN_H
