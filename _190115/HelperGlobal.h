#ifndef GUARD_HELPERGLOBAL_H
#define GUARD_HELPERGLOBAL_H
///
/// @file : HelperGlobal.h
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-11 11:58 Updated
/// @description : Global function API container file
///

#include "HelperMacro.h"

typedef void(*CbSignal)(int);

void global__SetTcProperty(int iSave);
void global__BindSIGINT(CbSignal);
void global__BindSIGQUIT(CbSignal);
void global__BindSIGALRM(CbSignal);
void global__BindSIGIO(CbSignal);
void global__BindSIGCHLD(CbSignal);
void global__SetEcho(int enable);
void global__SetCanonical(int enable);
void global__SetAsyncIO(int enable);
void global__GetScreenSize(int* width, int* height);

static const struct
{
	void (*SetTcProperty)(int);
	void (*BindSIGINT)(void(*)(int));
	void (*BindSIGQUIT)(void(*)(int));
	void (*BindSIGALRM)(void(*)(int));
	void (*BindSIGIO)(void(*)(int));
	void (*BindSIGCHLD)(void(*)(int));
	void (*SetEcho)(int);
	void (*SetCanonical)(int);
	void (*SetAsyncIO)(int);
	void (*GetScreenSize)(int*, int*);
} NAMESPACE(global) = {
	global__SetTcProperty,
	global__BindSIGINT,
	global__BindSIGQUIT,
	global__BindSIGALRM,
	global__BindSIGIO,
	global__BindSIGCHLD,
	global__SetEcho,
	global__SetCanonical,
	global__SetAsyncIO,
	global__GetScreenSize,
};

// @brief Execute another program.
// @param isChild If value is `M_CHILD`, fork new child process and let it execute program.
// Parent process will receive child process id.
// If not but `M_NOTCHILD`, Parent process will execute specified program.
int system__ExecuteProgram(const char* iProgramPath, char* const iProgramArgs[], int isChild);
#define M_CHILD	   1
#define M_NOTCHILD 0

// @brief Calculate and get return value, dump flag and signal number of child process.
// @param iWstatus wait status which is returned by child process when exit. 
void system__GetWaitStatus(const int iWstatus, int* iReturnedValue, int* iDumpFlag, int* iSignalNumber); 

// @brief Fork child process, parent process will get -1 (error) or child pid index.
// Child pid will get 0 when creation is succeeded.
int system__ForkChild();

// @brief Fork child process, parent process will get -1 (error) or child pid index.
// Child pid will get 0 when creation is succeeded.
int system__ForkChildExt(
		void (*iChildCb)(void* iChildParam), 
		void* iChildArg,
		void (*iParentCb)(void* iParentParam),
		void* iParentArg);

static const struct
{
	int (*ExecuteProgram)(const char*, char* const[], int);
	void (*GetWaitStatus)(const int, int*, int*, int*);
	int (*ForkChild)();
	int (*ForkChildExt)(void (*)(void*), void*, void (*)(void*), void*);
} NAMESPACE(systm) = {
	system__ExecuteProgram,
	system__GetWaitStatus,
	system__ForkChild,
	system__ForkChildExt,
};

// @brief Assert this code is never readched by any process of this application.
// If reached, print error and exit abnormally with error status value.
void assert__NeverReachThisLine(const char* iAdditionalMsg);

// @brief Assert fatal error, with two error message and exit with status value.
void assert__Fatal(char* iS1, char* iS2, int n);

// @brief Assert with message.
void assert__Assert(int iResult, const char* iExpression, const char* iMsg);

// @brief Assert with message.
void assert__MustSuccess(int iResult, const char* iMsg);

#ifndef NDEBUG
#define M_ASSERT(__Expression__, __Message__) \
		assert.Assert(__Expression__, #__Expression__, __Message__)
#else
#define M_ASSERT(__Expression__, __Message__) (void)0;
#endif

static const struct
{
	void (*NeverReachThisLine)(const char*);
	void (*Fatal)(char*, char*, int);
	void (*Assert)(int, const char*, const char*);
	void (*MustSuccess)(int, const char*);
} NAMESPACE(assert) = {
	assert__NeverReachThisLine,
	assert__Fatal,
	assert__Assert,
	assert__MustSuccess,
};

#endif /// GUARD_HELPERGLOBAL_H
