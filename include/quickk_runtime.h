#include "../include/quickk_parser.h"
#include "../include/quickk_AST.h"
#include "../include/stack_frame.h"
typedef struct QUICKK_RUNTIME_STRUCT {
  struct QUICKK_SCOPE_STRUCT* global_scope;
  struct QUICKK_PARSER_STRUCT* parser;
} Runtime_T;

Runtime_T* init_quickk_runtime(struct QUICKK_SCOPE_STRUCT* scope);
AST_T* quickk_runtime_visit(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_integer(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_float(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_char(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_string(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_boolean(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_identifier(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_array_value(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_function_call(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_function_definition(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_class_definition(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_variable(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_variable_definition(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_compound(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_binary_operation(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_unary_operation(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_list_access(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_if(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_for(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_while(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_null(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_return(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_access(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_break(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_continue(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_new(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_pointer(Runtime_T* runtime, AST_T* node);
AST_T* quickk_runtime_visit_type_conversion(Runtime_T* runtime, AST_T* node);
void quickk_runtime_expect_arguments(Runtime_T* runtime, list_T* args, int argc, int arguments[], const char* fname);