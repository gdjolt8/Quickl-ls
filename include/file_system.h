#ifndef QUICKK_FILE_SYSTEM_H
#define QUICKK_FILE_SYSTEM_H

char* FS_ReadFileContents(const char* path_name);
void FS_WriteFileContents(const char* path_name, char* contents);
void FS_ShellCommand(const char* cmd);
#endif