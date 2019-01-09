#ifndef GUARD_HELPERGLOBAL_H
#define GUARD_HELPERGLOBAL_H
///
/// @file : HelperGlobal.h
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:02
/// @description : Global function API container file
///

#include "HelperMacro.h"

typedef void(*CbSignal)(int);

void global__SetTcProperty(int iSave);
void global__BindSIGINT(CbSignal);
void global__BindSIGALRM(CbSignal);
void global__BindSIGIO(CbSignal);
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
	void (*SetEcho)(int);
	void (*SetCanonical)(int);
	void (*SetAsyncIO)(int);
	void (*GetScreenSize)(int*, int*);
} NAMESPACE(global) = {
	global__SetTcProperty,
	global__BindSIGINT,
	global__BindSIGALRM,
	global__BindSIGIO,
	global__SetEcho,
	global__SetCanonical,
	global__SetAsyncIO,
	global__GetScreenSize,
};

#endif /// GUARD_HELPERGLOBAL_H
