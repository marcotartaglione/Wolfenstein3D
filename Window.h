//
// Created by Marco on 14/07/2024.
//

#ifndef WOLFENSTEIN3D_WINDOW_H
#define WOLFENSTEIN3D_WINDOW_H

#include <windows.h>
#include "Commons.h"

#define WOLFENSTEIN3D_WINDOW_WIDTH  (900)
#define WOLFENSTEIN3D_WINDOW_HEIGHT (600)

//
// Frame structure. The array of pixels is stored in the pixels field.
//
struct Frame {
    int width;
    int height;
    uint32_t *pixels;
} frame;

//
// Local Variables
//
static const char title[] = "Wolf3D";
static WNDCLASS windowClass;
static HWND windowHandle;

//
// GDI structures
//
static BITMAPINFO frameBitmapInfo;          // pixel format details
static HBITMAP frameBitmap = 0;              // bitmap info + array data
static HDC frameDeviceContext = 0;          // pointer to bitmap handle

void startWindow(HINSTANCE hInstance);
static LRESULT CALLBACK WindowProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif //WOLFENSTEIN3D_WINDOW_H
