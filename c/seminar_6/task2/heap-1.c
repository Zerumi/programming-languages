/* heap-1.c */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_BLOCKS 16
#define BLOCK_CAPACITY 1024

enum block_status { BLK_FREE = 0, BLK_ONE, BLK_FIRST, BLK_CONT, BLK_LAST };

struct heap {
  struct block {
    char contents[BLOCK_CAPACITY];
  } blocks[HEAP_BLOCKS];
  enum block_status status[HEAP_BLOCKS];
} global_heap = {0};

struct block_id {
  size_t       value;
  bool         valid;
  struct heap* heap;
};

struct block_id block_id_new(size_t value, struct heap* from) {
  return (struct block_id){.valid = true, .value = value, .heap = from};
}
struct block_id block_id_invalid() {
  return (struct block_id){.valid = false};
}

bool block_id_is_valid(struct block_id bid) {
  return bid.valid && bid.value < HEAP_BLOCKS;
}

/* Find block */

bool block_is_free(struct block_id bid) {
  if (!block_id_is_valid(bid))
    return false;
  return bid.heap->status[bid.value] == BLK_FREE;
}

/* Allocate */
struct block_id block_allocate(struct heap* heap, size_t size) {
  size_t amount_of_blocks_required = size / BLOCK_CAPACITY + (size % BLOCK_CAPACITY ? 1 : 0);
  for (size_t i = 0, free_in_row = 1; i < HEAP_BLOCKS && free_in_row <= amount_of_blocks_required; i++, free_in_row++) {
      struct block_id block = block_id_new(i, heap);
      if (!block_is_free(block)) free_in_row = 0;
      // allocate block
      if (free_in_row == amount_of_blocks_required) {
          if (amount_of_blocks_required == 1) {
              heap->status[i] = BLK_ONE;
              return block_id_new(i, heap);
          }
          heap->status[i + 1 - free_in_row] = BLK_FIRST;
          for (size_t j = 1; j < free_in_row - 1; j++) {
              heap->status[i - j] = BLK_CONT;
          }
          heap->status[i] = BLK_LAST;
          return block_id_new(i + 1 - free_in_row, heap);
      }
  }
  return block_id_invalid();
}
/* Free */

void block_free(struct block_id b) {
    if (b.heap->status[b.value] == BLK_ONE)
        b.heap->status[b.value] = BLK_FREE;
    if (b.heap->status[b.value] == BLK_FIRST) {
        size_t next;
        for (next = b.value; b.heap->status[next] != BLK_LAST; next++) {
            b.heap->status[next] = BLK_FREE;
        }
        b.heap->status[next] = BLK_FREE;
    }
}

/* Printer */
const char* block_repr(struct block_id b) {
  static const char* const repr[] = {[BLK_FREE] = " .",
                                     [BLK_ONE] = " *",
                                     [BLK_FIRST] = "[=",
                                     [BLK_LAST] = "=]",
                                     [BLK_CONT] = " ="};
  if (b.valid)
    return repr[b.heap->status[b.value]];
  else
    return "INVALID";
}

void block_debug_info(struct block_id b, FILE* f) {
  fprintf(f, "%s", block_repr(b));
}

void block_foreach_printer(struct heap* h, size_t count,
                           void printer(struct block_id, FILE* f), FILE* f) {
  for (size_t c = 0; c < count; c++)
    printer(block_id_new(c, h), f);
}

void heap_debug_info(struct heap* h, FILE* f) {
  block_foreach_printer(h, HEAP_BLOCKS, block_debug_info, f);
  fprintf(f, "\n");
}
/*  -------- */

int main() {
    heap_debug_info(&global_heap, stdout);

    block_allocate(&global_heap, 1025);
    struct block_id b = block_allocate(&global_heap, 3025);
    block_allocate(&global_heap, 3025);
    heap_debug_info(&global_heap, stdout);

    block_free(b);
    heap_debug_info(&global_heap, stdout);

    struct block_id c = block_allocate(&global_heap, 1024);
    block_allocate(&global_heap, 5025);
    heap_debug_info(&global_heap, stdout);
    block_allocate(&global_heap, 1024);
    block_free(c);
    block_allocate(&global_heap, 1025);
    heap_debug_info(&global_heap, stdout);

  return 0;
}
