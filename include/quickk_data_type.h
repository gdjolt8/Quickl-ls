#ifndef QUICKK_DATA_TYPE_H
#define QUICKK_DATA_TYPE_H
#include "quickk_AST.h"
enum DataTypeEnum {
  DT_I8,
  DT_I16,
  DT_I32,
  DT_I64,
  DT_I128,
  DT_F32,
  DT_F64,
  DT_F128,
  DT_U8,
  DT_U16,
  DT_U32,
  DT_U64,
  DT_U128,
  DT_CHAR,
  DT_STRING,
  DT_BOOL,
  DT_ARRAYTYPE,
  DT_VOID,
  DT_CLASS,
  DT_POINTER
};

typedef struct QUICKK_DATA_TYPE_STRUCT {
  int data_type;
  int bytes_size;
  int min_size;
  int max_size;
  struct QUICKK_DATA_TYPE_STRUCT *array_ptr;
  struct QUICKK_DATA_TYPE_STRUCT *ptr;
  struct QUICKK_AST_STRUCT *node;
} DataType_T;

DataType_T *init_data_type(int type);
const char *data_type_to_string(DataType_T *type);
const char *dt_integer_to_string(int type);
DataType_T *init_data_type_with_dt(DataType_T *type);
void free_data_type(DataType_T *type);
#endif