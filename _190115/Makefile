all : 
	make tinybc
	make tinybc_debug
	make timeserv
	make timeserv_debug

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_COMMON	= $(M_HELPERGLOBAL) $(M_HELPERMACRO)
O_STRICT	= -Werror -Wall -Wextra
O_RELEASE = -D NDEBUG
O_OPT			= -O2

tinybc : tinybc.c $(M_COMMON) 
	gcc $(O_OPT) $(O_RELEASE) -o $@ tinybc.c $(M_COMMON) 

tinybc_debug : tinybc.c $(M_COMMON) 
	gcc -o $@ tinybc.c $(M_COMMON) 

timeserv : timeserv.c $(M_COMMON) 
	gcc $(O_OPT) $(O_RELEASE) -o $@ timeserv.c $(M_COMMON) 

timeserv_debug : timeserv.c $(M_COMMON) 
	gcc -o $@ timeserv.c $(M_COMMON) 

clean:
	rm tinybc
