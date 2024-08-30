//
// Created by Marco on 31/07/2024.
//

#include "EditorContext.h"

WolfensteinContext editorContext = {
        .init = editorContextInit,
        .loop = editorContextLoop,
        .close = editorContextClose,
        .keyboardCallback = editorKeyboardCallback,
        .mouseCallback = editorMouseCallback,
};

static void saveEpisode() {
    FILE *fp;

    char path[128];
    snprintf(path, 128, "assets/episode/%d/episode.data", selectedEpisode + 1);


    if (fopen_s(&fp, path, "wb") != 0) {
        char error[256];
        snprintf(error, 256, "%s %s", "Could not open the episode.data file. This path could be missing:", path);
        TRT_error("Save Episode", error, false);
        return;
    }

    Episode_save(fp, episodes[selectedEpisode]);

    fclose(fp);
}

void editorContextInit() {

}

LoopResult editorContextLoop() {
    editor_renderers[editor_currentRenderer]();

    return editor_loopResult;
}

void editorContextClose() {
    selectedEpisode = 0;
    selectedFloor = 0;
    selectedTile = 0;
    tileSize = EDITOR_MIN_TILE_SIZE;
    selectedWall = WALL_STONE1;
    shownWall = WALL_STONE1;
    editor_loopResult = LOOP_RESULT_IDLE;
    currentRow = 0;
    currentCol = 0;
    modifingRow = true;
    drawExitMessage = false;
    drawNavigationMenu = false;
}

void editorKeyboardCallback(uint32_t key) {
    editor_keyboardCallbacks[editor_currentRenderer](key);
}

void editorMouseCallback(Click click, uint32_t x, uint32_t y) {

}

void Editor_setEpisodeAndFloor(int8_t episode, int8_t floor) {
    selectedFloor = floor;
    selectedEpisode = episode;
}

static void increaseTileSize() {
    if (tileSize + EDITOR_TILE_SIZE_STEP <= EDITOR_MAX_TILE_SIZE) {
        tileSize += EDITOR_TILE_SIZE_STEP;
    }
}

static void decreaseTileSize() {
    if (tileSize - EDITOR_TILE_SIZE_STEP >= EDITOR_MIN_TILE_SIZE) {
        tileSize -= EDITOR_TILE_SIZE_STEP;
    }
}

