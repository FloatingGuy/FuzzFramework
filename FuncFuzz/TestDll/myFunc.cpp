#include "stdafx.h"
#include "myFunc.h"


void myCopy(char *dst, char *src)
{
	strcpy(dst, src);
	printf("myCopy Func : %s\n", dst);
	myShow("auto");
	
}

void myShow(char *str)
{
	printf("myShow Func £º%s\n", str);
}