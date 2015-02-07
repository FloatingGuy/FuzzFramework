#include "stdio.h"
extern "C" __declspec(dllexport) void myCopy(char *dst, char *src);

void myCopy(char *dst, char *src);

void myShow(char *str);