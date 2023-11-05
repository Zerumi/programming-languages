#define SCAN_ERROR 2

#include <stdio.h>
#include <inttypes.h>

int is_prime(u_int64_t number) {
    uint64_t i;

    if (number < 1) return 0;
    if (number == 2) return 1;
    if (number % 2 == 0) return 0;

    for (i = 3; i * i < number; i+=2) {
        if (number % i == 0) return 0;
    }

    return 1;
}

int main(int argc, char* argv[]) {
    u_int64_t to_check;

    puts("enter number for check it's prime: ");

    if (!scanf("%" SCNu64, &to_check)) {
        perror("scan error");
        return SCAN_ERROR;
    }

    char* res = is_prime(to_check) ? "yes" : "no";

    puts(res);
}
