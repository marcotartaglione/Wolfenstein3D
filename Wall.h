//
// Created by Marco on 30/07/2024.
//

#ifndef WOLFENSTEIN3D_WALL_H
#define WOLFENSTEIN3D_WALL_H

#include "TRT_Engine.h"

typedef enum {
    //
    // Every tile has the dark version next to it
    //
    WALL_BLUESTONE1, WALL_BLUESTONE2,
    WALL_BLUESTONE3, WALL_BLUESTONE4,
    WALL_BLUESTONE5, WALL_BLUESTONE6,
    WALL_BLUESTONE7, WALL_BLUESTONE8,
    WALL_BLUESTONE9, WALL_BLUESTONE10,

    WALL_BLUEWALL1, WALL_BLUEWALL2,
    WALL_BLUEWALL3, WALL_BLUEWALL4,
    WALL_BLUEWALL5, WALL_BLUEWALL6,

    WALL_BRICK1, WALL_BRICK2,
    WALL_BRICK3, WALL_BRICK4,
    WALL_BRICK5, WALL_BRICK6,
    WALL_BRICK7, WALL_BRICK8,

    WALL_BROWNSTONE1, WALL_BROWNSTONE2,
    WALL_BROWNSTONE3, WALL_BROWNSTONE4,
    WALL_BROWNSTONE5, WALL_BROWNSTONE6,
    WALL_BROWNSTONE7, WALL_BROWNSTONE8,
    WALL_BROWNSTONE9, WALL_BROWNSTONE10,

    WALL_CAVE1, WALL_CAVE2,
    WALL_CAVE3, WALL_CAVE4,
    WALL_CAVE5, WALL_CAVE6,
    WALL_CAVE7, WALL_CAVE8,

    WALL_DIRTYSTONE1, WALL_DIRTYSTONE2,
    WALL_DIRTYSTONE3, WALL_DIRTYSTONE4,

    WALL_DOOR1, WALL_DOOR2,
    WALL_DOOR3, WALL_DOOR4,
    WALL_DOOR5, WALL_DOOR6,
    WALL_DOOR7, WALL_DOOR8,

    WALL_DOORSLOT1, WALL_DOORSLOT2,

    WALL_GREYBRICK1, WALL_GREYBRICK2,
    WALL_GREYBRICK3, WALL_GREYBRICK4,
    WALL_GREYBRICK5, WALL_GREYBRICK6,
    WALL_GREYBRICK7, WALL_GREYBRICK8,
    WALL_GREYBRICK9, WALL_GREYBRICK10,

    WALL_HELL1, WALL_HELL2,

    WALL_MARBLE1, WALL_MARBLE2,
    WALL_MARBLE3, WALL_MARBLE4,
    WALL_MARBLE5, WALL_MARBLE6,

    WALL_METAL1, WALL_METAL2,
    WALL_METAL3, WALL_METAL4,

    WALL_PURPLE1, WALL_PURPLE2,
    WALL_PURPLE3, WALL_PURPLE4,

    WALL_PURPLEBRICK1, WALL_PURPLEBRICK2,

    WALL_SGLASS1, WALL_SGLASS2,

    WALL_STONE1, WALL_STONE2,
    WALL_STONE3, WALL_STONE4,
    WALL_STONE5, WALL_STONE6,
    WALL_STONE7, WALL_STONE8,
    WALL_STONE9, WALL_STONE10,
    WALL_STONE11, WALL_STONE12,
    WALL_STONE13, WALL_STONE14,
    WALL_STONE15, WALL_STONE16,
    WALL_STONE17, WALL_STONE18,
    WALL_STONE19, WALL_STONE20,
    WALL_STONE21, WALL_STONE22,
    WALL_STONE23, WALL_STONE24,
    WALL_STONE25, WALL_STONE26,

    WALL_WOOD1, WALL_WOOD2,
    WALL_WOOD3, WALL_WOOD4,
    WALL_WOOD5, WALL_WOOD6,
    WALL_WOOD7, WALL_WOOD8,
    WALL_WOOD9, WALL_WOOD10,

    //
    // These tiles have no dark version
    //
    WALL_ELEVATOR1,
    WALL_ELEVATOR2, WALL_ELEVATOR3,
    WALL_ELEVATOR4,
    WALL_ELEVATOR5,
    WALL_NULL,
} WallTexture;

typedef enum {
    WALL_OPEN_STATE_CLOSED,
    WALL_OPEN_STATE_OPEN,
    WALL_OPEN_STATE_OPENING,
    WALL_OPEN_STATE_CLOSING,
} WallOpenState;

typedef struct {
    WallTexture wallTexture;

    bool isDoor;
    bool isSecret;
    bool isElevator;

    float openPercentage;
    float openTime;

    WallOpenState openState;
} WallData;

#define WALL_NUMBER                 (WALL_NULL)
#define WALL_NO_DARK_VERSION        (WALL_ELEVATOR1)
#define DOOR_RECESS                 (0.25f)

#define WALL_IS_DOOR(wall)          (wall >= WALL_DOOR1 && wall <= WALL_DOOR8)

const char* Wall_toString(WallTexture wall);

WallData *WallData_get(FILE *fp);

void WallData_save(FILE *fp, WallData *wall);

void WallData_free(WallData *wall);

extern Image *wallTextures[WALL_NUMBER];

#endif //WOLFENSTEIN3D_WALL_H
