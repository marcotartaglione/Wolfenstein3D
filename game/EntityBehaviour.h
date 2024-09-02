//
// Created by Marco on 02/08/2024.
//

#ifndef ENTITYMOVEMENT_H
#define ENTITYMOVEMENT_H

#include "TRT_Engine.h"
#include "../Entity.h"
#include "../Map.h"

#define ENTITY_MIN_DISTANCE_FROM_WALL (0.2f)

typedef enum {
    ENTITY_ROTATE_LEFT,
    ENTITY_ROTATE_RIGHT
} EntityBehaviour_Rotation;

typedef enum {
    ENTITY_MOVE_FORWARD,
    ENTITY_MOVE_BACKWARD
} EntityBehaviour_Movement;

void EntityBehaviour_move(Entity* entity, Map* map, EntityBehaviour_Movement movement);

void EntityBehaviour_rotate(Entity* entity, EntityBehaviour_Rotation rotation);

typedef struct {
    bool success;
    uint32_t wallIndex;
} EntityBehaviour_InteractResult;

EntityBehaviour_InteractResult EntityBehaviour_interact(Entity* entity, Map* map);

#endif //ENTITYMOVEMENT_H
