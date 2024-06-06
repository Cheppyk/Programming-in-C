HOF_SOURCE = hof.c
K_SOURCE = k.c
UI_SOURCE = ui.c

TARGET = game

CC = gcc

hof.o: $(HOF_SOURCE)
	$(CC) -c $(HOF_SOURCE) -o hof.o

k.o: $(K_SOURCE)
	$(CC) -c $(K_SOURCE) -o k.o

ui.o: $(UI_SOURCE)
	$(CC) -c $(UI_SOURCE) -o ui.o

all: hof.o k.o ui.o
	$(CC) hof.o k.o ui.o -o $(TARGET)

clean:
	rm -f hof.o k.o ui.o $(TARGET)
