//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_CONTEXT_H
#define WOLFENSTEIN3D_CONTEXT_H

#include "TRT_Engine.h"

typedef struct WolfensteinContext {
    void (*init)();
    void (*loop)();
    void (*keyboardCallback)(uint32_t);
    void (*mouseCallback)(Click, uint32_t, uint32_t);
    void (*close)();
} WolfensteinContext;

#endif //WOLFENSTEIN3D_CONTEXT_H
