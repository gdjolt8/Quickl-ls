#include "../include/quickk_data_type.h"
#include <stdlib.h>

DataType_T *init_data_type(int type) {
  DataType_T *data_type = calloc(1, sizeof(DataType_T));
  data_type->data_type = type;
  data_type->node = NULL;
  data_type->ptr = NULL;
  data_type->array_ptr = NULL;

  switch (data_type->data_type) {
  case DT_U8: {
    data_type->min_size = 0;
    data_type->bytes_size = 1;
    data_type->max_size = 255;
  }
  }
  return data_type;
}
const char *data_type_to_string(DataType_T *type) {
  switch(type->data_type) {
       case DT_I8: return "i8";
    case DT_I16: return "i16";
    case DT_I32: return "i32";
    case DT_I64: return "i64";
    case DT_I128: return "i128";
    case DT_F32: return "f32";
    case DT_F64: return "f64";
    case DT_F128: return "f128";
    case DT_U8: return "u8";
    case DT_U16: return "u16";
    case DT_U32: return "u32";
    case DT_U64: return "u64";
    case DT_U128: return "u128";
    case DT_CHAR: return "char";
    case DT_STRING: return "string";
    case DT_BOOL: return "bool";
  }
}
const char *dt_integer_to_string(int type) {
  switch (type) {
    case DT_I8: return "i8";
    case DT_I16: return "i16";
    case DT_I32: return "i32";
    case DT_I64: return "i64";
    case DT_I128: return "i128";
    case DT_F32: return "f32";
    case DT_F64: return "f64";
    case DT_F128: return "f128";
    case DT_U8: return "u8";
    case DT_U16: return "u16";
    case DT_U32: return "u32";
    case DT_U64: return "u64";
    case DT_U128: return "u128";
    case DT_CHAR: return "char";
    case DT_STRING: return "string";
    case DT_BOOL: return "bool";
    case DT_ARRAYTYPE: {
      return "{array}";
    }
    case DT_POINTER: return "{pointer}";
  }
  return NULL;
}
DataType_T *init_data_type_with_dt(DataType_T *type) {
  DataType_T* data_type = (DataType_T*)calloc(1, sizeof(DataType_T));
  data_type->data_type = type->data_type;
  data_type->node = type->node;
  data_type->ptr = type->ptr;
  data_type->array_ptr = type->array_ptr;
  return data_type;
}
void free_data_type(DataType_T *type) {
  if (type->array_ptr) free_data_type(type->array_ptr);
  if (type->node) free_ast_expr(type->node);
  if (type->ptr) free_data_type(type->ptr);
  free(type);
}