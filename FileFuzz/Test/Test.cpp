// Test.cpp : a console application
//

#include "stdafx.h"
#include "string.h"

void BufferOverflow()
{
	TCHAR dst[8] = { 0 };
	TCHAR src[255] = { 0 };
	memset(src, 'A', 255);
	wcscpy(dst, src);
	printf("%s\n", dst);
}

void DivideByZero()
{
	int iZero = 0;
	int iRet = 5 / iZero;	
	printf("%d\n", iRet);
}


int _tmain(int argc, _TCHAR* argv[])
{
	BufferOverflow();
	//DivideByZero();
	printf("test\n");
	getchar();
	return 0;
}

