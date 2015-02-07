#include "stdafx.h"
#include <string>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>

using namespace std;

#include "Exception.h"

//	以下几行是能够定义到发生内存泄露的代码行。在每个.cpp文件都应该声明。
#ifdef _DEBUG 
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable: 4700 )

void vNullPointer()
{
	int* pInt = NULL;
	*pInt = 5;
}

void vDivideByZero()
{
	int iZero = 0;
	int iRet = 5 / iZero;
	cout << "i/0= " << iRet << endl;
}

void vTestVectorThrow()
{
	vector<int> vectInt;
	vectInt[3] = 100;
}

void ETest()
{
	ofstream ofLog("exception.txt", ios_base::app);
	streambuf *outbuf = cout.rdbuf(ofLog.rdbuf());
	cout << "test1" << endl;
	cout << "test1" << endl;
	cout << "test1" << endl;
	// restore the buffers
	cout.rdbuf(outbuf);
	cout << "test2" << endl;
	cout << "test2" << endl;
	cout << "test2" << endl;

	//	在每个线程函数的入口加上以下语句。
	//	检查内存泄露。
	CWinUtil::vCheckMemoryLeak();
	//	使new函数失败时抛出异常。
	CWinUtil::vSetThrowNewException();
	//	把WINDOWS中的结构化异常转化为C++异常。
	CWinUtil::vMapSEHtoCE();
	//	设置未处理异常的过滤函数。
	CWinUtil::vSetUnExpectedExceptionFilter();

	//	初始化。
	CWinUtil::vInitStackEnviroment();

	try
	{
		vTestVectorThrow();	//无法捕获
		void vNullPointer();
	}
	catch (const CRecoverableSEHException &bug)	//	用于捕获可恢复的结构化异常。
	{
		cout << bug.what() << endl;
	}
	catch (const CUnRecoverableSEHException &bug)	//	用于捕获不可恢复的结构化异常
	{
		cout << bug.what() << endl;
	}
	catch (const exception& e)	//	用于捕获标准C++异常及其子类
	{
		cout << e.what() << endl;
	}
	catch (...)	//	用于捕获那些抛出非结构化异常和不是继承exception的异常
	{
		/*		__asm int 3*/
		cout << " else exception." << endl;
	}

	/*	ofstream ofLog( "exception.txt", ios_base::app );
	streambuf *outbuf = cout.rdbuf( ofLog.rdbuf() );
	streambuf *errbuf = cerr.rdbuf( ofLog.rdbuf() );
	*/
	try
	{
		throw CMyException(" my exception");
	}
	catch (const CRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const CUnRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << " else exception." << endl;
	}

	char* pcLeak = new char[100];

	/*	// restore the buffers
	cout.rdbuf( outbuf );
	cerr.rdbuf( errbuf );
	*/

	try
	{
		vDivideByZero();
		int i = 1;
	}
	catch (const CRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const CUnRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << " else exception." << endl;
	}


	try
	{
		vTestVectorThrow();	//无法捕获
		int i = 1;
	}
	catch (const CRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const CUnRecoverableSEHException &bug)
	{
		cout << bug.what() << endl;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "else exception." << endl;
	}

	int i;
	cin >> i;
}