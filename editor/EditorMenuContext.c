//
// Created by Marco on 30/07/2024.
//

#include "EditorMenuContext.h"

WolfensteinContext editorMenuContext = {
        .init = editorMenuContextInit,
        .loop = editorMenuContextLoop,
        .close = editorMenuContextClose,
        .keyboardCallback = editorMenuKeyboardCallback,
        .mouseCallback = editorMenuMouseCallback,
};

static void createFiles() {
    char *titles[] = {
            "Escape from Wolfenstein",
            "Operation: Eisenfaust",
            "Die, Fuhrer, Die!",
            "A Dark Secret",
            "Trail of the Madman",
            "Confrontation",
    };

    for (int k = 0; k < EPISODES_COUNT; ++k) {
        Map **floors = malloc(sizeof(Map *) * EPISODE_N_FLOORS);

        for (uint8_t i = 0; i < EPISODE_N_FLOORS; ++i) {
            Map *map = malloc(sizeof(Map));

            map->width = 64;
            map->height = 64;
            map->walls = malloc(map->width * map->height * sizeof(Wall));

            for (int j = 0; j < map->width * map->height; ++j) {
                map->walls[j] = WALL_NULL;
            }

            map->enemiesCount = 10;

            Entity *player = malloc(sizeof(Entity));

            player->position = (Vec2) {32, 32};
            player->lookingAngle = 0;

            strcpy(player->textureName, "assets/entities/player.png");

            player->texture = NULL;
            player->height = 32;
            player->moveSpeed = 0.1f;
            player->rotationSpeed = 0.1f;
            player->health = 100;
            player->armor = 0;
            player->ammo = 100;
            player->points = 0;

            map->player = player;

            Entity **enemies = malloc(sizeof(Entity *) * map->enemiesCount);

            for (int j = 0; j < map->enemiesCount; ++j) {
                Entity *enemy = malloc(sizeof(Entity));

                enemy->position = (Vec2) {32, 32};
                enemy->lookingAngle = 0;

                strcpy(enemy->textureName, "assets/entities/enemy.png");

                enemy->texture = NULL;
                enemy->height = 32;
                enemy->moveSpeed = 0.1f;
                enemy->rotationSpeed = 0.1f;
                enemy->health = 100;
                enemy->armor = 0;
                enemy->ammo = 100;
                enemy->points = 0;

                enemies[j] = enemy;
            }

            map->enemies = enemies;

            floors[i] = map;
        }

        Episode episode = {
                .thumbnail = NULL,
                .floors = floors
        };

        strcpy(episode.title, titles[k]);

        char path[512];
        snprintf(path, 512, "assets/episode/%d/thumbnail.png", k + 1);
        strcpy(episode.thumbnailPath, path);

        FILE *fp;
        snprintf(path, 512, "assets/episode/%d/episode.data", k + 1);
        fp = fopen(path, "wb");

        Episode_save(fp, &episode);

        fclose(fp);
    }
}

void editorMenuContextInit() {
//    createFiles();

    title = TRT_image_get(EDITOR_TITLE_PATH);
    if (title == NULL) {
        TRT_error("Editor init", "Error loading title image", true);
    }

    controls = TRT_image_get(CONTROLS_IMAGE);
    if (controls == NULL) {
        TRT_error("Editor init", "Error loading controls image", true);
    }

    quitBackground = TRT_image_get(QUIT_IMAGE);
    if (quitBackground == NULL) {
        TRT_error("Editor init", "Error loading quit background image", true);
    }
}

LoopResult editorMenuContextLoop() {
    editormenu_renderers[editormenu_currentRenderer]();
    return editormenu_loopResult;
}

void editorMenuContextClose() {
    Editor_setEpisodeAndFloor(currentEpisode, currentFloor);
    currentEpisode = 0;
    currentFloor = 0;
    editormenu_loopResult = LOOP_RESULT_IDLE;
    editormenu_currentRenderer = 0;
}

void editorMenuKeyboardCallback(uint32_t key) {
    editormenu_keyboardCallbacks[editormenu_currentRenderer](key);
}

void editorMenuMouseCallback(Click click, uint32_t x, uint32_t y) {

}

