#include "../include/file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* FS_ReadFileContents(const char* path_name) {
    FILE* fp = fopen(path_name, "r+");
    
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = calloc(length, length);
    fread(buffer, 1, length, fp);
    return buffer;
}
void FS_WriteFileContents(const char* path_name, char* contents) {
    FILE* fp = fopen(path_name, "wb");
    fputs(contents, fp);
    fclose(fp);
}
void FS_ShellCommand(const char* cmd) {
  char* output = (char*) calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  fp = popen(cmd, "r");

  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char*) realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
    strcat(output, path);
  }

  pclose(fp);
}