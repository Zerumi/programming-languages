#include "list_util.h"

void list_foreach(const struct list* list,
                  void (apply_element_proc) (int64_t list_element)) {
    while (list) {
        apply_element_proc(list->value);
        list = list->next;
    }
}

struct list* list_map(const struct list* list,
                      int64_t (apply_element_transform) (int64_t list_element)) {
    struct list* result = list_copy(list);
    list_map_mut(result, apply_element_transform);
    return result;
}

void list_map_mut(struct list* list,
                  int64_t (apply_element_transform) (int64_t list_element)) {
    while (list) {
        list->value = apply_element_transform(list->value);
        list = list->next;
    }
}

int64_t list_foldl(int64_t accumulator, const struct list* list,
              int64_t (accumulate) (int64_t next_list_element, int64_t accumulator)) {
    while (list) {
        accumulator = accumulate(accumulator, list->value);
        list = list->next;
    }
    return accumulator;
}

struct list* list_iterate(int64_t start_value, size_t list_size,
                          int64_t (generate_next_element) (int64_t previous_element)) {
    if (list_size == 0) return NULL;
    struct list* result = node_create(start_value);
    struct list* ptr = result;
    for (size_t i = 1; i < list_size; i++) {
        int64_t next_val = generate_next_element(ptr->value);
        ptr->next = node_create(next_val);
        ptr = ptr->next;
    }
    return result;
}

void list_print(const struct list* list) {
    list_foreach(list, &print_int);
    puts("");
}

bool list_save(struct list* list, const char* filename) {
    size_t result = 0;
    FILE* f = fopen(filename, "w");
    if (!f) return false;
    while (list) {
        result += fprintf(f, "%" PRId64 " ", list->value);
        list = list->next;
    }
    fclose(f);
    return result;
}

bool list_load(struct list** list, const char* filename) {
    int64_t next_val;
    struct list* prev = NULL;
    FILE* f = fopen(filename, "r");
    if (!f) return false;
    while (fscanf(f, "%" SCNd64 " ", &next_val) != EOF) {
        struct list* new_node = node_create(next_val);
        new_node->next = prev;
        prev = new_node;
    }
    if (!prev) return false;
    struct list* reversed = list_reverse(prev);
    *list = reversed;
    fclose(f);
    return true;
}

bool list_serialize(struct list* list, const char* filename) {
    size_t result = 0;
    FILE* f = fopen(filename, "wb");
    if (!f) return false;
    struct array_int64* from_list = list_to_array(list);
    result += fwrite(from_list->array, from_list->size*sizeof(int64_t), 1, f);
    array_int_destroy(from_list);
    fclose(f);
    return result;
}

bool list_deserialize(struct list** list, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return false;
    struct list* prev = NULL;
    int64_t next_val = 0;
    while (fread(&next_val, sizeof (int64_t), 1, f)) {
        struct list* new_node = node_create(next_val);
        new_node->next = prev;
        prev = new_node;
    }
    if (!prev) return false;
    struct list* reversed = list_reverse(prev);
    *list = reversed;
    fclose(f);
    return true;
}
