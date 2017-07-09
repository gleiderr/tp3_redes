all: servent client

servent: src/servent.c obj/dictionary.o obj/pilha.o src/msg.h
	gcc -o servent src/servent.c obj/dictionary.o obj/pilha.o -Wall

client: obj/msg.o src/client.c src/msg.h
	gcc -o client src/client.c

obj/dictionary.o: src/dictionary.c src/dictionary.h src/pilha.h src/msg.h
	gcc -o obj/dictionary.o -c src/dictionary.c -Wall

# obj/neighborhood.o: src/neighborhood.h src/neighborhood.c
	# gcc -o obj/neighborhood.o -c src/neighborhood.c -Wall

obj/pilha.o: src/pilha.c src/pilha.h
	gcc -o obj/pilha.o -c src/pilha.c -Wall

clean:
	rm -rf obj/*.o