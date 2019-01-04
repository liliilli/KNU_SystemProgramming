all :
	make mywrite
	make col_ls
all_debug:
	make mywrite_debug
	make col_ls_debug

mywrite : mywrite.c
	gcc -O2 -Werror -Wextra -Wall -o mywrite mywrite.c
mywrite_debug : mywrite.c
	gcc -o mywrite_d mywrite.c

col_ls : col_ls.c
	gcc -O2 -Werror -Wextra -Wall -o col_ls col_ls.c
col_ls_debug : col_ls.c
	gcc -o col_ls_d col_ls.c

clean :
	rm ./*.o
