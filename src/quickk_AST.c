 #include "../include/quickk_AST.h"
#include <stdlib.h>

AST_T* init_ast_expr(int type) {
  AST_T* ast = calloc(1, sizeof(AST_T));
  ast->ast_type = type;
  ast->function_call_arguments = init_list();
  ast->function_definition_arguments = init_list();
  ast->compound_children = init_list();
  ast->array_children = init_list();
  ast->class_types = init_list();
  ast->class_constructors = init_list();
  ast->class_function_definitions = init_list();
  ast->class_variable_definitions = init_list();
  ast->class_identifier_types = (type == AST_CLASS_IDENT ? init_list() : NULL);
  return ast;
}


char const *ast_type_to_string(int type) {
  switch (type) {
  case AST_INT:
    return "integer";
  case AST_FLOAT:
    return "float";
  case AST_STRING:
    return "string";
  case AST_BOOLEAN:
    return "boolean";
  case AST_CHAR:
    return "char";
  case AST_ARRAY:
    return "{array}";
  case AST_FUNCTION_CALL:
    return "[Function]";
  case AST_FUNCTION_DEFINITION:
    return "Function definition";
  case AST_NULL:
    return "null";
  }
  return "none";
}
void free_ast_expr(AST_T *ast) {
  if (!ast)
    return;
  int ready_to_free = 1;
  if (ast->ast_type == AST_COMPOUND) {
    for (int i = 0; i < ast->compound_children->used; i++) {
      free_ast_expr(ast->compound_children->items[i]);
    }
  }

  if (ast->ast_type == AST_IDENT) {
    free(ast->identifier_value);
  }

  if (ast->ast_type == AST_VARIABLE) {
    free_ast_expr(ast->variable_name);
  }

  if (ast->ast_type == AST_STRING) {
    free(ast->string_value);
  }

  if (ast->ast_type == AST_ARRAY) {
    for (int i = 0; i < ast->array_children->used; i++) {
      free_ast_expr(ast->array_children->items[i]);
    }
  }

  if (ast->ast_type == AST_CLASS) {
    free_ast_expr(ast->class_name);
    free_ast_expr(ast->class_body);
    free_list(ast->class_constructors);
    free_list(ast->class_function_definitions);
    free_list(ast->class_identifier_types);
    free_list(ast->class_variable_definitions);
  }

  if (ast->ast_type == AST_IF) {
    free_ast_expr(ast->if_condition);
    free_ast_expr(ast->if_body);
  }

  if (ast->ast_type == AST_WHILE) {
    free_ast_expr(ast->while_condition);
    free_ast_expr(ast->while_body);
  }

  if (ast->ast_type == AST_FOR) {
    free_ast_expr(ast->for_variable);
    free_ast_expr(ast->for_iterator);
    free_ast_expr(ast->for_body);
  }

  if (ast->ast_type == AST_UNOP) {
    free_ast_expr(ast->unop_right);
  }

  if (ast->ast_type == AST_BINOP) {
    free_ast_expr(ast->binop_left);
    free_ast_expr(ast->binop_right);
  }

  if (ast->ast_type == AST_NEW) {
    free_ast_expr(ast->new_value);
  }

  if (ast->ast_type == AST_RETURN) {
    free_ast_expr(ast->return_value);
  }

  if (ast->ast_type == AST_VARIABLE_DEFINITION) {
    free_data_type(ast->data_type);
    free_ast_expr(ast->variable_definition_name);
    free_ast_expr(ast->variable_definition_value);
  }

  if (ast->ast_type == AST_FUNCTION_CALL) {
    free_ast_expr(ast->function_call_name);
    free_list(ast->function_call_arguments);
  }

  if (ast->ast_type == AST_FUNCTION_DEFINITION) {
    free_ast_expr(ast->function_definition_name);
    free_list(ast->function_call_arguments);
    free_ast_expr(ast->function_definition_body);
  }

  if (ast->ast_type == AST_ACCESS) {
    free_ast_expr(ast->binop_left);
    free_ast_expr(ast->binop_right);
  }

  if (ast->ast_type == AST_CONSTRUCTOR) {
    free_ast_expr(ast->function_definition_body);
    free_list(ast->function_definition_arguments);
  }

  if (ast->ast_type == AST_LIST_ACCESS) {
    free_ast_expr(ast->list_access_pointer);
    free_ast_expr(ast->list_value);
  }
  if (ready_to_free)
    free(ast);
}