#include "image_util.h"

struct image rotate(struct image const src) {
    size_t position = 0;
    uint32_t height = src.height;
    uint32_t width = src.width;

    struct image result = { 0 };

    /* Структура массива: [width0, width1, etc...., width^(height - 1)]
     * Поворот на 90: [(height0[i], height1[i])...]
     */

    result.height = width;
    result.width = height;
    result.pixels = malloc(height * width * sizeof (struct pixel));

    for (uint32_t i = 1; i <= width; i++) {
        for (uint32_t j = 1; j <= height; j++, position++) {
            result.pixels[position] = src.pixels[j * width - i];
        }
    }

    return result;
}
