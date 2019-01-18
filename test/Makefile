all:
	make mydir
	make mydir_d

RELEASE = -O2 -D NDEBUG
STRICT = -Wextra -Wall -Werror

mydir: mydir.c
	gcc $(RELEASE) $(STRICT) -o $@ mydir.c

mydir_d: mydir.c
	gcc $(STRICT) -o $@ mydir.c

clean:
	rm mydir
	rm mydir_d
