CC = gcc
CFLAGS = -Wall -g
DEPS = backpack.h command.h container.h game.h item.h parser.h room.h world.h
OBJ = backpack.o command.o container.o game.o item.o main.o parser.o room.o world.o

all:	adventure
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

adventure: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o adventure
