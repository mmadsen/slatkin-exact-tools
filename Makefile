CC=gcc
CFLAGS=-g -c -Wall
INCLUDEDIR=-I.
LDFLAGS=
EXECUTABLES=slatkin-enumerate slatkin-mc 
SOURCES=slatkin.c enumerate.c montecarlo.c mersenne.c
SWIG_SRC=slatkin.i
OBJECTS=$(SOURCES:.c=.o)
SWIG=swig -python


all: $(EXECUTABLES) $(OBJECTS) python
	

python:
	$(SWIG) $(SWIG_SRC)
	python setup.py build_ext
	python setup.py build

install:
	python setup.py install


slatkin-enumerate: enumerate.o
	$(CC) -o slatkin-enumerate enumerate.o

slatkin-mc: $(OBJECTS)
	$(CC) -o slatkin-mc slatkin.o montecarlo.o mersenne.o

mc-loop-test: $(OBJECTS)
	$(CC) -o mc-loop-test mc_loop_test.o slatkin.o


.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLES) slatkin_wrap.c slatkin.py *.so *.pyc
	rm -rf build
