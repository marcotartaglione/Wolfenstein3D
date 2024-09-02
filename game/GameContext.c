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
    Game_updateMapElements();
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
        case VK_RETURN:
            EntityBehaviour_InteractResult result = EntityBehaviour_interact(GameContext_Map->player, GameContext_Map);
            if (!result.success)
                break;

            if (GameContext_UpdateMapElementsCount == 16) {
                TRT_error("Game_keyboardCallback", "The update map elements buffer is full", true);
                return;
            }

            GameContext_UpdatingMapElements[GameContext_UpdateMapElementsCount++] = GameContext_Map->walls[result.wallIndex];
            break;
        default:
            break;
    }
}

void Game_mouseCallback(Click click, uint32_t x, uint32_t y) {
}

void Game_drawFrame(Vec2 frameSize) {
    TRT_window_fill(GAME_FRAME_BACKGROUND_COLOR);

    TRT_window_drawRectangle(
            (Vec2) {ELEMENT_ALIGN_CENTER,
                    (TRT_window_getSize().y + GAME_FRAME_OFFSET_FROM_BOTTOM - GameContext_GameSize.y) / 2},
            frameSize,
            0x000000,
            true);
}

static void Game_updateMapElements() {
    for (uint8_t i = 0; i < GameContext_UpdateMapElementsCount; ++i) {
        WallData *wall = GameContext_UpdatingMapElements[i];
        uint32_t animationDurationInFrames = (uint32_t) (wall->openTimeSeconds * (float) WOLFENSTEIN3D_TARGET_FPS);

        switch (wall->openState) {
            case WALL_OPEN_STATE_OPENING:
                wall->elapsedFrames++;
                wall->openPercentage += 1.0f / (float) animationDurationInFrames;
                if (wall->elapsedFrames >= animationDurationInFrames) {
                    wall->elapsedFrames = 0;
                    wall->openPercentage = 1.0f;
                    wall->openState = WALL_OPEN_STATE_OPEN;
                }
                break;
            case WALL_OPEN_STATE_OPEN:
                // check if any enemy is blocking the door
                for (uint8_t j = 0; j < GameContext_Map->enemiesCountPerDifficulty[GameContext_Difficulty]; ++j) {
                    Entity *entity = GameContext_Map->enemies[j];
                    if (entity->position.x == wall->position.x && entity->position.y == wall->position.y) {
                        break;
                    }
                }
                // check if the player is blocking the door
                if ((int32_t) GameContext_Map->player->position.x == wall->position.x &&
                    (int32_t) GameContext_Map->player->position.y == wall->position.y) {
                    break;
                }

                wall->elapsedFrames++;
                if (wall->elapsedFrames >= animationDurationInFrames) {
                    wall->elapsedFrames = 0;
                    wall->openState = WALL_OPEN_STATE_CLOSING;
                }
                break;
            case WALL_OPEN_STATE_CLOSING:
                // check if any enemy is blocking the door
                for (uint8_t j = 0; j < GameContext_Map->enemiesCountPerDifficulty[GameContext_Difficulty]; ++j) {
                    Entity *entity = GameContext_Map->enemies[j];
                    if (entity->position.x == wall->position.x && entity->position.y == wall->position.y) {
                        break;
                    }
                }
                // check if the player is blocking the door
                if ((int32_t) GameContext_Map->player->position.x == wall->position.x &&
                    (int32_t) GameContext_Map->player->position.y == wall->position.y) {
                    break;
                }

                wall->elapsedFrames++;
                wall->openPercentage -= 1.0f / (float) animationDurationInFrames;
                if (wall->elapsedFrames >= animationDurationInFrames) {
                    wall->elapsedFrames = 0;
                    wall->openPercentage = 0.0f;
                    wall->openState = WALL_OPEN_STATE_CLOSED;

                    for (uint8_t j = i; j < GameContext_UpdateMapElementsCount; ++j) {
                        GameContext_UpdatingMapElements[j] = GameContext_UpdatingMapElements[j + 1];
                    }
                    GameContext_UpdateMapElementsCount--;
                }
                break;
            case WALL_OPEN_STATE_CLOSED:
            default:
                TRT_error("Game_updateMapElements", "This state should not happen when updating elements", true);
                break;
        }
    }
}

static void Game_drawHUD() {
    TRT_image_draw(GameContext_PlayerStats,
                   (Vec2) {0, GAME_HUD_OFFSET_BOTTOM},
                   GAME_HUD_SIZE);
}

static void Game_drawPlayerView() {
    float currentAngle = GameContext_Map->player->lookingAngle - GAME_FOV_ANGLE / 2;
    float angleDelta = GAME_FOV_ANGLE / GameContext_GameSize.x;

    for (uint32_t column = 0; column < GameContext_GameSize.x; ++column) {
        float distance;
        float hitPerc;
        int side;

        if (column == GameContext_GameSize.x / 2) {
            static int temp = 0;
        }

        WallTexture wall = Game_raycast(20, currentAngle, &distance, &hitPerc, &side);

        if (column == GameContext_GameSize.x / 2) {
            char temp[100];
            sprintf(temp, "Angle: %.2f    Distance: %.2f    Hitperc: %.2f    Wall: %d", currentAngle, distance, hitPerc, wall);
            TRT_window_setTitle(temp);
        }

        if (wall == WALL_NULL) {
            continue;
        }

        Game_drawTextureColumn(column, hitPerc, distance, wall);
        currentAngle += angleDelta;
    }
}

