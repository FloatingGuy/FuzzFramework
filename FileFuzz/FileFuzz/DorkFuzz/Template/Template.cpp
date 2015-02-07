#include "stdafx.h"
#include "Template.h"
#include "../FuzzLog.h"
#include "../MyHead.h"

#include "../Extend/libdasm.h"
using namespace std;

CTemplate::CTemplate(void)
{
}
CTemplate::~CTemplate(void)
{
}

BOOL CTemplate::GetTemplateInfo(char *in_CfgFileName, PSTemplate in_out_PCTemplate)
{
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	char szTemplatePath[MAX_PATH] = { 0 };

	dwRet = GetPrivateProfileStringA("Template", "TemplatePath ", NULL, szTemplatePath, MAX_PATH, in_CfgFileName);
	switch (dwRet)
	{
	case 0:
		CFuzzLog::OutputConsole("Can't Find Template Path!");
		break;
	case MAX_PATH - 1:
		CFuzzLog::OutputConsole("buffer too small!");
		break;
	case MAX_PATH - 2:
		CFuzzLog::OutputConsole("Key Name is NULL!");
		break;
	default:
		if (CMyFunc::GetFileFullPath(szTemplatePath, in_out_PCTemplate->szTemplatePath))
			bRet = TRUE;
		break;
	}
	dwRet = GetPrivateProfileStringA("Template", "TargetProgram", NULL, in_out_PCTemplate->szTargetProgram, MAX_PATH, in_CfgFileName);
	switch (dwRet)
	{
	case 0:
		CFuzzLog::OutputConsole("Can't Find Target Program!");
		break;
	case MAX_PATH - 1:
		CFuzzLog::OutputConsole("buffer too small!");
		break;
	case MAX_PATH - 2:
		CFuzzLog::OutputConsole("Key Name is NULL!");
		break;
	default:
		if (PathFileExistsA(in_out_PCTemplate->szTargetProgram))
			bRet = TRUE;
		else
			CFuzzLog::OutputConsole("Target Program Invalid!");
		break;
	}
	in_out_PCTemplate->wait_time = GetPrivateProfileIntA("Template", "wait_time", -1, in_CfgFileName);
	if (-1 == in_out_PCTemplate->wait_time)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Get wait_time False!");
		goto gleave;
	}
	in_out_PCTemplate->begin_pos = GetPrivateProfileIntA("Template", "begin_pos", -1, in_CfgFileName);
	if (-1 == in_out_PCTemplate->begin_pos)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Get begin_pos False!");
		goto gleave;
	}
	in_out_PCTemplate->end_pos = GetPrivateProfileIntA("Template", "end_pos", -1, in_CfgFileName);
	if (-1 == in_out_PCTemplate->end_pos)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Get end_pos False!");
		goto gleave;
	}
	in_out_PCTemplate->begin_byte = GetPrivateProfileIntA("Template", "begin_byte", -1, in_CfgFileName);
	if (-1 == in_out_PCTemplate->begin_byte)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Get begin_byte False!");
		goto gleave;
	}
	in_out_PCTemplate->end_byte = GetPrivateProfileIntA("Template", "end_byte", -1, in_CfgFileName);
	if (-1 == in_out_PCTemplate->end_byte)
	{
		bRet = FALSE;
		CFuzzLog::OutputConsole("Get end_byte False!");
		goto gleave;
	}

gleave:
	return bRet;
}

