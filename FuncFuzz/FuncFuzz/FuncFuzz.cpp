// FuncFuzz.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FuncFuzz/FuzzMain.h"
#include "FuncFuzz/Exception/ETest.h"
int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	ETest();
	bRet = FuzzMain();
	if (!bRet)
	{
		MessageBoxA(NULL, "False", "FuncFuzz", NULL);
	}
	return 0;
}

