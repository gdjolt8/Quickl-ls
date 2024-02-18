#include "../include/utils/pos.h"
#include <stdlib.h>

Position_T* init_position(long long column, long long line) {
  Position_T* pos = calloc(1, sizeof(Position_T));
  pos->column = column;
  pos->line = line;
  return pos;
}