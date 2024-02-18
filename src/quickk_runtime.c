#include "../include/quickk_runtime.h"
#include "../include/error.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils/format.h"
#include "../include/quickk_bootstrap.h"
bool quickk_runtime_arguments_is_equal(list_T *left, list_T *right) {
  list_T *lhs = left;
  list_T *rhs = right;
  if (lhs->used != rhs->used)
    return false;
  if (lhs->used == 0) {
    return true;
  }
  int equality = 0;
  for (int i = 0; i < lhs->used; i++) {
    AST_T *l = lhs->items[i];
    AST_T *r = rhs->items[i];
    if (l->data_type->data_type == r->data_type->data_type) {
      equality += 1;
    }
  }

  if (equality == lhs->used)
    return true;
  return false;
}

Runtime_T *init_quickk_runtime(struct QUICKK_SCOPE_STRUCT *scope) {
  Runtime_T *runtime = calloc(1, sizeof(Runtime_T));
  runtime->global_scope = scope;
  return runtime;
}
AST_T *quickk_runtime_visit(Runtime_T *runtime, AST_T *node) {
  switch (node->ast_type) {
  case AST_INT:
    return quickk_runtime_visit_integer(runtime, node);
  case AST_FLOAT:
    return quickk_runtime_visit_float(runtime, node);
  case AST_CHAR:
    return quickk_runtime_visit_char(runtime, node);
  case AST_STRING:
    return quickk_runtime_visit_string(runtime, node);
  case AST_ARRAY:
    return quickk_runtime_visit_array_value(runtime, node);
  case AST_BOOLEAN:
    return quickk_runtime_visit_boolean(runtime, node);
  case AST_FUNCTION_CALL:
    return quickk_runtime_visit_function_call(runtime, node);
  case AST_FUNCTION_DEFINITION:
    return quickk_runtime_visit_function_definition(runtime, node);
  case AST_CLASS:
    return quickk_runtime_visit_class_definition(runtime, node);
  case AST_VARIABLE:
    return quickk_runtime_visit_variable(runtime, node);
  case AST_VARIABLE_DEFINITION: {
    return quickk_runtime_visit_variable_definition(runtime, node);
  }
  case AST_COMPOUND:
    return quickk_runtime_visit_compound(runtime, node);
  case AST_BINOP:
    return quickk_runtime_visit_binary_operation(runtime, node);
  case AST_UNOP:
    return quickk_runtime_visit_unary_operation(runtime, node);
  case AST_LIST_ACCESS:
    return quickk_runtime_visit_list_access(runtime, node);
  case AST_IF:
    return quickk_runtime_visit_if(runtime, node);
  case AST_FOR:
    return quickk_runtime_visit_for(runtime, node);
  case AST_WHILE:
    return quickk_runtime_visit_while(runtime, node);
  case AST_RETURN:
    return quickk_runtime_visit_return(runtime, node);
  case AST_ACCESS:
    return quickk_runtime_visit_access(runtime, node);
  case AST_NULL:
    return quickk_runtime_visit_null(runtime, node);
  case AST_BREAK:
    return quickk_runtime_visit_break(runtime, node);
  case AST_CONTINUE:
    return quickk_runtime_visit_continue(runtime, node);
  default: {
    fprintf(stderr, "Unexpected AST type: `%d`\n", node->ast_type);
    exit(1);
  }
  }
}
AST_T *quickk_runtime_visit_integer(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_float(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_char(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_string(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_boolean(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_identifier(Runtime_T *runtime, AST_T *node) {
  return node;
}
AST_T *quickk_runtime_visit_array_value(Runtime_T *runtime, AST_T *node) {
  AST_T *new_array = init_ast_expr(AST_ARRAY);
  new_array->data_type = node->data_type;
  for (int i = 0; i < node->array_children->used; i++) {
    add_element_list(
        new_array->array_children,
        quickk_runtime_visit(runtime, node->array_children->items[i]));
  }
  return new_array;
}
AST_T *quickk_runtime_visit_function_call(Runtime_T *runtime, AST_T *node) {
  if (strcmp(node->function_call_name->identifier_value, "println") == 0) {
    quickk_runtime_expect_arguments(runtime, node->function_call_arguments, -1,
                                    (int[]){DT_CHAR}, "println");
    list_T* args = init_list();
    for(int i = 0; i < node->function_call_arguments->used; i++) {
      add_element_list(args, quickk_runtime_visit(runtime, (AST_T*)node->function_call_arguments->items[i]));
    } 
    AST_T** args_ = args->items;

    for (size_t i = 0; i < args->used; i++)
    {
      char* str_value = quickk_to_string(args_[i]);
      if (i != args->used) printf("%s ", str_value);
      else printf("%s", str_value);
    }
    putc('\n', stdout);
    
    
    return init_ast_expr(AST_NOOP);
  }
  if (strcmp(node->function_call_name->identifier_value, "to_string") == 0) {
    quickk_runtime_expect_arguments(runtime, node->function_call_arguments, 1,
                                    (int[]){DT_CHAR}, node->function_call_name->identifier_value);
    list_T* args = init_list();
    for(int i = 0; i < node->function_call_arguments->used; i++) {
      add_element_list(args, quickk_runtime_visit(runtime, (AST_T*)node->function_call_arguments->items[i]));
    } 
    AST_T** args_ = args->items;

    for (size_t i = 0; i < args->used; i++)
    {
      if (i != args->used) printf("%s ", args_[i]->string_value);
      else printf("%s", args_[i]->string_value);
    }
    putc('\n', stdout);
    
    
    return init_ast_expr(AST_NOOP);
  }

  list_T *func_names = init_list();

  // makes the function actual values
  AST_T *fdef = NULL;
  for (int i = 0; i < node->function_call_arguments->used; i++) {
    ((AST_T *)node->function_call_arguments->items[i])->scope = node->scope;
    node->function_call_arguments->items[i] =
        quickk_runtime_visit(runtime, node->function_call_arguments->items[i]);
  }
  // get the function definition overload
  for (int i = 0; i < node->scope->scope_function_definitions->used; i++) {
    AST_T *def = node->scope->scope_function_definitions->items[i];
    if (strcmp(def->function_definition_name->identifier_value,
               node->function_call_name->identifier_value) == 0) {
      if (quickk_runtime_arguments_is_equal(def->function_definition_arguments,
                                            node->function_call_arguments)) {
        fdef = def;
      }
    }
  }

  if (!fdef) {
    char *message =
        calloc(strlen(node->function_call_name->identifier_value) + 100,
               sizeof(char));
    sprintf(message, "No overload for function definition (%s)",
      node->function_call_name->identifier_value);

    if (!message) {
      puts("Err: Segfault");
      exit(1);
    }
    /*Quickk_ErrorT *error = init_error_object(
        "ReferenceError", runtime->parser->lexer->path, "",
        node->token->position->line, node->token->position->column);*/
    fprintf(stderr, "%s\n", message);
    exit(1);
  }

  char *function_definition_tuple = calloc(1000, sizeof(char));
  sprintf(function_definition_tuple, "(");
  for (int i = 0; i < node->function_call_arguments->used; i++) {
    DataType_T *dt =
        ((AST_T *)node->function_call_arguments->items[i])->data_type;
    if (i == node->function_call_arguments->used - 1)
      sprintf(function_definition_tuple + strlen(function_definition_tuple),
              "%s", data_type_to_string(dt));
    else
      sprintf(function_definition_tuple + strlen(function_definition_tuple),
              "%s, ", data_type_to_string(dt));
  }
  sprintf(function_definition_tuple + strlen(function_definition_tuple), ")");

  int *type_arguments =
      calloc(fdef->function_definition_arguments->used, sizeof(int));
  for (int i = 0; i < fdef->function_definition_arguments->used; i++) {
    type_arguments[i] = ((AST_T *)fdef->function_definition_arguments->items[i])
                            ->data_type->data_type;
  }
  quickk_runtime_expect_arguments(
      runtime, node->function_call_arguments,
      fdef->function_definition_arguments->used, type_arguments,
      fdef->function_definition_name->identifier_value);

  AST_T *ast_variable = NULL;
  AST_T *ast_value = NULL;
  AST_T *ast_variable_definition = NULL;
  for (int i = 0; i < node->function_call_arguments->used; i++) {
    ast_variable = fdef->function_definition_arguments->items[i];
    ast_value =
        quickk_runtime_visit(runtime, node->function_call_arguments->items[i]);
    ast_variable_definition = init_ast_expr(AST_VARIABLE_DEFINITION);
    ast_variable_definition->variable_definition_name = ast_variable;
    ast_variable_definition->data_type = ast_variable->data_type;
    ast_variable_definition->variable_definition_value = ast_value;
    add_variable_definitions_scope(fdef->function_definition_body->scope,
                                   ast_variable_definition);
  }

  for (int i = 0; i < fdef->function_definition_body->compound_children->used;
       i++) {
    AST_T *statement =
        fdef->function_definition_body->compound_children->items[i];
    if (statement->ast_type == AST_RETURN) {
      AST_T *value = statement->return_value;
      value->data_type = fdef->data_type;
      return value;
    }
    quickk_runtime_visit(runtime, statement);
  }
  return init_ast_expr(AST_NOOP);
}
AST_T *quickk_runtime_visit_function_definition(Runtime_T *runtime,
                                                AST_T *node) {
  add_function_definitions_scope(runtime->global_scope, node);
  return node;
}
AST_T *quickk_runtime_visit_class_definition(Runtime_T *runtime, AST_T *node) {
  add_class_definitions_scope(runtime->global_scope, node);

  return node;
}
AST_T *quickk_runtime_visit_variable(Runtime_T *runtime, AST_T *node) {
  AST_T *variable = retrieve_variable_definition(
      runtime->global_scope, node->variable_name->identifier_value);

  return variable->variable_definition_value;//quickk_runtime_visit(runtime, variable->variable_definition_value);
}
AST_T *quickk_runtime_visit_variable_definition(Runtime_T *runtime, AST_T *node) {
  if (node->data_type->data_type == DT_CLASS) {
    AST_T* class = retrieve_class_definition(node->scope, node->data_type->node->class_identifier->identifier_value);

    if (node->data_type->node->class_identifier_types->used != class->class_types->used) {
      char *error = calloc(512, sizeof(char));
      sprintf(error, "Types (%s) for class definition and variable/function argument data type are contrasting", node->data_type->node->class_identifier->identifier_value);
      Quickk_ErrorT* err = init_error_object(error, runtime->parser->lexer->path, "ReferenceError", runtime->parser->lexer->line_offset, runtime->parser->lexer->char_offset);
      fprintf(stderr, "%s", Error_AsString(err)); 
      exit(1);
    }
    for(int i = 0; i < node->scope->scope_class_definitions->used; i++) {
      AST_T* a = node->scope->scope_class_definitions->items[i];
    }
    char *error = calloc(64 + strlen(node->data_type->node->class_identifier->identifier_value), sizeof(char));
    sprintf(error, "No type named `%s` in current scope.", node->data_type->node->class_identifier->identifier_value);
    Quickk_ErrorT* err = init_error_object(error, runtime->parser->lexer->path, "ReferenceError", runtime->parser->lexer->line_offset, runtime->parser->lexer->char_offset);
      fprintf(stderr, "%s", Error_AsString(err)); 
      exit(1);
  }
  node->variable_definition_value = quickk_runtime_visit(runtime, node->variable_definition_value);
    node->variable_definition_value->data_type = node->data_type;
  add_variable_definitions_scope(node->scope, node);

  return node;  
}
AST_T *quickk_runtime_visit_compound(Runtime_T *runtime, AST_T *node) {
    for(int i = 0; i < node->compound_children->used; i++) {
      AST_T* statement = (AST_T*)node->compound_children->items[i];
    quickk_runtime_visit(runtime, statement);
  }
  return node;
}
AST_T *quickk_runtime_visit_binary_operation(Runtime_T *runtime, AST_T *node){
  AST_T* new_binop = init_ast_expr(AST_BINOP);
  AST_T* binop_left = quickk_runtime_visit(runtime, node->binop_left);
  AST_T* binop_right = quickk_runtime_visit(runtime, node->binop_right);
  switch(node->binop_op) {
    case PLUS:
    case MINUS:
    case ASTERISK:
    case SLASH:
    case PLUSEQ:
    case MINUSEQ:
    case ASTERISKEQ:
    case SLASHEQ:
    case EQEQ:
    case BANGEQ:
    case LTE:
    case GTE:
    case EQ: 
        new_binop->binop_left = binop_left;
        new_binop->binop_right = binop_right;
        new_binop->binop_op = node->binop_op;
        break;
    default: {
        char* err = calloc(200, sizeof(char));
        sprintf(err, "cannot use operator '%s' for %s and %s.", token_type_to_string(node->binop_op), ast_type_to_string(binop_left->ast_type), ast_type_to_string(binop_right->ast_type));
      // init_error_object(char *message, const char *path, char *line_message, long line, long column)
        Quickk_ErrorT* error = init_error_object(err, runtime->parser->lexer->path, "TypeError", node->binop_left->token->position->line, node->binop_left->token->position->column);
        fprintf(stderr, "%s", Error_AsString(error));
        exit(1);
      }
  }
  return new_binop;
}
AST_T *quickk_runtime_visit_unary_operation(Runtime_T *runtime, AST_T *node){
  AST_T* new_unop = init_ast_expr(AST_UNOP);
  AST_T* unop_right = quickk_runtime_visit(runtime, node->unop_right);
  switch (node->unop_op) {
    case NOT:
    case MINUS:
    case BIT_AND: {
      new_unop->unop_right = unop_right;
      new_unop->unop_op = node->unop_op;
      break;
    }
    default: {
      char* err = calloc(200, sizeof(char));
      sprintf(err, "cannot use operator '%s' for %s", token_type_to_string(node->unop_op), ast_type_to_string(unop_right->ast_type));
    // init_error_object(char *message, const char *path, char *line_message, long line, long column)
      printf("Length: %ld\n", runtime->parser->lexer->lines->used);
      Quickk_ErrorT* error = init_error_object(err, runtime->parser->lexer->path, runtime->parser->lexer->lines->items[node->unop_right->token->position->line-1], node->unop_right->token->position->line, node->unop_right->token->position->column);
      fprintf(stderr, "%s", Error_AsString(error));
      exit(1);
    }
  }
  return new_unop;
}
AST_T *quickk_runtime_visit_list_access(Runtime_T *runtime, AST_T *node){};
AST_T *quickk_runtime_visit_if(Runtime_T *runtime, AST_T *node){};
AST_T *quickk_runtime_visit_for(Runtime_T *runtime, AST_T *node){};
AST_T *quickk_runtime_visit_while(Runtime_T *runtime, AST_T *node){};
AST_T *quickk_runtime_visit_null(Runtime_T *runtime, AST_T *node){};
AST_T *quickk_runtime_visit_return(Runtime_T *runtime, AST_T *node){}
AST_T *quickk_runtime_visit_access(Runtime_T *runtime, AST_T *node){
  AST_T* access = init_ast_expr(AST_ACCESS);
  access->binop_left = quickk_runtime_visit(runtime, node->binop_left);
  switch (access->binop_left->ast_type) {
    case AST_INT: {
      if(strcmp(node->binop_right->function_call_name->identifier_value, "lol") == 0) {
        printf("It worked!");
        exit(1);
      }
      break;
    }
  }
}
AST_T *quickk_runtime_visit_break(Runtime_T *runtime, AST_T *node){}
AST_T *quickk_runtime_visit_continue(Runtime_T *runtime, AST_T *node){}
AST_T *quickk_runtime_visit_new(Runtime_T *runtime, AST_T *node);

AST_T *quickk_runtime_visit_pointer(Runtime_T *runtime, AST_T *node){}
AST_T *quickk_runtime_visit_type_conversion(Runtime_T *runtime, AST_T *node){}
void quickk_runtime_expect_arguments(Runtime_T *runtime, list_T *args, int argc,
                                     int arguments[], const char *fname){

  if (argc != args->used) {
    if(argc == -1) goto end;
    fprintf(stderr, "Expected (%d) arguments, but got (%d)", argc, args->used);
    exit(1);
  }
  end:

  for(int i = 0; i < argc; i++) {
    if (arguments[i] != ((AST_T*)args->items[i])->ast_type) {
      fprintf(stderr, "Expected (%d)  but got (%d)", arguments[i], ((AST_T*)args->items[i])->ast_type);
     exit(1);
    }
  }
}