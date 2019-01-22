all : 
	make snake
	make snake_debug

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c HelperCurses.h HelperCurses.c SetTicker.h SetTicker.c
M_COMMON  = $(M_HELPERGLOBAL) $(M_HELPERMACRO)
M_CURSES  = -lcurses
O_STRICT  = -Werror -Wall -Wextra
O_RELEASE = -D NDEBUG
O_OPT     = -O2

snake : snake.c $(M_COMMON) 
	gcc $(O_OPT) $(O_RELEASE) -o $@ snake.c $(M_COMMON) $(M_CURSES) 

snake_debug : snake.c $(M_COMMON) 
	gcc -o $@ snake.c $(M_COMMON) $(M_CURSES)

clean:
	rm snake
	rm snake_debug
