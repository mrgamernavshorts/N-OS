#ifndef NLIBC_MATH_C
#define NLIBC_MATH_C

#include "math.h"

int pow(int base, int power){
  int FinalInt = 1;
  for(int i = 0; i < power; i++){
    FinalInt *= base;
  }

  return FinalInt;
}

#endif
