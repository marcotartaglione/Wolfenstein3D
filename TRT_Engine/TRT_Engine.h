//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_TRT_ENGINE_H
#define WOLFENSTEIN3D_TRT_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>
#include "vendor/stb/stb_image.h"

typedef struct Image {
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

void TRT_message(char *text);

//
// Window management
//
void TRT_setupWindow(HINSTANCE hInstance, char *className);

void TRT_interpretateSize(Vec2 *size, bool considerUpScaling);

void TRT_interpretatePosition(Vec2 *position, Vec2 size);

void TRT_startWindow(char *title, Vec2 size, Vec2 position);

void TRT_runWindow(uint8_t targetFPS, void (*loop)(), void (*close)());

void TRT_clearFrame();

void TRT_setWindowUpScaling(uint32_t upScaling);

void TRT_setWindowPixel(uint32_t x, uint32_t y, uint32_t color);


typedef enum {
    TEXT_ALIGN_NONE,
    TEXT_ALIGN_START,
    TEXT_ALIGN_END,
    TEXT_ALIGN_CENTER
} TextAlignment;

void TRT_windowDrawText(char *text, Vec2 position, uint32_t height, uint32_t color, TextAlignment horizontalAlignment,
                        TextAlignment verticalAlignment);

void TRT_setFontBackgroundColor(uint32_t color);

void TRT_windowDrawImage(Image *image, Vec2 position, Vec2 size);

typedef enum FADE {
    FADE_IN,
    FADE_OUT,
    FADE_OVER
} Fade;

Fade TRT_windowFade(uint32_t fadeSpeedMilliseconds);

uint32_t TRT_getWindowPixel(uint32_t x, uint32_t y);

Vec2 TRT_getWindowSize();

long long TRT_getTime();

//
// Input management
//
typedef enum Click {
    CLICK_LEFT,
    CLICK_RIGHT
} Click;

void TRT_setKeyCallback(void (*keyCallbackFunction)(uint32_t));

void TRT_setMouseCallback(void (*mouseCallbackFunction)(Click, uint32_t, uint32_t));

//
// Image management
//
Image *TRT_getImage(char *path);

//
// Font management
//
#define MAX_SYMBOLS 256

void TRT_loadSymbols(char *directory);

#define FONT_SPACE_MIN_WIDTH    (10)
#define FONT_LINE_OFFSET_MIN    (3)
#define FONT_LETTER_SPACING     (1)

#define FONT_MAX_LINES          (10)



//
// Math
//
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
