///
/// @file : HelperCurses.c
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:42
/// @description : Helpering Curses function API container file
///

#include "HelperCurses.h"

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

void curse__initialize()
{
	initscr();
	curse.Clear();
}

void curse__setCursor(int x, int y) 
{ 
	move(y, x); 
}

void curse__write(const char* str) 
{ 
	addstr(str); 
}

void curse__release() 
{ 
	endwin(); 
}

void curse__clear()   
{
       	clear(); 
}

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


