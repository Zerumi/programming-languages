#include "array_int64.h"
#include <stdlib.h>

void array_int_destroy(struct array_int64* to_destroy) {
    free(to_destroy->array);
    free(to_destroy);
}
