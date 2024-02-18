#include "../include/utils/list.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
list_T *init_list() {
  list_T* list = (list_T*)calloc(1, sizeof(list_T));
  list->items = 0;
  list->used = 0;
  return list;
}
void add_element_list (list_T* list, void *item) {
  if (!list) {
    fprintf(stderr, "List (%p) is null!", list);
  }
  list->used++;
  if (!list->items) {
    list->items = calloc(1, sizeof(void*));
  } else {
    list->items = realloc(list->items, sizeof(void*) * list->used);
  }
  list->items[list->used - 1] = item;
}

void free_list(list_T* list) {
  if(!list) return;
  if (list->used != 0) free(list->items);
  free(list);
}

void remove_element_list(list_T* list, int index){
  
  if (!list) {
    printf("ListNullException: List (%p) is null/not set.\n", list);
    return;
  }
  if (index >= list->used || index < 0) {
    printf("IndexOutOfBoundsException: Index (%d) is out of bounds.\n", index);
    return;
  }

  for (int i = index; i < list->used - 1; i++) {
    list->items[i] = list->items[i + 1];
  }
  list->used--;
  list->items = realloc(list->items, sizeof(void*) * list->used);
}
list_T* split(char* text, char const* delimiter) {
    list_T *result = init_list();
    
    char *mutable_string = strdup(text);
    char *token = strtok(mutable_string, delimiter);
    while (token) {
        char *copy = strdup(token);  // Create a copy of the token
      add_element_list(result, copy);
        token = strtok(NULL, delimiter);
    }
    
    free(mutable_string);
    return result;
}