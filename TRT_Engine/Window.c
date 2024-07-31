#include "TRT_Engine.h"
#include "../resources/Resources.h"

//
// Local Variables
//
static WNDCLASS windowClass;
static HWND windowHandle;
static uint32_t windowUpScaling = 1;

static struct Frame {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
} frame;

static LARGE_INTEGER frequency;
static LARGE_INTEGER lastTime;
static LARGE_INTEGER currentTime;

//
// External functions
//
void (*keyCallback)(uint32_t key) = NULL;
void (*mouseCallback)(Click, uint32_t, uint32_t) = NULL;

//
// GDI structures
//
static BITMAPINFO frameBitmapInfo;          // pixel format details
static HBITMAP frameBitmap = 0;              // bitmap info + array data
static HDC frameDeviceContext = 0;          // pointer to bitmap handle

//
// Internal functions
//
static LRESULT CALLBACK WindowProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        case WM_PAINT: {
            for (int x = 0; x < frame.width * frame.height; ++x) {
                uint8_t r = (frame.pixels[x] & 0xFF0000) >> 16;
                uint8_t g = (frame.pixels[x] & 0x00FF00) >>  8;
                uint8_t b = (frame.pixels[x] & 0x0000FF) >>  0;

                if (currentFade == FADE_IN) {
                    r = r > fadeValue ? r - fadeValue : 0;
                    g = g > fadeValue ? g - fadeValue : 0;
                    b = b > fadeValue ? b - fadeValue : 0;
                } else {
                    r = fadeValue < r ? fadeValue : r;
                    g = fadeValue < g ? fadeValue : g;
                    b = fadeValue < b ? fadeValue : b;
                }

                frame.pixels[x] = (r << 16) | (g << 8) | b;
            }

            PAINTSTRUCT paint;
            HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frameDeviceContext,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint);

            break;
        }

        case WM_SIZE: {
            frameBitmapInfo.bmiHeader.biWidth = LOWORD(lParam);
            frameBitmapInfo.bmiHeader.biHeight = HIWORD(lParam);

            if (frameBitmap)
                DeleteObject(frameBitmap);

            frameBitmap = CreateDIBSection(NULL, &frameBitmapInfo, DIB_RGB_COLORS, (void **) &frame.pixels, 0, 0);
            SelectObject(frameDeviceContext, frameBitmap);

            frame.width = LOWORD(lParam);
            frame.height = HIWORD(lParam);

            break;
        }

        case WM_KEYDOWN: {
            if (keyCallback != NULL)
                keyCallback(wParam);

            break;
        }

        case WM_LBUTTONDOWN: {
            if (mouseCallback != NULL)
                mouseCallback(CLICK_LEFT, LOWORD(lParam), HIWORD(lParam));

            break;
        }

        case WM_RBUTTONDOWN: {
            if (mouseCallback != NULL)
                mouseCallback(CLICK_RIGHT, LOWORD(lParam), HIWORD(lParam));

            break;
        }

        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

static void setupWindowClass(HINSTANCE hInstance, const char *className) {
    windowClass.lpfnWndProc = WindowProcessMessage;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = className;

#ifdef WOLFEINSTEIN3D_ICON1
    windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(WOLFEINSTEIN3D_ICON1));
#endif

    if (!RegisterClass(&windowClass)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        exit(-1);
    }
}

static void setupFrame() {
    frameBitmapInfo.bmiHeader.biSize = sizeof(frameBitmapInfo.bmiHeader);
    frameBitmapInfo.bmiHeader.biPlanes = 1;
    frameBitmapInfo.bmiHeader.biBitCount = sizeof(uint32_t) * 8;
    frameBitmapInfo.bmiHeader.biCompression = BI_RGB;
    frameDeviceContext = CreateCompatibleDC(0);
}

static void redraw() {
    InvalidateRect(windowHandle, NULL, FALSE);
    UpdateWindow(windowHandle);
}

static void initTimer() {
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
}

static void waitForNextFrame() {
    while (true) {
        QueryPerformanceCounter(&currentTime);
        LONGLONG elapsedTime = currentTime.QuadPart - lastTime.QuadPart;
        if ((elapsedTime * 1000 / frequency.QuadPart) >= windowTargetFps) {
            lastTime = currentTime;
            break;
        }
    }
}

//
// External functions
//
void TRT_message(char *text) {
    MessageBoxA(NULL, text, text, MB_OK);
}

//
// Window management
//
void TRT_window_setup(HINSTANCE hInstance, char *className) {
    setupWindowClass(hInstance, className);
    setupFrame();
}

void TRT_window_start(char *title, Vec2 size, Vec2 position) {
    TRT_window_interpretateSize(&size, true);
    TRT_window_interpretatePosition(&position, size, true);

    windowHandle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            title,
            WS_POPUP | WS_VISIBLE,
            position.x,
            position.y,
            size.x,
            size.y,
            NULL,
            NULL,
            windowClass.hInstance,
            NULL
    );

    if (windowHandle == NULL) {
        TRT_message("TRT_window_start: Window Creation Failed!");
        exit(-1);
    }
}

void TRT_window_run(uint8_t targetFPS, void (*loop)(), void (*close)()) {
    windowTargetFps = targetFPS;

    initTimer();

    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!IsWindow(windowHandle))
            break;

        loop();

        if (isFading) {
            switch (TRT_animation_fade(fadeTime)) {
                case FADE_IN:
                    break;
                case FADE_OUT:
                    redraw();
                    break;
                case FADE_OVER:
                    isFading = false;
                    break;
            }
        }

        redraw();

        waitForNextFrame();
    }
    TRT_window_clear();
    close();
}

