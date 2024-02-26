CFLAGS = -Wall -ggdb -Werror -pedantic-errors -O3


# network: Networking.c
# 	gcc -Wall -ggdb -std=c99 -pedantic-errors -O3 -o out Networking.c

# thread: general.c Threads.c receiver.c
# 	gcc $(CFLAGS) Threads.c general.c receiver.c -lpthread -o main


all: main.c sockets.c
	gcc $(CFLAGS) main.c sockets.c -lpthread -o s-talk 

clean:
	rm -f s-talk