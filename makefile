project : main.o mine.o
	cc main.o mine.o -o project -lmenu -lncurses
mine.o : mine.c
	cc -c mine.c
main.o : main.c
	cc -c main.c
clean:
	rm -f *.o project
