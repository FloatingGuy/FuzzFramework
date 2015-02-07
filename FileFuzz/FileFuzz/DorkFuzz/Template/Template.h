#pragma once
#include "../MyFunc.h"

typedef struct STemplate
{
	DWORD begin_pos;
	DWORD end_pos;
	DWORD wait_time;
	BYTE begin_byte;
	BYTE end_byte;
	char *szTemplatePath;
	char *szTargetProgram;

}*PSTemplate;

typedef struct SDirectory
{
	char *szFuzzFileDirectory;
	char *szCrashFileDirectory;
	char *szLogFileDirectory;

}*PSDirectory;


class CTemplate
{
public:
	CTemplate(void);
	~CTemplate(void);

public:
	BOOL GetTemplateInfo(char *in_CfgFileName, PSTemplate in_out_PSTemplate);
	BOOL GetDirectoryInfo(char *in_CfgFileName, PSDirectory in_out_PSDirectory);
	DWORD CreateFuzzFile(PSTemplate in_PSTemplate, PSDirectory in_PSDirectory);
	BOOL CreateFuzzProcess(	PSTemplate in_PSTemplate,
									char *in_FilePath, 
									char* in_FileName, 
									char *in_szCrashFileDirectory, 
									char *in_szLogFile);
	BOOL KillDebugProcess(DWORD in_dwPorcessId);
};	

