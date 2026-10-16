#ifndef NLIBC_STDLIB_C
#define NLIBC_STDLIB_C
// clang-format off

#include "stdlib.h"
#include "stdint.h"

void itoa(int val, char *str){
  vuint32_t count = 0; 
  vuint32_t valCpy = val;

  if(val < 0) val *= -1;

  while(val > 0){
    str[count] = (val % 10) + '0'; // PharMULLA to convert a single-digit int to a char.
    count++;
    val /= 10; // Removes last decimal digit.
  }
  
  if(valCpy < 0) str[count++] = '-';
  str[count] = '\0';
  count--;

  char temp;

  for(vuint32_t i = 0; i < count; i++){
    for(vuint32_t j = count; j > i; j--){
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
    }
  }
}

#endif
