// FuncFuzz.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FuncFuzz/FuzzMain.h"
int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	bRet = FuzzMain();
	if (!bRet)
	{
		MessageBoxA(NULL, "False", "FuncFuzz", NULL);
	}
	return 0;
}

