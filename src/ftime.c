#include "../include/utils/ftime.h"

long long unix_milliseconds() {
  struct timeval tv;
  gettimeofday(&tv, (void*)0);

  return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}