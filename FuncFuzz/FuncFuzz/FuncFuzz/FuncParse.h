#pragma once
#include "ComHead.h"

typedef struct FuncInfor
{
	char *szDllName;
	char *szFuncName;
	DWORD isExp;
	DWORD dwFuncRva;
}*PFuncInfor;

class CFuncParse
{
public:
	CFuncParse(void);
	~CFuncParse(void);

public:
	BOOL GetFileFullPath(char *in_szFileName, char *out_szFullPath);
	DWORD GetFuncAddr(PFuncInfor in_PFuncInfor);
	BOOL GetCfgFileInfor(char *in_szCfgFileName, PFuncInfor in_out_PFuncInfor);
};

