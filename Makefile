run: build
	./server.out

build: server.out

server.out: server.c $(wildcard *.h)
	gcc -o server.out server.c

clean:
	rm -f server.out server.o