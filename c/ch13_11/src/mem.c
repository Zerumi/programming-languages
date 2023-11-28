#define _DEFAULT_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mem_internals.h"
#include "mem.h"
#include "util.h"

void debug_block(struct block_header* b, const char* fmt, ... );
void debug(const char* fmt, ... );

extern inline block_size size_from_capacity( block_capacity cap );
extern inline block_capacity capacity_from_size( block_size sz );

static bool            block_is_big_enough( size_t query, struct block_header* block ) { return block->capacity.bytes >= query; }
static size_t          pages_count   ( size_t mem )                      { return mem / getpagesize() + ((mem % getpagesize()) > 0); }
static size_t          round_pages   ( size_t mem )                      { return getpagesize() * pages_count( mem ) ; }

static void block_init( void* restrict addr, block_size block_sz, void* restrict next ) {
  *((struct block_header*)addr) = (struct block_header) {
    .next = next,
    .capacity = capacity_from_size(block_sz),
    .is_free = true
  };
}

static size_t region_actual_size( size_t query ) { return size_max( round_pages( query ), REGION_MIN_SIZE ); }

extern inline bool region_is_invalid( const struct region* r );



static void* map_pages(void const* addr, size_t length, int additional_flags) {
  return mmap( (void*) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | additional_flags , -1, 0 );
}

/*  аллоцировать регион памяти и инициализировать его блоком */
static struct region alloc_region(void const* addr, size_t query) {
    bool extends = true;
    query += size_from_capacity((block_capacity) { 0 }).bytes;
    size_t alloc_size = region_actual_size(query);
    block_size size = { .bytes = alloc_size };
    void* address = map_pages(addr, alloc_size, MAP_FIXED);
    if (address == MAP_FAILED) {
        address = map_pages(addr, alloc_size, 0);
        extends = false;
    }
    if (address == MAP_FAILED)
        return (struct region) { 0 };
    block_init(address, size, NULL);
    return (struct region) { .addr = address, .size = alloc_size, .extends = extends };
}


static void* block_after( struct block_header const* block );
static bool blocks_continuous(
    struct block_header const* fst,
    struct block_header const* snd);

void* heap_init( size_t initial ) {
  const struct region region = alloc_region( HEAP_START, initial );
  if ( region_is_invalid(&region) ) return NULL;

  return region.addr;
}

/*  освободить всю память, выделенную под кучу */
void heap_term( ) {
    struct block_header* start_addr = (struct block_header*) HEAP_START;
    void* next_clean_addr = start_addr;
    struct block_header* addr = start_addr;
    while (addr != NULL) {
        size_t len = 0;
        while (blocks_continuous(addr, addr->next)) {
            len += size_from_capacity(addr->capacity).bytes;
            addr = addr->next;
        }
        len += size_from_capacity(addr->capacity).bytes;
        addr = addr->next;
        munmap(next_clean_addr, len);
        next_clean_addr = addr;
    }
}

#define BLOCK_MIN_CAPACITY 24

/*  --- Разделение блоков (если найденный свободный блок слишком большой )--- */

static bool block_splittable( struct block_header* restrict block, size_t query) {
  return block-> is_free && query + offsetof( struct block_header, contents ) + BLOCK_MIN_CAPACITY <= block->capacity.bytes;
}

static bool split_if_too_big( struct block_header* block, size_t query ) {
    if (block_splittable(block, query)) {
        void* old_address = block;
        block_size old_size = size_from_capacity((block_capacity) {.bytes = query});
        void* new_address = block->contents + query;
        block_size new_size = { .bytes = block->capacity.bytes - query };
        void* next_block = block->next;
        block_init(old_address, old_size, new_address);
        block_init(new_address, new_size, next_block);
        return true;
    }
    return false;
}


/*  --- Слияние соседних свободных блоков --- */

