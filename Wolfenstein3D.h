//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_WOLFENSTEIN_H
#define WOLFENSTEIN3D_WOLFENSTEIN_H

#include "TRT_Engine/TRT_Engine.h"
#include "Context.h"
#include "Commons.h"

#define GAME_TARGET_FPS (30)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
void loop();
void close();

//
// Game contexts
//
extern WolfensteinContext mainMenuContext;

#endif //WOLFENSTEIN3D_WOLFENSTEIN_H
