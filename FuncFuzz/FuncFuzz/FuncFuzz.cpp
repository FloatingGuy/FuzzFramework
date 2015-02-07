// FuncFuzz.cpp : 定义控制台应用程序的入口点。
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

