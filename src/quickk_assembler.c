#include "../include/quickk_assembler.h"
#include "../include/utils/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
int bytes_used = 0;
int str_counter = 0;

typedef struct ASSEMBLY_VARIABLE_PAIR
{
  char* label;
  AST_T* value;
  uint32_t bytes_allocated;
  uint32_t byte_label;
} AVPair;

list_T* variables;
AVPair* init_assembly_variable_pair(char* label, AST_T* value, uint32_t bytes_allocated, uint32_t byte_label) {
  AVPair* avpair = calloc(1, sizeof(AVPair));
  if (label == NULL) avpair->label = "";
  else avpair->label = label;
  avpair->value = value;
  avpair->bytes_allocated = bytes_allocated;
  avpair->byte_label = byte_label;
  return avpair;
}

AVPair* get_variable_by_name(char* name) {
  for (unsigned int i = 0; i < variables->used; i++) {
    AVPair* avpair = (AVPair*)variables->items[i];
    if (strcmp(name, avpair->label) == 0) {
      return avpair;
    }
  }
  return NULL;
}


char *assembled_compound(AST_T *ast, list_T *list) {

  const char *template = "# compound (%p)\n";
  char *value = calloc(strlen(template) + 128, sizeof(char));
  sprintf(value, template, ast);
  
  for (unsigned int i = 0; i < ast->compound_children->used; i++) {
    AST_T *child_ast = (AST_T *)ast->compound_children->items[i];
    char *next_value = assembled(child_ast, list);
    value =
        realloc(value, (strlen(value) + strlen(next_value) + 2) * sizeof(char));
    strcat(value, next_value);
  }

  return value;
}

char* check_str_existence(AST_T* ast) {
  for (unsigned int i = 0; i < variables->used; i++) {
    AVPair* avpair = (AVPair*)variables->items[i];
    if(avpair->value->ast_type != AST_STRING) continue;
    if (strcmp(avpair->value->string_value, ast->variable_definition_value->string_value) == 0) {
      return avpair->label;
    }
  }
  return NULL;
}
char* assembled_function(AST_T* ast, list_T* list) {
  char* name = ast->function_definition_name->identifier_value;
  char* template =  ".global main\n"
                    "%s:\n"
                    "\tpushq %%rbp\n" 
                    "\tmovq %%rsp, %%rbp\n";
  char* s = calloc((strlen(template) + (strlen(name)*2) + 1) + ast->function_definition_body->compound_children->used * 1000, sizeof(char));
  sprintf(s, (char*)template, name);
  AST_T* assembled_value = ast;
  for (int i = 0; i < ast->function_definition_body->compound_children->used; i++) {
    AST_T* farg = ast->function_definition_body->compound_children->items[i];
    sprintf(s + strlen(s), "\t%s\n", assembled(farg, list));
  }
  char* end_of_function = 
                        "\tmovl $0, %eax\n"
                        "\tpopq %rbp\n";
  sprintf(s + strlen(s), "%s\n", end_of_function);
  return s;
}
char *assembled_integer(AST_T *ast, list_T *list) {
  char *int_asmt_template = NULL;
  char *s = NULL;
  switch (ast->data_type->data_type) {
  case DT_I8: {
    bytes_used += 1;
    int_asmt_template = "movb $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->i8_value, bytes_used);
    break;
  }
  case DT_U8: {
    bytes_used += 1;
    int_asmt_template = "movb $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->u8_value, bytes_used);
    break;
  }
  case DT_I16: {
    bytes_used += 2;
    int_asmt_template = "movw $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->i16_value, bytes_used);
    break;
  }
  case DT_U16: {
    bytes_used += 2;
    int_asmt_template = "movw $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->u16_value, bytes_used);
    break;
  }
  case DT_I32: {
    bytes_used += 4;
    int_asmt_template = "movl $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->i32_value, bytes_used);
    break;
  }
  case DT_U32: {
    bytes_used += 4;
    int_asmt_template = "movl $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->u32_value, bytes_used);
    break;
  }
  case DT_I64: {
    bytes_used += 8;
    int_asmt_template = "movq $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->i64_value, bytes_used);
    break;
  }
  case DT_U64: {
    bytes_used += 8;
    int_asmt_template = "movq $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->u64_value, bytes_used);
    break;
  }
  case DT_I128: {
    bytes_used += 8;
    int_asmt_template = "movq $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->i128_value, bytes_used);
    break;
  }
  case DT_U128: {
    bytes_used += 8;
    int_asmt_template = "movq $%d, -%d(%rsp)";
    s = calloc(strlen(int_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)int_asmt_template, ast->u128_value, bytes_used);
    break;
  }
  }

  return s;
}

