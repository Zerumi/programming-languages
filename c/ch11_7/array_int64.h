#ifndef CH11_7_ARRAY_INT64_H
#define CH11_7_ARRAY_INT64_H

#include <stddef.h>
#include <stdint.h>

struct array_int64 {
    size_t size;
    int64_t* array;
};

void array_int_destroy(struct array_int64* to_destroy);

#endif //CH11_7_ARRAY_INT64_H
