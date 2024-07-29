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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
void loop();
void close();

void activateCurrentContext();
void deactivateCurrentContext();

//
// Episodes
//

void loadEpisodes();

Episode *episodes[20];
uint8_t currentEpisode = 0;

//
// Game contexts
//
extern WolfensteinContext mainMenuContext;
extern WolfensteinContext optionsContext;
extern WolfensteinContext gameContext;

uint8_t currentContext = 0;
WolfensteinContext contexts[256];

#endif //WOLFENSTEIN3D_WOLFENSTEIN_H
