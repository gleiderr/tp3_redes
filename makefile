CC=gcc
CFLAGS=-W -Wall -ansi -pedantic

all: servent client

servent: obj/mensagem.o src/servent.c
	gcc -o servent src/servent.c obj/msg.o

client: obj/msg.o src/client.c
	gcc -o client src/client.c obj/msg.o

obj/msg.o: src/msg.h
	gcc -o obj/msg.o

clean:
	rm -rf obj/*.o
