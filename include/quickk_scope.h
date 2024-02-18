#ifndef QUICKK_QUICKK_SCOPE_H
#define QUICKK_QUICKK_SCOPE_H
#include "utils/list.h"
#include "quickk_AST.h"
typedef struct QUICKK_SCOPE_STRUCT {
  list_T* scope_variable_definitions;
  list_T* scope_function_definitions;
  list_T* scope_class_definitions;
} Scope_T;

Scope_T* init_quickk_scope();
void free_quickk_scope(Scope_T* scope);
void add_variable_definitions_scope(Scope_T* scope, AST_T* variable_definition);
void add_function_definitions_scope(Scope_T* scope, AST_T* function_definition);
void add_class_definitions_scope(Scope_T* scope, AST_T* class_definition);

AST_T* retrieve_variable_definition(Scope_T* scope, const char *vname);
AST_T* retrieve_function_definition(Scope_T* scope, const char *fname);
AST_T* retrieve_class_definition(Scope_T* scope, const char *cname);
int retrieve_variable_definition_index(Scope_T* scope, const char *vname);
void change_vdef_value(Scope_T* scope, const char* vname, AST_T* expr);
void remove_vdef_scope(Scope_T* scope, const char* fname);
#endif