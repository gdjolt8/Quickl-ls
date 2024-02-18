#ifndef CLORC_UTILS_LIST_H
#define CLORC_UTILS_LIST_H
#include <stddef.h>

typedef struct LIST_STRUCT {
  void **items;
  size_t used;
} list_T;

list_T *init_list();
void add_element_list (list_T* list, void *item);
void free_list(list_T* list);
void remove_element_list(list_T* list, int index);
list_T* split(char* text, char const* delimiter);
#endif