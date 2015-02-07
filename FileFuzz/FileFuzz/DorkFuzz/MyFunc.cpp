#include "stdafx.h"
#include "MyFunc.h"
#include "FuzzLog.h"
#include "MyHead.h"

BOOL CMyFunc::EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp = { 0 };
	LUID luid = { 0 };
	BOOL bRet = FALSE;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY | TOKEN_READ,
		&hToken))
	{
		bRet = FALSE;
		goto gleave;
	}
	if (LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
	{
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = (bEnable) ? SE_PRIVILEGE_ENABLED : 0;
		if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL))
		{
			bRet = TRUE;
		}
	}
gleave:
	if (hToken)
		CloseHandle(hToken);
	//return (GetLastError() == ERROR_SUCCESS);
	return bRet;
}

BOOL CMyFunc::GetSysTime(char *out_StrTime)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	//printf( "%4d/%02d/%02d %02d:%02d:%02d.%03d Week%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
	sprintf_s(out_StrTime, MAX_PATH,
		"%4d-%02d-%02d-%02d-%02d-%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return TRUE;
}

BOOL CMyFunc::GetFileFullPath(char *in_FileName, char *out_FullPath)
{
	BOOL bRet = FALSE;
	char szCurPath[MAX_PATH] = { 0 };
	DWORD dwLen = GetCurrentDirectoryA(MAX_PATH, szCurPath);
	if (dwLen > MAX_PATH)
	{
		CFuzzLog::OutputConsole("GetCurrentDirectory False!");
		goto gleave;
	}
	sprintf_s(out_FullPath, MAX_PATH, "%s\\%s", szCurPath, in_FileName);
	bRet = TRUE;
gleave:
	return bRet;
}

BOOL CMyFunc::ReleaseFile(char *in_FileName, char *in_WriteBytes, DWORD dwFileSize)
{
	BOOL bRet = FALSE;
	DWORD dwWrite = 0;
	HANDLE hFile = CreateFileA(	in_FileName,
								GENERIC_WRITE,
								NULL,
								NULL,
								OPEN_ALWAYS,
								NULL,
								NULL);
	if (INVALID_HANDLE_VALUE!= hFile)
	{
		bRet = WriteFile(hFile, in_WriteBytes, dwFileSize, &dwWrite, NULL);
		if (!bRet || dwFileSize != dwWrite)
		{
			bRet = FALSE;
			CFuzzLog::OutputConsole("ReleaseFile False!");
			goto gleave;
		}
		bRet = TRUE;
	}
	
gleave:
	if (hFile)
		CloseHandle(hFile);
	return bRet;
}

BOOL CMyFunc::TraverseFiles(char *in_szDirectory, 
							vector<SFilePath> &out_vFileList, 
							vector<SDirectoryPath> &out_vDirectoryList, 
							char *in_FileType)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATAA fileData;
	HANDLE hSearch;
	char szDirParam[MAX_PATH] = { 0 };
	char szPath[MAX_PATH] = { 0 };
	char szDir[MAX_PATH] = { 0 };

	SFilePath sFilePath;
	SDirectoryPath sDirectoryPath;

	strcpy_s(szDirParam, MAX_PATH, in_szDirectory);
	// has "\\"?
	if (szDirParam[strlen(szDirParam) - 1] != '\\')
	{
		strcat_s(szDirParam, MAX_PATH, "\\");
	}
	//backup directory
	strcpy_s(szPath, MAX_PATH, szDirParam);
	//FileType == *.*
	strcat_s(szDirParam, MAX_PATH, in_FileType);
	hSearch = FindFirstFileA(szDirParam, &fileData);

	if (hSearch == INVALID_HANDLE_VALUE)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("No files found!");
		goto gleave;
	}
	strcpy_s(szDir, MAX_PATH, szPath);
	if (szDir[strlen(szDir) - 1] != '\\')
	{
		strcat_s(szDir, "\\");
	}
	//Traverse, get file list 
	while (FindNextFileA(hSearch, &fileData))
	{
		char szFileName[MAX_PATH] = { 0 };
		if (StrCmpA(fileData.cFileName, ".") && StrCmpA(fileData.cFileName, ".."))
		{
			char szFile[MAX_PATH] = { 0 };
			char szTmp[MAX_PATH] = { 0 };
			strcpy_s(szFile, MAX_PATH, szDir);

			if (!_strnicmp(fileData.cFileName, "content.ie5", 11))
			{
				continue;
			}
			strcat_s(szFile, MAX_PATH, fileData.cFileName);
			CharLowerA(szFile);

			//con,nul is MS system reserve name
			if ((!_strnicmp(fileData.cFileName, "con", 3) || !_strnicmp(fileData.cFileName, "nul", 3))
				&& ((strlen(fileData.cFileName) == 3) || (strlen(fileData.cFileName) >= 4 && fileData.cFileName[3] == '.'))
				)
			{
				memset(sDirectoryPath.szDirectoryPath, 0, MAX_PATH);
				strcpy_s(sDirectoryPath.szDirectoryPath, MAX_PATH, szFile);
				out_vDirectoryList.push_back(sDirectoryPath);
				continue;
			}
			if (GetFileAttributesA(szFile) & FILE_ATTRIBUTE_DIRECTORY)
			{
				//get directory's file
				memset(sDirectoryPath.szDirectoryPath, 0, MAX_PATH);
				strcpy_s(sDirectoryPath.szDirectoryPath, MAX_PATH, szFile);
				out_vDirectoryList.push_back(sDirectoryPath);
				TraverseFiles(szFile, out_vFileList, out_vDirectoryList, in_FileType);//ตÝน้
			}
			else
			{
				//get file
				memset(sFilePath.szFilePath, 0, MAX_PATH);
				strcpy_s(sFilePath.szFilePath, MAX_PATH, szFile);
				strcpy_s(sFilePath.szFileName, MAX_PATH, fileData.cFileName);
				out_vFileList.push_back(sFilePath);
			}
		}
	}
	if (out_vFileList.size() == 0)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Traverse No Files!");
		goto gleave;
	}
	else
	{
		bRet = TRUE;
	}

gleave:
	if (hSearch)
		FindClose(hSearch);
	return bRet;
}

BOOL CMyFunc::MyCreateFile(char *in_out_FileName)
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
		CFuzzLog::OutputConsole("CreateFile False!");
	}
	if (hFile)
		CloseHandle(hFile);
	return bRet;
}

BOOL CMyFunc::AppendWriteFile(char *in_FileName, char *in_WritePrefix , char *in_WriteInfo, char *in_WriteSuffix)
{
	BOOL bRet = FALSE;
	DWORD dwWrite = 0;
	char szAppend[MAX_PATH] = { 0 };
	sprintf_s(szAppend, MAX_PATH, "%s%s%s", in_WritePrefix, in_WriteInfo, in_WriteSuffix);
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
			szAppend,
			strlen(szAppend),
			&dwWrite,
			NULL);
	}
	else
	{
		CFuzzLog::OutputConsole("WriteFile False!");
	}

	if (hFile)
		CloseHandle(hFile);
	return bRet;
}