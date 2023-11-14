#include "vector.h"

int main( void ) {

    struct vector* a = vector_create(5);

    vector_change_size(a, 100);

    for (int64_t i = 0; i <= 100; i++) {
        vector_push_int64(a, i*i);
    }


    vector_change_size(a, 10);

    vector_print(a);
    vector_destroy(a);

    return 0;
}
