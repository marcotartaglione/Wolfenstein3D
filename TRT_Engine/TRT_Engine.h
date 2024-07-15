//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_TRT_ENGINE_H
#define WOLFENSTEIN3D_TRT_ENGINE_H

#include <stdint.h>
#include <Windows.h>

struct Frame {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
} frame;

typedef struct Vec2 {
    int32_t x, y;
} Vec2;

typedef struct Vec3 {
    struct Vec2;
    int32_t z;
} Vec3;

extern void loop();

void setupWindow(HINSTANCE hInstance, char* className);
void startWindow(char* title, Vec2 size, Vec2 position);
void clearFrame();

#define ABS(x) ((x) < 0 ? -(x) : (x))

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
