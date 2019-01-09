#ifndef GUARD_HELPERCURSES_H
#define GUARD_HELPERCURSES_H
///
/// @file : HelperCurses.h
/// @author : 2012104208 Jongmin Yun
/// @datetime : 2019-01-09 21:35
/// @description : Helpering Curses function API container file
///

#include "HelperMacro.h"

void curse__initialize();
void curse__release();
void curse__write(const char*);
void curse__clear();
void curse__reverseMode(int);
void curse__setCursor(int x, int y);
void curse__swapBuffer();
void curse__writeTo(int x, int y, const char*);
void curse__moveWrite(int x, int y, const char*);

static const struct
{
	void (*Initialize)(void);
	void (*SetCursor)(int, int);
	void (*Write)(const char*);
	void (*WriteTo)(int, int, const char*);
	void (*MoveWrite)(int, int, const char*);
	void (*Clear)(void);
	void (*Release)(void);
	void (*SwapBuffer)(void);
	void (*SetReverseMode)(int i);
} NAMESPACE(curse) = {
	curse__initialize,
	curse__setCursor,
	curse__write,
	curse__writeTo,
	cusre__moveWrite,
	curse__release,
	curse__clear,
	curse__swapBuffer,
	curse__reverseMode,
};

#endif /// GUARD_HELPERCURSES_H
