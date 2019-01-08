#include <stdio.h>
#include <curses.h>

#define NOTUSE(__Expression__) ((void)__Expression__)
#define NAMESPACE(__Specifier__) __Specifier__

void curse__initialize();
void curse__release();
void curse__write(const char*);
void curse__clear();
void curse__reverseMode(int);
void curse__move(int x, int y);

static const struct
{
	void (*Initialize)(void);
	void (*MoveCursor)(int, int);
	void (*Write)(const char*);
	void (*Clear)(void);
	void (*Release)(void);
	void (*SetReverseMode)(int i);
} NAMESPACE(curse) = {
	curse__initialize,
	curse__move,
	curse__write,
	curse__release,
	curse__clear,
	curse__reverseMode,
};

void curse__initialize()
{
	initscr();
	curse.Clear();
}
void curse__move(int x, int y) { move(x, y); }
void curse__write(const char* str) { addstr(str); }
void curse__release() { endwin(); }
void curse__clear()   { clear(); }
void curse__reverseMode(int isReverse)
{
	if (isReverse) { standout(); } else { standend(); }
}

int main(int argc, char** argv)
{
	NOTUSE(argc);
	NOTUSE(argv);

	curse.Initialize();
	curse.Clear();

	for (int i = 0; i < LINES; ++i)
	{
		curse.MoveCursor(i, i+1);
		if (i % 2 == 1) { curse.SetReverseMode(1); }
		curse.Write("Hello world!");
		if (i % 2 == 1) { curse.SetReverseMode(0); }
	}

	refresh();
	getch();
	curse.Release();

	return 0;
}
