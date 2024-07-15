#include "TRT_Engine.h"
#include "../resources/Resources.h"

//
// Local Variables
//
static WNDCLASS windowClass;
static HWND windowHandle;

//
// GDI structures
//
static BITMAPINFO frameBitmapInfo;          // pixel format details
static HBITMAP frameBitmap = 0;              // bitmap info + array data
static HDC frameDeviceContext = 0;          // pointer to bitmap handle

static LRESULT CALLBACK WindowProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }

        case WM_PAINT: {
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

        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

static void setupWindowClass(HINSTANCE hInstance, char* className) {
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

void setupWindow(HINSTANCE hInstance, char* className) {
    setupWindowClass(hInstance, className);
    setupFrame();
}

static void redraw() {
    InvalidateRect(windowHandle, NULL, FALSE);
    UpdateWindow(windowHandle);
}

static void interpretateSize(Vec2 *size) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (size->x < 0)
        size->x = screenWidth / ABS(size->x);
    if (size->y < 0)
        size->y = screenHeight / ABS(size->y);
}

static void interpretatePosition(Vec2 *position, Vec2 size) {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (position->x == -1)
        position->x = screenWidth / 2 - size.x / 2;
    if (position->y == -1)
        position->y = screenHeight / 2 - size.y / 2;
    if (position->x < -1)
        position->x = screenWidth / ABS(position->x) - size.x / 2;
    if (position->y < -1)
        position->y = screenHeight / ABS(position->y) - size.y / 2;
}

void startWindow(char* title, Vec2 size, Vec2 position) {
    interpretateSize(&size);
    interpretatePosition(&position, size);

    windowHandle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        exit(-1);
    }

    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        loop();
        redraw();
    }
}

void clearFrame() {
    memset(frame.pixels, 0, frame.width * frame.height * sizeof(uint32_t));
}