#include <stdio.h>
#include <curses.h>

#define NOTUSE(__Expression__) ((void)__Expression__)
#define NAMESPACE(__Specifier__) __Specifier__

void curse__initialize();
void curse__release();
void curse__clear();

static const struct
{
	void (*Initialize)(void);
	void (*Clear)(void);
	void (*Release)(void);
} NAMESPACE(curse) = {
	curse__initialize,
	curse__release,
	curse__clear,
};

void curse__initialize()
{
	initscr();
	curse.Clear();
}
void curse__release() { endwin(); }
void curse__clear()   { clear(); }

int main(int argc, char** argv)
{
	NOTUSE(argc);
	NOTUSE(argv);

	curse.Initialize();
	curse.Clear();

	move(10, 20);
	addstr("Hello, world!");
	move(LINES-1, 0);

	refresh();
	getch();
	curse.Release();

	return 0;
}
