#include "../include/quickk_bootstrap.h"
#include <stdio.h>
AST_T* quickk_to_string(AST_T* arg) {
  return arg->token->value;
}
AST_T* quickk_io_getln(list_T* args);