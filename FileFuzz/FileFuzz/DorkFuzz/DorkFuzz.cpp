#include "stdafx.h"
#include "Template/Template.h"
#include "MyFunc.h"
#include "Fuzzlog.h"

BOOL Init()
{
	CFuzzLog::OutputConsole(GetCommandLineA());
	HANDLE hMutex = CreateMutexA(NULL, FALSE, "idhyt");
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		CFuzzLog::OutputConsole("The Program has been running!");
		return FALSE;
	}
	if (CMyFunc::EnablePrivilege(SE_DEBUG_NAME, TRUE) == FALSE)
	{
		CFuzzLog::OutputConsole("LookupPrivilege False!");
		return FALSE;
	}
	return TRUE;
}

// mode 01 10 11
BOOL FuzzBegin(BYTE mode)
{
	BOOL bRet = FALSE;
	char szCfgFilePath[MAX_PATH] = { 0 };
	PSTemplate pSTemplate = new STemplate;
	pSTemplate->szTemplatePath = new char[MAX_PATH];
	pSTemplate->szTargetProgram = new char[MAX_PATH];

	PSDirectory pSDirectory = new SDirectory;
	pSDirectory->szFuzzFileDirectory = new char[MAX_PATH];
	pSDirectory->szCrashFileDirectory = new char[MAX_PATH];
	pSDirectory->szLogFileDirectory = new char[MAX_PATH];

	CTemplate MyTemplate;
	if (CMyFunc::GetFileFullPath("FuzzCfg.ini", szCfgFilePath))
	{
		if (!MyTemplate.GetTemplateInfo(szCfgFilePath, pSTemplate) ||
			!MyTemplate.GetDirectoryInfo(szCfgFilePath, pSDirectory))
		{
			CFuzzLog::OutputConsole("Get Config Info False!");
			goto gleave;
		}
	}

	if (mode & 1)
	{
		DWORD dwCount = 0;
		dwCount = MyTemplate.CreateFuzzFile(pSTemplate, pSDirectory);
		printf("-> create %d fuzz file\n", dwCount);
	}

	if (mode & 2)
	{
		vector<SFilePath> vSfilePath;
		vector<SDirectoryPath> vSDirectoryPath;
		//get fuzz file
		bRet = CMyFunc::TraverseFiles(	pSDirectory->szFuzzFileDirectory,
										vSfilePath,
										vSDirectoryPath,
										"*");
		if (FALSE == bRet)
		{
			goto gleave;
		}
		//create log file
		char szLogFileName[MAX_PATH] = { 0 };
		char szLogFilePath[MAX_PATH] = { 0 };
		if (CMyFunc::GetSysTime(szLogFileName))
		{
			sprintf_s(szLogFilePath, MAX_PATH, "%s\\%s%s", pSDirectory->szLogFileDirectory, szLogFileName, ".log");
			bRet = CMyFunc::MyCreateFile(szLogFilePath);
			if (FALSE == bRet)
			{
				CFuzzLog::OutputConsole("Create Log File False!");
				goto gleave;
			}
		}
		//create fuzz process
		for (vector<SFilePath>::iterator it = vSfilePath.begin();
			it != vSfilePath.end();
			++it)
		{
			MyTemplate.CreateFuzzProcess(pSTemplate,
											it->szFilePath, 
											it->szFileName, 
											pSDirectory->szCrashFileDirectory,
											szLogFilePath);
		}
	}

gleave:
	if (pSTemplate)
		delete[] pSTemplate;
	if (pSDirectory)
		delete[] pSDirectory;

	return bRet;	
}