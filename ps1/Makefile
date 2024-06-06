CC = gcc
CFLAGS = -std=c11 -Werror -Wall -lm

all: ps1

ps1: playfair.o bmp.o main.o
	$(CC) $(CFLAGS) -o ps1 playfair.o bmp.o main.o

playfair.o: playfair.c playfair.h
	$(CC) $(CFLAGS) -c playfair.c

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o ps1