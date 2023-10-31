/* generic_list.c */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define f_int64_t PRId64
#define f_double "lf"
#define EMPTY_STRING ""

void error(const char *s) {
  fprintf(stderr, "%s", s);
  abort();
}

#define list_push(x, y)						\
	_Generic((y),							\
			int64_t : list_int64_t_push(x,y),		\
			double : list_double_push(x,y),			\
			default : error("Unsupported operation"))

#define list_print(x)							\
	_Generic((x),							\
			struct list_int64_t* : list_int64_t_print(x),	\
			struct list_double* : list_double_print(x),	\
			default : error("Unsupported operation"))
	
#define list_destroy(x)							\
	_Generic((x),							\
			struct list_int64_t* : list_int64_t_destroy(x),	\
			struct list_double* : list_double_destroy(x),	\
			default : error("Unsupported operation"))

#define node_create(x)							\
	_Generic((x),							\
			int64_t : node_int64_t_create(x),		\
			double : node_double_create(x),			\
			default : error("Unsupported operation"))

#define DEFINE_LIST(type)                                               \
  struct list_##type {                                                  \
    type value;                                                         \
    struct list_##type* next;                                           \
  };									\
  struct list_##type* node_##type##_create( type value ) {		\
    struct list_##type* result = malloc(sizeof(struct list_##type) * 1);\
    result->value = value;						\
    return result;							\
  }									\
  void list_##type##_add_front(struct list_##type** old, int64_t value){\
    struct list_##type* start_node = node_##type##_create(value);	\
    start_node->next = *old;						\
    *old = start_node;							\
  }									\
  struct list_##type* list_##type##_last( struct list_##type * list ) {	\
    if (!list) return NULL;						\
    struct list_##type* result = list;					\
    while(list->next) {							\
        list = list->next;						\
        result = list;							\
    }									\
    return result;							\
  }									\
  void list_##type##_push(struct list_##type** old, type value ) {	\
    struct list_##type* prev_last_element = list_##type##_last(*old);	\
    if (prev_last_element) {						\
        prev_last_element->next = node_##type##_create(value);		\
    } else {								\
        list_##type##_add_front(old, value);				\
    }									\
  }									\
  void list_##type##_destroy( struct list_##type* list ) {		\
    struct list_##type* next = list;					\
    while (next) {							\
        struct list_##type* temp = next->next;				\
        free(next);							\
        next = temp;							\
    }									\
  }									\
  void list_##type##_print(const struct list_##type* l) {		\
    while (l) {								\
      printf("%" f_##type  " ", l->value);				\
      l = l->next;							\
    }									\
  }									

DEFINE_LIST(int64_t)
DEFINE_LIST(double)

int main (int argc, char* argv[]) {
	/* first list */
	int64_t value1 = 1;
	struct list_int64_t* list1 = node_create(value1);
	list_push(&list1, value1);
	list_push(&list1, value1);
	list_print(list1);
	list_destroy(list1);
	puts(EMPTY_STRING);
	/* second list */
	int64_t value2 = 42;
	struct list_int64_t* list2 = node_create(value2);
	list_push(&list2, value2);
	list_push(&list2, value1);
	list_push(&list2, value2);
	list_print(list2);
	list_destroy(list2);
	puts(EMPTY_STRING);
	/* third list */
	double value3 = 1337.0;
	struct list_double* list3 = node_create(value3);
	list_push(&list3, value3);
	list_print(list3);
	list_destroy(list3);
	puts(EMPTY_STRING);
	return 0;
}

