//
// @file : bounce_async.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-09 23:41
// @description : 
//
// To make application, do commend like this.
// >> make bounce_async
// or
// >> make all
//

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
int delay;
int scrWidth;
int scrHeight;
int dir = 1;
int done = 0;
const char* sBlankString = "      ";
const char* sMessageString = "HELLO";

// @brief Callback for `ctrl+C`.
// when signal is interrupted, restore previous setting of terminal console,
// and exit program.  
// iSig argument will not be used.
void CallbackSIGINT(int iSig);

// @brief Callback for alarm, display screen each given delay time.
// iSig will not be used.
void CallbackSIGALRM(int iSig);

// @brief Input callback for SIGIO.
void OnInput(int iSig);

int main(int argc, char** argv)
{
	NOTUSE(argc); NOTUSE(argv);

	// Store setting, and initialize curses. & initialize screen size.
	// and, set delay for display function.
	global.GetScreenSize(&scrWidth, &scrHeight);
	global.SetTcProperty(1);
	global.SetEcho(0);
	global.SetCanonical(0);
	global.SetAsyncIO(1);

	// Set callback of each signal.
	global.BindSIGINT(CallbackSIGINT);
	global.BindSIGALRM(CallbackSIGALRM);
	global.BindSIGIO(OnInput);

	// Set initial parameter.
	col = scrWidth / 2; row = scrHeight / 2;
	delay = 200;

	// Initialize curses
	curse.Initialize();
	curse.Clear();
	SetTicker(delay);

	curse.SetCursor(col, row);
	curse.Write(sMessageString);

	while (done == 0)
	{
		pause();
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
	global.BindSIGALRM(CallbackSIGALRM);
	mvaddstr(row, col, sBlankString);
	col += dir;
	mvaddstr(row, col, sMessageString);
	refresh();
	
	if (dir == -1 && col <= 0) { dir = 1; }
	else if (dir == 1 && col + strlen(sMessageString) >= (unsigned)scrWidth) { dir = -1; }
}


void OnInput(int iSig)
{
	NOTUSE(iSig);
	int c = getch();
	if (c == 'Q' || c == 'q' || c == EOF)
	{
		done = 1;
	}
	else if (c == ' ')
	{
		dir = -dir;
	}
}
