#include <stdio.h>
#include <curses.h>
#include <unistd.h>

#define NOTUSE(__Expression__) ((void)__Expression__)
#define NAMESPACE(__Specifier__) __Specifier__
#define PARSE1(__LHS__, __RHS__) __LHS__ ## __RHS__
#define PARSE2(__LHS__, __RHS__) PARSE1(__LHS__, __RHS__)

void curse__initialize();
void curse__release();
void curse__write(const char*);
void curse__clear();
void curse__reverseMode(int);
void curse__setCursor(int x, int y);
void curse__swapBuffer();

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

void curse__initialize()
{
	initscr();
	curse.Clear();
}
void curse__setCursor(int x, int y) { move(y, x); }
void curse__write(const char* str) { addstr(str); }
void curse__release() { endwin(); }
void curse__clear()   { clear(); }
void curse__swapBuffer(void)
{
	curse.SetCursor(LINES - 1, COLS - 1);
	refresh();
}
void curse__reverseMode(int isReverse)
{
	if (isReverse) { standout(); } else { standend(); }
}

int main(int argc, char** argv)
{
	NOTUSE(argc); NOTUSE(argv);

	int pos = 10;
	int direction = 1;

	curse.Initialize();
	curse.Clear();

	while (1)
	{
		curse.SetCursor(pos, 10); 
		curse.Write("Hello!");
		curse.SwapBuffer();
		usleep(32000);
		curse.SetCursor(pos, 10);
		curse.Write("      ");
		pos += direction;

		if (pos >= 30) { direction = -1; }
		else if (pos <= 10) { direction += 1; }
	}

	refresh();
	getch();
	curse.Release();

	return 0;
}
