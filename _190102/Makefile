all : 
	make myspwd
	make spwd
all_debug :
	make myspwd_debug
	make spwd_debug

myspwd : myspwd.c
	gcc -D NDEBUG -Wextra -Werror -Wall -O2 -o myspwd myspwd.c
spwd : spwd.c
	gcc -D NDEBUG -Wextra -Werror -Wall -O2 -o spwd spwd.c

myspwd_debug : myspwd.c
	gcc -Wextra -Werror -Wall -o myspwd_debug myspwd.c
spwd_debug : spwd.c
	gcc -Wextra -Werror -Wall -o spwd_debug spwd.c

clean :
	rm ./*.o
