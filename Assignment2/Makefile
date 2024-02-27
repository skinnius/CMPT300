CFLAGS = -Wall -ggdb -pedantic-errors -O3

# network: Networking.c
# 	gcc -Wall -ggdb -std=c99 -pedantic-errors -O3 -o out Networking.c

# thread: general.c Threads.c receiver.c
# 	gcc $(CFLAGS) Threads.c general.c receiver.c -lpthread -o main


all: main.c sockets.c send.c
	gcc $(CFLAGS) main.c sockets.c send.c input.c -lpthread -o s-talk 

test: send.c
	gcc $(CFLAGS) send.c -o a

test2: receive.c
	gcc $(CFLAGS) receive.c -o a2

clean:
	rm -f s-talk