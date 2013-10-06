CC=gcc
CFLAGS=-g -c -Wall
INCLUDEDIR=-I.
LDFLAGS=
EXECUTABLES=slatkin-enumerate slatkin-mc 
SOURCES=slatkin-common.c enumerate.c montecarlo.c mersenne.c
SWIG_SRC=slatkinexact.i
OBJECTS=$(SOURCES:.c=.o)
SWIG=swig -python


all: $(EXECUTABLES) $(OBJECTS) python-ext
	

python-ext:
	$(SWIG) $(SWIG_SRC)

slatkin-enumerate: enumerate.o
	$(CC) -o slatkin-enumerate enumerate.o

slatkin-mc: $(OBJECTS)
	$(CC) -o slatkin-mc slatkin-common.o montecarlo.o mersenne.o

mc-loop-test: $(OBJECTS)
	$(CC) -o mc-loop-test mc_loop_test.o slatkin-common.o


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES) slatkinexact_wrap.c slatkinexact.py *.so
	rm -rf build