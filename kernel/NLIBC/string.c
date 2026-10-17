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

int strcmp(char *s1, char *s2){
  if(!s1 || !s2) return 1;

  if(strlen(s1) != strlen(s2)) return 1;

  for(vuint32_t i = 0; s1[i] != '\0'; i++){
    if(s1[i] != s2[i]){
      return 1;
    }
  }

  return 0;
}

int strncmp(char *str1, char *str2, uint32_t n){
  if(!str1 || !str2) return 1;

  for(vuint32_t i = 0; i < n; i++){
    if(!(str1[i] == str2[i])){
      return 1;
    }
  }

  return 0;
}

int memcpyc(char* dest, char* src, uint32_t n){
  if(!dest || !src) return 1;
  for(vuint32_t i = 0; i < n; i++){
    dest[i] = src[i];
  }

  return 0;
}

int memcpycUntil(char* dest, char* src, char c){
  if(!dest || !src || !c) return 1;

  vuint32_t index;
  for(vuint32_t i = 0; src[i] != c; i++){
    dest[i] = src[i];
    index = i;
  }
  index++;
  dest[index] = '\0';
  return 0;
}

int memcpycUntilVolatile(char* dest, vuint16_t* src, char c){
  if(!dest || !src || !c) return 1;

  vuint32_t index;
  for(vuint32_t i = 0; src[i] != c; i++){
    dest[i] = src[i];
    index = i;
  }
  index++;
  dest[index] = '\0';
  return 0;
}

void memsetc(char* s, char c, vuint32_t n){
  for(vuint32_t i = 0; i < n; i++){
    s[i] = c;
  }
}

#endif
