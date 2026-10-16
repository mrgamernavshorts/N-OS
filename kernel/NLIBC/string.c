#ifndef NLIBC_STRING_C
#define NLIBC_STRING_C

#include "string.h"
#include "stdint.h"
// clang-format off

int strlen(char *str) {
  vuint32_t count = 0;
  for (vuint32_t i = 0; str[i] != '\0'; i++) count++;
  return count;
}

#endif
