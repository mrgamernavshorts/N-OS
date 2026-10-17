#ifndef NLIBC_STRING
#define NLIBC_STRING

#include "stdint.h"

// clang-format off
// -------------------------------------------------------------------------
// [WARNING]: ALL OF THE MEMORY-MANUPLATION COMMANDS COULD BE MEMORY UNSAFE.
// -------------------------------------------------------------------------

int strlen(char* str); // Returns the length of a string.
int strcmp(char* s1, char* s2); // Compares 2 strings, and returns 0 if strings match.
int strncmp(char* str1, char* str2, uint32_t n); // Compares n bytes of 2 strings, and returns a 0 if they match.
                                                 //
// Copies n bytes of wherever src is pointing to dest. (char only)[Unsafe]
// Returns 0 if successful and 1 if cpy failed.
int memcpyc(char* dest, char* src, uint32_t n); 

// Copies the char's from src to dest until a char 'c'.
// Note that it null-terminates '\0' the dest.
// Returns 0 if successful and 1 if cpy failed.
int memcpycUntil(char* dest, char* src, char c); 

// Same as memcpycUntil, but takes a 'vuint16_t' as the src pointer.
int memcpycUntilVolatile(char* dest, vuint16_t* src, char c); 

// Sets n amount of memory starting from s to 'c'.
void memsetc(char* s, char c, vuint32_t n);
#endif
