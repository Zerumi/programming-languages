#include "linked_list.h"

struct list* node_create( int64_t value ) {
    struct list* result = malloc(sizeof(struct list) * 1);
    result->value = value;
    return result;
}

void list_add_front( struct list** old, int64_t value ) {
    struct list* start_node = node_create(value);
    start_node->next = *old;
    *old = start_node;
}

size_t list_length( const struct list* list ) {
    size_t result = 0;
    const struct list* ptr = list;
    while (ptr) {
        result++;
        ptr = ptr->next;
    }

    return result;
}

struct list* list_copy(const struct list* l ) {
    if (l) {
        struct list* result = node_create( l->value );
        struct list* ptr = result;
        while (l->next) {
            l = l->next;
            ptr->next = node_create(l->value);
            ptr = ptr->next;
        }
        return result;
    }
    return NULL;
}

void list_destroy( struct list* list ) {
    struct list* next = list;
    while (next) {
        struct list* temp = next->next;
        free(next);
        next = temp;
    }
}

struct list* list_last( struct list * list ) {
    if (!list) return NULL;
    struct list* result = list;
    while(list->next) {
        list = list->next;
        result = list;
    }
    return result;
}

void list_add_back( struct list** old, int64_t value ) {
    struct list* prev_last_element = list_last(*old);
    if (prev_last_element) {
        prev_last_element->next = node_create(value);
    } else {
        list_add_front(old, value);
    }
}

int64_t list_sum( const struct list* list ) {
    if (list)
        return list->value + list_sum(list->next);
    return 0;
}

struct maybe_int64 list_at( const struct list* list, size_t idx ) {
    for (size_t i = 0; i < idx && list; i++)
        list = list->next;
    if (list) return some_int64(list->value);
    return none_int64;
}

struct list* list_reverse( const struct list* list ) {
    struct list* reverse_copy = NULL;
    while (list) {
        list_add_front(&reverse_copy, list->value);
        list = list->next;
    }
    return reverse_copy;
}

struct list* list_read( void ) {

    struct maybe_int64 first_val = maybe_read_int64();
    if (!first_val.valid) return NULL;

    struct list* result = node_create(first_val.value);
    struct list* ptr = result;

    struct maybe_int64 value = maybe_read_int64();

    while(value.valid) {
        ptr->next = node_create(value.value);
        ptr = ptr->next;
        value = maybe_read_int64();
    }

    return result;
}

struct array_int64* list_to_array(const struct list* list) {
    size_t i;
    struct array_int64* result = malloc(sizeof(struct array_int64));
    size_t size = list_length(list);
    int64_t* arr = malloc(size * sizeof(int64_t));
    for(i = 0; i < size; i++) {
        arr[i] = list->value;
        list = list->next;
    }
    result->size = size;
    result->array = arr;
    return result;
}
