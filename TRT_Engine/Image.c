//
// Created by Marco on 17/07/2024.
//

#include "TRT_Engine.h"

Image *TRT_image_get(char *path) {
    Image *image = malloc(sizeof(Image));

    stbi_set_flip_vertically_on_load(1);

    FILE *fp;
    if (fopen_s(&fp, path, "r") != 0) {
        TRT_error("TRT_image_get", "File not found", false);
        return NULL;
    }
    fclose(fp);

    image->data = stbi_load(path, &image->width, &image->height, &image->channels, 3);
    return image;
}

void TRT_image_draw(Image *image, Vec2 position, Vec2 size) {
    if (image == NULL) {
        TRT_error("TRT_image_draw", "Image is NULL", false);
        return;
    }

    TRT_window_interpretateSize(&size, false);
    TRT_window_interpretatePosition(&position, size, false);

    float xRatio = (float) size.x / (float) image->width;
    float yRatio = (float) size.y / (float) image->height;

    for (uint32_t x = 0; x < size.x; ++x) {
        for (uint32_t y = 0; y < size.y; ++y) {
            uint32_t textureIndex = ((int) ((float) y / yRatio) * image->width + (int) ((float) x / xRatio)) * 3;

            uint32_t r = image->data[textureIndex + 0] << 16;
            uint32_t g = image->data[textureIndex + 1] << 8;
            uint32_t b = image->data[textureIndex + 2];

            TRT_window_setPixel(position.x + x, position.y + y, r | g | b);
        }
    }
}

void TRT_image_free(Image *image) {
    if (image == NULL) {
        TRT_error("TRT_image_free", "Image is NULL", false);
        return;
    }

    free(image->data);
    free(image);
}