char* assembled_binop(AST_T* ast, list_T* list) {
  char* binop_template = NULL;
  char* s = NULL;
  printf("%d %s %d\n", ast->binop_left->ast_type, token_type_to_string(ast->binop_op), ast->binop_right->ast_type);
  switch(ast->binop_op) {
    case PLUS: {
      puts("Bruhter");
      printf("%p\n", ast->binop_right->ast_type);
      AVPair* left = ast->binop_left;
      puts("Here 1");
      AVPair* right = ast->binop_right;
      puts("Here 2");
      binop_template = "movl -%d(%%rbp), %%edx\nmovl -%d(%%rbp), %%eax\naddl %%edx, %%eax\nmovl %%eax, -%d(%%rbp)";
      char* buf = calloc(strlen(binop_template) + 256, sizeof(char));
      sprintf(buf, binop_template, left->byte_label, right->byte_label, (bytes_used + 4) );
      s = buf;
      break;
    }
    case MINUS: {
      exit(1);
      AVPair* left = get_variable_by_name(ast->binop_left->variable_name->identifier_value);

      AVPair* right = get_variable_by_name(ast->binop_right->variable_name->identifier_value);

      binop_template = "movl -%d(%%rbp), %%eax\nsubl -%d(%%rbp), %%eax\nmovl %%eax, -%d(%%rbp)";
      char* buf = calloc(strlen(binop_template) + 256, sizeof(char));
      sprintf(buf, binop_template, left->byte_label, right->byte_label, (bytes_used + 4) );
      s = buf;
      break;
    }
  }
  return s;
}

char *assembled_float(AST_T *ast, list_T *list) {
  char *flt_asmt_template = NULL;
  char *s = NULL;
  switch (ast->data_type->data_type) {
  case DT_F32: {
    bytes_used += 4;
      flt_asmt_template = "movss %s(%%rip), %%xmm0\n\tmovss %%xmm0, -%d(%%rip)";
    s = calloc(strlen(flt_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)flt_asmt_template, ast->variable_definition_name->identifier_value, bytes_used);
    break;
  }
  case DT_F64: {
    bytes_used += 12;
      flt_asmt_template = "movsd %s(%%rip), %%xmm0";
    s = calloc(strlen(flt_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)flt_asmt_template, ast->variable_definition_name->identifier_value, bytes_used);
    break;
  }
  case DT_F128: {
    bytes_used += 16;
      flt_asmt_template = "movw $%d, -%d(%rsp)";
    s = calloc(strlen(flt_asmt_template) + 128, sizeof(char));
    sprintf(s, (char *)flt_asmt_template, ast->variable_definition_name->identifier_value, bytes_used);
    break;
  }
  }

  return s;
}

char *assembled_string_section_text(AST_T *ast, list_T *list) {
  char *value = calloc(128 + strlen(ast->variable_definition_value->string_value), sizeof(char));
  sprintf(value, "%s:\n\t.string \"%s\"\n", ast->variable_definition_name->identifier_value, ast->variable_definition_value->string_value);
  AVPair* string_section_text = init_assembly_variable_pair(ast->variable_definition_name->identifier_value, ast->variable_definition_value, 0, 0);
  for(int i = 0; i < variables->used; i++) {
    AVPair* avpair = (AVPair*)variables->items[i];
    if (avpair->value->ast_type != AST_STRING) break;
    if (strcmp(avpair->
      value->
      string_value, 
      ast->
      variable_definition_value->
      string_value) == 0) { 
      return "";
    }
  }
  add_element_list(variables, string_section_text);
  return value;
}

