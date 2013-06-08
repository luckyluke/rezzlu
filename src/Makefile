CFLAGS+= -Wall -pedantic -g -pg

OBJ+= solver.o dict.o

all: test-dict test-solver

test-solver: $(OBJ) test-solver.o
	$(CC) $(CFLAGS) -o $@ $^

test-dict: dict.o test-dict.o
	$(CC) $(CFLAGS) -o $@ $^

solver.o: solver.h dict.h

dict.o: dict.h
test-dict.o: test.h

.PHONY: clean mtrace

clean:
	rm -f  test-dict test-solver *~ *.o

mtrace: test-solver
        export MALLOC_TRACE=$(pwd)/memory.log
