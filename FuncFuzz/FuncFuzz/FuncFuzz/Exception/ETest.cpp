#include "stdafx.h"
#include <string>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>

using namespace std;

#include "Exception.h"

//	���¼������ܹ����嵽�����ڴ�й¶�Ĵ����С���ÿ��.cpp�ļ���Ӧ��������
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

	//	��ÿ���̺߳�������ڼ���������䡣
	//	����ڴ�й¶��
	CWinUtil::vCheckMemoryLeak();
	//	ʹnew����ʧ��ʱ�׳��쳣��
	CWinUtil::vSetThrowNewException();
	//	��WINDOWS�еĽṹ���쳣ת��ΪC++�쳣��
	CWinUtil::vMapSEHtoCE();
	//	����δ�����쳣�Ĺ��˺�����
	CWinUtil::vSetUnExpectedExceptionFilter();

	//	��ʼ����
	CWinUtil::vInitStackEnviroment();

	try
	{
		vTestVectorThrow();	//�޷�����
		void vNullPointer();
	}
	catch (const CRecoverableSEHException &bug)	//	���ڲ���ɻָ��Ľṹ���쳣��
	{
		cout << bug.what() << endl;
	}
	catch (const CUnRecoverableSEHException &bug)	//	���ڲ��񲻿ɻָ��Ľṹ���쳣
	{
		cout << bug.what() << endl;
	}
	catch (const exception& e)	//	���ڲ����׼C++�쳣��������
	{
		cout << e.what() << endl;
	}
	catch (...)	//	���ڲ�����Щ�׳��ǽṹ���쳣�Ͳ��Ǽ̳�exception���쳣
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
		vTestVectorThrow();	//�޷�����
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