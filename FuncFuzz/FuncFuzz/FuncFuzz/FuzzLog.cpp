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


BOOL CFuzzLog::GetSysTime(char *out_StrTime)
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	//printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d ÐÇÆÚ%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
	sprintf_s(	out_StrTime, MAX_PATH, 
				"%4d-%02d-%02d-%02d-%02d-%02d", 
				sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond);
	return TRUE;
}

BOOL CFuzzLog::MyCreateFile(char *in_out_FileName)
{
	BOOL bRet = FALSE;
	HANDLE hFile = CreateFileA(in_out_FileName,
								NULL,
								NULL,
								NULL,
								OPEN_ALWAYS,
								NULL,
								NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bRet = TRUE;
	}
	else
	{
		OutputConsole("CreateFile False!");
	}
	if (hFile)
		CloseHandle(hFile);	
	return bRet;
}

BOOL CFuzzLog::MyWriteFile(char *in_FileName, char *in_LogInfor)
{
	BOOL bRet = FALSE;
	DWORD dwWrite = 0;
	HANDLE hFile = CreateFileA(in_FileName,
								GENERIC_WRITE,
								NULL,
								NULL,
								OPEN_ALWAYS,
								NULL,
								NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		DWORD dwHigh;
		DWORD dwPos = GetFileSize(hFile, &dwHigh);
		SetFilePointer(hFile, dwPos, 0, FILE_BEGIN);
		bRet = WriteFile(hFile,
						in_LogInfor,
						strlen(in_LogInfor),
						&dwWrite,
						NULL);
	}
	else
	{
		OutputConsole("WriteFile False!");
	}

	if (hFile)
		CloseHandle(hFile);
	return bRet;
}