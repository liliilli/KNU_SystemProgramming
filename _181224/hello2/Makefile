hello : hello.o func.o
	gcc -Wall -Werror -Wextra -o hello hello.o func.o
hello.o : hello.c
	gcc -c hello.c
func.o : func.c
	gcc -c func.c

clean:
	rm ./*.o
