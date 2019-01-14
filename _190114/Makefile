all : 
	make pipe2

M_HELPERMACRO  = HelperMacro.h
M_HELPERGLOBAL = HelperGlobal.h HelperGlobal.c
M_COMMON	= $(M_HELPERGLOBAL) $(M_HELPERMACRO)
O_STRICT	= -Werror -Wall -Wextra
O_RELEASE = -D NDEBUG
O_OPT			= -O2

pipe2 : pipe2.c $(M_COMMON) 
	gcc $(O_OPT) $(O_RELEASE) -o $@ pipe2.c $(M_COMMON) 

clean:
	rm pipe2
