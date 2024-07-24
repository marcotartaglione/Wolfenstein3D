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

static Fade currentFade = FADE_IN;
static int32_t fadeValue = 0;

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

void TRT_message(char *text) {
    MessageBoxA(NULL, text, text, MB_OK);
}

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

static void setupWindowClass(HINSTANCE hInstance, char *className) {
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

void TRT_setupWindow(HINSTANCE hInstance, char *className) {
    setupWindowClass(hInstance, className);
    setupFrame();
}

static void redraw() {
    InvalidateRect(windowHandle, NULL, FALSE);
    UpdateWindow(windowHandle);
}

void TRT_interpretateSize(Vec2 *size, bool considerUpScaling) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (size->x < 0)
        size->x = screenWidth / ABS(size->x);
    if (size->y < 0)
        size->y = screenHeight / ABS(size->y);

    if (!considerUpScaling)
        return;

    size->x *= windowUpScaling;
    size->y *= windowUpScaling;
}

void TRT_interpretatePosition(Vec2 *position, Vec2 size, bool toScreen) {
    uint32_t screenWidth = GetSystemMetrics(SM_CXSCREEN);
    uint32_t screenHeight = GetSystemMetrics(SM_CYSCREEN);

    Vec2 windowSize = TRT_getWindowSize();

    uint32_t width, height;

    if (toScreen) {
        width = screenWidth;
        height = screenHeight;
    } else {
        width = windowSize.x;
        height = windowSize.y;
    }

    if (position->x == -1)
        position->x = width / 2 - size.x / 2;
    if (position->y == -1)
        position->y = height / 2 - size.y / 2;
    if (position->x < -1)
        position->x = width / ABS(position->x) - size.x / 2;
    if (position->y < -1)
        position->y = height / ABS(position->y) - size.y / 2;
}

void TRT_startWindow(char *title, Vec2 size, Vec2 position) {
    TRT_interpretateSize(&size, true);
    TRT_interpretatePosition(&position, size, true);

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
        TRT_message("TRT_startWindow: Window Creation Failed!");
        exit(-1);
    }
}

static LARGE_INTEGER frequency;
static LARGE_INTEGER lastTime;
static LARGE_INTEGER currentTime;

static uint8_t windowTargetFps;

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

void TRT_runWindow(uint8_t targetFPS, void (*loop)(), void (*close)()) {
    windowTargetFps = targetFPS;

    initTimer();

    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        loop();
        redraw();

        waitForNextFrame();
    }
    TRT_clearFrame();
    close();
}

void TRT_clearFrame() {
    memset(frame.pixels, 0, frame.width * frame.height * sizeof(uint32_t));
}

void TRT_setWindowUpScaling(uint32_t upScaling) {
    if (upScaling == 0)
        exit(-1);

    windowUpScaling = upScaling;
}

void TRT_setWindowPixel(uint32_t x, uint32_t y, uint32_t color) {
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

void TRT_fillScreenWithColor(uint32_t color) {
    Vec2 windowSize = TRT_getWindowSize();

    for (int i = 0; i < windowSize.x; ++i) {
        for (int j = 0; j < windowSize.y; ++j) {
            TRT_setWindowPixel(i, j, color);
        }
    }
}

void TRT_windowDrawRectangle(Vec2 position, Vec2 size, uint32_t color) {
    TRT_interpretateSize(&size, false);
    TRT_interpretatePosition(&position, size, false);

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            TRT_setWindowPixel(position.x + x, position.y + y, color);
        }
    }
}

uint32_t TRT_getWindowPixel(uint32_t x, uint32_t y) {
    return frame.pixels[y * windowUpScaling * frame.width + x * windowUpScaling];
}

void TRT_setKeyCallback(void (*keyCallbackFunction)(uint32_t)) {
    keyCallback = keyCallbackFunction;
}

void TRT_setMouseCallback(void (*mouseCallbackFunction)(Click, uint32_t, uint32_t)) {
    mouseCallback = mouseCallbackFunction;
}

Fade TRT_windowFade(uint32_t fadeSpeedMilliseconds) {
    float totalFrames = (fadeSpeedMilliseconds / 1000.0f) * windowTargetFps;
    static uint32_t currentFrame = 0;

    uint32_t fadeDecrement = totalFrames > 0 ? 255 / totalFrames : 255;

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

Vec2 TRT_getWindowSize() {
    return (Vec2) {frame.width / windowUpScaling, frame.height / windowUpScaling};
}

long long TRT_getTime() {
    LARGE_INTEGER frequency, start;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    return (start.QuadPart * 1000) / frequency.QuadPart;
}