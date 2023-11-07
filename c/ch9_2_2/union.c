#include <stdio.h>
#include <inttypes.h>

union dword {
    int32_t integer;
    int16_t shorts[2];
};

int main(int argc, char* argv[]) {
    union dword test;
    test.integer = -42;
    printf("%" PRId32 " %" PRId32 "\n", test.shorts[0], test.shorts[1]);
}
