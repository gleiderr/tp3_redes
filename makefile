all: servent #client

servent: src/servent.c obj/dictionary.o obj/neighborhood.o obj/pilha.o obj/util.o obj/msg.o
	gcc -o servent src/servent.c obj/dictionary.o obj/neighborhood.o obj/pilha.o obj/util.o obj/msg.o -Wall

#client: src/client.c src/msg.h
#	gcc -o client src/client.c -Wall

obj/dictionary.o: src/dictionary.c src/dictionary.h src/pilha.h src/msg.h
	gcc -o obj/dictionary.o -c src/dictionary.c -Wall

obj/neighborhood.o: src/neighborhood.h src/neighborhood.c src/pilha.h src/msg.h src/util.h
	gcc -o obj/neighborhood.o -c src/neighborhood.c -Wall

obj/msg.o: src/msg.c src/msg.h src/pilha.h
	gcc -o obj/msg.o -c src/msg.c -Wall

obj/pilha.o: src/pilha.c src/pilha.h
	gcc -o obj/pilha.o -c src/pilha.c -Wall

obj/util.o: src/util.c src/util.h
	gcc -o obj/util.o -c src/util.c -Wall

clean:
	rm -rf obj/*.o