all: server client

server: bin/aurrasd

client: bin/aurras

bin/aurrasd: obj/aurrasd.o obj/basicOperations.o obj/request.o obj/filters.o
	gcc -g obj/aurrasd.o obj/basicOperations.o obj/request.o obj/filters.o -o bin/aurrasd

obj/aurrasd.o: src/aurrasd.c
	gcc -Wall -g -c src/aurrasd.c -o obj/aurrasd.o

bin/aurras: obj/aurras.o obj/basicOperations.o
	gcc -g obj/aurras.o obj/basicOperations.o -o bin/aurras

obj/aurras.o: src/aurras.c
	gcc -Wall -g -c src/aurras.c -o obj/aurras.o

obj/basicOperations.o: src/basicOperations.c
	gcc -Wall -g -c src/basicOperations.c -o obj/basicOperations.o

obj/request.o: src/request.c
	gcc -Wall -g -c src/request.c -o obj/request.o

obj/filters.o: src/filters.c
	gcc -Wall -g -c src/filters.c -o obj/filters.o

clean:
	rm obj/* tmp/* bin/aurras bin/aurrasd

test:
	bin/aurras transform samples/sample-1-so.m4a tmp/sample-1-soOutput.mp3 alto alto
	bin/aurras transform samples/sample-2-miei.m4a tmp/sample-1-mieiOutput.mp3 alto
