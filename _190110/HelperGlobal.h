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

static const struct
{
	int (*ExecuteProgram)(const char*, char* const[], int);
	void (*GetWaitStatus)(const int, int*, int*, int*);
} NAMESPACE(systm) = {
	system__ExecuteProgram,
	system__GetWaitStatus,
};

#endif /// GUARD_HELPERGLOBAL_H
