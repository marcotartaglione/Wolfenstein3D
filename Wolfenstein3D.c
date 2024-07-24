#include "Wolfenstein3D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    TRT_loadSymbols(FONT_DIRECTORY);
    TRT_setFontBackgroundColor(FONT_BG_COLOR);

    TRT_setupWindow(
            hInstance,
            "Wolfenstein 3D"
    );
    TRT_setWindowUpScaling(3);
    TRT_startWindow(
            "Wolfenstein 3D",
            (Vec2) {320, 200},
            (Vec2) {-1, -1}
    );

    mainMenuContext.init();

    TRT_setKeyCallback(mainMenuContext.keyboardCallback);
    TRT_setMouseCallback(mainMenuContext.mouseCallback);

    TRT_runWindow(GAME_TARGET_FPS, loop, close);

    return 0;
}

void loop() {
    mainMenuContext.loop();
}

void close() {
    mainMenuContext.close();
}