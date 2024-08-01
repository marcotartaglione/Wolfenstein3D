//
// Created by Marco on 29/07/2024.
//
#ifndef WOLFENSTEIN3D_LEVEL_H
#define WOLFENSTEIN3D_LEVEL_H

#include "TRT_Engine.h"
#include "Commons.h"
#include "Map.h"

#define EPISODE_TITLE_LEN           (512)
#define EPISODE_THUMBNAILPATH_LEN   (512)
#define EPISODE_N_FLOORS            (10)

typedef struct Episode {
    char title[EPISODE_TITLE_LEN];
    char thumbnailPath[EPISODE_THUMBNAILPATH_LEN];
    Image *thumbnail;
    Map **floors;
} Episode;

Episode *Episode_get(FILE *fp);

void Episode_save(FILE *fp, Episode *episode);

void Episode_free(Episode *episode);

extern Episode *episodes[EPISODES_COUNT];

#endif //WOLFENSTEIN3D_LEVEL_H
