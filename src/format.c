#include "../include/utils/format.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
char *format_literal(char* str) {
  char *result = (char*)calloc(1, sizeof(char));
  result[0] = '\0';
  for (int i = 0; i < strlen(str); i++) {
    char c;
    switch (str[i]) {
      case '\\': {
        char escape = str[i+1];
        if (escape == 'n') c = '\n';
        if (escape == 't') c = '\t';
        i++;
        break;
      }
      default: {
        c = str[i];
        break;
      }
    }
    result = (char*)realloc(result, strlen(result) + 2);
    strcat(result, (char[]){c, '\0'});
  }
  return result;
}

int is_digit_char (char c) {
  return isdigit(c);
}
int is_alpha_char (char c) {
  return isalpha(c);
}
int is_alnum_char (char c) {
  return isalnum(c);
}
int is_whitespace_char (char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\b' || c == '\r';
}

char *char_to_string_literal(char c) {
  char *buffer = (char*)calloc(2, sizeof(char));
  *buffer = c;
  *(buffer + 1) = '\0';
  return buffer;
}

char* string_to_hex(char* instr) {
  unsigned int len = strlen(instr);
  char* hexstr = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < len+1; i++)
  {
    char* newstr = calloc(4, sizeof(char));
    sprintf(newstr, "%x", instr[(len-i)]);
    hexstr = realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
    strcat(hexstr, newstr);
    free(newstr);
  }

  return hexstr;
}
list_T* string_to_hex_string(char* instr) {
  list_T* list = init_list(sizeof(char*));

  unsigned int i = 0;
  char* tmp = calloc(1, sizeof(char));
  unsigned int len = strlen(instr);
  while (instr[i] != '\0')
  {
    tmp = realloc(tmp, (strlen(tmp) + 2) * sizeof(char));
    strcat(tmp, (char[]){ instr[i], 0 });

    if (((i > 0 && (strlen(tmp) % 4 == 0)) || i >= len-1 ) || instr[i] == '\n' || instr[i] == '\t')
    {
      char* hexstr = string_to_hex(tmp);
      free(tmp);
      add_element_list(list, hexstr);
      tmp = calloc(1, sizeof(char));
    }

    i += 1;
  }

  return list;
}