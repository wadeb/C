# Simple Makefile to compile a very simple driver sim

CFLAGS= -Wall -Werror -Wextra
LFLAGS = -lpthread

all: request_and_service.o
	gcc request_and_service.o -o simple_driver $(LFLAGS)

file_gen: file_gen.o
	gcc file_gen.o -o file_gen

file_gen.o: file_gen.c
	gcc $(CFLAGS) -c file_gen.c

request_and_service.o: request_and_service.c
	gcc $(CFLAGS) -c request_and_service.c

clean:
	rm -f *.o simple_driver test* file_gen

speed: CFLAGS += -O3
speed: all

debug: CFLAGS += -g
debug: all