BOOL CTemplate::GetDirectoryInfo(char *in_CfgFileName, PSDirectory in_out_PSDirectory)
{
	BOOL bRet = FALSE;
	BOOL dwRet = 0;
	char szFuzzFileDirectory[MAX_PATH] = { 0 };
	char szCrashFileDirectory[MAX_PATH] = { 0 };
	char szLogFileDirectory[MAX_PATH] = { 0 };

	dwRet = GetPrivateProfileStringA("Directory", "FuzzFileDirectory", NULL, szFuzzFileDirectory, MAX_PATH, in_CfgFileName);
	switch (dwRet)
	{
	case 0:
		if (MAX_PATH >= GetCurrentDirectoryA(MAX_PATH, in_out_PSDirectory->szFuzzFileDirectory))
			bRet = TRUE;
		break;
	case MAX_PATH - 1:
		CFuzzLog::OutputConsole("buffer too small!");
		break;
	case MAX_PATH - 2:
		CFuzzLog::OutputConsole("Key Name is NULL!");
		break;
	default:
		if (CMyFunc::GetFileFullPath(szFuzzFileDirectory, in_out_PSDirectory->szFuzzFileDirectory))
			bRet = TRUE;
		break;
	}
	if (!bRet)
		goto gleave;
	bRet = FALSE;

	dwRet = GetPrivateProfileStringA("Directory", "CrashFileDirectory", NULL, szCrashFileDirectory, MAX_PATH, in_CfgFileName);
	switch (dwRet)
	{
	case 0:
		if (MAX_PATH >= GetCurrentDirectoryA(MAX_PATH, in_out_PSDirectory->szCrashFileDirectory))
			bRet = TRUE;
		break;
	case MAX_PATH - 1:
		CFuzzLog::OutputConsole("buffer too small!");
		break;
	case MAX_PATH - 2:
		CFuzzLog::OutputConsole("Key Name is NULL!");
		break;
	default:
		if (CMyFunc::GetFileFullPath(szCrashFileDirectory, in_out_PSDirectory->szCrashFileDirectory))
			bRet = TRUE;
		break;
	}
	if (!bRet)
		goto gleave;
	bRet = FALSE;

	dwRet = GetPrivateProfileStringA("Directory", "LogFileDirectory", NULL, szLogFileDirectory, MAX_PATH, in_CfgFileName);
	switch (dwRet)
	{
	case 0:
		if (MAX_PATH >= GetCurrentDirectoryA(MAX_PATH, in_out_PSDirectory->szLogFileDirectory))
			bRet = TRUE;
		break;
	case MAX_PATH - 1:
		CFuzzLog::OutputConsole("buffer too small!");
		break;
	case MAX_PATH - 2:
		CFuzzLog::OutputConsole("Key Name is NULL!");
		break;
	default:
		if (CMyFunc::GetFileFullPath(szLogFileDirectory, in_out_PSDirectory->szLogFileDirectory))
			bRet = TRUE;
		break;
	}
	CreateDirectoryA(in_out_PSDirectory->szFuzzFileDirectory, NULL);
	CreateDirectoryA(in_out_PSDirectory->szCrashFileDirectory, NULL);
	CreateDirectoryA(in_out_PSDirectory->szLogFileDirectory, NULL);

gleave:
	return bRet;
}

DWORD CTemplate::CreateFuzzFile(PSTemplate in_PSTemplate, PSDirectory in_PSDirectory)
{
	HANDLE hTemplate = NULL;
	HANDLE hTemplateMap = NULL;
	DWORD dwTemplateSize = 0;
	char *lpMapAddress = NULL;
	char *lpRewriteMem = NULL;
	char szReleaseFileName[MAX_PATH] = { 0 };
	DWORD index = 0;
	BYTE replByte = 0;
	DWORD dwCount = 0;

	hTemplate = CreateFileA(in_PSTemplate->szTemplatePath,
							GENERIC_READ,
							FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,
							NULL,
							NULL);
	if (INVALID_HANDLE_VALUE == hTemplate)
	{
		CFuzzLog::OutputConsole("CreateFileA False!");
		goto gleave;
	}
	dwTemplateSize = GetFileSize(hTemplate, NULL);
	hTemplateMap = CreateFileMappingA(hTemplate, NULL, PAGE_READONLY, 0, 0, NULL);
	if (INVALID_HANDLE_VALUE == hTemplateMap)
	{
		CFuzzLog::OutputConsole("CreateFileMappingA False!");
		goto gleave;
	}
	lpMapAddress = (char *)MapViewOfFile(hTemplateMap, FILE_MAP_READ, NULL, NULL, dwTemplateSize);
	if (!lpMapAddress)
	{
		CFuzzLog::OutputConsole("MapViewOfFile False!");
		goto gleave;
	}

	lpRewriteMem = (char *)malloc(dwTemplateSize + 1);
	memset(lpRewriteMem, 0, dwTemplateSize +1);
	char *szFileExt = PathFindExtensionA(in_PSTemplate->szTemplatePath);

	for (index = in_PSTemplate->begin_pos; index <= in_PSTemplate->end_pos; index++)
	{
		memcpy(lpRewriteMem, lpMapAddress, dwTemplateSize);
		for (replByte = in_PSTemplate->begin_byte; replByte <= in_PSTemplate->end_byte; replByte++)
		{
			*(lpRewriteMem + index) = replByte;
			sprintf_s(szReleaseFileName, MAX_PATH, "%s\\%d_%x%s", in_PSDirectory->szFuzzFileDirectory, index, replByte, szFileExt);
			CMyFunc::ReleaseFile(szReleaseFileName, lpRewriteMem, dwTemplateSize);
			dwCount++;
		}
		memset(lpRewriteMem, 0, dwTemplateSize);
	}

gleave:
	if (hTemplate)
		CloseHandle(hTemplate);
	if (hTemplateMap)
	{
		FlushViewOfFile(hTemplateMap, 0);
		UnmapViewOfFile(hTemplateMap); 
	}

	return dwCount;

}

