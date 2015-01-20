all: battleship.o
	gcc battleship.o
	make run
battleship.o: battleship.c battleship.h
	gcc -c battleship.c
clean:
	rm *.o
run:
	./a.out
