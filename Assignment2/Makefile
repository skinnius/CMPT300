CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

network: Networking.c
	gcc -Wall -ggdb -std=c99 -pedantic-errors -O3 -o out Networking.c

thread: general.c Threads.c 
	gcc $(CFLAGS) Threads.c general.c -lpthread -o main

clean:
	rm -f out.exe