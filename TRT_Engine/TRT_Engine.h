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
void TRT_window_setup(HINSTANCE hInstance, char *className);
void TRT_window_start(char *title, Vec2 size, Vec2 position);
void TRT_window_run(uint8_t targetFPS, void (*loop)(), void (*close)());
void TRT_window_clear();

//
// toScreen = true: reference to monitor size
// toScreen = false: reference to window size
//
void TRT_window_interpretatePosition(Vec2 *position, Vec2 size, bool toScreen);
void TRT_window_interpretateSize(Vec2 *size, bool considerUpScaling);

void TRT_window_setPixel(uint32_t x, uint32_t y, uint32_t color);
uint32_t TRT_window_getPixel(uint32_t x, uint32_t y);

Vec2 TRT_window_getSize();

void TRT_window_setUpscaling(uint32_t upScaling);
void TRT_window_fill(uint32_t color);
void TRT_window_DrawRectangle(Vec2 position, Vec2 size, uint32_t color);

//
// Animation management
//
typedef enum FADE {
    FADE_IN,
    FADE_OUT,
    FADE_OVER
} Fade;

Fade TRT_animation_fade(uint32_t fadeSpeedMilliseconds);

//
// Time management
//
long long TRT_time_get();

//
// Input management
//
typedef enum Click {
    CLICK_LEFT,
    CLICK_RIGHT
} Click;

void TRT_input_setKeyCallback(void (*keyCallbackFunction)(uint32_t));
void TRT_input_setMouseCallback(void (*mouseCallbackFunction)(Click, uint32_t, uint32_t));

//
// Image management
//
Image *TRT_image_get(char *path);

void TRT_image_draw(Image *image, Vec2 position, Vec2 size);

//
// Font management
//
#define FONT_MAX_SYMBOLS    (256)
#define FONT_MAX_LINES      (10)

static uint32_t spaceWidth = 0;
static uint32_t lineOffset = 0;
static uint32_t letterSpacing = 0;

void TRT_text_setSpaceWidth(uint32_t width);
void TRT_text_setLineOffset(uint32_t offset);
void TRT_text_setLetterSpacing(uint32_t spacing);
void TRT_text_setBackgroundColor(uint32_t color);

void TRT_text_loadFont(char *directory);

typedef struct LineData {
    uint32_t width;
    uint32_t height;
    uint32_t nLetters;
    uint32_t nSpaces;
    float lineFontHeightRatio;
} LineData;

//
// First element of the array is the total size, the rest are the sizes of each line in order
//
LineData *TRT_text_size(char *text, uint32_t *nLines, uint32_t textHeight);

typedef enum {
    ELEMENT_ALIGN_NONE,
    ELEMENT_ALIGN_START,
    ELEMENT_ALIGN_END,
    ELEMENT_ALIGN_CENTER
} ElementAlignment;

typedef enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER
} TextAlignment;

void TRT_text_draw(char *text, Vec2 position, uint32_t height, uint32_t color, ElementAlignment horizontalAlignment, ElementAlignment verticalAlignment, TextAlignment textAlignment);

//
// Math
//
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
