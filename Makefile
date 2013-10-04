CC=gcc
CFLAGS=-c -Wall
INCLUDEDIR=-I.
LDFLAGS=
EXECUTABLES=slatkin-enumerate slatkin-mc 
SOURCES=slatkin-common.c enumerate.c montecarlo.c
OBJECTS=$(SOURCES:.c=.o)


all: $(EXECUTABLES) $(OBJECTS)
	

slatkin-enumerate: enumerate.o
	$(CC) -o slatkin-enumerate enumerate.o

slatkin-mc: $(OBJECTS)
	$(CC) -o slatkin-mc slatkin-common.o montecarlo.o



.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES)