#include "../include/quickk_scope.h"
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"

Scope_T* init_quickk_scope() {
  Scope_T* scope = calloc(1, sizeof(Scope_T));
  scope->scope_class_definitions = init_list();
  scope->scope_function_definitions = init_list();
  scope->scope_variable_definitions = init_list();
  return scope;
}
void free_quickk_scope(Scope_T* scope) {
  if (!scope) return; // no seg fault today haha
  free_list(scope->scope_class_definitions);
  free_list(scope->scope_function_definitions);
  free_list(scope->scope_variable_definitions);
  free(scope);
}
void add_variable_definitions_scope(Scope_T* scope, AST_T* variable_definition) {
  add_element_list(scope->scope_variable_definitions, variable_definition);
}
void add_function_definitions_scope(Scope_T* scope, AST_T* function_definition) {
  add_element_list(scope->scope_function_definitions, function_definition);
}
void add_class_definitions_scope(Scope_T* scope, AST_T* class_definition) {
  add_element_list(scope->scope_class_definitions, class_definition);
}

AST_T* retrieve_variable_definition(Scope_T* scope, const char *vname) {
  for(int i = 0; i < scope->scope_variable_definitions->used; i++) {
    AST_T* item = (AST_T*)scope->scope_variable_definitions->items[i];

    if(strcmp(item->variable_definition_name->identifier_value, (char*)vname) == 0) {
      return item;
    }
  }
  return NULL;
}
AST_T* retrieve_function_definition(Scope_T* scope, const char *fname) {
  for(int i = 0; i < scope->scope_function_definitions->used; i++) {
    AST_T* item = (AST_T*)scope->scope_function_definitions->items[i];
    if(strcmp(item->function_definition_name->identifier_value, fname)) {
      return item;
    }
  }
  return NULL;
}
AST_T* retrieve_class_definition(Scope_T* scope, const char *cname) {
  for(int i = 0; i < scope->scope_class_definitions->used; i++) {
    AST_T* item = (AST_T*)scope->scope_class_definitions->items[i];
    if(strcmp(item->class_name->identifier_value, cname) == 0) {
      return item;
    }
  }
  return NULL;
}
int retrieve_variable_definition_index(Scope_T* scope, const char *vname) {
    for(int i = 0; i < scope->scope_variable_definitions->used; i++) {
    AST_T* item = (AST_T*)scope->scope_variable_definitions->items[i];
    if(strcmp(item->variable_definition_name->identifier_value, vname) == 0) {
      return i;
    }
  }
  return -1;
}
void remove_vdef_value(Scope_T* scope, const char* fname) {
  for(int i = 0; i < scope->scope_variable_definitions->used; i++) {
    AST_T* vdef = scope->scope_variable_definitions->items[i];
    if (strcmp(vdef->variable_definition_name->identifier_value, fname) == 0) {
      AST_T* dm = init_ast_expr(AST_VARIABLE_DEFINITION);
      dm->variable_definition_name = init_ast_expr(AST_IDENT);
      dm->variable_definition_name->identifier_value = "";
      scope->scope_variable_definitions->items[i] = dm;
    }
  }
}
void change_vdef_value(Scope_T* scope, const char* vname, AST_T* expr) {
  AST_T* newvdef = retrieve_variable_definition(scope, vname);
  newvdef->variable_definition_value = expr;
  int a = retrieve_variable_definition_index(scope, vname);
  scope->scope_variable_definitions->items[a] = newvdef;
}