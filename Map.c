//
// Created by Marco on 29/07/2024.
//

#include "Map.h"

Map *Map_get(FILE *fp) {
    if (fp == NULL) {
        TRT_error("Map_get", "File pointer is NULL", true);
        return NULL;
    }

    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        TRT_error("Map_get", "Malloc failed for map", true);
        return NULL;
    }

    fread(&map->width, sizeof(uint16_t), 1, fp);
    fread(&map->height, sizeof(uint16_t), 1, fp);

    map->walls = malloc(sizeof(WallData) * map->width * map->height);
    if (map->walls == NULL) {
        free(map);
        TRT_error("Map_get", "Malloc failed for map walls", true);
        return NULL;
    }

    for (uint32_t i = 0; i < map->width * map->height; ++i) {
        map->walls[i] = WallData_get(fp);
    }

    map->player = Entity_get(fp);
    if (map->player == NULL) {
        free(map->walls);
        free(map);
        TRT_error("Map_get", "Could not retreave player data", true);
        return NULL;
    }

    fread(map->enemiesCountPerDifficulty, sizeof(uint32_t), DIFFICULTY_COUNT, fp);

    uint32_t totalEnemies = 0;
    for (uint8_t i = 0; i < DIFFICULTY_COUNT; i++) totalEnemies += map->enemiesCountPerDifficulty[i];

    map->enemies = malloc(sizeof(Entity *) * totalEnemies);
    if (map->enemies == NULL) {
        Entity_free(map->player);
        free(map->walls);
        free(map);
        TRT_error("Map_get", "Malloc failed for map enemies", true);
        return NULL;
    }

    for (uint32_t i = 0; i < totalEnemies; ++i) {
        map->enemies[i] = Entity_get(fp);
        if (map->enemies[i] == NULL) {
            for (uint32_t j = 0; j < i; ++j) {
                Entity_free(map->enemies[j]);
            }
            free(map->enemies);
            Entity_free(map->player);
            free(map->walls);
            free(map);
            TRT_error("Map_get", "Could not retreave map enemies", true);
            return NULL;
        }
    }

    return map;
}

void Map_save(FILE *fp, Map *map) {
    if (fp == NULL) {
        TRT_error("Map_save", "File pointer is NULL", true);
        return;
    }
    if (map == NULL) {
        TRT_error("Map_save", "Map is NULL", true);
        return;
    }

    fwrite(&map->width, sizeof(uint16_t), 1, fp);
    fwrite(&map->height, sizeof(uint16_t), 1, fp);

    for (uint32_t i = 0; i < map->width * map->height; ++i) {
        WallData_save(fp, map->walls[i]);
    }

    Entity_save(fp, map->player);

    fwrite(map->enemiesCountPerDifficulty, sizeof(uint32_t), DIFFICULTY_COUNT, fp);

    uint32_t totalEnemies = 0;
    for(uint8_t i = 0; i < DIFFICULTY_COUNT; i++) totalEnemies += map->enemiesCountPerDifficulty[i];

    for (uint32_t i = 0; i < totalEnemies; ++i) {
        Entity_save(fp, map->enemies[i]);
    }
}

void Map_free(Map *map) {
    if (map == NULL) {
        TRT_error("Map_free", "Map is NULL", false);
        return;
    }

    for (uint32_t i = 0; i < map->width * map->height; ++i) {
        WallData_free(map->walls[i]);
    }

    free(map->walls);
    Entity_free(map->player);

    uint32_t totalEnemies = 0;
    for(uint8_t i = 0; i < DIFFICULTY_COUNT; i++) totalEnemies += map->enemiesCountPerDifficulty[i];

    for(uint32_t i = 0; i < totalEnemies; ++i) {
        Entity_free(map->enemies[i]);
    }

    free(map);
}