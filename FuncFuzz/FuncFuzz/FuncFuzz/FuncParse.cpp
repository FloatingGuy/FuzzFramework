#include "stdafx.h"
#include "FuncParse.h"
#include "FuzzLog.h"

CFuncParse::CFuncParse(void)
{
}
CFuncParse::~CFuncParse(void)
{
}

BOOL CFuncParse::GetFileFullPath(char *in_szFileName, char *out_szFullPath)
{
	BOOL bRet = FALSE;
	char szCurPath[MAX_PATH] = { 0 };
	DWORD dwLen = GetCurrentDirectoryA(MAX_PATH, szCurPath);
	if (dwLen > MAX_PATH)
	{
		CFuzzLog::OutputConsole("GetCurrentDirectory False!");
		goto gleave;
	}
	sprintf_s(out_szFullPath, MAX_PATH, "%s\\%s", szCurPath, in_szFileName);
	bRet = TRUE;
gleave:
	return bRet;
}

BOOL CFuncParse::GetCfgFileInfor(char *in_szCfgFileName, PFuncInfor in_out_PFuncInfor)
{
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	DWORD dwFuncRva = 0;
	char szCfgFilePath[MAX_PATH] = { 0 };
	char szDllName[MAX_PATH] = { 0 };

	bRet = this->GetFileFullPath(in_szCfgFileName, szCfgFilePath);
	if (bRet)
	{
		dwRet = GetPrivateProfileStringA("Target", "dllName", NULL, szDllName, MAX_PATH, szCfgFilePath);
		if (0 < dwRet || MAX_PATH - 1 != dwRet || MAX_PATH - 2 != dwRet)
		{
			bRet = this->GetFileFullPath(szDllName, in_out_PFuncInfor->szDllName);
			if (!bRet)
			{
				CFuzzLog::OutputConsole("Get dllName False!");
				goto gleave;
			}
		}
		dwRet = GetPrivateProfileStringA("Target", "funcName", NULL, in_out_PFuncInfor->szFuncName, MAX_PATH, szCfgFilePath);
		if (0 >= dwRet || MAX_PATH - 1 == dwRet || MAX_PATH - 2 == dwRet)
		{
				CFuzzLog::OutputConsole("Get funcName False!");
				goto gleave;
		}
		in_out_PFuncInfor->isExp = GetPrivateProfileIntA("Target", "isExp", 2, szCfgFilePath);
		if (2 == in_out_PFuncInfor->isExp)
		{
			CFuzzLog::OutputConsole("Get funcName False!");
			goto gleave;
		}
		in_out_PFuncInfor->dwFuncRva = GetPrivateProfileIntA("Target", "funcRva", 0, szCfgFilePath);
		if (0 == in_out_PFuncInfor->dwFuncRva)
		{
			CFuzzLog::OutputConsole("Get funcRva False!");
			goto gleave;
		}
	}
	bRet = TRUE;

gleave:
	return bRet;
}


DWORD CFuncParse::GetFuncAddr(PFuncInfor in_PFuncInfor)
{
	HMODULE hDll = NULL;
	DWORD dwFuncAddr = 0;
	hDll = LoadLibraryA(in_PFuncInfor->szDllName);
	if (hDll)
	{
		CFuzzLog::OutputConsole("Load Dll Success!");
		if (in_PFuncInfor->isExp)
		{
			FARPROC farFunAddr = GetProcAddress(hDll, in_PFuncInfor->szFuncName);
			if (farFunAddr)
			{
				dwFuncAddr = (DWORD)farFunAddr;
			}
			else
			{
				CFuzzLog::OutputConsole("Get FuncAddr False!");
			}	
		}
		else
		{
			dwFuncAddr = (DWORD)hDll + in_PFuncInfor->dwFuncRva;
		}	
	}
	else
	{
		CFuzzLog::OutputConsole("Load Dll False!");
	}

	return dwFuncAddr;
}