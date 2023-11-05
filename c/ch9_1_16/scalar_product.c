#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int32_t scalar_product(const size_t vectors_size, const int32_t* vector1, const int32_t* vector2) {
    int32_t result = 0;
    size_t i;

    for (i = 0; i < vectors_size; i++) {
        result += vector1[i] * vector2[i];
    }

    return result;
}

const size_t VECTOR_SIZE = 3;
const int32_t vector1[] = {1, 2, 3};
const int32_t vector2[] = {4, 5, 6};

int main(int argc, char* argv[]) {
    printf("Scalar product is %" PRId32, scalar_product(VECTOR_SIZE, vector1, vector2));
    return 0;
}
