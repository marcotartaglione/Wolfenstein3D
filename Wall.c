//
// Created by Marco on 30/07/2024.
//

#include "Wall.h"

Image *wallTextures[WALL_NUMBER];

const char *Wall_toString(WallTexture wall) {
    // If you are reading this, I'm sorry
    switch (wall) {
        case WALL_BLUESTONE1:
            return "WALL_BLUESTONE1";
        case WALL_BLUESTONE2:
            return "WALL_BLUESTONE2";
        case WALL_BLUESTONE3:
            return "WALL_BLUESTONE3";
        case WALL_BLUESTONE4:
            return "WALL_BLUESTONE4";
        case WALL_BLUESTONE5:
            return "WALL_BLUESTONE5";
        case WALL_BLUESTONE6:
            return "WALL_BLUESTONE6";
        case WALL_BLUESTONE7:
            return "WALL_BLUESTONE7";
        case WALL_BLUESTONE8:
            return "WALL_BLUESTONE8";
        case WALL_BLUESTONE9:
            return "WALL_BLUESTONE9";
        case WALL_BLUESTONE10:
            return "WALL_BLUESTONE10";
        case WALL_BLUEWALL1:
            return "WALL_BLUEWALL1";
        case WALL_BLUEWALL2:
            return "WALL_BLUEWALL2";
        case WALL_BLUEWALL3:
            return "WALL_BLUEWALL3";
        case WALL_BLUEWALL4:
            return "WALL_BLUEWALL4";
        case WALL_BLUEWALL5:
            return "WALL_BLUEWALL5";
        case WALL_BLUEWALL6:
            return "WALL_BLUEWALL6";
        case WALL_BRICK1:
            return "WALL_BRICK1";
        case WALL_BRICK2:
            return "WALL_BRICK2";
        case WALL_BRICK3:
            return "WALL_BRICK3";
        case WALL_BRICK4:
            return "WALL_BRICK4";
        case WALL_BRICK5:
            return "WALL_BRICK5";
        case WALL_BRICK6:
            return "WALL_BRICK6";
        case WALL_BRICK7:
            return "WALL_BRICK7";
        case WALL_BRICK8:
            return "WALL_BRICK8";
        case WALL_BROWNSTONE1:
            return "WALL_BROWNSTONE1";
        case WALL_BROWNSTONE2:
            return "WALL_BROWNSTONE2";
        case WALL_BROWNSTONE3:
            return "WALL_BROWNSTONE3";
        case WALL_BROWNSTONE4:
            return "WALL_BROWNSTONE4";
        case WALL_BROWNSTONE5:
            return "WALL_BROWNSTONE5";
        case WALL_BROWNSTONE6:
            return "WALL_BROWNSTONE6";
        case WALL_BROWNSTONE7:
            return "WALL_BROWNSTONE7";
        case WALL_BROWNSTONE8:
            return "WALL_BROWNSTONE8";
        case WALL_BROWNSTONE9:
            return "WALL_BROWNSTONE9";
        case WALL_BROWNSTONE10:
            return "WALL_BROWNSTONE10";
        case WALL_CAVE1:
            return "WALL_CAVE1";
        case WALL_CAVE2:
            return "WALL_CAVE2";
        case WALL_CAVE3:
            return "WALL_CAVE3";
        case WALL_CAVE4:
            return "WALL_CAVE4";
        case WALL_CAVE5:
            return "WALL_CAVE5";
        case WALL_CAVE6:
            return "WALL_CAVE6";
        case WALL_CAVE7:
            return "WALL_CAVE7";
        case WALL_CAVE8:
            return "WALL_CAVE8";
        case WALL_DIRTYSTONE1:
            return "WALL_DIRTYSTONE1";
        case WALL_DIRTYSTONE2:
            return "WALL_DIRTYSTONE2";
        case WALL_DIRTYSTONE3:
            return "WALL_DIRTYSTONE3";
        case WALL_DIRTYSTONE4:
            return "WALL_DIRTYSTONE4";
        case WALL_DOOR1:
            return "WALL_DOOR1";
        case WALL_DOOR2:
            return "WALL_DOOR2";
        case WALL_DOOR3:
            return "WALL_DOOR3";
        case WALL_DOOR4:
            return "WALL_DOOR4";
        case WALL_DOOR5:
            return "WALL_DOOR5";
        case WALL_DOOR6:
            return "WALL_DOOR6";
        case WALL_DOOR7:
            return "WALL_DOOR7";
        case WALL_DOOR8:
            return "WALL_DOOR8";
        case WALL_DOORSLOT1:
            return "WALL_DOORSLOT1";
        case WALL_DOORSLOT2:
            return "WALL_DOORSLOT2";
        case WALL_GREYBRICK1:
            return "WALL_GREYBRICK1";
        case WALL_GREYBRICK2:
            return "WALL_GREYBRICK2";
        case WALL_GREYBRICK3:
            return "WALL_GREYBRICK3";
        case WALL_GREYBRICK4:
            return "WALL_GREYBRICK4";
        case WALL_GREYBRICK5:
            return "WALL_GREYBRICK5";
        case WALL_GREYBRICK6:
            return "WALL_GREYBRICK6";
        case WALL_GREYBRICK7:
            return "WALL_GREYBRICK7";
        case WALL_GREYBRICK8:
            return "WALL_GREYBRICK8";
        case WALL_GREYBRICK9:
            return "WALL_GREYBRICK9";
        case WALL_GREYBRICK10:
            return "WALL_GREYBRICK10";
        case WALL_HELL1:
            return "WALL_HELL1";
        case WALL_HELL2:
            return "WALL_HELL2";
        case WALL_MARBLE1:
            return "WALL_MARBLE1";
        case WALL_MARBLE2:
            return "WALL_MARBLE2";
        case WALL_MARBLE3:
            return "WALL_MARBLE3";
        case WALL_MARBLE4:
            return "WALL_MARBLE4";
        case WALL_MARBLE5:
            return "WALL_MARBLE5";
        case WALL_MARBLE6:
            return "WALL_MARBLE6";
        case WALL_METAL1:
            return "WALL_METAL1";
        case WALL_METAL2:
            return "WALL_METAL2";
        case WALL_METAL3:
            return "WALL_METAL3";
        case WALL_METAL4:
            return "WALL_METAL4";
        case WALL_PURPLE1:
            return "WALL_PURPLE1";
        case WALL_PURPLE2:
            return "WALL_PURPLE2";
        case WALL_PURPLE3:
            return "WALL_PURPLE3";
        case WALL_PURPLE4:
            return "WALL_PURPLE4";
        case WALL_PURPLEBRICK1:
            return "WALL_PURPLEBRICK1";
        case WALL_PURPLEBRICK2:
            return "WALL_PURPLEBRICK2";
        case WALL_SGLASS1:
            return "WALL_SGLASS1";
        case WALL_SGLASS2:
            return "WALL_SGLASS2";
        case WALL_STONE1:
            return "WALL_STONE1";
        case WALL_STONE2:
            return "WALL_STONE2";
        case WALL_STONE3:
            return "WALL_STONE3";
        case WALL_STONE4:
            return "WALL_STONE4";
        case WALL_STONE5:
            return "WALL_STONE5";
        case WALL_STONE6:
            return "WALL_STONE6";
        case WALL_STONE7:
            return "WALL_STONE7";
        case WALL_STONE8:
            return "WALL_STONE8";
        case WALL_STONE9:
            return "WALL_STONE9";
        case WALL_STONE10:
            return "WALL_STONE10";
        case WALL_STONE11:
            return "WALL_STONE11";
        case WALL_STONE12:
            return "WALL_STONE12";
        case WALL_STONE13:
            return "WALL_STONE13";
        case WALL_STONE14:
            return "WALL_STONE14";
        case WALL_STONE15:
            return "WALL_STONE15";
        case WALL_STONE16:
            return "WALL_STONE16";
        case WALL_STONE17:
            return "WALL_STONE17";
        case WALL_STONE18:
            return "WALL_STONE18";
        case WALL_STONE19:
            return "WALL_STONE19";
        case WALL_STONE20:
            return "WALL_STONE20";
        case WALL_STONE21:
            return "WALL_STONE21";
        case WALL_STONE22:
            return "WALL_STONE22";
        case WALL_STONE23:
            return "WALL_STONE23";
        case WALL_STONE24:
            return "WALL_STONE24";
        case WALL_STONE25:
            return "WALL_STONE25";
        case WALL_STONE26:
            return "WALL_STONE26";
        case WALL_WOOD1:
            return "WALL_WOOD1";
        case WALL_WOOD2:
            return "WALL_WOOD2";
        case WALL_WOOD3:
            return "WALL_WOOD3";
        case WALL_WOOD4:
            return "WALL_WOOD4";
        case WALL_WOOD5:
            return "WALL_WOOD5";
        case WALL_WOOD6:
            return "WALL_WOOD6";
        case WALL_WOOD7:
            return "WALL_WOOD7";
        case WALL_WOOD8:
            return "WALL_WOOD8";
        case WALL_WOOD9:
            return "WALL_WOOD9";
        case WALL_WOOD10:
            return "WALL_WOOD10";
        case WALL_ELEVATOR1:
            return "WALL_ELEVATOR1";
        case WALL_ELEVATOR2:
            return "WALL_ELEVATOR2";
        case WALL_ELEVATOR3:
            return "WALL_ELEVATOR3";
        case WALL_ELEVATOR4:
            return "WALL_ELEVATOR4";
        case WALL_ELEVATOR5:
            return "WALL_ELEVATOR5";
        case WALL_NULL:
            return "WALL_NULL";
        default:
            TRT_error("Unknown wallTexture type", "Inserted wallTexture type is unknown", false);
            return "UNKNOWN";
    }
}