static void drawExit() {
    char text[] = "Save and back to menu: Y\nDon't save and back to menu: N\nContinue editing: ESC";

    Vec2 *size = TRT_text_size(
            text,
            NULL,
            FONT_HEIGHT,
            FONT_SPACE_WIDTH,
            FONT_LETTER_SPACING,
            FONT_LINE_OFFSET);

    if (size == NULL) {
        // Handle error
        return;
    }

    TRT_window_drawRectangle(
            (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
            (Vec2) {size->x + 10, size->y + 10},
            MAIN_BACKGROUND_COLOR,
            true);

    TRT_text_draw(text,
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);

    free(size);
}

static void drawNavigation() {
    char text[128];

    snprintf(text, 128, "Choose coordinates\nRow:%d %s Col:%d", currentRow, modifingRow ? "<- " : " ->", currentCol);

    Vec2 *size = TRT_text_size(
            text,
            NULL,
            FONT_HEIGHT,
            FONT_SPACE_WIDTH,
            FONT_LETTER_SPACING,
            FONT_LINE_OFFSET);

    if (size == NULL) {
        TRT_error("Draw Navigation", "Could not get the size of the text", false);
        return;
    }

    TRT_window_drawRectangle(
            (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
            (Vec2) {size->x + 10, size->y + 10},
            MAIN_BACKGROUND_COLOR,
            true);

    TRT_text_draw(text,
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);

    free(size);

}

void drawGrid() {
    TRT_window_fill(0x000000);
    Vec2 winSize = TRT_window_getSize();

    float tilePerRow = (float) winSize.x / tileSize;
    float tilePerCol = (float) winSize.y / tileSize;

    int centerTileX = tilePerRow / 2;
    int centerTileY = tilePerCol / 2;

    uint32_t xOffset = (uint32_t) ((tilePerRow - (int) tilePerRow) / 2 * tileSize);
    uint32_t yOffset = (uint32_t) ((tilePerCol - (int) tilePerCol) / 2 * tileSize);

    uint32_t selectedTileX = selectedTile % episodes[selectedEpisode]->floors[selectedFloor]->width;
    uint32_t selectedTileY = selectedTile / episodes[selectedEpisode]->floors[selectedFloor]->width;

    for (int i = 0; i < (int) tilePerRow; i++) {
        for (int j = 0; j < (int) tilePerCol; j++) {
            int32_t tileX = i - centerTileX + selectedTileX;
            int32_t tileY = j - centerTileY + selectedTileY;

            if (tileX < 0 || tileY < 0 ||
                tileX >= episodes[selectedEpisode]->floors[selectedFloor]->width ||
                tileY >= episodes[selectedEpisode]->floors[selectedFloor]->height)
                continue;

            WallTexture wall = episodes[selectedEpisode]->floors[selectedFloor]->walls[
                    tileY * episodes[selectedEpisode]->floors[selectedFloor]->width + tileX]->wallTexture;

            if (wall != WALL_NULL) {
                TRT_image_draw(wallTextures[wall],
                               (Vec2) {i * tileSize + xOffset, j * tileSize + yOffset},
                               (Vec2) {tileSize, tileSize});
            }

            uint32_t color = 0xFFFFFF;
            if (i == centerTileX && j == centerTileY) {
                color = 0xFF0000;
            } else if (tileX == (int32_t) episodes[selectedEpisode]->floors[selectedFloor]->player->position.x &&
                       tileY == (int32_t) episodes[selectedEpisode]->floors[selectedFloor]->player->position.y) {
                color = 0x0000FF;
            }

            TRT_window_drawRectangle(
                    (Vec2) {i * tileSize + xOffset, j * tileSize + yOffset},
                    (Vec2) {tileSize, tileSize},
                    color,
                    false);
        }
    }

    if (drawExitMessage) {
        drawExit();
    }

    if (drawNavigationMenu) {
        drawNavigation();
    }
}

void drawTileSelector() {
    TRT_window_fill(MAIN_BACKGROUND_COLOR);

    TRT_text_draw("Currently selected wallTexture:",
                  (Vec2) {ELEMENT_ALIGN_CENTER, TRT_window_getSize().y - 15},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);

    TRT_image_draw(wallTextures[shownWall],
                   (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2) {64, 64});

    char text[128];

    uint32_t actualWallCount = WALL_NUMBER - WALL_NO_DARK_VERSION / 2;
    uint32_t currentTile;

    if (shownWall <= WALL_NO_DARK_VERSION) {
        currentTile = shownWall / 2;
    } else {
        currentTile = shownWall - WALL_NO_DARK_VERSION / 2;
    }


    snprintf(text, 128, "This wallTexture is %d of %d", currentTile + 1, actualWallCount);
    TRT_text_draw(text,
                  (Vec2) {ELEMENT_ALIGN_CENTER, 15},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_CENTER);
}

void gridKeyboardCallback(uint32_t key) {
    if (drawNavigationMenu) {
        switch (key) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                uint32_t number = key - '0';
                if (modifingRow) {
                    currentRow *= 10;
                    currentRow += number;

                    if (currentRow >= episodes[selectedEpisode]->floors[selectedFloor]->height) {
                        currentRow = episodes[selectedEpisode]->floors[selectedFloor]->height - 1;
                    }
                } else {
                    currentCol *= 10;
                    currentCol += number;

                    if (currentCol >= episodes[selectedEpisode]->floors[selectedFloor]->width) {
                        currentCol = episodes[selectedEpisode]->floors[selectedFloor]->width - 1;
                    }
                }
                break;
            }
            case VK_BACK:
                if (modifingRow) {
                    currentRow /= 10;
                } else {
                    currentCol /= 10;
                }
                break;
            case VK_ESCAPE:
                if (modifingRow) {
                    currentRow = 0;
                    currentCol = 0;
                    drawNavigationMenu = false;
                } else {
                    modifingRow = true;
                }
                break;
            case VK_RETURN:
                if (modifingRow) {
                    modifingRow = false;
                } else {
                    selectedTile = currentCol + episodes[selectedEpisode]->floors[selectedFloor]->width * currentRow;

                    drawNavigationMenu = false;

                    currentRow = 0;
                    currentCol = 0;
                    modifingRow = true;
                }
                break;
            default:
                break;
        }
        return;
    }

    if (drawExitMessage) {
        switch (key) {
            case 'Y':
                saveEpisode();
                editor_loopResult = LOOP_RESULT_PREVIOUS;
                break;
            case 'N':
                editor_loopResult = LOOP_RESULT_PREVIOUS;
                break;
            case VK_ESCAPE:
                drawExitMessage = false;
                break;
            default:
                break;
        }
        return;
    }

    WallData *wall;
    switch (key) {
        case VK_TAB:
            shownWall = selectedWall;
            editor_currentRenderer = 1;
            break;
        case VK_ADD:
            increaseTileSize();
            break;
        case VK_SUBTRACT:
            decreaseTileSize();
            break;
        case VK_UP:
            if (selectedTile / episodes[selectedEpisode]->floors[selectedFloor]->width <
                episodes[selectedEpisode]->floors[selectedFloor]->height - 1) {
                selectedTile += episodes[selectedEpisode]->floors[selectedFloor]->width;
            }
            break;
        case VK_DOWN:
            if (selectedTile >= episodes[selectedEpisode]->floors[selectedFloor]->width) {
                selectedTile -= episodes[selectedEpisode]->floors[selectedFloor]->width;
            }
            break;
        case VK_LEFT:
            if (selectedTile % episodes[selectedEpisode]->floors[selectedFloor]->width != 0) {
                selectedTile--;
            }
            break;
        case VK_RIGHT:
            if (selectedTile % episodes[selectedEpisode]->floors[selectedFloor]->width !=
                episodes[selectedEpisode]->floors[selectedFloor]->width - 1) {
                selectedTile++;
            }
            break;
        case VK_DELETE:
            WallData_free(episodes[selectedEpisode]->floors[selectedFloor]->walls[selectedTile]);

            wall = malloc(sizeof(WallData));
            *wall = (WallData) {
                    .wallTexture = WALL_NULL,
                    .isDoor = false,
                    .isSecret = false,
                    .isElevator = false,
                    .openPercentage = 0.0f,
                    .openTimeSeconds = 0.0f,
                    .openState = WALL_OPEN_STATE_CLOSED
            };

            episodes[selectedEpisode]->floors[selectedFloor]->walls[selectedTile] = wall;
            break;
        case VK_RETURN:
            wall = malloc(sizeof(WallData));
            *wall = (WallData) {
                    .wallTexture = selectedWall,
                    .isDoor = WALL_IS_DOOR(selectedWall),
                    .isSecret = false,
                    .isElevator = false,
                    .openPercentage = 0.0f,
                    .openTimeSeconds = 1.5f,
                    .openState = WALL_OPEN_STATE_CLOSED
            };

            episodes[selectedEpisode]->floors[selectedFloor]->walls[selectedTile] = wall;
            break;
        case 'p':
        case 'P':
            episodes[selectedEpisode]->floors[selectedFloor]->player->position.x = selectedTile % episodes[selectedEpisode]->floors[selectedFloor]->width + 0.5f;
            episodes[selectedEpisode]->floors[selectedFloor]->player->position.y = selectedTile / episodes[selectedEpisode]->floors[selectedFloor]->width + 0.5f;
            break;
        case VK_ESCAPE:
            drawExitMessage = true;
            break;
        case 'q':
        case 'Q':
            drawNavigationMenu = true;
        default:
            break;
    }
}

void tileSelectorKeyboardCallback(uint32_t key) {
    switch (key) {
        case 13:
            selectedWall = shownWall;
            editor_currentRenderer = 0;
            break;
        case VK_ESCAPE:
            editor_currentRenderer = 0;
            break;
        case VK_LEFT:
            if (shownWall > 0) {
                if (shownWall <= WALL_NO_DARK_VERSION)
                    shownWall -= 2;
                else
                    shownWall--;
            }
            break;
        case VK_RIGHT:
            if (shownWall < WALL_NUMBER - 1) {
                if (shownWall < WALL_NO_DARK_VERSION)
                    shownWall += 2;
                else
                    shownWall++;
            }
            break;
        default:
            break;
    }
}