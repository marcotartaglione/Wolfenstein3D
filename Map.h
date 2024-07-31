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

    uint32_t enemiesCount;
    Entity *player;
    Entity **enemies;
} Map;

Map *Map_get(FILE *fp);

void Map_save(FILE *fp, Map *map);

void Map_free(Map *map);

#endif //WOLFENSTEIN3D_MAP_H
