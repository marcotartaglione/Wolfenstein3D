#include "Wolfenstein3D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    TRT_text_loadFont(FONT_DIRECTORY);
    TRT_text_setBackgroundColor(FONT_BG_COLOR);

    TRT_text_setLetterSpacing(FONT_LETTER_SPACING);
    TRT_text_setLineOffset(FONT_LINE_OFFSET);
    TRT_text_setSpaceWidth(FONT_SPACE_WIDTH);

    TRT_animation_setFadeTime(FADE_TIME);

    TRT_window_setup(
            hInstance,
            "Wolfenstein 3D"
    );
    TRT_window_setUpscaling(3);
    TRT_window_start(
            "Wolfenstein 3D",
            (Vec2) {320, 200},
            (Vec2) {-1, -1}
    );

    contexts[0] = mainMenuContext;
    contexts[1] = optionsContext;
    contexts[2] = gameContext;

    activateCurrentContext();

    TRT_window_run(GAME_TARGET_FPS, loop, close);

    return 0;
}

void loop() {
    if (contexts[currentContext].loop()) {
        deactivateCurrentContext();
        activateCurrentContext();
    }
}

void close() {
    contexts[currentContext].close();
}

void activateCurrentContext() {
    contexts[currentContext].init();
    TRT_input_setKeyCallback(contexts[currentContext].keyboardCallback);
    TRT_input_setMouseCallback(contexts[currentContext].mouseCallback);
}

void deactivateCurrentContext() {
    contexts[currentContext].close();
    currentContext++;
}