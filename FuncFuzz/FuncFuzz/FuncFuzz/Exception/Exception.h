/*
*	Copyright (c) 2002-2003, author: canco, email: canco_99@sina.com.

   You are free to use it, study it, trash it, modify it, sell it with or without changes, 
    or do whatever else you want, with one restrictions and one polite requests:
Restriction 1: It's all your fault.
	These samples are _samples_. They may do something useful, or may help in understanding 
	a problem; or they may not. If they do not, I would appreciate if you let me know, so 
	I can fix the sample; but I recognise no obligation on my part for doing that. As I 
	said, it's all your fault.
Polite request 1: Give credit.
   If you appropriate one of my samples, I don't even ask to be named: but I do ask that you 
   don't claim credit for my work.

NOTE: Code of stack trace comes from StackWalk program written by Felix Kasza. Felix Kasza's
web address is http://www.mvps.org/win32/.

*/

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#pragma warning( disable: 4786 )

//#include <eh.h>
#include <string>
#include <vector>
#include <new.h>

#include <windows.h>
#include <cfloat>

using namespace std;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, THIS_FILE, __LINE__ )



//#include <stdio.h>
#include <stdlib.h>
// imagehlp.h must be compiled with packing to eight-byte-boundaries,
// but does nothing to enforce that. I am grateful to Jeff Shanholtz
// <JShanholtz@premia.com> for finding this problem.
#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )



//	Ҫ��֤�ṹ���쳣�ڷ��а汾��Ч��������/EHa����ѡ���������Ĭ�ϵġ�

//	��׼C++���������쳣��
/*
	name				throw by					header
	bad_alloc			new							<new>
	bad_cast			dynamic_cast				<typeinfo>
	bad_typeid			typeid						<typeinfo>
	bad_exception		exception specification		<exception>
	out_of_range		at(),						<stdexcept>
	invalid_argument								<stdexcept>
	overflow_error									<stdexcept>
	ios_base::failure								<ios>
	length_error									<stdexcept>
	domain_error									<stdexcept>
	underflow_error									<stdexcept>


*/

typedef const char *__exString;

class CWinUtil {
private:
	CWinUtil();
public:
	//	��������ڴ�й¶�Ĺ��ܡ�
	static void vCheckMemoryLeak();

	//	��WINDOWS�еĽṹ���쳣ת��ΪC++�쳣��
	static void vMapSEHtoCE();

	//	ʹnew����ʧ��ʱ�׳��쳣��
	static void vSetThrowNewException();

	//	�ø����������׳��쳣
	static void vSetThrowFloatException();

	static void vSetUnExpectedExceptionFilter();

	

	static int NewHandler( size_t size );

	static void vInitStackEnviroment();

private:
	static void _cdecl TranslateSEHtoCE( UINT code, PEXCEPTION_POINTERS pep ) ;
	static long _stdcall UnExpectedExceptionFilter( LPEXCEPTION_POINTERS pe ); 



};




class CSEHException : public exception {

public:
	CSEHException( UINT code, PEXCEPTION_POINTERS pep );


	operator unsigned int() { return m_exceptionCode; }


	__exString what() const { return m_strMsg.c_str(); }

protected:
	UINT m_exceptionCode;

	EXCEPTION_RECORD m_exceptionRecord;

	CONTEXT m_context;

	string m_strMsg;


};

//	�ɻָ��Ľṹ���쳣��
class CRecoverableSEHException : public CSEHException {
public:
	CRecoverableSEHException( UINT code, PEXCEPTION_POINTERS pep ) :
	  CSEHException( code, pep ){}


};

//	���ɻָ��Ľṹ���쳣��
class CUnRecoverableSEHException : public CSEHException {
public:
	CUnRecoverableSEHException( UINT code, PEXCEPTION_POINTERS pep ) :
	  CSEHException( code, pep ){}

};

class CMyException : public domain_error {
public:
	CMyException( const string& str ) ;

};

//	��SOCKET�йص��쳣��
class CSocketException : public CMyException {
public:
	CSocketException( const string& str ) : CMyException( str ) {}

};
//	�������ļ��йص��쳣��
class CConfigException : public CMyException {
public:
	CConfigException( const string& str ) : CMyException( str.c_str() ) {}

};
//	�����ݿ�����йص��쳣��
class CDatabaseException : public CMyException {
public:
	CDatabaseException( const string& str ) : CMyException( str.c_str() ) {}

};

//	������ϵͳAPI���ش����йص��쳣��
class COSApiException : public CMyException {
public:
	COSApiException( const string& str ) : CMyException( str.c_str() ) {}

};





#endif