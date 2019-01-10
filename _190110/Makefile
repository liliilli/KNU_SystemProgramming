all : 
	make waitdemo3 

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_SETTICKER    = SetTicker.h SetTicker.c
M_HELPERCURSES = HelperCurses.h HelperCurses.c
M_COMMON = $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERMACRO)
L_CURSES = -lcurses
O_STRICT = -Werror -Wall -Wextra
O_OPT    = -O2

waitdemo3 : waitdemo3.c $(M_HELPERGLOBAL) $(M_HELPERMACRO)
	gcc $(O_OPT) $(O_STRICT) -o $@ $? $(M_HELPERGLOBAL) $(M_HELPERMACRO)

clean:
	rm waitdemo3
