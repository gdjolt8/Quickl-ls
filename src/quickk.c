#include "../include/quickk.h"
#include "../include/main.h"
#include "../include/quickk_lexer.h"
#include "../include/quickk_runtime.h"
#include "../include/quickk_assembler.h"
#include "../include/file_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils/ftime.h"

void Quickk_CompileFile(const char *path, char *contents) {
  long long start_of_execution = unix_milliseconds();
  Lexer_T *lexer = init_quickk_lexer(contents);
  lexer->path = path;
  Parser_T* parser = init_quickk_parser(lexer);
  AST_T* parsed_root = quickk_parser_parse_block_statement(parser);
  AST_T* main_function = NULL;
  int main_function_exists = 0;
  for(int i = 0; i < parsed_root->compound_children->used; i++) {
    AST_T* stmt = parsed_root->compound_children->items[i];
    if (stmt->ast_type == AST_FUNCTION_DEFINITION) {
      if (strcmp(stmt->function_definition_name->identifier_value, "main") == 0) {
        main_function = stmt;
        main_function_exists = 1;
      }
    }
  }
  Runtime_T* runtime = init_quickk_runtime(parser->global_scope);
  runtime->parser = parser;
  AST_T* root = quickk_runtime_visit(runtime, parsed_root);
  quickk_runtime_visit_function_definition(runtime, main_function);
  AST_T* fncall = init_ast_expr(AST_FUNCTION_CALL);
  fncall->function_call_name = init_ast_expr(AST_IDENT);
  fncall->function_call_name->identifier_value = "main";
  fncall->scope = parser->global_scope;
  fncall->token = main_function->token;
  AST_T* fn = quickk_runtime_visit_function_call(runtime, fncall);
  /*char* s = assembled_root(root, init_list(sizeof(AST_T*)));
  FS_WriteFileContents("as.s", s);
  FS_ShellCommand("gcc as.s -no-pie -W -o main_as");*/

}