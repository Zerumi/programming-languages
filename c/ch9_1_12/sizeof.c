#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("%zu\n", sizeof(void)); // error in c99 and below, 1 in other case
    printf("%zu\n", sizeof(0)); // 4 (int)
    printf("%zu\n", sizeof('x')); // 4 (char constant)
    printf("%zu\n", sizeof("hello")); // 6 (5 chars + '\0')

    int x = 10;
    printf("%zu %d", sizeof(x = 90), x);

    return 0;
}
