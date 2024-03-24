CFLAGS = -Wall -ggdb -pedantic-errors -O3

all: main.c send.c input.c display.c receive.c
	gcc $(CFLAGS) main.c send.c input.c display.c receive.c list.o -lpthread -o s-talk 

clean:
	rm -f s-talk