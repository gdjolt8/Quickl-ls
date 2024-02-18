#ifndef QUICKK_LEXER_H
#define QUICKK_LEXER_H

#include "../include/quickk_token.h"
#include "../include/utils/list.h"
#include "../include/quickk_error.h"
typedef struct LEXER_STRUCT
{
  const char* path;
  char* source_code;
  long current_position;
  unsigned char current_char;

  unsigned long line_offset;
  unsigned long char_offset;
  list_T* error_list;
  list_T* lines;
} Lexer_T;

Lexer_T* init_quickk_lexer(char* source);
void quickk_lexer_add_error_message(Lexer_T* lexer, char* message);
void lexer_struct_advance_char(Lexer_T* lexer);
char lexer_struct_peek_char(Lexer_T* lexer);
void quickk_lexer_skip_whitespace(Lexer_T* lexer);
void quickk_lexer_skip_comment(Lexer_T* lexer);
Token_T* quickk_lexer_read_next_token(Lexer_T* lexer);
char* quickk_lexer_read_identifier_string(Lexer_T* lexer);
char* quickk_lexer_read_integer_string(Lexer_T* lexer);
char* quickk_lexer_read_string_string(Lexer_T* lexer);
char* quickk_lexer_read_character_string(Lexer_T* lexer);
Token_T* quickk_lexer_read_number_token(Lexer_T* lexer);
#endif