//
// Created by Marco on 29/07/2024.
//

#ifndef WOLFENSTEIN3D_ENTITY_H
#define WOLFENSTEIN3D_ENTITY_H

#include "TRT_Engine.h"

typedef struct {
    //
    // Position in space
    //
    Vec2 position;
    float lookingAngle;

    //
    // Caracteristics
    //
    char textureName[512];

    Image* texture;
    uint8_t height;

    //
    // Movement
    //
    float moveSpeed;
    float rotationSpeed;

    //
    // Statistics
    //
    float health;
    float armor;
    uint16_t ammo;
    uint32_t points;
} Entity;

Entity *Entity_get(FILE *fp);

void *Entity_save(FILE *fp, Entity *entity);

void Entity_free(Entity *entity);

#endif //WOLFENSTEIN3D_ENTITY_H
