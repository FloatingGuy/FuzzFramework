#include "stdafx.h"
#include "FuzzLog.h"


CFuzzLog::CFuzzLog(void)
{
}

CFuzzLog::~CFuzzLog(void)
{
}

void CFuzzLog::OutputConsole(char *out_StrLog)
{
	printf("ErroCode: %d -> %s\n", GetLastError(), out_StrLog);
}
