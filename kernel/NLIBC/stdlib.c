#ifndef NLIBC_STDLIB_C
#define NLIBC_STDLIB_C
// clang-format off

#include "stdlib.h"
#include "string.c"
#include "stdint.h"

int atoi(char* str){
  int finalInt = 0;
  uint8_t Negative = str[0] == '-' ? 1 : 0;
  if(Negative) str++;

  for(uint32_t i = 0; str[i] != '\0'; i++){
    finalInt = (str[i] - '0') + (finalInt * 10);
  }

  return Negative ? -finalInt : finalInt;
}

void itoa(int val, char *str){
  vuint32_t count = 0; 
  uint8_t Negative = val < 0 ? 1 : 0;

  if(val < 0) val *= -1;

  while(val > 0){
    str[count] = (val % 10) + '0'; // PharMULLA to convert a single-digit int to a char.
    count++;
    val /= 10; // Removes last decimal digit.
  }
  
  if(Negative){
    str[count++] = '-';
  }
  str[count] = '\0';

  char buff[1024];
  uint32_t StringLength = strlen(str);

  for(vuint32_t i = 0; i < StringLength; i++){
    buff[i] = str[(StringLength-i)-1];
  }

  for(uint32_t i = 0; i < StringLength; i++){
    str[i] = buff[i];
  }
}

#endif
