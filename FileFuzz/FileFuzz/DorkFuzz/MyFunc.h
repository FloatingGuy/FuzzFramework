#pragma once
#include "stdio.h"
#include "windows.h"
#include <iostream>

#include <vector>
using namespace std;

typedef struct SFilePath
{
	char szFilePath[MAX_PATH];
	char szFileName[MAX_PATH];
}*PSFilePath;

typedef struct SDirectoryPath
{
	char szDirectoryPath[MAX_PATH];
}*PSDirectoryPath;

class CMyFunc
{
public:
	CMyFunc();
	~CMyFunc();

public:
	static BOOL EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable);
	static BOOL GetSysTime(char *out_Str);
	static BOOL MyCreateFile(char *in_out_FileName);
	static BOOL AppendWriteFile(char *in_FileName, 
								char *in_WritePrefix, 
								char *in_WriteInfo, 
								char *in_WriteSuffix);

public:
	static BOOL GetFileFullPath(char *in_FileName, char *out_FullPath);
	static BOOL ReleaseFile(char *in_FileName, char *in_WriteBytes, DWORD dwFileSize);
	static BOOL TraverseFiles(char *in_szDirectory,
							vector<SFilePath> &out_vFileList,
							vector<SDirectoryPath> &out_vDirectoryList,
							char *in_FileType);	
};