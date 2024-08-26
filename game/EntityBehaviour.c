//
// Created by Marco on 02/08/2024.
//

#include "EntityBehaviour.h"
#include <math.h>

void EntityBehaviour_move(Entity* entity, Map* map, EntityBehaviour_Movement movement) {
    const float dX = cosf(entity->lookingAngle) * entity->moveSpeed * (movement == ENTITY_MOVE_BACKWARD ? -1 : 1);
    const float dY = sinf(entity->lookingAngle) * entity->moveSpeed * (movement == ENTITY_MOVE_BACKWARD ? -1 : 1);
    const float newX = entity->position.x + dX;
    const float newY = entity->position.y + dY;
    const float checkX = newX + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dX);
    const float checkY = newY + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dY);

    int32_t wallIndex = (int32_t)checkX + (int32_t)checkY * map->width;
    if (map->walls[wallIndex]->wallTexture != WALL_NULL)
        return;

    entity->position.x = newX;
    entity->position.y = newY;
}

void EntityBehaviour_rotate(Entity* entity, EntityBehaviour_Rotation rotation) {
    entity->lookingAngle = rotation == ENTITY_ROTATE_LEFT ?
        entity->lookingAngle - entity->rotationSpeed :
        entity->lookingAngle + entity->rotationSpeed;
}