//
// Created by Marco on 29/07/2024.
//

#include "Episodes.h"

Episode *episodes[EPISODES_COUNT];

static void readTitle(Episode* episode, FILE *fp) {
    uint64_t titleLength;
    fread(&titleLength, sizeof(uint64_t), 1, fp);
    fread_s(episode->title, EPISODE_TITLE_LEN, sizeof(char), titleLength, fp);
    episode->title[titleLength] = '\0';
}

static void readThumbnail(Episode* episode, FILE *fp) {
    uint64_t thumbnailLength;
    fread(&thumbnailLength, sizeof(uint64_t), 1, fp);
    fread_s(episode->thumbnailPath, EPISODE_THUMBNAILPATH_LEN, sizeof(char), thumbnailLength, fp);
    episode->thumbnailPath[thumbnailLength] = '\0';
    episode->thumbnail = TRT_image_get(episode->thumbnailPath);
}

static void loadFloors(Episode* episode, FILE *fp) {
    episode->floors = malloc(sizeof(Map*) * EPISODE_N_FLOORS);
    if (episode->floors == NULL) {
        TRT_error("loadFloors", "Memory allocation failed", true);
        return;
    }

    for (uint16_t i = 0; i < EPISODE_N_FLOORS; ++i) {
        episode->floors[i] = Map_get(fp);
    }
}

Episode *Episode_get(FILE *fp) {
    if (fp == NULL) {
        TRT_error("Episode_get", "File is NULL", true);
        return NULL;
    }

    Episode *episode = malloc(sizeof(Episode));
    if (episode == NULL) {
        TRT_error("Episode_get", "Memory allocation failed", true);
        return NULL;
    }

    readTitle(episode, fp);
    readThumbnail(episode, fp);
    loadFloors(episode, fp);

    return episode;
}

void Episode_save(FILE *fp, Episode *episode) {
    if (fp == NULL) {
        TRT_error("Episode_save", "File is NULL", true);
        return;
    }
    if (episode == NULL) {
        TRT_error("Episode_save", "Episode is NULL", true);
        return;
    }

    uint64_t titleLength = strlen(episode->title);
    fwrite(&titleLength, sizeof(uint64_t), 1, fp);
    fwrite(episode->title, sizeof(char), titleLength, fp);

    uint64_t thumbnailLength = strlen(episode->thumbnailPath);
    fwrite(&thumbnailLength, sizeof(uint64_t), 1, fp);
    fwrite(episode->thumbnailPath, sizeof(char), thumbnailLength, fp);

    for (uint16_t i = 0; i < EPISODE_N_FLOORS; ++i) {
        Map_save(fp, episode->floors[i]);
    }
}

void Episode_free(Episode *episode) {
    if (episode == NULL) {
        TRT_error("Episode_free", "Episode is NULL", true);
        return;
    }

    TRT_image_free(episode->thumbnail);

    for (uint16_t i = 0; i < EPISODE_N_FLOORS; ++i) {
        Map_free(episode->floors[i]);
    }

    free(episode);
}