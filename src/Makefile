CFLAGS+= -Wall -pedantic -g -pg -fpic

OBJ+= solver.o dict.o game.o path.o

SWIG_MODULE=rezzlu_game

all: test-dict test-solver

test-solver: $(OBJ) test-solver.o
	$(CC) $(CFLAGS) -o $@ $^

test-dict: dict.o test-dict.o
	$(CC) $(CFLAGS) -o $@ $^

solver.o: solver.h dict.h
dict.o: dict.h
test-dict.o: test.h
game.o: game.h dict.h solver.h path.h

swig: $(OBJ) $(SWIG_MODULE).i
	swig -python $(SWIG_MODULE).i
	cc $(CFLAGS) $(shell python-config --includes) -c $(SWIG_MODULE)_wrap.c
	ld -shared $(OBJ) $(SWIG_MODULE)_wrap.o -o _$(SWIG_MODULE).so

.PHONY: clean mtrace

clean:
	rm -f  test-dict test-solver $(SWIG_MODULE).py $(SWIG_MODULE)_wrap.c _$(SWIG_MODULE).so *~ *.o *.pyc

mtrace: test-solver
        export MALLOC_TRACE=$(pwd)/memory.log
