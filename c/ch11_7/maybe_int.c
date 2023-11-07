#include "maybe_int.h"

// Первый случай; создаем функцию в помощь
// Не бойтесь за производительность
struct maybe_int64 some_int64( int64_t i ) {
    return (struct maybe_int64) { .value = i, .valid = true };
}

// Второй случай; можно создать не функцию,
// а константный экземпляр структуры
// Все поля инициализированы нулями
// .value = 0, .valid = false
const struct maybe_int64 none_int64 = { 0 };

struct maybe_int64 maybe_read_int64( void ) {
    int64_t result = 0;
    if (scanf("%" SCNd64, &result) == 1) return some_int64(result);
    return none_int64;
}

