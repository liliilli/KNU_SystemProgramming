all : 
	make smsh1
	make smsh2

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_SETTICKER    = SetTicker.h SetTicker.c
M_HELPERCURSES = HelperCurses.h HelperCurses.c
M_COMMON	= $(M_HELPERGLOBAL) $(M_SETTICKER) $(M_HELPERMACRO)
M_SMSH		= smsh.h splitline.c execute.c
M_SMSH2		= process.c controlflow.c
L_CURSES	= -lcurses
O_STRICT	= -Werror -Wall -Wextra
O_RELEASE = -D NDEBUG
O_OPT			= -O2

smsh1 : smsh1.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH)
	gcc $(O_OPT) $(O_RELEASE) -o $@ smsh1.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH)

smsh2 : smsh2.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH) $(M_SMSH2)
	gcc $(O_OPT) $(O_RELEASE) -o $@ smsh2.c $(M_HELPERGLOBAL) $(M_HELPERMACRO) $(M_SMSH) $(M_SMSH2)

clean:
	rm smsh1
	rm smsh2
