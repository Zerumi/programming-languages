#define SUM_START 0
#define SERIES_START 1
#define ITERATE_COUNT 10
#include "int_util.h"
#include "list_util.h"
#include "linked_list.h"
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

static void print_int_newline(int64_t value) {
    printf("%" PRId64 "\n", value);
}

static int64_t square(int64_t value) {
    return value * value;
}

static int64_t cube(int64_t value) {
    return square(value) * value;
}

static int64_t accumulate_sum(int64_t accumulator, int64_t value) {
    accumulator += value;
    return accumulator;
}

static int64_t accumulate_min(int64_t accumulator, int64_t value) {
    return accumulator > value ? value : accumulator;
}

static int64_t accumulate_max(int64_t accumulator, int64_t value) {
    return accumulator < value ? value : accumulator;
}

static int64_t abs_int64(int64_t value) {
    return value < 0 ? -value : value;
}

static int64_t power_by_2(int64_t value) {
    return value * 2;
}

int main(int argc, char* argv[]) {
    puts("Enter list:");
    struct list* list =  list_read();
    puts("Print 1:");
    list_foreach(list, &print_int);
    puts("\nPrint 2:");
    list_foreach(list, &print_int_newline);

    struct list* mapped_squared_copy = list_map(list, &square);
    struct list* mapped_cubed_copy = list_map(list, &cube);
    puts("Squared copy:");
    list_print(mapped_squared_copy);
    puts("Cubed copy:");
    list_print(mapped_cubed_copy);

    printf("Sum of list: %" PRId64 "\n",
           list_foldl(SUM_START, list, &accumulate_sum));
    printf("Max of list: %" PRId64 "\n",
           list_foldl(LONG_LONG_MIN, list, &accumulate_max));
    printf("Min of list: %" PRId64 "\n",
           list_foldl(LONG_LONG_MAX, list, &accumulate_min));

    list_map_mut(list, &abs_int64);
    puts("Module of list:");
    list_print(list);

    struct list* iterated_list = list_iterate(SERIES_START, ITERATE_COUNT,
            &power_by_2);
    puts("Iterated list:");
    list_print(iterated_list);

    struct list* new_list;
    printf("List saved: %s\n", list_save(list, "list.txt") ? "true" : "false");
    printf("List loaded: %s\n", list_load(&new_list, "list.txt") ? "true" : "false");
    list_print(new_list);

    struct list* serialize_list;
    printf("List serialized: %s\n", list_serialize(list, "list_serialized.txt") ? "true" : "false");
    printf("List deserialized: %s\n", list_deserialize(&serialize_list,
                                                       "list_serialized.txt") ? "true" : "false");
    list_print(serialize_list);

    list_destroy(serialize_list);
    list_destroy(iterated_list);
    list_destroy(mapped_squared_copy);
    list_destroy(mapped_cubed_copy);
    list_destroy(list);
    return 0;
}
