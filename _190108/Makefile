all : bounce1d bounce2d
	make bounce1d
	make bounce2d
	make bounce_async

M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_HELPERCURSES = HelperCurses.h HelperCurses.c
M_SETTICKER = SetTicker.h SetTicker.c
M_COMMON = $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERCURSES)
L_CURSES = -lcurses
O_STRICT = -Werror -Wall -Wextra

bounce1d : bounce1d.c $(M_COMMON)
	gcc -O2 $(O_STRICT) -o $@ bounce1d.c $(M_COMMON) $(L_CURSES)
	
bounce2d : bounce2d.c $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERCURSES)
	gcc -O2 $(O_STRICT) -o $@ bounce2d.c $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERCURSES) $(L_CURSES)
	
bounce_async : bounce_async.c $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERCURSES)
	gcc -O2 $(O_STRICT) -o $@ bounce_async.c $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERCURSES) $(L_CURSES)

clean:
	rm bounce1d
	rm bounce2d
	rm bounce_async



