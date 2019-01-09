///
/// @file : bounce1d.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:46
/// @description : Bounce "Hello" string 1d horizontally.
///

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
#include "HelperGlobal.h"
#include "HelperCurses.h"

int row;
int col;
int direction; 
int delay;
const char* sBlankString = "     ";
const char* sMessageString = "Hello";

// @brief Callback for `ctrl+C`.
// when signal is interrupted, restore previous setting of terminal console,
// and exit program.  
// iSig argument will not be used.
void CallbackSIGINT(int iSig);

// @brief Callback for alarm, display screen each given delay time.
// iSig will not be used.
void CallbackSIGALRM(int iSig);

int main(int argc, char** argv)
{
	NOTUSE(argc); NOTUSE(argv);

	// Set initial parameter.
	row = 10;
	col = 0;
	direction = 1;
	delay = 200;

	// Set callback of each signal.
	global.BindSIGINT(CallbackSIGINT);
	global.BindSIGALRM(CallbackSIGALRM);

	// Store setting, and initialize curses.
	// and, set delay for display function.
	global.SetTcProperty(1);
	curse.Initialize();
	curse.Clear();
	SetTicker(delay);

	while (1)
	{
		// Input sequence.
		int ndelay = 0;
		char c = getchar();
		tolower(c);

		if (c == 'q') { break; }
		if (c == ' ') { direction = -direction; }
		if (c == 'f' && delay > 2) { ndelay = delay / 2; }
		if (c == 's') { ndelay = delay * 2; }
		if (ndelay > 0) { SetTicker(delay = ndelay); }
	}

	// Release curses instance and restore stored properties.
	curse.SwapBuffer();
	curse.Release();
	global.SetTcProperty(0);

	return 0;
}

void CallbackSIGINT(int iSig)
{
	NOTUSE(iSig);
	global.SetTcProperty(0);
	exit(1);
}

void CallbackSIGALRM(int iSig)
{
	NOTUSE(iSig);
	signal(SIGALRM, CallbackSIGALRM);
	curse.SetCursor(col, row);
	curse.Write(sBlankString);

	// Moving.
	col += direction;
	curse.SetCursor(col, row);
	curse.Write(sMessageString);
	curse.SwapBuffer();

	// Boundary check.
	if (direction == -1 && col <= 0) { direction = 1; }
	else if (direction == 1 && col + strlen(sMessageString) >= 50) { direction = -1; }
}


