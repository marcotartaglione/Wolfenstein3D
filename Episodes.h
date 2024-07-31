//
// Created by Marco on 29/07/2024.
//
#ifndef WOLFENSTEIN3D_LEVEL_H
#define WOLFENSTEIN3D_LEVEL_H

#include "TRT_Engine.h"
#include "Commons.h"
#include "Map.h"

#define EPISODE_N_FLOORS 10

typedef struct Episode {
    char title[512];
    char thumbnailPath[512];
    Image *thumbnail;
    Map **floors;
} Episode;

Episode *Episode_get(FILE *fp);

void Episode_save(FILE *fp, Episode *episode);

void Episode_free(Episode *episode);

extern Episode *episodes[EPISODES_COUNT];

#endif //WOLFENSTEIN3D_LEVEL_H
