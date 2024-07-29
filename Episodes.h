//
// Created by Marco on 29/07/2024.
//
#ifndef WOLFENSTEIN3D_LEVEL_H
#define WOLFENSTEIN3D_LEVEL_H

#include "TRT_Engine.h"
#include "Commons.h"

typedef struct Episode {
    char title[512];
    uint64_t titleLength;
    Image *thumbnail;
} Episode;

Episode *Episode_get(char* path);

extern Episode *episodes[20];

#endif //WOLFENSTEIN3D_LEVEL_H
