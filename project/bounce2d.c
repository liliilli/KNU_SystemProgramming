//
// @file : snake.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-09 23:28
// @description : 
// Bounce "o" string 2d horizontally.
//
// 's' Fast up `x` complexitivity.
// 'f' Slow down `x` complexcitivty.
// 'S' Fast up 'y' complexcitivty.
// 'F' Slow down 'y' complexcitivity.
//
// 'q' Exit application.
// 
// To make application, do commend like this.
// >> make bounce2d
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
const char* sBlankString = " ";
const char* sMessageString = "o";

// Movement
int movement[4][2] =
{
	{1, 1},
	{1, -1},
	{-1, -1},
	{-1, 1},
};
int movementState = 0;
int xSpeed = 1;
int ySpeed = 1;

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


	// Set callback of each signal.
	global.BindSIGINT(CallbackSIGINT);
	global.BindSIGALRM(CallbackSIGALRM);

	// Store setting, and initialize curses. & initialize screen size.
	// and, set delay for display function.
	global.SetTcProperty(1);
	global.GetScreenSize(&scrWidth, &scrHeight);
	// Set initial parameter.
	col = scrWidth / 2; row = scrHeight / 2;
	delay = 1000 / 60;
	// Initialize curses
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
		if (c == ' ') { movementState = (movementState + 2) % 4; }
		if (c == 'f') 
		{ 
			if (xSpeed > 0) { xSpeed--; } 
		}
		if (c == 's') 
		{ 
			if (xSpeed < 10) { xSpeed++; } 
		}
		if (c == 'F') 
		{ 
			if (ySpeed > 0) { ySpeed--; } 
		}
		if (c == 'S') 
		{ 
			if (ySpeed < 10) { ySpeed++; } 
		}
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

// @brief Check [x, y] is out of bound of [width, height]
int IsOutOfBoundary(int width, int height, int x, int y);
// @brief Get next status of bounce ball.
int NextStatus(int i);
// @brief Move position without sweep.
void MovePos(int* x, int* y, int status, float xSpeed, float ySpeed);
// @brief Move position with sweep. [x, y] will be aligned within given [scrWidth, scrHeight]
int MoveSweep(int* x, int* y, int status, float xSpeed, float ySpeed);

void CallbackSIGALRM(int iSig)
{
	NOTUSE(iSig);
	signal(SIGALRM, SIG_IGN);
	curse.SetCursor(col, row);
	curse.Write(sBlankString);

	// Move & Boundary check.
	// If not oob, write.
	const int oob = MoveSweep(&col, &row, movementState, xSpeed, ySpeed);
	curse.SetCursor(col, row);
	curse.Write(sMessageString);

	if (oob == 1)
	{
		// First, make position to be oob of screen.
		MovePos(&col, &row, movementState, xSpeed, ySpeed);
		// Get appropriate status for next moving.
		while (IsOutOfBoundary(scrWidth, scrHeight, col, row) == 1)
		{
			movementState = NextStatus(movementState);
			MovePos(&col, &row, movementState, xSpeed, ySpeed);
		}
	}

	curse.SwapBuffer();
	signal(SIGALRM, CallbackSIGALRM);
}

int IsOutOfBoundary(int width, int height, int x, int y)
{
	// width - 1 is needed to avoiding sticking out from right to left 0 col.
	if (x < 0 || x >= width-1 || y < 0 || y >= height)
	{ return 1; }
	else
	{ return 0; }
}

int NextStatus(int i)
{
	return (i + 1) % 4;
}

void MovePos(int* x, int* y, int status, float xSpeed, float ySpeed)
{
	*x += (int)(movement[status][1] * xSpeed);
	*y += (int)(movement[status][0] * ySpeed);
}

int MoveSweep(int* x, int* y, int status, float xSpeed, float ySpeed)
{
	// Position-marching.
	float offset = 1.0f;
	int newX;
	int newY;

	// If moved position is oof of screen, flag will be setup.
	int oob = 0;
	while (1)
	{
		newX = *x;
		newY = *y;
		// Move
		MovePos(&newX, &newY, status, xSpeed * offset, ySpeed * offset);
		offset -= 0.1f;

		if (IsOutOfBoundary(scrWidth, scrHeight, newX, newY) == 1) { oob = 1; }
		else { break; }
	};

	// Reflect not oob position.
	*x = newX;
	*y = newY;
	return oob;
}
