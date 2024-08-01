//
// Created by Marco on 29/07/2024.
//

#include "Entity.h"
#include <stdio.h>

Entity *Entity_get(FILE *fp) {
    if (fp == NULL) {
        TRT_error("Entity_get", "File pointer is NULL", true);
        return NULL;
    }

    Entity *entity = malloc(sizeof(Entity));
    if (entity == NULL) {
        TRT_error("Entity_get", "Could not allocate memory for entity", true);
        return NULL;
    }

    fread(&entity->position, sizeof(Vec2), 1, fp);
    fread(&entity->lookingAngle, sizeof(float), 1, fp);

    uint64_t textureNameLength;
    fread(&textureNameLength, sizeof(uint64_t), 1, fp);
    fread_s(entity->textureName, ENTITY_TEXTURENAME_LEN, sizeof(char), textureNameLength, fp);
    entity->textureName[textureNameLength] = '\0';

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

void Entity_save(FILE *fp, Entity *entity) {
    if (fp == NULL) {
        TRT_error("Entity_save", "File pointer is NULL", true);
        return;
    }
    if (entity == NULL) {
        TRT_error("Entity_save", "Entity is NULL", true);
        return;
    }

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
    if (entity == NULL) {
        TRT_error("Entity_free", "Entity is NULL", true);
        return;
    }

    if (entity->texture == NULL) {
        TRT_error("Entity_free", "NULL value found in the entity texture. This should not happen", true);
    } else {
        TRT_image_free(entity->texture);
    }

    free(entity);
}