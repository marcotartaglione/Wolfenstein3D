#include "Wolfenstein3D.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ int nCmdShow) {
    TRT_debug_set(WOLFENSTEIN3D_IS_DEBUG);
    TRT_error_setLogFile(ERROR_LOG_FILE);

    loadEpisodes();
    loadWallTextures();

    TRT_text_loadFont(FONT_DIRECTORY);
    TRT_text_setBackgroundColor(FONT_BG_COLOR);

    TRT_text_setLetterSpacing(FONT_LETTER_SPACING);
    TRT_text_setLineOffset(FONT_LINE_OFFSET);
    TRT_text_setSpaceWidth(FONT_SPACE_WIDTH);

    TRT_animation_setFadeTime(FADE_TIME);

    TRT_window_addStyle(TRT_WINDOW_STYLE_POPUP);
    TRT_window_setup(
        hInstance,
        WOLFENSTEIN3D_CLASS_NAME
    );
    TRT_window_setUpscaling(WOLFENSTEIN3D_UPSCALING);
    TRT_window_start(
        WOLFENSTEIN3D_TITLE,
        WOLFENSTEIN3D_SIZE,
        WOLFENSTEIN3D_POSITION
    );

    isEditor = (strcmp(lpCmdLine, WOLFENSTEIN3D_EDITOR_FLAG) == 0);
    if (isEditor) {
        contexts[0] = editorMenuContext;
        contexts[1] = editorContext;

        contextsCount = 2;
    }
    else {
        contexts[0] = mainMenuContext;
        contexts[1] = optionsContext;
        contexts[2] = gameContext;

        // TODO - This is a temporary solution
        currentContext = 2;

        contextsCount = 3;
        optionsContextIndex = 1;
    }

    if (!activateCurrentContext())
        return 0;

    TRT_window_run(WOLFENSTEIN3D_TARGET_FPS, loop, close);

    return 0;
}

void loop() {
    LoopResult loopResult = contexts[currentContext].loop();

    if (loopResult >= LOOP_RESULT_SPECIED) {
        deactivateCurrentContext();
        lastContext = currentContext;
        currentContext = loopResult - LOOP_RESULT_SPECIED;
        activateCurrentContext();
        return;
    }

    switch (loopResult) {
        case LOOP_RESULT_IDLE:
            return;
        case LOOP_RESULT_NEXT:
            if (!deactivateCurrentContext()) {
                lastContext = currentContext;
                currentContext++;
                activateCurrentContext();
                return;
            }
        case LOOP_RESULT_PREVIOUS:
            deactivateCurrentContext();
            uint8_t temp = currentContext;
            currentContext = lastContext;
            lastContext = temp;

            activateCurrentContext();
            return;
        case LOOP_RESULT_END:
            break;
        default:
            return;
    }

    TRT_window_close();
}

void close() {
    for (uint8_t i = 0; i < EPISODES_COUNT; i++) {
        if (episodes[i] == NULL) {
            TRT_error("Wolfenstein3D.c",
                      "NULL value found in the array of episodes during closure. This should not happen", false);
            continue;
        }

        Episode_free(episodes[i]);
    }
    for (uint8_t i = 0; i < WALL_NUMBER; ++i) {
        if (wallTextures[i] == NULL) {
            TRT_error("Wolfenstein3D.c",
                      "NULL value found in the array of wall textures during closure. This should not happen", false);
            continue;
        }

        TRT_image_free(wallTextures[i]);
    }
}

bool activateCurrentContext() {
    if (currentContext == contextsCount) {
        return false;
    }

    contexts[currentContext].init();
    TRT_input_setKeyCallback(contexts[currentContext].keyboardCallback);
    TRT_input_setMouseCallback(contexts[currentContext].mouseCallback);

    return true;
}

bool deactivateCurrentContext() {
    contexts[currentContext].close();
    return currentContext == contextsCount - 1;
}

void loadEpisodes() {
    for (uint8_t i = 0; i < EPISODES_COUNT; i++) {
        char path[256];
        snprintf(path, sizeof(path), "%s%d/episode.data", EPISODES_MAIN_FOLDER, i + 1);

        FILE* fp = fopen(path, "rb");
        if (fp == NULL) {
            char error[512];
            snprintf(error, sizeof(error), "Invalid episode path while loading: %s", path);

            TRT_error("Wolfenstein3D.c", error, true);
            continue;
        }

        Episode* current = Episode_get(fp);
        episodes[i] = current;

        fclose(fp);
    }
}

void loadWallTextures() {
    for (WallTexture i = 0; i < WALL_NUMBER; i++) {
        char path[256];
        snprintf(path, sizeof(path), "%s%s.png", WALL_TEXTURES_FOLDER, Wall_toString(i));

        wallTextures[i] = TRT_image_get(path);

        if (wallTextures[i] == NULL) {
            TRT_error("Wolfenstein3D.c", "NULL value insiede loadWallTextures function", true);
        }
    }
}