WallData *WallData_get(FILE *fp) {
    if (fp == NULL) {
        TRT_error("WallData_get", "File pointer is NULL", true);
        return NULL;
    }

    WallData *wall = malloc(sizeof(WallData));
    if (wall == NULL) {
        TRT_error("WallData_get", "Malloc failed for wall", true);
        return NULL;
    }

    fread(&wall->wallTexture, sizeof(WallTexture), 1, fp);
    fread(&wall->position, sizeof(Vec2), 1, fp);
    fread(&wall->isDoor, sizeof(bool), 1, fp);
    fread(&wall->isSecret, sizeof(bool), 1, fp);
    fread(&wall->isElevator, sizeof(bool), 1, fp);
    fread(&wall->openPercentage, sizeof(float), 1, fp);
    fread(&wall->openTimeSeconds, sizeof(float), 1, fp);
    fread(&wall->elapsedFrames, sizeof(uint32_t), 1, fp);
    fread(&wall->openState, sizeof(WallOpenState), 1, fp);

    return wall;
}

void WallData_save(FILE *fp, WallData *wall) {
    if (fp == NULL) {
        TRT_error("WallData_save", "File pointer is NULL", true);
        return;
    }

    if (wall == NULL) {
        TRT_error("WallData_save", "WallData is NULL", true);
        return;
    }

    fwrite(&wall->wallTexture, sizeof(WallTexture), 1, fp);
    fwrite(&wall->position, sizeof(Vec2), 1, fp);
    fwrite(&wall->isDoor, sizeof(bool), 1, fp);
    fwrite(&wall->isSecret, sizeof(bool), 1, fp);
    fwrite(&wall->isElevator, sizeof(bool), 1, fp);
    fwrite(&wall->openPercentage, sizeof(float), 1, fp);
    fwrite(&wall->openTimeSeconds, sizeof(float), 1, fp);
    fwrite(&wall->elapsedFrames, sizeof(uint32_t), 1, fp);
    fwrite(&wall->openState, sizeof(WallOpenState), 1, fp);
}

void WallData_free(WallData *wall) {
    if (wall == NULL) {
        TRT_error("WallData_free", "WallData is NULL", false);
        return;
    }

    free(wall);
}