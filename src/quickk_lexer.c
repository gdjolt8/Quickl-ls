#include "../include/quickk_lexer.h"
#include "../include/utils/format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED_BOLD_TEXT "\x1B[1;31m"
#define ORANGE_BOLD_TEXT "\x1B[1;31m]"
#define YELLOW_BOLD_TEXT "\x1B[1;33m"
#define RESET_TEXT "\x1B[0m"

Lexer_T* init_quickk_lexer(char* source) {
  Lexer_T* lexer = calloc(1, sizeof(Lexer_T));
  lexer->current_position = 0;
  lexer->source_code = source;
  lexer->char_offset = 1;
  lexer->line_offset = 1;
  lexer->error_list = init_list();
  lexer->current_char = source[lexer->current_position];
  lexer->lines = split(source, "\n");
  return lexer;
}
void quickk_lexer_add_error_message(Lexer_T* lexer, char* message) {
  Quickk_ErrorT* errmsg = init_error_object(message, lexer->path, (char*)lexer->lines->items[lexer->line_offset-1], lexer->line_offset, lexer->char_offset);
  add_element_list(lexer->error_list, errmsg);
}
void lexer_struct_advance_char(Lexer_T* lexer) {
  lexer->char_offset++;
  lexer->current_position++;
  if(lexer->current_position >= strlen(lexer->source_code)) {
    lexer->current_char = (char)0;
  } else {
    lexer->current_char = lexer->source_code[lexer->current_position];
    if(lexer->current_char == '\n') {
      lexer->line_offset++;
      lexer->char_offset = 0;
    }
  }
}
char lexer_struct_peek_char(Lexer_T* lexer) {
  if (lexer->current_position + 1 >= strlen(lexer->source_code))
    return '\0';
  return lexer->source_code[lexer->current_position + 1];
}
void quickk_lexer_skip_whitespace(Lexer_T* lexer) {
  while(is_whitespace_char(lexer->current_char)) {
    lexer_struct_advance_char(lexer);
  }
}
void quickk_lexer_skip_comment(Lexer_T* lexer) {
  if(lexer->current_char == '/') {
    if (lexer_struct_peek_char(lexer) == '/') {
      while (lexer->current_char != '\n' && lexer->current_char >= '\0') {
        lexer_struct_advance_char(lexer);
      }
      return;
    } 
    
    if (lexer_struct_peek_char(lexer) == '*') {
      while (1) {
        lexer_struct_advance_char(lexer);
        if (lexer->current_char == '*' && lexer_struct_peek_char(lexer) == '/') {
          lexer_struct_advance_char(lexer);
          lexer_struct_advance_char(lexer);
          quickk_lexer_skip_whitespace(lexer);
          return;
        }
      }
    }
  }
}
Token_T* quickk_lexer_read_next_token(Lexer_T* lexer) {
  quickk_lexer_skip_whitespace(lexer);
  quickk_lexer_skip_comment(lexer);

  Token_T* token = (void*)0;

  switch(lexer->current_char) {
    case '+': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, PLUSEQ);
      } else if(lexer_struct_peek_char(lexer) == '+') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, PLUSPLUS);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), PLUS);
      }
      break;
    }

    case '-': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, MINUSEQ);
      } else if(lexer_struct_peek_char(lexer) == '-') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, MINUSMINUS);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), MINUS);
      }
      break;
    }
    case '*': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, ASTERISKEQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), PLUS);
      }
      break;
    }
    case '/': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, SLASHEQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), SLASH);
      }
      break;
    }
    case '(': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), LPAREN);
      break;
    }
    case ')': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), RPAREN);
      break;
    }
    case '{': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), LBRACE);
      break;
    }
    case '}': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), RBRACE);
      break;
    }
    case '[': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), LBRACK);
      break;
    }
    case ']': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), RBRACK);
      break;
    }
    case '.': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), DOT);
      break;
    }
    case ',': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), COMMA);
      break;
    }
     case ':': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), COLON);
      break;
    }
    case ';': {
      token = init_quickk_token(char_to_string_literal(lexer->current_char), SEMICOLON);
      break;
    }
    case '>': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, GTE);
      } else if(lexer_struct_peek_char(lexer) == '>') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, SHR);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), GT);
      }
      break;
    }
    case '<': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, LTE);
      } else if(lexer_struct_peek_char(lexer) == '<') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, SHL);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), LT);
      }
      break;
    }
    case '!': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, BANGEQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), BANG);
      }
      break;
    }
    case '=': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, EQEQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), EQ);
      }
      break;
    }
    case '&': {
      if(lexer_struct_peek_char(lexer) == '&') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, AND);
      } else if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, BIT_ANDEQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), BIT_AND);
      }
      break;
    }
    case '|': {
      if(lexer_struct_peek_char(lexer) == '|') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, OR);
      } else if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, BIT_OREQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), BIT_OR);
      }
      break;
    }
    case '^': {
      if(lexer_struct_peek_char(lexer) == '=') {
        char* buffer = calloc(3, sizeof(char));
        buffer[0] = lexer->current_char;
        lexer_struct_advance_char(lexer);
        buffer[1] = lexer->current_char;
        buffer[2] = '\0';
        token = init_quickk_token(buffer, BIT_XOREQ);
      } else {
        token = init_quickk_token(char_to_string_literal(lexer->current_char), BIT_XOR);
      }
      break;
    }
    
    case '\0': {
      token = init_quickk_token("<EOF>", EOF);
      break;
    }

    default: {
      if(is_digit_char(lexer->current_char)) {
        token = quickk_lexer_read_number_token(lexer);
      } else if (is_alpha_char(lexer->current_char) || lexer->current_char == '_') {
        char* value = quickk_lexer_read_identifier_string(lexer);
        enum TokenType type = quickk_token_return_keyword(value);
        token = init_quickk_token(value, type);
      } else if(lexer->current_char == '\"') {
        token = init_quickk_token(quickk_lexer_read_string_string(lexer), STRING);
      } else if(lexer->current_char == '\'') {
        token = init_quickk_token(quickk_lexer_read_character_string(lexer), CHAR);
      }
      if(!token) {
        char* buffer = calloc(512, sizeof(char));
        sprintf(buffer, "syntax error");
        
        quickk_lexer_add_error_message(lexer, buffer);
        lexer_struct_advance_char(lexer);
        return init_quickk_token(char_to_string_literal(lexer->current_char), ERR_TOKEN);
      }
    }
  }
  token->position = init_position(lexer->char_offset - strlen(token->value), lexer->line_offset);
  lexer_struct_advance_char(lexer);
  return token;
}
char* quickk_lexer_read_identifier_string(Lexer_T* lexer) {
  char* value = calloc(1, sizeof(char));
  value[0] = '\0';
  while(is_alnum_char(lexer_struct_peek_char(lexer)) || lexer_struct_peek_char(lexer) == '_') {
    value = realloc(value, strlen(value)+1);
    strcat(value, char_to_string_literal(lexer->current_char));
    lexer_struct_advance_char(lexer);
  }
  strcat(value, char_to_string_literal(lexer->current_char));
  return value;
}
char* quickk_lexer_read_integer_string(Lexer_T* lexer) {
  char* value = calloc(1, sizeof(char));
  value[0] = '\0';
  while(is_digit_char(lexer_struct_peek_char(lexer))) {
    value = realloc(value, strlen(value)+1);
    strcat(value, char_to_string_literal(lexer->current_char));
    lexer_struct_advance_char(lexer);
  }
  strcat(value, char_to_string_literal(lexer->current_char));
  return value;
}
char* quickk_lexer_read_string_string(Lexer_T* lexer) {
  lexer_struct_advance_char(lexer);
  char* value = calloc(1, sizeof(char));
  value[0] = '\0';
  while(lexer->current_char != '\"') {
    if(lexer->current_char == '\0') {
      quickk_lexer_add_error_message(lexer, "String literal expands to <EOF>");
    }
    value = realloc(value, strlen(value)+1);
    strcat(value, char_to_string_literal(lexer->current_char));
    lexer_struct_advance_char(lexer);
  }
  value = format_literal(value);
  return value;
}
char* quickk_lexer_read_character_string(Lexer_T* lexer) {
  lexer_struct_advance_char(lexer);
  char* value = calloc(1, sizeof(char));
  value[0] = '\0';
  while(lexer->current_char != '\'') {
    if(lexer->current_char == '\0') {
      quickk_lexer_add_error_message(lexer, "Character literal expands to <EOF>");
    }
    value = realloc(value, strlen(value)+1);
    strcat(value, char_to_string_literal(lexer->current_char));
    lexer_struct_advance_char(lexer);
  }
  value = format_literal(value);

  if(strlen(value) == 0) {
    quickk_lexer_add_error_message(lexer, "Character literal can't be empty");
  }
  if(strlen(value)>1){
    quickk_lexer_add_error_message(lexer, "Character literal is too long, write a string instead?");
  }
  return value;
}
Token_T* quickk_lexer_read_number_token(Lexer_T* lexer) {
  char* intPart = quickk_lexer_read_integer_string(lexer);
  if(lexer_struct_peek_char(lexer) != '.') {
    return init_quickk_token(intPart, INT);
  }
  lexer_struct_advance_char(lexer);
  if (!is_digit_char(lexer->current_char)) {
    exit(6);
  }
  lexer_struct_advance_char(lexer);
  char* fracPart = quickk_lexer_read_integer_string(lexer);
  char* numString = calloc(strlen(intPart) + strlen(fracPart) + 2, sizeof(char));
  sprintf(numString, "%s.%s", intPart, fracPart);
  return init_quickk_token(numString, FLOAT);
}