all: battleship.o
	gcc battleship.o

battleship.o: battleship.c battleship.h
	gcc -c battleship.c
clean:
	rm *.o
