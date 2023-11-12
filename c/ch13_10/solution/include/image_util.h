#ifndef IMAGE_TRANSFORMER_IMAGE_UTIL_H
#define IMAGE_TRANSFORMER_IMAGE_UTIL_H

#include "image.h"

#include <libc.h>

#define PIXELS_COUNT 9

enum blur_border_identity_padding {
    NO_PADDING = 0,
    BLACK,
    WHITE,
    REFLECTION,
    IMAGE_LOOP,
    BORDER_PIXEL
};

struct image rotate(struct image src);
struct image blur(struct image src, enum blur_border_identity_padding);

#endif //IMAGE_TRANSFORMER_IMAGE_UTIL_H