void TRT_window_close() {
    DestroyWindow(windowHandle);
}

void TRT_window_clear() {
    memset(frame.pixels, 0, frame.width * frame.height * sizeof(uint32_t));
}

void TRT_window_interpretateSize(Vec2 *size, bool considerUpScaling) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (size->x < 0)
        size->x = screenWidth / ABS(size->x);
    if (size->y < 0)
        size->y = screenHeight / ABS(size->y);

    if (!considerUpScaling)
        return;

    size->x *= (int32_t)windowUpScaling;
    size->y *= (int32_t)windowUpScaling;
}

void TRT_window_interpretatePosition(Vec2 *position, Vec2 size, bool toScreen) {
    uint32_t screenWidth = GetSystemMetrics(SM_CXSCREEN);
    uint32_t screenHeight = GetSystemMetrics(SM_CYSCREEN);

    Vec2 windowSize = TRT_window_getSize();

    uint32_t totalWidth, totalHeight;

    if (toScreen) {
        totalWidth = screenWidth;
        totalHeight = screenHeight;
    } else {
        totalWidth = windowSize.x;
        totalHeight = windowSize.y;
    }

    switch (position->x) {
        case ELEMENT_ALIGN_CENTER:
            position->x = (int32_t)(totalWidth - size.x) / 2;
            break;
        case ELEMENT_ALIGN_START:
            position->x = 0;
            break;
        case ELEMENT_ALIGN_END:
            position->x = (int32_t)totalWidth - size.x;
            break;
        default:
            if (position->x < 0)
                position->x = (int32_t)totalWidth / ABS(position->x) - size.x;
            break;
    }

    switch (position->y) {
        case ELEMENT_ALIGN_CENTER:
            position->y = (int32_t)(totalHeight - size.y) / 2;
            break;
        case ELEMENT_ALIGN_START:
            position->y = size.y;
            break;
        case ELEMENT_ALIGN_END:
            position->y = (int32_t)totalHeight - size.y;
            break;
        default:
            if (position->y < 0)
                position->y = (int32_t)totalHeight / ABS(position->y) - size.y;
            break;
    }
}

void TRT_window_setPixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= frame.width / windowUpScaling || y >= frame.height / windowUpScaling)
        return;

    for (uint32_t i = 0; i < windowUpScaling; ++i) {
        for (uint32_t j = 0; j < windowUpScaling; ++j) {
            if ((y * windowUpScaling + i) * frame.width + x * windowUpScaling + j >= frame.width * frame.height)
                return;

            frame.pixels[(y * windowUpScaling + i) * frame.width + x * windowUpScaling + j] = color;
        }
    }
}

uint32_t TRT_window_getPixel(uint32_t x, uint32_t y) {
    return frame.pixels[y * windowUpScaling * frame.width + x * windowUpScaling];
}

Vec2 TRT_window_getSize() {
    return (Vec2) {(int32_t)(frame.width / windowUpScaling), (int32_t)(frame.height / windowUpScaling)};
}

void TRT_window_setUpscaling(uint32_t upScaling) {
    if (upScaling == 0)
        exit(-1);

    windowUpScaling = upScaling;
}

void TRT_window_fill(uint32_t color) {
    Vec2 windowSize = TRT_window_getSize();

    for (int i = 0; i < windowSize.x; ++i) {
        for (int j = 0; j < windowSize.y; ++j) {
            TRT_window_setPixel(i, j, color);
        }
    }
}

void TRT_window_DrawRectangle(Vec2 position, Vec2 size, uint32_t color, bool fill) {
    TRT_window_interpretateSize(&size, false);
    TRT_window_interpretatePosition(&position, size, false);

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            if (!fill && (x != 0 && x != size.x - 1 && y != 0 && y != size.y - 1))
                continue;

            TRT_window_setPixel(position.x + x, position.y + y, color);
        }
    }
}

//
// Input management
//
void TRT_input_setKeyCallback(void (*keyCallbackFunction)(uint32_t)) {
    keyCallback = keyCallbackFunction;
}

void TRT_input_setMouseCallback(void (*mouseCallbackFunction)(Click, uint32_t, uint32_t)) {
    mouseCallback = mouseCallbackFunction;
}

//
// Created by Marco on 26/07/2024.
//
#include "TRT_Engine.h"

fade TRT_animation_fade(uint32_t fadeSpeedMilliseconds) {
    float totalFrames = ((float)fadeSpeedMilliseconds / 1000.0f) * (float)windowTargetFps;
    static uint32_t currentFrame = 0;

    int32_t fadeDecrement = totalFrames > 0 ? (int32_t)(255.0f / totalFrames) : 255;

    if (fadeDecrement == 0)
        fadeDecrement = 1;

    fadeValue += fadeDecrement;
    currentFrame++;

    if (currentFrame >= (uint32_t)(totalFrames * 2)) {
        fadeValue = 0;
        currentFrame = 0;
        currentFade = FADE_IN;

        return FADE_OVER;
    } else if (currentFrame == (uint32_t)totalFrames) {
        fadeValue = 0;
        currentFade = FADE_OUT;
    }

    return currentFade;
}

void TRT_animation_setFadeTime(uint32_t time) {
    fadeTime = time;
}

bool TRT_animation_startFade() {
    if (isFading)
        return false;

    isFading = true;
    return isFading;
}

bool TRT_animation_isFading() {
    if(!isFading)
        return false;

    return currentFade == FADE_IN;
}