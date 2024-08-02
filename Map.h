//
// Created by Marco on 29/07/2024.
//

#ifndef WOLFENSTEIN3D_MAP_H
#define WOLFENSTEIN3D_MAP_H

#include "TRT_Engine.h"
#include "Commons.h"
#include "Entity.h"
#include "Wall.h"

typedef struct {
    uint16_t width;
    uint16_t height;
    Wall *walls;

    Entity *player;

    uint32_t enemiesCountPerDifficulty[DIFFICULTY_COUNT];
    // Es: enemiesCount per difficulty = { 4, 4, 7 }
    // The first 4 enemies are associated with difficulty 0: enemies[0-3]
    // The first 4 enimies and the next 4 enemies are associated with difficulty 1: enemies[0-7]
    // The first 4, second 4 and last 7 enemies are for difficulty 2: enemies[0-14]

    Entity **enemies;
} Map;

Map *Map_get(FILE *fp);

void Map_save(FILE *fp, Map *map);

void Map_free(Map *map);

#endif //WOLFENSTEIN3D_MAP_H