static void* block_after( struct block_header const* block ) {
  return  (void*) (block->contents + block->capacity.bytes);
}
static bool blocks_continuous (
                               struct block_header const* fst,
                               struct block_header const* snd ) {
  return (void*)snd == block_after(fst);
}

static bool mergeable(struct block_header const* restrict fst, struct block_header const* restrict snd) {
  return fst->is_free && snd->is_free && blocks_continuous( fst, snd ) ;
}

static bool try_merge_with_next( struct block_header* block ) {
    if (block && block->next && mergeable(block, block->next)) {
        void* address = block;
        block_size size = { .bytes = 2 * offsetof(struct block_header, contents) + block->capacity.bytes + block->next->capacity.bytes };
        void* block_next = block->next->next;

        block_init(address, size, block_next);
        return true;
    }
    return false;
}


/*  --- ... ecли размера кучи хватает --- */

struct block_search_result {
  enum {BSR_FOUND_GOOD_BLOCK, BSR_REACHED_END_NOT_FOUND, BSR_CORRUPTED} type;
  struct block_header* block;
};


static struct block_search_result find_good_or_last  ( struct block_header* restrict block, size_t sz ) {
    struct block_search_result result = { .type = BSR_CORRUPTED };
    struct block_header* prev_block = NULL;
    struct block_header* current_block = block;
    bool not_corrupted = false;
    while (current_block) {
        not_corrupted = true;

        if (try_merge_with_next(current_block))
            continue;

        if (current_block->is_free && current_block->capacity.bytes >= sz) {
            result.type = BSR_FOUND_GOOD_BLOCK;
            result.block = current_block;
            break;
        }
        prev_block = current_block;
        current_block = current_block->next;
    }
    if (!current_block && not_corrupted) {
        result.type = BSR_REACHED_END_NOT_FOUND;
        result.block = prev_block;
    }
    return result;
}

/*  Попробовать выделить память в куче начиная с блока `block` не пытаясь расширить кучу
 Можно переиспользовать как только кучу расширили. */
static struct block_search_result try_memalloc_existing ( size_t query, struct block_header* block ) {
    struct block_search_result result = find_good_or_last(block, query);
    if (result.type == BSR_FOUND_GOOD_BLOCK) {
        split_if_too_big(result.block, query);
        result.block->is_free = false;
    }
    return result;
}



static struct block_header* grow_heap( struct block_header* restrict last, size_t query ) {
    void* end_addr = last->contents + last->capacity.bytes;
    struct region new_region = alloc_region(end_addr, query);
    last->next = new_region.addr;
    try_merge_with_next(last);
    struct block_header* result = last;
    if (!new_region.extends || !last->is_free) {
        result = new_region.addr;
    }
    return result;
}

static size_t block_actual_capacity(size_t query) { return size_max(query, BLOCK_MIN_CAPACITY); }

/*  Реализует основную логику malloc и возвращает заголовок выделенного блока */
static struct block_header* memalloc( size_t query, struct block_header* heap_start) {

    size_t actual_query = block_actual_capacity(query);

    struct block_search_result result = try_memalloc_existing(actual_query, heap_start);

    while (result.type == BSR_REACHED_END_NOT_FOUND) {
        result = try_memalloc_existing(actual_query, grow_heap(result.block, actual_query));
        // seems like it will loop when out of memory :)
    }

    if (result.type == BSR_FOUND_GOOD_BLOCK) {
        return result.block;
    }

    return NULL;
}

void* _malloc( size_t query ) {
  struct block_header* const addr = memalloc( query, (struct block_header*) HEAP_START );
  if (addr) return addr->contents;
  else return NULL;
}

static struct block_header* block_get_header(void* contents) {
  return (struct block_header*) (((uint8_t*)contents)-offsetof(struct block_header, contents));
}

void _free( void* mem ) {
  if (!mem) return;
  struct block_header* header = block_get_header( mem );
  header->is_free = true;
  try_merge_with_next(header);
}
