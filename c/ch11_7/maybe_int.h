#ifndef PROGRAMMING_LANGUAGES_MAYBE_INT_H
#define PROGRAMMING_LANGUAGES_MAYBE_INT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

// Один из двух случаев:
// - valid = true и value содержит осмысленный результат
// - valid = false и value может быть любым

struct maybe_int64 {
    bool valid;
    int64_t value;
};

struct maybe_int64 some_int64( int64_t i );
extern const struct maybe_int64 none_int64;
struct maybe_int64 maybe_read_int64( void );

#endif //PROGRAMMING_LANGUAGES_MAYBE_INT_H
