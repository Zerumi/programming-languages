#include <stdio.h>
#include "image_bmp_worker.h"
#include "image_util.h"

int main( int argc, char** argv ) {
    (void) argc; (void) argv; // suppress 'unused parameters' warning

    struct image img = { 0 };
    FILE* f = fopen(argv[1], "rb");
    from_bmp(f, &img);
    struct image rotated_img = rotate(img);
    FILE* o = fopen("output.bmp", "wb");
    to_bmp(o, &rotated_img);
    free(img.pixels);
    free(rotated_img.pixels);
    fclose(o);
    fclose(f);

    return 0;
}
