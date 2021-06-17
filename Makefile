all: server client

server: bin/aurrasd

client: bin/aurras

bin/aurrasd: obj/aurrasd.o obj/basicOperations.o obj/request.o obj/filters.o
	gcc -g obj/aurrasd.o obj/basicOperations.o obj/request.o obj/filters.o -o bin/aurrasd

obj/aurrasd.o: src/aurrasd.c
	gcc -Wall -g -c src/aurrasd.c -o obj/aurrasd.o

bin/aurras: obj/aurras.o
	gcc -g obj/aurras.o -o bin/aurras

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
	bin/aurras samples/sample-1.mp3 tmp/sample-1.mp3
	bin/aurras samples/sample-2.mp3 tmp/sample-2.mp3

serverMal:
	gcc -Wall -g src/filters.c src/request.c src/basicOperations.c src/aurrasd.c -o bin/aurrasd

clientMal:
	gcc -Wall -g src/clientMal.c -o bin/aurras
