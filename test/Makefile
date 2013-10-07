
CC=gcc
CFLAGS=-g -c -Wall
INCLUDEDIR=-I.
LDFLAGS=
EXECUTABLES=mc-loop-test 
SOURCES=mc_loop_test.c
OBJECTS=$(SOURCES:.c=.o)


all: $(EXECUTABLES) $(OBJECTS)
	

mc-loop-test: $(OBJECTS)
	$(CC) -o mc-loop-test mc_loop_test.o ../slatkin.o


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES)