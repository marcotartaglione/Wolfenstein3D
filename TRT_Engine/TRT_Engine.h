//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_TRT_ENGINE_H
#define WOLFENSTEIN3D_TRT_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>
#include "vendor/stb/stb_image.h"

typedef struct Image{
    int32_t width, height;
    int32_t channels;
    uint8_t *data;
} Image;

typedef struct Vec2 {
    int32_t x, y;
} Vec2;

typedef struct Vec3 {
    struct Vec2;
    int32_t z;
} Vec3;

//
// Window management
//
void setupWindow(HINSTANCE hInstance, char* className);
void startWindow(char* title, Vec2 size, Vec2 position);
void runWindow(void (*loop)(), void (*close)());
void clearFrame();

void setWindowUpScaling(uint32_t upScaling);
void setWindowPixel(uint32_t x, uint32_t y, uint32_t color);
Vec2 getWindowSize();

//
// Image management
//
Image* getImage(char* path);

//
// Font management
//
#define MAX_SYMBOLS 256
void loadSymbols(char* directory);

#define FONT_SPACE_MIN_WIDTH    (10)
#define FONT_LINE_OFFSET_MIN    (3)
#define FONT_LETTER_SPACING     (1)

#define FONT_MAX_LINES          (10)

typedef enum {
    TEXT_ALIGN_NONE,
    TEXT_ALIGN_START,
    TEXT_ALIGN_END,
    TEXT_ALIGN_CENTER
} TextAlignment;

void drawText(char* text, Vec2 position, uint32_t height, uint32_t color, TextAlignment horizontalAlignment, TextAlignment verticalAlignment);

void setFontBackgroundColor(uint32_t color);

//
// Math
//
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x < y ? x : y)

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
