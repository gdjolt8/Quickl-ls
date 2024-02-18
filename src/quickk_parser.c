#include "../include/quickk_parser.h"
#include "../include/main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
Parser_T *init_quickk_parser(Lexer_T *lexer) {
  Parser_T *parser = (Parser_T *)calloc(1, sizeof(Parser_T));
  parser->lexer = lexer;
  parser->current_token = NULL;
  parser->peek_token = NULL;
  parser->parsing_errors = init_list();
  parser->parsing_warnings = init_list();
  parser->global_scope = init_quickk_scope();
  quickk_parser_next_token(parser);
  quickk_parser_next_token(parser);
  return parser;
}
void quickk_parser_next_token(Parser_T *parser) {
  parser->current_token = parser->peek_token;
  parser->peek_token = quickk_lexer_read_next_token(parser->lexer);
  if (parser->lexer->error_list->used > 0) {
    fprintf(stderr, "%s", Error_AsString(parser->lexer->error_list->items[0]));
  }
}
void quickk_parser_add_error(Parser_T *parser, char *error) {
  quickk_lexer_add_error_message(parser->lexer, error);
}
void quickk_parser_add_warning(Parser_T *parser, char *warning) {
  add_element_list(parser->parsing_warnings, warning);
}
void quickk_parser_match(Parser_T *parser, enum TokenType kind) {
  if (kind != parser->current_token->type) {
    char *buf = calloc(128, sizeof(char));
    sprintf(buf, "Expected '%s', but got '%s'.", token_type_to_string(kind),
            token_type_to_string(parser->current_token->type));
    char *buffer = Error_AsString(init_error_object(
        buf, parser->lexer->path, "", parser->lexer->line_offset,
        parser->lexer->char_offset));
    quickk_parser_add_error(parser, buffer);
  }
  quickk_parser_next_token(parser);
}
int quickk_parser_current_token_is(Parser_T *parser, enum TokenType kind) {
   return parser->current_token->type == kind;
}
int quickk_parser_peek_token_is(Parser_T *parser, enum TokenType kind) {
  return parser->peek_token->type == kind;
}
int quickk_parser_is_current_token_a_datatype(Parser_T *parser) {
  return parser->current_token->type > DT_START &&
         parser->current_token->type < DT_END;
}
AST_T *quickk_parser_parse_integer(Parser_T *parser) {
  AST_T *integer = init_ast_expr(AST_INT);
  integer->token = parser->current_token;
  integer->data_type = init_data_type(DT_I32);

  integer->i32_value = strtoll(parser->current_token->value, NULL, 10);
  integer->scope = parser->global_scope;
  return integer;
}
AST_T *quickk_parser_parse_float(Parser_T *parser) {
  AST_T *float_lit = init_ast_expr(AST_FLOAT);
  float_lit->token = parser->current_token;
  float_lit->data_type = init_data_type(DT_I32);

  float_lit->f64_value = strtod(parser->current_token->value, NULL);
  float_lit->scope = parser->global_scope;
  return float_lit;
}
AST_T *quickk_parser_parse_character(Parser_T *parser) {
  AST_T *character = init_ast_expr(AST_CHAR);
  character->token = parser->current_token;
  character->data_type = init_data_type(DT_CHAR);
  character->char_value = parser->current_token->value[0];
  character->scope = parser->global_scope;
  return character;
}
AST_T *quickk_parser_parse_string_literal(Parser_T *parser) {
  AST_T *string = init_ast_expr(AST_STRING);
  string->token = parser->current_token;
  string->data_type = init_data_type(DT_STRING);
  string->string_value = parser->current_token->value;
  string->scope = parser->global_scope;
  return string;
}
AST_T *quickk_parser_parse_array(Parser_T *parser) {
  quickk_parser_match(parser, LBRACK);
  AST_T *array = init_ast_expr(AST_ARRAY);
  array->token = parser->current_token;
  array->scope = parser->global_scope;
  array->data_type = init_data_type(DT_ARRAYTYPE);
  if (!quickk_parser_current_token_is(parser, RBRACK)) {
    AST_T *expr = quickk_parser_parse_expr(parser);
    add_element_list(array->array_children, expr);
  }

  while (quickk_parser_current_token_is(parser, COMMA)) {
    quickk_parser_match(parser, COMMA);
    add_element_list(array->array_children, quickk_parser_parse_expr(parser));
  }
  quickk_parser_match(parser, RBRACK);
  return array;
}
AST_T *quickk_parser_parse_boolean(Parser_T *parser) {
  AST_T *boolean = init_ast_expr(AST_BOOLEAN);
  boolean->boolean_value = quickk_parser_current_token_is(parser, KW_TRUE);
  boolean->token = parser->current_token;
  boolean->data_type = init_data_type(DT_BOOL);
  boolean->scope = parser->global_scope;
  return boolean;
}
AST_T *quickk_parser_parse_identifier(Parser_T *parser) {
  AST_T *identifier = init_ast_expr(AST_IDENT);
  identifier->token = parser->current_token;
  identifier->identifier_value = parser->current_token->value;
  identifier->scope = parser->global_scope;
  return identifier;
}
AST_T *quickk_parser_parse_variable(Parser_T *parser) {
  AST_T *variable = init_ast_expr(AST_VARIABLE);
  variable->variable_name = quickk_parser_parse_identifier(parser);
  variable->scope = parser->global_scope;
  variable->token = parser->current_token;
  if (quickk_parser_peek_token_is(parser, LPAREN))
    return quickk_parser_parse_function_call(parser);

  quickk_parser_next_token(parser);
  while (quickk_parser_current_token_is(parser, EQ)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = variable;
    ast->binop_right = quickk_parser_parse_expr(parser);
    variable = ast;
  }

  if (quickk_parser_current_token_is(parser, DOT)) {
    AST_T *ast = init_ast_expr(AST_ACCESS);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = variable;
    ast->binop_right = quickk_parser_parse_factor(parser);
    variable = ast;
  }
  return variable;
}
AST_T *quickk_parser_parse_if_statement(Parser_T *parser) {
  AST_T *if_statement = init_ast_expr(AST_IF);
  if_statement->token = parser->current_token;
  quickk_parser_match(parser, KW_IF);
  if_statement->scope = parser->global_scope;
  if (quickk_parser_current_token_is(parser, LPAREN)) {
    quickk_parser_add_warning(parser, "LPAREN not needed in if statement.");
    quickk_parser_next_token(parser);
  }
  if_statement->if_condition = quickk_parser_parse_expr(parser);
  if (quickk_parser_current_token_is(parser, RPAREN)) {
    quickk_parser_add_warning(parser, "RPAREN not needed in if statement.");
    quickk_parser_next_token(parser);
  }
  if (quickk_parser_current_token_is(parser, KW_ELSE)) {
    quickk_parser_match(parser, KW_ELSE);
    if (quickk_parser_current_token_is(parser, KW_IF)) {
      if_statement->else_body = quickk_parser_parse_if_statement(parser);
    } else {
      if_statement->else_body = quickk_parser_parse_block_statement(parser);
    }
  }
  return if_statement;
}
AST_T *quickk_parser_parse_for_statement(Parser_T *parser) {
  AST_T *for_statement = init_ast_expr(AST_FOR);
  for_statement->token = parser->current_token;
  quickk_parser_match(parser, KW_FOR);
  for_statement->scope = parser->global_scope;
  if (quickk_parser_current_token_is(parser, LPAREN)) {
    quickk_parser_add_warning(parser, "LPAREN not needed in for statement.");
    quickk_parser_next_token(parser);
  }
  DataType_T *dt = quickk_parser_parse_datatype(parser);

  for_statement->for_variable = quickk_parser_parse_variable(parser);
  for_statement->for_variable->data_type = dt;
  quickk_parser_match(parser, COLON);
  for_statement->for_iterator = quickk_parser_parse_expr(parser);
  if (quickk_parser_current_token_is(parser, RPAREN)) {
    quickk_parser_add_warning(parser, "RPAREN not needed in for statement.");
    quickk_parser_next_token(parser);
  }
  for_statement->for_body = quickk_parser_parse_block_statement(parser);
  return for_statement;
}
AST_T *quickk_parser_parse_while_statement(Parser_T *parser) {
  AST_T *while_statement = init_ast_expr(AST_WHILE);
  while_statement->token = parser->current_token;
  quickk_parser_match(parser, KW_WHILE);
  while_statement->scope = parser->global_scope;
  if (quickk_parser_current_token_is(parser, LPAREN)) {
    quickk_parser_add_warning(parser, "LPAREN not needed in while statement.");
    quickk_parser_next_token(parser);
  }
  while_statement->for_iterator = quickk_parser_parse_expr(parser);
  if (quickk_parser_current_token_is(parser, RPAREN)) {
    quickk_parser_add_warning(parser, "RPAREN not needed in while statement.");
    quickk_parser_next_token(parser);
  }
  while_statement->else_body = quickk_parser_parse_block_statement(parser);
  return while_statement;
}
AST_T *quickk_parser_parse_class_statement(Parser_T *parser) {
  AST_T *class = init_ast_expr(AST_CLASS);
  class->token = parser->current_token;
  quickk_parser_match(parser, KW_CLASS);
  class->class_name = quickk_parser_parse_identifier(parser);
  quickk_parser_match(parser, IDENTIFIER);

  if (quickk_parser_current_token_is(parser, LT)) {
    list_T *class_types = init_list();
    quickk_parser_match(parser, LT);
    if (!quickk_parser_current_token_is(parser, GT)) {
      quickk_parser_match(parser, IDENTIFIER);
      add_element_list(class_types, quickk_parser_parse_identifier(parser));
    }

    while (quickk_parser_current_token_is(parser, COMMA)) {
      quickk_parser_match(parser, COMMA);
      add_element_list(class_types, quickk_parser_parse_identifier(parser));
      quickk_parser_match(parser, IDENTIFIER);
    }
    class->class_types = class_types;
    quickk_parser_match(parser, GT);
  }
  class->class_body = quickk_parser_parse_block_statement(parser);
  for (int i = 0; i < class->class_body->compound_children->used; i++) {
    AST_T *expr = class->class_body->compound_children->items[i];

    if (expr->ast_type == AST_VARIABLE_DEFINITION)
      add_element_list(class->class_variable_definitions, expr);
    if (expr->ast_type == AST_FUNCTION_DEFINITION)
      add_element_list(class->class_function_definitions, expr);
    if (expr->ast_type == AST_CONSTRUCTOR)
      add_element_list(class->class_constructors, expr);
  }
  return class;
}
AST_T *quickk_parser_parse_block_statement(Parser_T *parser) {

  AST_T *compound = init_ast_expr(AST_COMPOUND);
  compound->token = parser->current_token;
  compound->scope = parser->global_scope;
  if (!quickk_parser_current_token_is(parser, LBRACE)) {
    while (!quickk_parser_current_token_is(parser, EOF)) {
      AST_T *stmt = quickk_parser_parse_statement(parser);
      stmt->scope = compound->scope;
      add_element_list(compound->compound_children, stmt);
      if (quickk_parser_current_token_is(parser, SEMICOLON))
        quickk_parser_match(parser, SEMICOLON);
    }
    return compound;
  }
  quickk_parser_match(parser, LBRACE);
  while (!quickk_parser_current_token_is(parser, RBRACE)) {
    AST_T *stmt = quickk_parser_parse_statement(parser);
    stmt->scope = compound->scope;
    if (stmt)
      add_element_list(compound->compound_children, stmt);
    if (quickk_parser_current_token_is(parser, SEMICOLON))
      quickk_parser_match(parser, SEMICOLON);
  }
  quickk_parser_match(parser, RBRACE);
  return compound;
}
AST_T *quickk_parser_parse_variable_definition(Parser_T *parser) {
  AST_T *variable_definition = init_ast_expr(AST_VARIABLE_DEFINITION);
  variable_definition->token = parser->current_token;
  variable_definition->scope = parser->global_scope;
  variable_definition->variable_definition_name =
      quickk_parser_parse_identifier(parser);
  
  quickk_parser_match(parser, IDENTIFIER);
  quickk_parser_match(parser, COLON);
  variable_definition->data_type = quickk_parser_parse_datatype(parser);
  quickk_parser_match(parser, EQ);
  variable_definition->variable_definition_value =
      quickk_parser_parse_expr(parser);
  switch (variable_definition->variable_definition_value->ast_type) {
  case AST_INT: {
    switch (variable_definition->data_type->data_type) {
    case DT_I8: {
      variable_definition->variable_definition_value->i8_value = (int8_t)strtol(
          variable_definition->variable_definition_value->token->value, NULL,
          10);
      break;
    }
    case DT_U8: {
      variable_definition->variable_definition_value->u8_value =
          strtol(variable_definition->variable_definition_value->token->value,
                 NULL, 10);
      break;
    }
    case DT_I16: {
      variable_definition->variable_definition_value->i16_value = (int16_t)atoi(
          variable_definition->variable_definition_value->token->value);
      break;
    }
    case DT_U16: {
      variable_definition->variable_definition_value->u16_value =
          (uint16_t)strtoul(
              variable_definition->variable_definition_value->token->value,
              NULL, 10);
      break;
    }
    case DT_I32: {
      variable_definition->variable_definition_value->i32_value =
          atoi(variable_definition->variable_definition_value->token->value);
      break;
    }
    case DT_U32: {
      variable_definition->variable_definition_value->u32_value =
          (uint32_t)strtoul(
              variable_definition->variable_definition_value->token->value,
              NULL, 10);
      break;
    }
    case DT_I64: {
      variable_definition->variable_definition_value->i64_value =
          strtoll(variable_definition->variable_definition_value->token->value,
                  NULL, 10);
      break;
    }
    case DT_U64: {
      variable_definition->variable_definition_value->u64_value =
          strtoul(variable_definition->variable_definition_value->token->value,
                  NULL, 10);
      break;
    }
    case DT_I128: {
      variable_definition->variable_definition_value->i128_value =
          atoll(variable_definition->variable_definition_value->token->value);
      break;
    }
    case DT_U128: {
      variable_definition->variable_definition_value->u128_value =
          strtoull(variable_definition->variable_definition_value->token->value,
                   NULL, 10);
      break;
    }
    }
  }
    case AST_FLOAT: {
      switch (variable_definition->data_type->data_type) {
        case DT_F32: {
          variable_definition->variable_definition_value->f32_value = strtof(
              variable_definition->variable_definition_value->token->value, NULL);
          break;
        }
        case DT_F64: {
          variable_definition->variable_definition_value->f64_value = strtod(
              variable_definition->variable_definition_value->token->value, NULL);
          break;
        }
      }
    } 
  }
  variable_definition->variable_definition_value->data_type =
      variable_definition->data_type;
  quickk_parser_match(parser, SEMICOLON);
  return variable_definition;
}
AST_T *quickk_parser_parse_source_code(Parser_T *parser) {
  AST_T *compound = init_ast_expr(AST_COMPOUND);
  compound->is_global = 1;
  compound->scope = parser->global_scope;

  while (!quickk_parser_current_token_is(parser, EOF)) {
    AST_T *statement = quickk_parser_parse_statement(parser);
    statement->scope = compound->scope;
    if (quickk_parser_current_token_is(parser, SEMICOLON)) {
      quickk_parser_match(parser, SEMICOLON);
    }
    add_element_list(compound->compound_children, statement);
  }
  return compound;
}
AST_T *quickk_parser_parse_return_statement(Parser_T *parser) {
  AST_T *ret = init_ast_expr(AST_RETURN);
  ret->scope = parser->global_scope;
  quickk_parser_match(parser, KW_RETURN);
  ret->return_value = quickk_parser_parse_expr(parser);
  return ret;
}
AST_T *quickk_parser_parse_new_statement(Parser_T *parser) {
  AST_T *new_statement = init_ast_expr(AST_NEW);
  new_statement->scope = parser->global_scope;
  quickk_parser_match(parser, KW_NEW);
  new_statement->new_value = quickk_parser_parse_expr(parser);
  return new_statement;
}
AST_T *quickk_parser_parse_null_expr(Parser_T *parser) {
  AST_T *null = init_ast_expr(AST_NULL);
  quickk_parser_match(parser, KW_NULL);
  return null;
}
AST_T *quickk_parser_parse_function_call(Parser_T *parser) {
  AST_T *function_call = init_ast_expr(AST_FUNCTION_CALL);
  function_call->scope = parser->global_scope;
  function_call->function_call_name = quickk_parser_parse_identifier(parser);
  quickk_parser_match(parser, IDENTIFIER);
  quickk_parser_match(parser, LPAREN);
  if (!quickk_parser_current_token_is(parser, RPAREN)) {
    AST_T *expr = quickk_parser_parse_expr(parser);
    add_element_list(function_call->function_call_arguments, expr);
  }

  while (quickk_parser_current_token_is(parser, COMMA)) {
    quickk_parser_match(parser, COMMA);
    AST_T *expr = quickk_parser_parse_expr(parser);
    add_element_list(function_call->function_call_arguments, expr);
  }
  quickk_parser_match(parser, RPAREN);
  return function_call;
}
AST_T *quickk_parser_parse_function_definition(Parser_T *parser) {
  AST_T *function_definition = init_ast_expr(AST_FUNCTION_DEFINITION);
  function_definition->token = parser->current_token;
  quickk_parser_match(parser, KW_FUNC);
  bool is_void = true;
  DataType_T *dt = NULL;
  if (quickk_parser_current_token_is(parser, LPAREN)) {
    quickk_parser_match(parser, LPAREN);
    dt = quickk_parser_parse_datatype(parser);
    quickk_parser_match(parser, RPAREN);
    is_void = false;
  }

  function_definition->scope = parser->global_scope;
  function_definition->data_type = (is_void ? init_data_type(DT_VOID) : dt);
  function_definition->return_value = init_ast_expr(AST_VOID);
  function_definition->return_value->data_type = init_data_type(DT_VOID);
  function_definition->function_definition_name =
      quickk_parser_parse_identifier(parser);
  quickk_parser_match(parser, IDENTIFIER);
  function_definition->function_definition_arguments =
      quickk_parser_parse_argument_list(parser);
  function_definition->function_definition_body =
      quickk_parser_parse_block_statement(parser);

  for (int i = 0;
       i <
       function_definition->function_definition_body->compound_children->used;
       i++) {
    AST_T *stmt = function_definition->function_definition_body
                      ->compound_children->items[i];
    if (stmt->ast_type == AST_RETURN)
      function_definition->function_definition_return_value =
          stmt->return_value;
  }

  return function_definition;
}
AST_T *quickk_parser_parse_statement(Parser_T *parser) {
  switch (parser->current_token->type) {
  case KW_IF:
    return quickk_parser_parse_if_statement(parser);
  case KW_FOR:
    return quickk_parser_parse_for_statement(parser);
  case KW_WHILE:
    return quickk_parser_parse_while_statement(parser);
  case KW_FUNC:
    return quickk_parser_parse_function_definition(parser);
  case KW_CLASS:
    return quickk_parser_parse_class_statement(parser);
  case KW_RETURN:
    return quickk_parser_parse_return_statement(parser);
  case KW_BREAK:
    return quickk_parser_parse_break(parser);
  case KW_CONTINUE:
    return quickk_parser_parse_continue(parser);
  case IDENTIFIER: {
    if (quickk_parser_peek_token_is(parser, COLON)) {
      return quickk_parser_parse_variable_definition(parser);
    }
    return quickk_parser_parse_variable(parser);
  }
  case KW_NEW: {
    quickk_parser_next_token(parser);
    list_T *list = quickk_parser_parse_argument_list(parser);
    if (quickk_parser_current_token_is(parser, LBRACE)) {
      AST_T *cons = init_ast_expr(AST_CONSTRUCTOR);
      cons->function_definition_arguments = list;
      cons->function_definition_body =
          quickk_parser_parse_block_statement(parser);
      return cons;
    }
  }

  default: {
    return quickk_parser_parse_expr(parser);
  }
  }
}
AST_T *quickk_parser_parse_factor(Parser_T *parser) {
  while (
         quickk_parser_current_token_is(parser, BIT_AND) ||
         quickk_parser_current_token_is(parser, NOT)) {
    AST_T *unop = init_ast_expr(AST_UNOP);
    unop->unop_op = parser->current_token->type;
    unop->scope = parser->global_scope;
    quickk_parser_next_token(parser);
    unop->unop_right = quickk_parser_parse_term(parser);
    return unop;
  }

  AST_T *factor = NULL;
  bool move = true;
  if (quickk_parser_current_token_is(parser, IDENTIFIER)) {

    factor = quickk_parser_parse_variable(parser);
    move = false;
  }

  if (quickk_parser_current_token_is(parser, LPAREN)) {
    quickk_parser_match(parser, LPAREN);
    factor = quickk_parser_parse_expr(parser);
    quickk_parser_match(parser, RPAREN);
    return factor;
  }

  if (quickk_parser_is_current_token_a_datatype(parser)) {
    DataType_T *data_type = quickk_parser_parse_datatype(parser);
    quickk_parser_next_token(parser);
    quickk_parser_match(parser, COLON);

    AST_T *type_conversion = init_ast_expr(AST_TYPE_CONVERSION);
    type_conversion->scope = parser->global_scope;
    type_conversion->data_type = data_type;
    type_conversion->type_conversion_to = quickk_parser_parse_expr(parser);

    return type_conversion;
  }

  if (quickk_parser_current_token_is(parser, KW_TRUE) ||
      quickk_parser_current_token_is(parser, KW_FALSE)) {
    factor = quickk_parser_parse_boolean(parser);
  } else if (quickk_parser_current_token_is(parser, INT)) {
    factor = quickk_parser_parse_integer(parser);
  } else if (quickk_parser_current_token_is(parser, FLOAT)) {
    factor = quickk_parser_parse_float(parser);
  } else if (quickk_parser_current_token_is(parser, CHAR)) {
    factor = quickk_parser_parse_character(parser);
  } else if (quickk_parser_current_token_is(parser, STRING)) {
    factor = quickk_parser_parse_string_literal(parser);
  } else if (quickk_parser_current_token_is(parser, LBRACK)) {
    factor = quickk_parser_parse_array(parser);
  } else if (quickk_parser_current_token_is(parser, KW_NEW)) {
    factor = quickk_parser_parse_new_statement(parser);
  }
  int last = parser->current_token->type;
  if (move)
    quickk_parser_next_token(parser);
  char *string =
      (char *)calloc(36 + strlen(token_type_to_string(last)), sizeof(char));
  sprintf(string, "Unexpected expression: '%s'\n", token_type_to_string(last));
  if (factor == NULL) {
    char *buffer = Error_AsString(init_error_object(
        string, parser->lexer->path, "", parser->lexer->line_offset,
        parser->lexer->char_offset));
    quickk_parser_add_error(parser, buffer);
  }
  return factor;
}
AST_T *quickk_parser_parse_term(Parser_T *parser) {
  AST_T *node = quickk_parser_parse_factor(parser);
  while (quickk_parser_current_token_is(parser, ASTERISK) ||
         quickk_parser_current_token_is(parser, SLASH)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_factor(parser);
    node = ast;
  }

  while (quickk_parser_current_token_is(parser, DOT)) {
    AST_T *ast = init_ast_expr(AST_ACCESS);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_factor(parser);
    node = ast;
  }

  while (quickk_parser_current_token_is(parser, SHL) ||
         quickk_parser_current_token_is(parser, SHR) ||
         quickk_parser_current_token_is(parser, MODULO)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_factor(parser);
    node = ast;
  }

  while (quickk_parser_current_token_is(parser, LBRACK)) {
    AST_T *ast = init_ast_expr(AST_LIST_ACCESS);
    ast->scope = parser->global_scope;
    quickk_parser_match(parser, LBRACK);
    ast->list_value = node;
    ast->list_access_pointer = quickk_parser_parse_expr(parser);
    quickk_parser_match(parser, RBRACK);
    node = ast;
  }
  return node;
}
AST_T *quickk_parser_parse_expression(Parser_T *parser) {
  AST_T *node = quickk_parser_parse_term(parser);
  
  while (quickk_parser_current_token_is(parser, PLUS) ||
         quickk_parser_current_token_is(parser, MINUS)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_term(parser);
    node = ast;
  }

  while (quickk_parser_current_token_is(parser, EQ)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_expr(parser);
    node = ast;
  }

  return node;
}
AST_T *quickk_parser_parse_expr(Parser_T *parser) {
  AST_T *node = quickk_parser_parse_expression(parser);

  while (quickk_parser_current_token_is(parser, AND) ||
         quickk_parser_current_token_is(parser, BANGEQ) ||
         quickk_parser_current_token_is(parser, OR)) {
    AST_T *ast = init_ast_expr(AST_BINOP);
    ast->scope = parser->global_scope;
    ast->binop_op = parser->current_token->type;
    quickk_parser_next_token(parser);
    ast->binop_left = node;
    ast->binop_right = quickk_parser_parse_expression(parser);
    node = ast;
  }

  return node;
}
AST_T *quickk_parser_parse_break(Parser_T *parser) {
  quickk_parser_match(parser, KW_BREAK);
  AST_T *brk = init_ast_expr(AST_BREAK);
  return brk;
}
AST_T *quickk_parser_parse_continue(Parser_T *parser) {
  quickk_parser_match(parser, KW_CONTINUE);
  AST_T *cnt = init_ast_expr(AST_CONTINUE);
  return cnt;
}
AST_T *quickk_parser_parse_this(Parser_T *parser);
list_T *quickk_parser_parse_argument_list(Parser_T *parser) {
  list_T *list = init_list();
  quickk_parser_match(parser, LPAREN);
  if (!quickk_parser_current_token_is(parser, RPAREN)) {
    AST_T *identifier = quickk_parser_parse_identifier(parser);
    quickk_parser_match(parser, IDENTIFIER);
    quickk_parser_match(parser, COLON);
    identifier->data_type = quickk_parser_parse_datatype(parser);
    add_element_list(list, identifier);
  }

  while (quickk_parser_current_token_is(parser, COMMA)) {
    quickk_parser_match(parser, COMMA);
    AST_T* identifier = quickk_parser_parse_identifier(parser);
    quickk_parser_match(parser, IDENTIFIER);
    quickk_parser_match(parser, COLON);
    identifier->data_type = quickk_parser_parse_datatype(parser);
    add_element_list(list, identifier);
  }
  quickk_parser_match(parser, RPAREN);
  return list;
}
DataType_T *quickk_parser_parse_datatype(Parser_T *parser) {
  DataType_T *data_type = init_data_type(-1);
  if (quickk_parser_current_token_is(parser, KW_I8)) 
    data_type->data_type = DT_I8;
  else if (quickk_parser_current_token_is(parser, KW_I16))
    data_type->data_type = DT_I16;
  else if (quickk_parser_current_token_is(parser, KW_I32))
    data_type->data_type = DT_I32;
  else if (quickk_parser_current_token_is(parser, KW_I64))
    data_type->data_type = DT_I64;
  else if (quickk_parser_current_token_is(parser, KW_I128))
    data_type->data_type = DT_I128;
  else if (quickk_parser_current_token_is(parser, KW_U8))
    data_type->data_type = DT_U8;
  else if (quickk_parser_current_token_is(parser, KW_U16))
    data_type->data_type = DT_U16;
  else if (quickk_parser_current_token_is(parser, KW_U32))
    data_type->data_type = DT_U32;
  else if (quickk_parser_current_token_is(parser, KW_U64))
    data_type->data_type = DT_U64;
  else if (quickk_parser_current_token_is(parser, KW_U128))
    data_type->data_type = DT_U128;
  else if (quickk_parser_current_token_is(parser, KW_F32))
    data_type->data_type = DT_F32;
  else if (quickk_parser_current_token_is(parser, KW_F64))
    data_type->data_type = DT_F64;
  else if (quickk_parser_current_token_is(parser, KW_F128))
    data_type->data_type = DT_F128;
  else if (quickk_parser_current_token_is(parser, KW_CHAR))
    data_type->data_type = DT_CHAR;
  else if (quickk_parser_current_token_is(parser, KW_STRING))
    data_type->data_type = DT_STRING;
  else if (quickk_parser_current_token_is(parser, KW_BOOLEAN))
    data_type->data_type = DT_BOOL;
  else {
    data_type->data_type = DT_CLASS;
    data_type->node = quickk_parser_parse_class_identifier(parser);
  }

  if (quickk_parser_peek_token_is(parser, ASTERISK)) {
    quickk_parser_next_token(parser);
    while (quickk_parser_current_token_is(parser, ASTERISK)) {
      data_type->ptr = data_type;
      data_type->data_type = DT_POINTER;
      quickk_parser_match(parser, ASTERISK);
    }
  }

  if (quickk_parser_peek_token_is(parser, LBRACK)) {
    quickk_parser_next_token(parser);
    while (quickk_parser_current_token_is(parser, LBRACK)) {
      quickk_parser_match(parser, LBRACK);
      data_type->array_ptr = init_data_type_with_dt(data_type);
      data_type->data_type = DT_ARRAYTYPE;
      quickk_parser_match(parser, RBRACK);
    }
    return data_type;
  }
  quickk_parser_next_token(parser);
  return data_type;
}
AST_T *quickk_parser_parse_class_identifier(Parser_T *parser) {
  AST_T *class_ident = init_ast_expr(AST_CLASS_IDENT);
  class_ident->class_identifier = quickk_parser_parse_identifier(parser);
  if (quickk_parser_peek_token_is(parser, LT)) {
    quickk_parser_match(parser, IDENTIFIER);
    quickk_parser_match(parser, LT);
    if (!quickk_parser_current_token_is(parser, GT)) {
      DataType_T *data_type = quickk_parser_parse_datatype(parser);
      add_element_list(class_ident->class_identifier_types, data_type);
    }

    while (quickk_parser_current_token_is(parser, COMMA)) {
      quickk_parser_match(parser, COMMA);
      DataType_T *data_type = quickk_parser_parse_datatype(parser);
      add_element_list(class_ident->class_identifier_types, data_type);
    }
  }

  return class_ident;
}