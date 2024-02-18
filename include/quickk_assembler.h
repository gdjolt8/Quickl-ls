#ifndef QUICKK_ASSEMBLER_H
#define QUICKK_ASSEMBLER_H
#include "../include/quickk_AST.h"

char* assembled_root(AST_T* ast, list_T* list);
char* assembled_headers(AST_T *ast, list_T *list);
char* assembled(AST_T* ast, list_T* list);
#endif