all:
	make ls2
	make myls
ls2 : ls2.o
	gcc -Wextra -Wall -Werror -o ls2 ls2.o
ls2.o : ls2.c
	gcc -c ls2.c
myls : myls.o
	gcc -Wextra -Wall -Werror -o myls myls.o
myls.o : myls.c
	gcc -c myls.c
clean :
	rm ./*.o
