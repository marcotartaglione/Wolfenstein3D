//
// Created by Marco on 15/07/2024.
//

#ifndef WOLFENSTEIN3D_TRT_ENGINE_H
#define WOLFENSTEIN3D_TRT_ENGINE_H

#include <stdint.h>
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

typedef struct Vec2f {
    float x, y;
} Vec2f;

typedef enum {
    false = 0,
    true = 1
} bool;

void TRT_message(char *text);

static FILE *errorLogFile;
static bool isDebug = false;

void TRT_error(char *title, char *text, bool close);
void TRT_error_setLogFile(char *path);

void TRT_debug_set(bool debug);

//
// Window management
//
void TRT_window_setup(HINSTANCE hInstance, char *className);
void TRT_window_start(char *title, Vec2 size, Vec2 position);

static uint32_t windowTargetFps;

void TRT_window_run(uint8_t targetFPS, void (*loop)(), void (*close)());
void TRT_window_close();

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
void TRT_window_DrawRectangle(Vec2 position, Vec2 size, uint32_t color, bool fill);

//
// Animation management
//
typedef enum FADE {
    FADE_IN,
    FADE_OUT,
    FADE_OVER
} fade;

static bool isFading = false;
static fade currentFade = FADE_IN;

static int32_t fadeValue = 0;
static uint32_t fadeTime = 1000;

fade TRT_animation_fade(uint32_t fadeSpeedMilliseconds);
void TRT_animation_setFadeTime(uint32_t time);
bool TRT_animation_startFade();
bool TRT_animation_isFading();

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
void TRT_image_free(Image *image);

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

//
// First element of the array is the total size, the rest are the sizes of each line in order
//
Vec2 *TRT_text_size(char *text, uint32_t *nLines, uint32_t textHeight, uint32_t spaceWidth, uint32_t letterSpacing, uint32_t lineOffset);

typedef enum {
    ELEMENT_ALIGN_START     = -3,
    ELEMENT_ALIGN_END       = -2,
    ELEMENT_ALIGN_CENTER    = -1
} ElementAlignment;

typedef enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER
} TextAlignment;

void TRT_text_draw(char *text, Vec2 position, uint32_t height, uint32_t color, TextAlignment textAlignment);

//
// Math
//
#define PI (3.14159265358979323846)

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)
#define DECIMAL(x) (x - (int32_t)x)
#define ROUND(x) ((int32_t)(x + 0.5f))

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
