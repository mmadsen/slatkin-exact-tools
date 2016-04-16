CC=gcc
CFLAGS=-g -c -Wall 
INCLUDEDIR=-I.
LDFLAGS=-lm
EXECUTABLES=slatkin-enumerate slatkin-mc 
SOURCES=slatkin_impl.c enumerate.c montecarlo.c mersenne.c
OBJECTS=$(SOURCES:.c=.o)


all: python
	

python:
	python setup.py build_ext

install:
	python setup.py install


slatkin-enumerate: enumerate.o
	$(CC) -o slatkin-enumerate enumerate.o -lm

slatkin-mc: $(OBJECTS)
	$(CC) -o slatkin-mc slatkin_impl.o montecarlo.o mersenne.o -lm

mc-loop-test: $(OBJECTS)
	$(CC) -o mc-loop-test mc_loop_test.o slatkin.o -lm


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES) *.so *.pyc slatkin.c
	rm -rf build
