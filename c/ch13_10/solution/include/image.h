#ifndef IMAGE_TRANSFORMER_IMAGE_H
#define IMAGE_TRANSFORMER_IMAGE_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint32_t height;
    uint32_t width;
    struct pixel* pixels;
};

extern struct image ZERO_IMAGE;

bool image_compare(struct image* img1, struct image* img2);

#endif //IMAGE_TRANSFORMER_IMAGE_H
