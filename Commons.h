//
// Created by Marco on 18/07/2024.
//

#ifndef WOLFENSTEIN3D_COMMONS_H
#define WOLFENSTEIN3D_COMMONS_H

#define FONT_BG_COLOR (0xF00000)
#define FONT_DIRECTORY "assets/font/"

#define MAIN_BACKGROUND_COLOR           (0x74180A)

#define HEADER_LINE_Y_OFFSET            (10)
#define HEADER_LINE_BACKGROUND_HEIGHT   (22)
#define HEADER_LINE_BACKGROUND_COLOR    (0x000000)

#define FONT_HEIGHT             (10)
#define FONT_SPACE_WIDTH        (10)
#define FONT_LINE_OFFSET        (3)
#define FONT_LETTER_SPACING     (3)

static void drawHeaderLine() {
    Vec2 windowSize = TRT_window_getSize();

    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_window_DrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET -
                                        HEADER_LINE_BACKGROUND_HEIGHT},
                             (Vec2) {-1, HEADER_LINE_BACKGROUND_HEIGHT},
                             HEADER_LINE_BACKGROUND_COLOR);

    TRT_window_DrawRectangle((Vec2) {0, windowSize.y - HEADER_LINE_Y_OFFSET -
                                        HEADER_LINE_BACKGROUND_HEIGHT - 2},
                             (Vec2) {-1, 1},
                             HEADER_LINE_BACKGROUND_COLOR);
}

#endif //WOLFENSTEIN3D_COMMONS_H
