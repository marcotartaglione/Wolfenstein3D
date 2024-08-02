//
// Created by Marco on 18/07/2024.
//

#ifndef WOLFENSTEIN3D_COMMONS_H
#define WOLFENSTEIN3D_COMMONS_H

#define ERROR_LOG_FILE  ("./error.log")

#define FONT_BG_COLOR   (0xF00000)
#define FONT_DIRECTORY  ("./assets/font/")
#define FONT_COLOR      (0xBCBCBC)

#define QUIT_FONT_COLOR         (0x0)
#define QUIT_IMAGE              ("./assets/hud/options/quit.png")
#define CONTROLS_IMAGE          ("./assets/hud/options/controls.png")

#define MAIN_BACKGROUND_COLOR           (0x74180A)

#define HEADER_LINE_Y_OFFSET            (10)
#define HEADER_LINE_BACKGROUND_HEIGHT   (22)
#define HEADER_LINE_BACKGROUND_COLOR    (0x000000)

#define FONT_HEIGHT             (10)
#define FONT_SPACE_WIDTH        (7)
#define FONT_LINE_OFFSET        (3)
#define FONT_LETTER_SPACING     (1)

#define FADE_TIME               (500)

#define EPISODES_MAIN_FOLDER    ("./assets/episode/")
#define EPISODES_COUNT          (1)

#define DIFFICULTY_COUNT        (4)

#define WALL_TEXTURES_FOLDER    ("./assets/textures/walls/")

static void drawHeaderLine() {
    Vec2 windowSize = TRT_window_getSize();

    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_window_DrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET - HEADER_LINE_BACKGROUND_HEIGHT},
                             (Vec2) {-1, HEADER_LINE_BACKGROUND_HEIGHT},
                             HEADER_LINE_BACKGROUND_COLOR,
                             true);

    TRT_window_DrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET - HEADER_LINE_BACKGROUND_HEIGHT - 2},
                             (Vec2) {-1, 1},
                             HEADER_LINE_BACKGROUND_COLOR,
                             true);
}

typedef enum {
    LOOP_RESULT_IDLE,
    LOOP_RESULT_NEXT,
    LOOP_RESULT_PREVIOUS,
    LOOP_RESULT_END,
    LOOP_RESULT_SPECIED
} LoopResult;

#endif //WOLFENSTEIN3D_COMMONS_H
