#ifndef IMAGE_TRANSFORMER_IMAGE_H
#define IMAGE_TRANSFORMER_IMAGE_H

#include <inttypes.h>
#include <stdlib.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint32_t height;
    uint32_t width;
    struct pixel* pixels;
};

#endif //IMAGE_TRANSFORMER_IMAGE_H