BOOL CTemplate::CreateFuzzProcess(	PSTemplate in_PSTemplate,
									char *in_FilePath, 
									char* in_FileName, 
									char *in_szCrashFileDirectory, 
									char *in_szLogFile)
{	
	BOOL bRet = FALSE;
	BOOL isExit = FALSE;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	DEBUG_EVENT dbg;
	CONTEXT context;
	HANDLE hThread = NULL;
	HANDLE hProcess = NULL;
	DWORD start_time = 0;
	DWORD wait_time = 0;
	BOOL isException = FALSE;
	char command_line[MAX_PATH * 2] = { 0 };
	unsigned char inst_buf[32] = { 0 };
	INSTRUCTION sInst;
	char inst_string[MAX_PATH] = { 0 };
	char eInfo[MAX_PATH] = { 0 };

	wait_time = in_PSTemplate->wait_time;
	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	start_time = GetTickCount();
	sprintf_s(command_line, MAX_PATH * 2, "%s %s", in_PSTemplate->szTargetProgram, in_FilePath);
	CMyFunc::AppendWriteFile(in_szLogFile, "", in_FileName, ":\r\n");

	bRet = CreateProcessA(NULL,	// target file name.
						command_line,	// command line options.
						NULL,	// process attributes.
						NULL,	// thread attributes.
						FALSE,	// handles are not inherited.
						//CREATE_NEW_CONSOLE, // nomal console.
						DEBUG_PROCESS,	// debug the target process and all spawned children.
						NULL,	// use our current environment.
						NULL,	// use our current working directory.
						&si,	// pointer to STARTUPINFO structure.
						&pi);	// pointer to PROCESS_INFORMATION structure.
	if (!bRet)
	{
		CFuzzLog::OutputConsole("CreateProcess failed!");
		goto gleave;
	}

	// watch for an exception.
	DWORD dwTime = 0;
	while (GetTickCount() - start_time < wait_time)
	{
		if (WaitForDebugEvent(&dbg, 100))
		{
			// we are only interested in debug events.
			if (dbg.dwDebugEventCode != EXCEPTION_DEBUG_EVENT)
			{
				ContinueDebugEvent(dbg.dwProcessId, dbg.dwThreadId, DBG_CONTINUE);
				continue;
			}

			// get a handle to the offending thread.
			if ((hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dbg.dwThreadId)) == NULL)
			{
				CFuzzLog::OutputConsole("OpenThread failed!");
				goto gleave;
			}

			// get the context of the offending thread.
			context.ContextFlags = CONTEXT_FULL;

			if (GetThreadContext(hThread, &context) == 0)
			{
				CFuzzLog::OutputConsole("GetThreadContext failed!");
				goto gleave;
			}

			// examine the exception code.
			switch (dbg.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case EXCEPTION_ACCESS_VIOLATION:
				isException = TRUE;
				CFuzzLog::OutputConsole("Access Violation!");
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", "Access Violation", "\r\n");
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				isException = TRUE;
				CFuzzLog::OutputConsole("Divide by Zero!");
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", "Divide by Zero", "\r\n");
				break;
			case EXCEPTION_STACK_OVERFLOW:
				isException = TRUE;
				CFuzzLog::OutputConsole("Stack Overflow!");
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", "Stack Overflow", "\r\n");
				break;
			default:
				//printf("[*] Unknown Exception (%08x):\n", dbg.u.Exception.ExceptionRecord.ExceptionCode);
				/*
				if (0 == dwTime)
				{
					CFuzzLog::OutputConsole("Unknown Exception!");
					CMyFunc::AppendWriteFile(in_szLogFile, "\t", "Unknown Exception", "\r\n");
				}
				dwTime++;
				*/
				ContinueDebugEvent(dbg.dwProcessId, dbg.dwThreadId, DBG_CONTINUE);
				break;
			}

			// if an exception occured, print more information.
			if (isException)
			{
				// open a handle to the target process.
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dbg.dwProcessId);
				if (hProcess == NULL)
				{
					CFuzzLog::OutputConsole("OpenProcess failed!");
					goto gleave;
				}

				// grab some memory at EIP for disassembly.
				ReadProcessMemory(hProcess, (void *)context.Eip, &inst_buf, 32, NULL);

				// decode the instruction into a string.
				get_instruction(&sInst, inst_buf, MODE_32);
				get_instruction_string(&sInst, FORMAT_INTEL, 0, inst_string, sizeof(inst_string));
				/*
				// print the exception to screen.
				printf("[*] Exception caught at %08x %s\n", context.Eip, inst_string);
				printf("[*] EAX:%08x EBX:%08x ECX:%08x EDX:%08x\n", context.Eax, context.Ebx, context.Ecx, context.Edx);
				printf("[*] ESI:%08x EDI:%08x ESP:%08x EBP:%08x\n\n", context.Esi, context.Edi, context.Esp, context.Ebp);
				*/
				// log...
				sprintf_s(eInfo, MAX_PATH, "Exception caught at (EIP)%08x %s", context.Eip, inst_string);
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", eInfo, "\r\n");
				memset(eInfo, 0, MAX_PATH);
				sprintf_s(eInfo, MAX_PATH, "EAX:%08x EBX:%08x ECX:%08x EDX:%08x", context.Eax, context.Ebx, context.Ecx, context.Edx);
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", eInfo, "\r\n");
				memset(eInfo, 0, MAX_PATH);
				sprintf_s(eInfo, MAX_PATH, "ESI:%08x EDI:%08x ESP:%08x EBP:%08x", context.Esi, context.Edi, context.Esp, context.Ebp);
				CMyFunc::AppendWriteFile(in_szLogFile, "\t", eInfo, "\r\n");
				memset(eInfo, 0, MAX_PATH);

				//backup crash file
				sprintf_s(eInfo, MAX_PATH, "%s\\%s", in_szCrashFileDirectory, in_FileName);
				CopyFileA(in_FilePath, eInfo, false);

				//only record one import exception
				goto gleave;
			}
		}
	}

gleave:
	//kill process
	this->KillDebugProcess(pi.dwProcessId);
	if (hProcess)
		CloseHandle(hProcess);

	if (hThread)
		CloseHandle(hThread);

	return bRet;
}

BOOL CTemplate::KillDebugProcess(DWORD in_dwPorcessId)
{
	BOOL bRet = FALSE;
	HANDLE hProcess = NULL;
	bRet = DebugActiveProcessStop(in_dwPorcessId);
	if (FALSE == bRet)
	{
		CFuzzLog::OutputConsole("Stop Debug Process False!");
		goto gLeave;
	}
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, in_dwPorcessId);
	if (hProcess)
		TerminateProcess(hProcess, 0);
gLeave:
	if (hProcess)
		CloseHandle(hProcess);
	return bRet;

}