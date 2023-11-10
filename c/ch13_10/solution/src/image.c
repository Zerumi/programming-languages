#include "image.h"

struct image ZERO_IMAGE = { 0 };

bool image_compare(struct image* img1, struct image* img2) {
    return img1 == img2
    || (img1->width == img2->width
    && img1->height == img2->height
    && img1->pixels == img2->pixels);
}
