CC=gcc
CFLAGS=-g -c -Wall
INCLUDEDIR=-I.
LDFLAGS=
EXECUTABLES=slatkin-enumerate slatkin-mc mc-loop-test
SOURCES=slatkin-common.c enumerate.c montecarlo.c mc_loop_test.c
OBJECTS=$(SOURCES:.c=.o)


all: $(EXECUTABLES) $(OBJECTS)
	

slatkin-enumerate: enumerate.o
	$(CC) -o slatkin-enumerate enumerate.o

slatkin-mc: $(OBJECTS)
	$(CC) -o slatkin-mc slatkin-common.o montecarlo.o

mc-loop-test: $(OBJECTS)
	$(CC) -o mc-loop-test mc_loop_test.o slatkin-common.o


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES)