//
// Created by Marco on 29/07/2024.
//

#include "Map.h"

Map *Map_get(FILE *fp) {
    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        exit(EXIT_FAILURE);
    }

    fread(&map->width, sizeof(uint16_t), 1, fp);
    fread(&map->height, sizeof(uint16_t), 1, fp);

    map->walls = malloc(sizeof(Wall) * map->width * map->height);
    if (map->walls == NULL) {
        free(map);
        exit(EXIT_FAILURE);
    }

    fread(map->walls, sizeof(Wall), map->width * map->height, fp);
    fread(&map->enemiesCount, sizeof(uint32_t), 1, fp);

    map->player = Entity_get(fp);
    if (map->player == NULL) {
        free(map->walls);
        free(map);
        exit(EXIT_FAILURE);
    }

    map->enemies = malloc(sizeof(Entity *) * map->enemiesCount);
    if (map->enemies == NULL) {
        Entity_free(map->player);
        free(map->walls);
        free(map);
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < map->enemiesCount; ++i) {
        map->enemies[i] = Entity_get(fp);
        if (map->enemies[i] == NULL) {
            for (uint32_t j = 0; j < i; ++j) {
                Entity_free(map->enemies[j]);
            }
            free(map->enemies);
            Entity_free(map->player);
            free(map->walls);
            free(map);
            exit(EXIT_FAILURE);
        }
    }

    return map;
}

void Map_save(FILE *fp, Map *map) {
    fwrite(&map->width, sizeof(uint16_t), 1, fp);
    fwrite(&map->height, sizeof(uint16_t), 1, fp);

    fwrite(map->walls, sizeof(Wall), map->width * map->height, fp);
    fwrite(&map->enemiesCount, sizeof(uint32_t), 1, fp);

    Entity_save(fp, map->player);

    for (uint32_t i = 0; i < map->enemiesCount; ++i) {
        Entity_save(fp, map->enemies[i]);
    }
}

void Map_free(Map *map) {
    free(map->walls);
    Entity_free(map->player);
    Entity_freeMultiple(map->enemies, map->enemiesCount);
    free(map);
}