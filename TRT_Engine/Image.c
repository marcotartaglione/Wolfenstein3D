//
// Created by Marco on 17/07/2024.
//

#include "TRT_Engine.h"

Image* TRT_getImage(char* path) {
    Image *image = malloc(sizeof(Image));

    stbi_set_flip_vertically_on_load(1);

    FILE *fp;
    if (fopen_s(&fp, path, "r") != 0) {
        return NULL;
    }
    fclose(fp);

    image->data = stbi_load(path, &image->width, &image->height, &image->channels, 3);
    return image;
}