char *assembled_float_section_text(AST_T *ast, list_T *list) {
  char *value = calloc(12800, sizeof(char));
  int precision = 0;
  switch (ast->variable_definition_value->data_type->data_type) {
    case DT_F32: {
      precision = 1;
      break;
    }
    case DT_F64: {
      precision = 2;
      break;
    }
    case DT_F128: {
      precision = 4;
      break;
    }
  }
  sprintf(value, "%s:\n\t", ast->variable_definition_name->identifier_value);
  
  switch(precision) {
    case 1: {
      union {
        float floatValue;
        int ieee_754;
      } f;
      f.floatValue = ast->variable_definition_value->f32_value;
      sprintf(value + strlen(value), ".long %d\n", f.ieee_754);
      break;
    }
    case 2: {
      union {
        double floatValue;
        int ieee_754[2];
      } f;
      f.floatValue = ast->variable_definition_value->f64_value;
      sprintf(value + strlen(value), ".long %d\n\t.long %d\n", f.ieee_754[0], f.ieee_754[1]);
      break;
    }
    case 4: {
      union {
        float floatValue;
        int ieee_754;
      } f;
      f.floatValue = ast->variable_definition_value->f32_value;
      sprintf(value + strlen(value), ".long %d\n", f.ieee_754);
      break;
    }
    default: {
      sprintf(value + strlen(value), "uhoh\n");
      break;
    }
  }
  AVPair* float_section_text = init_assembly_variable_pair(ast->variable_definition_name->identifier_value, ast->variable_definition_value, 0, 0);
  for(int i = 0; i < variables->used; i++) {
    AVPair* avpair = (AVPair*)variables->items[i];
    if (avpair->value->f32_value == ast->variable_definition_value->f32_value) { 
      return value;
    }
  }
  add_element_list(variables, float_section_text);
  return value;
}

char *assembled(AST_T *ast, list_T *list) {
  char *value = calloc(1, sizeof(char *));
  char *next_value = calloc(1, sizeof(char));

  switch (ast->ast_type) {
    case AST_COMPOUND: {
      next_value = assembled_compound(ast, list);
      break;
    }
    case AST_VARIABLE_DEFINITION: {
      int initial = bytes_used;
      if (ast->variable_definition_value->ast_type == AST_INT) {
        next_value = assembled_integer(ast->variable_definition_value, list);
      } else if (ast->variable_definition_value->ast_type == AST_FLOAT) {
        next_value = assembled_float(ast, list);
      } else if (ast->variable_definition_value->ast_type == AST_STRING) {   
      bytes_used += 4;
      next_value = realloc(next_value, 376);
      char* label = check_str_existence(ast);
      if (label == NULL) {
        label = ast->variable_definition_name->identifier_value;
      }
      sprintf(next_value, "movl $%s, -%d(%%rbp)", label, bytes_used);
    } else if(ast->variable_definition_value->ast_type == AST_BOOLEAN) {
      AST_T* new_ast = ast;
      new_ast->variable_definition_value = init_ast_expr(AST_INT);
        new_ast->data_type = init_data_type(DT_I8);
      new_ast->i8_value = (ast->variable_definition_value->boolean_value == 1) ? 1 : 0;
      next_value = assembled_integer(new_ast, list);
    } else if(ast->variable_definition_value->ast_type == AST_BINOP) {
        next_value = assembled_binop(ast->variable_definition_value, list);
    }
    AVPair* avpair = init_assembly_variable_pair(ast->variable_definition_name->identifier_value, ast->variable_definition_value, bytes_used - initial, bytes_used);
      add_element_list(variables, avpair);
    break;
  }
    case AST_FUNCTION_DEFINITION: {
      next_value = assembled_function(ast, list);
      break;
    }
  default: {
    fprintf(stderr, "No frontend for AST: %d", ast->ast_type);
    exit(1);
  }
  }
  if (next_value == NULL) exit(213);
  return next_value;
}


char *assembled_root(AST_T *ast, list_T *list) {
  variables = init_list();
  const char *section_text = ".text\n";
  char *value = calloc(strlen(section_text) + 128, sizeof(char));

  for (int i = 0; i < ast->compound_children->used; i++) {
    AST_T* func_main = ast->compound_children->items[0];
    for(int i = 0; i < func_main->function_definition_body->compound_children->used; i++) {
      AST_T* stmt = func_main->function_definition_body->compound_children->items[i];
      if(stmt->ast_type == AST_VARIABLE_DEFINITION) {
        if (stmt->variable_definition_value->ast_type == AST_STRING) {
          char* str = assembled_string_section_text(stmt, list);
          value = realloc(value, strlen(value) + strlen(str) + 1);
          strcat(value, str);
        } else if (stmt->variable_definition_value->ast_type == AST_FLOAT) {
          char* str = assembled_float_section_text(stmt, list);
          value = realloc(value, strlen(value) + strlen(str) + 1);
          strcat(value, str);
        }
      }
    }
  }
  value = realloc(value, strlen(value) + strlen(section_text) + 1);
  strcat(value, section_text);
  char *assembly = assembled(ast, list);
  value = (char *)realloc(value, (strlen(value) + strlen(assembly) + 1) *
                                     sizeof(char));
  
  
  strcat(value, assembly);

  value =
      realloc(value, (strlen(value) + 1) *
                         sizeof(char));
  strcat(value, (char *)"");

  return value;
}
