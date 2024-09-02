//
// Created by Marco on 02/08/2024.
//

#include "EntityBehaviour.h"
#include <math.h>

void EntityBehaviour_move(Entity* entity, Map* map, EntityBehaviour_Movement movement) {
    const float dX = cosf(entity->lookingAngle) * entity->moveSpeed * (movement == ENTITY_MOVE_BACKWARD ? -1.0f : 1.0f);
    const float dY = sinf(entity->lookingAngle) * entity->moveSpeed * (movement == ENTITY_MOVE_BACKWARD ? -1.0f : 1.0f);
    const float newX = entity->position.x + dX;
    const float newY = entity->position.y + dY;
    const float checkX = newX + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dX);
    const float checkY = newY + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dY);

    int32_t wallIndex = (int32_t)checkX + (int32_t)checkY * map->width;
    if ((map->walls[wallIndex]->isDoor && map->walls[wallIndex]->openState != WALL_OPEN_STATE_OPEN) ||
    (!map->walls[wallIndex]->isDoor && map->walls[wallIndex]->wallTexture != WALL_NULL))
        return;

    entity->position.x = newX;
    entity->position.y = newY;
}

void EntityBehaviour_rotate(Entity* entity, EntityBehaviour_Rotation rotation) {
    entity->lookingAngle = rotation == ENTITY_ROTATE_LEFT ?
        entity->lookingAngle - entity->rotationSpeed :
        entity->lookingAngle + entity->rotationSpeed;
}

EntityBehaviour_InteractResult EntityBehaviour_interact(Entity* entity, Map* map) {
    const float dX = cosf(entity->lookingAngle);
    const float dY = sinf(entity->lookingAngle);
    const float checkX = entity->position.x + dX + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dX);
    const float checkY = entity->position.y + dY + ENTITY_MIN_DISTANCE_FROM_WALL * SIGN(dY);

    int32_t wallIndex = (int32_t)checkX + (int32_t)checkY * map->width;
    if (map->walls[wallIndex]->isDoor == false)
        return (EntityBehaviour_InteractResult){ .success = false, .wallIndex = 0 };

    if (map->walls[wallIndex]->openState != WALL_OPEN_STATE_CLOSED)
        return (EntityBehaviour_InteractResult){ .success = false, .wallIndex = 0 };

    map->walls[wallIndex]->openState = WALL_OPEN_STATE_OPENING;
    map->walls[wallIndex]->openPercentage = 0;
    map->walls[wallIndex]->elapsedFrames = 0;
    return (EntityBehaviour_InteractResult){ .success = true, .wallIndex = wallIndex };
}