static void Game_drawTextureColumn(int column, float hitPerc, float distance, WallTexture wall) {
    const int32_t wallHeight = (int) ((float) GameContext_GameSize.y / distance);
    const int32_t offset = (wallHeight != GameContext_GameSize.y)
                           ? ROUND((float) (GameContext_GameSize.y - wallHeight) / 2)
                           : 0;

    const float heightStep = (float) wallHeight / (float) wallTextures[wall]->height;
    const float widthOffset = (float) wallTextures[wall]->width * hitPerc;

    const Vec2 windowSize = TRT_window_getSize();
    const int offsetLeft = (windowSize.x - GameContext_GameSize.x) / 2;

    for (int i = 0; i < GameContext_GameSize.y; ++i) {
        uint32_t color;

        if (i >= GameContext_GameSize.y - offset) {
            color = GAME_ROOF_COLOR;
        } else if (i < offset) {
            color = GAME_FLOOR_COLOR;
        } else {
            const uint32_t textureIndex = (uint32_t) ((float) (i - offset) / heightStep);
            if (textureIndex >= wallTextures[wall]->height)
                continue;

            const uint32_t textureDataIndex = ((uint32_t) widthOffset + textureIndex * wallTextures[wall]->width) * 3;

            if (textureDataIndex >= wallTextures[wall]->width * wallTextures[wall]->height * 3)
                continue;

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

static WallTexture Game_raycast(float maxDistance, float angle, float *distance, float *hitPerc, int *side) {
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

    int mapX = (int) rayPosX;
    int mapY = (int) rayPosY;

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

    WallData *hittedWall;
    WallTexture resultTexture = WALL_NULL;
    bool currentlyThoughDoor = false;

    uint32_t iterationCount = 0;

    while (mapX >= 0 && (uint16_t) mapX < GameContext_Map->width && mapY >= 0 &&
           (uint16_t) mapY < GameContext_Map->height) {
        hittedWall = GameContext_Map->walls[mapX + mapY * GameContext_Map->width];

        if (hittedWall->wallTexture != WALL_NULL) {
            resultTexture = hittedWall->wallTexture;

            *distance = (float) (*side
                                 ? fabs((mapY - rayPosY + (1.0 - stepY) / 2.0) / rayDirY)
                                 : fabs((mapX - rayPosX + (1.0 - stepX) / 2.0) / rayDirX));
            *hitPerc = *side ? rayPosX + rayDirX * (*distance) : rayPosY + rayDirY * (*distance);
            *hitPerc = DECIMAL(*hitPerc);

            if (currentlyThoughDoor) {
                switch (*side) {
                    case 1:
                        if (mapY + 1 < GameContext_Map->height &&
                            GameContext_Map->walls[mapX + (mapY + 1) * GameContext_Map->width]->isDoor ||
                            mapY - 1 >= 0 &&
                            GameContext_Map->walls[mapX + (mapY - 1) * GameContext_Map->width]->isDoor) {
                            resultTexture = WALL_DOORSLOT1;
                        }
                        break;
                    case 0:
                        if (mapX + 1 < GameContext_Map->width &&
                            GameContext_Map->walls[mapX + 1 + mapY * GameContext_Map->width]->isDoor ||
                            mapX - 1 >= 0 && GameContext_Map->walls[mapX - 1 + mapY * GameContext_Map->width]->isDoor) {
                            resultTexture = WALL_DOORSLOT1;
                        }
                        break;
                }
            }

            if (!hittedWall->isDoor)
                break;

            if (iterationCount != 0) {
                // the extencion of the ray projected from the player
                float raycastExtencion = ABS(*side ?
                                             DOOR_RECESS / rayDirY :
                                             DOOR_RECESS / rayDirX);
                *distance += raycastExtencion;

                // the extencion of the hit point based on the raycast extencion
                float hitExtencion = *side ?
                                     raycastExtencion * rayDirX :
                                     raycastExtencion * rayDirY;
                *hitPerc += hitExtencion;

                if (*hitPerc < 0 || *hitPerc > 1) {
                    // door side
                    *side = !*side;

                    float outsideHitpoint = (*hitPerc < 1) ? *hitPerc : (1 - *hitPerc);

                    float distanceToHitpointFromWall = *side ?
                                                       outsideHitpoint / rayDirY :
                                                       outsideHitpoint / rayDirX;
                    *distance -= ABS(distanceToHitpointFromWall);

                    *hitPerc = *side ?
                               DOOR_RECESS - distanceToHitpointFromWall * rayDirX :
                               DOOR_RECESS - distanceToHitpointFromWall * rayDirY;

                    resultTexture = WALL_DOORSLOT1;
                    break;
                }

                if (hittedWall->openState == WALL_OPEN_STATE_CLOSED)
                    break;

                if (*hitPerc > hittedWall->openPercentage) {
                    *hitPerc -= hittedWall->openPercentage;
                    break;
                }
            }

            // raycast continues if the ray hits the door but the door is open in that point
            currentlyThoughDoor = true;
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

        if (++iterationCount > 1000) {
            TRT_error("Game_raycast", "Infinite loop detected", true);
            break;
        }
    }

    *distance = ABS(*distance) * cosf(angle - GameContext_Map->player->lookingAngle);
    return resultTexture;
}
