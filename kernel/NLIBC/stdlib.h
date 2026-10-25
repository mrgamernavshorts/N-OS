#ifndef NLIBC_STDLIB
#define NLIBC_STDLIB
// clang-format off

// Takes a int value and converts it into a string. The string buffer should be passed in,
// as it stores the string value there.
void itoa(int val, char* str); 

// Converts a string to a int.
int atoi(char* str);

#endif