static void drawChooseEpisode() {
    TRT_window_fill(MAIN_BACKGROUND_COLOR);
    drawHeaderLine();
    TRT_image_draw(title,
                   (Vec2) {ELEMENT_ALIGN_CENTER, TRT_window_getSize().y - MENU_TITLE_OFFSET_TOP - 37},
                   (Vec2) {115, 40});

    TRT_image_draw(controls,
                   (Vec2) {ELEMENT_ALIGN_CENTER, 0},
                   (Vec2) (Vec2) {102, 7});

    for (uint8_t i = 0; i < EPISODES_COUNT; i++) {
        char text[128];
        snprintf(text, 128, "%d) %s %s", i + 1, episodes[i]->title, (i == currentEpisode) ? "<-" : "");

        TRT_text_draw(text,
                      (Vec2) {ELEMENT_ALIGN_CENTER,
                              TRT_window_getSize().y - EPISODE_OFFSET_TITLE - (FONT_HEIGHT + EPISODE_GAP) * i},
                      FONT_HEIGHT,
                      FONT_COLOR,
                      TEXT_ALIGN_LEFT);
    }

    if (isDrawExit)
        drawExit();
}

static void drawChooseFloor() {
    TRT_window_fill(MAIN_BACKGROUND_COLOR);
    drawHeaderLine();
    TRT_image_draw(title,
                   (Vec2) {ELEMENT_ALIGN_CENTER, TRT_window_getSize().y - MENU_TITLE_OFFSET_TOP - 37},
                   (Vec2) {115, 40});

    TRT_image_draw(controls,
                   (Vec2) {ELEMENT_ALIGN_CENTER, 0},
                   (Vec2) (Vec2) {102, 7});

    char text[255];
    text[0] = '\0';

    for (uint8_t i = 1; i <= EPISODE_N_FLOORS; ++i) {
        char current[32];

        if (i % 2 == 1) {
            snprintf(current, 32, "%s %d %-2s", "Floor", i, (i - 1 == currentFloor) ? "<-" : "");
        } else {
            snprintf(current, 32, "%-2s %s %d", (i - 1 == currentFloor) ? "->" : "", "Floor", i);
        }

        strcat(text, current);

        if (i % 2 == 0) {
            strcat(text, "\n");
        }
    }

    TRT_text_draw(text,
                  (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                  FONT_HEIGHT,
                  FONT_COLOR,
                  TEXT_ALIGN_LEFT);
}

static void drawExit() {
    char text[] = "To close the editor press Y\nTo continue press N";

    Vec2 *size = TRT_text_size(text,
                               NULL,
                               FONT_HEIGHT,
                               FONT_SPACE_WIDTH,
                               FONT_LETTER_SPACING,
                               FONT_LINE_OFFSET);

    TRT_image_draw(quitBackground,
                   (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                   (Vec2) {size->x + 10, size->y + 10});

    TRT_text_draw(text,
                    (Vec2) {ELEMENT_ALIGN_CENTER, ELEMENT_ALIGN_CENTER},
                    FONT_HEIGHT,
                  QUIT_FONT_COLOR,
                    TEXT_ALIGN_CENTER);
}

static void chooseEpisodeKeyboardCallback(uint32_t key) {
    if (isDrawExit) {
        switch (key) {
            case 'y':
            case 'Y':
                editormenu_loopResult = LOOP_RESULT_END;
                break;
            case 'n':
            case 'N':
                isDrawExit = false;
                break;
            default:
                break;
        }

        return;
    }

    switch (key) {
        case VK_ESCAPE:
            isDrawExit = true;
            break;
        case 38:
            currentEpisode--;

            if (currentEpisode < 0) {
                currentEpisode = 0;
            }
            break;
        case 40:
            currentEpisode++;

            if (currentEpisode >= EPISODES_COUNT) {
                currentEpisode = EPISODES_COUNT - 1;
            }
            break;
        case 13:
            editormenu_currentRenderer++;
            break;
        default:
            break;
    }
}

static void chooseFloorKeyboardCallback(uint32_t key) {
    switch (key) {
        case VK_ESCAPE:
            currentFloor = 0;
            editormenu_currentRenderer = 0;
            break;
        case VK_LEFT:
            currentFloor--;
            break;
        case VK_UP:
            currentFloor -= 2;
            break;
        case VK_RIGHT:
            currentFloor++;
            break;
        case VK_DOWN:
            currentFloor += 2;
            break;
        case 13:
            editormenu_loopResult = LOOP_RESULT_NEXT;
            break;
        default:
            break;
    }

    if (currentFloor < 0) {
        currentFloor = 0;
    }
    if (currentFloor >= EPISODE_N_FLOORS) {
        currentFloor = EPISODE_N_FLOORS - 1;
    }
}