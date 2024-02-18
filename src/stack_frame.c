#include "../include/stack_frame.h"


StackFrame_T* init_stack_frame()
{
  StackFrame_T* stack_frame = calloc(1, sizeof(struct STACK_FRAME_STRUCT));
  stack_frame->stack = init_list(sizeof(char*));

  return stack_frame;
}