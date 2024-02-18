#ifndef QUICKK_QUICKK_PARSER_H
#define QUICKK_QUICKK_PARSER_H

#include "quickk_lexer.h"
#include "quickk_AST.h"
#include "quickk_scope.h"
typedef struct QUICKK_PARSER_STRUCT {
  Lexer_T* lexer;
  Token_T* current_token;
  Token_T* peek_token;
  list_T* parsing_errors;
  list_T* parsing_warnings;
  struct QUICKK_SCOPE_STRUCT* global_scope;
} Parser_T;

Parser_T* init_quickk_parser(Lexer_T* lexer);
void quickk_parser_next_token(Parser_T* parser);
void quickk_parser_add_error(Parser_T* parser, char* error);
void quickk_parser_add_warning(Parser_T* parser, char* warning);
void quickk_parser_match(Parser_T* parser, enum TokenType kind);
int quickk_parser_current_token_is(Parser_T* parser, enum TokenType kind);
int quickk_parser_peek_token_is(Parser_T* parser, enum TokenType kind);
int quickk_parser_is_current_token_a_datatype(Parser_T* parser);
AST_T* quickk_parser_parse_integer(Parser_T* parser);
AST_T* quickk_parser_parse_float(Parser_T* parser);
AST_T* quickk_parser_parse_character(Parser_T* parser);
AST_T* quickk_parser_parse_string_literal(Parser_T* parser);
AST_T* quickk_parser_parse_array(Parser_T* parser);
AST_T* quickk_parser_parse_boolean(Parser_T* parser);
AST_T* quickk_parser_parse_identifier(Parser_T* parser);
AST_T* quickk_parser_parse_variable(Parser_T* parser);
AST_T* quickk_parser_parse_if_statement(Parser_T* parser);
AST_T* quickk_parser_parse_for_statement(Parser_T* parser);
AST_T* quickk_parser_parse_while_statement(Parser_T* parser);
AST_T* quickk_parser_parse_class_statement(Parser_T* parser);
AST_T* quickk_parser_parse_block_statement(Parser_T* parser);
AST_T* quickk_parser_parse_variable_definition(Parser_T* parser);
AST_T* quickk_parser_parse_source_code(Parser_T* parser);
AST_T* quickk_parser_parse_return_statement(Parser_T* parser);
AST_T* quickk_parser_parse_new_statement(Parser_T* parser);
AST_T* quickk_parser_parse_null_expr(Parser_T* parser);
AST_T* quickk_parser_parse_function_call(Parser_T* parser);
AST_T* quickk_parser_parse_function_definition(Parser_T* parser);
AST_T* quickk_parser_parse_statement(Parser_T* parser);
AST_T* quickk_parser_parse_factor(Parser_T* parser);
AST_T* quickk_parser_parse_term(Parser_T* parser);
AST_T* quickk_parser_parse_expression(Parser_T* parser);
AST_T* quickk_parser_parse_expr(Parser_T* parser);
AST_T* quickk_parser_parse_break(Parser_T* parser);
AST_T* quickk_parser_parse_continue(Parser_T* parser);
AST_T* quickk_parser_parse_this(Parser_T* parser);
list_T* quickk_parser_parse_argument_list(Parser_T* parser);
DataType_T* quickk_parser_parse_datatype(Parser_T* parser);
AST_T* quickk_parser_parse_class_identifier(Parser_T* parser);
#endif