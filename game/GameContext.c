//
// Created by Marco on 28/07/2024.
//
#include "GameContext.h"
#include <math.h>

WolfensteinContext gameContext = {
        Game_contextInit,
        Game_contextLoop,
        Game_keyboardCallback,
        Game_mouseCallback,
        Game_contextClose};

void Game_setSize(Vec2 size) {
    gameSize = size;
}

Vec2 Game_getSize() {
    return gameSize;
}

void Game_setEpisode(uint32_t episode) {
    currentEpisode = episode;
}

void Game_setFloor(uint32_t floor) {
    currentFloor = floor;
}

void Game_setDifficulty(uint8_t newDifficulty) {
    difficulty = newDifficulty;
}

void Game_contextInit() {
    playerStats = TRT_image_get(GAME_HUD_PLAYER_STATS_IMAGE);
    if (playerStats == NULL) {
        exit(EXIT_FAILURE);
    }

    currentMap = episodes[currentEpisode]->floors[currentFloor];
}

LoopResult Game_contextLoop() {
    Game_drawFrame(gameSize);
    Game_drawPlayerView();
    Game_drawHUD();

    return LOOP_RESULT_IDLE;
}

void Game_contextClose() {
    currentEpisode = 0;
    currentFloor = 0;
    difficulty = 0;
    currentMap = NULL;

    TRT_image_free(playerStats);
}

void Game_keyboardCallback(uint32_t key) {
    switch (key) {
        case VK_LEFT:
            EntityBehaviour_rotate(currentMap->player, ENTITY_ROTATE_LEFT);
            break;
        case VK_RIGHT:
            EntityBehaviour_rotate(currentMap->player, ENTITY_ROTATE_RIGHT);
            break;
        case VK_UP:
            EntityBehaviour_move(currentMap->player, currentMap, ENTITY_MOVE_FORWARD);
            break;
        case VK_DOWN:
            EntityBehaviour_move(currentMap->player, currentMap, ENTITY_MOVE_BACKWARD);
            break;
        default:
            break;
    }
}

void Game_mouseCallback(Click click, uint32_t x, uint32_t y) {

}

void Game_drawFrame(Vec2 frameSize) {
    TRT_window_fill(GAME_FRAME_BACKGROUND_COLOR);
    TRT_window_DrawRectangle(
            (Vec2) {ELEMENT_ALIGN_CENTER, GAME_FRAME_OFFSET_FROM_BOTTOM},
            frameSize,
            0x000000,
            true);
}

static void Game_drawHUD() {
    TRT_image_draw(playerStats,
                   (Vec2) {0, GAME_HUD_OFFSET_BOTTOM},
                   GAME_HUD_SIZE);
}

static void Game_drawPlayerView() {
    float currentAngle = currentMap->player->lookingAngle - GAME_FOV_ANGLE / 2;
    float angleDelta = GAME_FOV_ANGLE / gameSize.x;

    for (uint32_t column = 0; column < gameSize.x; ++column) {
        float distance;
        float hitPerc;
        int side;
        Wall wall = Game_raycast(20, currentAngle, &distance, &hitPerc, &side);

        if (wall == WALL_NULL) {
            continue;
        }

        Game_drawTextureColumn(column, hitPerc, distance, wall);
        currentAngle += angleDelta;
    }
}

static void Game_drawTextureColumn(int column, float hitPerc, float distance, Wall wall) {
    const int32_t wallHeight = (int) ((float) gameSize.y / distance);
    const int32_t offset = (wallHeight != gameSize.y) ? ROUND((gameSize.y - wallHeight) * 0.5) : 0;

    const float heightStep = (float) wallHeight / (float) wallTextures[wall]->height;
    const float widthOffset = (float) wallTextures[wall]->width * hitPerc;

    const Vec2 windowSize = TRT_window_getSize();
    const int offsetLeft = (windowSize.x - gameSize.x) / 2;

    for (int i = -MIN(0, offset); i < gameSize.y - offset; ++i) {
        const int textureIndex = (int) ((float) i / heightStep);
        if (textureIndex >= wallTextures[wall]->height - 2)
            break;

        const int textureDataIndex = ((int) widthOffset + textureIndex * wallTextures[wall]->width) * 3;
        const uint32_t r = wallTextures[wall]->data[textureDataIndex] << 16;
        const uint32_t g = wallTextures[wall]->data[textureDataIndex + 1] << 8;
        const uint32_t b = wallTextures[wall]->data[textureDataIndex + 2];

        TRT_window_setPixel(column + offsetLeft, i + offset + (TRT_window_getSize().y + GAME_FRAME_OFFSET_FROM_BOTTOM - gameSize.y) / 2, r | g | b);
    }
}

static Wall Game_raycast(float maxDistance, float angle, float *distance, float *hitPerc, int *side) {
    if (currentMap == NULL) {
        TRT_error("Game_raycast", "The map is NULL, how tf?", true);
        return WALL_NULL;
    }

    if (distance == NULL || hitPerc == NULL || side == NULL) {
        TRT_error("Game_raycast", "The distance, hitPerc or side is NULL", true);
        return WALL_NULL;
    }

    float rayPosX = currentMap->player->position.x;
    float rayPosY = currentMap->player->position.y;
    float rayDirX = cosf(angle);
    float rayDirY = sinf(angle);

    float mapX = (int) rayPosX;
    float mapY = (int) rayPosY;

    float sideDistX;
    float sideDistY;

    float deltaDistX = sqrtf(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    float deltaDistY = sqrtf(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

    float stepX;
    float stepY;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (rayPosX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1 - rayPosX) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (rayPosY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1 - rayPosY) * deltaDistY;
    }

    Wall result;
    while (mapX >= 0 && (uint16_t) mapX < currentMap->width && mapY >= 0 && (uint16_t) mapY < currentMap->height) {
        if ((result = currentMap->walls[(int) mapX + (int) mapY * currentMap->width]) != WALL_NULL) {
            break;
        }

        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            *side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            *side = 1;
        }
    }

    *distance = (float) (*side ?
                         fabs((mapY - rayPosY + (1.0 - stepY) / 2.0) / rayDirY) :
                         fabs((mapX - rayPosX + (1.0 - stepX) / 2.0) / rayDirX));
    *hitPerc = *side ?
               rayPosX + rayDirX * (*distance) :
               rayPosY + rayDirY * (*distance);

    *distance = ABS(*distance) * cosf(angle - currentMap->player->lookingAngle);
    *hitPerc = DECIMAL(*hitPerc);

    return result;
}