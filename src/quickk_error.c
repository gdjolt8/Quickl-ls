#include "../include/quickk_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE_BOLD_TEXT "\x1B[1;37m"
#define RED_BOLD_TEXT "\x1B[1;31m"
#define ORANGE_BOLD_TEXT "\x1B[1;31m]"
#define YELLOW_BOLD_TEXT "\x1B[1;33m"
#define RESET_TEXT "\x1B[0m"
Quickk_ErrorT* init_error_object(char* message, const char* path, char* line_message, long line, long column) {
    Quickk_ErrorT* error_object = calloc(1, sizeof(Quickk_ErrorT));
    
    error_object->message = message;
    error_object->path = path;
    error_object->line = line;
    error_object->line_message = line_message;
    error_object->column = column;
    return error_object;
}

char* Error_AsString(Quickk_ErrorT* error) {
    char* err_string = calloc(1024, sizeof(char));
    sprintf(err_string, "%s:%ld:%ld: " RED_BOLD_TEXT "error: " RESET_TEXT WHITE_BOLD_TEXT "%s" RESET_TEXT "\n%s\n", error->path, error->line, error->column, error->message, error->line_message);
    for(int i = 0; i < (error->column - 1); i++)
      sprintf(err_string + strlen(err_string), " ");
    sprintf(err_string + strlen(err_string), "^");
    return err_string;
}