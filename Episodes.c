//
// Created by Marco on 29/07/2024.
//

#include "Episodes.h"

Episode *episodes[20];

static void readTitle(Episode* episode, FILE *fp) {
    fread(&episode->titleLength, sizeof(uint64_t), 1, fp);
    fread(episode->title, sizeof(char), episode->titleLength, fp);
    episode->title[episode->titleLength] = '\0';
}

static void readThumbnail(Episode* episode, FILE *fp, char *path) {
    char thumbnail[512];
    uint64_t thumbnailLength;

    char filename[512];

    fread(&thumbnailLength, sizeof(uint64_t), 1, fp);
    fread(thumbnail, sizeof(char), thumbnailLength, fp);
    thumbnail[thumbnailLength] = '\0';

    snprintf(filename, sizeof(filename), "%s/%s", path, thumbnail);
    filename[thumbnailLength + strlen(path) + 1] = '\0';

    episode->thumbnail = TRT_image_get(filename);
}

Episode *Episode_get(char* path) {
    Episode *episode = malloc(sizeof(Episode));
    if (episode == NULL) {
        exit(EXIT_FAILURE);
    }

    char filename[512];
    snprintf(filename, sizeof(filename), "%s/%s", path, "level.data");

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    readTitle(episode, fp);
    readThumbnail(episode, fp, path);
    fclose(fp);

    return episode;
}