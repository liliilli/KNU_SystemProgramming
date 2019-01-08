#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

#include "SetTicker.h"

#define NOTUSE(__Expression__) ((void)__Expression__)
#define NAMESPACE(__Specifier__) __Specifier__
#define PARSE1(__LHS__, __RHS__) __LHS__ ## __RHS__
#define PARSE2(__LHS__, __RHS__) PARSE1(__LHS__, __RHS__)

int row;
int col;
int direction; 
int delay;
const char* sBlankString = "     ";
const char* sMessageString = "Hello";

typedef void(*CbSignal)(int);

void global__SetTcProperty(int iSave);
void global__BindSIGINT(CbSignal);
void global__BindSIGALRM(CbSignal);
void global__SetEcho(int enable);
void global__SetCanonical(int enable);
void CallbackSIGINT(int iSig);
void CallbackSIGALRM(int iSig);

void curse__initialize();
void curse__release();
void curse__write(const char*);
void curse__clear();
void curse__reverseMode(int);
void curse__setCursor(int x, int y);
void curse__swapBuffer();

static const struct
{
	void (*SetTcProperty)(int);
	void (*BindSIGINT)(void(*)(int));
	void (*BindSIGALRM)(void(*)(int));
	void (*SetEcho)(int);
	void (*SetCanonical)(int);
} NAMESPACE(global) = {
	global__SetTcProperty,
	global__BindSIGINT,
	global__BindSIGALRM,
	global__SetEcho,
	global__SetCanonical,
};

static const struct
{
	void (*Initialize)(void);
	void (*SetCursor)(int, int);
	void (*Write)(const char*);
	void (*Clear)(void);
	void (*Release)(void);
	void (*SwapBuffer)(void);
	void (*SetReverseMode)(int i);
} NAMESPACE(curse) = {
	curse__initialize,
	curse__setCursor,
	curse__write,
	curse__release,
	curse__clear,
	curse__swapBuffer,
	curse__reverseMode,
};

void global__SetTcProperty(int iSave)
{
	static struct termios globalTc; 
	static int tcFlag;

	if (iSave > 0)
	{
		tcgetattr(0, &globalTc);
		tcFlag = fcntl(0, F_GETFL);
	}
	else
	{
		tcsetattr(0, TCSANOW, &globalTc);
		fcntl(0, F_SETFL, tcFlag);
	}
}

void global__BindSIGINT(CbSignal cb)
{
	signal(SIGINT, cb); 
}
void CallbackSIGINT(int iSig)
{
	global.SetTcProperty(0);
	exit(1);
}

void global__BindSIGALRM(CbSignal cb)
{
	signal(SIGALRM, cb); 
}
void CallbackSIGALRM(int iSig)
{
	signal(SIGALRM, CallbackSIGALRM);
	curse.SetCursor(row, col);
	curse.Write(sBlankString);

	col += direction;
	curse.SetCursor(row, col);
	curse.Write(sMessageString);

	curse.SwapBuffer();

	if (direction == -1 && col <= 0) { direction = 1; }
	else if (direction == 1 && col + strlen(sMessageString) >= 50) { direction = -1; }
}

void global__SetEcho(int enable)
{
	struct termios tc;
	tcgetattr(0, &tc);

	if (enable <= 0) { tc.c_lflag &= ~ECHO; }
	else { tc.c_lflag |= ECHO; }

	tcsetattr(0, TCSANOW, &tc);
}

void global__SetCanonical(int enable)
{
	struct termios tc;
	tcgetattr(0, &tc);

	if (enable <= 0) { tc.c_lflag &= ~ICANON; }
	else { tc.c_lflag |= ICANON; }

	tcsetattr(0, TCSANOW, &tc);
}

void curse__initialize()
{
	initscr();
	curse.Clear();
}
void curse__setCursor(int x, int y) { move(y, x); }
void curse__write(const char* str) { addstr(str); }
void curse__release() 
{ 
	endwin(); 
}
void curse__clear()   { clear(); }
void curse__swapBuffer(void)
{
	curse.SetCursor(LINES - 1, COLS - 1);
	refresh();
}
void curse__reverseMode(int isReverse)
{
	if (isReverse) { standout(); } 
	else           { standend(); }
}

int main(int argc, char** argv)
{
	NOTUSE(argc); NOTUSE(argv);

	row = 10;
	col = 0;
	direction = 1;
	delay = 200;

	global.BindSIGINT(CallbackSIGINT);
	global.BindSIGALRM(CallbackSIGALRM);

	global.SetTcProperty(1);
	curse.Initialize();
	curse.Clear();
	SetTicker(delay);

	while (1)
	{
		int ndelay = 0;
		char c = getchar();
		tolower(c);

		if (c == 'q') { break; }
		if (c == ' ') { direction = -direction; }
		if (c == 'f' && delay > 2) { ndelay = delay / 2; }
		if (c == 's') { ndelay = delay * 2; }
		if (ndelay > 0) { SetTicker(delay = ndelay); }
	}

	curse.SwapBuffer();
	curse.Release();
	global.SetTcProperty(0);

	return 0;
}
