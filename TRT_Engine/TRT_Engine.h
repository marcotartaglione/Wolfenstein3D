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

// =====================================================================================================================
//
// Window management
//
// =====================================================================================================================
void TRT_window_setup(HINSTANCE hInstance, char *className);
void TRT_window_start(char *title, Vec2 size, Vec2 position);

typedef enum {
    TRT_WINDOW_STYLE_BORDER = WS_BORDER, 	// The window has a thin-line border
    TRT_WINDOW_STYLE_CAPTION = WS_CAPTION, 	// The window has a title bar (includes the WS_BORDER style).
    TRT_WINDOW_STYLE_CHILDWINDOW = WS_CHILDWINDOW, 	    // The window is a child window. A window with this style cannot have a menu bar. This style cannot be used with the WS_POPUP style.
    TRT_WINDOW_STYLE_CLIPCHILDREN = WS_CLIPCHILDREN, 	// Excludes the area occupied by child windows when drawing occurs within the parent window. This style is used when creating the parent window.
    TRT_WINDOW_STYLE_CLIPSIBLINGS = WS_CLIPSIBLINGS, 	// Clips child windows relative to each other; that is, when a particular child window receives a WM_PAINT message, the WS_CLIPSIBLINGS style clips all other overlapping child windows out of the region of the child window to be updated. If WS_CLIPSIBLINGS is not specified and child windows overlap, it is possible, when drawing within the client area of a child window, to draw within the client area of a neighboring child window.
    TRT_WINDOW_STYLE_DISABLED = WS_DISABLED, 	// The window is initially disabled. A disabled window cannot receive input from the user. To change this after a window has been created, use the EnableWindow function.
    TRT_WINDOW_STYLE_DLGFRAME = WS_DLGFRAME, 	// The window has a border of a style typically used with dialog boxes. A window with this style cannot have a title bar.
    TRT_WINDOW_STYLE_GROUP = WS_GROUP, 	// The window is the first control of a group of controls. The group consists of this first control and all controls defined after it, up to the next control with the WS_GROUP style. The first control in each group usually has the WS_TABSTOP style so that the user can move from group to group. The user can subsequently change the keyboard focus from one control in the group to the next control in the group by using the direction keys.
    TRT_WINDOW_STYLE_HSCROLL = WS_HSCROLL, 	// The window has a horizontal scroll bar.
    TRT_WINDOW_STYLE_ICONIC = WS_ICONIC, 	// The window is initially minimized. Same as the WS_MINIMIZE style.
    TRT_WINDOW_STYLE_MAXIMIZE = WS_MAXIMIZE, 	// The window is initially maximized.
    TRT_WINDOW_STYLE_MAXIMIZEBOX = WS_MAXIMIZEBOX, 	// The window has a maximize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
    TRT_WINDOW_STYLE_MINIMIZE = WS_MINIMIZE, 	// The window is initially minimized. Same as the WS_ICONIC style.
    TRT_WINDOW_STYLE_MINIMIZEBOX = WS_MINIMIZEBOX, 	// The window has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
    TRT_WINDOW_STYLE_OVERLAPPED = WS_OVERLAPPED, 	// The window is an overlapped window. An overlapped window has a title bar and a border. Same as the WS_TILED style.
    TRT_WINDOW_STYLE_OVERLAPPEDWINDOW = WS_OVERLAPPEDWINDOW, 	// The window is an overlapped window. Same as the WS_TILEDWINDOW style.
    TRT_WINDOW_STYLE_POPUP = WS_POPUP, 	// The window is a pop-up window. This style cannot be used with the WS_CHILD style.
    TRT_WINDOW_STYLE_POPUPWINDOW = WS_POPUPWINDOW, 	// The window is a pop-up window. The WS_CAPTION and WS_POPUPWINDOW styles must be combined to make the window menu visible.
    TRT_WINDOW_STYLE_SIZEBOX = WS_SIZEBOX, 	// The window has a sizing border. Same as the WS_THICKFRAME style.
    TRT_WINDOW_STYLE_SYSMENU = WS_SYSMENU, 	// The window has a window menu on its title bar. The WS_CAPTION style must also be specified.
    TRT_WINDOW_STYLE_TABSTOP = WS_TABSTOP, 	// The window is a control that can receive the keyboard focus when the user presses the TAB key. Pressing the TAB key changes the keyboard focus to the next control with the WS_TABSTOP style.
    TRT_WINDOW_STYLE_THICKFRAME = WS_THICKFRAME, 	// The window has a sizing border. Same as the WS_SIZEBOX style.
    TRT_WINDOW_STYLE_TILED = WS_TILED, 	// The window is an overlapped window. An overlapped window has a title bar and a border. Same as the WS_OVERLAPPED style.
    TRT_WINDOW_STYLE_TILEDWINDOW = WS_TILEDWINDOW, 	// The window is an overlapped window. Same as the WS_OVERLAPPEDWINDOW style.
    TRT_WINDOW_STYLE_VISIBLE = WS_VISIBLE, 	// The window is initially visible. This style can be turned on and off by using the ShowWindow or SetWindowPos function.
    TRT_WINDOW_STYLE_VSCROLL = WS_VSCROLL 	// The window has a vertical scroll bar.
} TRT_window_style;

static uint32_t windowTargetFps;

void TRT_window_run(uint8_t targetFPS, void (*loop)(), void (*close)());
void TRT_window_close();

void TRT_window_clear();

static unsigned long windowStyle = WS_VISIBLE;

void TRT_window_addStyle(TRT_window_style style);
void TRT_window_removeStyle(TRT_window_style style);

// toScreen = true: reference to monitor size
// toScreen = false: reference to window size
void TRT_window_interpretatePosition(Vec2 *position, Vec2 size, bool toScreen);
void TRT_window_interpretateSize(Vec2 *size, bool considerUpScaling);

void TRT_window_setPixel(uint32_t x, uint32_t y, uint32_t color);
uint32_t TRT_window_getPixel(uint32_t x, uint32_t y);

Vec2 TRT_window_getSize();

void TRT_window_setUpscaling(uint32_t upScaling);
void TRT_window_fill(uint32_t color);
void TRT_window_drawRectangle(Vec2 position, Vec2 size, uint32_t color, bool fill);

void TRT_window_setTitle(char* title);

// =====================================================================================================================
//
// Animation management
//
// =====================================================================================================================
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

// =====================================================================================================================
//
// Input management
//
// =====================================================================================================================
typedef enum Click {
    CLICK_LEFT,
    CLICK_RIGHT
} Click;

void TRT_input_setKeyCallback(void (*keyCallbackFunction)(uint32_t));
void TRT_input_setMouseCallback(void (*mouseCallbackFunction)(Click, uint32_t, uint32_t));

// =====================================================================================================================
//
// Image management
//
// =====================================================================================================================
Image *TRT_image_get(char *path);

void TRT_image_draw(Image *image, Vec2 position, Vec2 size);
void TRT_image_free(Image *image);

// =====================================================================================================================
//
// Font management
//
// =====================================================================================================================
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

// =====================================================================================================================
//
// Math
//
// =====================================================================================================================
#define PI (3.14159265358979323846)

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)
#define DECIMAL(x) (x - (int32_t)x)
#define ROUND(x) ((int32_t)((x) + 0.5f))
#define SIGN(x) (x < 0 ? -1 : 1)

#endif //WOLFENSTEIN3D_TRT_ENGINE_H
