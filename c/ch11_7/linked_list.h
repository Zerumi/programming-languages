#ifndef PROGRAMMING_LANGUAGES_LINKED_LIST_H
#define PROGRAMMING_LANGUAGES_LINKED_LIST_H

#include "maybe_int.h"
#include "array_int64.h"
#include <stdint.h>
#include <stdlib.h>

struct list {
    int64_t value;
    struct list* next;
};

struct list* node_create( int64_t value );
void list_add_front( struct list** old, int64_t value );
size_t list_length( const struct list* list );
struct list* list_copy(const struct list* list );
void list_destroy( struct list* list );
struct list* list_last( struct list * list );
void list_add_back( struct list** old, int64_t value );
int64_t list_sum( const struct list* list );
struct maybe_int64 list_at( const struct list* list, size_t idx );
struct list* list_reverse( const struct list* list );
struct list* list_read( void );
struct array_int64* list_to_array(const struct list* list);

#endif //PROGRAMMING_LANGUAGES_LINKED_LIST_H
