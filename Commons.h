//
// Created by Marco on 18/07/2024.
//

#ifndef WOLFENSTEIN3D_COMMONS_H
#define WOLFENSTEIN3D_COMMONS_H

#include "TRT_Engine/TRT_Engine.h"

//
// MODIFING THIS DEFINITIONS CAN CAUSE UNEXPECTED BEHAVIOUR
//
#define WOLFENSTEIN3D_IS_DEBUG      (1)
#define WOLFENSTEIN3D_CLASS_NAME    ("Wolfenstein3D")
#define WOLFENSTEIN3D_TITLE         (WOLFENSTEIN3D_CLASS_NAME)

#define WOLFENSTEIN3D_WIDTH         (320)
#define WOLFENSTEIN3D_HEIGHT        (200)

#define WOLFENSTEIN3D_SIZE          ((Vec2){ WOLFENSTEIN3D_WIDTH, WOLFENSTEIN3D_HEIGHT })
#define WOLFENSTEIN3D_POSITION      ((Vec2){-1, -1})

#define WOLFENSTEIN3D_UPSCALING     (3)

#define WOLFENSTEIN3D_EDITOR_FLAG   ("--editor")
#define WOLFENSTEIN3D_TARGET_FPS    (30)
//

//
// STYLE DEFINITIONS
//
#define MAIN_BACKGROUND_COLOR       (0x74180A)

#define FADE_TIME               (500)

#define FONT_DIRECTORY          ("./assets/font/")
#define FONT_BG_COLOR           (0xF00000)
#define FONT_COLOR              (0xBCBCBC)
#define FONT_HEIGHT             (10)
#define FONT_SPACE_WIDTH        (7)
#define FONT_LINE_OFFSET        (3)
#define FONT_LETTER_SPACING     (1)

#define HEADER_LINE_Y_OFFSET            (10)
#define HEADER_LINE_BACKGROUND_HEIGHT   (22)
#define HEADER_LINE_BACKGROUND_COLOR    (0x000000)

#define QUIT_IMAGE                  ("./assets/hud/options/quit.png")
#define QUIT_FONT_COLOR             (0x0)

#define CONTROLS_IMAGE              ("./assets/hud/options/controls.png")

//
// LOG FILE
//
#define ERROR_LOG_FILE              ("./error.log")

//
// GAME DEFINITIONS
//
#define WALL_TEXTURES_FOLDER    ("./assets/textures/walls/")

#define EPISODES_MAIN_FOLDER    ("./assets/episode/")
#define EPISODES_COUNT          (1)

#define DIFFICULTY_COUNT        (4)

static void drawHeaderLine() {
    Vec2 windowSize = TRT_window_getSize();

    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_window_drawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET - HEADER_LINE_BACKGROUND_HEIGHT},
                             (Vec2) {-1, HEADER_LINE_BACKGROUND_HEIGHT},
                             HEADER_LINE_BACKGROUND_COLOR,
                             true);

    TRT_window_drawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET - HEADER_LINE_BACKGROUND_HEIGHT - 2},
                             (Vec2) {-1, 1},
                             HEADER_LINE_BACKGROUND_COLOR,
                             true);
}



#endif //WOLFENSTEIN3D_COMMONS_H
