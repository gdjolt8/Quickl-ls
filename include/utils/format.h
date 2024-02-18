#ifndef QUICKK_FORMAT_H
#define QUICKK_FORMAT_H
#include "list.h"
char *format_literal(char* str);
int is_digit_char(char c);
int is_alpha_char(char c);
int is_alnum_char(char c);
int is_whitespace_char (char c);
char *char_to_string_literal(char c);
list_T* string_to_hex_string(char* instr);
#endif