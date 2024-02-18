#ifndef QUICKK_QUICKK_TOKEN_H
#define QUICKK_QUICKK_TOKEN_H
#include "../include/utils/pos.h"
enum TokenType {
  IDENTIFIER,
  INT,
  FLOAT,
  STRING,
  CHAR,

  PLUS, PLUSEQ, PLUSPLUS, MINUS, MINUSEQ, MINUSMINUS, ASTERISK, ASTERISKEQ, SLASH, SLASHEQ, MODULO, MODULOEQ,
  LBRACE, RBRACE, LBRACK, RBRACK, LPAREN, RPAREN,
  EQ, EQEQ, BANG, BANGEQ,
  COMMA, DOT, COLON, SEMICOLON,
  SHL, SHR, GT, LT, GTE, LTE,
  AND, NOT, OR,
  BIT_AND, BIT_ANDEQ, BIT_OR, BIT_OREQ, BIT_XOR, BIT_XOREQ,

  KW_START,
  DT_START,
  KW_I8 = 1 << 6, KW_I16, KW_I32, KW_I64, KW_I128,
  KW_F32, KW_F64, KW_F128, 
  KW_U8, KW_U16, KW_U32, KW_U64, KW_U128, KW_CHAR, KW_STRING, KW_BOOLEAN,
  DT_END,
  KW_IF = 1 << 7, KW_ELSE, KW_FUNC, KW_FOR, KW_WHILE,  KW_TRUE, KW_FALSE, KW_RETURN, KW_BREAK, KW_CONTINUE, KW_OBJECT, KW_NULL, KW_ENUM, KW_CLASS, KW_NEW,
  KW_END,
  ERR_TOKEN
};

typedef struct QUICKL_TOKEN_STRUCT {
  char* value;
  enum TokenType type;
  Position_T* position;
} Token_T;

Token_T* init_quickk_token(char* value, int type);
char const* token_type_to_string(int type);
enum TokenType quickk_token_return_keyword(char* value);
#endif