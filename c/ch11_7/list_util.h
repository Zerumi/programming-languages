#ifndef PROGRAMMING_LANGUAGES_LIST_UTIL_H
#define PROGRAMMING_LANGUAGES_LIST_UTIL_H

#include "linked_list.h"
#include "int_util.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

void list_foreach(const struct list* list,
        void (apply_element_proc) (int64_t list_element));
void list_print(const struct list* list);
struct list* list_map(const struct list* list,
        int64_t (apply_element_transform) (int64_t list_element));
void list_map_mut(struct list* list,
        int64_t (apply_element_transform) (int64_t list_element));
int64_t list_foldl(int64_t accumulator_start_value, const struct list* list,
        int64_t (accumulate) (int64_t next_list_element, int64_t accumulator));
struct list* list_iterate(int64_t start_value, size_t list_size,
        int64_t (generate_next_element) (int64_t previous_element));

bool list_save(struct list* list, const char* filename);
bool list_load(struct list** list, const char* filename);
bool list_serialize(struct list* list, const char* filename);
bool list_deserialize(struct list** list, const char* filename);

#endif //PROGRAMMING_LANGUAGES_LIST_UTIL_H
