//
// Created by Marco on 29/07/2024.
//

#include "Entity.h"

Entity *Entity_get(FILE *fp) {
    Entity *entity = malloc(sizeof(Entity));
    if (entity == NULL) {
        exit(EXIT_FAILURE);
    }

    fread(&entity->position, sizeof(Vec2), 1, fp);
    fread(&entity->lookingAngle, sizeof(float), 1, fp);

    uint64_t textureNameLength;
    fread(&textureNameLength, sizeof(uint64_t), 1, fp);
    fread(entity->textureName, sizeof(char), textureNameLength, fp);

    entity->texture = TRT_image_get(entity->textureName);

    fread(&entity->height, sizeof(uint8_t), 1, fp);
    fread(&entity->moveSpeed, sizeof(float), 1, fp);
    fread(&entity->rotationSpeed, sizeof(float), 1, fp);
    fread(&entity->health, sizeof(float), 1, fp);
    fread(&entity->armor, sizeof(float), 1, fp);
    fread(&entity->ammo, sizeof(uint16_t), 1, fp);
    fread(&entity->points, sizeof(uint32_t), 1, fp);

    return entity;
}

Entity **Entity_getMultiple(char* path, uint32_t* enemiesCount) {
    uint32_t count = 0;

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fread(&count, sizeof(uint32_t), 1, fp);

    Entity **entities = malloc(count * sizeof(Entity*));
    if (entities == NULL) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        entities[i] = malloc(sizeof(Entity));

        fread(&entities[i]->position, sizeof(Vec2), 1, fp);
        fread(&entities[i]->lookingAngle, sizeof(float), 1, fp);
        fread(&entities[i]->height, sizeof(uint8_t), 1, fp);
        fread(&entities[i]->moveSpeed, sizeof(float), 1, fp);
        fread(&entities[i]->rotationSpeed, sizeof(float), 1, fp);
        fread(&entities[i]->health, sizeof(float), 1, fp);
        fread(&entities[i]->armor, sizeof(float), 1, fp);
        fread(&entities[i]->ammo, sizeof(uint16_t), 1, fp);
        fread(&entities[i]->points, sizeof(uint32_t), 1, fp);
    }

    fclose(fp);

    if (enemiesCount != NULL) {
        *enemiesCount = count;
    }

    return entities;
}

void *Entity_save(FILE *fp, Entity *entity) {
    fwrite(&entity->position, sizeof(Vec2), 1, fp);
    fwrite(&entity->lookingAngle, sizeof(float), 1, fp);

    uint64_t len = strlen(entity->textureName);
    fwrite(&len, sizeof(uint64_t), 1, fp);
    fwrite(entity->textureName, sizeof(char), len, fp);

    fwrite(&entity->height, sizeof(uint8_t), 1, fp);
    fwrite(&entity->moveSpeed, sizeof(float), 1, fp);
    fwrite(&entity->rotationSpeed, sizeof(float), 1, fp);
    fwrite(&entity->health, sizeof(float), 1, fp);
    fwrite(&entity->armor, sizeof(float), 1, fp);
    fwrite(&entity->ammo, sizeof(uint16_t), 1, fp);
    fwrite(&entity->points, sizeof(uint32_t), 1, fp);
}

void Entity_free(Entity *entity) {
    TRT_image_free(entity->texture);
    free(entity);
}

void Entity_freeMultiple(Entity **entities, uint32_t enemiesCount) {
    for (int i = 0; i < enemiesCount; i++) {
        if (entities[i] != NULL) {
            Entity_free(entities[i]);
        }
    }

    free(entities);
}