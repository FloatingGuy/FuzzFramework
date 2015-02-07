#include "stdafx.h"
#include "FuzzMain.h"
#include "FuncParse.h"
#include "FuzzLog.h"
#include "FuzzFuncDef.h"

void FuzzBegin(PFuncInfor in_PFuncInfor, DWORD dwFuncAddr)
{
	//isExp
	if (in_PFuncInfor->isExp)
	{
		isExp MyIsExp;
		MyIsExp = (isExp)dwFuncAddr;
		char dst[8] = { 0 };
		char src[MAX_PATH] = { "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" };
		MyIsExp(dst, src);
	}
	//unExp
	else
	{
		unExp MyUnShow = (unExp)dwFuncAddr;
		MyUnShow("test");
	}
}


BOOL FuzzMain()
{	
	BOOL bRet = FALSE;
	char szTime[MAX_PATH] = {0};
	DWORD dwFuncAddr = 0;

	CFuncParse MyFuncParse;
	CFuzzLog MyFuzzLog;

	//init fuzz infor
	PFuncInfor pFuncInfor = new FuncInfor;
	pFuncInfor->szDllName = new char[MAX_PATH];
	pFuncInfor->szFuncName = new char[MAX_PATH];
	pFuncInfor->isExp = 0;
	pFuncInfor->dwFuncRva = 0;
	bRet = MyFuncParse.GetCfgFileInfor("FuzzCfg.ini", pFuncInfor);
	if (!bRet)
	{
		CFuzzLog::OutputConsole("Get Target Infor False!");
		goto gleave;
	}

	//init log file
	bRet = MyFuzzLog.GetSysTime(szTime);
	bRet = MyFuzzLog.MyCreateFile(szTime);
	if (!bRet)
	{
		CFuzzLog::OutputConsole("Init Log File False!");
		goto gleave;
	}

	//get func address
	dwFuncAddr = MyFuncParse.GetFuncAddr(pFuncInfor);
	if (dwFuncAddr)
	{
		CFuzzLog::OutputConsole("Fuzz Begin...");
		try
		{
			FuzzBegin(pFuncInfor, dwFuncAddr);
			int *i = NULL;
			*i = 13;
		}
		catch (...)
		{
			//¼ÇÂ¼ÈÕÖ¾¡£
			CFuzzLog::OutputConsole("catch error!");
		}
	}

gleave:
	if (pFuncInfor)
		delete[] pFuncInfor;
	return bRet;
}

