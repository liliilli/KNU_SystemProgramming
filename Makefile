all :
	make mywrite
all_debug:
	make mywrite_debug

mywrite : mywrite.c
	gcc -O2 -Werror -Wextra -Wall -o mywrite mywrite.c
mywrite_debug : mywrite.c
	gcc -o mywrite_d mywrite.c

clean :
	rm ./*.o
