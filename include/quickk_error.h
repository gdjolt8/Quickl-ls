#ifndef QUICKK_ERROR_H
#define QUICKK_ERROR_H

typedef struct ERROR_STRUCT {
    char* message;
    const char* path;
    char* line_message;
    long line;
    long column;
} Quickk_ErrorT;

Quickk_ErrorT* init_error_object(char* message, const char* path, char* line_message, long line, long column);
char* Error_AsString(Quickk_ErrorT* error);
#endif