#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/main.h"
#include "../include/file_system.h"
#include "../include/quickk.h"
#include "../include/quickk_assembler.h"
#define DEBUG 1
void Quickk_PrintHelpScreen() {
    fprintf(stderr, "Quickk is a programming language with focus on efficiency.\nCommands: \n\thelp: displays help screen (ie. this screen)\n\tcompile: compiles your Quickk code\n\n\nCopyright gdjolt8@2023");
}
int main(int argc, char** argv) {
  if (strcmp(argv[1], "compile") == 0 || DEBUG) {
    char* contents = FS_ReadFileContents(argc > 2 ? argv[2] : "./examples/main.qk");
    Quickk_CompileFile(argc > 2 ? argv[2] : "./examples/main.qk", contents);
    return 0;
  }
  
  if (argc < 2 || strcmp(argv[1], "help") == 0) {
    Quickk_PrintHelpScreen();
    return 0;
  }

  

}