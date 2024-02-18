#ifndef QUICKK_POS_H
#define QUICKK_POS_H

typedef struct QUICKK_POSITION_STRUCT
{
  long long column;
  long long line;
} Position_T;

Position_T* init_position(long long column, long long line);

#endif