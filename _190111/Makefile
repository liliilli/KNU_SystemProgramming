all : 
	make smsh1

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_SETTICKER    = SetTicker.h SetTicker.c
M_HELPERCURSES = HelperCurses.h HelperCurses.c
M_COMMON = $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERMACRO)
M_SMSH   = smsh.h splitline.c execute.c
L_CURSES = -lcurses
O_STRICT = -Werror -Wall -Wextra
O_OPT    = -O2

smsh1 : smsh1.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH)
	gcc $(O_OPT) -o $@ smsh1.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH)

clean:
	rm smsh1
