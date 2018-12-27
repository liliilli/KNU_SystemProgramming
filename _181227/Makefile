cp1: cp1.o
	gcc -Wextra -Wall -Werror -o cp1 cp1.o 
cp1.o: cp1.c
	gcc -c cp1.c

who3: who3.o utmplib.o
	gcc -Wextra -Wall -Werror -o who3 who3.o utmplib.o
who3.o: who3.c utmplib.c utmplib.h
	gcc -c who3.c utmplib.c utmplib.h

mywho: mywho.o utmplib.o
	gcc -Wextra -Wall -Werror -o mywho mywho.o utmplib.o
mywho.o: mywho.c utmplib.c utmplib.h
	gcc -c mywho.c utmplib.c utmplib.h
utmplib.o: utmplib.c
	gcc -c utmplib.c

clean:
	rm ./*.o
