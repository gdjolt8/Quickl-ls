#ifndef QUICKK_AST_H
#define QUICKK_AST_H
#include "utils/list.h"
#include "quickk_data_type.h"
#include "quickk_token.h"
#include <stdint.h>
#include "../include/stack_frame.h"
enum AST_Type {
  AST_COMPOUND = 1,
  AST_INT,
  AST_FLOAT,
  AST_CHAR,
  AST_STRING,
  AST_BOOLEAN,
  AST_ARRAY,
  AST_IDENT,
  AST_VOID,
  AST_VARIABLE,
  AST_VARIABLE_DEFINITION,
  AST_FUNCTION_CALL,
  AST_FUNCTION_DEFINITION,
  AST_IF,
  AST_FOR,
  AST_WHILE,
  AST_RETURN,
  AST_CLASS,
  AST_BINOP,
  AST_UNOP,
  AST_NULL,
  AST_NEW,
  AST_ACCESS,
  AST_LIST_ACCESS,
  AST_BREAK,
  AST_CONTINUE,
  AST_TYPE_CONVERSION,
  AST_POINTER,
  AST_CONSTRUCTOR,
  AST_THIS,
  AST_CLASSPTR,
  AST_CLASS_IDENT,
  AST_NOOP
};

typedef struct QUICKK_AST_STRUCT {
  int ast_type;
  unsigned int is_global;
  struct QUICKK_DATA_TYPE_STRUCT* data_type;
  struct LIST_STRUCT* compound_children;
  struct LIST_STRUCT* array_children;
  struct QUICKL_TOKEN_STRUCT* token;
  struct QUICKK_AST_STRUCT* function_definition_name;
  struct LIST_STRUCT* function_definition_arguments;
  struct QUICKK_AST_STRUCT* function_definition_body;
  struct QUICKK_AST_STRUCT* function_definition_return_value;
  char *string_value;
  unsigned int boolean_value;
  char *identifier_value;
  char char_value;

  struct QUICKK_AST_STRUCT* variable_name;

  struct QUICKK_AST_STRUCT* function_call_name;
  struct LIST_STRUCT* function_call_arguments;

  struct QUICKK_AST_STRUCT* variable_definition_value;
  struct QUICKK_AST_STRUCT* variable_definition_name;
  int variable_is_global;
  uint8_t u8_value;
  uint16_t u16_value;
  uint32_t u32_value;
  uint64_t u64_value;
  uintmax_t u128_value;

  int8_t i8_value;
  int16_t i16_value;
  int32_t i32_value;
  int64_t i64_value;
  intmax_t i128_value;

  float f32_value;
  double f64_value;
  long double f128_value;

  struct QUICKK_AST_STRUCT* binop_left;
  enum TokenType binop_op;
  struct QUICKK_AST_STRUCT* binop_right;

  struct QUICKK_AST_STRUCT* unop_right;
  enum TokenType unop_op;

  struct QUICKK_AST_STRUCT* class_name;
  list_T* class_variable_definitions;
  list_T* class_function_definitions;
  list_T* class_constructors;
  list_T* class_types;
  struct QUICKK_AST_STRUCT* class_body;
   struct QUICKK_AST_STRUCT* if_condition;
  struct QUICKK_AST_STRUCT* if_body;
  struct QUICKK_AST_STRUCT* else_body;

  struct QUICKK_AST_STRUCT* for_variable;
  struct QUICKK_AST_STRUCT* for_iterator;
  struct QUICKK_AST_STRUCT* for_body;

  struct QUICKK_AST_STRUCT* while_condition;
  struct QUICKK_AST_STRUCT* while_body;

  struct QUICKK_AST_STRUCT* return_value;

  struct QUICKK_AST_STRUCT* new_value;

  struct QUICKK_AST_STRUCT* list_access_pointer;
  struct QUICKK_AST_STRUCT* list_value;

  struct QUICKK_AST_STRUCT* type_conversion_to;
  
  char *pointer_address_str;
  struct QUICKK_AST_STRUCT* point_to;

  struct QUICKK_AST_STRUCT* class_identifier;
  list_T* class_identifier_types;
  struct QUICKK_SCOPE_STRUCT* scope;

  int stack_index;
  StackFrame_T* stack_frame;
} AST_T;

AST_T* init_ast_expr(int type);
char const* ast_type_to_string(int type);
void free_ast_expr(AST_T* ast);
#endif