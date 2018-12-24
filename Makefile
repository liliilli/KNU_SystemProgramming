mypgm : libfoo.a mypgm.o
	gcc -o mypgm mypgm.o libfoo.a
libfoo.a : bill.o fred.o
	ar crv libfoo.a bill.o fred.o
bill.o : bill.c
	gcc -c bill.c
fred.o : fred.c
	gcc -c fred.c
mypgm.o : mypgm.c
	gcc -c mypgm.c

clean :
	rm ./*.o ./*.a
