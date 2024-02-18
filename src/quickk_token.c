#include "../include/quickk_token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *keywords[] = {
  "i8", "i16", "i32", "i64", "i128", "f32", "f64", "f128", "u8", "u16", "u32", "u64", "u128", "char", "string", "bool", "if", "else", "func", "for", "while", "true", "false", "return", "break", "continue", "object", "null", "enum", "class", "new"
};

enum TokenType keyword_values[] = {
  KW_I8, KW_I16, KW_I32, KW_I64, KW_I128,
  KW_F32, KW_F64, KW_F128, 
  KW_U8, KW_U16, KW_U32, KW_U64, KW_U128, KW_CHAR, KW_STRING, KW_BOOLEAN, KW_IF, KW_ELSE, KW_FUNC, KW_FOR, KW_WHILE,  KW_TRUE, KW_FALSE, KW_RETURN, KW_BREAK, KW_CONTINUE, KW_OBJECT, KW_NULL, KW_ENUM, KW_CLASS, KW_NEW
};

Token_T* init_quickk_token(char* value, int type) 
{
  Token_T* token = calloc(1, sizeof(Token_T));
  token->value = value;
  token->type = type;
  return token;
}
char const* token_type_to_string(int type) {
  switch (type) {
    case INT:
      return "integer";
    case FLOAT:
      return "float";
    case STRING:
      return "string";
    case CHAR:
      return "char";
    default: {
      char* typ = calloc(300, sizeof(char));
      sprintf(typ, "%d", type);
      return typ;
    }
  }
}

enum TokenType quickk_token_return_keyword (char *value) {
  for(int i = 0; i < sizeof(keyword_values)/sizeof(keyword_values[0]); i++) {
    if (strcmp(value, keywords[i]) == 0) {
      return keyword_values[i];
    }
  }
  return IDENTIFIER;
}