//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_CONTEXT_H
#define WOLFENSTEIN3D_CONTEXT_H

#include "TRT_Engine.h"
#include "Commons.h"

typedef enum {
    LOOP_RESULT_IDLE,
    LOOP_RESULT_NEXT,
    LOOP_RESULT_PREVIOUS,
    LOOP_RESULT_END,
    LOOP_RESULT_SPECIED
} LoopResult;

typedef struct WolfensteinContext {
    void (*init)();
    LoopResult (*loop)();
    void (*keyboardCallback)(uint32_t);
    void (*mouseCallback)(Click, uint32_t, uint32_t);
    void (*close)();
} WolfensteinContext;

#endif //WOLFENSTEIN3D_CONTEXT_H
