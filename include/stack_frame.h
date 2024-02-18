#ifndef QUICKK_STACK_FRAME_H
#define QUICKK_STACK_FRAME_H
#include "utils/list.h"
typedef struct STACK_FRAME_STRUCT
{
  list_T* stack;
} StackFrame_T;

StackFrame_T* init_stack_frame();
#endif