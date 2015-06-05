# Simple Makefile to compile a very simple driver sim

CFLAGS= -Wall -Werror -Wextra
LFLAGS = -lpthread

all: request_and_service.o
	gcc request_and_service.o -o simple_driver $(LFLAGS)

request_and_service.o:
	gcc $(CFLAGS) -c request_and_service.c

clean:
	rm -f *.o simple_driver

speed: CFLAGS += -O3
speed: all

debug: CFLAGS += -g
debug: all

