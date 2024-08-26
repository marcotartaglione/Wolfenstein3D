//
// Created by Marco on 28/07/2024.
//
#include "GameContext.h"
#include <math.h>

#include "../Wolfenstein3D.h"

WolfensteinContext gameContext = {
    Game_contextInit,
    Game_contextLoop,
    Game_keyboardCallback,
    Game_mouseCallback,
    Game_contextClose
};

void Game_setSize(Vec2 size) {
    GameContext_GameSize = size;
}

Vec2 Game_getSize() {
    return GameContext_GameSize;
}

void Game_setEpisode(uint32_t episode) {
    GameContext_Episode = episode;
}

void Game_setFloor(uint32_t floor) {
    GameContext_Floor = floor;
}

void Game_setDifficulty(uint8_t newDifficulty) {
    GameContext_Difficulty = newDifficulty;
}

void Game_contextInit() {
    GameContext_LoopResult = LOOP_RESULT_IDLE;

    GameContext_PlayerStats = TRT_image_get(GAME_HUD_PLAYER_STATS_IMAGE);
    if (GameContext_PlayerStats == NULL) {
        exit(EXIT_FAILURE);
    }

    GameContext_Map = episodes[GameContext_Episode]->floors[GameContext_Floor];
}

LoopResult Game_contextLoop() {
    Game_drawFrame(GameContext_GameSize);
    Game_drawPlayerView();
    Game_drawHUD();

    return GameContext_LoopResult;
}

void Game_contextClose() {
    GameContext_Episode = 0;
    GameContext_Floor = 0;
    GameContext_Difficulty = 0;
    GameContext_Map = NULL;

    TRT_image_free(GameContext_PlayerStats);
}

void Game_keyboardCallback(uint32_t key) {
    switch (key) {
        case VK_ESCAPE:
            GameContext_LoopResult = LOOP_RESULT_SPECIED + optionsContextIndex;
            break;
        case 'a':
        case 'A':
        case VK_LEFT:
            EntityBehaviour_rotate(GameContext_Map->player, ENTITY_ROTATE_LEFT);
            break;
        case 'd':
        case 'D':
        case VK_RIGHT:
            EntityBehaviour_rotate(GameContext_Map->player, ENTITY_ROTATE_RIGHT);
            break;
        case 'w':
        case 'W':
        case VK_UP:
            EntityBehaviour_move(GameContext_Map->player, GameContext_Map, ENTITY_MOVE_FORWARD);
            break;
        case 's':
        case 'S':
        case VK_DOWN:
            EntityBehaviour_move(GameContext_Map->player, GameContext_Map, ENTITY_MOVE_BACKWARD);
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
    (Vec2){ELEMENT_ALIGN_CENTER, (TRT_window_getSize().y + GAME_FRAME_OFFSET_FROM_BOTTOM - GameContext_GameSize.y) / 2},
        frameSize,
        0x000000,
        true);
}

static void Game_drawHUD() {
    TRT_image_draw(GameContext_PlayerStats,
                   (Vec2){0, GAME_HUD_OFFSET_BOTTOM},
                   GAME_HUD_SIZE);
}

static void Game_drawPlayerView() {
    float currentAngle = GameContext_Map->player->lookingAngle - GAME_FOV_ANGLE / 2;
    float angleDelta = GAME_FOV_ANGLE / GameContext_GameSize.x;

    for (uint32_t column = 0; column < GameContext_GameSize.x; ++column) {
        float distance;
        float hitPerc;
        int side;
        WallTexture wall = Game_raycast(20, currentAngle, &distance, &hitPerc, &side);

        if (wall == WALL_NULL) {
            continue;
        }

        Game_drawTextureColumn(column, hitPerc, distance, wall);
        currentAngle += angleDelta;
    }
}

static void Game_drawTextureColumn(int column, float hitPerc, float distance, WallTexture wall) {
    const int32_t wallHeight = (int)((float)GameContext_GameSize.y / distance);
    const int32_t offset = (wallHeight != GameContext_GameSize.y)
                               ? ROUND((GameContext_GameSize.y - wallHeight) * 0.5)
                               : 0;

    const float heightStep = (float)wallHeight / (float)wallTextures[wall]->height;
    const float widthOffset = (float)wallTextures[wall]->width * hitPerc;

    const Vec2 windowSize = TRT_window_getSize();
    const int offsetLeft = (windowSize.x - GameContext_GameSize.x) / 2;

    for (int i = 0; i < GameContext_GameSize.y; ++i) {
        uint32_t color;

        if (i >= GameContext_GameSize.y - offset) {
            color = GAME_ROOF_COLOR;
        }
        else if (i < offset) {
            color = GAME_FLOOR_COLOR;
        }
        else {
            const int textureIndex = (int)((float)(i - offset) / heightStep);
            if (textureIndex >= wallTextures[wall]->height)
                continue;

            const int textureDataIndex = ((int)widthOffset + textureIndex * wallTextures[wall]->width) * 3;
            const uint32_t r = wallTextures[wall]->data[textureDataIndex] << 16;
            const uint32_t g = wallTextures[wall]->data[textureDataIndex + 1] << 8;
            const uint32_t b = wallTextures[wall]->data[textureDataIndex + 2];

            color = r | g | b;
        }

        TRT_window_setPixel(column + offsetLeft,
                            i + (TRT_window_getSize().y + GAME_FRAME_OFFSET_FROM_BOTTOM - GameContext_GameSize.
                                y) / 2, color);
    }
}

static WallTexture Game_raycast(float maxDistance, float angle, float* distance, float* hitPerc, int* side) {
    if (GameContext_Map == NULL) {
        TRT_error("Game_raycast", "The map is NULL, how tf?", true);
        return WALL_NULL;
    }

    if (distance == NULL || hitPerc == NULL || side == NULL) {
        TRT_error("Game_raycast", "The distance, hitPerc or side is NULL", true);
        return WALL_NULL;
    }

    float rayPosX = GameContext_Map->player->position.x;
    float rayPosY = GameContext_Map->player->position.y;
    float rayDirX = cosf(angle);
    float rayDirY = sinf(angle);

    int mapX = (int)rayPosX;
    int mapY = (int)rayPosY;

    float sideDistX;
    float sideDistY;

    float deltaDistX = sqrtf(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    float deltaDistY = sqrtf(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

    int stepX;
    int stepY;

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

    WallTexture result = WALL_NULL;
    while (mapX >= 0 && (uint16_t)mapX < GameContext_Map->width && mapY >= 0 && (uint16_t)mapY < GameContext_Map->height) {
        if ((result = GameContext_Map->walls[mapX + mapY * GameContext_Map->width]->wallTexture) != WALL_NULL) {
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

    *distance = (float)(*side
                        ? fabs((mapY - rayPosY + (1.0 - stepY) / 2.0) / rayDirY)
                        : fabs((mapX - rayPosX + (1.0 - stepX) / 2.0) / rayDirX));
    *hitPerc = *side ? rayPosX + rayDirX * (*distance) : rayPosY + rayDirY * (*distance);
    *hitPerc = DECIMAL(*hitPerc);

    *distance = ABS(*distance) * cosf(angle - GameContext_Map->player->lookingAngle);

    return result;
}
