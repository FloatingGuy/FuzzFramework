#pragma once
#include "ComHead.h"

class CFuzzLog
{
public:
	CFuzzLog(void);
	~CFuzzLog(void);

public:
	static void OutputConsole(char *out_StrLog);
	BOOL GetSysTime(char *out_Str);
	BOOL MyCreateFile(char *in_out_FileName);
	BOOL MyWriteFile(char *in_FileName, char *in_LogInfor);
	
};

